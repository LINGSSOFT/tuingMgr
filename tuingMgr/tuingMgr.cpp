#include "framework.h"

// 이 프로그램이 보유한 위젯, 티젯 목록. 이 중 사용자가 선택한 것만 사용한다. 
STuinggetInfo arWidgetInfo[] = {
	{_T("시계"), _T("현재 시간을 보여 준다. 디지털/아날로그 페이스를 선택할 수 있으며 초단위 표시, 24시간제, 두 자리 표기 등의 옵션이 있다."), _T("Clock"), TRUE, 80, 120, GetColorFromName(_T("PapayaWhip"))},
	{_T("진법변환기"), _T("2진수, 10진수, 16진수를 자동 변환한다. 세 값 하나를 입력하면 나머지 진법을 즉시 변환한다."), _T("Radix"), TRUE, 50, 120, GetColorFromName(_T("Violet"))},
	{_T("CPU 사용량"), _T("CPU 점유율을 총 60회의 그래프로 표시한다. 조사주기를 선택할 수 있고 최대 점유율에 따라 Y축 범위를 자동 결정한다."), _T("CpuUsage"), TRUE, 100, 140, GetColorFromName(_T("PaleTurquoise"))},
	{_T("메모리 사용량"), _T("메모리 총 설치량, 사용량, 사용 비율을 표시하며 주기적으로 갱신한다."), _T("MemUsage"), TRUE, 50, 120, GetColorFromName(_T("LightBlue"))},
	{_T("배터리"), _T("배터리 남은 용량과 시간, 충전 상태를 조사하여 막대 그래프로 충전 비율을 표시한다."), _T("Battery"), TRUE, 50, 120, GetColorFromName(_T("LightSkyBlue"))},
	{_T("디스크 사용량"), _T("하드, SSD 등의 스토리지 용량과 사용량을 표시한다. 이동식, 네트워크 드라이브도 같이 표시할 수 있으며 최대 용량 스토리지에 대한 상대적인 비율로 표시한다."), _T("DiskUsage"), TRUE, 50, 120, GetColorFromName(_T("Wheat"))},
	{_T("계산기"), _T("다항식을 계산한다. 사칙 연산과 누승(^), 괄호 우선 순위 등을 지원한다. 이전 계산식을 콤보 박스에서 다시 불러올 수 있고 계산 결과를 클립보드로 복사한다."), _T("Calc"), TRUE, 50, 120, GetColorFromName(_T("Pink"))},
	{_T("음악 재생기"), _T("MP3 음악을 재생한다. 탐색기에서 파일을 드래그하여 음악을 추가하며 상단의 그룹탭을 사용하여 4개의 그룹에 음악 목록을 각각 관리한다. 프로그래스 바를 드래그하여 임의 위치로 이동하며 랜덤 재생 옵션을 지원한다. Pause키는 전역 재생/정지 단축키이다."), _T("MusicPlayer"), TRUE, 150, 120, GetColorFromName(_T("Gainsboro"))},
	{_T("이미지 슬라이더"), _T("이미지 목록을 주기적으로 교체하며 보여 준다. 파일을 드래그하여 목록에 추가하며 마우스 오른쪽 버튼을 눌러 나타나는 팝업창에서 순서 및 시간을 선택한다."), _T("ImageSlide"), TRUE, 100, 120, GetColorFromName(_T("DarkGray"))},
	{_T("메모"), _T("텍스트 형식의 간단한 메모를 기록 및 편집한다. 위젯 화면에서 직접 편집 가능하다. 우상단의 확대 버튼을 클릭하여 포스트잇 형식의 별도의 메모창으로 열어 편집할 수 있으며 각 메모의 색상, 글꼴 등을 선택할 수 있다."), _T("Memo"), TRUE, 150, 150, GetColorFromName(_T("Ivory"))},
	{_T("날짜 세기"), _T("특정 날짜 이후 경과 시간 또는 특정 날짜까지 남은 시간을 보여 줍니다. 예를 들어 금연 12일째, 제대 625일 전의 시간 정보를 보여 준다. 일, 시, 분까지 상세하게 볼 수 있다."), _T("Dday"), TRUE, 80, 120, GetColorFromName(_T("SpringGreen"))},
	{_T("론처"), _T("자주 사용하는 프로그램을 등록해 두고 클릭하여 실행한다."), _T("Launcher"), TRUE, 100, 120, GetColorFromName(_T("Lavender"))},
	{_T("달력"), _T("한달 달력을 보여 주며 별도의 창을 열면 연력과 기념일 목록을 보여 준다."), _T("Calendar"), TRUE, 120, 120, GetColorFromName(_T("Ivory"))},
	{_T(""), _T(""), _T(""), FALSE, 0, 0, RGB(0, 0, 0)},
};

STuingjetInfo arTidgetInfo[] = {
	{_T("LCD 테스트"), _T("LCD 액정을 점검합니다. 색상, 모니터를 단축키로 변경하며 애니메이션을 통해 잔상 검사를 수행한다. Esc키로 종료한다."), _T("LCDTest"), FALSE},
	{_T("클립보드 변환기"), _T("클립보드에 텍스트 포맷만 남기는 Ctrl + Shift + C 단축키를 제공한다. 서식없이 텍스트만 복사할 때 사용하며 티젯을 클릭하여 동작 여부를 토글한다."), _T("TextClip"), FALSE},
	{_T("색상 테이블"), _T("표준 색상표를 보여주고 색상값을 복사한다. 세가지 형식의 색상표를 지원하며 두 가지 색상을 차례대로 선택하여 비교한다."), _T("ColorTable"), FALSE},
	{_T("커서 이동"), _T("주기적으로 커서를 이동하여 화면 보호기 동작을 방지한다. 장기간의 작업을 수행할 때 컴퓨터를 계속 동작 상태로 유지한다."), _T("MoveCursor"), FALSE},
	{_T("윈도우 크기 측정"), _T("표준 모니터 크기의 면적을 실제 윈도우로 확인한다. 창 크기를 바꾸어 임의 크기를 픽셀 단위로 확인한다."), _T("SizeMeter"), FALSE},
	{_T("테트리스"), _T("심심풀이 테트리스 게임이며 다양한 크기를 지원한다."), _T("Tetris"), FALSE},
	{_T("참조 이미지"), _T("단 하나의 이미지를 원하는 곳에 원하는 크기로 신속하게 열어 확인한다. 등록해 놓은 이미지와 위치, 크기를 유지하여 클릭 동작으로 항상 같은 위치에서 이미지를 확인할 수 있다."), _T("OneImage"), TRUE},
	{_T("폰트 열거"), _T("설치되어 있는 폰트의 목록을 보고 모양을 확인하며 특성을 조사한다. 폰트 생성 코드를 작성하여 복사한다."), _T("EnumFont"), TRUE},
	{_T("스파이"), _T("커서 위치의 윈도우 속성과 윈도우 계층 조사한다. 1초 주기로 동작하며 스파이창 안으로 들어오면 조사를 일시 중지한다."), _T("Spy"), TRUE},
	{_T("마우스 버튼 교체"), _T("좌우 마우스 버튼의 기능을 바꾼다. 오른팔이 아파 가끔 왼손으로 마우스를 쓸 때 신속하게 버튼 설정을 변경한다."), _T("SwapMouse"), TRUE},
	{_T(""), _T(""), _T(""), FALSE},
};

// Global Variables:
HINSTANCE g_hInst;									// current instance
HWND hOptionPopup;
NOTIFYICONDATA nidApp;								// Notify Icon
HMENU hPopMenu;										// Notif popup

BOOL modiSetting;									// 전역 세팅 변경 여부
BOOL modiDget;										// 위젯, 티젯 편집 여부.
BOOL bDisable = FALSE;								// keep application state
BOOL bAboutDlg = FALSE;								// About DialogBox status
const INT VERSION = 100;

CMainWnd mainWindow;
CBaseTray trayWindow;
eLang lang = eLang::KOR;
HFONT mainFont[6];
CGdiPlusStarter g_gps;
CSetting setting(TRUE);
vector<CTuingGet*> arWidget;						// running widget list
vector<CTuingJet*> arTidget;						// running tigjet list

VOID CMainWnd::Create() {
	RECT wrt;
	wrt.left = setting.get(_T("main"), _T("left"), 100);
	wrt.top = setting.get(_T("main"), _T("top"), 100);
	wrt.right = setting.get(_T("main"), _T("right"), 300);
	wrt.bottom = setting.get(_T("main"), _T("bottom"), 600);

	// 생성할 때부터 결정해야 하는 확장 스타일 적용
	alwaysOnTop = setting.get(_T("main"), _T("alwaysontop"), 1);
	dockEdge = setting.get(_T("main"), _T("dockEdge"), -1);
	dockMonitor = setting.get(_T("main"), _T("dockMonitor"), 0);

	CreateWindowEx((dockEdge == -1 ? 0 : WS_EX_TOOLWINDOW) | (alwaysOnTop ? WS_EX_TOPMOST : 0),
		_T("CBaseWnd"), _T("main"), WS_POPUP | WS_CLIPCHILDREN,
		wrt.left, wrt.top, wrt.right - wrt.left, wrt.bottom - wrt.top,
		NULL, (HMENU)NULL, g_hInst, this);
	HICON hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_TUINGMGR));
	::SetClassLong(hBaseWnd, -14, (LONG)(LONG_PTR)hIcon);
}

