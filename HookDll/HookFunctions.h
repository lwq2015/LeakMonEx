#ifndef __HOOK_FUNCTIONS_H__
#define __HOOK_FUNCTIONS_H__

#if _WIN32_WINNT < 0x0600 // to compile under vs6

typedef struct _ICONINFOEXA
{
	DWORD   cbSize;
	BOOL    fIcon;
	DWORD   xHotspot;
	DWORD   yHotspot;
	HBITMAP hbmMask;
	HBITMAP hbmColor;
	WORD    wResID;
	CHAR    szModName[MAX_PATH];
	CHAR    szResName[MAX_PATH];
} ICONINFOEXA, * PICONINFOEXA;
typedef struct _ICONINFOEXW
{
	DWORD   cbSize;
	BOOL    fIcon;
	DWORD   xHotspot;
	DWORD   yHotspot;
	HBITMAP hbmMask;
	HBITMAP hbmColor;
	WORD    wResID;
	WCHAR   szModName[MAX_PATH];
	WCHAR   szResName[MAX_PATH];
} ICONINFOEXW, * PICONINFOEXW;


typedef enum _FILE_ID_TYPE
{
	FileIdType,
	ObjectIdType,
	MaximumFileIdType
} FILE_ID_TYPE, * PFILE_ID_TYPE;

typedef struct FILE_ID_DESCRIPTOR
{
	DWORD dwSize;  // Size of the struct
	FILE_ID_TYPE Type; // Describes the type of identifier passed in.
	union
	{
		LARGE_INTEGER FileId;
		GUID ObjectId;
	};
} FILE_ID_DESCRIPTOR, * LPFILE_ID_DESCRIPTOR;

typedef LONG LSTATUS;

#endif

// memory allocations

typedef enum _SECTION_INHERIT
{
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT;

typedef LPVOID(WINAPI* HeapAllocDef)(IN HANDLE hHeap, IN DWORD dwFlags, IN SIZE_T dwBytes);
typedef BOOL(WINAPI* HeapFreeDef)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
typedef LPVOID(WINAPI* HeapReAllocDef)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);

typedef LPVOID(WINAPI* VirtualAllocDef)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef BOOL(WINAPI* VirtualFreeDef)(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
typedef LPVOID(WINAPI* VirtualAllocExDef)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef BOOL(WINAPI* VirtualFreeExDef)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);

typedef LPVOID(WINAPI* CoTaskMemAllocDef)(SIZE_T cb);
typedef LPVOID(WINAPI* CoTaskMemReallocDef)(LPVOID pv, SIZE_T cb);
typedef void   (WINAPI* CoTaskMemFreeDef)(LPVOID pv);
typedef LPVOID(WINAPI* MapViewOfFileDef)(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap);
typedef LPVOID(WINAPI* MapViewOfFileExDef)(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress);
typedef BOOL(WINAPI* UnmapViewOfFileDef)(LPCVOID lpBaseAddress);
typedef INT(*NtMapViewOfSectionDef)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
typedef INT(*NtUnmapViewOfSectionDef)(HANDLE ProcessHandle, PVOID BaseAddress);

static HeapAllocDef             s_pfnOrgHeapAlloc = 0;
static HeapReAllocDef           s_pfnOrgHeapReAlloc = 0;
static HeapFreeDef	            s_pfnOrgHeapFree = 0;
static VirtualAllocDef          s_pfnOrgVirtualAlloc = 0;
static VirtualFreeDef           s_pfnOrgVirtualFree = 0;
static VirtualAllocExDef        s_pfnOrgVirtualAllocEx = 0;
static VirtualFreeExDef         s_pfnOrgVirtualFreeEx = 0;
static CoTaskMemAllocDef	    s_pfnOrgCoTaskMemAlloc = 0;
static CoTaskMemReallocDef	    s_pfnOrgCoTaskMemRealloc = 0;
static CoTaskMemFreeDef		    s_pfnOrgCoTaskMemFree = 0;
static MapViewOfFileDef		    s_pfnOrgMapViewOfFile = 0;
static MapViewOfFileExDef	    s_pfnOrgMapViewOfFileEx = 0;
static UnmapViewOfFileDef	    s_pfnOrgUnmapViewOfFile = 0;
static NtMapViewOfSectionDef	s_pfnOrgNtMapViewOfSection = 0;
static NtUnmapViewOfSectionDef	s_pfnOrgNtUnmapViewOfSection = 0;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//GDI allocation
//bitmap - LoadBitmapA, LoadBitmapW, CreateBitmap, CreateBitmapIndirect, CreateCompatibleBitmap ,CreateDIBitmap, CreateDIBSection,LoadImage 


