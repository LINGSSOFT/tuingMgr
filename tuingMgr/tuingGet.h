#pragma once
struct STuinggetInfo {				// widget information
	wstring name;					// 문자열로 된 위젯 설명. UI에 사용
	wstring desc;					// 위젯에 대한 상세 설명
	wstring cls;					// 위젯의 소속 클래스.멀티인 경우 일련 번호를 붙여 id 를 만든다.
	BOOL allowMulti;				// 멀티 인스턴스 가능 여부. 
	INT initHeight;					// 수직 배치일 때의 초기 높이. 
	INT initWidth;					// 수평 배치일 때의 초기 너비
	COLORREF initColor;				// 배경색
};

class CTuingGet : public CBaseGet {
public:
	INT vertHeight;					// 수직일 때의 높이
	INT horzWidth;					// 수평일 때의 너비
	BOOL span;						// true 이면 앞 위젯과 셀 공유
	INT spanRatio;					// 분할량. 양수는 비율, 음수는 픽셀

	static CTuingGet* CreateObject(wstring cls);
	static CTuingGet* CreateDefaultObject(wstring cls);
	static CTuingGet* Deserialize(LPCTSTR pack);
	static BOOL GetPackToken(LPCTSTR pack, INT idx, TCHAR token[MAX_PATH]);
	VOID Serialize(TCHAR* pack);
};

class CClock : public CTuingGet {
public:
	INT faceType;					// clock type
	BOOL hour24;					// 24 hour-time
	BOOL outSec;					// display second
	BOOL digit2;					// 2 digit(hour/minute/second)
	INT month, day;
	wstring yoil;
	INT hour, min, sec;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID getRadialPos(INT cx, INT cy, INT radius, DOUBLE hour, INT& x, INT& y);
	VOID LoadSetting();
	VOID SaveSetting();
	static INT_PTR CALLBACK ClockDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CRadix : public CTuingGet {
public:
	HWND hEdit2;
	HWND hEdit10;
	HWND hEdit16;
	bool editByCode;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
};

class CCpuUsage : public CTuingGet {
public:
	static const INT USAGE_NUM = 60;
	DOUBLE arUsage[USAGE_NUM];
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;
	INT timerInterval;
	BOOL varYAxis;
	BOOL viewThread;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID LoadSetting();
	VOID SaveSetting();
	static INT_PTR CALLBACK CpuUsageDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CMemUsage : public CTuingGet {
public:
	INT timerInterval;
	MEMORYSTATUSEX mem;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
};

class CBattery : public CTuingGet {
public:
	SYSTEM_POWER_STATUS sps;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	VOID OnDraw(HDC hdc, INT width, INT height);
};

class CDiskUsage : public CTuingGet {
public:
	struct DiskInfo {
		TCHAR drive;
		ULARGE_INTEGER Avail, Total, Free;
	};

