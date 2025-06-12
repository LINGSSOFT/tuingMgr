#include "framework.h"

CTuingJet* CTuingJet::CreateObject(wstring cls) {
	if (cls == _T("LCDTest")) return new CLCDTest();
	if (cls == _T("TextClip")) return new CTextClip();
	if (cls == _T("ColorTable")) return new CColorTable();
	if (cls == _T("MoveCursor")) return new CMoveCursor();
	if (cls == _T("SizeMeter")) return new CSizeMeter();
	if (cls == _T("Tetris")) return new CTetris();
	if (cls == _T("OneImage")) return new COneImage();
	if (cls == _T("EnumFont")) return new CEnumFont();
	if (cls == _T("Spy")) return new CSpy();
	if (cls == _T("SwapMouse")) return new CSwapMouse();

	return nullptr;
}

CTuingJet* CTuingJet::CreateDefaultObject(wstring cls) {
	CTuingJet* pTuingjet = CreateObject(cls);

	INT ti;
	for (ti = 0;; ti++) {
		if (arTidgetInfo[ti].cls == _T("")) return nullptr;
		if (arTidgetInfo[ti].cls == cls) break;
	}

	pTuingjet->baseget_cls = cls;
	if (arTidgetInfo[ti].allowMulti == FALSE) {
		pTuingjet->baseget_id = cls;
	} else {
		bool bFind;
		wstring id;
		for (int num = 1;; num++) {
			if (num == 1) {
				id = cls;
			} else {
				TCHAR tnum[10];
				wsprintf(tnum, _T("%d"), num);
				id = cls + _T("_") + tnum;
			}
			bFind = false;
			for (size_t ti = 0; ti < arTidget.size(); ti++) {
				if (arTidget[ti]->baseget_id == id) {
					bFind = true;
					break;
				}
			}
			if (bFind == false) break;
		}

		pTuingjet->baseget_id = id;
	}

	pTuingjet->baseget_visible = TRUE;

	return pTuingjet;
}

VOID CTuingJet::Serialize(TCHAR* pack) {
	wsprintf(pack, _T("%s§%s§%d"),
		baseget_cls.c_str(), baseget_id.c_str(), baseget_visible);
}

CTuingJet* CTuingJet::Deserialize(LPCTSTR pack) {
	TCHAR token[MAX_PATH];
	CTuingGet::GetPackToken(pack, 0, token);
	CTuingJet* pTuingjet = CreateObject(token);
	pTuingjet->baseget_cls = token;
	CTuingGet::GetPackToken(pack, 1, token); pTuingjet->baseget_id = token;
	CTuingGet::GetPackToken(pack, 2, token); pTuingjet->baseget_visible = _ttoi(token);

	return pTuingjet;
}

LRESULT CTuingJet::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	SetCapture(hBaseWnd);
	return FALSE;
}

LRESULT CTuingJet::OnLButtonUp(WPARAM wParam, LPARAM lParam) {
	if (GetCapture() != hBaseWnd) return FALSE;
	ReleaseCapture();

	POINT pt = { (INT)(SHORT)LOWORD(lParam) , (INT)(SHORT)HIWORD(lParam) };
	RECT crt;
	GetClientRect(hBaseWnd, &crt);
	if (PtInRect(&crt, pt) == FALSE) return FALSE;

	OnClick();
	return FALSE;
}

VOID CLCDTestWin::Create(HWND hParent) {
	// 항상 위. 마지막 인수에 this 유의
	CreateWindowEx(WS_EX_TOPMOST, _T("CBaseWnd"), _T(""), WS_POPUP | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParent, (HMENU)NULL, g_hInst, this);
}

LRESULT CLCDTestWin::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		RotateColor();
		return FALSE;
	case WM_RBUTTONDOWN:
		RotateMonitor();
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CLCDTestWin::OnCreate(WPARAM wParam, LPARAM lParam) {
	arColor[0] = RGB(0, 0, 0);
	arColor[1] = RGB(255, 255, 255);
	arColor[2] = RGB(255, 0, 0);
	arColor[3] = RGB(0, 255, 0);
	arColor[4] = RGB(0, 0, 255);
	arColor[5] = RGB(255, 255, 0);
	arColor[6] = RGB(0, 255, 255);
	arColor[7] = RGB(255, 0, 255);
	colorIdx = 0;
	displayText = TRUE;
	animate = FALSE;

	// 메인 윈도우가 있는 모니터를 가득 채운다. 
	nowMonitor = GetMonitorOfWindow(mainWindow.hBaseWnd);
	RECT rt = arMonitor[nowMonitor].rcMonitor;
	SetWindowPos(hBaseWnd, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
		SWP_NOZORDER);
	return FALSE;
}

LRESULT CLCDTestWin::OnPaint(WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	RECT crt, art;
	static LPCTSTR Mes = _T("LCD 테스트\r\n왼쪽 클릭, 스페이스:색상 변경\r\n오른쪽 클릭, Tab:모니터 이동\r\n")
		_T("Esc:종료, Enter:안내 문자열 제거\r\nRGBWKYCM:각 색상 선택\r\nA:애니메이션");

	GetClientRect(hBaseWnd, &crt);
	hdc = BeginPaint(hBaseWnd, &ps);
	if (animate) {
		for (INT x = -aniOffset, c = aniIdx; x < crt.right;) {
			hBrush = CreateSolidBrush(arColor[c]);
			SetRect(&art, x, crt.top, x + tileWidth, crt.bottom);
			FillRect(hdc, &art, hBrush);
			DeleteObject(hBrush);
			x += tileWidth;
			moveNext(c, Count(arColor));
		}
	} else {
		hBrush = CreateSolidBrush(arColor[colorIdx]);
		FillRect(hdc, &crt, hBrush);
		DeleteObject(hBrush);

		if (displayText) {
			if (colorIdx == 0) {
				SetTextColor(hdc, RGB(255, 255, 255));
			}
			SetBkMode(hdc, TRANSPARENT);
			crt.top = crt.bottom / 2;
			DrawText(hdc, Mes, -1, &crt, DT_CENTER);
		}
	}

	EndPaint(hBaseWnd, &ps);
	return FALSE;
}

LRESULT CLCDTestWin::OnKeyDown(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_SPACE:
		RotateColor();
		break;
	case VK_TAB:
		RotateMonitor();
		break;
	case VK_ESCAPE:
		// 윈도우 파괴 및 객체맵에서도 제거
		if (animate) {
			KillTimer(hBaseWnd, 1);
		}
		DestroyWindow(hBaseWnd);
		Dispose();
		break;
	case VK_RETURN:
		displayText = !displayText;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'R':
		colorIdx = 2;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'G':
		colorIdx = 3;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'B':
		colorIdx = 4;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'W':
		colorIdx = 1;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'K':
		colorIdx = 0;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'Y':
		colorIdx = 5;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'C':
		colorIdx = 6;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'M':
		colorIdx = 7;
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case 'A':
		animate = !animate;
		if (animate) {
			SetTimer(hBaseWnd, 1, 10, NULL);
			aniIdx = 0;
			aniOffset = 0;
			tileWidth = 100;
		}
		else {
			KillTimer(hBaseWnd, 1);
			InvalidateRect(hBaseWnd, NULL, FALSE);
		}
		break;
	}

	return FALSE;
}

LRESULT CLCDTestWin::OnTimer(WPARAM wParam, LPARAM lParam) {
	if (aniOffset == tileWidth - 1) {
		aniOffset = 0;
		moveNext(aniIdx, Count(arColor));
	} else {
		aniOffset++;
	}
	InvalidateRect(hBaseWnd, NULL, FALSE);
	return FALSE;
}

LRESULT CLCDTestWin::RotateColor() {
	moveNext(colorIdx, Count(arColor));
	InvalidateRect(hBaseWnd, NULL, FALSE);

	return FALSE;
}

LRESULT CLCDTestWin::RotateMonitor() {
	if (monitorCount > 1) {
		moveNext(nowMonitor, monitorCount);
		RECT rt = arMonitor[nowMonitor].rcMonitor;
		SetWindowPos(hBaseWnd, NULL, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
			SWP_NOZORDER);
	}

	return FALSE;
}

LRESULT CLCDTest::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LCDTEST));
	return FALSE;
}

LRESULT CLCDTest::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CLCDTest::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CLCDTest::OnClick() {
	CLCDTestWin* pWin = new CLCDTestWin();
	pWin->delegateDispose = TRUE;
	pWin->Create(NULL);
}

LRESULT CTextClip::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_HOTKEY:
		ClipboardTextOnly();
		return FALSE;
	case WM_CHANGECBCHAIN:
		if ((HWND)wParam == hNextChain) {
			hNextChain = (HWND)lParam;
		}
		if (hNextChain) {
			SendMessage(hNextChain, WM_CHANGECBCHAIN, wParam, lParam);
		}
		return FALSE;
	case WM_DRAWCLIPBOARD:
		if (hNextChain) {
			SendMessage(hNextChain, WM_DRAWCLIPBOARD, wParam, lParam);
		}
		if (autoText) {
			ClipboardTextOnly();
		}
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CTextClip::OnCreate(WPARAM wParam, LPARAM lParam) {
	bitOn = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIPTEXTON));
	bitOff = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLIPTEXTOFF));
	hNextChain = NULL;

	clipToText = setting.get(baseget_id, _T("clipToText"), 1);
	// 이 옵션은 따로 저장하지 않으며 실행중에 잠시만 켜서 사용한다.
	autoText = FALSE;
	clipCount = 0;

	if (clipToText) RegisterHotKey(hBaseWnd, 0, MOD_SHIFT | MOD_CONTROL, 'C');

	return FALSE;
}

LRESULT CTextClip::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(bitOn);
	DeleteObject(bitOff);
	UnregisterHotKey(hBaseWnd, 0);
	if (autoText) ChangeAutoText(FALSE);

	return FALSE;
}

VOID CTextClip::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	if (clipToText) {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOn);
	} else {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOff);
	}
}