typedef HBITMAP(WINAPI* LoadBitmapADef)(HINSTANCE hInstance, LPCSTR lpBitmapName);
typedef HBITMAP(WINAPI* LoadBitmapWDef)(HINSTANCE hInstance, LPCWSTR lpBitmapName);
typedef HANDLE(WINAPI* LoadImageADef)(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad);
typedef HANDLE(WINAPI* LoadImageWDef)(HINSTANCE hInst, LPCWSTR name, UINT type, int cx, int cy, UINT fuLoad);
typedef HBITMAP(WINAPI* CreateBitmapDef)(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, CONST VOID* lpBits);
typedef HBITMAP(WINAPI* CreateBitmapIndirectDef)(CONST BITMAP* pbm);
typedef HBITMAP(WINAPI* CreateCompatibleBitmapDef)(HDC hdc, int cx, int cy);
typedef HBITMAP(WINAPI* CreateDIBitmapDef)(HDC hdc, CONST BITMAPINFOHEADER* pbmih, DWORD flInit, CONST VOID* pjBits, CONST BITMAPINFO* pbmi, UINT iUsage);
typedef HBITMAP(WINAPI* CreateDIBSectionDef)(HDC hdc, CONST BITMAPINFO* lpbmi, UINT usage, VOID** ppvBits, HANDLE hSection, DWORD offset);
typedef HBITMAP(WINAPI* CreateDiscardableBitmapDef)(HDC hdc, int cx, int cy);
typedef HANDLE(WINAPI* CopyImageDef)(HANDLE h, UINT type, int cx, int cy, UINT flags);
typedef BOOL(WINAPI* GetIconInfoDef)(HICON hIcon, PICONINFO piconinfo);
typedef BOOL(WINAPI* GetIconInfoExADef)(HICON hicon, PICONINFOEXA piconinfo);
typedef BOOL(WINAPI* GetIconInfoExWDef)(HICON hicon, PICONINFOEXW piconinfo);
typedef BOOL(WINAPI* DeleteObjectDef)(HGDIOBJ ho);

static LoadBitmapADef               s_pfnOrgLoadBitmapA;
static LoadBitmapWDef               s_pfnOrgLoadBitmapW;
static LoadImageADef                s_pfnOrgLoadImageA;
static LoadImageWDef                s_pfnOrgLoadImageW;
static CreateBitmapDef              s_pfnOrgCreateBitmap;
static CreateBitmapIndirectDef      s_pfnOrgCreateBitmapIndirect;
static CreateCompatibleBitmapDef    s_pfnOrgCreateCompatibleBitmap;
static CreateDIBitmapDef            s_pfnOrgCreateDIBitmap;
static CreateDIBSectionDef          s_pfnOrgCreateDIBSection;
static CreateDiscardableBitmapDef   s_pfnOrgCreateDiscardableBitmap;
static CopyImageDef                 s_pfnOrgCopyImage;
static GetIconInfoDef	            s_pfnOrgGetIconInfo;
static GetIconInfoExADef            s_pfnOrgGetIconInfoExA;
static GetIconInfoExWDef            s_pfnOrgGetIconInfoExW;
static DeleteObjectDef              s_pfnOrgDeleteObject;

//////////////////////////////////////////////////////////////////////////
//icons
typedef HICON(WINAPI* CopyIconDef)(HICON hIcon);
typedef HICON(WINAPI* CreateIconDef)(HINSTANCE hInstance, int nWidth, int nHeight, BYTE cPlanes, BYTE cBitsPixel, CONST BYTE* lpbANDbits, CONST BYTE* lpbXORbits);
typedef HICON(WINAPI* CreateIconFromResourceDef)(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer);
typedef HICON(WINAPI* CreateIconFromResourceExDef)(PBYTE presbits, DWORD dwResSize, BOOL fIcon, DWORD dwVer, int cxDesired, int cyDesired, UINT Flags);
typedef HICON(WINAPI* CreateIconIndirectDef)(PICONINFO piconinfo);
typedef BOOL(WINAPI* DestroyIconDef)(HICON hIcon);
typedef HICON(WINAPI* DuplicateIconDef)(HINSTANCE hInst, HICON hIcon);
typedef HICON(WINAPI* ExtractAssociatedIconADef)(HINSTANCE hInst, LPSTR lpIconPath, LPWORD lpiIcon);
typedef HICON(WINAPI* ExtractAssociatedIconWDef)(HINSTANCE hInst, LPWSTR lpIconPath, LPWORD lpiIcon);
typedef HICON(WINAPI* ExtractAssociatedIconExADef)(HINSTANCE hInst, LPSTR lpIconPath, LPWORD lpiIconIndex, LPWORD lpiIconId);
typedef HICON(WINAPI* ExtractAssociatedIconExWDef)(HINSTANCE hInst, LPWSTR lpIconPath, LPWORD lpiIconIndex, LPWORD lpiIconId);
typedef HICON(WINAPI* ExtractIconADef)(HINSTANCE hInst, LPCSTR lpszExeFileName, UINT nIconIndex);
typedef HICON(WINAPI* ExtractIconWDef)(HINSTANCE hInst, LPCWSTR lpszExeFileName, UINT nIconIndex);
typedef UINT(WINAPI* ExtractIconExADef)(LPCSTR lpszFile, int nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIcons);
typedef UINT(WINAPI* ExtractIconExWDef)(LPCWSTR lpszFile, int nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIcons);
typedef HICON(WINAPI* LoadIconADef)(HINSTANCE hInstance, LPCSTR lpIconName);
typedef HICON(WINAPI* LoadIconWDef)(HINSTANCE hInstance, LPCWSTR lpIconName);
typedef UINT(WINAPI* PrivateExtractIconsADef)(LPCSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON* phicon, UINT* piconid, UINT nIcons, UINT flags);
typedef UINT(WINAPI* PrivateExtractIconsWDef)(LPCWSTR szFileName, int nIconIndex, int cxIcon, int cyIcon, HICON* phicon, UINT* piconid, UINT nIcons, UINT flags);