LRESULT CMainWnd::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	POINT pt;
	RECT crt;
	APPBARDATA abd;

	switch (iMessage) {
	case WM_NCHITTEST:
		INT nHit;
		nHit = (INT)DefWindowProc(hBaseWnd, WM_NCHITTEST, wParam, lParam);
		// 음수 좌표가 전달될 수도 있어 부호 변환 캐스팅이 반드시 필요하다.
		pt.x = (INT)(SHORT)LOWORD(lParam);
		pt.y = (INT)(SHORT)HIWORD(lParam);
		ScreenToClient(hBaseWnd, &pt);
		GetClientRect(hBaseWnd, &crt);

		if (nHit == HTCLIENT) {
			if (dockEdge == -1) {
				if (bVertical) {
					if (pt.x < bdThick) return HTLEFT;
					if (pt.x > crt.right - 1 - bdThick) return HTRIGHT;
				}
				else {
					if (pt.y < bdThick) return HTTOP;
					if (pt.y > crt.bottom - 1 - bdThick) return HTBOTTOM;
				}
				if (IsInCaption(pt.x, pt.y)) return HTCAPTION;
			}
			else {
				// 타이틀 바를 드래그할 수 없으며 도킹된 반대쪽만 크기 변경이 가능하다.
				switch (dockEdge) {
				case ABE_LEFT:
					if (pt.x > crt.right - 1 - bdThick) return HTRIGHT;
					break;
				case ABE_TOP:
					if (pt.y > crt.bottom - 1 - bdThick) return HTBOTTOM;
					break;
				case ABE_RIGHT:
					if (pt.x < bdThick) return HTLEFT;
					break;
				case ABE_BOTTOM:
					if (pt.y < bdThick) return HTTOP;
					break;
				}
			}
		}
		return nHit;
	case WM_EXITSIZEMOVE:
		if (dockEdge != -1) {
			GetWindowRect(hBaseWnd, &crt);
			INT thick = 0;
			if ((dockEdge == ABE_LEFT) || (dockEdge == ABE_RIGHT)) thick = vertThick = crt.right - crt.left;
			if ((dockEdge == ABE_TOP) || (dockEdge == ABE_BOTTOM)) thick = horzThick = crt.bottom - crt.top;
			Trace(_T("----ExitSize, thick = %d"), thick);
			AppBarSetPos(hBaseWnd, -1, dockEdge, thick, FALSE);
		}
		return FALSE;
	case WM_SETCURSOR:
		GetCursorPos(&pt);
		ScreenToClient(hBaseWnd, &pt);

		if (LOWORD(lParam) == HTCLIENT) {
			if (bVertical) {
				if (FindSplitVirt(pt.y) != -1) {
					SetCursor(cursorVert);
					return FALSE;
				}
			}
			else {
				if (FindSplitHorz(pt.x) != -1) {
					SetCursor(cursorHorz);
					return FALSE;
				}
			}
		}
		break;
		// 두께는 최소 100 ~ 640까지
	case WM_GETMINMAXINFO:
		LPMINMAXINFO pmmi;
		pmmi = (LPMINMAXINFO)lParam;
		if (bVertical) {
			pmmi->ptMinTrackSize.x = 100;
			pmmi->ptMaxTrackSize.x = 640;
		}
		else {
			pmmi->ptMinTrackSize.y = 100;
			pmmi->ptMaxTrackSize.y = 640;
		}
		return FALSE;
	case WM_WINDOWPOSCHANGING:
		if (magneticEffet) {
			LPWINDOWPOS wp = (LPWINDOWPOS)lParam;
			RECT mrt;
			// 현재 모니터의 화면 영역이 아닌 작업 영역을 기준으로 한다. 
			CopyRect(&mrt, &arMonitor[GetMonitorOfWindow(hBaseWnd)].rcWork);

			// 모니터의 벽에 달라 붙는다. 
			if (ABS(wp->x - mrt.left) < 30) wp->x = mrt.left;
			if (ABS(wp->x + wp->cx - mrt.right) < 30) wp->x = mrt.right - wp->cx;
			if (ABS(wp->y - mrt.top) < 30) wp->y = mrt.top;
			if (ABS(wp->y + wp->cy - mrt.bottom) < 30) wp->y = mrt.bottom - wp->cy;
		}
		return FALSE;
	case WM_MOVE:
		if (IsWindowVisible(hBaseWnd)) modiSetting = TRUE;
		return FALSE;
	case WM_ACTIVATE:
		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = hBaseWnd;
		SHAppBarMessage(ABM_ACTIVATE, &abd);
		break; // return FALSE; 단순히 알리기만 하므로 DefWindowProc으로도 보내야 한다. 
	case WM_WINDOWPOSCHANGED:
		abd.cbSize = sizeof(APPBARDATA);
		abd.hWnd = hBaseWnd;
		SHAppBarMessage(ABM_WINDOWPOSCHANGED, &abd);
		break;// return FALSE; 해 버리면 WM_SIZE가 가지 않음. 
	case WM_SETTINGCHANGE:
		Trace(_T("----setting changed"));
		if (wParam == SPI_SETWORKAREA && dockEdge != -1) {
			AppBarSetPos(hBaseWnd, -1, dockEdge, bVertical ? vertThick : horzThick, FALSE);
		}
		break;// return FALSE;
	case WM_MYAPPCALLBACK:
		switch (wParam) {
		case ABN_POSCHANGED:
			Trace(_T("----abn_posChanged"));
			AppBarSetPos(hBaseWnd, -1, dockEdge, bVertical ? vertThick : horzThick, TRUE);
			break;
		case ABN_FULLSCREENAPP:
			if (alwaysOnTop) {
				if (lParam == TRUE) {
					SetWindowPos(hBaseWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				}
				else {
					SetWindowPos(hBaseWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				}
			}
			break;
		}
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CMainWnd::OnCreate(WPARAM wParam, LPARAM lParam) {
	monitorCount = 0;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)NULL);
	// lang = eLang::KOR;
	INT arFontSize[] = { 12, 15, 20, 26, 32, 40 };
	for (INT i = 0; i < Count(mainFont); i++) {
		mainFont[i] = CreateFont(arFontSize[i], 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET,
			3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("맑은 고딕"));	// Fixedsys
	}

	btnClose = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_PUSH,
		0, 0, 16, 16, hBaseWnd, (HMENU)mainBTNCLOSE, g_hInst, NULL);
	SendMessage(btnClose, GBM_SETIMAGE, MAKEWPARAM(IDB_CLOSEUP, IDB_CLOSEHOT), MAKEWPARAM(IDB_CLOSEDOWN, IDB_CLOSEUP));
	btnMenu = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_PUSH,
		0, 0, 16, 16, hBaseWnd, (HMENU)mainBTNMENU, g_hInst, NULL);
	SendMessage(btnMenu, GBM_SETIMAGE, MAKEWPARAM(IDB_MENUUP, IDB_MENUHOT), MAKEWPARAM(IDB_MENUDOWN, IDB_MENUUP));

	cursorHorz = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_SIZEHORZ));
	cursorVert = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_SIZEVERT));

	bVertical = setting.get(_T("main"), _T("vertical"), 1);
	capThick = setting.get(_T("main"), _T("capThick"), 24);
	capColor = setting.get(_T("main"), _T("capColor"), GetColorFromName(_T("DodgerBlue")));
	bdThick = setting.get(_T("main"), _T("bdThick"), 4);
	bdColor = setting.get(_T("main"), _T("bdColor"), GetColorFromName(_T("Silver")));
	splitThick = setting.get(_T("main"), _T("splitThick"), 5);
	splitColor = setting.get(_T("main"), _T("splitColor"), GetColorFromName(_T("Indigo")));
	vertThick = setting.get(_T("main"), _T("vertThick"), 220);
	horzThick = setting.get(_T("main"), _T("horzThick"), 160);
	magneticEffet = setting.get(_T("main"), _T("magneticEffet"), 1);
	widgetMinThick = setting.get(_T("main"), _T("widgetMinThick"), 30);
	widgetMaxThick = setting.get(_T("main"), _T("widgetMaxThick"), 300);
	SetBackColor(setting.get(_T("main"), _T("backColor"), GetColorFromName(_T("DarkSlateGray"))));

	TCHAR key[32];
	TCHAR pack[256];
	size_t widget_num = setting.get(_T("main"), _T("widget_num"), 0);
	size_t tidget_num = setting.get(_T("main"), _T("tidget_num"), 0);
	CTuingGet* pTuingget;
	CTuingJet* pTuingjet;
	if (widget_num == 0) {
		// init Widget
		pTuingget = CTuingGet::CreateDefaultObject(_T("Clock")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("CpuUsage")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("MemUsage")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("Battery"));
		pTuingget->span = TRUE;
		arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("Calc")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("MusicPlayer")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("ImageSlide")); arWidget.push_back(pTuingget);
		pTuingget = CTuingGet::CreateDefaultObject(_T("Launcher")); arWidget.push_back(pTuingget);

		// init Tidget
		pTuingjet = CTuingJet::CreateDefaultObject(_T("LCDTest")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("ColorTable")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("TextClip")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("MoveCursor")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("OneImage")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("EnumFont")); arTidget.push_back(pTuingjet);
		pTuingjet = CTuingJet::CreateDefaultObject(_T("Spy")); arTidget.push_back(pTuingjet);
		modiDget = TRUE;
	}
	else {
		// 저장된 정보를 읽어온다. 
		for (size_t wi = 0; wi < widget_num; wi++) {
			wsprintf(key, _T("widget_%d"), static_cast<INT>(wi));
			setting.get(_T("main"), key, _T(""), pack, 256);
			pTuingget = CTuingGet::Deserialize(pack);
			arWidget.push_back(pTuingget);
		}

		for (size_t ti = 0; ti < tidget_num; ti++) {
			wsprintf(key, _T("tidget_%d"), static_cast<INT>(ti));
			setting.get(_T("main"), key, _T(""), pack, 256);
			pTuingjet = CTuingJet::Deserialize(pack);
			arTidget.push_back(pTuingjet);
		}
		modiDget = FALSE;
	}

	// 위젯 생성
	for (CTuingGet* pTuingget : arWidget) {
		pTuingget->Create(hBaseWnd);
	}

	// 트레이와 티젯 생성
	trayWindow.Create(hBaseWnd);
	trayWindow.SetBackColor(setting.get(_T("main"), _T("traybackColor"), GetColorFromName(_T("Gray"))));
	for (CTuingJet* pTuingjet : arTidget) {
		pTuingjet->Create(trayWindow.hBaseWnd);
	}

	// 도킹 상태이면 도킹 처리
	if (dockEdge != -1) {
		Docking(dockMonitor, dockEdge);
	}
	else {
		UpdateBorderEdge();
		Relayout();
		EnsureVisible(hBaseWnd);
	}

	modiSetting = FALSE;
	if (modiDget) modiSetting = TRUE;

	return FALSE;
}

LRESULT CMainWnd::OnDestroy(WPARAM wParam, LPARAM lParam) {
	if (modiSetting) {
		RECT wrt;
		GetWindowRect(hBaseWnd, &wrt);
		setting.set(_T("main"), _T("left"), wrt.left);
		setting.set(_T("main"), _T("top"), wrt.top);
		setting.set(_T("main"), _T("right"), wrt.right);
		setting.set(_T("main"), _T("bottom"), wrt.bottom);

		setting.set(_T("main"), _T("vertical"), bVertical);
		setting.set(_T("main"), _T("dockEdge"), dockEdge);
		setting.set(_T("main"), _T("dockMonitor"), dockMonitor);
		setting.set(_T("main"), _T("alwaysontop"), alwaysOnTop);
		setting.set(_T("main"), _T("capThick"), capThick);
		setting.set(_T("main"), _T("capColor"), capColor);
		setting.set(_T("main"), _T("bdThick"), bdThick);
		setting.set(_T("main"), _T("bdColor"), (INT)bdColor);
		setting.set(_T("main"), _T("splitThick"), splitThick);
		setting.set(_T("main"), _T("splitColor"), (INT)splitColor);
		setting.set(_T("main"), _T("vertThick"), vertThick);
		setting.set(_T("main"), _T("horzThick"), horzThick);
		setting.set(_T("main"), _T("widgetMinThick"), widgetMinThick);
		setting.set(_T("main"), _T("widgetMaxThick"), widgetMaxThick);
		setting.set(_T("main"), _T("backColor"), BaseBackColor);
		setting.set(_T("main"), _T("traybackColor"), trayWindow.BaseBackColor);

		setting.set(_T("main"), _T("version"), VERSION);
	}

	if (modiDget) {
		TCHAR key[32];
		TCHAR pack[256];
		setting.set(_T("main"), _T("widget_num"), static_cast<INT>(arWidget.size()));
		for (size_t wi = 0; wi < arWidget.size(); wi++) {
			wsprintf(key, _T("widget_%d"), static_cast<INT>(wi));
			arWidget[wi]->Serialize(pack);
			setting.set(_T("main"), key, pack);
		}
		setting.set(_T("main"), _T("tidget_num"), static_cast<INT>(arTidget.size()));
		for (size_t ti = 0; ti < arTidget.size(); ti++) {
			wsprintf(key, _T("tidget_%d"), static_cast<INT>(ti));
			arTidget[ti]->Serialize(pack);
			setting.set(_T("main"), key, pack);
		}

		for (int i = 0; i < Count(mainFont); i++) {
			DeleteObject(mainFont[i]);
		}
	}

	// 도킹 해제
	if (dockEdge != -1) {
		AppBarUnRegister(hBaseWnd);
	}
	PostQuitMessage(FALSE);
	return FALSE;
}

