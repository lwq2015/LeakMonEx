#if !defined(AFX_ONFIGDLG_H__C97A22C8_2FBF_4D76_BD8E_8E0F794A0314__INCLUDED_)
#define AFX_ONFIGDLG_H__C97A22C8_2FBF_4D76_BD8E_8E0F794A0314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// onfigDlg.h : header file
//
#include "EditEx.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// C_ConfigDlg dialog

extern bool g_IS_TEST_APP;

class C_ConfigDlg : public CDialog
{
	// Construction
public:
	C_ConfigDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CONFIG };
	CListCtrl	m_List;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	static UINT __cdecl ThreadEntry(LPVOID pParam);
	static BOOL CALLBACK SymRegisterCallbackProc64(HANDLE hProcess
		, ULONG ActionCode
		, ULONG64 CallbackData
		, ULONG64 UserContext);

	void LoadSymbols();
	void ShowWaitDialog();

	DECLARE_MESSAGE_MAP()


private:
	bool m_bChanged;
	CString m_csPath;
	EditEx m_EditCtrl;
	CDialog m_ProgressDlg;

protected:
	afx_msg void OnBnClickedAddPath();
	afx_msg void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnBnClickedDelPath();
	afx_msg void OnBnClickedAddSymbolServer();
	afx_msg void OnBnClickedRadioMem();
	afx_msg void OnBnClickedRadioGdi();
	afx_msg void OnBnClickedRadioHandle();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONFIGDLG_H__C97A22C8_2FBF_4D76_BD8E_8E0F794A0314__INCLUDED_)
