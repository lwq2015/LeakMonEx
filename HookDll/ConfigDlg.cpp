// onfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HookDll.h"
#include "ConfigDlg.h"
#include <Dbghelp.h>
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool g_IS_TEST_APP = false;

/////////////////////////////////////////////////////////////////////////////
// C_ConfigDlg dialog

C_ConfigDlg::C_ConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C_ConfigDlg::IDD, pParent)
	, m_EditCtrl(this)
	, m_ProgressDlg(IDD_PROGRESS)
	, m_bChanged(false)
{
}


void C_ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(C_ConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, C_ConfigDlg::OnOk)
	ON_BN_CLICKED(IDC_RDO_MEM, C_ConfigDlg::OnBnClickedRadioMem)
	ON_BN_CLICKED(IDC_RDO_GDI, C_ConfigDlg::OnBnClickedRadioGdi)
	ON_BN_CLICKED(IDC_RDO_HANDLE, C_ConfigDlg::OnBnClickedRadioHandle)
	ON_BN_CLICKED(IDC_BTN_ADD_SYM_SRV, C_ConfigDlg::OnBnClickedAddSymbolServer)
	ON_BN_CLICKED(IDC_BTN_ADD_FOLDER, C_ConfigDlg::OnBnClickedAddPath)
	ON_BN_CLICKED(IDC_BTN_DEL_PATH, C_ConfigDlg::OnBnClickedDelPath)
END_MESSAGE_MAP()

int CALLBACK BrowseCallbackProc(HWND hwnd
	, UINT uMsg
	, LPARAM lParam
	, LPARAM lpData)
{
	static bool bFirst = true;
	if (bFirst && uMsg == BFFM_INITIALIZED)
	{
		bFirst = false;
		TCHAR szExePath[MAX_PATH] = { 0 };

		GetCurrentDirectory(MAX_PATH, szExePath);
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szExePath);
	}
	return 0;
}

void C_ConfigDlg::OnBnClickedAddPath()
{
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = m_hWnd;
	bi.lpfn = BrowseCallbackProc;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	LPITEMIDLIST pIt = SHBrowseForFolder(&bi);

	if (pIt)
	{
		// Set the new path as the selected folder and refresh
		TCHAR szPath[MAX_PATH] = { 0 };
		if (SHGetPathFromIDList(pIt, szPath))
		{
			m_List.InsertItem(0, szPath);
			m_bChanged = true;
		}
	}
}

#if _DEBUG
#define TEST_APP_NAME "callfunctions.exe"
bool SetAppMode()
{
	CString csProcessName;
	::GetModuleFileName(0, csProcessName.GetBuffer(MAX_PATH), MAX_PATH);
	csProcessName.ReleaseBuffer();
	csProcessName = csProcessName.MakeLower();
	g_IS_TEST_APP = (-1 != csProcessName.Find(TEST_APP_NAME));

	return true;
}
#endif

BOOL C_ConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ListView_SetExtendedListViewStyleEx(m_List, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0, "PDB Path", LVCFMT_LEFT, 500);
	m_bChanged = false;

	//////////////////////////////////////////////////////////////////////////
	CString csExeName;
	GetModuleFileName(0, csExeName.GetBuffer(MAX_PATH), MAX_PATH);
	csExeName.ReleaseBuffer();
	csExeName = csExeName.MakeLower();
	csExeName.Replace(_T(".exe"), _T("Mem.ini"));
	if (PathFileExists(csExeName))
	{
		CString csLine;
		CStdioFile File;

		File.Open(csExeName, CFile::modeRead);
		while (File.ReadString(csLine))
		{
			csLine.Replace(_T("\r"), _T(""));
			m_List.InsertItem(0, csLine);
			m_csPath += csLine + _T(";");
		}
		File.Close();
	}
	else
	{
		CString csSystemPath;
		if (GetEnvironmentVariable("SYSTEMROOT", csSystemPath.GetBuffer(MAX_PATH), MAX_PATH) > 0)
		{
			csSystemPath.ReleaseBuffer();
			csSystemPath += _T("\\system32");
		}
		else
		{
			csSystemPath.ReleaseBuffer();
		}
		m_List.InsertItem(0, csSystemPath);
		csSystemPath += _T("\r\n");

		CString SymbolPath;
		if (GetEnvironmentVariable("_NT_SYMBOL_PATH", SymbolPath.GetBuffer(MAX_PATH), MAX_PATH) > 0)
		{
			SymbolPath.ReleaseBuffer();
			csSystemPath += SymbolPath + _T("\r\n");
			m_List.InsertItem(0, SymbolPath);
		}
		else
		{
			csSystemPath.ReleaseBuffer();
		}

		//add the hook dll path so that it can load the pdb of hookdll
		CString csDllPath;
		HMODULE hHookDll = GetModuleHandle(_T("HookDll.dll"));
		if (GetModuleFileName(hHookDll, csDllPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			csDllPath.ReleaseBuffer();
			int nPos = csDllPath.ReverseFind(_T('\\'));
			if (0 < nPos)
			{
				csDllPath = csDllPath.Left(nPos + 1);
				m_List.InsertItem(0, csDllPath);
			}
		}
	}

	m_EditCtrl.Create(WS_CHILD | WS_BORDER, CRect(0, 0, 0, 0), &m_List, 1);
	g_HookType = HT_MEMORY;
	((CButton*)GetDlgItem(IDC_RDO_MEM))->SetCheck(BST_CHECKED);
	SetDlgItemText(IDC_EDT_CALLSTACK_DEPTH, _T("50"));


#if _DEBUG
	SetAppMode();
	if (g_IS_TEST_APP)
	{
		LoadSymbols();
		PostMessage(WM_CLOSE, 0, 0);
	}
#endif
	return TRUE;
}