// 메인 윈도우는 차일드 사이 사이에 그려야 하기 때문에 더블버퍼링하지 않는다. 
LRESULT CMainWnd::OnPaint(WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH capBrush, bdBrush, splitBrush;
	HFONT font, oldFont;
	RECT crt;
	LPCTSTR caption = _T("TuingApp");

	GetClientRect(hBaseWnd, &crt);
	hdc = BeginPaint(hBaseWnd, &ps);

	// 도킹 상태이면 일단 배경색으로 채운다. 
	if (dockEdge != -1) {
		FillRect(hdc, 0, 0, crt.right, crt.bottom, BaseBackBrush);
	}

	// 경계선 그림. 도킹 상태면 한면만 그린다. 
	bdBrush = CreateSolidBrush(bdColor);
	capBrush = CreateSolidBrush(capColor);
	splitBrush = CreateSolidBrush(splitColor);
	SetBkMode(hdc, TRANSPARENT);
	switch (dockEdge) {
	case -1:
		FillRect(hdc, 0, 0, clientWidth, bdThick, bdBrush);
		FillRect(hdc, 0, 0, bdThick, clientHeight, bdBrush);
		FillRect(hdc, 0, clientHeight - bdThick, clientWidth, clientHeight, bdBrush);
		FillRect(hdc, clientWidth - bdThick, 0, clientWidth, clientHeight, bdBrush);
		if (bVertical) {
			FillRect(hdc, bdThick, bdThick + capThick, clientWidth - bdThick,
				clientHeight - bdThick, splitBrush);
		}
		else {
			FillRect(hdc, bdThick + capThick, bdThick, clientWidth - bdThick,
				clientHeight - bdThick, splitBrush);
		}
		break;
	case ABE_LEFT:
		// 오른쪽에만 경계선 그림
		FillRect(hdc, clientWidth - bdThick, 0, clientWidth, crt.bottom, bdBrush);
		// 스플릿바 그림
		FillRect(hdc, 0, capThick, clientWidth - bdThick, clientHeight - trayWindow.trayThick, splitBrush);
		break;
	case ABE_TOP:
		FillRect(hdc, 0, clientHeight - bdThick, crt.right, clientHeight, bdBrush);
		FillRect(hdc, capThick, 0, clientWidth - trayWindow.trayThick, clientHeight - bdThick, splitBrush);
		break;
	case ABE_RIGHT:
		FillRect(hdc, 0, 0, bdThick, crt.bottom, bdBrush);
		FillRect(hdc, bdThick, capThick, clientWidth - bdThick, clientHeight - trayWindow.trayThick, splitBrush);
		break;
	case ABE_BOTTOM:
		FillRect(hdc, 0, 0, crt.right, bdThick, bdBrush);
		FillRect(hdc, capThick, bdThick, clientWidth - trayWindow.trayThick, clientHeight, splitBrush);
		break;
	}

	if (bVertical) {
		// 타이틀 영역 그림
		FillRect(hdc, bl, bt, clientWidth - br, capThick + bt, capBrush);

		font = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_MODERN, _T("맑은 고딕"));
		oldFont = (HFONT)SelectObject(hdc, font);
		// 수직 중앙 위치에 타이틀 출력
		TextOutVCenter(hdc, bl + 4, bt + capThick / 2, caption);
	}
	else {
		FillRect(hdc, bl, bt, capThick + bl, clientHeight - bb, capBrush);

		font = CreateFont(15, 0, 900, 0, 0, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_MODERN, _T("맑은 고딕"));
		oldFont = (HFONT)SelectObject(hdc, font);
		SIZE sz;
		GetTextExtentPoint32(hdc, caption, lstrlen(caption), &sz);
		TextOutSz(hdc, bl + capThick / 2 - sz.cy / 2, clientHeight - bb - 50, caption);
	}

	DeleteObject(capBrush);
	DeleteObject(bdBrush);
	DeleteObject(splitBrush);
	SelectObject(hdc, oldFont);
	DeleteObject(font);
	EndPaint(hBaseWnd, &ps);
	return FALSE;
}

LRESULT CMainWnd::OnSize(WPARAM wParam, LPARAM lParam) {
	RECT crt;
	GetClientRect(hBaseWnd, &crt);
	Trace(_T("OnSize, %d * %d"), crt.right, crt.bottom);

	if (IsWindowVisible(hBaseWnd)) {
		modiSetting = TRUE;
	}

	if (bVertical) {
		// 실제로 바뀐 경우에만 레이아웃을 다시 잡는다. Relayout에서 크기 변경시에도 WM_SIZE가 오는데 
		// 이때는 크기가 이미 결정된 상태여서 다시 Relayout을 호출할 필요 없다. 
		if (vertThick != crt.right) {
			vertThick = crt.right;
			Relayout();
		}
	}
	else {
		if (horzThick != crt.bottom) {
			horzThick = crt.bottom;
			Relayout();
		}
	}

	return FALSE;
}

LRESULT CMainWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	POINT pt;

	pt.x = (INT)(SHORT)LOWORD(lParam);
	pt.y = (INT)(SHORT)HIWORD(lParam);

	if (bVertical) {
		dragWidget = FindSplitVirt(pt.y);
		if (dragWidget == -1) return FALSE;
		SetCapture(hBaseWnd);
	}
	else {
		dragWidget = FindSplitHorz(pt.x);
		if (dragWidget == -1) return FALSE;
		SetCapture(hBaseWnd);
	}

	return FALSE;
}

LRESULT CMainWnd::OnMouseMove(WPARAM wParam, LPARAM lParam) {
	RECT wrt;
	POINT pt;

	if (GetCapture() != hBaseWnd) {
		return FALSE;
	}

	// 경계선을 조금이라도 드래그했으면 크기가 바뀐 것으로 본다. 
	modiDget = TRUE;

	pt.x = (INT)(SHORT)LOWORD(lParam);
	pt.y = (INT)(SHORT)HIWORD(lParam);

	if (bVertical) {
		// 최소, 최대 높이 적용
		GetWindowRect(arWidget[dragWidget]->hBaseWnd, &wrt);
		ScreenToClient(mainWindow.hBaseWnd, (LPPOINT)&wrt.left);
		INT newHeight = min(max(pt.y - wrt.top, widgetMinThick), widgetMaxThick);

		// 드래그중인 위젯의 높이 조정. 현재 높이와 설정상의 높이 같이 조정한다.
		arWidget[dragWidget]->vertHeight = newHeight;
	}
	else {
		// 최소, 최대 폭 적용
		GetWindowRect(arWidget[dragWidget]->hBaseWnd, &wrt);
		ScreenToClient(mainWindow.hBaseWnd, (LPPOINT)&wrt.left);
		INT newWidth = min(max(pt.x - wrt.left, widgetMinThick), widgetMaxThick);

		// 드래그중인 위젯의 높이 조정
		//arWidget[dragWidget].pTuingget->win_w = newWidth;
		arWidget[dragWidget]->horzWidth = newWidth;
	}

	// 레이아웃 다시 계산
	Relayout();
	// 메인 윈도우 즉시 다시 그림
	InvalidateRect(hBaseWnd, NULL, FALSE);
	UpdateWindow(hBaseWnd);
	return FALSE;
}

LRESULT CMainWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam) {
	if (GetCapture() == hBaseWnd) {
		dragWidget = -1;
		ReleaseCapture();
	}
	return FALSE;
}

LRESULT CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case mainBTNCLOSE:
		DestroyWindow(hBaseWnd);
		mainWindow.Dispose();
		break;
	case mainBTNMENU:
		HMENU hMenu, hPopup;
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUPMENU));
		hPopup = GetSubMenu(hMenu, 0);

		if (alwaysOnTop) {
			CheckMenuItem(hPopup, IDM_ALWAYSONTOP, MF_BYCOMMAND | MF_CHECKED);
		}

		switch (dockEdge) {
		case ABE_LEFT:
			CheckMenuItem(hPopup, IDM_DOCKLEFT, MF_BYCOMMAND | MF_CHECKED);
			break;
		case ABE_TOP:
			CheckMenuItem(hPopup, IDM_DOCKTOP, MF_BYCOMMAND | MF_CHECKED);
			break;
		case ABE_RIGHT:
			CheckMenuItem(hPopup, IDM_DOCKRIGHT, MF_BYCOMMAND | MF_CHECKED);
			break;
		case ABE_BOTTOM:
			CheckMenuItem(hPopup, IDM_DOCKBOTTOM, MF_BYCOMMAND | MF_CHECKED);
			break;
		case -1:
			CheckMenuItem(hPopup, IDM_UNDOCK, MF_BYCOMMAND | MF_CHECKED);
			break;
		}

		// 도킹 상태이면 회전 명령은 사용할 수 없다. 
		if (dockEdge != -1) {
			EnableMenuItem(hPopup, IDM_ROTATE, MF_BYCOMMAND | MF_DISABLED);
		}

		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, pt.x, pt.y, 0, hBaseWnd, NULL);
		DestroyMenu(hMenu);
		break;
	case IDM_ALWAYSONTOP:
		modiSetting = TRUE;
		alwaysOnTop = !alwaysOnTop;
		if (alwaysOnTop) {
			SetWindowPos(hBaseWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			SetWindowPos(hBaseWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		break;
	case IDM_ROTATE:
		modiSetting = TRUE;
		bVertical = !bVertical;
		Relayout();
		break;
	case IDM_ABOUT:
		MessageBox(hBaseWnd, _T("여러 가지 간단한 위젯을 하나로 묶어 놓은 패키지 프로그램이다.\r\n")
			_T("누구나 무료로 쓸 수 있되 맘에 들면 기부할 수 있는 도네이션웨어이다.\r\n")
			_T("(단, 조기봉, 조종권, 박요한은 일단 기부부터 해야 함)"),
			_T("TuingApp"), MB_OK);
		return FALSE;
	case IDM_SETTING:
		modiSetting = TRUE;
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MAIN), hBaseWnd, MainDlgProc);
		break;
	case IDM_HELP:
		RECT wrt;
		GetWindowRect(hBaseWnd, &wrt);
		Trace(_T("ABSetPos-After %d,%d-%d,%d"), wrt.left, wrt.top, wrt.right, wrt.bottom);
		MessageBox(hBaseWnd, _T("도움말은 차후 기능이 어느 정도 완성된 후에 제공할 예정입니다.\r\n"),
			_T("TuingApp"), MB_OK);
		MoveWindow(hBaseWnd, 0, 0, 200, 1000, TRUE);
		break;
	case IDM_SELWIDGET:
		modiDget = TRUE;
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SELWIDGET), hBaseWnd, SelWidgetDlgProc);
		break;
	case IDM_SELTIDGET:
		modiDget = TRUE;
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SELTIDGET), hBaseWnd, SelTidgetDlgProc);
		break;
	case IDM_DOCKLEFT:
		Docking(-1, ABE_LEFT);
		break;
	case IDM_DOCKTOP:
		Docking(-1, ABE_TOP);
		break;
	case IDM_DOCKRIGHT:
		Docking(-1, ABE_RIGHT);
		break;
	case IDM_DOCKBOTTOM:
		Docking(-1, ABE_BOTTOM);
		break;
	case IDM_UNDOCK:
		UnDocking();
		break;
	}

	return FALSE;
}