VOID CTextClip::OnClick() {
	ChangeTextClip(!clipToText);
}

LRESULT CTextClip::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_TEXTCLIP),
			hBaseWnd, TextClipDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

INT_PTR CALLBACK CTextClip::TextClipDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CTextClip* pTuingjet;
	RECT wrt;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingjet = (CTextClip*)lParam;
		GetWindowRect(trayWindow.hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		if (pTuingjet->clipToText) {
			CheckDlgButton(hDlg, IDC_CHKTEXTCLIP, BST_CHECKED);
		}
		if (pTuingjet->autoText) {
			CheckDlgButton(hDlg, IDC_CHKAUTOTEXTCLIP, BST_CHECKED);
		}
		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			pTuingjet->ChangeTextClip(IsDlgButtonChecked(hDlg, IDC_CHKTEXTCLIP));
			pTuingjet->ChangeAutoText(IsDlgButtonChecked(hDlg, IDC_CHKAUTOTEXTCLIP));

			hOptionPopup = NULL;
			DestroyWindow(hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// 텍스트만 복사
VOID CTextClip::ClipboardTextOnly() {
	HGLOBAL hmem;
	UINT Format = 0;
	BOOL bOther = FALSE;
	TCHAR* p, * p2;

	// 일단 텍스트가 들어 있어야 한다.
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) return;

	// 텍스트외에 다른 포맷도 들어 있어야 한다.
	OpenClipboard(mainWindow.hBaseWnd);
	for (;;) {
		Format = EnumClipboardFormats(Format);
		if (Format == 0) break;
		if (Format != CF_TEXT && Format != CF_OEMTEXT &&
			Format != CF_UNICODETEXT && Format != CF_LOCALE) {
			bOther = TRUE;
			break;
		}
	}
	CloseClipboard();

	// 텍스트만 있다면 변환할 필요가 없다.
	if (bOther == FALSE) return;

	OpenClipboard(mainWindow.hBaseWnd);
	hmem = GetClipboardData(CF_UNICODETEXT);
	p = (TCHAR*)GlobalLock(hmem);
	p2 = (TCHAR*)calloc(lstrlen(p) + 1, sizeof(TCHAR));
	lstrcpy(p2, p);
	GlobalUnlock(hmem);
	CloseClipboard();

	CopyClipboardText(p2);
	free(p2);
	clipCount++;
}

VOID CTextClip::ChangeTextClip(BOOL clipToText) {
	if (this->clipToText == clipToText) return;

	this->clipToText = clipToText;
	if (clipToText) {
		RegisterHotKey(hBaseWnd, 0, MOD_SHIFT | MOD_CONTROL, 'C');
	} else {
		UnregisterHotKey(hBaseWnd, 0);
	}
	InvalidateRect(hBaseWnd, NULL, FALSE);
	setting.set(baseget_id, _T("clipToText"), clipToText);
}

VOID CTextClip::ChangeAutoText(BOOL autoText)
{
	if (this->autoText == autoText) return;

	this->autoText = autoText;
	if (autoText) {
		hNextChain = SetClipboardViewer(hBaseWnd);
	} else {
		ChangeClipboardChain(hBaseWnd, hNextChain);
	}
}

LRESULT CColorTable::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_COLORTABLE));
	return FALSE;
}

LRESULT CColorTable::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CColorTable::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CColorTable::OnClick() {
	CColorTableWin* pWin = new CColorTableWin();
	pWin->delegateDispose = TRUE;
	pWin->Create(NULL);
}

// 색상표 창. 
// todo : 키보드로도 상하좌우 이동하는 기능 추가할 것
VOID CColorTableWin::Create(HWND hParent) {
	// 최소화는 할 수 없음
	CreateWindow(_T("CBaseWnd"), _T("색상표"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,
		0, 0, 0, 0, hParent, (HMENU)NULL, g_hInst, this);

	// 최후 실행 위치로 복귀.
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.flags = 0;
	wndpl.rcNormalPosition.left = setting.get(_T("ColorTableApp"), _T("left"), 10);
	wndpl.rcNormalPosition.top = setting.get(_T("ColorTableApp"), _T("top"), 10);
	wndpl.rcNormalPosition.right = setting.get(_T("ColorTableApp"), _T("right"), 1500);
	wndpl.rcNormalPosition.bottom = setting.get(_T("ColorTableApp"), _T("bottom"), 1024);
	wndpl.showCmd = setting.get(_T("ColorTableApp"), _T("showCmd"), SW_SHOWNORMAL);
	SetWindowPlacement(hBaseWnd, &wndpl);
}

LRESULT CColorTableWin::OnCreate(WPARAM wParam, LPARAM lParam) {
	hList = CreateWindow(_T("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY, 5, 5, 130, 150, hBaseWnd, (HMENU)CT_LIST, g_hInst, NULL);
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)_T("웹 표준색(색상순)"));
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)_T("웹 표준색(이름순)"));
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)_T("웹 안전색"));
	SendMessage(hList, LB_SETCURSEL, 0, 0);
	SendMessage(hList, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	// 4개의 복사 버튼 배치
	for (INT i = 0; i < 4; i++) {
		hBtnCopy[i] = CreateWindow(_T("button"), _T(""), WS_CHILD | WS_VISIBLE |
			BS_PUSHBUTTON, 5, 480 + i * 30, 130, 25, hBaseWnd, (HMENU)(UINT_PTR)(CT_COPY1 + i), g_hInst, NULL);
		SendMessage(hBtnCopy[i], WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	}

	PrepareColor(0);
	selIdx = 0;
	ChangeSelColor(0);
	return 0;
}

LRESULT CColorTableWin::OnDestroy(WPARAM wParam, LPARAM lParam) {
	// 최후 실행 위치 저장
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hBaseWnd, &wndpl);

	setting.set(_T("ColorTableApp"), _T("left"), (INT)wndpl.rcNormalPosition.left);
	setting.set(_T("ColorTableApp"), _T("top"), (INT)wndpl.rcNormalPosition.top);
	setting.set(_T("ColorTableApp"), _T("right"), (INT)wndpl.rcNormalPosition.right);
	setting.set(_T("ColorTableApp"), _T("bottom"), (INT)wndpl.rcNormalPosition.bottom);
	setting.set(_T("ColorTableApp"), _T("showCmd"), (INT)wndpl.showCmd);

	return FALSE;
}

LRESULT CColorTableWin::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	CalcTileSize();
	return FALSE;
}

LRESULT CColorTableWin::OnCommand(WPARAM wParam, LPARAM lParam) {
	TCHAR str[128];

	switch (LOWORD(wParam)) {
	case CT_LIST:
		switch (HIWORD(wParam)) {
		case LBN_SELCHANGE:
			PrepareColor(static_cast<INT>(SendMessage(hList, LB_GETCURSEL, 0, 0)));
			CalcTileSize();
			InvalidateRect(hBaseWnd, NULL, TRUE);
			return 0;
		}
		break;
	case CT_COPY1:
	case CT_COPY2:
	case CT_COPY3:
	case CT_COPY4:
		GetWindowText((HWND)lParam, str, 128);
		CopyClipboardText(str);
		break;
	}

	return FALSE;
}

VOID CColorTableWin::OnDraw(HDC hdc, INT width, INT height) {
	INT x = leftMargin;
	INT y = 0;
	HBRUSH brush, oldBrush;
	TCHAR value[128];
	HFONT oldFont;
	INT rgb;

	// 현재 선택 색상 보여 줌
	rgb = RGB(GetBValue(arColor[selIdx].color), GetGValue(arColor[selIdx].color),
		GetRValue(arColor[selIdx].color));
	brush = CreateSolidBrush(rgb);
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, 5, 170, 135, 300);
	DeleteObject(SelectObject(hdc, oldBrush));

	// 그 아래에 이전 색상 보여줌
	rgb = RGB(GetBValue(arColor[prevSel].color), GetGValue(arColor[prevSel].color),
		GetRValue(arColor[prevSel].color));
	brush = CreateSolidBrush(rgb);
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, 5, 305, 135, 435);
	DeleteObject(SelectObject(hdc, oldBrush));

	SetBkMode(hdc, TRANSPARENT);
	oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	TextOutSz(hdc, 5, 455, _T("색상값 복사"));

	SetTextAlign(hdc, TA_CENTER);
	if (tile_width <= 80) {
		oldFont = (HFONT)SelectObject(hdc, mainFont[0]);
	}

	for (INT idx = 0; ; idx++) {
		if (lstrlen(arColor[idx].name) == 0) break;

		// 현재 선택 색상은 배경 강조
		if (idx == selIdx) {
			brush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, brush);
			Rectangle(hdc, x, y, x + tile_width, y + tile_height);
			DeleteObject(SelectObject(hdc, oldBrush));
			SetTextColor(hdc, RGB(255, 255, 255));
		} else {
			SetTextColor(hdc, RGB(0, 0, 0));
		}

		// 16진 색상은 BGR이어서 순서 바꿔서 색상값 만듬. 
		rgb = RGB(GetBValue(arColor[idx].color), GetGValue(arColor[idx].color),
			GetRValue(arColor[idx].color));
		brush = CreateSolidBrush(rgb);
		oldBrush = (HBRUSH)SelectObject(hdc, brush);

		if (namedColor) {
			TextOutSz(hdc, x + tile_width / 2, y + 2, arColor[idx].name);
			// 좌우로 4픽셀씩 패딩 적용
			Rectangle(hdc, x + 4, y + 20, x + tile_width - 4, y + tile_height - 20);
		} else {
			Rectangle(hdc, x + 4, y + 4, x + tile_width - 4, y + tile_height - 20);
		}
		DeleteObject(SelectObject(hdc, oldBrush));
		wsprintf(value, _T("%06X"), arColor[idx].color);
		TextOutSz(hdc, x + tile_width / 2, y + tile_height - 20, value);

		x += tile_width;
		if (x + tile_width > width) {
			x = leftMargin;
			y += tile_height;
		}
	}

	SelectObject(hdc, oldFont);
}