	INT timerInterval;
	DiskInfo arDisk[26];
	BOOL includeRemovable;
	BOOL includeNetwork;
	BOOL relativeChart;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID LoadSetting();
	VOID SaveSetting();
	static INT_PTR CALLBACK DiskUsageDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CCalc : public CTuingGet {
public:
	HWND hCbExp;
	HWND hEdit;
	HWND hBtnCopy;
	DOUBLE calcResult;
	TCHAR sResult[128];
	BOOL autoCopy;
	static WNDPROC CalcOldEditProc;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser1(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser2(WPARAM wParam, LPARAM lParam);

	DOUBLE CalcExp(TCHAR* exp, BOOL* bError/*=NULL*/);
	DOUBLE CalcPostfix(TCHAR* Post);
	INT GetPriority(INT op);
	BOOL isCalcChar(TCHAR c);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID MakePostfix(TCHAR* Post, TCHAR* Mid);
	VOID CopyResult();

	static LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CMusicPlayer : public CTuingGet {
public:
	HWND hPlay, hStop, hPrev, hNext;
	HWND hProg, hList;
	HWND hGroup1, hGroup2, hGroup3, hGroup4;
	HWND hAdd, hClear, hRandom;
	enum {
		MP_PLAY, MP_STOP, MP_PREV, MP_NEXT, MP_PROG,
		MP_LIST, MP_GROUP1, MP_GROUP2, MP_GROUP3, MP_GROUP4,
		MP_ADD, MP_CLEAR, MP_RANDOM
	};
	// 타이머 종류. 1번 빼고 다 1회용
	enum {
		MPTM_PROG = 1,				// 프로그래스 갱신용
		MPTM_PLAYACTIVE,			// 현재 포커스를 가진 곡 재생
		MPTM_PLAYNEXT,				// 재생 그룹의 다음곡 재생
		MPTM_PLAYPREV,				// 재생 그룹의 이전곡 재생
		MPTM_MOVEPOS,				// 현재 음악 위치 이동
	};

	vector<wstring> arMusic[4];		// 음악 목록
	INT playGroup;					// 지금 재생중인 그룹
	INT playIdx;					// 현재 재생중인 playGroup내에서의 곡
	INT groupIdx[4];				// 각 그룹의 현재곡
	BOOL randomPlay;				// 랜덤 재생
	UINT dev_id;					// MCI 장치. 0이면 재생중이 아님
	TCHAR stripPath[MAX_PATH];		// ID3 태그를 제거한 임시 파일 경로

	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	BOOL DoPlayCurrent(BOOL bNext);
	BOOL DoPlay(LPCTSTR musicPath, INT fromPos = 0);
	BOOL StripID3Tag(LPCTSTR path);
	BOOL AddFile(LPCTSTR Path);
	DWORD GetStatus();
	DWORD GetPosition();
	DWORD GetLength();
	INT GetActiveGroup();
	INT SelectRandomPlay();
	VOID PlayActiveCurrent();
	VOID LoadMusicList();
	VOID SaveMusicList();
	VOID FillListBox(INT group);
	VOID ChangeActiveGroup(INT group, BOOL init = FALSE);
};

class CImageViewer;
class CImageSlide : public CTuingGet {
public:
	INT timerInterval;
	INT nowIdx;
	Image* nowImage;
	vector<wstring> arImage;
	BOOL randomSlide;				// 랜덤 재생
	BOOL displayFileName;			// 파일명 표시
	BOOL multiViewer;				// 클릭할 때마다 뷰어 각각 하나씩 열기
	CImageViewer* pLastViewer;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser1(WPARAM wParam, LPARAM lParam);
	LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID LoadSetting();
	VOID SaveSetting();
	VOID LoadImageList();
	VOID SaveImageList();
	static INT_PTR CALLBACK ImageSlideDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CMemoViewer;
class CMemo : public CTuingGet {
public:
	HWND hCombo, hAdd, hDelete, hView, hEdit;
	enum {
		MEMO_COMBO, MEMO_ADD, MEMO_DELETE, MEMO_VIEW, MEMO_EDIT
	};

	struct sMemo {
		TCHAR title[64];				// 제목
		INT x, y, w, h;					// 윈도우 위치, 크기
		COLORREF foreColor, backColor;	// 전경, 배경색
		BOOL isText;					// 텍스트 또는 서식문서
		TCHAR fontFace[32];				// 폰트.
		INT fontSize;					// 폰트 크기(pt 단위)
		BOOL wordWrap;					// 자동 개행 여부
		TCHAR* content;					// 내용
		INT content_len;				// 내용의 길이

		BOOL isModi;					// 편집 여부
		CMemoViewer* pViewer;			// 뷰어 객체. 안 열려 있으면 NULL
	};

	vector<sMemo*> arMemo;
	INT nowIdx;
	BOOL isModi;						// 메모중 하나라도 바뀐게 있는지 기억

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnUser1(WPARAM wParam, LPARAM lParam);

	VOID SaveMemo(INT idx);
	VOID SaveMemo(sMemo* pMemo, HWND hEdit);
	VOID SaveMemoList();
	VOID LoadMemoList();
	VOID GetMemoFilePath(TCHAR* Path);
};

class CMemoViewer : public CBaseWnd {
public:
	CMemo* pMemoWidget;
	INT nowIdx;
	CMemo::sMemo* pMemo;
	BOOL isModi;
	HWND hClose, hAttr, hEdit;
	HFONT fontEdit;
	HBRUSH backBrush;
	BOOL destroyBySelf;
	enum { MV_CLOSE, MV_ATTR, MV_EDIT };

	LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	VOID Create(CMemo* pMemoWidget, INT idx);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID SendMemoToWidget();
	VOID RefreshMemo();
};

class CDday : public CTuingGet {
public:
	wstring targetTitle;
	SYSTEMTIME targetDate;
	TCHAR remain[128];
	TCHAR remainOld[128];
	BOOL displayHour;
	BOOL displayMinute;

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID LoadSetting();
	VOID SaveSetting();
	static INT_PTR CALLBACK DdayDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CLauncher : public CTuingGet {
public:
	struct sProgram {
		TCHAR path[MAX_PATH];		// 프로그램 또는 파일 경로
		INT nIcon;					// 아이콘 번호이되 현재 0으로 고정

		// 런타임 데이터
		HICON hIcon;				// 표시할 아이콘
		RECT rt;					// 아이콘 영역
	};
	vector<sProgram> arProgram;
	INT iconSize;					// 아이콘 크기
	INT iconGap;					// 아이콘 간격
	BOOL dispCap;					// 제목 표시
	INT capHeight;					// 제목 높이
	INT focusIcon;					// 포커스를 가진 아이콘
	INT oldFocusIcon;
	INT clickIcon;					// 클릭한 아이콘

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	INT FindIconFromPt(INT x, INT y);
	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID LoadSetting();
	VOID SaveSetting();
	VOID ArrangeIcon();
	VOID ExtractFileIcon(sProgram& prg);
	static INT_PTR CALLBACK LauncherDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

class CCalendar : public CTuingGet {
public:
	INT top_year, top_month;		// 최상단 표시할 년월
	INT today_y, today_m;			// 오늘
	INT today_d;

	BOOL checkMidnight;
	INT lineGap;
	INT minMonthWidth;
	INT monthGap;
	HFONT holyFont;
	POINT click_pt;					// 클릭한 좌표
	INT click_y, click_m, click_d;	// 클릭한 날짜

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);

	VOID OnDraw(HDC hdc, INT width, INT height);
	VOID DrawMonth(HDC hdc, INT width, INT year, INT month, INT xpos, INT ypos);
};