VOID CMainWnd::Docking(INT nMonitor, UINT edge) {
	// 같은 면에 재도킹하지 않되 윈도우 초기화 중에 처음 도킹할 때는 조건을 볼 필요 없이 무조건 도킹한다.
	if (IsWindowVisible(hBaseWnd)) {
		if (dockEdge == edge) return;

		// 도킹 해제 상태에서 도킹할 때는 폭과 높이를 경계선만큼 뺀다. 그래야 위젯 넓이에 변화가 없다.
		// 예를 들어 폭 100, 경계선 4일 때 도킹 해제 상태에서 위젯폭은 100 - 4 * 2 = 92이다.
		// 이 상태에서 좌우 좌우로 도킹하면 한쪽 경계선이 없어 위젯폭이 96이 되어 레이아웃이 틀어진다.
		// 윈도우 폭이 달라지더라도 위젯 레이아웃을 유지하기 위해 메인의 높이를 조정한다.
		// 단, 도킹 상태에서 다른 도킹 상태로 바꿀 때는 그럴 필요 없다. 
		if (dockEdge == -1) {
			vertThick -= bdThick;
			horzThick -= bdThick;
		}
	}

	dockEdge = edge;
	UpdateBorderEdge();

	// 도킹면에 따라 방향 자동 결정
	INT thick;
	if (dockEdge == ABE_LEFT || dockEdge == ABE_RIGHT) {
		bVertical = TRUE;
		thick = vertThick;
	}
	else {
		bVertical = FALSE;
		thick = horzThick;
	}
	AppBarRegister(hBaseWnd, WM_MYAPPCALLBACK);
	// 모니터가 강제 조정되었으면 갱신한다. 
	dockMonitor = AppBarSetPos(hBaseWnd, nMonitor, dockEdge, thick, TRUE);
	Relayout();

	// 도킹시는 TOOLWINDOW 확장 스타일을 주어야 한다. 그렇지 않으면 영역 재계산시 바탕 화면 안쪽으로 들어와 버린다. 
	SetWindowLongPtr(hBaseWnd, GWL_EXSTYLE, GetWindowLongPtr(hBaseWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);

	modiSetting = TRUE;
}

VOID CMainWnd::UnDocking() {
	if (dockEdge == -1) return;

	// 도킹 상태에서 해제하면 경계선이 하나 더 늘어나므로 위젯폭 유지를 위해 메인 윈도우의 폭을 늘려준다. 
	vertThick += bdThick;
	horzThick += bdThick;

	// 현재 도킹 방향에 맞게 방향 설정하고 도킹을 해제한다. 
	if (dockEdge == ABE_LEFT || dockEdge == ABE_RIGHT) {
		bVertical = TRUE;
	}
	else {
		bVertical = FALSE;
	}
	dockEdge = -1;
	UpdateBorderEdge();
	AppBarUnRegister(hBaseWnd);
	Relayout();

	// 도킹을 풀면 TOOLWINDOW 속성을 해제하여 타스크 바에 표시한다. 
	SetWindowLongPtr(hBaseWnd, GWL_EXSTYLE, GetWindowLongPtr(hBaseWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);

	modiSetting = TRUE;
}

VOID CMainWnd::UpdateBorderEdge() {
	bl = bt = br = bb = 0;
	switch (dockEdge) {
	case -1:
		bl = bt = br = bb = bdThick;
		break;
	case ABE_LEFT:
		br = bdThick;
		break;
	case ABE_TOP:
		bb = bdThick;
		break;
	case ABE_RIGHT:
		bl = bdThick;
		break;
	case ABE_BOTTOM:
		bt = bdThick;
		break;
	}
}

VOID CMainWnd::Relayout() {
	INT span_num;
	INT totalPercent;
	INT remainWidth, remainHeight;

	if (bVertical) {
		// 영역의 폭. 경계선 포함
		clientWidth = vertThick;
		// 경계선을 제외한 위젯폭
		int widgetWidth = clientWidth - (bl + br);

		// 타이틀바의 수직 중앙 위치에 닫기, 메뉴 버튼 배치. 
		MoveWindow(btnClose, bl + widgetWidth - 22, bt + capThick / 2 - 8, 16, 16, TRUE);
		UpdateWindow(btnClose);
		MoveWindow(btnMenu, bl + widgetWidth - 42, bt + capThick / 2 - 8, 16, 16, TRUE);
		UpdateWindow(btnMenu);

		// 모든 위젯의 위치를 다시 잡는다.
		INT y = bt + capThick;
		for (size_t wi = 0; wi < arWidget.size(); wi++) {
			// 일단 대입받은 후 사용한다. 컬렉션의 [ ] 연산자를 쓰면 디버깅중에 값을 볼 수 없다. 
			CTuingGet* pTuingget = arWidget[wi];
			// 숨겨진 위젯은 건너 뛴다. 병합된 위젯은 루프 내부에서 이미 배치했으므로 건너 뛴다.
			if (pTuingget->baseget_visible == FALSE || arWidget[wi]->span) continue;

			// 병합 개수 조사
			span_num = GetSpanNum(static_cast<INT>(wi));

			// 혼자이면 자기 높이만큼 차지한다.
			if (span_num == 1) {
				MoveWindow(pTuingget->hBaseWnd, bl, y, widgetWidth, pTuingget->vertHeight, TRUE);
				// 이동 후 즉시 그려야 깜박임이 없다. 아래, 위로 순서를 바꿀 때는 무효화도 해야 바로 그려진다. 
				InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);
				UpdateWindow(pTuingget->hBaseWnd);
			}
			else {
				// 병합셀이면 선두 위젯(pTuingget)의 높이만큼만 차지하고 나머지는 수평으로 분할 배치한다.
				// 비율의 총합을 구한다. spanRatio가 양수이면 비율, 음수이면 절대 픽셀이다.
				totalPercent = 0;
				// 위젯폭에서 중간 중간의 경계선 폭을 빼 위젯이 차지할 너비를 계산한다. 
				remainWidth = widgetWidth - (span_num - 1) * splitThick;
				for (size_t gi = wi; gi < wi + span_num; gi++) {
					if (arWidget[gi]->spanRatio > 0) {
						// 분할을 위한 총 비율 계산
						totalPercent += arWidget[gi]->spanRatio;
					}
					else {
						// 절대폭 위젯의 너비만큼 남은 폭 제외
						remainWidth -= -arWidget[gi]->spanRatio;
					}
				}

				INT x = bl;
				INT w;
				// 그룹내의 위젯을 수평으로 나열한다. 
				for (size_t gi = wi; gi < wi + span_num; gi++) {
					// 양수는 남는 폭의 비율만큼 폭을 차지한다.
					if (arWidget[gi]->spanRatio > 0) {
						w = remainWidth * arWidget[gi]->spanRatio / totalPercent;
						// 음수는 절대 픽셀 크기만큼 차지한다.
					}
					else {
						w = -arWidget[gi]->spanRatio;
					}
					// 마지막 위젯은 나머지를 다 차지하되 음수일 수는 없다. 비율 나누기에 의한 1 모자람 방지
					if (gi == wi + span_num - 1) {
						w = clientWidth - x - br;
						if (w < 0) w = 0;
					}
					MoveWindow(arWidget[gi]->hBaseWnd, x, y, w, arWidget[wi]->vertHeight, TRUE);
					InvalidateRect(arWidget[gi]->hBaseWnd, NULL, TRUE);
					UpdateWindow(arWidget[gi]->hBaseWnd);

					x += (w + splitThick);
				}
			}

			y += pTuingget->vertHeight + splitThick;
		}

		// 트레이 윈도우의 위치 지정 및 티젯 정렬
		trayWindow.UpdateTrayThick(widgetWidth);
		if (dockEdge == -1) {
			SetWindowPos(trayWindow.hBaseWnd, NULL, bl, y, widgetWidth,
				trayWindow.trayThick, SWP_NOZORDER);
		}
		else {
			// 윈도우의 높이를 다 쓰고 하단에 트레이를 배치한다. 
			RECT crt;
			GetClientRect(hBaseWnd, &crt);
			SetWindowPos(trayWindow.hBaseWnd, NULL, bl, crt.bottom - trayWindow.trayThick - bb,
				widgetWidth, trayWindow.trayThick, SWP_NOZORDER);
		}
		InvalidateRect(trayWindow.hBaseWnd, NULL, FALSE);
		UpdateWindow(trayWindow.hBaseWnd);
		trayWindow.ArrangeTrayTidget(widgetWidth);

		// 영역의 높이 결정. 모든 위젯의 높이 + 트레이 + 아래 경계선 두께
		clientHeight = y + trayWindow.trayThick + bb;
	}
	else {
		// 수직 배열과 모든 논리는 같되 방향만 다르다. 
		clientHeight = horzThick;
		INT widgetHeight = clientHeight - (bt + bb);

		MoveWindow(btnClose, bl + capThick / 2 - 8, bt + widgetHeight - 16 - 4, 16, 16, TRUE);
		UpdateWindow(btnClose);
		MoveWindow(btnMenu, bl + capThick / 2 - 8, bt + widgetHeight - 16 - 4 - 20, 16, 16, TRUE);
		UpdateWindow(btnMenu);

		INT x = bl + capThick;
		for (size_t wi = 0; wi < arWidget.size(); wi++) {
			CTuingGet* pTuingget = arWidget[wi];
			if (pTuingget->baseget_visible == FALSE || arWidget[wi]->span) continue;

			span_num = GetSpanNum(static_cast<INT>(wi));

			if (span_num == 1) {
				MoveWindow(pTuingget->hBaseWnd, x, bt, pTuingget->horzWidth, widgetHeight, TRUE);
				InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);
				UpdateWindow(pTuingget->hBaseWnd);
			}
			else {
				totalPercent = 0;
				remainHeight = widgetHeight - (span_num - 1) * splitThick;
				for (size_t gi = wi; gi < wi + span_num; gi++) {
					if (arWidget[gi]->spanRatio > 0) {
						totalPercent += arWidget[gi]->spanRatio;
					}
					else {
						remainHeight -= -arWidget[gi]->spanRatio;
					}
				}

				INT y = bt;
				INT h;
				for (size_t gi = wi; gi < wi + span_num; gi++) {
					if (arWidget[gi]->spanRatio > 0) {
						h = remainHeight * arWidget[gi]->spanRatio / totalPercent;
					}
					else {
						h = -arWidget[gi]->spanRatio;
					}
					if (gi == wi + span_num - 1) {
						h = clientHeight - y - bb;
						if (h < 0) h = 0;
					}
					MoveWindow(arWidget[gi]->hBaseWnd, x, y, arWidget[wi]->horzWidth, h, TRUE);
					InvalidateRect(arWidget[gi]->hBaseWnd, NULL, TRUE);
					UpdateWindow(arWidget[gi]->hBaseWnd);

					y += (h + splitThick);
				}
			}

			x += pTuingget->horzWidth + splitThick;
		}

		trayWindow.UpdateTrayThick(widgetHeight);
		if (dockEdge == -1) {
			SetWindowPos(trayWindow.hBaseWnd, NULL, x, bt, trayWindow.trayThick,
				widgetHeight, SWP_NOZORDER);
		}
		else {
			RECT crt;
			GetClientRect(hBaseWnd, &crt);
			SetWindowPos(trayWindow.hBaseWnd, NULL, crt.right - trayWindow.trayThick - br, bt, trayWindow.trayThick,
				widgetHeight, SWP_NOZORDER);
		}
		InvalidateRect(trayWindow.hBaseWnd, NULL, FALSE);
		UpdateWindow(trayWindow.hBaseWnd);
		trayWindow.ArrangeTrayTidget(widgetHeight);

		clientWidth = x + trayWindow.trayThick + br;
	}

	// 메인 윈도우의 크기를 변경한다. 트레이의 높이가 바뀔 수 있어 메인도 크기가 바뀔 수 있다. 
	if (dockEdge == -1) {
		Trace(_T("크기 조정 %d * %d"), clientWidth, clientHeight);
		SetWindowPos(mainWindow.hBaseWnd, NULL, 0, 0, clientWidth, clientHeight,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	InvalidateRect(mainWindow.hBaseWnd, NULL, FALSE);
}

BOOL CMainWnd::IsInCaption(INT cx, INT cy) {
	if (bVertical) {
		if (cy > bdThick && cy <= bdThick + capThick) return TRUE;
	}
	else {
		if (cx > bdThick && cx <= bdThick + capThick) return TRUE;
	}
	return FALSE;
}

// 몇번째 위젯 스플릿터 아래에 있는지 조사한다. 아니면 -1 리턴
INT CMainWnd::FindSplitVirt(INT cy) {
	// 수직 도킹 상태이면 위쪽 경계선이 없다. 
	INT y = bt + capThick;
	for (size_t wi = 0; wi < arWidget.size(); wi++) {
		if (arWidget[wi]->baseget_visible == FALSE || arWidget[wi]->span) continue;
		y += arWidget[wi]->vertHeight;
		if (cy < y) return -1;
		// 스플릿터 사이에 있으면 수직 조정 커서
		if (cy >= y && cy <= y + splitThick) return static_cast<INT>(wi);
		y += splitThick;
	}

	return -1;
}

INT CMainWnd::FindSplitHorz(INT cx) {
	INT x = bl + capThick;
	for (size_t wi = 0; wi < arWidget.size(); wi++) {
		if (arWidget[wi]->baseget_visible == FALSE || arWidget[wi]->span) continue;
		x += arWidget[wi]->horzWidth;
		if (cx < x) return -1;
		if (cx >= x && cx <= x + splitThick) return static_cast<INT>(wi);
		x += splitThick;
	}

	return -1;
}

// 병합 개수 조사. 자신은 일단 포함하고 이후 위젯의 span속성이 true인 개수를 센다.
INT CMainWnd::GetSpanNum(INT wi) {
	INT num = 1;
	for (size_t gi = wi + 1; gi < arWidget.size(); gi++) {
		if (arWidget[gi]->span) {
			num++;
		}
		else {
			break;
		}
	}

	return num;
}

INT_PTR CALLBACK CMainWnd::SelWidgetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	wstring name;
	wstring cls;
	wstring id;
	TCHAR list_name[64];
	INT idx;
	INT idx_dget;
	INT idx_use;
	INT cidx;
	INT count;
	INT value;
	static BOOL editByCode = FALSE;
	LPMEASUREITEMSTRUCT lpmis;
	static sNamedColor arWidgetBackColor[] = {
		{_T("Pink"), 0}, {_T("Khaki"), 0},{_T("LightYellow"), 0},{_T("PapayaWhip"), 0},{_T("Moccasin"), 0},
		{_T("PaleTurquoise"), 0}, {_T("LightBlue"), 0}, {_T("LightSkyBlue"), 0},{_T("SpringGreen"), 0},
		{_T("MediumAquaMarine"), 0},{_T("Turquoise"), 0},{_T("Lavender"), 0},{_T("Thistle"), 0},
		{_T("Violet"), 0},{_T("HoneyDew"), 0},{_T("Beige"), 0}, {_T("Ivory"), 0}, {_T("CornSilk"), 0},
		{_T("Bisque"), 0},{_T("Wheat"), 0},{_T("Gainsboro"), 0},{_T("LightGrey"), 0}, {_T("Silver"), 0},
		{_T("DarkGray"), 0},
	};
	RECT wrt;
	INT infoidx;

	idx_use = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETCURSEL, 0, 0));

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(mainWindow.hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		// 표준 색상표에서 밝은 파스텔 계통으로 선별함. 
		if (arWidgetBackColor[0].color == 0) {
			for (sNamedColor& nc : arWidgetBackColor) {
				nc.color = GetColorFromName(nc.name);
			}
		}

		// 색상수만큼 콤보 박스에 채운다.
		for (INT i = 0; i < Count(arWidgetBackColor); i++) {
			SendDlgItemMessage(hDlg, IDC_CBBACKCOLOR, CB_ADDSTRING, 0, 0);
		}

		// 사용 가능한 위젯 목록 출력. 
		for (INT wi = 0; wi < Count(arWidgetInfo); wi++) {
			// 이름과  배열상의 첨자를 저장한다. 항목 데이터에 포인터를 저장하는 것은 위험하다. 
			// 배열상 첨자와 리스트 박스의 첨자는 달라질 수 있어 따로 저장해야 한다. 
			idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_ADDSTRING, 0, (LPARAM)arWidgetInfo[wi].name.c_str()));
			SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_SETITEMDATA, idx, (LPARAM)wi);
		}

		// 현재 사용중인 위젯 목록 출력. 이름과 실제 id를 같이 저장. 인스턴스간의 구분을 위한 id가 필요하다. 
		// 첨자는 삭제에 의해 무효해져서 안되며 항목 데이터에 포인터 할당해서 저장하는 것은 번거롭다. 
		for (size_t wi = 0; wi < arWidget.size(); wi++) {
			infoidx = FindWidgetClsIdx(arWidget[wi]->baseget_cls.c_str());
			name = arWidgetInfo[infoidx].name;
			// 멀티 위젯인 경우만 ID 붙여서 출력
			if (arWidget[wi]->baseget_id.find('_') != string::npos) {
				name += _T("(") + arWidget[wi]->baseget_id + _T(")");
			}
			SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_ADDSTRING, 0, (LPARAM)name.c_str());
		}

		// 스핀 컨트롤의 범위 설정
		SendDlgItemMessage(hDlg, IDC_SPINHEIGHT, UDM_SETRANGE, 0,
			MAKELPARAM(mainWindow.widgetMaxThick, mainWindow.widgetMinThick));
		SendDlgItemMessage(hDlg, IDC_SPINWIDTH, UDM_SETRANGE, 0,
			MAKELPARAM(mainWindow.widgetMaxThick, mainWindow.widgetMinThick));
		SendDlgItemMessage(hDlg, IDC_SPINSPAN, UDM_SETRANGE, 0,
			MAKELPARAM(mainWindow.widgetMaxThick, 1));

		// 두 리스트 박스 모두 첫 번째 항목을 선택해 준다. 
		SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_SETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_SETCURSEL, 0, 0);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LIWIDGET, LBN_SELCHANGE), NULL);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LIUSEWIDGET, LBN_SELCHANGE), NULL);
		return TRUE;
	case WM_MEASUREITEM:
		lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight = 18;
		return TRUE;
	case WM_DRAWITEM:
		if (wParam == IDC_CBBACKCOLOR) {
			DrawOwnerItem(lParam, arWidgetBackColor);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTNADDWIDGET:
			idx_dget = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_GETCURSEL, 0, 0));
			if (idx_dget != -1) {
				// 추가할 위젯의 정보를 구한다. 
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_GETITEMDATA, idx_dget, 0));
				cls = arWidgetInfo[idx].cls;

				// 복수 비허용일 경우 이미 사용중인지 조사한다. 이때는 숨겨진 위젯까지 포함한다.
				if (arWidgetInfo[idx].allowMulti == FALSE) {
					BOOL bFind = FALSE;
					for (size_t wi = 0; wi < arWidget.size(); wi++) {
						if (arWidget[wi]->baseget_cls == cls) {
							bFind = TRUE;
							break;
						}
					}
					if (bFind) {
						MessageBox(hDlg, _T("이 위젯은 여러 개 사용할 수 없거나 그럴 필요가 없습니다."),
							_T("알림"), MB_OK);
						return TRUE;
					}
				}

				// 새 위젯을 추가한다. 
				CTuingGet* pTuingget = CTuingGet::CreateDefaultObject(cls.c_str());
				arWidget.push_back(pTuingget);
				pTuingget->Create(mainWindow.hBaseWnd);

				// 새 위젯을 사용 위젯 리스트 박스에 추가
				infoidx = FindWidgetClsIdx(pTuingget->baseget_cls.c_str());
				name = arWidgetInfo[infoidx].name;
				if (pTuingget->baseget_id.find('_') != string::npos) {
					name += _T("(") + pTuingget->baseget_id + _T(")");
				}
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_ADDSTRING, 0, (LPARAM)name.c_str()));
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_SETCURSEL, idx, 0);
				SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LIUSEWIDGET, LBN_SELCHANGE), NULL);

				// 새로 생성한 위젯을 화면에 바로 출력한다. 
				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_BTNREMOVEWIDGET:
			if (idx_use != -1) {
				if (MessageBox(hDlg, _T("위젯 제거시 모든 정보도 같이 삭제됩니다. 제거하시겠습니까?"),
					_T("확인"), MB_YESNO) == IDNO) {
					return TRUE;
				}
				// 삭제할 위젯의 id를 구한다. 괄호가 없으면 cls가 id이며 있으면 괄호안의 id를 읽는다.
				memset(list_name, 0, sizeof(name));
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				name = list_name;
				size_t pos = name.find('(');
				if (pos == string::npos) {
					infoidx = FindWidgetNameIdx(list_name);
					id = arWidgetInfo[infoidx].cls;
				}
				else {
					id = name.substr(pos + 1, name.find(')'));
				}

				// 위젯 윈도우 파괴. 위젯 객체도 제거하고 벡터에서도 제거
				DestroyWindow(arWidget[idx_use]->hBaseWnd);
				arWidget[idx_use]->Dispose();
				arWidget.erase(arWidget.begin() + idx_use);

				// 리스트 박스에서 제거하고 다음 위젯 선택해 줌. 끝 위젯이면 직전 위젯 선택
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_DELETESTRING, idx_use, 0);
				count = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETCOUNT, 0, 0));
				if (idx_use == count) idx_use--;
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_SETCURSEL, idx_use, 0);

				// 삭제되는 위젯의 이전 정보 제거. DestroyWindow에서 정보를 기록하므로 파괴 후 제거
				// 이전 정보를 제거하지 않으면 다음 추가될 위젯이 id를 재활용하여 쓰레기 정보를 읽는다.
				setting.deleteSection(id);
				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_BTNUPWIDGET:
			// 제일 위에 있으면 더 위로 올릴 수 없다. 
			if (idx_use != -1 && idx_use != 0) {
				// 아래, 위 위젯 교환.
				CTuingGet* t = arWidget[idx_use];
				arWidget[idx_use] = arWidget[idx_use - 1];
				arWidget[idx_use - 1] = t;

				// 리스트 박스의 현재 항목 백업 후 삭제
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_DELETESTRING, idx_use, 0);

				// 바로 위에 다시 삽입하고 포커스를 준다. 
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_INSERTSTRING, idx_use - 1, (LPARAM)list_name));
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_SETCURSEL, idx, 0);

				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_BTNDOWNWIDGET:
			count = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETCOUNT, 0, 0));
			if (idx_use != -1 && idx_use != count - 1) {
				CTuingGet* t = arWidget[idx_use];
				arWidget[idx_use] = arWidget[idx_use + 1];
				arWidget[idx_use + 1] = t;

				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_DELETESTRING, idx_use, 0);

				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_INSERTSTRING, idx_use + 1, (LPARAM)list_name));
				SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_SETCURSEL, idx, 0);

				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_LIWIDGET:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				idx_dget = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_GETCURSEL, 0, 0));
				if (idx_dget != -1) {
					idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIWIDGET, LB_GETITEMDATA, idx_dget, 0));
					SetDlgItemText(hDlg, IDC_EDWIDGETDESC, arWidgetInfo[idx].desc.c_str());
				}
				return TRUE;
			}
			break;
		case IDC_LIUSEWIDGET:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				if (idx_use != -1) {
					CheckDlgButton(hDlg, IDC_CHKHIDEWIDGET,
						arWidget[idx_use]->baseget_visible ? BST_UNCHECKED : BST_CHECKED);
					editByCode = TRUE;
					SetDlgItemInt(hDlg, IDC_EDHEIGHT, arWidget[idx_use]->vertHeight, FALSE);
					SetDlgItemInt(hDlg, IDC_EDWIDTH, arWidget[idx_use]->horzWidth, FALSE);

					// 양수면 상대 비율, 음수면 절대 픽셀
					CheckDlgButton(hDlg, IDC_CHKSPAN,
						arWidget[idx_use]->span ? BST_CHECKED : BST_UNCHECKED);
					CheckRadioButton(hDlg, IDC_RASPANRATIO, IDC_RASPANPIXEL,
						arWidget[idx_use]->spanRatio > 0 ? IDC_RASPANRATIO : IDC_RASPANPIXEL);
					SetDlgItemInt(hDlg, IDC_EDSPAN, ABS(arWidget[idx_use]->spanRatio), FALSE);
					editByCode = FALSE;

					// 위젯의 색상을 선택한다. 색상표에 없으면 0번을 선택한다.
					cidx = 0;
					for (INT i = 0; i < Count(arWidgetBackColor); i++) {
						if (arWidget[idx_use]->BaseBackColor == arWidgetBackColor[i].color) {
							cidx = i;
							break;
						}
					}
					SendDlgItemMessage(hDlg, IDC_CBBACKCOLOR, CB_SETCURSEL, cidx, 0);
				}
				return TRUE;
			}
			break;
		case IDC_CHKHIDEWIDGET:
			if (idx_use != -1) {
				arWidget[idx_use]->baseget_visible =
					(IsDlgButtonChecked(hDlg, IDC_CHKHIDEWIDGET) == BST_UNCHECKED);
				ShowWindow(arWidget[idx_use]->hBaseWnd,
					arWidget[idx_use]->baseget_visible ? SW_SHOW : SW_HIDE);
				// 위젯을 숨기면 분할에는 참여할 수 없다. 만약 선두 위젯을 숨기면 그룹 전체를 건너뛴다. 
				if (arWidget[idx_use]->baseget_visible == FALSE) {
					arWidget[idx_use]->span = FALSE;
					CheckDlgButton(hDlg, IDC_CHKSPAN, BST_UNCHECKED);
				}
				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_EDHEIGHT:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				// 코드에서 값을 변경할 때는 이벤트 처리할 필요 없다. 
				if (editByCode) return TRUE;
				value = GetDlgItemInt(hDlg, IDC_EDHEIGHT, NULL, FALSE);
				value = min(max(value, mainWindow.widgetMinThick), mainWindow.widgetMaxThick);
				if (idx_use != -1) {
					// 설정은 즉시 변경하고 현재 높이는 방향이 수직일 때만 변경한다.
					arWidget[idx_use]->vertHeight = value;
					if (mainWindow.bVertical) {
						arWidget[idx_use]->vertHeight = value;
						SetTimer(hDlg, 1, 800, NULL);
					}
					return TRUE;
				}
			}
			break;
		case IDC_EDWIDTH:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (editByCode) return TRUE;
				value = GetDlgItemInt(hDlg, IDC_EDWIDTH, NULL, FALSE);
				value = min(max(value, mainWindow.widgetMinThick), mainWindow.widgetMaxThick);
				if (idx_use != -1) {
					arWidget[idx_use]->horzWidth = value;
					if (mainWindow.bVertical == FALSE) {
						arWidget[idx_use]->horzWidth = value;
						SetTimer(hDlg, 1, 800, NULL);
					}
					return TRUE;
				}
			}
			break;
		case IDC_CHKSPAN:
			if (idx_use != -1) {
				arWidget[idx_use]->span = (IsDlgButtonChecked(hDlg, IDC_CHKSPAN) == BST_CHECKED);
				SetTimer(hDlg, 1, 800, NULL);
			}
			return TRUE;
		case IDC_EDSPAN:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (editByCode) return TRUE;
				value = GetDlgItemInt(hDlg, IDC_EDSPAN, NULL, FALSE);
				if (idx_use != -1) {
					if (IsDlgButtonChecked(hDlg, IDC_RASPANRATIO) == BST_CHECKED) {
						arWidget[idx_use]->spanRatio = value;
					}
					else {
						arWidget[idx_use]->spanRatio = -value;
					}
					SetTimer(hDlg, 1, 800, NULL);
				}
				return TRUE;
			}
			break;
		case IDC_RASPANRATIO:
			if (idx_use != -1) {
				value = GetDlgItemInt(hDlg, IDC_EDSPAN, NULL, FALSE);
				arWidget[idx_use]->spanRatio = value;
				SetTimer(hDlg, 1, 800, NULL);
			}
			return TRUE;
		case IDC_RASPANPIXEL:
			if (idx_use != -1) {
				value = GetDlgItemInt(hDlg, IDC_EDSPAN, NULL, FALSE);
				arWidget[idx_use]->spanRatio = -value;
				SetTimer(hDlg, 1, 800, NULL);
			}
			return TRUE;
		case IDC_CBBACKCOLOR:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, LOWORD(wParam), CB_GETCURSEL, 0, 0));
				idx_use = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSEWIDGET, LB_GETCURSEL, 0, 0));
				if (idx_use != -1) {
					arWidget[idx_use]->SetBackColor(arWidgetBackColor[idx].color);
					InvalidateRect(arWidget[idx_use]->hBaseWnd, NULL, TRUE);
				}
				break;
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}	// end WM_COMMAND
		break;
		// 일회용 타이머. 에디트 편집시 즉시 처리하지 말고 0.8초간 시차를 두고 레이아웃에 반영한다.
	case WM_TIMER:
		if (wParam == 1) {
			KillTimer(hDlg, 1);
			mainWindow.Relayout();
		}
	}
	return FALSE;
}