LRESULT CColorTableWin::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	POINT pt;
	INT idx;

	pt.x = (INT)(SHORT)LOWORD(lParam);
	pt.y = (INT)(SHORT)HIWORD(lParam);

	// 왼쪽 마진은 클릭해도 아무 처리도 하지 않는다.
	if (pt.x <= leftMargin) return FALSE;

	// 좌표로부터 클릭한 타일 조사
	idx = (pt.y / tile_height) * perLine + (pt.x - leftMargin) / tile_width;

	// 화면 아래 클릭시 마지막 값 선택
	if (idx >= colorCount) idx = colorCount - 1;

	ChangeSelColor(idx);
	return FALSE;
}

VOID CColorTableWin::PrepareColor(INT colorType) {
	INT i;

	switch (colorType) {
	case 0:
	case 1:
		for (i = 0; ; i++) {
			if (lstrlen(arStandardColor[i].name) == 0) break;
			lstrcpy(arColor[i].name, arStandardColor[i].name);
			arColor[i].color = arStandardColor[i].color;
		}
		lstrcpy(arColor[i].name, _T(""));
		colorCount = i;
		if (colorType == 1) {
			qsort(arColor, i, sizeof(sOneColor), colorNameCompare);
		}
		namedColor = TRUE;
		selIdx = prevSel = 0;
		break;
	case 2:
		i = 0;
		for (INT r = 0; r <= 0xff; r += 0x33) {
			for (INT g = 0; g <= 0xff; g += 0x33) {
				for (INT b = 0; b <= 0xff; b += 0x33) {
					wsprintf(arColor[i].name, _T("%02X%02X%02X"), r, g, b);
					arColor[i].color = r * 0x10000 + g * 0x100 + b;
					i++;
				}
			}
		}
		lstrcpy(arColor[i].name, _T(""));
		colorCount = i;
		namedColor = FALSE;
		selIdx = prevSel = 0;

		break;
	}
}

INT CColorTableWin::colorNameCompare(const VOID* a, const VOID* b) {
	CColorTableWin::sOneColor ra, rb;
	ra = *(CColorTableWin::sOneColor*)a;
	rb = *(CColorTableWin::sOneColor*)b;

	if (lstrcmp(ra.name, rb.name) > 0) return TRUE;

	return -1;
}

VOID CColorTableWin::CalcTileSize() {
	RECT crt;
	GetClientRect(hBaseWnd, &crt);
	INT width = crt.right - leftMargin;
	INT height = crt.bottom;

	// 줄당 16개씩. 최소 픽셀 보장 후 줄당 개수 구함. 폭 1280 이하면 16개보다 작아진다.
	tile_width = width / 16;
	if (tile_width < 80) tile_width = 80;
	perLine = width / tile_width;

	// 0으로 나누기 방지
	if (perLine == 0) perLine = 1;

	// 필요 줄 수로 높이를 나누어 구하되 최소 높이 보장.
	INT needRow = colorCount / perLine + 1;
	tile_height = height / needRow;
	if (tile_height < 50) tile_height = 50;
}

VOID CColorTableWin::ChangeSelColor(INT idx) {
	// 현재색 변경하기 전에 이전색상 백업
	prevSel = selIdx;
	selIdx = idx;

	INT color = arColor[selIdx].color;
	TCHAR text[128];

	SetWindowText(hBtnCopy[0], arColor[selIdx].name);
	wsprintf(text, _T("RGB(%d, %d, %d)"), GetBValue(color), GetGValue(color),
		GetRValue(color));
	SetWindowText(hBtnCopy[1], text);
	wsprintf(text, _T("0X%0X"), color);
	SetWindowText(hBtnCopy[2], text);
	wsprintf(text, _T("#%0X"), color);
	SetWindowText(hBtnCopy[3], text);

	InvalidateRect(hBaseWnd, NULL, TRUE);
}

LRESULT CMoveCursor::OnCreate(WPARAM wParam, LPARAM lParam) {
	bitOn = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MOVECURSORON));
	bitOff = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MOVECURSOROFF));

	LoadSetting();
	RejectCount = 0;
	if (bCursorMove) SetTimer(hBaseWnd, 1, TimerInterval, NULL);

	return FALSE;
}

LRESULT CMoveCursor::OnDestroy(WPARAM wParam, LPARAM lParam) {
	setting.set(baseget_id, _T("moveCursor"), bCursorMove);
	DeleteObject(bitOn);
	DeleteObject(bitOff);
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

VOID CMoveCursor::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	if (bCursorMove) {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOn);
	} else {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOff);
	}
}

VOID CMoveCursor::OnClick() {
	bCursorMove = !bCursorMove;
	if (bCursorMove) {
		SetTimer(hBaseWnd, 1, TimerInterval, NULL);
	} else {
		KillTimer(hBaseWnd, 1);
	}
	SaveSetting();
	InvalidateRect(hBaseWnd, NULL, FALSE);
}

LRESULT CMoveCursor::OnTimer(WPARAM wParam, LPARAM lParam) {
	POINT pt;

	GetCursorPos(&pt);
	// 같은 자리에 있는 누적 시간 갱신 
	if (pt.x == OldCursorPt.x && pt.y == OldCursorPt.y) {
		CursorKeepSec += TimerInterval;
		// 스크린 세이버가 뜰 시간이 되면 강제로 커서 한칸 오른쪽으로 옮김 
		if (CursorKeepSec >= CursorInterval * 1000) {
			CursorKeepSec = 0;
			mouse_event(MOUSEEVENTF_MOVE, 1, 0, 0, 0);
			RejectCount++;
		}
		// 위치가 바뀌면 누적 시간 리셋 
	} else {
		CursorKeepSec = 0;
		OldCursorPt = pt;
	}
	return FALSE;
}

LRESULT CMoveCursor::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_MOVECURSOR),
			hBaseWnd, MoveCursorDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

INT_PTR CALLBACK CMoveCursor::MoveCursorDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CMoveCursor* pTuingjet;
	TCHAR str[128];
	RECT wrt;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingjet = (CMoveCursor*)lParam;
		GetWindowRect(trayWindow.hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		// 범위를 반대로 줘야 위가 증가, 아래가 감소이다. 
		SendDlgItemMessage(hDlg, IDC_SPINMOVECURSORCYCLE, UDM_SETRANGE, 0, MAKELPARAM(600, 60));

		wsprintf(str, _T("이동 횟수 : %d번"), pTuingjet->RejectCount);
		SetDlgItemText(hDlg, IDC_STMOVECURSORCOUNT, str);
		if (pTuingjet->bCursorMove) {
			CheckDlgButton(hDlg, IDC_CHKMOVECURSOR, BST_CHECKED);
		}
		SetDlgItemInt(hDlg, IDC_EDMOVECURSORCYCLE, pTuingjet->CursorInterval, FALSE);
		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			pTuingjet->CursorInterval = GetDlgItemInt(hDlg, IDC_EDMOVECURSORCYCLE, NULL, FALSE);
			pTuingjet->TimerInterval = pTuingjet->CursorInterval * 100;
			if (IsDlgButtonChecked(hDlg, IDC_CHKMOVECURSOR) == BST_CHECKED) {
				pTuingjet->bCursorMove = true;
				SetTimer(pTuingjet->hBaseWnd, 1, pTuingjet->TimerInterval, NULL);
			} else {
				pTuingjet->bCursorMove = false;
				KillTimer(pTuingjet->hBaseWnd, 1);
			}

			pTuingjet->SaveSetting();

			hOptionPopup = NULL;
			DestroyWindow(hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

VOID CMoveCursor::LoadSetting() {
	bCursorMove = setting.get(baseget_id, _T("moveCursor"), 0);
	CursorInterval = setting.get(baseget_id, _T("CursorInterval"), 180);
	// 타이머는 이동주기의 1/10으로 자동 설정한다. 180초면 18초에 한번씩 확인하면 충분하다.
	// 최초 옵션으로 만들었는데 사용자가 이런 내부적인 값까지 선택할 필요는 없다. 
	TimerInterval = CursorInterval * 100;
	CursorKeepSec = 0;
}

VOID CMoveCursor::SaveSetting() {
	setting.set(baseget_id, _T("bCursorMove"), bCursorMove);
	setting.set(baseget_id, _T("CursorInterval"), CursorInterval);
}

LRESULT CSizeMeter::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SIZEMETER));
	return FALSE;
}

LRESULT CSizeMeter::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CSizeMeter::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CSizeMeter::OnClick() {
	CSizeMeterWin* pWin = new CSizeMeterWin();
	pWin->delegateDispose = TRUE;
	pWin->Create(NULL);
}

VOID CSizeMeterWin::Create(HWND hParent) {
	CreateWindow(_T("CBaseWnd"), _T("SizeMeter"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, g_hInst, this);
}

LRESULT CSizeMeterWin::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

VOID CSizeMeterWin::OnDraw(HDC hdc, INT width, INT height) {
	TCHAR str[111];

	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_CENTER);
	wsprintf(str, _T("%d x %d"), width, height);
	TextOutSz(hdc, width / 2, height / 2, str);
	lstrcpy(str, _T("1 ~ 9 : 표준 크기"));
	TextOutSz(hdc, width / 2, height / 2 + 30, str);
}

LRESULT CSizeMeterWin::OnKeyDown(WPARAM wParam, LPARAM lParam) {
	RECT crt;
	INT w, h;

	w = 0;
	switch (wParam) {
	case '1':
		w = 320; h = 240; break;
	case '2':
		w = 640; h = 480; break;
	case '3':
		w = 800; h = 600; break;
	case '4':
		w = 1024; h = 768; break;
	case '5':
		w = 1280; h = 1024; break;
	case '6':
		w = 1600; h = 1200; break;
	case '7':
		w = 1920; h = 1080; break;
	case '8':
		w = 2560; h = 1440; break;
	case '9':
		w = 3840; h = 2160; break;
	}
	if (w != 0) {
		SetRect(&crt, 0, 0, w, h);
		DWORD Style = GetWindowLong(hBaseWnd, GWL_STYLE);
		DWORD ExStyle = GetWindowLong(hBaseWnd, GWL_EXSTYLE);

		AdjustWindowRectEx(&crt, Style, GetMenu(hBaseWnd) != NULL, ExStyle);
		SetWindowPos(hBaseWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);
	}
	return FALSE;
}

