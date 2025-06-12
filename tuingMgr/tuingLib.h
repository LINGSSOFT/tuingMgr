#pragma once
// macro function
#define Count(obj) (sizeof(obj)/sizeof(obj[0]))
#define TextOutSz(h,x,y,t) TextOut(h,x,y,t,lstrlen(t));
#define ABS(val) ((val) >= 0 ? (val):-(val))
#define Random(n) (rand()%(n))

// utility proto function
VOID Trace(LPCTSTR format, ...);
VOID WriteLog(LPCTSTR strLog, ...);
VOID DrawBitmap(HDC hdc, INT x, INT y, HBITMAP hBit);
VOID StretchBitmap(HDC hdc, INT x, INT y, INT w, INT h, HBITMAP hBit);
VOID TextOutVCenter(HDC hdc, INT x, INT y, LPCTSTR str);
VOID FillRect(HDC hdc, INT left, INT top, INT right, INT bottom, HBRUSH brush);
VOID moveNext(INT& now, INT size);
VOID movePrev(INT& now, INT size);
VOID MySetImeMode(HWND hEdit, BOOL bHan);

struct SMonitorInfo {
	HMONITOR hMonitor;
	RECT rcMonitor;
	RECT rcWork;
	bool primary;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
INT GetMonitorOfWindow(HWND hWnd);
INT GetPrimaryMonitor();

// language type
enum class eLang { KOR, ENG, JPN, CHN };

// GrpBtn style
#define GBS_PUSH 0
#define GBS_CHECK 1

// GrpBtn message
#define GBM_SETIMAGE WM_USER+1 
#define GBM_GETSTATE WM_USER+2
#define GBM_SETSTATE WM_USER+3

// 통지 메시지
#define GBN_CLICKED 0

// 버튼의 현재 상태
enum eState { GB_NORMAL, GB_HOT, GB_DOWN, GB_DISABLE };
VOID ChangeState(HWND hWnd, eState State);

class CSetting {
public:
	BOOL useIni;
	TCHAR iniPath[MAX_PATH] = _T("");
	TCHAR regRoot[MAX_PATH] = _T("");		// 사용전에 \\도 포함하여 직접 경로 설정할 것. 
	HKEY hive;

	// 지역 설정인지 생성 시점에 결정한다. 레지스트리 경로와 하이브는 생성 후 직접 대입한다.
	CSetting(BOOL useIni) {
		this->useIni = useIni;
		hive = HKEY_CURRENT_USER;
	}

	// 실행 파일 폴더로 iniFile 경로를 초기화한다. 
	INT get(wstring section, LPCTSTR key, INT defValue);
	VOID SetIniPath(LPCTSTR filename);
	VOID get(wstring section, LPCTSTR key, LPCTSTR defValue, LPTSTR retValue, INT nSize = 128);
	DOUBLE getDouble(wstring section, LPCTSTR key, DOUBLE defValue);

	BOOL set(wstring section, LPCTSTR key, INT value);
	BOOL set(wstring section, LPCTSTR key, LPCTSTR value);
	BOOL setDouble(wstring section, LPCTSTR key, DOUBLE value);

	VOID deleteSection(wstring section);
};

VOID EnsureVisible(HWND hWnd);

struct sNamedColor {
	LPCTSTR name;
	COLORREF color;
};

COLORREF GetColorFromName(LPCTSTR name);
INT FindWidgetClsIdx(LPCTSTR cls);
INT FindWidgetNameIdx(LPCTSTR name);
INT FindTidgetNameIdx(LPCTSTR name);
INT FindTidgetClsIdx(LPCTSTR cls);
DWORD MyGetAbsSecond(SYSTEMTIME st);
VOID MovePopupToSide(HWND hDlg, INT wx, INT wy);
VOID MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME& st);
VOID FixImageOrientation(Image* pImage);
VOID DrawImageCenter(HDC hdc, Image* pImage, INT width, INT height, BOOL erase, HBRUSH backBrush);
VOID makeFullScreen(HWND hWnd);

class CGdiPlusStarter {
private:
	ULONG_PTR m_gpToken;

public:
	BOOL m_bSuccess;
	CGdiPlusStarter() {
		GdiplusStartupInput gpsi;
		m_bSuccess = (GdiplusStartup(&m_gpToken, &gpsi, NULL) == Ok);
	}
	~CGdiPlusStarter() {
		GdiplusShutdown(m_gpToken);
	}
};

VOID CopyClipboardText(LPCTSTR content);

template<typename T>
class TStack {
protected:
	T* Stack;
	INT Size;
	INT Top;

public:
	TStack(INT aSize) {
		Size = aSize;
		Stack = (T*)malloc(Size * sizeof(T));
		Top = -1;
	}
	virtual ~TStack() {
		free(Stack);
	}
	virtual BOOL Push(T data) {
		if (Top < Size - 1) {
			Top++;
			Stack[Top] = data;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	virtual T Pop() {
		return Stack[Top--];
	}
	virtual INT GetTop() { return Top; }
	virtual T GetValue(INT n) { return Stack[n]; }
};

VOID TrimTrailZero(TCHAR* num);

// 그래픽 스크롤 바 관련 코드. GrpScr의 스타일
#define GSS_HORZ 1
#define GSS_VERT 2

// GrpScr의 메시지
#define GSM_SETRANGE WM_USER+1
#define GSM_GETRANGEHI WM_USER+2
#define GSM_GETRANGELOW WM_USER+3
#define GSM_SETPOS WM_USER+4
#define GSM_GETPOS WM_USER+5
#define GSM_SETTSIZE WM_USER+6
#define GSM_GETTSIZE WM_USER+7
#define GSM_SETTGAP WM_USER+8
#define GSM_GETTGAP WM_USER+9
#define GSM_SETBACKCOLOR WM_USER+10
#define GSM_SETTHUMBCOLOR WM_USER+11

VOID DrawOwnerItem(LPARAM lParam, sNamedColor arColor[]);
LPCTSTR GetFileExtension(LPCTSTR path, TCHAR* ext);
BOOL AppBarRegister(HWND hWnd, UINT uCallBack);
BOOL AppBarUnRegister(HWND hWnd);
INT AppBarSetPos(HWND hWnd, INT nMonitor, UINT uEdge, UINT nPixel, BOOL move);