INT_PTR CALLBACK CMainWnd::SelTidgetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	wstring name;
	wstring cls;
	wstring id;
	TCHAR list_name[64];
	INT idx;
	INT idx_dget;
	INT idx_use;
	INT count;
	RECT wrt;
	INT infoidx;

	idx_use = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETCURSEL, 0, 0));

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(mainWindow.hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		// 사용 가능한 티젯 목록 출력. 
		for (INT ti = 0; ti < Count(arTidgetInfo); ti++) {
			idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_ADDSTRING, 0, (LPARAM)arTidgetInfo[ti].name.c_str()));
			SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_SETITEMDATA, idx, (LPARAM)ti);
		}

		for (size_t ti = 0; ti < arTidget.size(); ti++) {
			infoidx = FindTidgetClsIdx(arTidget[ti]->baseget_cls.c_str());
			name = arTidgetInfo[infoidx].name;
			if (arTidget[ti]->baseget_id.find('_') != string::npos) {
				name += _T("(") + arTidget[ti]->baseget_id + _T(")");
			}
			SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_ADDSTRING, 0, (LPARAM)name.c_str());
		}

		// 두 리스트 박스 모두 첫 번째 항목을 선택해 준다. 
		SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_SETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_SETCURSEL, 0, 0);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LITIDGET, LBN_SELCHANGE), NULL);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LIUSETIDGET, LBN_SELCHANGE), NULL);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTNADDTIDGET:
			idx_dget = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_GETCURSEL, 0, 0));
			if (idx_dget != -1) {
				// 추가할 위젯의 정보를 구한다. 
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_GETITEMDATA, idx_dget, 0));
				cls = arTidgetInfo[idx].cls;

				// 복수 비허용일 경우 이미 사용중인지 조사한다. 이때는 숨겨진 위젯까지 포함한다.
				if (arTidgetInfo[idx].allowMulti == FALSE) {
					BOOL bFind = FALSE;
					for (size_t ti = 0; ti < arTidget.size(); ti++) {
						if (arTidget[ti]->baseget_cls == cls) {
							bFind = TRUE;
							break;
						}
					}
					if (bFind) {
						MessageBox(hDlg, _T("이 티젯은 여러 개 사용할 수 없거나 그럴 필요가 없습니다."),
							_T("알림"), MB_OK);
						return TRUE;
					}
				}

				// 새 티젯을 추가한다. 
				CTuingJet* pTuingjet = CTuingJet::CreateDefaultObject(cls.c_str());
				arTidget.push_back(pTuingjet);
				pTuingjet->Create(trayWindow.hBaseWnd);

				// 새 티젯을 사용 티젯 리스트 박스에 추가
				infoidx = FindTidgetClsIdx(pTuingjet->baseget_cls.c_str());
				name = arTidgetInfo[infoidx].name;
				if (pTuingjet->baseget_id.find('_') != string::npos) {
					name += _T("(") + pTuingjet->baseget_id + _T(")");
				}
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_ADDSTRING, 0, (LPARAM)name.c_str()));
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_SETCURSEL, idx, 0);
				SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_LIUSETIDGET, LBN_SELCHANGE), NULL);

				// 새로 생성한 티젯을 화면에 바로 출력한다. 
				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_BTNREMOVETIDGET:
			if (idx_use != -1) {
				if (MessageBox(hDlg, _T("티젯 제거시 모든 정보도 같이 삭제됩니다. 제거하시겠습니까?"),
					_T("확인"), MB_YESNO) == IDNO) {
					return TRUE;
				}
				memset(list_name, 0, sizeof(name));
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				name = list_name;
				size_t pos = name.find('(');
				if (pos == string::npos) {
					infoidx = FindTidgetNameIdx(list_name);
					id = arTidgetInfo[infoidx].cls;
				}
				else {
					id = name.substr(pos + 1, name.find(')'));
				}

				DestroyWindow(arTidget[idx_use]->hBaseWnd);
				arTidget[idx_use]->Dispose();
				arTidget.erase(arTidget.begin() + idx_use);

				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_DELETESTRING, idx_use, 0);
				count = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETCOUNT, 0, 0));
				if (idx_use == count) idx_use--;
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_SETCURSEL, idx_use, 0);

				setting.deleteSection(id);
				mainWindow.Relayout();
			}

			return TRUE;
		case IDC_BTNUPTIDGET:
			if (idx_use != -1 && idx_use != 0) {
				CTuingJet* t = arTidget[idx_use];
				arTidget[idx_use] = arTidget[idx_use - 1];
				arTidget[idx_use - 1] = t;

				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_DELETESTRING, idx_use, 0);

				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_INSERTSTRING, idx_use - 1, (LPARAM)list_name));
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_SETCURSEL, idx, 0);

				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_BTNDOWNTIDGET:
			count = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETCOUNT, 0, 0));
			if (idx_use != -1 && idx_use != count - 1) {
				CTuingJet* t = arTidget[idx_use];
				arTidget[idx_use] = arTidget[idx_use + 1];
				arTidget[idx_use + 1] = t;

				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_GETTEXT, idx_use, (LPARAM)list_name);
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_DELETESTRING, idx_use, 0);

				idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_INSERTSTRING, idx_use + 1, (LPARAM)list_name));
				SendDlgItemMessage(hDlg, IDC_LIUSETIDGET, LB_SETCURSEL, idx, 0);

				mainWindow.Relayout();
			}
			return TRUE;
		case IDC_LITIDGET:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				idx_dget = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_GETCURSEL, 0, 0));
				if (idx_dget != -1) {
					idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LITIDGET, LB_GETITEMDATA, idx_dget, 0));
					SetDlgItemText(hDlg, IDC_EDTIDGETDESC, arTidgetInfo[idx].desc.c_str());
				}
				return TRUE;
			}
			break;
		case IDC_LIUSETIDGET:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				if (idx_use != -1) {
					CheckDlgButton(hDlg, IDC_CHKHIDETIDGET,
						arTidget[idx_use]->baseget_visible ? BST_UNCHECKED : BST_CHECKED);
				}
				return TRUE;
			}
			break;
		case IDC_CHKHIDETIDGET:
			if (idx_use != -1) {
				arTidget[idx_use]->baseget_visible =
					(IsDlgButtonChecked(hDlg, IDC_CHKHIDETIDGET) == BST_UNCHECKED);
				ShowWindow(arTidget[idx_use]->hBaseWnd,
					arTidget[idx_use]->baseget_visible ? SW_SHOW : SW_HIDE);
				mainWindow.Relayout();
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}	// end WM_COMMAND
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK CMainWnd::MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LPMEASUREITEMSTRUCT lpmis;
	static sNamedColor arMainColor[] = {
		{_T("Black"), 0}, {_T("Red"), 0},{_T("Green"), 0},{_T("Blue"), 0},{_T("Yellow"), 0},{_T("Magenta"), 0},{_T("Cyan"), 0},
		{_T("Gainsboro"), 0}, {_T("Silver"), 0}, {_T("Gray"), 0}, {_T("DarkSlateGray"), 0},
		{_T("Orange"), 0},{_T("Indigo"), 0}, {_T("DodgerBlue"), 0}, {_T("Lime"), 0},
	};
	RECT wrt;
	INT tDock = -1;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(mainWindow.hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		if (arMainColor[0].color == 0) {
			for (sNamedColor& nc : arMainColor) {
				nc.color = GetColorFromName(nc.name);
			}
		}

		// 색상수만큼 콤보 박스에 채운다.
		for (INT i = 0; i < Count(arMainColor); i++) {
			SendDlgItemMessage(hDlg, IDC_CBCAPCOLOR, CB_ADDSTRING, 0, 0);
			SendDlgItemMessage(hDlg, IDC_CBBDCOLOR, CB_ADDSTRING, 0, 0);
			SendDlgItemMessage(hDlg, IDC_CBSPLITCOLOR, CB_ADDSTRING, 0, 0);
			SendDlgItemMessage(hDlg, IDC_CBMAINBACKCOLOR, CB_ADDSTRING, 0, 0);
			SendDlgItemMessage(hDlg, IDC_CBTRAYBACKCOLOR, CB_ADDSTRING, 0, 0);
		}

		// 스핀 컨트롤의 범위 설정
		SendDlgItemMessage(hDlg, IDC_SPINCAPTHICK, UDM_SETRANGE, 0, MAKELPARAM(50, 24));
		SendDlgItemMessage(hDlg, IDC_SPINBDTHICK, UDM_SETRANGE, 0, MAKELPARAM(16, 2));
		SendDlgItemMessage(hDlg, IDC_SPINSPLITTHICK, UDM_SETRANGE, 0, MAKELPARAM(16, 2));
		SendDlgItemMessage(hDlg, IDC_SPINTIDGETSIZE, UDM_SETRANGE, 0, MAKELPARAM(48, 16));
		SendDlgItemMessage(hDlg, IDC_SPINTIDGETGAP, UDM_SETRANGE, 0, MAKELPARAM(16, 1));

		// 메인의 속성값을 읽어온다. 
		CheckRadioButton(hDlg, IDC_RADOCKLEFT, IDC_RAUNDOCK,
			mainWindow.dockEdge == -1 ? IDC_RAUNDOCK : IDC_RADOCKLEFT + mainWindow.dockEdge);
		CheckRadioButton(hDlg, IDC_RAVERT, IDC_RAHORZ,
			mainWindow.bVertical ? IDC_RAVERT : IDC_RAHORZ);
		if (mainWindow.dockEdge != -1) {
			EnableWindow(GetDlgItem(hDlg, IDC_RAVERT), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RAHORZ), FALSE);
		}
		SendDlgItemMessage(hDlg, IDC_CHKALWAYSONTOP, BM_SETCHECK,
			mainWindow.alwaysOnTop ? BST_CHECKED : BST_UNCHECKED, 0);
		SetDlgItemInt(hDlg, IDC_EDCAPTHICK, mainWindow.capThick, FALSE);
		SetDlgItemInt(hDlg, IDC_EDBDTHICK, mainWindow.bdThick, FALSE);
		SetDlgItemInt(hDlg, IDC_EDSPLITTHICK, mainWindow.splitThick, FALSE);
		SetDlgItemInt(hDlg, IDC_EDTIDGETSIZE, trayWindow.tidgetSize, FALSE);
		SetDlgItemInt(hDlg, IDC_EDTIDGETGAP, trayWindow.tidgetGap, FALSE);

		for (INT i = 0; i < Count(arMainColor); i++) {
			if (mainWindow.capColor == arMainColor[i].color) {
				SendDlgItemMessage(hDlg, IDC_CBCAPCOLOR, CB_SETCURSEL, i, 0);
			}
			if (mainWindow.bdColor == arMainColor[i].color) {
				SendDlgItemMessage(hDlg, IDC_CBBDCOLOR, CB_SETCURSEL, i, 0);
			}
			if (mainWindow.splitColor == arMainColor[i].color) {
				SendDlgItemMessage(hDlg, IDC_CBSPLITCOLOR, CB_SETCURSEL, i, 0);
			}
			if (mainWindow.BaseBackColor == arMainColor[i].color) {
				SendDlgItemMessage(hDlg, IDC_CBMAINBACKCOLOR, CB_SETCURSEL, i, 0);
			}
			if (trayWindow.BaseBackColor == arMainColor[i].color) {
				SendDlgItemMessage(hDlg, IDC_CBTRAYBACKCOLOR, CB_SETCURSEL, i, 0);
			}
		}
		return TRUE;
	case WM_MEASUREITEM:
		lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight = 18;
		return TRUE;
	case WM_DRAWITEM:
		DrawOwnerItem(lParam, arMainColor);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADOCKLEFT:
		case IDC_RADOCKRIGHT:
			CheckRadioButton(hDlg, IDC_RAVERT, IDC_RAHORZ, IDC_RAVERT);
			EnableWindow(GetDlgItem(hDlg, IDC_RAVERT), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RAHORZ), FALSE);
			return TRUE;
		case IDC_RADOCKTOP:
		case IDC_RADOCKBOTTOM:
			CheckRadioButton(hDlg, IDC_RAVERT, IDC_RAHORZ, IDC_RAHORZ);
			EnableWindow(GetDlgItem(hDlg, IDC_RAVERT), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RAHORZ), FALSE);
			return TRUE;
		case IDC_RAUNDOCK:
			EnableWindow(GetDlgItem(hDlg, IDC_RAVERT), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_RAHORZ), TRUE);
			return TRUE;
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_RADOCKLEFT)) tDock = ABE_LEFT;
			if (IsDlgButtonChecked(hDlg, IDC_RADOCKTOP)) tDock = ABE_TOP;
			if (IsDlgButtonChecked(hDlg, IDC_RADOCKRIGHT)) tDock = ABE_RIGHT;
			if (IsDlgButtonChecked(hDlg, IDC_RADOCKBOTTOM)) tDock = ABE_BOTTOM;
			if (IsDlgButtonChecked(hDlg, IDC_RAUNDOCK)) tDock = -1;

			mainWindow.bVertical = (IsDlgButtonChecked(hDlg, IDC_RAVERT) == BST_CHECKED);
			mainWindow.alwaysOnTop = (IsDlgButtonChecked(hDlg, IDC_CHKALWAYSONTOP) == BST_CHECKED);
			if (mainWindow.alwaysOnTop) {
				SetWindowPos(mainWindow.hBaseWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else {
				SetWindowPos(mainWindow.hBaseWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			mainWindow.capThick = GetDlgItemInt(hDlg, IDC_EDCAPTHICK, NULL, FALSE);
			mainWindow.bdThick = GetDlgItemInt(hDlg, IDC_EDBDTHICK, NULL, FALSE);
			mainWindow.splitThick = GetDlgItemInt(hDlg, IDC_EDSPLITTHICK, NULL, FALSE);
			trayWindow.tidgetSize = GetDlgItemInt(hDlg, IDC_EDTIDGETSIZE, NULL, FALSE);
			trayWindow.tidgetGap = GetDlgItemInt(hDlg, IDC_EDTIDGETGAP, NULL, FALSE);

			mainWindow.capColor = arMainColor[SendDlgItemMessage(hDlg, IDC_CBCAPCOLOR,
				CB_GETCURSEL, 0, 0)].color;
			mainWindow.bdColor = arMainColor[SendDlgItemMessage(hDlg, IDC_CBBDCOLOR,
				CB_GETCURSEL, 0, 0)].color;
			mainWindow.splitColor = arMainColor[SendDlgItemMessage(hDlg, IDC_CBSPLITCOLOR,
				CB_GETCURSEL, 0, 0)].color;
			mainWindow.SetBackColor(arMainColor[SendDlgItemMessage(hDlg, IDC_CBMAINBACKCOLOR,
				CB_GETCURSEL, 0, 0)].color);
			trayWindow.SetBackColor(arMainColor[SendDlgItemMessage(hDlg, IDC_CBTRAYBACKCOLOR,
				CB_GETCURSEL, 0, 0)].color);

			// 도킹 변경. 변화없으면 그냥 리턴해 온다.
			if (tDock == -1) {
				mainWindow.UnDocking();
			}
			else {
				mainWindow.Docking(-1, tDock);
			}

			// 도킹이 바뀌지 않아도 경계선 두께가 바뀔 수 있어 바로 업데이트한다. 
			mainWindow.UpdateBorderEdge();

			mainWindow.Relayout();
			InvalidateRect(mainWindow.hBaseWnd, NULL, TRUE);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}	// end WM_COMMAND
		break;
	}
	return FALSE;
}

VOID CBaseTray::Create(HWND hParent) {
	CreateWindow(_T("CBaseWnd"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
		0, 0, 0, 0, hParent, (HMENU)2, g_hInst, this);
}

LRESULT CBaseTray::OnCreate(WPARAM wParam, LPARAM lParam) {
	tidgetSize = setting.get(_T("main"), _T("tidgetSize"), 24);
	tidgetGap = setting.get(_T("main"), _T("tidgetGap"), 4);
	return FALSE;
}

LRESULT CBaseTray::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	setting.set(_T("main"), _T("tidgetSize"), tidgetSize);
	setting.set(_T("main"), _T("tidgetGap"), tidgetGap);
	return FALSE;
}

// 트레이의 두께를 결정한다. 인수로 경계선을 제외한 위젯 두께를 전달한다.
// 수평, 수직에 상관 없이 논리는 같되 Thick이 폭이냐 높이냐만 다르다.
VOID CBaseTray::UpdateTrayThick(INT widgetThick) {
	// 한줄에 들어가는 티젯 개수. 위젯폭에서 왼쪽의 갭 빼고 폭+갭으로 나눈 수.
	// 예를 들어 위젯폭이 100일 때 첫 갭인 4를 뺀 96에 폭+갭인 28이 몇 개나 들어가는지 본다. 이 경우는 3개이다.
	INT perLine = (widgetThick - tidgetGap) / (tidgetSize + tidgetGap);
	// 보이는 티젯의 개수를 센다.
	INT num = 0;
	for (size_t ti = 0; ti < arTidget.size(); ti++) {
		if (arTidget[ti]->baseget_visible == FALSE) continue;
		num++;
	}
	// 개수에 따른 줄 수. 1, 2, 3개까지는 1줄이고 4, 5, 6은 두 줄이다. 게시물 페이징하는 수식과 같다.
	INT trayLine = (num - 1) / perLine + 1;
	// 트레이의 두께 = 위쪽 갭에서 폭+갭 높이와 줄 수를 곱한 값이다.
	trayThick = tidgetGap + (tidgetSize + tidgetGap) * trayLine;
}

// 트레이의 티젯을 배치한다. 수직은 좌우에서 우로, 위에서 아래로 배치하고 수평은 위에서 아래로, 좌에서 우로 배치한다.
// 방향만 다를 뿐 배치 논리는 동일하다. 
VOID CBaseTray::ArrangeTrayTidget(INT widgetThick) {
	INT x, y;

	x = tidgetGap;
	y = tidgetGap;
	for (size_t ti = 0; ti < arTidget.size(); ti++) {
		if (arTidget[ti]->baseget_visible == FALSE) continue;
		if (mainWindow.bVertical) {
			// 티젯 오른쪽이 트레이 오른쪽에 닿았으면 개행한다. 제일 끝 아이콘의 갭까지도 포함해야 한다.
			if (x + tidgetSize + tidgetGap > widgetThick) {
				x = tidgetGap;
				y += (tidgetSize + tidgetGap);
			}
			MoveWindow(arTidget[ti]->hBaseWnd, x, y, tidgetSize, tidgetSize, TRUE);
			UpdateWindow(arTidget[ti]->hBaseWnd);
			x += (tidgetSize + tidgetGap);
		}
		else {
			if (y + tidgetSize + tidgetGap > widgetThick) {
				y = tidgetGap;
				x += (tidgetSize + tidgetGap);
			}
			MoveWindow(arTidget[ti]->hBaseWnd, x, y, tidgetSize, tidgetSize, TRUE);
			UpdateWindow(arTidget[ti]->hBaseWnd);
			y += (tidgetSize + tidgetGap);
		}
	}
}

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, INT);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					_In_opt_ HINSTANCE hPrevInstance,
					_In_ LPWSTR    lpCmdLine,
					_In_ INT       nCmdShow) {
	MSG msg;
	HACCEL hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CreateMutex(NULL, TRUE, szProgName);
	if (GetLastError() == ERROR_ALREADY_EXISTS) return FALSE;

	MyRegisterClass(hInstance);								// Initialize global strings
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;	// Perform application initialization:

	setting.SetIniPath(NULL);
	lstrcpy(setting.regRoot, _T("SOFTWARE\\LINGSSOFT\\TuingMgr\\"));

	INT version = setting.get(_T("main"), _T("version"), 0);
	if (version != 0 && version != VERSION) {
		MessageBox(NULL, _T("not matching, download new version"), _T("버전"), MB_OK);
	}

	mainWindow.Create();
	::ShowWindow(mainWindow.hBaseWnd, nCmdShow);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TUINGMGR));
	while (GetMessage(&msg, NULL, 0, 0)) {
		/*if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(mainWindow.hBaseWnd);
	mainWindow.Dispose();
	Shell_NotifyIcon(NIM_DELETE, &nidApp);

	PostQuitMessage(FALSE);
	CBaseWnd::DeleteMap();
	return static_cast<INT>(msg.wParam);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
//  COMMENTS:
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUINGMGR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDS_STR_TUINGMGR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow) {
	HWND hWnd;
	HICON hMainIcon;

	g_hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindow(szWindowClass, szProgName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) return FALSE;
	hMainIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_TUINGMGR));

	nidApp.cbSize = sizeof(NOTIFYICONDATA);				// sizeof the struct in bytes 
	nidApp.hWnd = (HWND)hWnd;							// handle of the window which will process this app. messages 
	nidApp.uID = IDI_TUINGMGR;							// ID of the icon that willl appear in the system tray 
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;	// ORing of all the flags 
	nidApp.hIcon = hMainIcon;							// handle of the Icon to be displayed, obtained from LoadIcon 
	nidApp.uCallbackMessage = WM_ICON_NOTIFY;
	LoadString(hInstance, IDS_STR_TUINGMGR, nidApp.szTip, ITEM_LENGTH);
	Shell_NotifyIcon(NIM_ADD, &nidApp);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	INT wmId, wmEvent;
	POINT lpClickPoint;

	switch (message) {
	case WM_CREATE: {
		HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUINGMGR));
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		return FALSE;
	}
	case WM_ICON_NOTIFY:
		switch (LOWORD(lParam)) {
		case WM_RBUTTONDOWN:
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			GetCursorPos(&lpClickPoint);
			hPopMenu = CreatePopupMenu();
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_ABOUT, _T("About"));
			if (bDisable == TRUE) uFlag |= MF_GRAYED;
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST2, _T("Test 2")); // Test 2
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_TEST1, _T("Test 1")); // Test 1				
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
			if (bDisable == TRUE) {
				InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_ENABLE, _T("Enable"));
			}
			else {
				InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_DISABLE, _T("Disable"));
			}
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, IDM_SEP, _T("SEP"));
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case IDM_ABOUT:
			if (!bAboutDlg) DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_TEST1:
			MessageBox(NULL, _T("This is a test for menu Test 1"), _T("Test 1"), MB_OK);
			break;
		case IDM_TEST2:
			MessageBox(NULL, _T("This is a test for menu Test 2"), _T("Test 2"), MB_OK);
			break;
		case IDM_DISABLE:
			bDisable = TRUE;
			break;
		case IDM_ENABLE:
			bDisable = FALSE;
			break;
		case IDM_EXIT:
			Shell_NotifyIcon(NIM_DELETE, &nidApp);
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		/*
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;*/
	case WM_DESTROY:
		PostQuitMessage(FALSE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Message handler for about box.
//
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	HWND hStatic;
	HFONT hFont = CreateFont(
		15,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		DEFAULT_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial")					// lpszFacename
	);

	switch (message) {
	case WM_INITDIALOG:
		hStatic = GetDlgItem(hDlg, /*대화 상자 핸들 */ IDC_STATIC /*정적 텍스트 상자의 제어 ID */);
		SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, /*폰트 핸들*/TRUE /*bRedraw*/);
		bAboutDlg = TRUE;
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			bAboutDlg = FALSE;
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