static CopyIconDef					s_pfnOrgCopyIcon;
static CreateIconDef				s_pfnOrgCreateIcon;
static CreateIconFromResourceDef	s_pfnOrgCreateIconFromResource;
static CreateIconFromResourceExDef	s_pfnOrgCreateIconFromResourceEx;
static CreateIconIndirectDef		s_pfnOrgCreateIconIndirect;
static DestroyIconDef				s_pfnOrgDestroyIcon;
static DuplicateIconDef				s_pfnOrgDuplicateIcon;
static ExtractAssociatedIconADef	s_pfnOrgExtractAssociatedIconA;
static ExtractAssociatedIconWDef	s_pfnOrgExtractAssociatedIconW;
static ExtractAssociatedIconExADef	s_pfnOrgExtractAssociatedIconExA;
static ExtractAssociatedIconExWDef	s_pfnOrgExtractAssociatedIconExW;
static ExtractIconADef				s_pfnOrgExtractIconA;
static ExtractIconWDef				s_pfnOrgExtractIconW;
static ExtractIconExADef			s_pfnOrgExtractIconExA;
static ExtractIconExWDef			s_pfnOrgExtractIconExW;
static LoadIconADef					s_pfnOrgLoadIconA;
static LoadIconWDef					s_pfnOrgLoadIconW;
static PrivateExtractIconsADef		s_pfnOrgPrivateExtractIconsA;
static PrivateExtractIconsWDef		s_pfnOrgPrivateExtractIconsW;

//////////////////////////////////////////////////////////////////////////
//cursor
typedef HCURSOR(WINAPI* CreateCursorDef)(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, CONST VOID* pvANDPlane, CONST VOID* pvXORPlane);
typedef BOOL(WINAPI* DestroyCursorDef)(HCURSOR hCursor);
typedef HCURSOR(WINAPI* LoadCursorADef)(HINSTANCE hInstance, LPCSTR lpCursorName);
typedef HCURSOR(WINAPI* LoadCursorWDef)(HINSTANCE hInstance, LPCWSTR lpCursorName);
typedef HCURSOR(WINAPI* LoadCursorFromFileADef)(LPCSTR lpFileName);
typedef HCURSOR(WINAPI* LoadCursorFromFileWDef)(LPCWSTR lpFileName);
static CreateCursorDef			s_pfnOrgCreateCursor;
static LoadCursorADef			s_pfnOrgLoadCursorA;
static LoadCursorWDef			s_pfnOrgLoadCursorW;
static LoadCursorFromFileADef	s_pfnOrgLoadCursorFromFileA;
static LoadCursorFromFileWDef	s_pfnOrgLoadCursorFromFileW;
static DestroyCursorDef			s_pfnOrgDestroyCursor;


//brush 
typedef HBRUSH(WINAPI* CreateBrushIndirectDef)(CONST LOGBRUSH* plbrush);
typedef HBRUSH(WINAPI* CreateSolidBrushDef)(COLORREF color);
typedef HBRUSH(WINAPI* CreatePatternBrushDef)(HBITMAP hbm);
typedef HBRUSH(WINAPI* CreateDIBPatternBrushDef)(HGLOBAL h, UINT iUsage);
typedef HBRUSH(WINAPI* CreateDIBPatternBrushPtDef)(CONST VOID* lpPackedDIB, UINT iUsage);
typedef HBRUSH(WINAPI* CreateHatchBrushDef)(int iHatch, COLORREF color);
static CreateBrushIndirectDef       s_pfnOrgCreateBrushIndirect;
static CreateSolidBrushDef          s_pfnOrgCreateSolidBrush;
static CreatePatternBrushDef        s_pfnOrgCreatePatternBrush;
static CreateDIBPatternBrushDef     s_pfnOrgCreateDIBPatternBrush;
static CreateDIBPatternBrushPtDef   s_pfnOrgCreateDIBPatternBrushPt;
static CreateHatchBrushDef          s_pfnOrgCreateHatchBrush;


//device context 
typedef HDC(WINAPI* CreateCompatibleDCDef)(HDC hdc);
typedef HDC(WINAPI* CreateDCADef)(LPCSTR pwszDriver, LPCSTR pwszDevice, LPCSTR pszPort, CONST DEVMODEA* pdm);
typedef HDC(WINAPI* CreateDCWDef)(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pszPort, CONST DEVMODEW* pdm);
typedef HDC(WINAPI* CreateICADef)(LPCSTR pszDriver, LPCSTR pszDevice, LPCSTR pszPort, CONST DEVMODEA* pdm);
typedef HDC(WINAPI* CreateICWDef)(LPCWSTR pszDriver, LPCWSTR pszDevice, LPCWSTR pszPort, CONST DEVMODEW* pdm);
typedef HDC(WINAPI* GetDCDef)(HWND hWnd);
typedef HDC(WINAPI* GetDCExDef)(HWND hWnd, HRGN hrgnClip, DWORD flags);
typedef HDC(WINAPI* GetWindowDCDef)(HWND hWnd);
typedef int (WINAPI* ReleaseDCDef)(HWND hWnd, HDC hDC);
typedef BOOL(WINAPI* DeleteDCDef)(HDC hdc);

static CreateCompatibleDCDef s_pfnOrgCreateCompatibleDC;
static CreateDCADef          s_pfnOrgCreateDCA;
static CreateDCWDef          s_pfnOrgCreateDCW;
static CreateICADef          s_pfnOrgCreateICA;
static CreateICWDef          s_pfnOrgCreateICW;
static GetDCDef              s_pfnOrgGetDC;
static GetDCExDef            s_pfnOrgGetDCEx;
static GetWindowDCDef        s_pfnOrgGetWindowDC;
static ReleaseDCDef          s_pfnOrgReleaseDC;
static DeleteDCDef           s_pfnOrgDeleteDC;

//font 
typedef HFONT(WINAPI* CreateFontADef)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision,
	DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);