void C_ConfigDlg::OnCancel()
{
	CDialog::OnCancel();
}

typedef BOOL(WINAPI* SymRefreshModuleListDef)(HANDLE hProcess);
void C_ConfigDlg::OnOk()
{
	CDialog::OnOK();
	LoadSymbols();
}

void C_ConfigDlg::LoadSymbols()
{
	g_StackDepth = GetDlgItemInt(IDC_EDT_CALLSTACK_DEPTH, NULL, FALSE);
	if (m_bChanged)
	{
		CString csExeName;
		CStdioFile File;

		GetModuleFileName(0, csExeName.GetBuffer(MAX_PATH), MAX_PATH);
		csExeName.ReleaseBuffer();
		csExeName = csExeName.MakeLower();
		csExeName.Replace(_T(".exe"), _T("Mem.ini"));
		if (!File.Open(csExeName, CFile::modeCreate | CFile::modeWrite))
		{
			goto LoadDll;
		}

		int nCount = m_List.GetItemCount();
		m_csPath.Empty();
		for (int nId = 0; nId < nCount; nId++)
		{
			CString csItem = m_List.GetItemText(nId, 0);
			m_csPath += csItem + _T(";");
			csItem += _T("\r\n");
			File.WriteString(csItem);
		}
		File.Close();
	}

LoadDll:

	HMODULE hModule = GetModuleHandle(_T("dbghelp.dll"));
	SymRefreshModuleListDef pSymRefreshModuleList;

	if (hModule)
	{
		pSymRefreshModuleList = (SymRefreshModuleListDef)GetProcAddress(hModule, _T("SymRefreshModuleList"));
		CString csLoadedDll;
		GetModuleFileName(hModule, csLoadedDll.GetBuffer(MAX_PATH), MAX_PATH);
		csLoadedDll.ReleaseBuffer();
		if (!pSymRefreshModuleList)
		{
			// old version of dbghelp :(
			MessageBox("Your application has already loaded dbghelp.dll from " + csLoadedDll +
				"\n\nFor acqurate results, replace this dll with the latest version of dbghelp.dll"
				"coming with \"Debugging tools for windows\" or with the dll the application folder of this utility",
				"Error", MB_OK);
		}
		else
		{
			MessageBox("Your application has already loaded dbghelp.dll from " + csLoadedDll +
				" Please confirm that the symsrv.dll exists in th same folder.\n"
				"Otherwise symbol server will not work",
				"Warning", MB_OK);
		}

	}
	else
	{
		// 			static int nTempvariable;
		// 			MEMORY_BASIC_INFORMATION MemInfo;
		// 			CString csDllPath;
		// 			if( !VirtualQuery( &nTempvariable, &MemInfo, sizeof(MemInfo)))
		// 			{
		// 				goto LoadDll;
		// 			}
		CString csDllPath;
		HMODULE hHookDll = GetModuleHandle(_T("HookDll.dll"));
		if (!GetModuleFileName(hHookDll, csDllPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			goto LoadDll;
		}
		csDllPath.ReleaseBuffer();

		int nPos = csDllPath.ReverseFind(_T('\\'));
		if (0 >= nPos)
		{
			goto LoadDll;
		}
		csDllPath = csDllPath.Left(nPos + 1);
		//csDllPath = "C:\\Program Files\\Debugging Tools for Windows (x86)\\";
		csDllPath += _T("dbghelp.dll");

		hModule = LoadLibrary(csDllPath);
		if (!hModule)
		{
			hModule = LoadLibrary(_T("dbghelp.dll"));
			pSymRefreshModuleList = (SymRefreshModuleListDef)GetProcAddress(hModule, _T("SymRefreshModuleList"));
			if (!pSymRefreshModuleList)
			{
				MessageBox("Failed to load the dbghelp.dll from the local directory\n\n"
					"The application will continue with the default dbghelp.dll. But some feature may"
					"be unavailable", "Error", MB_OK);
			}
		}
		else
		{
			pSymRefreshModuleList = (SymRefreshModuleListDef)GetProcAddress(hModule, _T("SymRefreshModuleList"));
		}

	}

	SymCleanup(GetCurrentProcess());
	CString csWholePath = m_csPath;
	csWholePath.TrimRight(';');
	DWORD dwOption = SymGetOptions();
	dwOption |= SYMOPT_CASE_INSENSITIVE | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS |
		SYMOPT_LOAD_ANYTHING | SYMOPT_UNDNAME;
	SymSetOptions(dwOption);
	CWinThread* pThread = AfxBeginThread(ThreadEntry, this);
	HANDLE hThread = pThread->m_hThread;
	BOOL fInvadeProcess = (0 == pSymRefreshModuleList) ? TRUE : FALSE;
	BOOL bRet = SymInitialize(GetCurrentProcess(), (LPTSTR)csWholePath.operator LPCTSTR(), fInvadeProcess);
	SymRegisterCallback64(GetCurrentProcess(), SymRegisterCallbackProc64, (ULONG64)this);
	while (!m_ProgressDlg.m_hWnd)// wait untill the dialog is created
	{
		Sleep(50);
	}

	if (pSymRefreshModuleList)
	{
		pSymRefreshModuleList(GetCurrentProcess());
	}
	//SymRefreshModuleList( GetCurrentProcess());
	m_ProgressDlg.SendMessage(WM_CLOSE);
	WaitForSingleObject(hThread, 10000);
}

BOOL CALLBACK C_ConfigDlg::SymRegisterCallbackProc64(HANDLE hProcess
	, ULONG ActionCode
	, ULONG64 CallbackData
	, ULONG64 UserContext
)
{
	if (CBA_DEFERRED_SYMBOL_LOAD_START == ActionCode)
	{
		PIMAGEHLP_DEFERRED_SYMBOL_LOAD64 pSybolLoadInfo = (PIMAGEHLP_DEFERRED_SYMBOL_LOAD64)CallbackData;
		C_ConfigDlg* pDlg = (C_ConfigDlg*)UserContext;
		CString csLoadtext = _T("Loading symbol for file: ");
		csLoadtext += pSybolLoadInfo->FileName;
		if (pDlg->m_ProgressDlg)
			pDlg->m_ProgressDlg.SetDlgItemText(IDC_LOAD_INFO, csLoadtext);
	}
	return FALSE;
}

UINT __cdecl C_ConfigDlg::ThreadEntry(LPVOID pParam)
{
	C_ConfigDlg* pDlg = (C_ConfigDlg*)pParam;
	pDlg->ShowWaitDialog();
	return 0;
}

void C_ConfigDlg::ShowWaitDialog()
{
	m_ProgressDlg.DoModal();
}


void C_ConfigDlg::OnBnClickedDelPath()
{
	// Remove path;
	int nSelected = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (-1 == nSelected)
	{
		return;
	}
	m_List.DeleteItem(nSelected);
	m_bChanged = true;
}

void C_ConfigDlg::OnBnClickedAddSymbolServer()
{
	if (m_EditCtrl.IsWindowVisible())
	{
		return;
	}

	int nitemCount = m_List.GetItemCount();

	m_List.InsertItem(nitemCount, _T(""));
	m_List.EnsureVisible(nitemCount, FALSE);
	CRect ItemRect(0, 0, 0, 0);
	m_List.GetItemRect(nitemCount, ItemRect, LVIR_BOUNDS);
	m_EditCtrl.MoveWindow(ItemRect);
	m_EditCtrl.SetWindowText(_T("SRV*c:\\Windows\\Symbols*http://msdl.microsoft.com/download/symbols"));
	m_EditCtrl.ShowWindow(SW_SHOW);
	m_EditCtrl.SetFocus();
}


LRESULT C_ConfigDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_EDIT_MSG && m_EditCtrl.IsWindowVisible())
	{
		m_EditCtrl.ShowWindow(SW_HIDE);
		int nItemCount = m_List.GetItemCount();
		CString csText;
		m_EditCtrl.GetWindowText(csText);
		if (csText.IsEmpty())
		{
			m_List.DeleteItem(nItemCount - 1);
		}
		else
		{
			m_List.SetItemText(nItemCount - 1, 0, csText);
			m_bChanged = true;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void C_ConfigDlg::OnBnClickedRadioMem()
{
	g_HookType = HT_MEMORY;
}

void C_ConfigDlg::OnBnClickedRadioGdi()
{
	g_HookType = HT_GDI;
}

void C_ConfigDlg::OnBnClickedRadioHandle()
{
	g_HookType = HT_HANDLE;
}