LRESULT CTetris::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TETRIS));
	return FALSE;
}

LRESULT CTetris::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CTetris::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CTetris::OnClick() {
	// 게임 윈도우를 생성하여 띄운다. 
	CTetrisWin* pWin = new CTetrisWin();
	pWin->delegateDispose = true;
	CreateWindow(_T("CBaseWnd"), _T("Tetris"), WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE,
		10, 10, 100, 100,
		NULL, (HMENU)NULL, g_hInst, pWin);
}

// 클래스안에 초기화된 배열 선언하는 방법을 모르겠음. C문법을 다 까쟈 묵음. 
struct TetPoint {
	INT x, y;
};

TetPoint Shape[][4][4] = {
	{ {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },
	{ {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },
	{ {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },
	{ {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },
	{ {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },
	{ {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },
};

LRESULT CTetrisWin::OnCreate(WPARAM wParam, LPARAM lParam) {
	ts = 24;
	bw = 10;
	bh = 20;
	//memset(board, 0, sizeof(board));

	GameStatus = eTetStatus::GAMEOVER;
	srand((DWORD)GetTickCount64());
	AdjustMainWindow();

	// 메인 윈도우의 옆에 연다. 
	RECT wrt;
	GetWindowRect(mainWindow.hBaseWnd, &wrt);
	MovePopupToSide(hBaseWnd, wrt.left, wrt.top);

	// 0번은 빈칸, 1~6은 벽돌. 7은 벽
	COLORREF arTileColor[] = { 0, 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0xff00ff, 0x00ffff, 0x000000 };
	for (INT i = 1; i <= 7; i++) {
		arBrush[i] = CreateSolidBrush(arTileColor[i]);
	}

	return FALSE;
}

LRESULT CTetrisWin::OnDestroy(WPARAM wParam, LPARAM lParam) {
	for (INT i = 1; i <= 7; i++) {
		DeleteObject(arBrush[i]);
	}
	return FALSE;
}

LRESULT CTetrisWin::OnTimer(WPARAM wParam, LPARAM lParam) {
	if (MoveDown() == TRUE) {
		MakeNewBrick();
	}
	return FALSE;
}

VOID CTetrisWin::OnDraw(HDC hdc, INT width, INT height) {
	INT x, y, i;
	TCHAR str[128];

	FillRect(hdc, 0, 0, width, height, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

	// 테두리 그림
	for (x = 0; x < bw + 1; x++) {
		PrintTile(hdc, x, 0, (INT)eTetCell::WALL);
		PrintTile(hdc, x, bh + 1, (INT)eTetCell::WALL);
	}
	for (y = 0; y < bh + 2; y++) {
		PrintTile(hdc, 0, y, (INT)eTetCell::WALL);
		PrintTile(hdc, bw + 1, y, (INT)eTetCell::WALL);
	}

	// 게임판 그림
	for (x = 1; x < bw + 1; x++) {
		for (y = 1; y < bh + 1; y++) {
			PrintTile(hdc, x, y, board[x][y]);
		}
	}

	// 이동중인 벽돌 그림
	if (GameStatus != eTetStatus::GAMEOVER && brick != -1) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, nx + Shape[brick][rot][i].x, ny + Shape[brick][rot][i].y, brick + 1);
		}
	}

	// 다음 벽돌 그림
	for (x = bw + 3; x <= bw + 11; x++) {
		for (y = bh - 5; y <= bh + 1; y++) {
			if (x == bw + 3 || x == bw + 11 || y == bh - 5 || y == bh + 1) {
				PrintTile(hdc, x, y, (INT)eTetCell::WALL);
			} else {
				PrintTile(hdc, x, y, 0);
			}
		}
	}
	if (GameStatus != eTetStatus::GAMEOVER) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, bw + 7 + Shape[nbrick][0][i].x, bh - 2 + Shape[nbrick][0][i].y, nbrick + 1);
		}
	}

	// 정보 출력
	HFONT oldFont;
	oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	SetBkMode(hdc, TRANSPARENT);
	lstrcpy(str, _T("1~9:게임판 크기"));
	TextOutSz(hdc, (bw + 4) * ts, 30, str);
	lstrcpy(str, _T("Enter:시작/중지"));
	TextOutSz(hdc, (bw + 4) * ts, 60, str);
	wsprintf(str, _T("점수 : %d   "), score);
	TextOutSz(hdc, (bw + 4) * ts, 90, str);
	wsprintf(str, _T("벽돌 : %d 개   "), bricknum);
	TextOutSz(hdc, (bw + 4) * ts, 120, str);
	SelectObject(hdc, oldFont);
}

LRESULT CTetrisWin::OnKeyDown(WPARAM wParam, LPARAM lParam) {
	INT x, y;
	INT trot;

	// 게임 시작 여부에 상관없이 동작하는 키
	if (wParam == VK_RETURN) {
		if (GameStatus == eTetStatus::GAMEOVER) {
			for (x = 0; x < bw + 2; x++) {
				for (y = 0; y < bh + 2; y++) {
					board[x][y] = (y == 0 || y == bh + 1 || x == 0 || x == bw + 1) ?
						(INT)eTetCell::WALL : (INT)eTetCell::EMPTY;
				}
			}
			score = 0;
			bricknum = 0;
			GameStatus = eTetStatus::RUNNING;
			nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
			MakeNewBrick();
			Interval = 1000;
			SetTimer(hBaseWnd, 1, Interval, NULL);
		} else if (GameStatus == eTetStatus::RUNNING) {
			GameStatus = eTetStatus::PAUSE;
			KillTimer(hBaseWnd, 1);
		} else if (GameStatus == eTetStatus::PAUSE) {
			GameStatus = eTetStatus::RUNNING;
			SetTimer(hBaseWnd, 1, Interval, NULL);
		}
	}

	// 게임판 크기는 시작 전에만 바꿀 수 있다. 
	if (GameStatus == eTetStatus::GAMEOVER && wParam >= '1' && wParam <= '9') {
		switch (wParam) {
		case '1':
			bw = 8; bh = 16; ts = 32;
			break;
		case '2':	// 디폴트
			bw = 10; bh = 20; ts = 24;
			break;
		case '3':
			bw = 12; bh = 24; ts = 40;
			break;
		case '4':
			bw = 15; bh = 30; ts = 28;
			break;
		case '5':	// FHD 최대 크기
			bw = 200; bh = 100; ts = 8;
			break;
		case '6':
			bw = 20; bh = 40; ts = 32;
			break;
		case '7':
			bw = 50; bh = 100; ts = 16;
			break;
		case '8':
			bw = 200; bh = 100; ts = 16;
			break;
		case '9':	// 4k 최대 크기
			bw = 400; bh = 200; ts = 8;
			break;
		}
		// 이전판에 하던건 다 지우고 조정한다. 
		memset(board, 0, sizeof(board));
		AdjustMainWindow();
		return FALSE;
	}

	// 이하의 키는 게임 중에는 쓸 수 없다. 
	if (GameStatus != eTetStatus::RUNNING || brick == -1) {
		return FALSE;
	}

	switch (wParam) {
	case VK_LEFT:
		if (GetAround(nx - 1, ny, brick, rot) == (int)eTetCell::EMPTY) {
			nx--;
			InvalidateRect(hBaseWnd, NULL, FALSE);
		}
		break;
	case VK_RIGHT:
		if (GetAround(nx + 1, ny, brick, rot) == (int)eTetCell::EMPTY) {
			nx++;
			InvalidateRect(hBaseWnd, NULL, FALSE);
		}
		break;
	case VK_UP:
		trot = (rot == 3 ? 0 : rot + 1);
		if (GetAround(nx, ny, brick, trot) == (int)eTetCell::EMPTY) {
			rot = trot;
			InvalidateRect(hBaseWnd, NULL, FALSE);
		}
		break;
	case VK_DOWN:
		if (MoveDown() == TRUE) {
			MakeNewBrick();
		}
		break;
	case VK_SPACE:
		while (MoveDown() == FALSE) { ; }
		MakeNewBrick();
		break;
	}

	return FALSE;
}

VOID CTetrisWin::MakeNewBrick() {
	bricknum++;
	brick = nbrick;
	nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
	nx = bw / 2;
	ny = 3;
	rot = 0;
	InvalidateRect(hBaseWnd, NULL, FALSE);
	if (GetAround(nx, ny, brick, rot) != (int)eTetCell::EMPTY) {
		KillTimer(hBaseWnd, 1);
		GameStatus = eTetStatus::GAMEOVER;
		MessageBox(hBaseWnd, _T("GAME OVER"), _T("알림"), MB_OK);
	}
}

INT CTetrisWin::GetAround(INT x, INT y, INT b, INT r) {
	INT i, k = (INT)eTetCell::EMPTY;

	for (i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL CTetrisWin::MoveDown() {
	if (GetAround(nx, ny + 1, brick, rot) != (INT)eTetCell::EMPTY) {
		TestFull();
		return TRUE;
	}
	ny++;
	// 즉시 그려서 벽돌이 내려가는 모양을 보여 준다.
	InvalidateRect(hBaseWnd, NULL, FALSE);
	// 폭이 너무 넓으면 애니메이션하지 않는다. 
	if (bw < 100) UpdateWindow(hBaseWnd);

	return FALSE;
}

VOID CTetrisWin::TestFull() {
	INT i, x, y, ty;
	INT count = 0;
	static INT arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brick + 1;
	}
	// 이동중인 벽돌이 잠시 없는 상태. 
	brick = -1;

	for (y = 1; y < bh + 1; y++) {
		for (x = 1; x < bw + 1; x++) {
			if (board[x][y] == (INT)eTetCell::EMPTY) break;
		}
		if (x == bw + 1) {
			count++;
			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < bw + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			InvalidateRect(hBaseWnd, NULL, FALSE);
			UpdateWindow(hBaseWnd);
			Sleep(150);
		}
	}
	score += arScoreInc[count];
	if (bricknum % 10 == 0 && Interval > 200) {
		Interval -= 50;
		SetTimer(hBaseWnd, 1, Interval, NULL);
	}
}

VOID CTetrisWin::PrintTile(HDC hdc, INT x, INT y, INT c) {
	HPEN oldPen;
	HBRUSH oldBrush;
	RECT brt;

	if (c == (INT)eTetCell::EMPTY) {
		if (ts > 20) {
			Rectangle(hdc, x * ts + ts / 2 - 1, y * ts + ts / 2 - 1, x * ts + ts / 2 + 1, y * ts + ts / 2 + 1);
		} else {
			SetPixel(hdc, x * ts + ts / 2, y * ts + ts / 2, RGB(128, 128, 128));
		}
	} else {
		SetRect(&brt, x * ts, y * ts, x * ts + ts, y * ts + ts);

		oldBrush = (HBRUSH)SelectObject(hdc, arBrush[c]);
		oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
		Rectangle(hdc, brt.left, brt.top, brt.right, brt.bottom);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);

		// 입체감을 줄려고 했는데 예쁘지 않고 좀 느림. 
		/*white = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
		gray = CreatePen(PS_SOLID, 1, RGB(32, 32, 32));
		oldPen = (HPEN)SelectObject(hdc, white);
		MoveToEx(hdc, brt.left, brt.top, NULL); LineTo(hdc, brt.left, brt.bottom);
		MoveToEx(hdc, brt.left, brt.top, NULL); LineTo(hdc, brt.right, brt.top);
		SelectObject(hdc, gray);
		MoveToEx(hdc, brt.left, brt.bottom, NULL); LineTo(hdc, brt.right, brt.bottom);
		MoveToEx(hdc, brt.right, brt.top, NULL); LineTo(hdc, brt.right, brt.bottom);
		SelectObject(hdc, oldPen);
		DeleteObject(white);
		DeleteObject(gray);
		*/
	}
}

VOID CTetrisWin::AdjustMainWindow() {
	RECT crt;
	SetRect(&crt, 0, 0, (bw + 12) * ts, (bh + 2) * ts);
	AdjustWindowRect(&crt, WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION, FALSE);
	SetWindowPos(hBaseWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
		SWP_NOMOVE | SWP_NOZORDER);
}

LRESULT COneImage::OnCreate(WPARAM wParam, LPARAM lParam) {
	nowImage = NULL;
	pViewer = NULL;

	setting.get(baseget_id, _T("imagePath"), _T(""), imagePath, MAX_PATH);
	if (lstrlen(imagePath) != 0) {
		nowImage = Image::FromFile(imagePath);
		FixImageOrientation(nowImage);
	}
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ONEIMAGE));

	return FALSE;
}

