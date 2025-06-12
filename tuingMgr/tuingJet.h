#pragma once
struct STuingjetInfo {			// tidget information
	wstring name;				// 문자열로 된 위젯 설명. UI에 사용
	wstring desc;				// 위젯에 대한 상세 설명
	wstring cls;				// 위젯의 소속 클래스.멀티인 경우 일련 번호를 붙여 id를 만든다.
	BOOL allowMulti;			// 멀티 인스턴스 가능 여부. 
};

class CTuingJet : public CBaseGet {
public:
	virtual VOID OnClick() = 0;
	// 클릭 검출 기능 제공. 차일드는 OnClick만 재정의하면 된다. 마우스 기능을 직접 사용하려면
	// 두 핸들러를 재정의하되 클릭까지 받으려면 부모의 메서드를 먼저 호출해야 한다.
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);

	static CTuingJet* CreateObject(wstring cls);
	static CTuingJet* CreateDefaultObject(wstring cls);
	static CTuingJet* Deserialize(LPCTSTR pack);
	VOID Serialize(TCHAR* pack);
};

class CLCDTestWin : public CBaseWnd {
public:
	COLORREF arColor[8];
	BOOL displayText;
	BOOL animate;
	INT colorIdx;
	INT nowMonitor;
	INT tileWidth;
	INT aniIdx;
	INT aniOffset;

	VOID Create(HWND hParent);
	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

	LRESULT RotateColor();
	LRESULT RotateMonitor();
};

class CLCDTest : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CTextClip : public CTuingJet {
public:
	HBITMAP bitOn, bitOff;
	BOOL clipToText;
	BOOL autoText;
	INT clipCount;
	HWND hNextChain;

	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
	VOID ClipboardTextOnly();
	VOID ChangeTextClip(BOOL clipToText);
	VOID ChangeAutoText(BOOL autoText);
	static INT_PTR CALLBACK TextClipDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CColorTableWin : public CBaseWnd {
public:
	HWND hList, hBtnCopy[4];
	enum { CT_LIST, CT_COPY1, CT_COPY2, CT_COPY3, CT_COPY4 };
	struct sOneColor {
		TCHAR name[32];
		COLORREF color;
	};
	sOneColor arColor[300];
	BOOL namedColor;
	INT colorCount;
	INT tile_width;
	INT tile_height;
	INT perLine;
	INT selIdx, prevSel;
	const INT leftMargin = 150;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);

	static INT colorNameCompare(const VOID* a, const VOID* b);
	VOID Create(HWND hParent);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID PrepareColor(INT colorType);
	VOID CalcTileSize();
	VOID ChangeSelColor(INT idx);
};

class CColorTable : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CMoveCursor : public CTuingJet {
public:
	HBITMAP bitOn, bitOff;
	BOOL bCursorMove;			// 커서 강제 이동 여부
	INT TimerInterval;			// 타이머 주기
	INT CursorInterval;			// 커서 강제 이동 주기(초단위)
	INT CursorKeepSec;			// 커서가 움직이지 않고 제 자리에 있었던 시간
	INT RejectCount;			// 커서 이동 회수(스크린 세이버 방지 회수)
	POINT OldCursorPt;			// 이전 커서 위치

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
	VOID LoadSetting();
	VOID SaveSetting();
	static INT_PTR CALLBACK MoveCursorDlgProc(HWND hDlg,
		UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CSizeMeter : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CSizeMeterWin : public CBaseWnd {
public:
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	VOID Create(HWND hParent);
	VOID OnDraw(HDC hdc, INT width, INT height);
};

class CTetris : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CTetrisWin : public CBaseWnd {
public:
	enum class eTetCell { EMPTY, BRICK, WALL = 7/*sizeof(Shape) / sizeof(Shape[0]) + 1*/ };
	INT ts;
	INT bw;
	INT bh;
	INT nx, ny;
	INT brick, rot;
	INT nbrick;
	INT score;
	INT bricknum;
	INT Interval;
	UCHAR board[405][205];
	enum class eTetStatus { GAMEOVER, RUNNING, PAUSE };
	eTetStatus GameStatus;
	HBRUSH arBrush[8];

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);

	INT GetAround(INT x, INT y, INT b, INT r);
	BOOL MoveDown();
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID MakeNewBrick();
	VOID TestFull();
	VOID PrintTile(HDC hdc, INT x, INT y, INT c);
	VOID AdjustMainWindow();
};

class CImageViewer;
class COneImage : public CTuingJet {
public:
	TCHAR imagePath[MAX_PATH];
	Image* nowImage;
	HBITMAP icon;
	CImageViewer* pViewer;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser1(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CImageSlide;
class CImageViewer : public CBaseWnd {
public:
	INT parentType;			// 누가 부모인지. 0 = 슬라이더, 1 = oneImage
	CBaseGet* pParent;		// 부모 객체. 아래 변수로 미리 캐스팅해 둔다.
	CImageSlide* pSlide;
	COneImage* pOneImage;
	INT nowIdx;						// 슬라이더의 현재 인덱스
	TCHAR imagePath[MAX_PATH];		// OneImage의 이미지 경로
	Image* nowImage;				// 런타임에 생성하는 이미지 객체
	BOOL fullScreen;
	WINDOWPLACEMENT prevWndPlace;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser1(WPARAM wParam, LPARAM lParam);

	VOID Create(INT parentType, CBaseGet* parent);
	VOID toggleFullScreen();
};

class CEnumFont : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CEnumFontWin : public CBaseWnd {
public:
	HWND hList, hChkAt, hChkSort, hStNum, hBtnApi, hEdApi, hBtnMfc, hEdMfc;
	enum { EF_LIST, EF_AT, EF_SORT, EF_NUM, EF_API, EF_EDAPI, EF_MFC, EF_EDMFC };
	static const INT MAXFONT = 1000;
	LOGFONT arFont[CEnumFontWin::MAXFONT];
	INT arFontType[CEnumFontWin::MAXFONT];
	INT numFont;	// 발견된 폰트수
	INT nowFont;	// 현재 폰트
	BOOL includeAt;	// @ 폰트 포함 여부

	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID EnumFont();
	VOID GetCharSetStr(INT CharSet, TCHAR* str);
	VOID GetPitchFamStr(INT Pitch, TCHAR* str);
	VOID SetCreateFontCommand();
	static INT CALLBACK EnumFamCallBack(ENUMLOGFONT FAR* lpelf, NEWTEXTMETRIC FAR* lpntm, INT FontType, LPARAM lParam);
	static INT compareFace(const VOID* a, const VOID* b);
};

class CSpy : public CTuingJet {
public:
	HBITMAP icon;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};

class CSpyWin : public CBaseWnd {
public:
	int TimerInterval;
	HWND hWndOld;
	HWND hEditInfo, hChkPause, hEditHier;
	enum { SPY_INFO, SPY_PAUSE, SPY_HIER };

	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

	VOID GetStyleString(DWORD dwStyle, TCHAR* sStyle);
	VOID GetExStyleString(DWORD dwExStyle, TCHAR* sExStyle);
	static BOOL CALLBACK EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam);
};

class CSwapMouse : public CTuingJet {
public:
	HBITMAP bitOn, bitOff;
	bool swapButton;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID OnClick();
};