typedef HFONT(WINAPI* CreateFontWDef)(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision,
	DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName);
typedef HFONT(WINAPI* CreateFontIndirectADef)(CONST LOGFONTA* lplf);
typedef HFONT(WINAPI* CreateFontIndirectWDef)(CONST LOGFONTW* lplf);
static CreateFontADef         s_pfnOrgCreateFontA;
static CreateFontWDef         s_pfnOrgCreateFontW;
static CreateFontIndirectADef s_pfnOrgCreateFontIndirectA;
static CreateFontIndirectWDef s_pfnOrgCreateFontIndirectW;

//metafile 
typedef HDC(WINAPI* CreateMetaFileADef)(LPCSTR pszFile);
typedef HDC(WINAPI* CreateMetaFileWDef)(LPCWSTR pszFile);
typedef HDC(WINAPI* CreateEnhMetaFileADef)(HDC hdc, LPCSTR lpFilename, CONST RECT* lprc, LPCSTR lpDesc);
typedef HDC(WINAPI* CreateEnhMetaFileWDef)(HDC hdc, LPCWSTR lpFilename, CONST RECT* lprc, LPCWSTR lpDesc);
typedef HENHMETAFILE(WINAPI* GetEnhMetaFileADef)(LPCSTR lpName);
typedef HENHMETAFILE(WINAPI* GetEnhMetaFileWDef)(LPCWSTR lpName);
typedef HMETAFILE(WINAPI* GetMetaFileADef)(LPCSTR lpName);
typedef HMETAFILE(WINAPI* GetMetaFileWDef)(LPCWSTR lpName);
typedef BOOL(WINAPI* DeleteMetaFileDef)(HMETAFILE hmf);
typedef BOOL(WINAPI* DeleteEnhMetaFileDef)(HENHMETAFILE hmf);
typedef HENHMETAFILE(WINAPI* CopyEnhMetaFileADef)(HENHMETAFILE hEnh, LPCSTR lpFileName);
typedef HENHMETAFILE(WINAPI* CopyEnhMetaFileWDef)(HENHMETAFILE hEnh, LPCWSTR lpFileName);
typedef HENHMETAFILE(WINAPI* CloseEnhMetaFileDef)(HDC hdc);
typedef HMETAFILE(WINAPI* CloseMetaFileDef)(HDC hdc);


static CreateMetaFileADef		s_pfnOrgCreateMetaFileA;
static CreateMetaFileWDef		s_pfnOrgCreateMetaFileW;
static CreateEnhMetaFileADef	s_pfnOrgCreateEnhMetaFileA;
static CreateEnhMetaFileWDef	s_pfnOrgCreateEnhMetaFileW;
static GetEnhMetaFileADef		s_pfnOrgGetEnhMetaFileA;
static GetEnhMetaFileWDef		s_pfnOrgGetEnhMetaFileW;
static GetMetaFileADef			s_pfnOrgGetMetaFileA;
static GetMetaFileWDef			s_pfnOrgGetMetaFileW;
static DeleteMetaFileDef		s_pfnOrgDeleteMetaFile;
static DeleteEnhMetaFileDef		s_pfnOrgDeleteEnhMetaFile;
static CopyEnhMetaFileADef		s_pfnOrgCopyEnhMetaFileA;
static CopyEnhMetaFileWDef		s_pfnOrgCopyEnhMetaFileW;
static CloseEnhMetaFileDef		s_pfnOrgCloseEnhMetaFile;
static CloseMetaFileDef			s_pfnOrgCloseMetaFile;

//pen
typedef HPEN(WINAPI* CreatePenDef)(int iStyle, int cWidth, COLORREF color);
typedef HPEN(WINAPI* CreatePenIndirectDef)(CONST LOGPEN* plpen);
typedef HPEN(WINAPI* ExtCreatePenDef)(DWORD iPenStyle, DWORD cWidth, CONST LOGBRUSH* plbrush, DWORD cStyle, CONST DWORD* pstyle);
static CreatePenDef         s_pfnOrgCreatePen;
static CreatePenIndirectDef s_pfnOrgCreatePenIndirect;
static ExtCreatePenDef      s_pfnOrgExtCreatePen;

//region 
typedef HRGN(WINAPI* PathToRegionDef)(HDC hdc);
typedef HRGN(WINAPI* CreateEllipticRgnDef)(int x1, int y1, int x2, int y2);
typedef HRGN(WINAPI* CreateEllipticRgnIndirectDef)(CONST RECT* lprect);
typedef HRGN(WINAPI* CreatePolygonRgnDef)(CONST POINT* pptl, int cPoint, int iMode);
typedef HRGN(WINAPI* CreatePolyPolygonRgnDef)(CONST POINT* pptl, CONST INT* pc, int cPoly, int iMode);
typedef HRGN(WINAPI* CreateRectRgnDef)(int x1, int y1, int x2, int y2);
typedef HRGN(WINAPI* CreateRectRgnIndirectDef)(CONST RECT* lprect);
typedef HRGN(WINAPI* CreateRoundRectRgnDef)(int x1, int y1, int x2, int y2, int w, int h);
typedef HRGN(WINAPI* ExtCreateRegionDef)(CONST XFORM* lpx, DWORD nCount, CONST RGNDATA* lpData);
static PathToRegionDef              s_pfnOrgPathToRegion;
static CreateEllipticRgnDef         s_pfnOrgCreateEllipticRgn;
static CreateEllipticRgnIndirectDef s_pfnOrgCreateEllipticRgnIndirect;
static CreatePolygonRgnDef          s_pfnOrgCreatePolygonRgn;
static CreatePolyPolygonRgnDef      s_pfnOrgCreatePolyPolygonRgn;
static CreateRectRgnDef             s_pfnOrgCreateRectRgn;
static CreateRectRgnIndirectDef     s_pfnOrgCreateRectRgnIndirect;
static CreateRoundRectRgnDef        s_pfnOrgCreateRoundRectRgn;
static ExtCreateRegionDef           s_pfnOrgExtCreateRegion;