LRESULT COneImage::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	if (nowImage != NULL) delete nowImage;
	if (pViewer != NULL) DestroyWindow(pViewer->hBaseWnd);

	return FALSE;
}

VOID COneImage::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	if (nowImage == NULL) {
		StretchBitmap(hdc, 0, 0, wh, wh, icon);
	} else {
		DrawImageCenter(hdc, nowImage, wh, wh, TRUE, BaseBackBrush);
	}
}

VOID COneImage::OnClick() {
	if (nowImage != NULL) {
		// 뷰어를 연다.
		if (pViewer == NULL) {
			pViewer = new CImageViewer();
			pViewer->delegateDispose = TRUE;
			lstrcpy(pViewer->imagePath, imagePath);
			pViewer->Create(1, this);
		}
	}
}

// todo : 옵션 대화상자를 만들고 간단한 설명과 파일 열기 버튼을 배치한다.
// 이걸 하기 전에 포커스를 잃어도 옵션창이 닫히지 않도록 정책부터 결정해야 한다. 
LRESULT COneImage::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	OPENFILENAME OFN;
	lstrcpy(imagePath, _T(""));
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hBaseWnd;
	OFN.lpstrFilter = _T("이미지 파일(jpg, png)\0*.jpg;*.png\0");
	OFN.lpstrFile = imagePath;
	OFN.nMaxFile = MAX_PATH;
	if (GetOpenFileName(&OFN) == 0) return FALSE;

	// 이미지가 결정되는 즉시 기록한다. 
	setting.set(baseget_id, _T("imagePath"), imagePath);
	nowImage = Image::FromFile(imagePath);
	FixImageOrientation(nowImage);
	InvalidateRect(hBaseWnd, NULL, TRUE);

	return FALSE;
}

// 뷰어가 종료 신호를 보내면 기록해 둔다. 그래야 새로 열 수 있다. 
LRESULT COneImage::OnUser1(WPARAM wParam, LPARAM lParam) {
	pViewer = NULL;
	return FALSE;
}

// 이미지 뷰어 관련 todo - 기본 기능 구현해 놨고 다음은 차후에 천천히 할 것
// 타이틀바 제거하고 두꺼운 액자 두른 후 액자 드래그해서 크기 조정. 바닥 드래그해서 이동
// 마우스가 상단으로 올라가면 파일명과 닫기, 최대화 버튼 보여줄 것. 또는 상단 액자 두껍게 해서 항상 표시도 가능
// 확대, 축소, 패닝, 회전, 반사, 밝기 조정 기능 구현. 슬라이더의 경우 PgUp, PgDn으로 네비게이션 이동
VOID CImageViewer::Create(int parentType, CBaseGet* parent) {
	// 타입에 따라 부모를 캐스팅해 둔다. 위젯과 미니 위젯이 형제 관계여서 C 스타일로 다형성을 구현한다.
	// CBaseWnd의 멤버를 참조할 때는 pParent를 바로 읽으면 된다. 
	pParent = parent;
	this->parentType = parentType;
	switch (parentType) {
	case 0:
		pSlide = (CImageSlide*)pParent;
		break;
	case 1:
		pOneImage = (COneImage*)pParent;
		break;
	}

	CreateWindow(_T("CBaseWnd"), _T("이미지 보기"), WS_POPUPWINDOW | WS_THICKFRAME | WS_CAPTION,
		0, 0, 0, 0,
		NULL, (HMENU)NULL, g_hInst, this);

	// 최후 실행 위치를 읽어온다. 일단 노말 좌표로 초기 좌표를 잡는다.
	prevWndPlace.length = sizeof(WINDOWPLACEMENT);
	prevWndPlace.flags = 0;
	prevWndPlace.rcNormalPosition.left = setting.get(pParent->baseget_id, _T("left"), 10);
	prevWndPlace.rcNormalPosition.top = setting.get(pParent->baseget_id, _T("top"), 10);
	prevWndPlace.rcNormalPosition.right = setting.get(pParent->baseget_id, _T("right"), 650);
	prevWndPlace.rcNormalPosition.bottom = setting.get(pParent->baseget_id, _T("bottom"), 490);
	prevWndPlace.showCmd = setting.get(pParent->baseget_id, _T("showCmd"), SW_SHOWNORMAL);
	SetWindowPlacement(hBaseWnd, &prevWndPlace);

	// 전체 화면으로 전환한다. 
	fullScreen = setting.get(pParent->baseget_id, _T("fullScreen"), FALSE);
	if (fullScreen) {
		prevWndPlace.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hBaseWnd, &prevWndPlace);
		makeFullScreen(hBaseWnd);
	}
}

LRESULT CImageViewer::OnLButtonDblClk(WPARAM wParam, LPARAM lParam) {
	// 일단 더블클릭으로 최대화한다. 차후 다른 방법으로 대체할 것.
	toggleFullScreen();
	return FALSE;
}

LRESULT CImageViewer::OnUser1(WPARAM wParam, LPARAM lParam) {
	// wParam으로 전달한 새 이미지를 표시한다.
	if (nowImage != NULL) delete nowImage;

	nowIdx = (INT)wParam;
	nowImage = Image::FromFile(pSlide->arImage[nowIdx].c_str());
	SetWindowText(hBaseWnd, pSlide->arImage[nowIdx].c_str());
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

LRESULT CImageViewer::OnCreate(WPARAM wParam, LPARAM lParam) {
	// 슬라이더는 idx를 주고, OneImage는 경로를 직접 준다.
	LPCTSTR Path;
	if (parentType == 0) {
		Path = pSlide->arImage[nowIdx].c_str();
	} else {
		Path = imagePath;
	}
	nowImage = Image::FromFile(Path);
	FixImageOrientation(nowImage);
	SetWindowText(hBaseWnd, Path);
	return FALSE;
}

LRESULT CImageViewer::OnDestroy(WPARAM wParam, LPARAM lParam) {
	if (nowImage != NULL) delete nowImage;

	// 최후 실행 위치를 조사한다. 단, 전체 화면 모드이면 전체 화면 전의 노말 좌표를 기록한다.
	if (fullScreen == FALSE) {
		prevWndPlace.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hBaseWnd, &prevWndPlace);
	}

	setting.set(pParent->baseget_id, _T("left"), prevWndPlace.rcNormalPosition.left);
	setting.set(pParent->baseget_id, _T("top"), prevWndPlace.rcNormalPosition.top);
	setting.set(pParent->baseget_id, _T("right"), prevWndPlace.rcNormalPosition.right);
	setting.set(pParent->baseget_id, _T("bottom"), prevWndPlace.rcNormalPosition.bottom);
	setting.set(pParent->baseget_id, _T("showCmd"), prevWndPlace.showCmd);
	setting.set(pParent->baseget_id, _T("fullScreen"), fullScreen);

	// 부모에게 자신이 종료했음을 알린다. 그래야 클릭시 새로운 뷰어를 만들 수 있다. 
	SendMessage(pParent->hBaseWnd, WM_USER + 1, 0, 0);

	// 자기 자신을 삭제한다. 객체맵에서도 삭제된다. -> 이러면 닫힐 때 NCDESTROY를 처리하지 못해 다운됨. -> 부모가 관리해 줘야 할 듯 하다. 
	// delete this;
	return FALSE;
}

