#define ITEM_LENGTH			128					// length limit 128
#define	WM_ICON_NOTIFY		WM_USER+78			// user Defined Notify Message
#define WM_MYAPPCALLBACK	WM_USER+200			// user Defined Notify Message

static CONST TCHAR szProgName[] = _T("TUINGMGR");				// prevent duplicate execution
static CONST TCHAR szWindowClass[] = _T("TuingMgr");			// the main window class name

class CMainWnd : public CBaseWnd {
public:
	// member function
	VOID Create();
	VOID Relayout();
	VOID Docking(INT nMonitor, UINT edge);
	VOID UnDocking();
	VOID UpdateBorderEdge();
	BOOL IsInCaption(INT cx, INT cy);
	INT FindSplitVirt(INT cy);
	INT FindSplitHorz(INT cx);
	INT GetSpanNum(INT wi);

	// callback function
	static INT_PTR CALLBACK SelWidgetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK SelTidgetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	// override function
	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	// member variable
	INT dockEdge;					// 도킹면. -1이면 도킹 아님. 
	INT dockMonitor;				// 도킹 모니터.
	BOOL bVertical;					// 방향
	INT alwaysOnTop;				// 항상 위
	INT capThick;					// 캡션 두께
	COLORREF capColor;				// 캡션 색상
	INT bdThick;					// 경계선 두께
	COLORREF bdColor;				// 경계선 색상
	INT splitThick;					// 스플릿 두께
	COLORREF splitColor;			// 스플릿 색상
	INT vertThick, horzThick;		// 각 방향별 두께. 경계선 포함. 사용자가 조정해야 바뀜
	INT clientWidth, clientHeight;	// 방향에 따라 바뀌는 현재 폭과 높이. 경계선 포함
	BOOL magneticEffet;				// 자석 효과 사용 여부
	INT widgetMinThick;				// 위젯 최소, 최대 크기
	INT widgetMaxThick;

	HCURSOR cursorHorz, cursorVert;
	INT dragWidget;
	HWND btnClose, btnMenu;
	enum { mainBTNCLOSE = 1, mainBTNMENU };
	INT bl, bt, br, bb;
};

class CBaseTray : public CBaseWnd {
public:
	INT tidgetSize;		// 티젯 크기
	INT tidgetGap;		// 티젯간의 간격
	INT trayThick;		// 트레이의 높이 또는 폭

	VOID Create(HWND hParent);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID UpdateTrayThick(INT widgetThick);
	VOID ArrangeTrayTidget(INT widgetThick);
};