//palette 
typedef HPALETTE(WINAPI* CreateHalftonePaletteDef)(HDC hdc);
typedef HPALETTE(WINAPI* CreatePaletteDef)(CONST LOGPALETTE* plpal);
static CreateHalftonePaletteDef s_pfnOrgCreateHalftonePalette;
static CreatePaletteDef         s_pfnOrgCreatePalette;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Handle creation function

// sync function
typedef HANDLE(WINAPI* CreateEventADef)(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
typedef HANDLE(WINAPI* CreateEventWDef)(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName);
typedef HANDLE(WINAPI* CreateEventExADef)(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* CreateEventExWDef)(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* OpenEventADef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
typedef HANDLE(WINAPI* OpenEventWDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpName);

typedef HANDLE(WINAPI* CreateMutexADef)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
typedef HANDLE(WINAPI* CreateMutexWDef)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName);
typedef HANDLE(WINAPI* CreateMutexExADef)(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* CreateMutexExWDef)(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* OpenMutexADef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
typedef HANDLE(WINAPI* OpenMutexWDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpName);

typedef HANDLE(WINAPI* CreateSemaphoreADef)(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName);
typedef HANDLE(WINAPI* CreateSemaphoreWDef)(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName);
typedef HANDLE(WINAPI* CreateSemaphoreExADef)(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* CreateSemaphoreExWDef)(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* OpenSemaphoreADef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
typedef HANDLE(WINAPI* OpenSemaphoreWDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPWSTR lpName);

typedef HANDLE(WINAPI* CreateWaitableTimerADef)(LPSECURITY_ATTRIBUTES lpTimerAttributes, BOOL bManualReset, LPCSTR lpTimerName);
typedef HANDLE(WINAPI* CreateWaitableTimerWDef)(LPSECURITY_ATTRIBUTES lpTimerAttributes, BOOL bManualReset, LPCWSTR lpTimerName);
typedef HANDLE(WINAPI* CreateWaitableTimerExADef)(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCSTR lpTimerName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* CreateWaitableTimerExWDef)(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCWSTR lpTimerName, DWORD dwFlags, DWORD dwDesiredAccess);
typedef HANDLE(WINAPI* OpenWaitableTimerADef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpTimerName);
typedef HANDLE(WINAPI* OpenWaitableTimerWDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpTimerName);

// file function
typedef HANDLE(WINAPI* CreateFileADef)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE(WINAPI* CreateFileWDef)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE(WINAPI* CreateFileTransactedADef)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID  lpExtendedParameter);
typedef HANDLE(WINAPI* CreateFileTransactedWDef)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID  lpExtendedParameter);
typedef HANDLE(WINAPI* FindFirstFileADef)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
typedef HANDLE(WINAPI* FindFirstFileWDef)(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
typedef HANDLE(WINAPI* FindFirstFileExADef)(LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);
typedef HANDLE(WINAPI* FindFirstFileExWDef)(LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);
typedef HANDLE(WINAPI* FindFirstFileNameTransactedWDef)(LPCWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName, HANDLE hTransaction);
typedef HANDLE(WINAPI* FindFirstFileNameWDef)(LPCWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName);
typedef HANDLE(WINAPI* FindFirstFileTransactedADef)(LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags, HANDLE hTransaction);
typedef HANDLE(WINAPI* FindFirstFileTransactedWDef)(LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags, HANDLE hTransaction);
typedef HANDLE(WINAPI* FindFirstStreamTransactedWDef)(LPCWSTR lpFileName, STREAM_INFO_LEVELS InfoLevel, LPVOID lpFindStreamData, DWORD dwFlags, HANDLE hTransaction);
typedef HANDLE(WINAPI* FindFirstStreamWDef)(LPCWSTR lpFileName, STREAM_INFO_LEVELS InfoLevel, LPVOID lpFindStreamData, DWORD dwFlags);
typedef BOOL(WINAPI* FindCloseDef)(HANDLE hFindFile);
typedef HANDLE(WINAPI* OpenFileByIdDef)(HANDLE hFile, LPFILE_ID_DESCRIPTOR lpFileID, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwFlags);
typedef HANDLE(WINAPI* ReOpenFileDef)(HANDLE hOriginalFile, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlags);
typedef HANDLE(WINAPI* CreateIoCompletionPortDef)(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads);

//Authorization function
typedef BOOL(WINAPI* CreateRestrictedTokenDef)(HANDLE ExistingTokenHandle, DWORD Flags, DWORD DisableSidCount, PSID_AND_ATTRIBUTES SidsToDisable, DWORD DeletePrivilegeCount, PLUID_AND_ATTRIBUTES PrivilegesToDelete, DWORD RestrictedSidCount, PSID_AND_ATTRIBUTES SidsToRestrict, PHANDLE NewTokenHandle);
typedef BOOL(WINAPI* DuplicateTokenDef)(HANDLE ExistingTokenHandle, SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, PHANDLE DuplicateTokenHandle);
typedef BOOL(WINAPI* DuplicateTokenExDef)(HANDLE hExistingToken, DWORD dwDesiredAccess, LPSECURITY_ATTRIBUTES lpTokenAttributes, SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, TOKEN_TYPE TokenType, PHANDLE phNewToken);
typedef BOOL(WINAPI* OpenProcessTokenDef)(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
typedef BOOL(WINAPI* OpenThreadTokenDef)(HANDLE ThreadHandle, DWORD DesiredAccess, BOOL OpenAsSelf, PHANDLE TokenHandle);

//Directory management
typedef HANDLE(WINAPI* FindFirstChangeNotificationADef)(LPCSTR lpPathName, BOOL bWatchSubtree, DWORD dwNotifyFilter);
typedef HANDLE(WINAPI* FindFirstChangeNotificationWDef)(LPCWSTR lpPathName, BOOL bWatchSubtree, DWORD dwNotifyFilter);
typedef BOOL(WINAPI* FindCloseChangeNotificationDef)(HANDLE hChangeHandle);

// File mapping
typedef HANDLE(WINAPI* CreateMemoryResourceNotificationDef)(MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType);
typedef HANDLE(WINAPI* CreateFileMappingADef)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);
typedef HANDLE(WINAPI* CreateFileMappingWDef)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName);
typedef HANDLE(WINAPI* CreateFileMappingNumaADef)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName, DWORD nndPreferred);
typedef HANDLE(WINAPI* CreateFileMappingNumaWDef)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName, DWORD nndPreferred);
typedef HANDLE(WINAPI* OpenFileMappingADef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
typedef HANDLE(WINAPI* OpenFileMappingWDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpName);

//Memory
typedef HANDLE(WINAPI* HeapCreateDef)(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
typedef BOOL(WINAPI* HeapDestroyDef)(HANDLE hHeap);
typedef HANDLE(WINAPI* GlobalAllocDef)(UINT uFlags, SIZE_T dwBytes);
typedef HANDLE(WINAPI* GlobalReAllocDef)(HGLOBAL hMem, SIZE_T dwBytes, UINT uFlags);
typedef HANDLE(WINAPI* GlobalFreeDef)(HGLOBAL hMem);
typedef HLOCAL(WINAPI* LocalAllocDef)(UINT uFlags, SIZE_T uBytes);
typedef HLOCAL(WINAPI* LocalReAllocDef)(HLOCAL hMem, SIZE_T uBytes, UINT uFlags);
typedef HLOCAL(WINAPI* LocalFreeDef)(HLOCAL hMem);

//Process and thread
typedef BOOL(WINAPI* CreateProcessADef)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI* CreateProcessWDef)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI* CreateProcessAsUserADef)(HANDLE hToken, LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI* CreateProcessAsUserWDef)(HANDLE hToken, LPWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI* CreateProcessWithLogonWDef)(LPCWSTR lpUsername, LPCWSTR lpDomain, LPCWSTR lpPassword, DWORD dwLogonFlags, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI* CreateProcessWithTokenWDef)(HANDLE hToken, DWORD dwLogonFlags, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef HANDLE(WINAPI* OpenProcessDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
typedef HANDLE(WINAPI* CreateThreadDef)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
typedef HANDLE(WINAPI* CreateRemoteThreadDef)(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
typedef HANDLE(WINAPI* OpenThreadDef)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
typedef HANDLE(WINAPI* CreateJobObjectADef)(LPSECURITY_ATTRIBUTES lpJobAttributes, LPCSTR lpName);
typedef HANDLE(WINAPI* CreateJobObjectWDef)(LPSECURITY_ATTRIBUTES lpJobAttributes, LPCWSTR lpName);

// Mail slot
typedef HANDLE(WINAPI* CreateMailslotADef)(LPCSTR lpName, DWORD nMaxMessageSize, DWORD lReadTimeout, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef HANDLE(WINAPI* CreateMailslotWDef)(LPCWSTR lpName, DWORD nMaxMessageSize, DWORD lReadTimeout, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

// pipe
typedef BOOL(WINAPI* CreatePipeDef)(PHANDLE hReadPipe, PHANDLE hWritePipe, LPSECURITY_ATTRIBUTES lpPipeAttributes, DWORD nSize);
typedef HANDLE(WINAPI* CreateNamedPipeADef)(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef HANDLE(WINAPI* CreateNamedPipeWDef)(LPCWSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

//Registry
typedef LSTATUS(WINAPI* RegCreateKeyExADef)(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
typedef LSTATUS(WINAPI* RegCreateKeyExWDef)(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
typedef LSTATUS(WINAPI* RegCreateKeyTransactedADef)(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);
typedef LSTATUS(WINAPI* RegCreateKeyTransactedWDef)(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);
typedef LSTATUS(WINAPI* RegOpenCurrentUserDef)(REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegOpenKeyADef)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegOpenKeyWDef)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegOpenKeyExADef)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegOpenKeyExWDef)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegOpenKeyTransactedADef)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID  pExtendedParemeter);
typedef LSTATUS(WINAPI* RegOpenKeyTransactedWDef)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID  pExtendedParemeter);
typedef LSTATUS(WINAPI* RegOpenUserClassesRootDef)(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY  phkResult);
typedef LSTATUS(WINAPI* RegCreateKeyADef)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegCreateKeyWDef)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);
typedef LSTATUS(WINAPI* RegCloseKeyDef)(HKEY hKey);

////////////////////////////////start v3 additions//////////////////////////////////////////////////
// Timers
typedef HANDLE(WINAPI* CreateTimerQueueDef)(void);
typedef BOOL(WINAPI* CreateTimerQueueTimerDef)(PHANDLE phNewTimer, HANDLE TimerQueue, WAITORTIMERCALLBACK Callback, PVOID Parameter, DWORD DueTime, DWORD Period, ULONG Flags);
typedef BOOL(WINAPI* DeleteTimerQueueTimerDef)(HANDLE TimerQueue, HANDLE Timer, HANDLE CompletionEvent);
typedef BOOL(WINAPI* DeleteTimerQueueExDef)(HANDLE TimerQueue, HANDLE CompletionEvent);
typedef BOOL(WINAPI* DeleteTimerQueueDef)(HANDLE TimerQueue);

//Critical section
typedef void (WINAPI* InitializeCriticalSectionDef)(LPCRITICAL_SECTION lpCriticalSection);
typedef BOOL(WINAPI* InitializeCriticalSectionExDef)(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags);
typedef BOOL(WINAPI* InitializeCriticalSectionAndSpinCountDef)(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
typedef void (WINAPI* DeleteCriticalSectionDef)(LPCRITICAL_SECTION lpCriticalSection);

////////////////////////////////end v3 additions//////////////////////////////////////////////////


typedef BOOL(WINAPI* DuplicateHandleDef)(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwOptions);
typedef BOOL(WINAPI* CloseHandleDef)(HANDLE hObject);

static CreateEventADef			s_pfnOrgCreateEventA;
static CreateEventWDef			s_pfnOrgCreateEventW;
static CreateEventExADef		s_pfnOrgCreateEventExA;
static CreateEventExWDef		s_pfnOrgCreateEventExW;
static OpenEventADef			s_pfnOrgOpenEventA;
static OpenEventWDef			s_pfnOrgOpenEventW;

static CreateMutexADef			s_pfnOrgCreateMutexA;
static CreateMutexWDef			s_pfnOrgCreateMutexW;
static CreateMutexExADef		s_pfnOrgCreateMutexExA;
static CreateMutexExWDef		s_pfnOrgCreateMutexExW;
static OpenMutexADef			s_pfnOrgOpenMutexA;
static OpenMutexWDef			s_pfnOrgOpenMutexW;

static CreateSemaphoreADef		s_pfnOrgCreateSemaphoreA;
static CreateSemaphoreWDef		s_pfnOrgCreateSemaphoreW;
static CreateSemaphoreExADef	s_pfnOrgCreateSemaphoreExA;
static CreateSemaphoreExWDef	s_pfnOrgCreateSemaphoreExW;
static OpenSemaphoreADef		s_pfnOrgOpenSemaphoreA;
static OpenSemaphoreWDef		s_pfnOrgOpenSemaphoreW;

static CreateWaitableTimerADef   s_pfnOrgCreateWaitableTimerA;
static CreateWaitableTimerWDef   s_pfnOrgCreateWaitableTimerW;
static CreateWaitableTimerExADef s_pfnOrgCreateWaitableTimerExA;
static CreateWaitableTimerExWDef s_pfnOrgCreateWaitableTimerExW;
static OpenWaitableTimerADef     s_pfnOrgOpenWaitableTimerA;
static OpenWaitableTimerWDef     s_pfnOrgOpenWaitableTimerW;

// file function
static CreateFileADef                   s_pfnOrgCreateFileA;
static CreateFileWDef                   s_pfnOrgCreateFileW;
static CreateFileTransactedADef         s_pfnOrgCreateFileTransactedA;
static CreateFileTransactedWDef         s_pfnOrgCreateFileTransactedW;
static FindFirstFileADef                s_pfnOrgFindFirstFileA;
static FindFirstFileWDef                s_pfnOrgFindFirstFileW;
static FindFirstFileExADef              s_pfnOrgFindFirstFileExA;
static FindFirstFileExWDef              s_pfnOrgFindFirstFileExW;
static FindFirstFileNameTransactedWDef  s_pfnOrgFindFirstFileNameTransactedW;
static FindFirstFileNameWDef            s_pfnOrgFindFirstFileNameW;
static FindFirstFileTransactedADef      s_pfnOrgFindFirstFileTransactedA;
static FindFirstFileTransactedWDef      s_pfnOrgFindFirstFileTransactedW;
static FindFirstStreamTransactedWDef    s_pfnOrgFindFirstStreamTransactedW;
static FindFirstStreamWDef              s_pfnOrgFindFirstStreamW;
static FindCloseDef                     s_pfnOrgFindClose;
static OpenFileByIdDef                  s_pfnOrgOpenFileById;
static ReOpenFileDef                    s_pfnOrgReOpenFile;
static CreateIoCompletionPortDef        s_pfnOrgCreateIoCompletionPort;

//Authorization function
static CreateRestrictedTokenDef			s_pfnOrgCreateRestrictedToken;
static DuplicateTokenDef				s_pfnOrgDuplicateToken;
static DuplicateTokenExDef				s_pfnOrgDuplicateTokenEx;
static OpenProcessTokenDef				s_pfnOrgOpenProcessToken;
static OpenThreadTokenDef				s_pfnOrgOpenThreadToken;

//Directory management
static FindFirstChangeNotificationADef s_pfnOrgFindFirstChangeNotificationA;
static FindFirstChangeNotificationWDef s_pfnOrgFindFirstChangeNotificationW;
static FindCloseChangeNotificationDef  s_pfnOrgFindCloseChangeNotification;

// File mapping
static CreateMemoryResourceNotificationDef  s_pfnOrgCreateMemoryResourceNotification;
static CreateFileMappingADef                s_pfnOrgCreateFileMappingA;
static CreateFileMappingWDef                s_pfnOrgCreateFileMappingW;
static CreateFileMappingNumaADef            s_pfnOrgCreateFileMappingNumaA;
static CreateFileMappingNumaWDef            s_pfnOrgCreateFileMappingNumaW;
static OpenFileMappingADef                  s_pfnOrgOpenFileMappingA;
static OpenFileMappingWDef                  s_pfnOrgOpenFileMappingW;

//Memory
static HeapCreateDef				s_pfnOrgHeapCreate;
static HeapDestroyDef				s_pfnOrgHeapDestroy;
static GlobalAllocDef				s_pfnOrgGlobalAlloc;
static GlobalReAllocDef				s_pfnOrgGlobalReAlloc;
static GlobalFreeDef				s_pfnOrgGlobalFree;
static LocalAllocDef				s_pfnOrgLocalAlloc;
static LocalReAllocDef				s_pfnOrgLocalReAlloc;
static LocalFreeDef					s_pfnOrgLocalFree;

//Process and thread
static CreateProcessADef			s_pfnOrgCreateProcessA;
static CreateProcessWDef			s_pfnOrgCreateProcessW;
static CreateProcessAsUserADef		s_pfnOrgCreateProcessAsUserA;
static CreateProcessAsUserWDef		s_pfnOrgCreateProcessAsUserW;
static CreateProcessWithLogonWDef	s_pfnOrgCreateProcessWithLogonW;
static CreateProcessWithTokenWDef	s_pfnOrgCreateProcessWithTokenW;
static OpenProcessDef				s_pfnOrgOpenProcess;
static CreateThreadDef				s_pfnOrgCreateThread;
static CreateRemoteThreadDef		s_pfnOrgCreateRemoteThread;
static OpenThreadDef				s_pfnOrgOpenThread;
static CreateJobObjectADef			s_pfnOrgCreateJobObjectA;
static CreateJobObjectWDef			s_pfnOrgCreateJobObjectW;

// Mail slot
static CreateMailslotADef			s_pfnOrgCreateMailslotA;
static CreateMailslotWDef			s_pfnOrgCreateMailslotW;

// pipe
static CreatePipeDef				s_pfnOrgCreatePipe;
static CreateNamedPipeADef			s_pfnOrgCreateNamedPipeA;
static CreateNamedPipeWDef			s_pfnOrgCreateNamedPipeW;

//Registry
static RegCreateKeyExADef           s_pfnOrgRegCreateKeyExA;
static RegCreateKeyExWDef           s_pfnOrgRegCreateKeyExW;
static RegCreateKeyTransactedADef   s_pfnOrgRegCreateKeyTransactedA;
static RegCreateKeyTransactedWDef   s_pfnOrgRegCreateKeyTransactedW;
static RegOpenCurrentUserDef        s_pfnOrgRegOpenCurrentUser;
static RegOpenKeyADef               s_pfnOrgRegOpenKeyA;
static RegOpenKeyWDef               s_pfnOrgRegOpenKeyW;
static RegOpenKeyExADef             s_pfnOrgRegOpenKeyExA;
static RegOpenKeyExWDef             s_pfnOrgRegOpenKeyExW;
static RegOpenKeyTransactedADef     s_pfnOrgRegOpenKeyTransactedA;
static RegOpenKeyTransactedWDef     s_pfnOrgRegOpenKeyTransactedW;
static RegOpenUserClassesRootDef    s_pfnOrgRegOpenUserClassesRoot;
static RegCreateKeyADef             s_pfnOrgRegCreateKeyA;
static RegCreateKeyWDef             s_pfnOrgRegCreateKeyW;
static RegCloseKeyDef               s_pfnOrgRegCloseKey;

static DuplicateHandleDef			s_pfnOrgDuplicateHandle;
static CloseHandleDef				s_pfnOrgCloseHandle;

////////////////////////////////v3 additions//////////////////////////////////////////////////
// Timers
static CreateTimerQueueDef			s_pfnOrgCreateTimerQueue;
static CreateTimerQueueTimerDef		s_pfnOrgCreateTimerQueueTimer;
static DeleteTimerQueueTimerDef		s_pfnOrgDeleteTimerQueueTimer;
static DeleteTimerQueueExDef		s_pfnOrgDeleteTimerQueueEx;
static DeleteTimerQueueDef			s_pfnOrgDeleteTimerQueue;

//Critical section
static InitializeCriticalSectionDef             s_pfnOrgInitializeCriticalSection;
static InitializeCriticalSectionExDef           s_pfnOrgInitializeCriticalSectionEx;
static InitializeCriticalSectionAndSpinCountDef s_pfnOrgInitializeCriticalSectionAndSpinCount;
static DeleteCriticalSectionDef                 s_pfnOrgDeleteCriticalSection;
////////////////////////////////v3 additions//////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


typedef struct tag_HOOKFUNCDESC
{
	LPCSTR szFunc; // The name of the function to hook.
	DWORD dwOrdinal;
	PROC pProc;    // The procedure to blast in.
	PROC pOrigProc;    // The procedure to blast in.
	LPCTSTR lpszDllName;
} HOOKFUNCDESC, * LPHOOKFUNCDESC;


PIMAGE_IMPORT_DESCRIPTOR GetNamedImportDescriptor(HMODULE hModule, LPCSTR szImportMod);
BOOL HookDynamicLoadedFun(UINT uiCount, LPHOOKFUNCDESC paHookArray, bool bRestore = false);

#endif //__HOOK_FUNCTIONS_H__