LRESULT CImageViewer::OnPaint(WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBaseWnd, &ps);
	RECT crt;
	GetClientRect(hBaseWnd, &crt);

	// 부모의 배경 브러시 사용
	DrawImageCenter(hdc, nowImage, crt.right, crt.bottom, TRUE, pParent->BaseBackBrush);

	EndPaint(hBaseWnd, &ps);
	return FALSE;
}

LRESULT CImageViewer::OnSize(WPARAM wParam, LPARAM lParam) {
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

LRESULT CImageViewer::OnKeyDown(WPARAM wParam, LPARAM lParam) {
	if (wParam == VK_ESCAPE) DestroyWindow(hBaseWnd);
	if (wParam == VK_RETURN) toggleFullScreen();

	return FALSE;
}

VOID CImageViewer::toggleFullScreen() {
	fullScreen = !fullScreen;
	if (fullScreen) {
		// 전체 화면으로 전환하기 전의 노말 좌표를 미리 조사해 둔다.
		prevWndPlace.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hBaseWnd, &prevWndPlace);
		makeFullScreen(hBaseWnd);
	} else {
		SetWindowLongPtr(hBaseWnd, GWL_STYLE, WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE);
		SetWindowPos(hBaseWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		// 노말 좌표로 다시 복귀한다. 
		prevWndPlace.length = sizeof(WINDOWPLACEMENT);
		SetWindowPlacement(hBaseWnd, &prevWndPlace);
	}
}

LRESULT CEnumFont::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENUMFONT));
	return FALSE;
}

LRESULT CEnumFont::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CEnumFont::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CEnumFont::OnClick() {
	CEnumFontWin* pWin = new CEnumFontWin();
	pWin->delegateDispose = TRUE;

	CreateWindow(_T("CBaseWnd"), _T("폰트 열거"),
		WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_CLIPCHILDREN,
		100, 100, 900, 800,
		NULL, (HMENU)NULL, g_hInst, pWin);
}

LRESULT CEnumFontWin::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HFONT hFont, oldFont;
	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;
	HBRUSH bkBrush;
	LOGFONT tmpFont;

	switch (iMessage) {
	case WM_MEASUREITEM:
		lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight = 30;
		return FALSE;
	case WM_DRAWITEM:
		lpdis = (LPDRAWITEMSTRUCT)lParam;

		// 선택된 항목은 파란 배경에 흰색으로 출력한다.
		if (lpdis->itemState & ODS_SELECTED) {
			bkBrush = GetSysColorBrush(COLOR_HIGHLIGHT);
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOW));
		} else {
			bkBrush = GetSysColorBrush(COLOR_WINDOW);
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_WINDOW));
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
		}
		FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);

		// 25 픽셀 높이로 글꼴 이름을 출력한다.
		tmpFont = arFont[lpdis->itemID];
		tmpFont.lfHeight = 25;
		tmpFont.lfWidth = 0;
		hFont = CreateFontIndirect(&tmpFont);
		oldFont = (HFONT)SelectObject(lpdis->hDC, hFont);

		TextOutSz(lpdis->hDC, lpdis->rcItem.left + 5, lpdis->rcItem.top + 2,
			arFont[lpdis->itemID].lfFaceName);
		SelectObject(lpdis->hDC, oldFont);
		DeleteObject(hFont);
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CEnumFontWin::OnCreate(WPARAM wParam, LPARAM lParam) {
	INT left, top;
	left = setting.get(_T("EnumFont"), _T("left"), 100);
	top = setting.get(_T("EnumFont"), _T("top"), 100);
	SetWindowPos(hBaseWnd, NULL, left, top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	EnsureVisible(hBaseWnd);

	SetBackColor(GetSysColor(COLOR_BTNFACE));
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, _T("listbox"), NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED,
		10, 10, 280, 550, hBaseWnd, (HMENU)EF_LIST, g_hInst, NULL);
	hChkAt = CreateWindow(_T("button"), _T("@ 폰트 포함"), WS_CHILD | WS_VISIBLE |
		BS_AUTOCHECKBOX, 10, 570, 120, 25, hBaseWnd, (HMENU)EF_AT, g_hInst, NULL);
	hChkSort = CreateWindow(_T("button"), _T("폰트명 정렬"), WS_CHILD | WS_VISIBLE |
		BS_AUTOCHECKBOX, 10, 600, 120, 25, hBaseWnd, (HMENU)EF_SORT, g_hInst, NULL);
	hStNum = CreateWindow(_T("static"), _T("발견된 폰트"), WS_CHILD | WS_VISIBLE,
		10, 630, 120, 25, hBaseWnd, (HMENU)EF_NUM, g_hInst, NULL);
	hBtnApi = CreateWindow(_T("button"), _T("API 코드 복사"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, 300, 450, 120, 30, hBaseWnd, (HMENU)EF_API, g_hInst, NULL);
	hEdApi = CreateWindowEx(WS_EX_CLIENTEDGE, _T("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE,
		300, 490, 580, 60, hBaseWnd, (HMENU)EF_EDAPI, g_hInst, NULL);
	SendMessage(hEdApi, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	hBtnMfc = CreateWindow(_T("button"), _T("MFC 코드 복사"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, 300, 560, 120, 30, hBaseWnd, (HMENU)EF_MFC, g_hInst, NULL);
	hEdMfc = CreateWindowEx(WS_EX_CLIENTEDGE, _T("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE,
		300, 590, 580, 60, hBaseWnd, (HMENU)EF_EDMFC, g_hInst, NULL);
	SendMessage(hEdMfc, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	SendMessage(hChkSort, BM_SETCHECK, BST_CHECKED, 0);
	EnumFont();

	return FALSE;
}

LRESULT CEnumFontWin::OnDestroy(WPARAM wParam, LPARAM lParam) {
	RECT wrt;
	GetWindowRect(hBaseWnd, &wrt);
	setting.set(_T("EnumFont"), _T("left"), wrt.left);
	setting.set(_T("EnumFont"), _T("top"), wrt.top);

	return FALSE;
}

VOID CEnumFontWin::OnDraw(HDC hdc, INT width, INT height) {
	INT TabPos[] = { 250 };
	TCHAR str[256], str2[128];
	HFONT hFont, oldFont;

	SetBkMode(hdc, TRANSPARENT);

	wsprintf(str, _T("Height\t: %d"), arFont[nowFont].lfHeight);
	TabbedTextOut(hdc, 300, 20, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Width\t: %d"), arFont[nowFont].lfWidth);
	TabbedTextOut(hdc, 300, 40, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Escapement\t: %d"), arFont[nowFont].lfEscapement);
	TabbedTextOut(hdc, 300, 60, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Orientation\t: %d"), arFont[nowFont].lfOrientation);
	TabbedTextOut(hdc, 300, 80, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Weight\t: %d"), arFont[nowFont].lfWeight);
	TabbedTextOut(hdc, 300, 100, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Italic\t: %s"), arFont[nowFont].lfItalic ? _T("TRUE") : _T("FALSE"));
	TabbedTextOut(hdc, 300, 120, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Underline\t: %s"), arFont[nowFont].lfUnderline ? _T("TRUE") : _T("FALSE"));
	TabbedTextOut(hdc, 300, 140, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("StrikeOut\t: %s"), arFont[nowFont].lfStrikeOut ? _T("TRUE") : _T("FALSE"));
	TabbedTextOut(hdc, 300, 160, str, lstrlen(str), 1, TabPos, 0);

	GetCharSetStr(arFont[nowFont].lfCharSet, str2);
	wsprintf(str, _T("CharSet\t: %s"), str2);
	TabbedTextOut(hdc, 300, 180, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("OutPrecision\t: %d"), arFont[nowFont].lfOutPrecision);
	TabbedTextOut(hdc, 300, 200, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("ClipPrecision\t: %d"), arFont[nowFont].lfClipPrecision);
	TabbedTextOut(hdc, 300, 220, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("Quality\t: %d"), arFont[nowFont].lfQuality);
	TabbedTextOut(hdc, 300, 240, str, lstrlen(str), 1, TabPos, 0);

	GetPitchFamStr(arFont[nowFont].lfPitchAndFamily, str2);
	wsprintf(str, _T("PitchAndFamily\t: %s"), str2);
	TabbedTextOut(hdc, 300, 260, str, lstrlen(str), 1, TabPos, 0);

	wsprintf(str, _T("FaceName\t: %s"), arFont[nowFont].lfFaceName);
	TabbedTextOut(hdc, 300, 280, str, lstrlen(str), 1, TabPos, 0);

	// 폰트의 타입을 출력한다. 이 값은 LOGFONT 구조체에는 없다.
	switch (arFontType[nowFont]) {
	case 1:
		wsprintf(str, _T("FontType\t: RASTER_FONTTYPE"));
		break;
	case 2:
		wsprintf(str, _T("FontType\t: DEVICE_FONTTYPE"));
		break;
	case 4:
		wsprintf(str, _T("FontType\t: TRUETYPE_FONTTYPE"));
		break;
	default:
		wsprintf(str, _T("FontType\t: UnKnown(%d)"), arFontType[nowFont]);
	}
	TabbedTextOut(hdc, 300, 300, str, lstrlen(str), 1, TabPos, 0);

	// 폰트 샘플을 출력한다.
	lstrcpy(str, _T("English 한글 1234!@#$"));
	hFont = CreateFontIndirect(&arFont[nowFont]);
	oldFont = (HFONT)SelectObject(hdc, hFont);
	TextOutSz(hdc, 300, 340, str);
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

LRESULT CEnumFontWin::OnCommand(WPARAM wParam, LPARAM lParam) {
	TCHAR content[1024];

	switch (LOWORD(wParam)) {
	case EF_AT:
	case EF_SORT:
		EnumFont();
		break;
	case EF_LIST:
		switch (HIWORD(wParam)) {
		case LBN_SELCHANGE:
			nowFont = static_cast<INT>(SendMessage(hList, LB_GETCURSEL, 0, 0));
			SetCreateFontCommand();
			InvalidateRect(hBaseWnd, NULL, TRUE);
			break;
		}
		break;
	case EF_API:
		GetWindowText(hEdApi, content, 1024);
		CopyClipboardText(content);
		break;
	case EF_MFC:
		GetWindowText(hEdMfc, content, 1024);
		CopyClipboardText(content);
		break;
	}

	return FALSE;
}

VOID CEnumFontWin::EnumFont() {
	HDC hdc;
	INT i;

	numFont = 0;
	nowFont = -1;
	includeAt = (SendMessage(hChkAt, BM_GETCHECK, 0, 0) == BST_CHECKED);
	hdc = GetDC(mainWindow.hBaseWnd);
	EnumFontFamilies(hdc, NULL, (FONTENUMPROC)EnumFamCallBack, (LPARAM)this);
	ReleaseDC(mainWindow.hBaseWnd, hdc);

	if (SendMessage(hChkSort, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		qsort(&arFont[0], numFont, sizeof(LOGFONT), CEnumFontWin::compareFace);
	}

	// 발견된 모든 폰트의 타입 페이스를 리스트 박스에 출력한다.
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (i = 0; i < numFont; i++) {
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)arFont[i].lfFaceName);
	}

	TCHAR szNum[111];
	wsprintf(szNum, _T("발견된 폰트 : %d"), numFont);
	SetWindowText(hStNum, szNum);

	nowFont = 0;
	SendMessage(hList, LB_SETCURSEL, nowFont, 0);
	SendMessage(hBaseWnd, WM_COMMAND, MAKEWPARAM(EF_LIST, LBN_SELCHANGE), (LPARAM)hList);
	InvalidateRect(hBaseWnd, NULL, TRUE);
}

INT CALLBACK CEnumFontWin::EnumFamCallBack(ENUMLOGFONT FAR* lpelf, NEWTEXTMETRIC FAR* lpntm, INT FontType, LPARAM lParam) {
	CEnumFontWin* pWin = (CEnumFontWin*)lParam;

	if (pWin->includeAt == FALSE && _tcsrchr(lpelf->elfLogFont.lfFaceName, '@') != nullptr) {
		return TRUE;
	}

	if (pWin->numFont < CEnumFontWin::MAXFONT) {
		pWin->arFont[pWin->numFont] = lpelf->elfLogFont;
		pWin->arFontType[pWin->numFont] = FontType;
		pWin->numFont++;
		return TRUE;
	} else {
		return FALSE;
	}
}

INT CEnumFontWin::compareFace(const VOID* a, const VOID* b) {
	LOGFONT* aFont = (LOGFONT*)a;
	LOGFONT* bFont = (LOGFONT*)b;

	return lstrcmp(aFont->lfFaceName, bFont->lfFaceName);
}

VOID CEnumFontWin::GetCharSetStr(INT CharSet, TCHAR* str) {
	switch (CharSet) {
	case 0:lstrcpy(str, _T("ANSI_CHARSET")); break;
	case 1:lstrcpy(str, _T("DEFAULT_CHARSET")); break;
	case 2:lstrcpy(str, _T("SYMBOL_CHARSET")); break;
	case 128:lstrcpy(str, _T("SHIFTJIS_CHARSET")); break;
	case 129:lstrcpy(str, _T("HANGUL_CHARSET")); break;
	case 136:lstrcpy(str, _T("CHINESEBIG5_CHARSET")); break;
	case 255:lstrcpy(str, _T("OEM_CHARSET")); break;
	case 130:lstrcpy(str, _T("JOHAB_CHARSET")); break;
	case 177:lstrcpy(str, _T("HEBREW_CHARSET")); break;
	case 178:lstrcpy(str, _T("ARABIC_CHARSET")); break;
	case 161:lstrcpy(str, _T("GREEK_CHARSET")); break;
	case 162:lstrcpy(str, _T("TURKISH_CHARSET")); break;
	case 163:lstrcpy(str, _T("VIETNAMESE_CHARSET")); break;
	case 222:lstrcpy(str, _T("THAI_CHARSET")); break;
	case 238:lstrcpy(str, _T("EASTEUROPE_CHARSET")); break;
	case 204:lstrcpy(str, _T("RUSSIAN_CHARSET")); break;
	case 77:lstrcpy(str, _T("MAC_CHARSET")); break;
	case 186:lstrcpy(str, _T("BALTIC_CHARSET")); break;
	default:lstrcpy(str, _T("Etc")); break;
	}
}

VOID CEnumFontWin::GetPitchFamStr(INT Pitch, TCHAR* str) {
	switch (Pitch & 0x0f) {
	case 0:lstrcpy(str, _T("DEFAULT_PITCH"));
		break;
	case 1:lstrcpy(str, _T("FIXED_PITCH"));
		break;
	case 2:lstrcpy(str, _T("VARIABLE_PITCH"));
		break;
	default:lstrcpy(str, _T("Etc"));
	}
	switch (Pitch & 0xf0) {
	case 0x00:lstrcat(str, _T(" | FF_DONTCARE"));
		break;
	case 0x10:lstrcat(str, _T(" | FF_ROMAN"));
		break;
	case 0x20:lstrcat(str, _T(" | FF_SWISS"));
		break;
	case 0x30:lstrcat(str, _T(" | FF_MODERN"));
		break;
	case 0x40:lstrcat(str, _T(" | FF_SCRIPT"));
		break;
	case 0x50:lstrcat(str, _T(" | FF_DECORATIVE"));
		break;
	}
}

VOID CEnumFontWin::SetCreateFontCommand() {
	TCHAR Argu[128];
	TCHAR Api[1024];
	TCHAR Mfc[1024];
	TCHAR CharSetStr[32];
	TCHAR PitchFamStr[32];

	GetCharSetStr(arFont[nowFont].lfCharSet, CharSetStr);
	GetPitchFamStr(arFont[nowFont].lfPitchAndFamily, PitchFamStr);
	wsprintf(Argu, _T("(%d,0,0,0,0,0,0,0,%s,3,2,1,\r\n\t%s,TEXT(\"%s\"));"),
		arFont[nowFont].lfHeight, CharSetStr, PitchFamStr, arFont[nowFont].lfFaceName);

	wsprintf(Api, _T("hFont = CreateFont(%d,0,0,0,0,0,0,0,%s,3,2,1,"
		"\r\n\t%s,TEXT(\"%s\"));"),
		arFont[nowFont].lfHeight, CharSetStr, PitchFamStr, arFont[nowFont].lfFaceName);
	wsprintf(Api, _T("hFont=CreateFont%s"), Argu);
	SetWindowText(hEdApi, Api);
	wsprintf(Mfc, _T("Font.CreateFont%s\r\n"), Argu);
	SetWindowText(hEdMfc, Mfc);
}

LRESULT CSpy::OnCreate(WPARAM wParam, LPARAM lParam) {
	icon = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SPY));
	return FALSE;
}

LRESULT CSpy::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(icon);
	return FALSE;
}

VOID CSpy::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	StretchBitmap(hdc, 0, 0, wh, wh, icon);
}

VOID CSpy::OnClick() {
	CSpyWin* pWin = new CSpyWin();
	pWin->delegateDispose = true;

	CreateWindow(_T("CBaseWnd"), _T("Spy"),
		WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_CLIPCHILDREN,
		100, 100, 635, 650,
		NULL, (HMENU)NULL, g_hInst, pWin);
}

LRESULT CSpyWin::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case 0:
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CSpyWin::OnCreate(WPARAM wParam, LPARAM lParam) {
	INT left, top;
	left = setting.get(_T("Spy"), _T("left"), 100);
	top = setting.get(_T("Spy"), _T("top"), 100);
	SetWindowPos(hBaseWnd, NULL, left, top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	EnsureVisible(hBaseWnd);

	SetBackColor(GetSysColor(COLOR_BTNFACE));
	CreateWindow(_T("static"), _T("윈도우 정보"), WS_CHILD | WS_VISIBLE,
		10, 10, 300, 25, hBaseWnd, (HMENU)-1, g_hInst, NULL);
	hChkPause = CreateWindow(_T("button"), _T("일시 정지"), WS_CHILD | WS_VISIBLE |
		BS_AUTOCHECKBOX, 510, 10, 100, 25, hBaseWnd, (HMENU)SPY_PAUSE, g_hInst, NULL);
	hEditInfo = CreateWindowEx(WS_EX_CLIENTEDGE, _T("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL,
		10, 40, 600, 250, hBaseWnd, (HMENU)SPY_INFO, g_hInst, NULL);
	SendMessage(hEditInfo, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	CreateWindow(_T("static"), _T("윈도우 계층"), WS_CHILD | WS_VISIBLE,
		10, 310, 300, 25, hBaseWnd, (HMENU)-1, g_hInst, NULL);
	hEditHier = CreateWindowEx(WS_EX_CLIENTEDGE, _T("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL,
		10, 340, 600, 250, hBaseWnd, (HMENU)SPY_HIER, g_hInst, NULL);
	SendMessage(hEditHier, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	// 1초에 한번 조사하되 차후 사용자가 선택할 수 있도록 옵션화
	TimerInterval = 1000;
	SetTimer(hBaseWnd, 1, TimerInterval, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	return FALSE;
}

LRESULT CSpyWin::OnDestroy(WPARAM wParam, LPARAM lParam) {
	RECT wrt;
	GetWindowRect(hBaseWnd, &wrt);
	setting.set(_T("Spy"), _T("left"), wrt.left);
	setting.set(_T("Spy"), _T("top"), wrt.top);

	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CSpyWin::OnTimer(WPARAM wParam, LPARAM lParam) {
	POINT pt;
	RECT wrt, crt;
	HWND hWndPoint;
	HWND hWndParent;
	DWORD dwStyle, dwExStyle;
	TCHAR sCaption[256], sClass[256];
	TCHAR sStyle[256], sExStyle[256];
	TCHAR sLine[256];
	TCHAR sText[1024];

	GetCursorPos(&pt);
	GetWindowRect(hBaseWnd, &wrt);
	hWndPoint = WindowFromPoint(pt);
	BOOL pause = (SendMessage(hChkPause, BM_GETCHECK, 0, 0) == BST_CHECKED);

	// 이 윈도우 안이거나 이전 윈도우와 같거나 일시 정지 상태이면면 정보를 갱신하지 않는다.
	if (PtInRect(&wrt, pt) || hWndPoint == hWndOld || pause) return FALSE;

	hWndOld = hWndPoint;

	if (hWndPoint == NULL) {
		lstrcpy(sText, _T("윈도우가 없음. 데스크탑을 가리키는 경우임"));
	} else {
		GetWindowText(hWndPoint, sCaption, 256);
		GetClassName(hWndPoint, sClass, 256);
		hWndParent = GetParent(hWndPoint);
		GetWindowRect(hWndPoint, &crt);
		dwStyle = GetWindowLong(hWndPoint, GWL_STYLE);
		GetStyleString(dwStyle, sStyle);
		dwExStyle = GetWindowLong(hWndPoint, GWL_EXSTYLE);
		GetExStyleString(dwExStyle, sExStyle);

		wsprintf(sText,
			_T("핸들 : %d(0x%x)\r\n")
			_T("클래스 : %s\r\n")
			_T("캡션 : %s\r\n")
			_T("부모 : %d(0x%x)\r\n")
			_T("영역 : (%d,%d) - (%d,%d), %d*%d\r\n")
			_T("스타일 : 0X%X\r\n    %s\r\n")
			_T("확장 스타일 : 0X%X\r\n    %s\r\n"),
			(unsigned long long)hWndPoint, (unsigned long long)hWndPoint, sClass, sCaption, (unsigned long long)hWndParent, (unsigned long long)hWndParent,
			crt.left, crt.top, crt.right, crt.bottom,
			crt.right - crt.left, crt.bottom - crt.top,
			dwStyle, sStyle, dwExStyle, sExStyle);
	}
	SetWindowText(hEditInfo, sText);

	// todo : 윈도우 계층을 그래픽으로 보여 주고 계층 내의 다른 윈도우 정보 조사 기능 작성
	wstring ancestor = _T("");
	HWND hwnd = hWndPoint;
	for (;;) {
		hwnd = GetParent(hwnd);
		if (hwnd == NULL) {
			HWND hdesk = GetDesktopWindow();
			lstrcpy(sCaption, _T("Desktop"));
			GetWindowRect(hdesk, &crt);
		} else {
			GetWindowText(hwnd, sCaption, 256);
			GetWindowRect(hwnd, &crt);
		}
		wsprintf(sLine, _T("%d(%s) (%d,%d) - (%d,%d), %d*%d\r\n"),
			(unsigned long long)hwnd, sCaption,
			crt.left, crt.top, crt.right, crt.bottom,
			crt.right - crt.left, crt.bottom - crt.top
		);
		ancestor = sLine + ancestor;
		if (hwnd == NULL) break;
	}

	wstring childs = _T("");
	EnumChildWindows(hWndPoint, EnumChildProc, (LPARAM)&childs);

	ancestor = _T("조상 계층\r\n") + ancestor + _T("\r\n자식 목록\r\n") + childs;
	SetWindowText(hEditHier, ancestor.c_str());

	return FALSE;
}

#define TEST(n) (dwStyle & (1 << n))
VOID CSpyWin::GetStyleString(DWORD dwStyle, TCHAR* sStyle) {
	if (TEST(31)) lstrcpy(sStyle, _T("POPUP"));
	else if (TEST(30)) lstrcpy(sStyle, _T("CHILD"));
	else lstrcpy(sStyle, _T("OVERLAPPED"));

	if (TEST(29)) lstrcat(sStyle, _T(" | MINIMIZE"));
	if (TEST(28)) lstrcat(sStyle, _T(" | VISIBLE"));
	if (TEST(27)) lstrcat(sStyle, _T(" | DISABLED"));
	if (TEST(26)) lstrcat(sStyle, _T(" | CLIPSIBLINGS"));
	if (TEST(25)) lstrcat(sStyle, _T(" | CLIPCHILDREN"));
	if (TEST(24)) lstrcat(sStyle, _T(" | MAXIMIZE"));
	if (TEST(23)) lstrcat(sStyle, _T(" | BORDER"));
	if (TEST(22)) lstrcat(sStyle, _T(" | DLGFRAME"));
	// lstrcat(sStyle, _T("\r\n    "));
	if (TEST(21)) lstrcat(sStyle, _T(" | VSCROLL"));
	if (TEST(20)) lstrcat(sStyle, _T(" | HSCROLL"));
	if (TEST(19)) lstrcat(sStyle, _T(" | SYSMENU"));
	if (TEST(18)) lstrcat(sStyle, _T(" | THICKFRAME"));
	if (TEST(17)) lstrcat(sStyle, _T(" | MINIMIZEBOX"));
	if (TEST(16)) lstrcat(sStyle, _T(" | MAXIMIZEBOX"));
}

VOID CSpyWin::GetExStyleString(DWORD dwExStyle, TCHAR* sExStyle) {
	if (dwExStyle & 0x00001000L) lstrcpy(sExStyle, _T("RIGHT"));
	else lstrcpy(sExStyle, _T("LEFT"));

	if (dwExStyle & 0x00000010L) lstrcat(sExStyle, _T(" | ACCEPTFILES"));
	if (dwExStyle & 0x00040000L) lstrcat(sExStyle, _T(" | APPWINDOW"));
	if (dwExStyle & 0x00000200L) lstrcat(sExStyle, _T(" | CLIENTEDGE"));
	if (dwExStyle & 0x02000000L) lstrcat(sExStyle, _T(" | COMPOSITED"));
	if (dwExStyle & 0x00000400L) lstrcat(sExStyle, _T(" | CONTEXTHELP"));
	if (dwExStyle & 0x00010000L) lstrcat(sExStyle, _T(" | CONTROLPARENT"));
	if (dwExStyle & 0x00000001L) lstrcat(sExStyle, _T(" | DLGMODALFRAME"));
	if (dwExStyle & 0x00080000L) lstrcat(sExStyle, _T(" | LAYERED"));
	if (dwExStyle & 0x00400000L) lstrcat(sExStyle, _T(" | LAYOUTRTL"));
	if (dwExStyle & 0x00004000L) lstrcat(sExStyle, _T(" | LEFTSCROLLBAR"));
	if (dwExStyle & 0x00000040L) lstrcat(sExStyle, _T(" | MDICHILD"));
	if (dwExStyle & 0x08000000L) lstrcat(sExStyle, _T(" | NOACTIVATE"));
	if (dwExStyle & 0x00100000L) lstrcat(sExStyle, _T(" | NOINHERITLAYOUT"));
	if (dwExStyle & 0x00000004L) lstrcat(sExStyle, _T(" | NOPARENTNOTIFY"));
	if (dwExStyle & 0x00200000L) lstrcat(sExStyle, _T(" | NOREDIRECTIONBITMAP"));
	if (dwExStyle & 0x00002000L) lstrcat(sExStyle, _T(" | RTLREADING"));
	if (dwExStyle & 0x00020000L) lstrcat(sExStyle, _T(" | STATICEDGE"));
	if (dwExStyle & 0x00000080L) lstrcat(sExStyle, _T(" | TOOLWINDOW"));
	if (dwExStyle & 0x00000008L) lstrcat(sExStyle, _T(" | TOPMOST"));
	if (dwExStyle & 0x00000020L) lstrcat(sExStyle, _T(" | TRANSPARENT"));
	if (dwExStyle & 0x00000100L) lstrcat(sExStyle, _T(" | WINDOWEDGE"));
}

BOOL CALLBACK CSpyWin::EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam) {
	TCHAR sCaption[256];
	TCHAR sLine[256];
	RECT crt;
	wstring* pChilds = (wstring*)lParam;

	GetWindowText(hwnd, sCaption, 256);
	GetWindowRect(hwnd, &crt);
	wsprintf(sLine, _T("%d(%s) (%d,%d) - (%d,%d), %d*%d\r\n"),
		(unsigned long long)hwnd, sCaption,
		crt.left, crt.top, crt.right, crt.bottom,
		crt.right - crt.left, crt.bottom - crt.top
	);
	(*pChilds) += sLine;

	return TRUE;
}

LRESULT CSwapMouse::OnCreate(WPARAM wParam, LPARAM lParam) {
	bitOn = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SWAPMOUSEON));
	bitOff = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SWAPMOUSEOFF));

	// 따로 저장하지 않고 윈도우 설정에서 현재 상태를 읽어 온다.
	swapButton = GetSystemMetrics(SM_SWAPBUTTON);
	return FALSE;
}

VOID CSwapMouse::OnDraw(HDC hdc, INT width, INT height) {
	INT wh = trayWindow.tidgetSize;
	if (swapButton) {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOn);
	} else {
		StretchBitmap(hdc, 0, 0, wh, wh, bitOff);
	}
}

VOID CSwapMouse::OnClick() {
	// 좌우 교체 상태를 토글한다. 윈도우 설정에 반영하므로 따로 저장할 필요는 없다.
	swapButton = !swapButton;
	SystemParametersInfo(SPI_SETMOUSEBUTTONSWAP, swapButton, NULL, SPIF_UPDATEINIFILE);
	InvalidateRect(hBaseWnd, NULL, FALSE);
}
