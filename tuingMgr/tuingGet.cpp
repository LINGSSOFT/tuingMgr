#include "framework.h"

// 클래스명으로부터 위젯 객체 생성. 타입명과 객체를 연결한다.
CTuingGet* CTuingGet::CreateObject(wstring cls) {
	if (cls == _T("Clock")) return new CClock();
	if (cls == _T("Radix")) return new CRadix();
	if (cls == _T("CpuUsage")) return new CCpuUsage();
	if (cls == _T("MemUsage")) return new CMemUsage();
	if (cls == _T("Battery")) return new CBattery();
	if (cls == _T("DiskUsage")) return new CDiskUsage();
	if (cls == _T("Calc")) return new CCalc();
	if (cls == _T("MusicPlayer")) return new CMusicPlayer();
	if (cls == _T("ImageSlide")) return new CImageSlide();
	if (cls == _T("Memo")) return new CMemo();
	if (cls == _T("Dday")) return new CDday();
	if (cls == _T("Launcher")) return new CLauncher();
	if (cls == _T("Calendar")) return new CCalendar();

	return nullptr;
}

// 디폴트 오브젝트를 생성하고 초기 속성을 대입한다.
CTuingGet* CTuingGet::CreateDefaultObject(wstring cls) {
	CTuingGet* pTuingget = CreateObject(cls);

	INT wi;
	for (wi = 0;; wi++) {
		if (arWidgetInfo[wi].cls == _T("")) return nullptr;
		if (arWidgetInfo[wi].cls == cls) break;
	}

	pTuingget->baseget_cls = cls;
	// 멀티를 허용하지 않으면 클래스명을 그대로 사용한다.
	if (arWidgetInfo[wi].allowMulti == FALSE) {
		pTuingget->baseget_id = cls;
	} else {
		BOOL bFind;
		wstring id;
		// 일련 번호를 증가시켜 가면 빈 번호를 찾는다. 
		for (INT num = 1;; num++) {
			// 첫 위젯이면 클래스명을 그대로 사용한다. 
			if (num == 1) {
				id = cls;
			} else {
				TCHAR tnum[10];
				wsprintf(tnum, _T("%d"), num);
				id = cls + _T("_") + tnum;
			}
			// 이미 생성한 위젯중에 id를 사용하는 위젯이 있는지 조사한다.
			bFind = FALSE;
			for (size_t wi = 0; wi < arWidget.size(); wi++) {
				if (arWidget[wi]->baseget_id == id) {
					bFind = TRUE;
					break;
				}
			}
			if (bFind == FALSE) break;
		}

		pTuingget->baseget_id = id;
	}

	pTuingget->baseget_visible = TRUE;
	pTuingget->vertHeight = arWidgetInfo[wi].initHeight;
	pTuingget->horzWidth = arWidgetInfo[wi].initWidth;
	pTuingget->SetBackColor(arWidgetInfo[wi].initColor);
	pTuingget->span = FALSE;
	pTuingget->spanRatio = 100;

	return pTuingget;
}

VOID CTuingGet::Serialize(TCHAR* pack) {
	wsprintf(pack, _T("%s§%s§%d§%d§%d§%d§%d§%d§%d"),
		baseget_cls.c_str(), baseget_id.c_str(), BaseBackColor, baseget_visible,
		vertHeight, horzWidth, span, spanRatio);
}

CTuingGet* CTuingGet::Deserialize(LPCTSTR pack) {
	TCHAR token[MAX_PATH];
	GetPackToken(pack, 0, token);
	CTuingGet* pTuingget = CreateObject(token);
	pTuingget->baseget_cls = token;
	GetPackToken(pack, 1, token); pTuingget->baseget_id = token;
	GetPackToken(pack, 2, token);
	pTuingget->SetBackColor(_ttoi(token));
	GetPackToken(pack, 3, token); pTuingget->baseget_visible = _ttoi(token);
	GetPackToken(pack, 4, token); pTuingget->vertHeight = _ttoi(token);
	GetPackToken(pack, 5, token); pTuingget->horzWidth = _ttoi(token);
	GetPackToken(pack, 6, token); pTuingget->span = _ttoi(token);
	GetPackToken(pack, 7, token); pTuingget->spanRatio = _ttoi(token);

	return pTuingget;
}

// § 마크로 구분된 문자열에서 idx 번째 토큰을 읽는다. 
BOOL CTuingGet::GetPackToken(LPCTSTR pack, INT idx, TCHAR token[MAX_PATH]) {
	LPCTSTR p = pack;
	LPCTSTR pEnd;
	// idx 번째 § 마크를 찾는다.
	for (INT i = 0;; i++) {
		if (i == idx) break;
		pEnd = _tcsstr(p, _T("§"));
		if (pEnd == nullptr) return FALSE;
		p = pEnd + 1;
	}

	// p 부터 다음 § 또는 문자열 끝까지 읽는다.
	memset(token, 0, sizeof(TCHAR) * MAX_PATH);
	pEnd = _tcsstr(p, _T("§"));
	if (pEnd != nullptr) {
		_tcsncpy_s(token, MAX_PATH, p, pEnd - p);
	} else {
		lstrcpy(token, p);
	}

	return TRUE;
}

LRESULT CClock::OnCreate(WPARAM wParam, LPARAM lParam) {
	LoadSetting();
	SetTimer(hBaseWnd, 1, 1000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	return FALSE;
}

LRESULT CClock::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CClock::OnTimer(WPARAM wParam, LPARAM lParam) {
	SYSTEMTIME st;
	GetLocalTime(&st);
	static TCHAR arYoil[][4] = { _T("일"), _T("월"), _T("화"), _T("수"),
		_T("목"), _T("금"), _T("토") };
	static TCHAR arYoilEng[][16] = { _T("Sun"), _T("Mon"), _T("Tue"),
		_T("Wen"), _T("Thr"), _T("Fri"), _T("Sat") };

	month = st.wMonth;
	day = st.wDay;
	if (lang == eLang::KOR) {
		yoil = arYoil[st.wDayOfWeek];
	} else {
		yoil = arYoilEng[st.wDayOfWeek];
	}

	BOOL needRepaint;
	if (outSec) {
		needRepaint = (hour != st.wHour || min != st.wMinute || sec != st.wSecond);
	} else {
		needRepaint = (hour != st.wHour || min != st.wMinute);
	}

	hour = st.wHour;
	min = st.wMinute;
	sec = st.wSecond;

	// 표시할 시간이 실제로 바뀌었을 때만 다시 그린다.
	if (needRepaint) InvalidateRect(hBaseWnd, NULL, TRUE);

	return FALSE;
}

VOID CClock::OnDraw(HDC hdc, INT width, INT height) {
	TCHAR sTime[128];
	HFONT oldFont;

	if (faceType == 0) {
		LPCTSTR ampm;
		INT center = height / 2 - 3;

		SetBkMode(hdc, TRANSPARENT);
		SetTextAlign(hdc, TA_CENTER);
		_stprintf_s(sTime, 128, _T("%d월%d일 %s"), month, day, yoil.c_str());

		oldFont = (HFONT)SelectObject(hdc, mainFont[3]);
		TextOutSz(hdc, width / 2, center - 20, sTime);

		// 초를 표시하지 않으면 폰트를 조금 더 키운다.
		SelectObject(hdc, mainFont[outSec ? 4 : 5]);

		INT tHour = hour;
		if (hour24 == FALSE) {
			// 자정은 0시, 정오는 12시로 표기한다.
			if (tHour > 12) {
				tHour -= 12;
			}
		}

		TCHAR sDigit[128];
		if (digit2) {
			lstrcpy(sDigit, _T("%02d"));
		} else {
			lstrcpy(sDigit, _T("%d"));
		}

		TCHAR sFormat[128];
		lstrcpy(sFormat, sDigit);
		lstrcat(sFormat, _T(":"));
		lstrcat(sFormat, sDigit);
		if (outSec) {
			lstrcat(sFormat, _T(":"));
			lstrcat(sFormat, sDigit);
			wsprintf(sTime, sFormat, tHour, min, sec);
		} else {
			wsprintf(sTime, sFormat, tHour, min);
		}
		TextOutSz(hdc, width / 2, center, sTime);

		// 시간 앞에 기본 폰트로 오전/오후를 붙인다.
		if (hour24 == FALSE) {
			SIZE sz;
			GetTextExtentPoint32(hdc, sTime, lstrlen(sTime), &sz);
			SetTextAlign(hdc, TA_RIGHT | TA_BASELINE);
			SelectObject(hdc, mainFont[1]);
			SetTextColor(hdc, RGB(128, 128, 128));
			if (lang == eLang::KOR) {
				ampm = tHour >= 12 ? _T("오후") : _T("오전");
			} else {
				ampm = tHour >= 12 ? _T("PM") : _T("AM");
			}
			TextOutSz(hdc, width / 2 - sz.cx / 2 - 3, center + sz.cy - 8, ampm);
		}
		SelectObject(hdc, oldFont);
	}

	if (faceType == 1) {
		INT cx = width / 2;
		INT cy = height / 2;
		// 짧은쪽 길이의 90%폭으로 그린다.
		INT radius = INT(min(cx, cy) * 0.9);
		INT timeR;
		HPEN pen, oldPen;
		INT rx, ry;

		// 각 시간마다 원을 그린다. 
		for (INT h = 0; h <= 11; h++) {
			getRadialPos(cx, cy, radius, h, rx, ry);
			if (h % 3 == 0) {
				timeR = 3;
			} else {
				timeR = 2;
			}
			Ellipse(hdc, rx - timeR, ry - timeR, rx + timeR, ry + timeR);
		}

		// 날짜와 요일을 표시한다. 시계 바늘보다 더 아래에 그린다.
		SetBkMode(hdc, TRANSPARENT);
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
		SetTextAlign(hdc, TA_CENTER);
		wsprintf(sTime, _T("%d"), month);
		TextOutVCenter(hdc, cx - radius / 2, cy - 7, sTime);
		wsprintf(sTime, _T("%d"), day);
		TextOutVCenter(hdc, cx - radius / 2, cy + 7, sTime);
		TextOutVCenter(hdc, cx + radius / 2, cy, yoil.c_str());
		SelectObject(hdc, oldFont);

		INT tx, ty;
		// 시침 좌표. 시간 뿐만 아니라 분값도 더한다. 30분은 0.5시간에 해당한다.
		pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, pen);
		getRadialPos(cx, cy, int(radius * 0.6), hour + min / 60.0, tx, ty);
		MoveToEx(hdc, cx, cy, NULL); LineTo(hdc, tx, ty);
		DeleteObject(SelectObject(hdc, oldPen));

		// 분침 좌표. 초를 더해 계산한다. 60진법의 분을 12진법의 시 단위로 바꾸려면 5로 나눈다.
		pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, pen);
		getRadialPos(cx, cy, INT(radius * 0.8), (min + sec / 60.0) / 5, tx, ty);
		MoveToEx(hdc, cx, cy, NULL); LineTo(hdc, tx, ty);
		DeleteObject(SelectObject(hdc, oldPen));

		// 초침 좌표
		if (outSec) {
			pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
			oldPen = (HPEN)SelectObject(hdc, pen);
			getRadialPos(cx, cy, INT(radius * 0.8), sec / 5.0, tx, ty);
			MoveToEx(hdc, cx, cy, NULL); LineTo(hdc, tx, ty);
			DeleteObject(SelectObject(hdc, oldPen));
		}
	}
}

LRESULT CClock::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_CLOCK),
			hBaseWnd, ClockDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

// 시간값에 대한 원주상의 좌표를 구한다.
VOID CClock::getRadialPos(INT cx, INT cy, INT radius, DOUBLE hour, INT& x, INT& y) {
	// 시간을 각도로 바꾼다. 음수 각도가 나오면 360을 더해 양수로 바꾼다.
	DOUBLE angle = -30 * hour + 90;
	if (angle < 0) angle += 360;
	// 각도를 라디안으로 바꾼다.
	DOUBLE radian = angle * 3.1415 / 180;

	// 모니터가 1/4분면이므로 y 좌표는 뺀다.
	x = INT(cx + radius * cos(radian));
	y = INT(cy - radius * sin(radian));
}

VOID CClock::LoadSetting() {
	faceType = setting.get(baseget_id, _T("faceType"), 0);
	hour24 = setting.get(baseget_id, _T("hour24"), FALSE);
	outSec = setting.get(baseget_id, _T("outSec"), TRUE);
	digit2 = setting.get(baseget_id, _T("digit2"), TRUE);
}

VOID CClock::SaveSetting() {
	setting.set(baseget_id, _T("faceType"), faceType);
	setting.set(baseget_id, _T("hour24"), hour24);
	setting.set(baseget_id, _T("outSec"), outSec);
	setting.set(baseget_id, _T("digit2"), digit2);
}

INT_PTR CALLBACK CClock::ClockDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CClock* pTuingget;
	INT radio = 0;
	static LPCTSTR arFace[] = { _T("디지털"),_T("아날로그") };
	ULONGLONG tick;
	INT Min, Hour, Day;
	TCHAR str[255];
	SYSTEMTIME st, boot;
	DWORD abs;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CClock*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		for (INT i = 0; i < Count(arFace); i++) {
			SendDlgItemMessage(hDlg, IDC_CLOCKFACE, LB_ADDSTRING, 0, (LPARAM)arFace[i]);
		}

		SendDlgItemMessage(hDlg, IDC_CLOCKFACE, LB_SETCURSEL, pTuingget->faceType, 0);
		SendDlgItemMessage(hDlg, IDC_CHKHOUR24, BM_SETCHECK, pTuingget->hour24 ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKOUTSEC, BM_SETCHECK, pTuingget->outSec ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKDIGIT2, BM_SETCHECK, pTuingget->digit2 ? BST_CHECKED : BST_UNCHECKED, 0);

		tick = GetTickCount64() / 1000;
		Day = INT(tick / 86400);
		Hour = (tick % 86400) / 3600;
		Min = (tick % 3600) / 60;

		GetLocalTime(&st);
		abs = MyGetAbsSecond(st);
		MyAbsSecondToSystem(DWORD(abs - tick), boot);

		wsprintf(str, _T("%d/%d일 %d:%d분 부팅(%d일 %d시간 %d분)"), boot.wMonth, boot.wDay, boot.wHour, boot.wMinute, Day, Hour, Min);
		SetDlgItemText(hDlg, IDC_STBOOT, str);

		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			pTuingget->hour24 = IsDlgButtonChecked(hDlg, IDC_CHKHOUR24);
			pTuingget->outSec = IsDlgButtonChecked(hDlg, IDC_CHKOUTSEC);
			pTuingget->digit2 = IsDlgButtonChecked(hDlg, IDC_CHKDIGIT2);
			pTuingget->faceType = (INT)SendDlgItemMessage(hDlg, IDC_CLOCKFACE, LB_GETCURSEL, 0, 0);
			InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);

			// 세팅을 저장한다. 매번 저장하는 것보다는 세팅 변경시 저장하는게 더 나아 보인다. 
			pTuingget->SaveSetting();

			// 여기서 파괴하면 WM_NCACTIVATE 재진입함. 재진입 조건을 먼저 없앤 후 파괴해야 한다.
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			// ESC 버튼을 누르면 저장없이 취소한다.
		case IDCANCEL:
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CRadix::OnCreate(WPARAM wParam, LPARAM lParam) {
	editByCode = FALSE;

	// 각 에디트의 ID를 진법으로 지정한다.
	hEdit2 = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 0, 0, 0, 0, hBaseWnd, (HMENU)2, g_hInst, NULL);
	hEdit10 = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 0, 0, 0, 0, hBaseWnd, (HMENU)10, g_hInst, NULL);
	hEdit16 = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 0, 0, 0, 0, hBaseWnd, (HMENU)16, g_hInst, NULL);
	return FALSE;
}

LRESULT CRadix::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);
	INT width = LOWORD(lParam);
	INT height = HIWORD(lParam);
	INT center;

	// 2진수와 10진수 사이의 여백 중간쯤을 중앙 좌표로 잡는다.
	// 폭이 100보다 작으면 이보다 13픽셀 더 위쪽으로 옮겨 셋다 수직 중앙 정렬한다.
	if (width > 100) {
		center = height / 2 + 3;
	} else {
		center = height / 2 - 10;
	}

	// 수직 중앙 기준 2진수는 위로 25올리고 10, 16진수는 중앙에 맞춤. 높이는 모두 20
	// 2진수 왼쪽은 20, 오른쪽 5만큼 띄우고 10진수는 절반까지, 16진수는 나머지 절반 차지
	MoveWindow(hEdit2, 20, center - 25, width - 25, 20, TRUE);
	if (width > 100) {
		MoveWindow(hEdit10, 20, center, width / 2 - 20, 20, TRUE);
		MoveWindow(hEdit16, width / 2 + 25, center, width / 2 - 30, 20, TRUE);
	} else {
		// 폭이 100보다 작으면 10, 16진수를 수직으로 나란히 정렬한다.
		MoveWindow(hEdit10, 20, center, width - 25, 20, TRUE);
		MoveWindow(hEdit16, 20, center + 25, width - 25, 20, TRUE);
	}
	SendMessage(hEdit2, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	SendMessage(hEdit10, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	SendMessage(hEdit16, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	return FALSE;
}

VOID CRadix::OnDraw(HDC hdc, INT width, INT height) {
	HFONT oldFont;
	INT center;

	if (width > 100) {
		center = height / 2 + 3;
	} else {
		center = height / 2 - 10;
	}

	oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_CENTER);

	// 캡션은 직접 그린다. 
	TextOutVCenter(hdc, 10, center - 12, _T("2"));
	TextOutVCenter(hdc, 10, center + 10, _T("10"));
	if (width > 100) {
		TextOutVCenter(hdc, width / 2 + 15, center + 10, _T("16"));
	} else {
		TextOutVCenter(hdc, 10, center + 35, _T("16"));
	}
	SelectObject(hdc, oldFont);
}

LRESULT CRadix::OnCommand(WPARAM wParam, LPARAM lParam) {
	INT num;
	TCHAR str[128];

	switch (LOWORD(wParam)) {
	case 10:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			if (editByCode == FALSE) {
				editByCode = TRUE;
				num = GetDlgItemInt(hBaseWnd, 10, NULL, FALSE);
				memset(str, 0, 128 * sizeof(TCHAR));
				_itot_s<128>(num, str, 16);
				SetDlgItemText(hBaseWnd, 16, str);
				_itot_s<128>(num, str, 2);
				SetDlgItemText(hBaseWnd, 2, str);
				editByCode = FALSE;
			}
			return TRUE;
		}
		break;
	case 16:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			if (editByCode == FALSE) {
				editByCode = TRUE;
				GetDlgItemText(hBaseWnd, 16, str, 128);
				num = _tcstoul(str, NULL, 16);
				SetDlgItemInt(hBaseWnd, 10, num, FALSE);
				_itot_s<128>(num, str, 2);
				SetDlgItemText(hBaseWnd, 2, str);
				editByCode = FALSE;
			}
			return TRUE;
		}
		break;
	case 2:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			if (editByCode == false) {
				editByCode = TRUE;
				GetDlgItemText(hBaseWnd, 2, str, 128);
				num = _tcstoul(str, NULL, 2);
				SetDlgItemInt(hBaseWnd, 10, num, FALSE);
				_itot_s<128>(num, str, 16);
				SetDlgItemText(hBaseWnd, 16, str);
				editByCode = FALSE;
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CCpuUsage::OnCreate(WPARAM wParam, LPARAM lParam) {
	LoadSetting();
	SetTimer(hBaseWnd, 1, timerInterval * 1000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, NULL);
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, _T("\\Processor(_Total)\\% Processor Time"), NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);

	memset(arUsage, 0, sizeof(arUsage));
	return FALSE;
}

LRESULT CCpuUsage::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CCpuUsage::OnTimer(WPARAM wParam, LPARAM lParam) {
	PDH_FMT_COUNTERVALUE counterVal;

	// 마지막 칸에 최근값 조사
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

	// 하나씩 앞쪽으로 이동하고 마지막에 최근값 기록
	for (INT i = 1; i < USAGE_NUM; i++) {
		arUsage[i - 1] = arUsage[i];
	}
	arUsage[USAGE_NUM - 1] = counterVal.doubleValue;

	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

VOID CCpuUsage::OnDraw(HDC hdc, INT width, INT height) {
	// 60개의 점유율 + 우하, 좌하, 좌상으로 복귀
	POINT pts[USAGE_NUM + 3];
	DOUBLE Min = 100, Max = 0, Sum = 0, Avg;
	HPEN hPen, OldPen;
	HBRUSH hBrush, OldBrush;
	HFONT OldFont;
	TCHAR sUsage[128];
	TCHAR yMax[10];
	RECT trt;
	RECT grt;
	INT twidth, theight;
	INT tleft, tright;

	SetBkMode(hdc, TRANSPARENT);

	// 트렌드 사각형
	SetRect(&trt, 18, 5, width - 5, height - 35);
	// 그래프 영역은 트렌드 사각형보다 1픽셀 더 안쪽
	CopyRect(&grt, &trt);
	InflateRect(&grt, -1, -1);
	twidth = (grt.right - grt.left);
	theight = (grt.bottom - grt.top);

	// 트렌드 사각형을 그린다.
	hPen = CreatePen(PS_SOLID, 1, RGB(16, 80, 160));
	OldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(240, 240, 255));
	OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, trt.left, trt.top, trt.right, trt.bottom);
	SelectObject(hdc, OldPen);
	DeleteObject(hPen);
	SelectObject(hdc, OldBrush);
	DeleteObject(hBrush);

	// 그래프 영역에 바둑판을 그힌다.
	hPen = CreatePen(PS_SOLID, 1, RGB(220, 220, 255));
	OldPen = (HPEN)SelectObject(hdc, hPen);
	for (INT i = 1; i <= 3; i++) {
		MoveToEx(hdc, trt.left + 1, trt.top + theight * i / 4, NULL);
		LineTo(hdc, trt.right - 1, trt.top + theight * i / 4);
	}
	for (INT i = 1; i <= 5; i++) {
		MoveToEx(hdc, trt.left + twidth * i / 6, trt.top + 1, NULL);
		LineTo(hdc, trt.left + twidth * i / 6, trt.bottom - 1);
	}
	SelectObject(hdc, OldPen);
	DeleteObject(hPen);

	// 통계를 구한다. 평균에서 0%는 아직 조사되지 않은 값이어서 제외한다.
	INT validCount = 0;
	for (INT i = 0; i < USAGE_NUM; i++) {
		Min = min(Min, arUsage[i]);
		Max = max(Max, arUsage[i]);
		Sum = Sum + arUsage[i];
		if (arUsage[i] != 0) validCount++;
	}
	if (validCount == 0) {
		Avg = 0;
	} else {
		Avg = Sum / validCount;
	}

	// 가변 Y축 옵션 적용. 전부 40% 미만이면 50까지, 20미만이면 25까지, 10미만이면 12를 y축 범위로 사용한다.
	// 고정 Y축이면 무조건 100%이다.
	int topValue = 100;
	if (varYAxis) {
		if (Max < 10) {
			topValue = 12;
		} else if (Max < 20) {
			topValue = 25;
		} else if (Max < 40) {
			topValue = 50;
		}
	}
	wsprintf(yMax, _T("%d"), topValue);

	// 다각형 생성. topValue를 사각형 상단에 맞춘다. 
	for (INT i = 0; i < USAGE_NUM; i++) {
		pts[i].x = LONG(grt.left + (DOUBLE)twidth * i / ((DOUBLE)USAGE_NUM - 1));
		pts[i].y = LONG(grt.top + theight - theight * arUsage[i] / topValue);
	}
	pts[USAGE_NUM].x = pts[USAGE_NUM - 1].x;
	pts[USAGE_NUM].y = grt.top + theight;
	pts[USAGE_NUM + 1].x = grt.left;
	pts[USAGE_NUM + 1].y = grt.top + theight;
	pts[USAGE_NUM + 2].x = pts[0].x;
	pts[USAGE_NUM + 2].y = pts[0].y;

	// 다각형을 그린다.
	Polygon(hdc, pts, USAGE_NUM + 3);

	// Y축 레이블을 출력한다.
	OldFont = (HFONT)SelectObject(hdc, mainFont[0]);
	UINT oldAlign = SetTextAlign(hdc, TA_RIGHT);
	TextOutVCenter(hdc, 16, trt.bottom, _T("0"));
	TextOutVCenter(hdc, 16, trt.top, yMax);
	SetTextAlign(hdc, oldAlign);

	// 트렌드의 좌우 영역에 맞추되 폭이 좁으면 벽에 더 바짝 붙인다.
	tleft = trt.left;
	tright = width - 3;
	if (width < 120) {
		tleft = 2;
		tright = width;
	}

	// 통계값은 정수로 변환하여 출력한다.
	OldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	wsprintf(sUsage, _T("범위:%d~%d"), (INT)Min, (INT)Max);
	TextOutSz(hdc, tleft, trt.bottom + 2, sUsage);
	wsprintf(sUsage, _T("평균:%d%%"), (INT)Avg);
	TextOutSz(hdc, tleft, trt.bottom + 18, sUsage);
	SelectObject(hdc, OldFont);
	OldFont = (HFONT)SelectObject(hdc, mainFont[2]);
	oldAlign = SetTextAlign(hdc, TA_RIGHT);
	SetTextColor(hdc, RGB(0, 128, 0));
	wsprintf(sUsage, _T("%d%%"), (INT)arUsage[USAGE_NUM - 1]);
	TextOutSz(hdc, tright, trt.bottom + 5, sUsage);
	SetTextAlign(hdc, oldAlign);
	SelectObject(hdc, OldFont);
}

LRESULT CCpuUsage::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_CPUUSAGE),
			hBaseWnd, CpuUsageDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

VOID CCpuUsage::LoadSetting() {
	timerInterval = setting.get(baseget_id, _T("timerInterval"), 5);
	varYAxis = setting.get(baseget_id, _T("varYAxis"), 1);
	viewThread = setting.get(baseget_id, _T("viewThread"), 0);
}

VOID CCpuUsage::SaveSetting() {
	setting.set(baseget_id, _T("timerInterval"), timerInterval);
	setting.set(baseget_id, _T("varYAxis"), varYAxis);
	setting.set(baseget_id, _T("viewThread"), viewThread);
}

INT_PTR CALLBACK CCpuUsage::CpuUsageDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CCpuUsage* pTuingget;
	INT radio = 0;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CCpuUsage*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		if (pTuingget->timerInterval == 1) radio = IDC_CPUTIME1;
		if (pTuingget->timerInterval == 2) radio = IDC_CPUTIME2;
		if (pTuingget->timerInterval == 5) radio = IDC_CPUTIME3;
		if (pTuingget->timerInterval == 10) radio = IDC_CPUTIME4;
		CheckRadioButton(hDlg, IDC_CPUTIME1, IDC_CPUTIME4, radio);

		SendDlgItemMessage(hDlg, IDC_CHKVARYAXIS, BM_SETCHECK,
			pTuingget->varYAxis ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKVIEWTHREAD, BM_SETCHECK,
			pTuingget->viewThread ? BST_CHECKED : BST_UNCHECKED, 0);

		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			if (IsDlgButtonChecked(hDlg, IDC_CPUTIME1)) pTuingget->timerInterval = 1;
			if (IsDlgButtonChecked(hDlg, IDC_CPUTIME2)) pTuingget->timerInterval = 2;
			if (IsDlgButtonChecked(hDlg, IDC_CPUTIME3)) pTuingget->timerInterval = 5;
			if (IsDlgButtonChecked(hDlg, IDC_CPUTIME4)) pTuingget->timerInterval = 10;
			SetTimer(pTuingget->hBaseWnd, 1, pTuingget->timerInterval * 1000, NULL);
			pTuingget->varYAxis = IsDlgButtonChecked(hDlg, IDC_CHKVARYAXIS);
			pTuingget->viewThread = IsDlgButtonChecked(hDlg, IDC_CHKVIEWTHREAD);
			InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);

			pTuingget->SaveSetting();

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

LRESULT CMemUsage::OnCreate(WPARAM wParam, LPARAM lParam) {
	timerInterval = 10;
	SetTimer(hBaseWnd, 1, timerInterval * 1000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	return FALSE;
}

LRESULT CMemUsage::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CMemUsage::OnTimer(WPARAM wParam, LPARAM lParam) {
	mem.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&mem);
	InvalidateRect(hBaseWnd, NULL, FALSE);
	return FALSE;
}

VOID CMemUsage::OnDraw(HDC hdc, INT width, INT height) {
	TCHAR str[128];
	HFONT oldFont;
	UINT oldAlign;
	DOUBLE total = mem.ullTotalPhys / 1073741824.0;
	DOUBLE avail = mem.ullAvailPhys / 1073741824.0;
	DOUBLE used = total - avail;
	INT center = height / 2;

	SetBkMode(hdc, TRANSPARENT);
	if (width < 100) {
		oldFont = (HFONT)SelectObject(hdc, mainFont[0]);
	} else {
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	}
	oldAlign = SetTextAlign(hdc, TA_BASELINE);
	_stprintf_s(str, _T("%.1fG중"), total);
	TextOutSz(hdc, 4, center - 2, str);
	SetTextAlign(hdc, TA_TOP);
	_stprintf_s(str, _T("%.1fG사용"), used);
	TextOutSz(hdc, 4, center + 2, str);

	if (width < 100) {
		SelectObject(hdc, mainFont[1]);
	} else {
		SelectObject(hdc, mainFont[2]);
	}
	_stprintf_s(str, _T("%d%%"), mem.dwMemoryLoad);
	SetTextAlign(hdc, TA_RIGHT);
	SetTextColor(hdc, RGB(0, 0, 255));
	TextOutVCenter(hdc, width - 4, center, str);
	SelectObject(hdc, oldFont);
	SetTextAlign(hdc, oldAlign);
}

LRESULT CBattery::OnCreate(WPARAM wParam, LPARAM lParam) {
	SetTimer(hBaseWnd, 1, 10000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	return FALSE;
}

LRESULT CBattery::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CBattery::OnTimer(WPARAM wParam, LPARAM lParam) {
	GetSystemPowerStatus(&sps);
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

VOID CBattery::OnDraw(HDC hdc, INT width, INT height) {
	HFONT oldFont;
	HBRUSH brush, oldbrush;
	brush = CreateSolidBrush(RGB(255, 0, 0));
	UINT oldAlign;
	SetBkMode(hdc, TRANSPARENT);

	TCHAR ac[128];
	TCHAR remain[128] = _T("");
	TCHAR percent[128];
	if (sps.ACLineStatus) {
		lstrcpy(ac, _T("전원 연결"));
	} else {
		if (sps.BatteryLifeTime != (DWORD)-1) {
			if (sps.BatteryLifeTime >= 3600) {
				wsprintf(remain, _T("%d:%d분"), sps.BatteryLifeTime / 3600, sps.BatteryLifeTime % 3600 / 60);
			} else {
				wsprintf(remain, _T("%d분"), sps.BatteryLifeTime % 3600 / 60);
			}
			wsprintf(ac, _T("배터리(%s)"), remain);
		} else {
			lstrcpy(ac, _T("배터리"));
		}
	}
	wsprintf(percent, _T("%d%%"), sps.BatteryLifePercent);

	if (width < 100) {
		oldFont = (HFONT)SelectObject(hdc, mainFont[0]);
	} else {
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	}
	if (mainWindow.bVertical) {
		TextOutVCenter(hdc, 5, height / 4, ac);
	} else {
		oldAlign = SetTextAlign(hdc, TA_CENTER);
		TextOutSz(hdc, width / 4, height / 4, ac);
		SetTextAlign(hdc, oldAlign);
	}

	if (width < 100) {
		SelectObject(hdc, mainFont[1]);
	} else {
		SelectObject(hdc, mainFont[2]);
	}

	if (mainWindow.bVertical) {
		oldAlign = SetTextAlign(hdc, TA_RIGHT);
		TextOutVCenter(hdc, width - 5, height / 4, percent);
		SetTextAlign(hdc, oldAlign);

		// 좌우 문자열 길이 빼고 높이 20으로 중앙 정렬
		Rectangle(hdc, 5, height / 2, width - 5, height / 2 + 20);
		INT twidth = width - 14;
		INT gaugewidth = sps.BatteryLifePercent * twidth / 100;
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		Rectangle(hdc, 5, height / 2 + 2, 7 + gaugewidth, height / 2 + 20 - 2);
		SelectObject(hdc, oldbrush);
	} else {
		oldAlign = SetTextAlign(hdc, TA_CENTER);
		TextOutSz(hdc, width / 4, height / 2, percent);
		SetTextAlign(hdc, oldAlign);

		INT gcenter = width * 3 / 4;
		Rectangle(hdc, gcenter - 10, 5, gcenter + 10, height - 5);
		INT theight = height - 14;
		INT gaugeheight = sps.BatteryLifePercent * theight / 100;
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		Rectangle(hdc, gcenter - 8, height - 5 - gaugeheight, gcenter + 8, height - 5);
		SelectObject(hdc, oldbrush);
	}

	DeleteObject(brush);
	SelectObject(hdc, oldFont);
}

LRESULT CDiskUsage::OnCreate(WPARAM wParam, LPARAM lParam) {
	timerInterval = setting.get(baseget_id, _T("timerInterval"), 60);
	includeRemovable = setting.get(baseget_id, _T("includeRemovable"), 1);
	includeNetwork = setting.get(baseget_id, _T("includeNetwork"), 0);
	relativeChart = setting.get(baseget_id, _T("relativeChart"), 1);
	SetTimer(hBaseWnd, 1, timerInterval * 1000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	return FALSE;
}

LRESULT CDiskUsage::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CDiskUsage::OnTimer(WPARAM wParam, LPARAM lParam) {
	TCHAR Drive[MAX_PATH];
	UINT Type;
	INT idx = 0;

	for (INT i = 'C'; i <= 'Z'; i++) {
		wsprintf(Drive, _T("%c:\\"), i);
		Type = GetDriveType(Drive);
		if (Type == DRIVE_FIXED || (includeRemovable && Type == DRIVE_REMOVABLE)
			|| (includeNetwork && Type == DRIVE_REMOTE)) {
			arDisk[idx].drive = i;
			GetDiskFreeSpaceEx(Drive, &arDisk[idx].Avail, &arDisk[idx].Total, NULL);
			idx++;
		}
	}
	// 끝 표식
	arDisk[idx].drive = '0';
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

VOID CDiskUsage::OnDraw(HDC hdc, INT width, INT height) {
	TCHAR str[111];
	HFONT oldFont;
	INT y;
	INT maxTotal = 0;
	HBRUSH uesdGraph;
	RECT urt;
	const INT Thick = 12;
	HPEN pen, oldPen;

	oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	SetBkMode(hdc, TRANSPARENT);
	uesdGraph = CreateSolidBrush(RGB(0, 255, 0));
	pen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	oldPen = (HPEN)SelectObject(hdc, pen);

	// 최대 용량을 찾는다.
	for (INT idx = 0; arDisk[idx].drive != '0'; idx++) {
		INT total = INT(arDisk[idx].Total.QuadPart / 1073741824);
		if (total > maxTotal) maxTotal = total;
	}

	y = 2 + 7;
	for (INT idx = 0; arDisk[idx].drive != '0'; idx++) {
		wsprintf(str, _T("%c"), arDisk[idx].drive);
		TextOutVCenter(hdc, 2, y, str);

		INT total = INT(arDisk[idx].Total.QuadPart / 1073741824);
		INT avail = INT(arDisk[idx].Avail.QuadPart / 1073741824);
		INT used = total - avail;

		// 왼쪽 20, 오른쪽 5만큼 띄운 폭에 그래프 그림
		INT gwidth = width - 25;
		// 현재 드라이브의 그래프 폭
		INT dwidth;
		if (relativeChart) {
			dwidth = gwidth * total / maxTotal;
		} else {
			dwidth = gwidth;
		}

		// 그래프 외곽선 그림
		Rectangle(hdc, 20, y - Thick / 2, 20 + dwidth, y + Thick / 2);
		// 사용량의 폭. 그래프의 경계선 안쪽에 그린다.
		INT uwidth = (dwidth - 2) * used / total;
		SetRect(&urt, 21, y - (Thick / 2 - 1), 21 + uwidth, y + (Thick / 2 - 1));
		FillRect(hdc, &urt, uesdGraph);

		y += 15;
		// 폭 140 이상일 때만 M 단위와 비율 출력
		if (width > 140) {
			wsprintf(str, _T("%dM-%dM=%dM(%d%%)"), total, used, avail, avail * 100 / total);
		} else {
			if (width < 100) {
				SelectObject(hdc, mainFont[0]);
			}
			wsprintf(str, _T("%d-%d=%d"), total, used, avail);
		}
		TextOutVCenter(hdc, 2, y, str);
		y += 15 + 5;
	}
	SelectObject(hdc, oldFont);
	DeleteObject(uesdGraph);
	DeleteObject(SelectObject(hdc, oldPen));
}

LRESULT CDiskUsage::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_DISKUSAGE),
			hBaseWnd, DiskUsageDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

VOID CDiskUsage::LoadSetting() {
	timerInterval = setting.get(baseget_id, _T("timerInterval"), 60);
	includeRemovable = setting.get(baseget_id, _T("includeRemovable"), 1);
	includeNetwork = setting.get(baseget_id, _T("includeNetwork"), 0);
	relativeChart = setting.get(baseget_id, _T("relativeChart"), 1);
}

VOID CDiskUsage::SaveSetting() {
	setting.set(baseget_id, _T("timerInterval"), timerInterval);
	setting.set(baseget_id, _T("includeRemovable"), includeRemovable);
	setting.set(baseget_id, _T("includeNetwork"), includeNetwork);
	setting.set(baseget_id, _T("relativeChart"), relativeChart);
}

INT_PTR CALLBACK CDiskUsage::DiskUsageDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CDiskUsage* pTuingget;
	INT radio = 0;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CDiskUsage*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		SendDlgItemMessage(hDlg, IDC_CHKRELATIVECHART, BM_SETCHECK,
			pTuingget->relativeChart ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKREMOVABLE, BM_SETCHECK,
			pTuingget->includeRemovable ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKNETWORK, BM_SETCHECK,
			pTuingget->includeNetwork ? BST_CHECKED : BST_UNCHECKED, 0);

		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			pTuingget->relativeChart = IsDlgButtonChecked(hDlg, IDC_CHKRELATIVECHART);
			pTuingget->includeRemovable = IsDlgButtonChecked(hDlg, IDC_CHKREMOVABLE);
			pTuingget->includeNetwork = IsDlgButtonChecked(hDlg, IDC_CHKNETWORK);
			SendMessage(pTuingget->hBaseWnd, WM_TIMER, 1, 0);
			InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);

			pTuingget->SaveSetting();

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

WNDPROC CCalc::CalcOldEditProc;
LRESULT CCalc::OnCreate(WPARAM wParam, LPARAM lParam) {
	hCbExp = CreateWindow(_T("combobox"), _T(""),
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_AUTOHSCROLL,
		2, 2, 160, 125, hBaseWnd, (HMENU)1, g_hInst, NULL);
	SendMessage(hCbExp, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	// 콤보 안의 에디트를 서브클래싱하여 Enter키 입력을 검출한다.
	hEdit = GetWindow(hCbExp, GW_CHILD);
	CCalc::CalcOldEditProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);

	calcResult = 0;
	lstrcpy(sResult, _T("=0"));
	autoCopy = false;

	// 복사 버튼
	hBtnCopy = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_PUSH,
		0, 0, 16, 16, hBaseWnd, (HMENU)2, g_hInst, NULL);
	SendMessage(hBtnCopy, GBM_SETIMAGE, MAKEWPARAM(IDB_CALCCOPYUP, IDB_CALCCOPYHOT),
		MAKEWPARAM(IDB_CALCCOPYDOWN, IDB_CALCCOPYUP));
	return FALSE;
}

LRESULT CCalc::OnSize(WPARAM wParam, LPARAM lParam) {
	// 일단 부모의 메서드를 먼저 호출하여 디폴트 처리한다.
	CBaseWnd::OnSize(wParam, lParam);

	RECT crt;
	GetClientRect(hBaseWnd, &crt);

	// 수직 중앙 정렬하되 윗변과 최소한 2픽셀 띄운다. 
	INT y = (crt.bottom - 52) / 2;
	if (y < 2) y = 2;
	MoveWindow(hCbExp, 2, y, crt.right - 4, 125, TRUE);
	y += 25 + 2;
	MoveWindow(hBtnCopy, crt.right - 18, y, 16, 16, TRUE);

	return FALSE;
}

LRESULT CCalc::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case 2:
		CopyResult();
		break;
	case IDM_CALC_RESULTUP:
		SendMessage(hBaseWnd, WM_USER + 2, 0, 0);
		break;
	case IDM_CALC_AUTOCOPY:
		autoCopy = !autoCopy;
		break;
	}
	return FALSE;
}

VOID CCalc::OnDraw(HDC hdc, INT width, INT height) {
	HFONT oldFont;

	// 복사 버튼과 같은 수직 위치에 출력한다.
	RECT wrt;
	GetWindowRect(hBtnCopy, &wrt);
	ScreenToClient(hBaseWnd, (LPPOINT)&wrt.left);

	oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	SetBkMode(hdc, TRANSPARENT);
	TextOutSz(hdc, 2, wrt.top, sResult);
	SelectObject(hdc, oldFont);
}

LRESULT CCalc::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	HMENU hMenu, hPopup;
	hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUPMENU));
	hPopup = GetSubMenu(hMenu, 1);

	// 자동 복사 상태 메뉴에 표시
	if (autoCopy) {
		CheckMenuItem(hPopup, IDM_CALC_AUTOCOPY, MF_BYCOMMAND | MF_CHECKED);
	}

	TrackPopupMenu(hPopup, TPM_LEFTALIGN, (SHORT)LOWORD(lParam),
		(SHORT)HIWORD(lParam), 0, hBaseWnd, NULL);
	DestroyMenu(hMenu);
	return FALSE;
}

// 수식 계산. 서브클래싱한 에디트가 부모의 함수를 호출할 수 없어 사용자 정의 메시지로 처리함
LRESULT CCalc::OnUser1(WPARAM wParam, LPARAM lParam) {
	TCHAR sExp[256];
	BOOL bError;

	GetWindowText(hCbExp, sExp, 256);
	calcResult = CalcExp(sExp, &bError);
	if (bError) {
		lstrcpy(sResult, _T("수식이 잘못되었습니다."));
	} else {
		// 콤보 목록에 삽입. 상위 몇 개만 INI 파일에 목록을 저장해 두는 것도 괜찮을 거 같다. 
		SendMessage(hCbExp, CB_INSERTSTRING, 0, (LPARAM)sExp);

		_stprintf_s(sResult, _T("= %f"), calcResult);
		TrimTrailZero(sResult);

		// 자동 복사 실행
		if (autoCopy) {
			CopyResult();
		}
	}
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

// 계산 결과 위로 올리기
LRESULT CCalc::OnUser2(WPARAM wParam, LPARAM lParam) {
	TCHAR str[128];
	HWND hEdit;
	INT len;

	_stprintf_s(str, _T("%f"), calcResult);
	TrimTrailZero(str);
	lstrcat(str, _T(" "));
	SetWindowText(hCbExp, str);

	// 뒤에 공백 하나 더 넣어 주고 끝으로 이동한 후 포커스까지 준다. 
	SetFocus(hCbExp);
	hEdit = GetWindow(hCbExp, GW_CHILD);
	len = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, len, len);
	return FALSE;
}

VOID CCalc::CopyResult() {
	TCHAR str[128];
	_stprintf_s(str, _T("%f"), calcResult);
	TrimTrailZero(str);

	HGLOBAL hmem;
	TCHAR* ptr;
	hmem = GlobalAlloc(GHND, (lstrlen(str) + 1) * sizeof(TCHAR));
	ptr = (TCHAR*)GlobalLock(hmem);
	memcpy(ptr, str, (lstrlen(str) + 1) * sizeof(TCHAR));
	GlobalUnlock(hmem);
	if (OpenClipboard(hBaseWnd)) {
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hmem);
		CloseClipboard();
	} else {
		GlobalFree(hmem);
	}
}

LRESULT CALLBACK CCalc::EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			PostMessage(GetParent(GetParent(hWnd)), WM_USER + 1, 0, 0);
			return 0;
		}
		if (wParam == VK_TAB) {
			PostMessage(GetParent(GetParent(hWnd)), WM_USER + 2, 0, 0);
			break;
		}
		break;
	case WM_KEYUP:
	case WM_CHAR:
		if (wParam == VK_RETURN || wParam == VK_TAB) {
			return 0;
		}
		break;
	}

	return CallWindowProc(CCalc::CalcOldEditProc, hWnd, iMessage, wParam, lParam);
}

DOUBLE CCalc::CalcExp(TCHAR* exp, BOOL* bError/*=NULL*/) {
	TCHAR Post[256];
	TCHAR* p;
	INT count;

	// 괄호 짝 맞는지 확인. 짝이 맞지 않으면 즉시 리턴한다.
	if (bError != NULL) {
		for (p = exp, count = 0; *p; p++) {
			if (*p == '(') count++;
			if (*p == ')') count--;
		}
		*bError = (count != 0);
	}
	if (*bError) return 0;

	// 숫자, 연산자 외에 불필요한 문자가 있는지 확인한다.
	for (TCHAR* p = exp; *p; p++) {
		if (isCalcChar(*p) == FALSE) {
			*bError = TRUE;
			return FALSE;
		}
	}
	*bError = FALSE;

	MakePostfix(Post, exp);
	return CalcPostfix(Post);
}

DOUBLE CCalc::CalcPostfix(TCHAR* Post) {
	TCHAR* p = Post;
	DOUBLE num;
	DOUBLE left, right;
	TStack<DOUBLE> dS(256);

	while (*p) {
		// 숫자는 스택에 넣는다.
		if (isdigit(*p)) {
			num = _ttof(p);
			dS.Push(num);
			for (; isdigit(*p) || *p == '.'; p++) { ; }
		} else {
			// 연산자는 스택에서 두 수를 꺼내 연산하고 다시 푸시한다.
			if (strchr("^*/+-", *p)) {
				right = dS.Pop();
				left = dS.Pop();
				switch (*p) {
				case '+':
					dS.Push(left + right);
					break;
				case '-':
					dS.Push(left - right);
					break;
				case '*':
					dS.Push(left * right);
					break;
				case '/':
					if (right == 0.0) {
						dS.Push(0.0);
					}
					else {
						dS.Push(left / right);
					}
					break;
				case '^':
					dS.Push(pow(left, right));
					break;
				}
			}
			// 연산 후 또는 연산자가 아닌 경우 다음 문자로
			p++;
		}
	}
	if (dS.GetTop() != -1) {
		num = dS.Pop();
	} else {
		num = 0.0;
	}
	return num;
}

VOID CCalc::MakePostfix(TCHAR* Post, TCHAR* Mid) {
	TCHAR* m = Mid;
	TCHAR* p = Post, c;
	TStack<TCHAR> cS(256);

	while (*m) {
		// 숫자 - 그대로 출력하고 뒤에 공백 하나를 출력한다.
		if (isdigit(*m)) {
			while (isdigit(*m) || *m == '.') *p++ = *m++;
			*p++ = ' ';
		} else {
			// 연산자 - 스택에 있는 자기보다 높은 연산자를 모두 꺼내 출력하고 자신은 푸시한다.
			if (strchr("^*/+-", *m)) {
				while (cS.GetTop() != -1 && GetPriority(cS.GetValue(cS.GetTop())) >=
					GetPriority(*m)) {
					*p++ = cS.Pop();
				}
				cS.Push(*m++);
			} else {
				// 여는 괄호 - 푸시한다.
				if (*m == '(') {
					cS.Push(*m++);
				} else {
					// 닫는 괄호 - 여는 괄호가 나올 때까지 팝해서 출력하고 여는 괄호는 버린다.
					if (*m == ')') {
						for (;;) {
							c = cS.Pop();
							if (c == '(') break;
							*p++ = c;
						}
						m++;
					} else {
						m++;
					}
				}
			}
		}
	}
	// 스택에 남은 연산자들 모두 꺼낸다.
	while (cS.GetTop() != -1) {
		*p++ = cS.Pop();
	}
	*p = 0;
}

INT CCalc::GetPriority(INT op) {
	switch (op) {
	case '(':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	}
	return 100;
}

BOOL CCalc::isCalcChar(TCHAR c) {
	if (c >= '0' && c <= '9') return TRUE;
	if (c == ' ' || c == '+' || c == '-' || c == '*' || c == '/' || c == '^'
		|| c == '(' || c == ')' || c == '.') return TRUE;

	return FALSE;
}

// 음악 재생기 todo
// 리스트의 팝업 - 개별 곡 제거, 파일 삭제, 이동. 이동은 드래그 & 드롭으로 구현할 것
// 재생 외에도 이전, 다음곡까지 임의의 단축키 등록하는 기능 작성하고 등록 실패 처리
LRESULT CMusicPlayer::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case MM_MCINOTIFY:
		switch (wParam) {
		case MCI_NOTIFY_ABORTED:
			// MCI_STOP에 대해 ABORTED 통지 날라오며 여기서 장치를 닫는다. 
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			dev_id = 0;
			break;
		case MCI_NOTIFY_SUCCESSFUL:
			// 한곡 재생 완료. 이때는 이미 STOP 상태이다. 장치를 닫은 후 다음 곡 재생. 
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			dev_id = 0;

			// 재생 그룹의 다음 곡으로 이동한 후 재생한다.
			if (randomPlay) {
				playIdx = SelectRandomPlay();
			}
			else {
				moveNext(playIdx, static_cast<INT>(arMusic[playGroup].size()));
			}
			groupIdx[playGroup] = playIdx;
			// 재생 그룹이 보이는 상태이면 리스트 박스의 포커스도 옮겨 준다.
			if (GetActiveGroup() == playGroup) {
				SendMessage(hList, LB_SETCURSEL, playIdx, 0);
			}
			DoPlayCurrent(TRUE);
			break;
		}
		return FALSE;
	case WM_HSCROLL:
		if (lParam == (LPARAM)hProg) {
			if (dev_id != 0) {
				// 재생을 멈추고 프로그래스 위치에서 다시 시작한다. 
				mciSendCommand(dev_id, MCI_STOP, 0, (DWORD)NULL);
				SetTimer(hBaseWnd, MPTM_MOVEPOS, 100, NULL);
			}
		}
		return FALSE;
	case WM_HOTKEY:
		switch (wParam) {
		case 0:
			PostMessage(hBaseWnd, WM_COMMAND, MAKEWPARAM(MP_PLAY, 0), 0);
			break;
		}
		return FALSE;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CMusicPlayer::OnCreate(WPARAM wParam, LPARAM lParam) {
	hPlay = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 2, 2, 24, 16, hBaseWnd, (HMENU)MP_PLAY, g_hInst, NULL);
	SendMessage(hPlay, GBM_SETIMAGE, MAKEWPARAM(IDB_PLAYUP, IDB_PLAYHOT), MAKEWPARAM(IDB_PLAYDOWN, IDB_PLAYUP));
	hStop = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 28, 2, 16, 16, hBaseWnd, (HMENU)MP_STOP, g_hInst, NULL);
	SendMessage(hStop, GBM_SETIMAGE, MAKEWPARAM(IDB_STOPUP, IDB_STOPHOT), MAKEWPARAM(IDB_STOPDOWN, IDB_STOPUP));
	hPrev = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 46, 2, 16, 16, hBaseWnd, (HMENU)MP_PREV, g_hInst, NULL);
	SendMessage(hPrev, GBM_SETIMAGE, MAKEWPARAM(IDB_PREVUP, IDB_PREVHOT), MAKEWPARAM(IDB_PREVDOWN, IDB_PREVUP));
	hNext = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 64, 2, 16, 16, hBaseWnd, (HMENU)MP_NEXT, g_hInst, NULL);
	SendMessage(hNext, GBM_SETIMAGE, MAKEWPARAM(IDB_NEXTUP, IDB_NEXTHOT), MAKEWPARAM(IDB_NEXTDOWN, IDB_NEXTUP));
	hRandom = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_CHECK, 90, 2, 16, 16, hBaseWnd, (HMENU)MP_RANDOM, g_hInst, NULL);
	SendMessage(hRandom, GBM_SETIMAGE, MAKEWPARAM(IDB_MPRANDOMOFF, IDB_MPRANDOMOFF), MAKEWPARAM(IDB_MPRANDOMON, IDB_MPRANDOMOFF));

	//// 스크롤 바의 배경색 위젯 색상으로 초기화
	//hProg = CreateWindow(_T("GrpScrl"), NULL, WS_CHILD | WS_VISIBLE | GSS_HORZ, 2, 20, 128, 14, hBaseWnd, (HMENU)MP_PROG, g_hInst, NULL);
	//SendMessage(hProg, GSM_SETBACKCOLOR, (WPARAM)BaseBackColor, 0);

	hGroup1 = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_CHECK, 2, 38, 16, 12, hBaseWnd, (HMENU)MP_GROUP1, g_hInst, NULL);
	SendMessage(hGroup1, GBM_SETIMAGE, MAKEWPARAM(IDB_GROUP1UP, IDB_GROUP1UP), MAKEWPARAM(IDB_GROUP1DOWN, IDB_GROUP1DOWN));
	hGroup2 = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_CHECK, 18, 38, 16, 12, hBaseWnd, (HMENU)MP_GROUP2, g_hInst, NULL);
	SendMessage(hGroup2, GBM_SETIMAGE, MAKEWPARAM(IDB_GROUP2UP, IDB_GROUP2UP), MAKEWPARAM(IDB_GROUP2DOWN, IDB_GROUP2DOWN));
	hGroup3 = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_CHECK, 34, 38, 16, 12, hBaseWnd, (HMENU)MP_GROUP3, g_hInst, NULL);
	SendMessage(hGroup3, GBM_SETIMAGE, MAKEWPARAM(IDB_GROUP3UP, IDB_GROUP3UP), MAKEWPARAM(IDB_GROUP3DOWN, IDB_GROUP3DOWN));
	hGroup4 = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_CHECK, 50, 38, 16, 12, hBaseWnd, (HMENU)MP_GROUP4, g_hInst, NULL);
	SendMessage(hGroup4, GBM_SETIMAGE, MAKEWPARAM(IDB_GROUP4UP, IDB_GROUP4UP), MAKEWPARAM(IDB_GROUP4DOWN, IDB_GROUP4DOWN));

	hAdd = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, 16, 16, hBaseWnd, (HMENU)MP_ADD, g_hInst, NULL);
	SendMessage(hAdd, GBM_SETIMAGE, MAKEWPARAM(IDB_MPADDUP, IDB_MPADDHOT), MAKEWPARAM(IDB_MPADDDOWN, IDB_MPADDUP));
	hClear = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, 16, 16, hBaseWnd, (HMENU)MP_CLEAR, g_hInst, NULL);
	SendMessage(hClear, GBM_SETIMAGE, MAKEWPARAM(IDB_MPCLEARUP, IDB_MPCLEARHOT), MAKEWPARAM(IDB_MPCLEARDOWN, IDB_MPCLEARUP));

	hList = CreateWindow(_T("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY | WS_VSCROLL | LBS_NOINTEGRALHEIGHT, 2, 50, 120, 100, hBaseWnd, (HMENU)MP_LIST, g_hInst, NULL);
	SendMessage(hList, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	// 저장해 놓은 음악 목록을 불러 온다. 저장은 목록이 변할 때마다. 
	// 또는 변경 플래그 두고 종료시에 일괄 저장하는 방법 고려해 볼 것
	LoadMusicList();

	// 최후 선택 그룹을 조사하여 선택한다. 디폴트는 첫 그룹
	playGroup = setting.get(baseget_id, _T("playGroup"), 0);
	playIdx = setting.get(baseget_id, _T("playIdx"), -1);

	// 각 그룹의 최후 선택곡을 조사한다. 빈 목록의 경우도 있어 디폴트는 선택 없음이다. 
	TCHAR key[32];
	for (INT group = 0; group < 4; group++) {
		wsprintf(key, _T("groupIdx%d"), group);
		groupIdx[group] = setting.get(baseget_id, key, -1);
	}
	// 재생 그룹은 재생곡을 강제 선택한다. 3번 재생중 5번 선택해 놓고 종료해도 다시 시작하면 3번을 선택한다.
	groupIdx[playGroup] = playIdx;

	// 랜덤 재생 여부 조사
	randomPlay = setting.get(baseget_id, _T("randomPlay"), 0);
	if (randomPlay) {
		SendMessage(hRandom, GBM_SETSTATE, GB_DOWN, 0);
	}

	// 재생 그룹으로 변경하고 목록 채워 넣는다. 
	ChangeActiveGroup(playGroup, TRUE);
	FillListBox(playGroup);

	DragAcceptFiles(hBaseWnd, TRUE);
	dev_id = 0;

	// 프로그래스 갱신용 타이머. 주기적으로 폴링한다. 
	SetTimer(hBaseWnd, MPTM_PROG, 200, NULL);

	// Pause키를 재생 핫키로 등록한다. 다른 프로그램이 이미 등록했으면 실패할 수도 있다. 
	if (RegisterHotKey(hBaseWnd, 0, 0, VK_PAUSE) == FALSE) {
		// 소리로 실패 사실을 알리되 디버그 버전에서는 귀찮으니 생략한다. 
#ifndef _DEBUG
		MessageBeep(0);
#endif
	}

	// 임시 파일 경로를 미리 조립해 둔다. 
	// todo :  실제 파일이 아닌 메모리 맵 파일을 경유하는 방법 연구해 볼 것
	GetModuleFileName(NULL, stripPath, MAX_PATH);
	TCHAR* bs = _tcsrchr(stripPath, '\\');
	*(bs + 1) = 0;
	lstrcat(stripPath, _T("TuingApp-temp.mp3"));

	return FALSE;
}

LRESULT CMusicPlayer::OnDestroy(WPARAM wParam, LPARAM lParam) {
	// 현재 그룹을 저장한다. 
	setting.set(baseget_id, _T("playGroup"), playGroup);
	setting.set(baseget_id, _T("playIdx"), playIdx);
	setting.set(baseget_id, _T("randomPlay"), randomPlay);

	TCHAR key[32];
	// 각 그룹의 현재 idx를 저장한다.
	for (INT group = 0; group < 4; group++) {
		wsprintf(key, _T("groupIdx%d"), group);
		setting.set(baseget_id, key, groupIdx[group]);
	}

	KillTimer(hBaseWnd, MPTM_PROG);
	UnregisterHotKey(hBaseWnd, 0);
	return FALSE;
}

LRESULT CMusicPlayer::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	// 처음 생성시는 크기가 0인데 이때는 배치하지 않음. 
	// 안 그럼 스크롤 바의 폭이 일시적으로 0이 되어 썸이 4로 줄어 들어 버림. 스크롤 바에 줄어든 크기 복원하는 기능 필요
	INT width = LOWORD(lParam);
	if (width == 0) return FALSE;

	MoveWindow(hProg, 2, 20, width - 4, 14, TRUE);
	MoveWindow(hList, 2, 50, width - 4, HIWORD(lParam) - 48, TRUE);
	MoveWindow(hAdd, width - 36, 34, 16, 16, TRUE);
	MoveWindow(hClear, width - 18, 34, 16, 16, TRUE);

	return FALSE;
}

LRESULT CMusicPlayer::OnTimer(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
		// 프로그래스 진행. 재생중일 때 주기적으로 갱신
	case MPTM_PROG:
		if (dev_id != 0) {
			SendMessage(hProg, GSM_SETPOS, GetPosition(), 0);
		}
		break;
		// 재생중 더블클릭시 재생 멈춘 후 현재곡 재생
	case MPTM_PLAYACTIVE:
		KillTimer(hBaseWnd, MPTM_PLAYACTIVE);
		PlayActiveCurrent();
		break;
		// 재생중 다음곡으로 이동
	case MPTM_PLAYNEXT:
		KillTimer(hBaseWnd, MPTM_PLAYNEXT);
		DoPlayCurrent(TRUE);
		break;
		// 재생중 이전곡으로 이동
	case MPTM_PLAYPREV:
		KillTimer(hBaseWnd, MPTM_PLAYPREV);
		DoPlayCurrent(FALSE);
		break;
		// 재생 위치 이동시. 현재 스크롤 바 썸의 위치에서 재생 시작
	case MPTM_MOVEPOS:
		KillTimer(hBaseWnd, MPTM_MOVEPOS);
		DoPlay(arMusic[playGroup][groupIdx[playGroup]].c_str(), static_cast<INT>(SendMessage(hProg, GSM_GETPOS, 0, 0)));
		break;
	}
	return FALSE;
}

// 아직 별다른 옵션은 없음. 
LRESULT CMusicPlayer::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	return FALSE;
}

LRESULT CMusicPlayer::OnDropFiles(WPARAM wParam, LPARAM lParam) {
	TCHAR Path[MAX_PATH];
	INT Count, n, addCount = 0;

	Count = DragQueryFile((HDROP)wParam, 0xffffffff, NULL, 0);
	for (n = 0; n < Count; n++) {
		DragQueryFile((HDROP)wParam, n, Path, MAX_PATH);
		AddFile(Path);
	}

	// 빈 목록에 파일을 드래그했으면 첫 번째 항목 선택해 줌. 가급적 선택항목이 없는 상태를 만들지 말아야 한다. 
	if (groupIdx[GetActiveGroup()] == -1) {
		groupIdx[GetActiveGroup()] = 0;
		SendMessage(hList, LB_SETCURSEL, 0, 0);
	}

	// 목록에 변화가 있을 때마다 저장한다. 
	SaveMusicList();
	return FALSE;
}

LRESULT CMusicPlayer::OnCommand(WPARAM wParam, LPARAM lParam) {
	INT group;
	INT list_idx;

	switch (LOWORD(wParam)) {
	case MP_PLAY:
		if (dev_id != 0) {
			// 열려 있는 상태이면 현재 상태에 따라 일시정지 또는 재개한다.
			switch (GetStatus()) {
			case MCI_MODE_PLAY:
				mciSendCommand(dev_id, MCI_PAUSE, 0, (DWORD)NULL);
				break;
			case MCI_MODE_PAUSE:
				mciSendCommand(dev_id, MCI_RESUME, 0, (DWORD)NULL);
				break;
			}
		} else {
			// 열려 있지 않으면 액티브 그룹의 현재 곡을 재생한다.
			// 사용자가 열어 놓은 목록에서 선택한 곡이 대상이다. 
			PlayActiveCurrent();
		}
		break;
	case MP_STOP:
		if (dev_id != 0) {
			mciSendCommand(dev_id, MCI_STOP, 0, (DWORD)NULL);
			SendMessage(hProg, GSM_SETPOS, 0, 0);
		}
		break;
	case MP_NEXT:
		// 재생중이 아니면 액티브 그룹의 다음곡으로 이동하고 리스트 박스에도 표시
		if (dev_id == 0) {
			group = GetActiveGroup();
			if (groupIdx[group] != -1) {
				moveNext(groupIdx[group], static_cast<INT>(arMusic[group].size()));
				SendMessage(hList, LB_SETCURSEL, groupIdx[group], 0);
			}
		} else {
			if (randomPlay) {
				playIdx = SelectRandomPlay();
			} else {
				// 재생 그룹의 다음곡으로 이동. 
				moveNext(playIdx, static_cast<INT>(arMusic[playGroup].size()));
			}
			// 그룹의 현재곡은 일단 변경한다. 
			groupIdx[playGroup] = playIdx;
			// 재생 그룹이 보이는 상태이면 리스트 박스에도 표시. 다른 페이지로 가 있으면 무시
			if (GetActiveGroup() == playGroup) {
				SendMessage(hList, LB_SETCURSEL, playIdx, 0);
			}

			// 재생 정지하고 재생 그룹의 현재곡 재생
			mciSendCommand(dev_id, MCI_STOP, MCI_WAIT, (DWORD)NULL);
			SetTimer(hBaseWnd, MPTM_PLAYNEXT, 100, NULL);
		}
		break;
	case MP_PREV:
		if (dev_id == 0) {
			group = GetActiveGroup();
			if (groupIdx[group] != -1) {
				movePrev(groupIdx[group], static_cast<INT>(arMusic[group].size()));
				SendMessage(hList, LB_SETCURSEL, groupIdx[group], 0);
			}
		} else {
			if (randomPlay) {
				playIdx = SelectRandomPlay();
			} else {
				movePrev(playIdx, static_cast<INT>(arMusic[playGroup].size()));
			}
			groupIdx[playGroup] = playIdx;
			if (GetActiveGroup() == playGroup) {
				SendMessage(hList, LB_SETCURSEL, playIdx, 0);
			}

			mciSendCommand(dev_id, MCI_STOP, MCI_WAIT, (DWORD)NULL);
			SetTimer(hBaseWnd, MPTM_PLAYPREV, 100, NULL);
		}
		break;
	case MP_LIST:
		switch (HIWORD(wParam)) {
		case LBN_DBLCLK:
			// 더블클릭한 곡 재생. 재생중이면 정지 후 재생
			if (dev_id != 0) {
				mciSendCommand(dev_id, MCI_STOP, MCI_WAIT, (DWORD)NULL);
				SetTimer(hBaseWnd, MPTM_PLAYACTIVE, 100, NULL);
			} else {
				PlayActiveCurrent();
			}
			break;
		case LBN_SELCHANGE:
			// 선택 변경시 액티브 그룹의 현재 곡을 변경한다.
			list_idx = static_cast<INT>(SendMessage(hList, LB_GETCURSEL, 0, 0));
			groupIdx[GetActiveGroup()] = list_idx;
			break;
		}
		break;
	case MP_ADD:
		OPENFILENAME OFN;
		TCHAR* files;
		TCHAR* p;
		TCHAR dir[MAX_PATH];
		TCHAR file[MAX_PATH];
		TCHAR path[MAX_PATH];
		files = (TCHAR*)calloc(32000, sizeof(TCHAR));
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hBaseWnd;
		OFN.lpstrFilter = _T("사운드 파일(mp3, wav)\0*.mp3;*.wav\0");
		OFN.lpstrFile = files;
		OFN.nMaxFile = 320000;
		OFN.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
		// 복수 개의 파일 한꺼번에 추가. 대략 만곡 정도 한꺼번에 추가 가능하다. 
		if (GetOpenFileName(&OFN) != 0) {
			p = files;
			lstrcpy(dir, p);
			if (*(p + lstrlen(dir) + 1) == 0) {
				AddFile(dir);
			} else {
				lstrcpy(file, dir);
				for (;;) {
					p = p + lstrlen(file) + 1;
					if (*p == 0)
						break;
					lstrcpy(file, p);
					lstrcpy(path, dir);
					lstrcat(path, _T("\\"));
					lstrcat(path, file);
					AddFile(path);
				}
			}
		} else {
			if (CommDlgExtendedError() == FNERR_BUFFERTOOSMALL) {
				MessageBox(hBaseWnd, _T("한꺼번에 너무 많은 파일을 선택했습니다."), _T("에러"), MB_OK);
			}
		}
		SaveMusicList();
		break;
	case MP_CLEAR:
		// 재생 중이면 일단 정지한다. 
		if (dev_id != 0) {
			mciSendCommand(dev_id, MCI_STOP, 0, (DWORD)NULL);
			SendMessage(hProg, GSM_SETPOS, 0, 0);
		}

		// 리스트 박스 비움
		SendMessage(hList, LB_RESETCONTENT, 0, 0);

		// 그룹의 목록을 비운다. 
		group = GetActiveGroup();
		arMusic[group].clear();
		groupIdx[group] = -1;

		SaveMusicList();
		break;
	case MP_GROUP1:
	case MP_GROUP2:
	case MP_GROUP3:
	case MP_GROUP4:
		ChangeActiveGroup(LOWORD(wParam) - MP_GROUP1);
		break;
	case MP_RANDOM:
		randomPlay = (SendMessage(hRandom, GBM_GETSTATE, 0, 0) == GB_DOWN);
		break;
	}
	return FALSE;
}

// 현재 그룹의 포커스 가진 곡을 재생한다.
// Play 버튼 누를시, 더블클릭시, 재생중 더블클릭시 등
VOID CMusicPlayer::PlayActiveCurrent() {
	playGroup = GetActiveGroup();
	playIdx = groupIdx[playGroup];
	// 빈 그룹인 경우는 재생할 수 없다. 조용히 리턴한다.
	if (playIdx == -1) return;
	// 그룹의 곡 개수 이상의 번호는 무효하다. 재생중인 그룹을 비워 버리고 종료하면 playIdx가 무효한 위치를 가리킨다. 
	// 비워 버린 그룹이 playGroup일 경우 playIdx를 현재곡으로 설정하고 있어 재생시 예외 처리가 필요하다. 
	if (playIdx >= (INT)arMusic[playGroup].size()) return;

	DoPlayCurrent(TRUE);
}

// playGroup의 playIdx곡을 재생하며 호출원은 이 두 변수를 설정해 두어야 한다. 
// 실패시 이전 또는 다음곡을 계속 찾는다. MCI의 한계뿐만 아니라 잘못된 파일은 언제든지 있을 수 있어 루프가 필요하다.
// 항상 정지 상태에서 이 메서드를 호출해야 하며 dev_id는 항상 0이다. 
BOOL CMusicPlayer::DoPlayCurrent(BOOL bNext) {
	INT oldIdx = playIdx;

	// 현재 위치에서 성공할 때까지 다음 곡으로 이동
	while (DoPlay(arMusic[playGroup][playIdx].c_str()) == FALSE) {
		if (bNext) {
			moveNext(playIdx, static_cast<INT>(arMusic[playGroup].size()));
		} else {
			movePrev(playIdx, static_cast<INT>(arMusic[playGroup].size()));
		}

		// 재생 그룹이 보이면 리스트 박스의 현재 위치도 갱신한다.
		if (playGroup == GetActiveGroup()) {
			SendMessage(hList, LB_SETCURSEL, playIdx, 0);
		}
		// 한바퀴 돌아 처음으로 돌아 올 때까지 계속 실패이면 무한 루프 방지를 위해 리턴한다. 
		if (playIdx == oldIdx) return FALSE;

		// 약간 쉰다.
		MessageBeep(0);
		Sleep(300);
	}

	return TRUE;
}

// 지정한 파일을 재생한다. fromPos가 주어지면 이 위치부터 재생한다.
BOOL CMusicPlayer::DoPlay(LPCTSTR musicPath, INT fromPos /*= 0*/) {
	DWORD Result;
	MCI_OPEN_PARMS openParam;
	MCI_PLAY_PARMS playParam;
	TCHAR sError[256];

	// ID3 태그를 제거했으면 변환한 파일을 재생하고 그렇지 않으면 원본을 재생한다. 
	TCHAR tPath[MAX_PATH];
	if (StripID3Tag(musicPath)) {
		lstrcpy(tPath, stripPath);
	} else {
		lstrcpy(tPath, musicPath);
	}

	// 열려 있지 않은 상태이면 장치를 열고 재생을 시작한다. 
	// 장치를 Open하고 ID를 발급받는다.
	openParam.lpstrDeviceType = _T("mpegvideo");
	openParam.lpstrElementName = tPath;
	Result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(DWORD_PTR)(LPVOID)&openParam);
	if (Result) {
		// MCI 277 에러 - ID3 태그 또는 기타 이유로 발생 가능. ID3 태그 문제는 해결했으나 다른 문제가 더 있을 수도 있다. 
		mciGetErrorString(Result, sError, 256);

		// 파일 닫고 실패를 리턴한다. 
		mciSendCommand(dev_id, MCI_CLOSE, 0, (DWORD)NULL);
		return FALSE;
	}
	dev_id = openParam.wDeviceID;

	// 재생을 시작한다. fromPos가 없으면 처음부터 재생한다. 메인 윈도우로 통지를 보낸다.
	playParam.dwCallback = (DWORD)(DWORD_PTR)hBaseWnd;
	DWORD dwFlag = MCI_NOTIFY;
	if (fromPos != 0) {
		playParam.dwFrom = fromPos;
		dwFlag = dwFlag | MCI_FROM;
	}
	Result = mciSendCommand(dev_id, MCI_PLAY, dwFlag, (DWORD)(DWORD_PTR)(LPVOID)&playParam);

	// 길이 조사해서 트랙바로 범위를 설정한다. 초단위이며 32비트로 범위를 설정해야 한다.
	SendMessage(hProg, GSM_SETRANGE, 0, GetLength());

	return TRUE;
}

// 현재 상태를 조사한다. dev_id는 열려 있는 상태여야 한다.
DWORD CMusicPlayer::GetStatus() {
	MCI_STATUS_PARMS statusParam;
	DWORD Result;
	TCHAR sError[256];

	statusParam.dwItem = MCI_STATUS_MODE;
	Result = mciSendCommand(dev_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(DWORD_PTR)(LPVOID)&statusParam);
	if (Result) {
		mciGetErrorString(Result, sError, 256);
	}

	return static_cast<DWORD>(statusParam.dwReturn);
}

// 재생 길이를 조사한다.
DWORD CMusicPlayer::GetLength() {
	MCI_STATUS_PARMS statusParam;
	DWORD Result;
	TCHAR sError[256];

	statusParam.dwItem = MCI_STATUS_LENGTH;
	Result = mciSendCommand(dev_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(DWORD_PTR)(LPVOID)&statusParam);
	if (Result) {
		mciGetErrorString(Result, sError, 256);
	}

	return static_cast<DWORD>(statusParam.dwReturn);
}

// 현재 재생 위치를 조사한다.
DWORD CMusicPlayer::GetPosition() {
	MCI_STATUS_PARMS statusParam;
	DWORD Result;
	TCHAR sError[256];

	statusParam.dwItem = MCI_STATUS_POSITION;
	Result = mciSendCommand(dev_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(DWORD_PTR)(LPVOID)&statusParam);
	if (Result) {
		mciGetErrorString(Result, sError, 256);
	}

	return static_cast<DWORD>(statusParam.dwReturn);
}

// 현재 표시 그룹에 파일을 추가한다.
BOOL CMusicPlayer::AddFile(LPCTSTR Path) {
	TCHAR ext[MAX_PATH];

	// 확장자를 소문자로 바꾼 후 비교한다.
	lstrcpy(ext, PathFindExtension(Path));
	_tcslwr_s(ext, MAX_PATH);
	if (lstrcmp(ext, _T(".mp3")) == 0 || lstrcmp(ext, _T(".wav")) == 0) {
		arMusic[GetActiveGroup()].push_back(Path);
		// 경로는 빼고 파일명만 추출해서 표시
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)PathFindFileName(Path));
		return TRUE;
	}

	return FALSE;
}

VOID CMusicPlayer::LoadMusicList() {
	TCHAR* buf;
	TCHAR* p, * pEnd;
	TCHAR oneFile[MAX_PATH];
	TCHAR key[32];

	INT bufSize = MAX_PATH * 1000 * sizeof(TCHAR);
	buf = (TCHAR*)malloc(bufSize);
	for (INT group = 0; group < 4; group++) {
		wsprintf(key, _T("MusicList%d"), group);
		setting.get(baseget_id, key, _T(""), buf, bufSize);
		arMusic[group].clear();
		for (p = buf; *p != 0;) {
			pEnd = _tcsstr(p, _T("§"));
			memset(oneFile, 0, sizeof(oneFile));
			_tcsncpy_s(oneFile, MAX_PATH, p, pEnd - p);
			arMusic[group].push_back(oneFile);
			p = pEnd + 1;
		}
	}
	free(buf);
}

VOID CMusicPlayer::SaveMusicList() {
	TCHAR key[32];
	TCHAR* buf;
	INT num;

	// 4개의 그룹 정보를 모두 저장한다.
	for (INT group = 0; group < 4; group++) {
		num = static_cast<INT>(arMusic[group].size());
		if (num == 0) {
			// 음악이 하나도 없으면 빈 문자열 분량이라도 할당해서 기록해야 한다.
			buf = (TCHAR*)calloc(128, sizeof(TCHAR));
		} else {
			buf = (TCHAR*)calloc(MAX_PATH * num, sizeof(TCHAR));
			for (size_t i = 0; i < (INT)arMusic[group].size(); i++) {
				lstrcat(buf, arMusic[group][i].c_str());
				lstrcat(buf, _T("§"));
			}
		}
		wsprintf(key, _T("MusicList%d"), group);
		setting.set(baseget_id, key, buf);
		free(buf);
	}
}

// 그룹을 변경한다. 
VOID CMusicPlayer::ChangeActiveGroup(INT group, BOOL init/* = false*/) {
	ChangeState(hGroup1, GB_DOWN);
	ChangeState(hGroup2, GB_DOWN);
	ChangeState(hGroup3, GB_DOWN);
	ChangeState(hGroup4, GB_DOWN);

	switch (group) {
	case 0:
		ChangeState(hGroup1, GB_NORMAL);
		break;
	case 1:
		ChangeState(hGroup2, GB_NORMAL);
		break;
	case 2:
		ChangeState(hGroup3, GB_NORMAL);
		break;
	case 3:
		ChangeState(hGroup4, GB_NORMAL);
		break;
	}

	// 현재 표시 그룹의 음악 목록을 보여 준다. 
	FillListBox(group);
}

// 그룹 번호를 조사한다.
INT CMusicPlayer::GetActiveGroup() {
	if (SendMessage(hGroup1, GBM_GETSTATE, 0, 0) == GB_NORMAL) return 0;
	if (SendMessage(hGroup2, GBM_GETSTATE, 0, 0) == GB_NORMAL) return 1;
	if (SendMessage(hGroup3, GBM_GETSTATE, 0, 0) == GB_NORMAL) return 2;
	if (SendMessage(hGroup4, GBM_GETSTATE, 0, 0) == GB_NORMAL) return 3;

	return -1;
}

// 그룹의 파일 목록을 채우고 마지막 선택곡을 선택해 준다. 
VOID CMusicPlayer::FillListBox(INT group) {
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (size_t i = 0; i < (INT)arMusic[group].size(); i++) {
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)PathFindFileName(arMusic[group][i].c_str()));
	}

	SendMessage(hList, LB_SETCURSEL, groupIdx[group], 0);
}

// playGroup에서 무작위하게 하나 골라 준다. 
// todo : 전체 그룹에서 무작위로 고를 수도 있다. UI에 그룹간 연속 옵션을 만들고 
// 다음, 이전, 랜덤시 그룹간을 이동하도록 하면 될 듯하다. 
INT CMusicPlayer::SelectRandomPlay() {
	// 하나밖에 없으면 무조건 0번이다.
	if (arMusic[playGroup].size() == 1) return 0;

	INT idx;
	do {
		idx = random(arMusic[playGroup].size());
	} while (playIdx == idx);

	return idx;
}

BOOL CMusicPlayer::StripID3Tag(LPCTSTR path) {
	HANDLE hFile;
	DWORD dwRead, dwWritten;
	DWORD size;
	VOID* buf = NULL;

	// 원본 파일을 열고 버퍼로 읽어 들인다. 
	// 읽을 수 없다면 원본을 그대로 읽도록 무변환 상태로 리턴한다. 
	hFile = CreateFile(path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	size = GetFileSize(hFile, NULL);
	buf = malloc(size);
	ReadFile(hFile, buf, size, &dwRead, NULL);
	CloseHandle(hFile);

	INT offset = 0;
	CHAR* p = (CHAR*)buf;

	// ID3로 시작하면 태그가 있는 것이다. 태그를 빼고 나머지 음성 데이터만으로 임시 파일을 만든다. 
	if (p[0] == 'I' && p[1] == 'D' && p[2] == '3') {
		// 태그의 길이는 6~9바이트에 있되 synchsafe 포맷으로 되어 있다.
		// 첫 비트는 모두 0이며 하위 7비트만 사용하는 리틀엔디언이다. 
		offset = int(p[6] * pow(2, 21) + p[7] * pow(2, 14) + p[8] * pow(2, 7) + p[9]);
		// 태그의 길이에서 헤더 길이는 빠지므로 헤더 길이 10만큼 더해야 음성이 있는 위치가 된다.
		offset += 10;

		// 실행 파일 위치에 임시 파일을 생성한다. MCI가 메모리상의 음성 파일을 지원하지 않아 반드시 파일을 경유해야 한다.
		// 숨김 파일로 생성하되 프로그램 종료시 이 파일을 반드시 지워야 한다. 
		hFile = CreateFile(stripPath, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN, NULL);
		WriteFile(hFile, p + offset, size - offset, &dwWritten, NULL);
		CloseHandle(hFile);
	}
	free(buf);

	// 변환 여부를 리턴한다. 
	if (offset == 0) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

LRESULT CImageSlide::OnCreate(WPARAM wParam, LPARAM lParam) {
	pLastViewer = NULL;
	nowImage = NULL;

	LoadSetting();
	LoadImageList();
	// 최초 0번부터 시작하기 위해 -1을 대입한다.
	nowIdx = -1;
	nowImage = NULL;

	DragAcceptFiles(hBaseWnd, TRUE);
	SetTimer(hBaseWnd, 1, timerInterval * 1000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);

	return FALSE;
}

LRESULT CImageSlide::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	if (nowImage != NULL) delete nowImage;

	return FALSE;
}

VOID CImageSlide::OnDraw(HDC hdc, INT width, INT height) {
	// 파일이 없으면 안내 메시지를 출력한다. 
	if (arImage.size() == 0) {
		RECT crt = { 0, 0, width, height };
		InflateRect(&crt, -2, -5);
		SetTextColor(hdc, RGB(255, 255, 0));
		SetBkMode(hdc, TRANSPARENT);
		HFONT oldFont;
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
		LPCTSTR mes = _T("이미지 슬라이더\r\n그래픽 파일(*.jpg, *.png)을 드래그하여 목록에 추가하십시오.");
		DrawText(hdc, mes, -1, &crt, DT_WORDBREAK);
		SelectObject(hdc, oldFont);
		return;
	}

	DrawImageCenter(hdc, nowImage, width, height, FALSE, BaseBackBrush);

	// 파일명 출력
	if (displayFileName) {
		SetTextColor(hdc, RGB(255, 255, 0));
		SetBkMode(hdc, TRANSPARENT);
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		HFONT oldFont;
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
		const TCHAR* p = _tcsrchr(arImage[nowIdx].c_str(), '\\');
		p++;
		TextOutSz(hdc, width / 2, height - 2, p);
		SelectObject(hdc, oldFont);
	}
}

LRESULT CImageSlide::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	// 목록이 비었으면 리턴한다.
	if (nowIdx == -1) return FALSE;

	// 단일 뷰어 상태에서 이미 뜬 뷰어가 있으면 이미지 번호만 새로 알려 준다.
	if (multiViewer == FALSE && pLastViewer != NULL) {
		SendMessage(pLastViewer->hBaseWnd, WM_USER + 1, nowIdx, NULL);
	} else {
		CImageViewer* pImgViewer;
		pImgViewer = new CImageViewer();
		pImgViewer->delegateDispose = TRUE;
		pImgViewer->nowIdx = nowIdx;
		pImgViewer->Create(0, this);
		pLastViewer = pImgViewer;
	}

	return FALSE;
}

LRESULT CImageSlide::OnTimer(WPARAM wParam, LPARAM lParam) {
	// 이미지 등록전이면 아무 것도 하지 않는다.
	if (arImage.size() == 0) return FALSE;

	// 메모 : nowIdx가 첫 이전인 -1로 초기화되므로 이미지가 하나밖에 없어도 아래 코드를 생략해서는 안된다.

	// 다음 이미지로 이동
	if (randomSlide) {
		INT oldIdx = nowIdx;
		// 난수로 고르되 이전 사진과는 달라야 한다. 단 이미지가 하나밖에 없으면 할 수 없다.
		for (;;) {
			nowIdx = random(arImage.size());
			if (arImage.size() == 1) break;
			if (nowIdx != oldIdx) break;
		}
	} else {
		nowIdx++;
		if (nowIdx >= (INT)arImage.size()) {
			nowIdx = 0;
		}
	}

	// 현재 이미지 삭제한 후 다시 생성
	if (nowImage != NULL) delete nowImage;

	nowImage = Image::FromFile(arImage[nowIdx].c_str());
	FixImageOrientation(nowImage);
	InvalidateRect(hBaseWnd, NULL, TRUE);
	return FALSE;
}

LRESULT CImageSlide::OnUser1(WPARAM wParam, LPARAM lParam) {
	// 단일 뷰어 상태에서 뷰어 하나가 종료되면 뷰어가 없는 상태임을 기억해 둔다.
	// 그래야 클릭시 다시 열 수 있다. 
	if (multiViewer == FALSE) pLastViewer = NULL;

	return FALSE;
}

LRESULT CImageSlide::OnDropFiles(WPARAM wParam, LPARAM lParam) {
	TCHAR Path[MAX_PATH];
	INT Count, n, addCount = 0;
	INT oldCount;
	TCHAR ext[MAX_PATH];

	oldCount = static_cast<INT>(arImage.size());

	Count = DragQueryFile((HDROP)wParam, 0xffffffff, NULL, 0);
	for (n = 0; n < Count; n++) {
		DragQueryFile((HDROP)wParam, n, Path, MAX_PATH);
		// 확장자를 소문자로 바꾼 후 비교한다.
		lstrcpy(ext, PathFindExtension(Path));
		_tcslwr_s(ext, MAX_PATH);
		if (lstrcmp(ext, _T(".jpg")) == 0 || lstrcmp(ext, _T(".png")) == 0) {
			arImage.push_back(Path);
			addCount++;
		}
	}
	SaveImageList();

	// 방금 추가한 이미지가 바로 보이도록 한다. 
	if (addCount != 0) {
		nowIdx = oldCount - 1;
		SendMessage(hBaseWnd, WM_TIMER, 1, 0);
	}
	return FALSE;
}

LRESULT CImageSlide::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_IMAGESLIDE),
			hBaseWnd, ImageSlideDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}
	return FALSE;
}

INT_PTR CALLBACK CImageSlide::ImageSlideDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CImageSlide* pTuingget;
	INT radio = 0;
	INT list_count, list_idx, new_idx;
	TCHAR oneFile[MAX_PATH];

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CImageSlide*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		SendDlgItemMessage(hDlg, IDC_SPINIMAGECYCLE, UDM_SETRANGE, 0, MAKELPARAM(300, 1));
		SetDlgItemInt(hDlg, IDC_EDIMAGECYCLE, pTuingget->timerInterval, FALSE);

		SendDlgItemMessage(hDlg, IDC_CHKRANDOMSLIDE, BM_SETCHECK, pTuingget->randomSlide ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKDISPLAYFILENAME, BM_SETCHECK, pTuingget->displayFileName ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_CHKMULTIVIEWER, BM_SETCHECK, pTuingget->multiViewer ? BST_CHECKED : BST_UNCHECKED, 0);

		// 이미지 목록을 읽어 온다. 
		for (INT i = 0; i < (INT)pTuingget->arImage.size(); i++) {
			SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_ADDSTRING, 0, (LPARAM)pTuingget->arImage[i].c_str());
		}
		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			pTuingget->timerInterval = GetDlgItemInt(hDlg, IDC_EDIMAGECYCLE, NULL, FALSE);
			pTuingget->randomSlide = IsDlgButtonChecked(hDlg, IDC_CHKRANDOMSLIDE);
			pTuingget->displayFileName = IsDlgButtonChecked(hDlg, IDC_CHKDISPLAYFILENAME);
			pTuingget->multiViewer = IsDlgButtonChecked(hDlg, IDC_CHKMULTIVIEWER);
			SetTimer(pTuingget->hBaseWnd, 1, pTuingget->timerInterval * 1000, NULL);

			// 설정 저장. 실제 변경할 때 저장한다.
			pTuingget->SaveSetting();

			// 파일 목록 재구성. 목록 비울 때 내부 버퍼 직접 정리해야 함. 
			pTuingget->arImage.clear();
			list_count = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCOUNT, 0, 0);
			for (INT i = 0; i < list_count; i++) {
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETTEXT, i, (LPARAM)oneFile);
				pTuingget->arImage.push_back(oneFile);
			}

			// 목록 저장. 대화상자 또는 드롭시에 저장한다. 
			pTuingget->SaveImageList();

			// 목록을 다 지워 버렸으면 안내 메시지를 다시 그리도록 한다.
			if (pTuingget->arImage.size() == 0) {
				InvalidateRect(pTuingget->hBaseWnd, NULL, TRUE);
			}

			// 여기서 파괴하면 WM_NCACTIVATE 재진입함. 재진입 조건을 먼저 없앤 후 파괴해야 한다.
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			// ESC 버튼을 누르면 저장없이 취소한다.
		case IDCANCEL:
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			return TRUE;
		case IDC_BTNIS_RESET:
			SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_RESETCONTENT, 0, 0);
			return TRUE;
		case IDC_BTNIS_DEL:
			list_idx = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCURSEL, 0, 0);
			if (list_idx != -1) {
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_DELETESTRING, list_idx, 0);
				list_count = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCOUNT, 0, 0);
				// 삭제한 위치에 선택 유지하되 목록 끝에서는 마지막 항목 선택
				new_idx = list_idx < list_count ? list_idx : list_count - 1;
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_SETCURSEL, new_idx, 0);
			}
			return TRUE;
			// todo : 드래그해서 이동하는 기능 필요. 이미지 미리 보기가 있어야 보고 순서를 조정할 수 있음. 
		case IDC_BTNIS_UP:
			list_idx = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCURSEL, 0, 0);
			if (list_idx != -1 && list_idx != 0) {
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETTEXT, list_idx, (LPARAM)oneFile);
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_DELETESTRING, list_idx, 0);
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_INSERTSTRING, list_idx - 1, (LPARAM)oneFile);
				new_idx = list_idx - 1;
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_SETCURSEL, new_idx, 0);
			}
			return TRUE;
		case IDC_BTNIS_DOWN:
			list_idx = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCURSEL, 0, 0);
			list_count = (INT)SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETCOUNT, 0, 0);
			if (list_idx != -1 && list_idx != list_count - 1) {
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_GETTEXT, list_idx, (LPARAM)oneFile);
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_DELETESTRING, list_idx, 0);
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_INSERTSTRING, list_idx + 1, (LPARAM)oneFile);
				new_idx = list_idx + 1;
				SendDlgItemMessage(hDlg, IDC_LIIMGLIST, LB_SETCURSEL, new_idx, 0);
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

VOID CImageSlide::LoadSetting() {
	timerInterval = setting.get(baseget_id, _T("timerInterval"), 5);
	randomSlide = setting.get(baseget_id, _T("randomSlide"), FALSE);
	displayFileName = setting.get(baseget_id, _T("displayFileName"), FALSE);
	multiViewer = setting.get(baseget_id, _T("multiViewer"), FALSE);
}

VOID CImageSlide::SaveSetting() {
	setting.set(baseget_id, _T("timerInterval"), timerInterval);
	setting.set(baseget_id, _T("randomSlide"), randomSlide);
	setting.set(baseget_id, _T("displayFileName"), displayFileName);
	setting.set(baseget_id, _T("multiViewer"), multiViewer);
}

VOID CImageSlide::LoadImageList() {
	TCHAR* buf;
	TCHAR* p, * pEnd;
	TCHAR oneFile[MAX_PATH];

	// 최대 1000개 정도이나 실제로는 더 됨. 평균 5000개는 충분히 될 듯 함.
	INT bufSize = MAX_PATH * 1000 * sizeof(TCHAR);
	buf = (TCHAR*)malloc(bufSize);
	setting.get(baseget_id, _T("ImageList"), _T(""), buf, bufSize);
	arImage.clear();
	for (p = buf; *p != 0;) {
		pEnd = _tcsstr(p, _T("§"));
		memset(oneFile, 0, sizeof(oneFile));
		_tcsncpy_s(oneFile, MAX_PATH, p, pEnd - p);
		arImage.push_back(oneFile);
		p = pEnd + 1;
	}
	free(buf);
}

VOID CImageSlide::SaveImageList() {
	TCHAR* buf;
	INT num;

	// INI 파일의 정보는 무조건 한줄이어서 개행해서는 안되다. 
	// 탭으로 구분했더니 INI 읽기 함수가 마지막 탭을 제외해 버려 섹션 마크를 구분자로 사용한다.
	// 목록이 비었을 때 0바이트를 할당할 수는 없으므로 빈 문자열이라도 할당해야 한다. 
	num = static_cast<INT>(arImage.size());
	if (num == 0) {
		buf = (TCHAR*)calloc(128, sizeof(TCHAR));
	} else {
		buf = (TCHAR*)calloc(MAX_PATH * num, sizeof(TCHAR));
		for (INT i = 0; i < (INT)arImage.size(); i++) {
			lstrcat(buf, arImage[i].c_str());
			lstrcat(buf, _T("§"));
		}
	}
	setting.set(baseget_id, _T("ImageList"), buf);
	free(buf);
}

// 메모장 위젯 todo
// 김태희 컨트롤 GDI 버전 만들어 lib로 링크. 서식있는 텍스트 지원
// 커스텀 색상 선택 콤보 박스 작성.
// 팝업의 위쪽 흰 영역 안 보이도록 제거할 것

sNamedColor arMemoBack[] = {
	{_T("LemonChiffon"), 0}, {_T("GreenYellow"), 0}, {_T("DeepSkyBlue"), 0},
	{_T("Plum"), 0}, {_T("LightPink"), 0}, {_T("Snow"), 0},
};

sNamedColor arMemoFore[] = {
	{_T("Black"), 0}, {_T("Red"), 0}, {_T("Blue"), 0},
	{_T("Magenta"), 0}, {_T("Maroon"), 0}, {_T("Indigo"), 0},
};

LRESULT CMemo::OnCreate(WPARAM wParam, LPARAM lParam) {
	hCombo = CreateWindow(_T("combobox"), NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, 2, 2, 100, 200, hBaseWnd, (HMENU)MEMO_COMBO, g_hInst, NULL);
	hAdd = CreateWindow(_T("button"), _T("+"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, 20, 20, 100, 25, hBaseWnd, (HMENU)MEMO_ADD, g_hInst, NULL);
	hDelete = CreateWindow(_T("button"), _T("-"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, 20, 50, 100, 25, hBaseWnd, (HMENU)MEMO_DELETE, g_hInst, NULL);
	hView = CreateWindow(_T("button"), _T("ㅁ"), WS_CHILD | WS_VISIBLE |
		BS_PUSHBUTTON, 20, 20, 100, 25, hBaseWnd, (HMENU)MEMO_VIEW, g_hInst, NULL);
	hEdit = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		ES_AUTOHSCROLL | ES_MULTILINE, 10, 10, 200, 25, hBaseWnd, (HMENU)MEMO_EDIT, g_hInst, NULL);
	SendMessage(hCombo, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));
	SendMessage(hEdit, WM_SETFONT, (WPARAM)mainFont[1], MAKELPARAM(FALSE, 0));

	// 색상 초기화
	if (arMemoBack[0].color == 0) {
		for (sNamedColor& nc : arMemoBack) {
			nc.color = GetColorFromName(nc.name);
		}
		for (sNamedColor& nc : arMemoFore) {
			nc.color = GetColorFromName(nc.name);
		}
	}

	// 최초 실행시 파일이 없으면 선택 없음으로 초기화
	nowIdx = -1;

	// 메모 목록을 읽어온다. 
	LoadMemoList();

	// 콤보 박스에 메모 목록을 채워 넣고 마지막 항목을 선택해 준다. 
	for (size_t i = 0; i < arMemo.size(); i++) {
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)arMemo[i]->title);
	}

	if (nowIdx == -1) {
		SetWindowText(hEdit, _T("메모가 하나도 없습니다.\r\n+ 버튼을 눌러 메모를 추가하십시오."));
		EnableWindow(hEdit, FALSE);
	} else {
		SendMessage(hCombo, CB_SETCURSEL, nowIdx, 0);
		SetWindowText(hEdit, arMemo[nowIdx]->content);
	}

	return FALSE;
}

LRESULT CMemo::OnDestroy(WPARAM wParam, LPARAM lParam) {
	SaveMemo(nowIdx);
	if (isModi) {
		SaveMemoList();
	}
	return FALSE;
}

LRESULT CMemo::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	INT w = LOWORD(lParam);
	INT h = HIWORD(lParam);

	MoveWindow(hCombo, 2, 2, w - 70, 200, TRUE);
	MoveWindow(hAdd, w - 66, 5, 20, 20, TRUE);
	MoveWindow(hDelete, w - 44, 5, 20, 20, TRUE);
	MoveWindow(hView, w - 22, 5, 20, 20, TRUE);
	MoveWindow(hEdit, 2, 28, w - 4, h - 30, TRUE);

	return FALSE;
}

LRESULT CMemo::OnCommand(WPARAM wParam, LPARAM lParam) {
	sMemo* pMemo;
	TCHAR name[64];
	BOOL find;
	INT idx;
	TCHAR mes[128];

	switch (LOWORD(wParam)) {
	case MEMO_ADD:
		SaveMemo(nowIdx);

		// 새 메모의 번호를 찾는다. 
		for (INT i = 1;; i++) {
			wsprintf(name, _T("새 메모%d"), i);
			find = FALSE;
			for (size_t m = 0; m < arMemo.size(); m++) {
				if (lstrcmp(arMemo[m]->title, name) == 0) {
					find = TRUE;
					break;
				}
			}
			if (find == FALSE) break;
		}

		// 전부 0으로 리셋한 후 초기화한다. 안 그러면 데이터 파일이 지저분하게 저장된다. 
		pMemo = (sMemo*)calloc(sizeof(sMemo), 1);
		lstrcpy(pMemo->title, name);
		// 대충 적당한 곳에 열어 준다. 
		pMemo->x = 10;
		pMemo->y = 10;
		pMemo->w = 400;
		pMemo->h = 500;
		lstrcpy(pMemo->fontFace, _T("맑은 고딕"));
		pMemo->fontSize = 10;
		pMemo->isText = TRUE;
		pMemo->foreColor = RGB(0, 0, 0);
		pMemo->backColor = arMemoBack[arMemo.size() % Count(arMemoBack)].color;
		pMemo->wordWrap = FALSE;
		pMemo->content = (TCHAR*)calloc(1, sizeof(TCHAR));
		pMemo->content_len = 1;
		pMemo->isModi = FALSE;
		pMemo->pViewer = NULL;

		arMemo.push_back(pMemo);
		nowIdx = static_cast<INT>(SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)pMemo->title));
		SendMessage(hCombo, CB_SETCURSEL, nowIdx, 0);

		SetWindowText(hEdit, _T(""));
		EnableWindow(hEdit, TRUE);
		MySetImeMode(hEdit, TRUE);
		SetFocus(hEdit);

		isModi = TRUE;
		break;
	case MEMO_DELETE:
		idx = static_cast<INT>(SendMessage(hCombo, CB_GETCURSEL, 0, 0));
		if (idx != -1) {
			GetWindowText(hCombo, name, 64);
			wsprintf(mes, _T("%s 메모를 삭제합니다."), name);
			if (MessageBox(hBaseWnd, mes, _T("질문"), MB_YESNO) == IDYES) {

				// 뷰어가 열려 있으면 닫는다. 단, 이때는 부모가 뷰어 객체를 직접 삭제하며 뷰어가 부모에게 삭제 요청을 보낼 필요가 없다.
				// 파괴만 하면 뷰어가 WM_USER+1을 붙이는데 OnUser1 처리 시점에 arMemo[idx]가 삭제되어 버려 다운된다. 
				if (arMemo[idx]->pViewer != NULL) {
					arMemo[idx]->pViewer->destroyBySelf = FALSE;
					DestroyWindow(arMemo[idx]->pViewer->hBaseWnd);
					delete(arMemo[idx]->pViewer);
				}

				// 내용 삭제하고 메모도 삭제한다.
				free(arMemo[idx]->content);
				arMemo.erase(arMemo.begin() + idx);

				// 콤보에서 삭제. 끝 항목이었으면 윗항목으로 이동
				SendMessage(hCombo, CB_DELETESTRING, idx, 0);
				if (nowIdx == SendMessage(hCombo, CB_GETCOUNT, 0, 0)) {
					nowIdx--;
				}
				// 새 위치의 메모 읽어옴
				if (nowIdx == -1) {
					SetWindowText(hEdit, _T("메모가 하나도 없습니다.\r\n+ 버튼을 눌러 메모를 추가하십시오."));
					EnableWindow(hEdit, FALSE);
				}
				else {
					SendMessage(hCombo, CB_SETCURSEL, nowIdx, 0);
					pMemo = arMemo[nowIdx];
					SetWindowText(hEdit, pMemo->content);
				}

			}
		}
		break;
	case MEMO_VIEW:
		if (nowIdx != -1) {
			// 일단 편집하던거부터 저장하고
			SaveMemo(nowIdx);
			// 뷰어가 열려 있으면 포커스를 주고 아니면 생성한다.
			if (arMemo[nowIdx]->pViewer != NULL) {
				SetFocus(arMemo[nowIdx]->pViewer->hBaseWnd);
				arMemo[nowIdx]->pViewer->RefreshMemo();
			}
			else {
				arMemo[nowIdx]->pViewer = new CMemoViewer();
				arMemo[nowIdx]->pViewer->Create(this, nowIdx);
			}
		}
		break;
	case MEMO_COMBO:
		switch (HIWORD(wParam)) {
		case CBN_SELCHANGE:
			SaveMemo(nowIdx);
			// 새 메모의 내용을 읽어온다. 
			nowIdx = static_cast<INT>(SendMessage(hCombo, CB_GETCURSEL, 0, 0));
			pMemo = arMemo[nowIdx];
			SetWindowText(hEdit, pMemo->content);

			break;
		}
		break;
	case MEMO_EDIT:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			if (nowIdx != -1) {
				// 메모와 전체 목록에 변화가 있음을 기록해 둔다. 
				arMemo[nowIdx]->isModi = true;
				isModi = true;
			}
			break;
		}
		break;
	}
	return FALSE;
}

// 뷰어가 닫힐 때 이 메시지를 붙인다. wParam에 메모의 idx를 전달한다.
LRESULT CMemo::OnUser1(WPARAM wParam, LPARAM lParam) {
	delete(arMemo[wParam]->pViewer);
	arMemo[wParam]->pViewer = NULL;
	return FALSE;
}

VOID CMemo::SaveMemo(INT idx) {
	sMemo* pMemo;
	// 최초 메모가 없는 상태에서는 그냥 리턴한다. 
	if (idx == -1) return;

	// 이전 메모가 변경되었으면 길이에 맞추어 메모리 재할당하고 저장한다. 
	pMemo = arMemo[idx];
	if (pMemo->isModi == TRUE) {
		SaveMemo(pMemo, hEdit);
		pMemo->isModi = FALSE;
		isModi = TRUE;
	}
}

// 메모의 내용을 hEdit의 내용으로 저장한다. hEdit는 뷰어의 것일 수도 있고 메모위젯의 것일 수도 있다. 
VOID CMemo::SaveMemo(sMemo* pMemo, HWND hEdit) {
	// 널 종료문자분만큼 할당한다.
	INT newlen = GetWindowTextLength(hEdit) + 1;
	if (pMemo->content_len < newlen) {
		pMemo->content = (TCHAR*)realloc(pMemo->content, newlen * sizeof(TCHAR));
		pMemo->content_len = newlen;
	}
	GetWindowText(hEdit, pMemo->content, pMemo->content_len);
}

VOID CMemo::SaveMemoList() {
	TCHAR Path[MAX_PATH];
	HANDLE hFile;
	DWORD dwWritten;

	GetMemoFilePath(Path);
	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// 식별자 저장
	LPCTSTR magic = _T("TuingAppMemo");
	WriteFile(hFile, magic, lstrlen(magic) * sizeof(TCHAR), &dwWritten, NULL);
	// 버전 저장
	INT Ver = 100;
	WriteFile(hFile, &Ver, sizeof(Ver), &dwWritten, NULL);
	// 마지막 보던 메모 번호와 개수
	WriteFile(hFile, &nowIdx, sizeof(nowIdx), &dwWritten, NULL);
	size_t num = arMemo.size();
	WriteFile(hFile, &num, sizeof(num), &dwWritten, NULL);

	// 순회하며 메모 저장
	for (size_t i = 0; i < arMemo.size(); i++) {
		// 메모 정보 저장.
		WriteFile(hFile, arMemo[i], sizeof(sMemo), &dwWritten, NULL);
		// 메모 내용 저장. 널 종료 문자도 같이 저장됨. 
		WriteFile(hFile, arMemo[i]->content, arMemo[i]->content_len * sizeof(TCHAR), &dwWritten, NULL);
	}

	CloseHandle(hFile);
}

VOID CMemo::LoadMemoList() {
	TCHAR Path[MAX_PATH];
	HANDLE hFile;
	DWORD dwRead;

	GetMemoFilePath(Path);
	hFile = CreateFile(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// 최초 실행시 파일이 없으면 그냥 리턴한다. 
	if (hFile == INVALID_HANDLE_VALUE) return;

	LPCTSTR magic = _T("TuingAppMemo");
	TCHAR buf[100] = { 0, };
	ReadFile(hFile, &buf, lstrlen(magic) * sizeof(TCHAR), &dwRead, NULL);
	if (lstrcmp(buf, magic) != 0) return;

	// 버전 확인
	INT ver;
	ReadFile(hFile, &ver, sizeof(INT), &dwRead, NULL);
	if (ver != 100) return;

	ReadFile(hFile, &nowIdx, sizeof(nowIdx), &dwRead, NULL);
	INT num;
	ReadFile(hFile, &num, sizeof(num), &dwRead, NULL);

	for (INT i = 0; i < num; i++) {
		sMemo* pMemo;
		pMemo = (sMemo*)malloc(sizeof(sMemo));
		ReadFile(hFile, pMemo, sizeof(sMemo), &dwRead, NULL);
		pMemo->content = (TCHAR*)calloc(pMemo->content_len, sizeof(TCHAR));
		ReadFile(hFile, pMemo->content, pMemo->content_len * sizeof(TCHAR), &dwRead, NULL);

		// 런타임 데이터는 리셋한다.
		pMemo->isModi = FALSE;
		pMemo->pViewer = nullptr;
		arMemo.push_back(pMemo);
	}
	CloseHandle(hFile);
}

// 실행 파일 폴더에 실행파일명.memo 파일 경로를 구한다. 
VOID CMemo::GetMemoFilePath(TCHAR* Path) {
	GetModuleFileName(NULL, Path, MAX_PATH);
	TCHAR* p = _tcsrchr(Path, '.');
	*(p + 1) = 0;
	lstrcat(Path, baseget_id.c_str());
}

VOID CMemoViewer::Create(CMemo* pMemoWidget, INT idx) {
	this->pMemoWidget = pMemoWidget;
	this->nowIdx = idx;
	this->pMemo = pMemoWidget->arMemo[idx];

	// 최후 실행 위치에 윈도우를 생성한다. 
	CreateWindow(_T("CBaseWnd"), pMemo->title, WS_POPUPWINDOW | WS_THICKFRAME | WS_VISIBLE,
		pMemo->x, pMemo->y, pMemo->w, pMemo->h,
		NULL, (HMENU)NULL, g_hInst, this);

	MoveWindow(hBaseWnd, pMemo->x, pMemo->y, pMemo->w, pMemo->h, FALSE);
}

LRESULT CMemoViewer::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	POINT pt;

	switch (iMessage) {
	case WM_NCHITTEST:
		INT nHit;
		nHit = (INT)DefWindowProc(hBaseWnd, WM_NCHITTEST, wParam, lParam);
		pt.x = (INT)(SHORT)LOWORD(lParam);
		pt.y = (INT)(SHORT)HIWORD(lParam);
		ScreenToClient(hBaseWnd, &pt);

		if (nHit == HTCLIENT) {
			if (pt.y < mainWindow.capThick) return HTCAPTION;
		}

		return nHit;
	case WM_SETFOCUS:
		RefreshMemo();
		SetFocus(hEdit);
		return 0;
	case WM_CTLCOLOREDIT:
		if ((HWND)lParam == hEdit) {
			SetTextColor((HDC)wParam, pMemo->foreColor);
			SetBkColor((HDC)wParam, pMemo->backColor);
			return (LRESULT)backBrush;
		}
		break;
	}

	return CBaseWnd::OnMessage(iMessage, wParam, lParam);
}

LRESULT CMemoViewer::OnCreate(WPARAM wParam, LPARAM lParam) {
	destroyBySelf = TRUE;

	hClose = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_PUSH,
		0, 0, 16, 16, hBaseWnd, (HMENU)MV_CLOSE, g_hInst, NULL);
	SendMessage(hClose, GBM_SETIMAGE, MAKEWPARAM(IDB_CLOSEUP, IDB_CLOSEHOT), MAKEWPARAM(IDB_CLOSEDOWN, IDB_CLOSEUP));
	hAttr = CreateWindow(_T("GrpBtn"), NULL, WS_CHILD | WS_VISIBLE | GBS_PUSH,
		0, 0, 16, 16, hBaseWnd, (HMENU)MV_ATTR, g_hInst, NULL);
	SendMessage(hAttr, GBM_SETIMAGE, MAKEWPARAM(IDB_PROPUP, IDB_PROPHOT), MAKEWPARAM(IDB_PROPDOWN, IDB_PROPUP));
	hEdit = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE |
		ES_AUTOHSCROLL, 0, 0, 200, 25, hBaseWnd, (HMENU)MV_EDIT, g_hInst, NULL);

	HDC hdc = GetDC(NULL);
	fontEdit = CreateFont(pMemo->fontSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72,
		0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_MODERN, pMemo->fontFace);
	ReleaseDC(NULL, hdc);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)fontEdit, MAKELPARAM(FALSE, 0));
	backBrush = CreateSolidBrush(pMemo->backColor);

	SetWindowText(hEdit, pMemo->content);
	SetFocus(hEdit);
	isModi = FALSE;

	return FALSE;
}

LRESULT CMemoViewer::OnDestroy(WPARAM wParam, LPARAM lParam) {
	// 최후 실행 위치를 저장한다.
	RECT wrt;
	GetWindowRect(hBaseWnd, &wrt);
	pMemo->x = wrt.left;
	pMemo->y = wrt.top;
	pMemo->w = (wrt.right - wrt.left);
	pMemo->h = (wrt.bottom - wrt.top);

	DeleteObject(fontEdit);
	DeleteObject(backBrush);
	// 부모에게 종료를 알려 뷰어 객체를 지우도록 한다. 파괴중의 남은 메시지를 다 처리해야 하기 때문에 Send가 아니고 Post다. 
	// 단, 자신이 직접 종료할 때만 부모에게 파괴를 요청하고 부모가 직접 파괴할 때는 그럴 필요 없다. 
	if (destroyBySelf) {
		PostMessage(pMemoWidget->hBaseWnd, WM_USER + 1, nowIdx, 0);
	}
	return FALSE;
}

VOID CMemoViewer::OnDraw(HDC hdc, INT width, INT height) {
	// 캡션 영역도 배경색으로 채우고
	HBRUSH brush = CreateSolidBrush(pMemo->backColor);
	FillRect(hdc, 0, 0, width, mainWindow.capThick, brush);
	DeleteObject(brush);

	// 아래쪽에 회색선을 하나 긋는다. 나의 디자인 한계는 여기까지이다. 
	HPEN pen, oldPen;
	pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	oldPen = (HPEN)SelectObject(hdc, pen);
	MoveToEx(hdc, 5, mainWindow.capThick - 1, NULL); LineTo(hdc, width - 5, mainWindow.capThick - 1);
	DeleteObject(SelectObject(hdc, oldPen));

	SetBkMode(hdc, TRANSPARENT);
	HFONT oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
	TextOutVCenter(hdc, 4, mainWindow.capThick / 2, pMemo->title);
	SelectObject(hdc, oldFont);
}

VOID CMemoViewer::SendMemoToWidget() {
	// 편집되었으면 메모를 저장한다. 
	if (isModi) {
		pMemoWidget->SaveMemo(pMemo, hEdit);
		// 지금 편집하고 있는 메모를 표시하고 있으면 에디트도 즉시 갱신한다. 
		if (pMemoWidget->nowIdx == nowIdx) {
			SetWindowText(pMemoWidget->hEdit, pMemo->content);
		}
		isModi = FALSE;
	}
}

LRESULT CMemoViewer::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	INT w = LOWORD(lParam);
	INT h = HIWORD(lParam);

	MoveWindow(hClose, w - 20, 4, 16, 16, TRUE);
	MoveWindow(hAttr, w - 40, 4, 16, 16, TRUE);
	MoveWindow(hEdit, 0, mainWindow.capThick, w, h - mainWindow.capThick, TRUE);

	return FALSE;
}

INT_PTR CALLBACK MemoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CMemoViewer* pViewer;
	static CMemo::sMemo* pMemo;
	LPMEASUREITEMSTRUCT lpmis;
	INT idx;

	switch (iMessage) {
	case WM_INITDIALOG:
		pViewer = (CMemoViewer*)lParam;
		pMemo = pViewer->pMemo;

		if (pMemo->isText) {
			CheckRadioButton(hDlg, IDC_RAFORMAT1, IDC_RAFORMAT2, IDC_RAFORMAT1);
		} else {
			CheckRadioButton(hDlg, IDC_RAFORMAT1, IDC_RAFORMAT2, IDC_RAFORMAT2);
		}

		SetDlgItemText(hDlg, IDC_EDTITLE, pMemo->title);

		SendDlgItemMessage(hDlg, IDC_LIFONT, LB_ADDSTRING, 0, (LPARAM)_T("굴림"));
		SendDlgItemMessage(hDlg, IDC_LIFONT, LB_ADDSTRING, 0, (LPARAM)_T("바탕"));
		SendDlgItemMessage(hDlg, IDC_LIFONT, LB_ADDSTRING, 0, (LPARAM)_T("궁서"));
		SendDlgItemMessage(hDlg, IDC_LIFONT, LB_ADDSTRING, 0, (LPARAM)_T("맑은 고딕"));

		if (lstrcmp(pMemo->fontFace, _T("굴림")) == 0) SendDlgItemMessage(hDlg, IDC_LIFONT, LB_SETCURSEL, 0, 0);
		if (lstrcmp(pMemo->fontFace, _T("바탕")) == 0) SendDlgItemMessage(hDlg, IDC_LIFONT, LB_SETCURSEL, 1, 0);
		if (lstrcmp(pMemo->fontFace, _T("궁서")) == 0) SendDlgItemMessage(hDlg, IDC_LIFONT, LB_SETCURSEL, 2, 0);
		if (lstrcmp(pMemo->fontFace, _T("맑은 고딕")) == 0) SendDlgItemMessage(hDlg, IDC_LIFONT, LB_SETCURSEL, 3, 0);

		SendDlgItemMessage(hDlg, IDC_SPINFONTSIZE, UDM_SETRANGE, 0, MAKELPARAM(64, 8));
		SetDlgItemInt(hDlg, IDC_EDFONTSIZE, pMemo->fontSize, FALSE);

		// 색상수만큼 콤보 박스 채우고 현재 색상을 선택한다. 색상표에 없는 색이면 0번을 선택한다.
		idx = 0;
		for (INT i = 0; i < Count(arMemoBack); i++) {
			SendDlgItemMessage(hDlg, IDC_CBBACK, CB_ADDSTRING, 0, 0);
			if (pMemo->backColor == arMemoBack[i].color) idx = i;
		}
		SendDlgItemMessage(hDlg, IDC_CBBACK, CB_SETCURSEL, idx, 0);

		idx = 0;
		for (INT i = 0; i < Count(arMemoFore); i++) {
			SendDlgItemMessage(hDlg, IDC_CBFORE, CB_ADDSTRING, 0, 0);
			if (pMemo->foreColor == arMemoFore[i].color) idx = i;
		}
		SendDlgItemMessage(hDlg, IDC_CBFORE, CB_SETCURSEL, idx, 0);

		return TRUE;
	case WM_MEASUREITEM:
		lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight = 18;
		return TRUE;
	case WM_DRAWITEM:
		DrawOwnerItem(lParam, wParam == IDC_CBFORE ? arMemoFore : arMemoBack);
		return TRUE;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDTITLE, pMemo->title, Count(pMemo->title));

			idx = static_cast<INT>(SendDlgItemMessage(hDlg, IDC_LIFONT, LB_GETCURSEL, 0, 0));
			if (idx == 0) lstrcpy(pMemo->fontFace, _T("굴림"));
			if (idx == 1) lstrcpy(pMemo->fontFace, _T("바탕"));
			if (idx == 2) lstrcpy(pMemo->fontFace, _T("궁서"));
			if (idx == 3) lstrcpy(pMemo->fontFace, _T("맑은 고딕"));

			pMemo->fontSize = GetDlgItemInt(hDlg, IDC_EDFONTSIZE, NULL, FALSE);
			pMemo->isModi = true;
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_CBFORE:
		case IDC_CBBACK:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				idx = static_cast<INT>(SendDlgItemMessage(hDlg, LOWORD(wParam), CB_GETCURSEL, 0, 0));
				if (LOWORD(wParam) == IDC_CBFORE) {
					pMemo->foreColor = arMemoFore[idx].color;
				}
				else {
					pMemo->backColor = arMemoBack[idx].color;
				}
				break;
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CMemoViewer::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case MV_CLOSE:
		DestroyWindow(hBaseWnd);
		break;
	case MV_ATTR:
		if (DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_MEMO), hBaseWnd, MemoDlgProc, (LPARAM)this) == IDOK) {
			DeleteObject(fontEdit);
			HDC hdc = GetDC(NULL);
			fontEdit = CreateFont(pMemo->fontSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72,
				0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1,
				VARIABLE_PITCH | FF_MODERN, pMemo->fontFace);
			ReleaseDC(NULL, hdc);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)fontEdit, MAKELPARAM(FALSE, 0));
			DeleteObject(backBrush);
			backBrush = CreateSolidBrush(pMemo->backColor);

			// 제목이 바뀐 경우는 위젯의 콤보 문자열도 바꾸고 뷰어의 타이틀 바도 바꾼다. 
			TCHAR tTitle[64];
			SendMessage(pMemoWidget->hCombo, CB_GETLBTEXT, nowIdx, (LPARAM)tTitle);
			if (lstrcmp(pMemo->title, tTitle) != 0) {
				// 콤보의 항목 문자열을 직접 바꾸는 방법이 없어 삭제후 재삽입. 이전 선택 상태 복원해 줘야 함
				// 더 좋은 방법이 없는지 찾아 볼 것
				INT oldIdx = static_cast<INT>(SendMessage(pMemoWidget->hCombo, CB_GETCURSEL, 0, 0));
				SendMessage(pMemoWidget->hCombo, CB_DELETESTRING, nowIdx, 0);
				SendMessage(pMemoWidget->hCombo, CB_INSERTSTRING, nowIdx, (LPARAM)pMemo->title);
				SendMessage(pMemoWidget->hCombo, CB_SETCURSEL, oldIdx, 0);
				// 뷰어의 타이틀 바도 다시 그린다. 
				InvalidateRect(hBaseWnd, NULL, TRUE);
			}
		}
		break;
	case MV_EDIT:
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			isModi = TRUE;
			break;
			// 포커스를 잃을 때 즉시 저장한다. 종료할 때도 자동으로 저장된다. 
		case EN_KILLFOCUS:
			SendMemoToWidget();
			return 0;
		}
		break;
	}
	return FALSE;
}

VOID CMemoViewer::RefreshMemo() {
	SetWindowText(hEdit, pMemo->content);
}

LRESULT CDday::OnCreate(WPARAM wParam, LPARAM lParam) {
	LoadSetting();
	SetTimer(hBaseWnd, 1, 60000, NULL);
	SendMessage(hBaseWnd, WM_TIMER, 1, 0);

	return FALSE;
}

LRESULT CDday::OnDestroy(WPARAM wParam, LPARAM lParam) {
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CDday::OnTimer(WPARAM wParam, LPARAM lParam) {
	if (targetTitle == _T("")) return FALSE;

	SYSTEMTIME now;
	FILETIME nowFt, targetFt;
	INT64 now64, target64;
	INT diffMin, diffHour, diffDay;

	GetLocalTime(&now);
	SystemTimeToFileTime(&now, &nowFt);
	SystemTimeToFileTime(&targetDate, &targetFt);
	now64 = (((INT64)nowFt.dwHighDateTime) << 32) + nowFt.dwLowDateTime;
	target64 = (((INT64)targetFt.dwHighDateTime) << 32) + targetFt.dwLowDateTime;
	diffMin = ABS(INT((now64 - target64) / 10000000 / 60));
	diffHour = diffMin / 60 % 24;
	diffDay = diffMin / 1440;

	// 날수와 시간수는 올림 처리한다. 1시 14시간 남았으면 이틀 남은 것으로 표시한다.
	if (displayMinute) {
		wsprintf(remain, _T("%d일 %d시간 %d분"), diffDay, diffHour, diffMin % 60);
	} else if (displayHour) {
		wsprintf(remain, _T("%d일 %d시간"), diffDay, diffHour + 1);
	} else {
		wsprintf(remain, _T("%d일"), diffDay + 1);
	}

	if (now64 > target64) {
		lstrcat(remain, _T("째"));
	} else {
		lstrcat(remain, _T("전"));
	}

	// 출력할 내용이 바뀌었을 때만 다시 그린다. 
	if (lstrcmp(remain, remainOld) != 0) {
		InvalidateRect(hBaseWnd, NULL, FALSE);
	}
	lstrcpy(remainOld, remain);

	return FALSE;
}

VOID CDday::OnDraw(HDC hdc, INT width, INT height) {
	HFONT oldFont;
	INT fontIndex;

	SetBkMode(hdc, TRANSPARENT);
	if (targetTitle == _T("")) {
		RECT crt = { 0, 0, width, height };
		InflateRect(&crt, -2, -5);
		SetTextColor(hdc, GetColorFromName(_T("OrangeRed")));
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
		LPCTSTR mes = _T("Dday 카운터\r\n마우스 오른쪽 버튼을 눌러 목표 날짜와 제목을 설정해라.");
		DrawText(hdc, mes, -1, &crt, DT_WORDBREAK);
		SelectObject(hdc, oldFont);
		return;
	}

	oldFont = (HFONT)SelectObject(hdc, mainFont[3]);
	TextOutSz(hdc, 5, 5, targetTitle.c_str());
	SelectObject(hdc, oldFont);

	if (displayMinute) {
		fontIndex = 3;
	}
	else if (displayHour) {
		fontIndex = 4;
	}
	else {
		fontIndex = 5;
	}
	if (width < 130) fontIndex--;
	if (width < 100) fontIndex--;

	oldFont = (HFONT)SelectObject(hdc, mainFont[fontIndex]);
	SetTextAlign(hdc, TA_CENTER);
	SetTextColor(hdc, GetColorFromName(_T("Blue")));
	TextOutVCenter(hdc, width / 2, height / 2 + 10, remain);
	SelectObject(hdc, oldFont);
}

LRESULT CDday::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	if (!IsWindow(hOptionPopup)) {
		hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_DDAY),
			hBaseWnd, DdayDlgProc, (LPARAM)this);
		ShowWindow(hOptionPopup, SW_SHOW);
	}

	return FALSE;
}

VOID CDday::LoadSetting() {
	TCHAR buf[256];
	setting.get(baseget_id, _T("targetTitle"), _T(""), buf, Count(buf));
	targetTitle = buf;
	targetDate.wYear = setting.get(baseget_id, _T("targetYear"), FALSE);
	targetDate.wMonth = setting.get(baseget_id, _T("targetMonth"), FALSE);
	targetDate.wDay = setting.get(baseget_id, _T("targetDay"), FALSE);
	displayHour = setting.get(baseget_id, _T("displayHour"), FALSE);
	displayMinute = setting.get(baseget_id, _T("displayMinute"), FALSE);
}

VOID CDday::SaveSetting() {
	setting.set(baseget_id, _T("targetTitle"), targetTitle.c_str());
	setting.set(baseget_id, _T("targetYear"), targetDate.wYear);
	setting.set(baseget_id, _T("targetMonth"), targetDate.wMonth);
	setting.set(baseget_id, _T("targetDay"), targetDate.wDay);
	setting.set(baseget_id, _T("displayHour"), displayHour);
	setting.set(baseget_id, _T("displayMinute"), displayMinute);
}

INT_PTR CALLBACK CDday::DdayDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CDday* pTuingget;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CDday*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		if (pTuingget->targetTitle == _T("")) {
			SYSTEMTIME st;
			GetLocalTime(&st);
			SetDlgItemInt(hDlg, IDC_EDTARGETYEAR, st.wYear, FALSE);
			SetDlgItemInt(hDlg, IDC_EDTARGETMONTH, st.wMonth, FALSE);
			SetDlgItemInt(hDlg, IDC_EDTARGETDAY, st.wDay, FALSE);
		} else {
			SetDlgItemText(hDlg, IDC_EDTARGETTITLE, pTuingget->targetTitle.c_str());
			SetDlgItemInt(hDlg, IDC_EDTARGETYEAR, pTuingget->targetDate.wYear, FALSE);
			SetDlgItemInt(hDlg, IDC_EDTARGETMONTH, pTuingget->targetDate.wMonth, FALSE);
			SetDlgItemInt(hDlg, IDC_EDTARGETDAY, pTuingget->targetDate.wDay, FALSE);
		}

		CheckDlgButton(hDlg, IDC_CHKDISPLAYHOUR, pTuingget->displayHour);
		CheckDlgButton(hDlg, IDC_CHKDISPLAYMINUTE, pTuingget->displayMinute);

		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			TCHAR buf[256];
			GetDlgItemText(hDlg, IDC_EDTARGETTITLE, buf, Count(buf));
			pTuingget->targetTitle = buf;
			pTuingget->targetDate.wYear = GetDlgItemInt(hDlg, IDC_EDTARGETYEAR, NULL, FALSE);
			if (pTuingget->targetDate.wYear < 100) pTuingget->targetDate.wYear += 2000;
			pTuingget->targetDate.wMonth = GetDlgItemInt(hDlg, IDC_EDTARGETMONTH, NULL, FALSE);
			pTuingget->targetDate.wDay = GetDlgItemInt(hDlg, IDC_EDTARGETDAY, NULL, FALSE);
			pTuingget->displayHour = IsDlgButtonChecked(hDlg, IDC_CHKDISPLAYHOUR);
			pTuingget->displayMinute = IsDlgButtonChecked(hDlg, IDC_CHKDISPLAYMINUTE);

			pTuingget->SaveSetting();

			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			SendMessage(pTuingget->hBaseWnd, WM_TIMER, 1, 0);
			InvalidateRect(pTuingget->hBaseWnd, NULL, FALSE);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHKDISPLAYMINUTE:
			// 분을 표시하면 시간도 자동으로 같이 표시한다.
			if (IsDlgButtonChecked(hDlg, IDC_CHKDISPLAYMINUTE)) {
				CheckDlgButton(hDlg, IDC_CHKDISPLAYHOUR, BST_CHECKED);
			}
			return TRUE;
		case IDC_CHKDISPLAYHOUR:
			// 시간을 표시하지 않으면 분도 자동으로 표시 해제한다. 
			if (IsDlgButtonChecked(hDlg, IDC_CHKDISPLAYHOUR) == BST_UNCHECKED) {
				CheckDlgButton(hDlg, IDC_CHKDISPLAYMINUTE, BST_UNCHECKED);
			}
			return TRUE;
		case IDCANCEL:
			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// 론처 todo : 드래그중인 아이콘 보여 주기. 0번 이외의 아이콘도 선택 가능하게
LRESULT CLauncher::OnCreate(WPARAM wParam, LPARAM lParam) {
	focusIcon = -1;
	oldFocusIcon = -1;
	clickIcon = -1;
	capHeight = 12;

	DragAcceptFiles(hBaseWnd, TRUE);
	LoadSetting();
	return FALSE;
}

LRESULT CLauncher::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);
	ArrangeIcon();
	return FALSE;
}

VOID CLauncher::OnDraw(HDC hdc, INT width, INT height) {
	HBRUSH bkBrush;
	INT nowIcon;
	RECT frt;
	HPEN pen, oldPen;
	SetBkMode(hdc, TRANSPARENT);
	HFONT oldFont;

	// 등록 아이콘이 하나도 없으면 안내 표시
	if (arProgram.size() == 0) {
		RECT crt = { 0, 0, width, height };
		InflateRect(&crt, -2, -5);
		SetTextColor(hdc, RGB(0, 0, 255));
		oldFont = (HFONT)SelectObject(hdc, mainFont[1]);
		LPCTSTR mes = _T("론처에 등록할 실팽 파일(*.exe) 또는 문서 파일을 드래그하여 목록에 추가하십시오.");
		DrawText(hdc, mes, -1, &crt, DT_WORDBREAK);
		SelectObject(hdc, oldFont);
		return;
	}

	// 커서 위치의 아이콘을 조사한다. 
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hBaseWnd, &pt);
	nowIcon = FindIconFromPt(pt.x, pt.y);
	oldFont = (HFONT)SelectObject(hdc, mainFont[0]);

	for (size_t i = 0; i < arProgram.size(); i++) {
		bkBrush = NULL;
		// 한칸 바깥쪽에 프레임을 그린다.
		CopyRect(&frt, &arProgram[i].rt);
		InflateRect(&frt, 1, 1);

		// 클릭한 아이콘이고 여전히 아이콘 위에 있으면 짙은 배경에 빨간색 테두리, 오른쪽 아래로
		if (i == clickIcon) {
			if (i == nowIcon) {
				OffsetRect(&frt, 1, 1);
				bkBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
				pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, pen);
				Rectangle(hdc, frt.left, frt.top, frt.right, frt.bottom);
				DeleteObject(SelectObject(hdc, oldPen));
			}
		} else {
			// 클릭이 아니면 포커스인지 본다. 클릭이 더 우선이다. 포커스이면 옅은 배경에 파란색 테두리로 그린다.
			if (i == focusIcon) {
				bkBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
				pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
				oldPen = (HPEN)SelectObject(hdc, pen);
				Rectangle(hdc, frt.left, frt.top, frt.right, frt.bottom);
				DeleteObject(SelectObject(hdc, oldPen));
			}
		}

		// 프레임보다 한칸 안쪽에 아이콘 그림. 축소시 품질이 별로 좋지 않아 개선 필요. HALFTON은 효과 없음. 
		DrawIconEx(hdc, frt.left + 1, frt.top + 1, arProgram[i].hIcon,
			iconSize, iconSize, 0, bkBrush, DI_NORMAL);

		// 캡션 표시. 한줄로 표시하되 오른쪽에 줄임 표시한다. 
		if (dispCap) {
			TCHAR fname[MAX_PATH];
			lstrcpy(fname, PathFindFileName(arProgram[i].path));
			TCHAR* p = _tcsrchr(fname, '.');
			if (p != nullptr) *p = 0;
			RECT drt;
			SetRect(&drt, frt.left, frt.bottom, frt.right, frt.bottom + capHeight);
			DrawText(hdc, fname, -1, &drt, DT_CENTER | DT_END_ELLIPSIS);
		}
	}

	SelectObject(hdc, oldFont);
}

LRESULT CLauncher::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	// 클릭했으면 캡처하고 클릭 표시
	clickIcon = FindIconFromPt(LOWORD(lParam), HIWORD(lParam));
	if (clickIcon != -1) {
		SetCapture(hBaseWnd);
		InvalidateRect(hBaseWnd, NULL, FALSE);
	}
	return FALSE;
}

LRESULT CLauncher::OnMouseMove(WPARAM wParam, LPARAM lParam) {
	// 클릭 상태로 이동중일 때는 포커스 변경하지 않는다.
	if (clickIcon != -1) {
		InvalidateRect(hBaseWnd, NULL, FALSE);
	} else {
		// 같은 아이콘 위에서 움직일 때는 굳이 다시 그리지 않는다.
		focusIcon = FindIconFromPt(LOWORD(lParam), HIWORD(lParam));
		if (focusIcon != oldFocusIcon) {
			oldFocusIcon = focusIcon;
			InvalidateRect(hBaseWnd, NULL, FALSE);
		}
	}
	return FALSE;
}

LRESULT CLauncher::OnLButtonUp(WPARAM wParam, LPARAM lParam) {
	// 캡처 상태가 아닐 때는 무시한다. 밖에서 클릭한 후 안으로 들어와 놓을 때
	if (GetCapture() != hBaseWnd) return FALSE;

	ReleaseCapture();
	INT nowIcon = FindIconFromPt(LOWORD(lParam), HIWORD(lParam));
	if (nowIcon != -1) {
		// 같은 아이콘 위에서 놓았으면 실행한다.
		// 메모 : 아이콘 밖으로 나갔다가 다시 들어온 경우도 일단 실행은 하되 이동 취소로 보는게 합리적이다.
		if (nowIcon == clickIcon) {
			TCHAR ext[MAX_PATH];
			if (lstrcmp(GetFileExtension(arProgram[nowIcon].path, ext), _T(".exe")) == 0) {
				STARTUPINFO si = { sizeof(STARTUPINFO), };
				PROCESS_INFORMATION pi;
				TCHAR dir[MAX_PATH];
				lstrcpy(dir, arProgram[nowIcon].path);
				TCHAR* p = _tcsrchr(dir, '\\');
				*(p + 1) = 0;
				CreateProcess(NULL, arProgram[nowIcon].path, NULL, NULL, FALSE, 0, NULL,
					dir, &si, &pi);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			} else {
				// 문서 파일은 연결 프로그램을 실행한다. 
				ShellExecute(hBaseWnd, _T("open"), arProgram[nowIcon].path, NULL, NULL, SW_SHOWNORMAL);
			}
		} else {
			// 위치가 바뀌었으면 마우스를 놓은 자리로 이동한다. 드래그중인 아이콘을 보여주지 않아 비직관적이다.
			sProgram clicked = arProgram[clickIcon];
			arProgram.erase(arProgram.begin() + clickIcon);
			arProgram.insert(arProgram.begin() + nowIcon, clicked);
			ArrangeIcon();
		}
	}
	clickIcon = -1;
	focusIcon = -1;
	oldFocusIcon = -1;
	InvalidateRect(hBaseWnd, NULL, FALSE);

	return FALSE;
}

LRESULT CLauncher::OnDropFiles(WPARAM wParam, LPARAM lParam) {
	TCHAR Path[MAX_PATH];
	INT Count;

	// 오동작 방지를 위해 딱 하나만 등록을 허가한다. 
	Count = DragQueryFile((HDROP)wParam, 0xffffffff, NULL, 0);
	if (Count != 1) {
		MessageBox(hBaseWnd, _T("한번에 하나만 등록할 수 있다."), _T("알림"), MB_OK);
		return FALSE;
	}

	// 마지막에 등록하고 아이콘 바로 추출한다.
	DragQueryFile((HDROP)wParam, 0, Path, MAX_PATH);
	sProgram prg;
	lstrcpy(prg.path, Path);
	prg.nIcon = 0;
	ExtractFileIcon(prg);
	arProgram.push_back(prg);

	// 변화가 있을 때 즉시 저장한다.
	SaveSetting();
	ArrangeIcon();
	InvalidateRect(hBaseWnd, NULL, FALSE);
	return FALSE;
}

LRESULT CLauncher::OnContextMenu(WPARAM wParam, LPARAM lParam) {
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hBaseWnd, &pt);
	INT nowIcon = FindIconFromPt(pt.x, pt.y);

	// 아이콘 바깥일 때 위젯 차제의 메뉴를 연다.
	if (nowIcon == -1) {
		if (!IsWindow(hOptionPopup)) {
			hOptionPopup = CreateDialogParam(g_hInst, MAKEINTRESOURCE(IDD_LAUNCHER),
				hBaseWnd, LauncherDlgProc, (LPARAM)this);
			ShowWindow(hOptionPopup, SW_SHOW);
		}

		return FALSE;
	}

	// 아이콘을 클릭했을 때는 아이콘의 팝업 메뉴를 연다. 
	clickIcon = nowIcon;

	HMENU hMenu, hPopup;
	hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUPMENU));
	hPopup = GetSubMenu(hMenu, 2);

	TrackPopupMenu(hPopup, TPM_LEFTALIGN, (SHORT)LOWORD(lParam),
		(SHORT)HIWORD(lParam), 0, hBaseWnd, NULL);
	DestroyMenu(hMenu);
	return FALSE;
}

LRESULT CLauncher::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case IDM_LAUNCHER_PROP:
		MessageBox(hBaseWnd, _T("낫 임플리멘티드 옛뜨. 아이콘 변경 기능 추가 예정"), _T("알림"), MB_OK);
		clickIcon = -1;
		break;
	case IDM_LAUNCHER_REMOVE:
		arProgram.erase(arProgram.begin() + clickIcon);
		SaveSetting();
		ArrangeIcon();
		InvalidateRect(hBaseWnd, NULL, FALSE);
		clickIcon = -1;
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK CLauncher::LauncherDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static CLauncher* pTuingget;
	INT radio = 0;

	switch (iMessage) {
	case WM_INITDIALOG:
		pTuingget = (CLauncher*)lParam;
		RECT wrt;
		GetWindowRect(pTuingget->hBaseWnd, &wrt);
		MovePopupToSide(hDlg, wrt.left, wrt.top);

		SendDlgItemMessage(hDlg, IDC_SPINLAUNCHERGAP, UDM_SETRANGE, 0, MAKELPARAM(10, 1));

		if (pTuingget->iconSize == 16) radio = IDC_RALAUNCHERICON1;
		if (pTuingget->iconSize == 24) radio = IDC_RALAUNCHERICON2;
		if (pTuingget->iconSize == 32) radio = IDC_RALAUNCHERICON3;
		CheckRadioButton(hDlg, IDC_RALAUNCHERICON1, IDC_RALAUNCHERICON3, radio);

		SetDlgItemInt(hDlg, IDC_EDLAUNCHERGAP, pTuingget->iconGap, FALSE);
		CheckDlgButton(hDlg, IDC_CHKLAUNCHERDISPCAP, pTuingget->dispCap);

		return TRUE;
	case WM_NCACTIVATE:
		if (wParam == FALSE && hOptionPopup != NULL) {
			if (IsDlgButtonChecked(hDlg, IDC_RALAUNCHERICON1)) pTuingget->iconSize = 16;
			if (IsDlgButtonChecked(hDlg, IDC_RALAUNCHERICON2)) pTuingget->iconSize = 24;
			if (IsDlgButtonChecked(hDlg, IDC_RALAUNCHERICON3)) pTuingget->iconSize = 32;

			pTuingget->iconGap = GetDlgItemInt(hDlg, IDC_EDLAUNCHERGAP, NULL, FALSE);
			pTuingget->dispCap = IsDlgButtonChecked(hDlg, IDC_CHKLAUNCHERDISPCAP);

			pTuingget->SaveSetting();

			hOptionPopup = NULL;
			DestroyWindow(hDlg);
			pTuingget->ArrangeIcon();
			InvalidateRect(pTuingget->hBaseWnd, NULL, FALSE);
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

VOID CLauncher::LoadSetting() {
	sProgram prg;
	iconSize = setting.get(baseget_id, _T("iconSize"), 32);
	iconGap = setting.get(baseget_id, _T("iconGap"), 2);
	dispCap = setting.get(baseget_id, _T("dispCap"), 0);

	TCHAR* buf;
	TCHAR* p, * pEnd;

	// 프로그램 목록 읽음. 아이콘은 저장 대상이 아니며 현재는 0번 아이콘만 쓴다.
	INT bufSize = MAX_PATH * 1000 * sizeof(TCHAR);
	buf = (TCHAR*)malloc(bufSize);
	setting.get(baseget_id, _T("ProgramList"), _T(""), buf, bufSize);
	arProgram.clear();
	for (p = buf; *p != 0;) {
		pEnd = _tcsstr(p, _T("§"));
		memset(prg.path, 0, sizeof(prg.path));
		_tcsncpy_s(prg.path, MAX_PATH, p, pEnd - p);
		prg.nIcon = 0;
		ExtractFileIcon(prg);
		arProgram.push_back(prg);
		p = pEnd + 1;
	}
	free(buf);
}

VOID CLauncher::SaveSetting() {
	setting.set(baseget_id, _T("iconSize"), iconSize);
	setting.set(baseget_id, _T("iconGap"), iconGap);
	setting.set(baseget_id, _T("dispCap"), dispCap);

	TCHAR* buf;
	INT num;

	// 프로그램 목록 저장. 
	num = static_cast<INT>(arProgram.size());
	if (num == 0) {
		buf = (TCHAR*)calloc(128, sizeof(TCHAR));
	} else {
		buf = (TCHAR*)calloc(MAX_PATH * num, sizeof(TCHAR));
		for (INT i = 0; i < (INT)arProgram.size(); i++) {
			lstrcat(buf, arProgram[i].path);
			lstrcat(buf, _T("§"));
		}
	}
	setting.set(baseget_id, _T("ProgramList"), buf);
	free(buf);
}

INT CLauncher::FindIconFromPt(INT x, INT y) {
	POINT pt;
	pt.x = x;
	pt.y = y;

	for (size_t i = 0; i < arProgram.size(); i++) {
		if (PtInRect(&arProgram[i].rt, pt)) {
			return static_cast<INT>(i);
		}
	}
	return -1;
}

VOID CLauncher::ArrangeIcon() {
	RECT crt;
	GetClientRect(hBaseWnd, &crt);

	INT x = iconGap;
	INT y = iconGap;
	for (sProgram& prg : arProgram) {
		SetRect(&prg.rt, x, y, x + iconSize, y + iconSize);

		// 한칸 오른쪽으로, 더 표시할 자리가 없으면 한칸 아래로
		x += (iconSize + iconGap);
		if (x > crt.right - iconSize) {
			x = iconGap;
			y += (iconSize + iconGap);
			// 제목이 있으면 제목 높이만큼 더 띄운다.
			if (dispCap) {
				y += capHeight;
			}
		}
	}
}

VOID CLauncher::ExtractFileIcon(sProgram& prg) {
	TCHAR ext[MAX_PATH];
	if (lstrcmp(GetFileExtension(prg.path, ext), _T(".exe")) == 0) {
		prg.hIcon = ExtractIcon(g_hInst, prg.path, prg.nIcon);
		// Win32로 만든 프로그램은 아이콘이 없을 수도 있다.
		if (prg.hIcon == NULL) {
			prg.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_DEFAULTPRG));
		}
	} else {
		WORD iIcon;
		// 문서와 연결된 아이콘을 추출한다.
		prg.hIcon = ExtractAssociatedIcon(g_hInst, prg.path, &iIcon);
	}
}

LRESULT CCalendar::OnCreate(WPARAM wParam, LPARAM lParam) {
	checkMidnight = false;
	lineGap = 18;
	minMonthWidth = 120;
	monthGap = 10;

	SYSTEMTIME st;
	GetLocalTime(&st);

	top_year = today_y = st.wYear;
	top_month = today_m = st.wMonth;
	today_d = st.wDay;

	holyFont = CreateFont(15, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,
		3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("맑은 고딕"));

	click_pt.x = click_pt.y = -1;
	click_y = click_m = click_d = -1;

	SetTimer(hBaseWnd, 1, 60 * 1000, NULL);

	return FALSE;
}

LRESULT CCalendar::OnDestroy(WPARAM wParam, LPARAM lParam) {
	DeleteObject(holyFont);
	KillTimer(hBaseWnd, 1);
	return FALSE;
}

LRESULT CCalendar::OnTimer(WPARAM wParam, LPARAM lParam) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 1분에 한번꼴로 날짜가 바뀌었는지 점검한다.
	if (today_d != st.wDay) {
		today_d = st.wDay;
		InvalidateRect(hBaseWnd, NULL, FALSE);
	}

	return FALSE;
}

LRESULT CCalendar::OnSize(WPARAM wParam, LPARAM lParam) {
	CBaseWnd::OnSize(wParam, lParam);

	return FALSE;
}

VOID CCalendar::OnDraw(HDC hdc, INT width, INT height) {
	INT year = top_year;
	INT month = top_month;
	INT columnNum;
	INT monthWidth;
	INT xpos = 0;
	INT ypos = 0;
	INT column = 0;

	// 한 행에 들어가는 달 수
	columnNum = width / (minMonthWidth + monthGap);
	if (columnNum == 0) columnNum = 1;

	// 한달의 폭.
	if (columnNum == 1) {
		monthWidth = width;
	} else {
		monthWidth = (width - (monthGap * (columnNum - 1))) / columnNum;
	}

	SetBkMode(hdc, TRANSPARENT);

	for (;;) {
		DrawMonth(hdc, monthWidth, year, month, xpos, ypos);
		column++;
		if (column == columnNum) {
			column = 0;
			ypos += 40 + lineGap * 6 + 10;
			xpos = 0;
		} else {
			xpos += (monthWidth + monthGap);
		}
		if (ypos > height) break;
		month++;
		if (month == 13) {
			month = 1;
			year++;
		}
	}
}

VOID CCalendar::DrawMonth(HDC hdc, INT width, INT year, INT month, INT xpos, INT ypos) {
	HBRUSH headBrush, oldBrush;
	HBRUSH todayBrush;
	HPEN headPen, clickPen, oldPen;
	HFONT oldFont;
	static TCHAR arYoil[][4] = { _T("일"), _T("월"), _T("화"), _T("수"),
		_T("목"), _T("금"), _T("토") };
	static TCHAR arYoilEng[][16] = { _T("Sun"), _T("Mon"), _T("Tue"),
		_T("Wen"), _T("Thr"), _T("Fri"), _T("Sat") };
	TCHAR head[128];
	TCHAR day[128];
	RECT drt;

	// 오늘의 배경색, 클릭한 날짜 테두리색
	todayBrush = CreateSolidBrush(GetColorFromName(_T("Gold")));
	clickPen = CreatePen(PS_SOLID, 2, GetColorFromName(_T("Blue")));

	// 헤더 사각형 출력
	headBrush = CreateSolidBrush(GetColorFromName(_T("LightBlue")));
	oldBrush = (HBRUSH)SelectObject(hdc, headBrush);
	headPen = CreatePen(PS_SOLID, 1, GetColorFromName(_T("LightBlue")));
	oldPen = (HPEN)SelectObject(hdc, headPen);
	Rectangle(hdc, xpos, ypos, xpos + width, ypos + 35);
	DeleteObject(SelectObject(hdc, oldPen));
	DeleteObject(SelectObject(hdc, oldBrush));

	// 년월 출력
	wsprintf(head, _T("%d년 %d월"), year, month);
	SetTextAlign(hdc, TA_LEFT);
	SetTextColor(hdc, GetColorFromName(_T("Magenta")));
	oldFont = (HFONT)SelectObject(hdc, mainFont[2]);
	TextOutSz(hdc, xpos + 5, ypos, head);

	// 요일 출력
	SetTextColor(hdc, GetColorFromName(_T("DimGray")));
	SelectObject(hdc, mainFont[1]);
	SetTextAlign(hdc, TA_CENTER);
	DOUBLE gap;
	gap = width / 7.0;
	INT left, right;
	for (INT yoil = 0; yoil < 7; yoil++) {
		left = xpos + INT(yoil * gap);
		right = INT(left + gap);
		TextOutSz(hdc, (left + right) / 2, ypos + 20, arYoil[yoil]);
	}

	// 1일의 요일 구하기
	SYSTEMTIME st = { (WORD)year, (WORD)month, 0, 1, };
	FILETIME fst;
	SystemTimeToFileTime(&st, &fst);
	FileTimeToSystemTime(&fst, &st);
	INT yoil = st.wDayOfWeek;

	// 그 달의 끝날 구하기
	INT endDay;
	static INT arDays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	endDay = arDays[month];
	if (month == 2) {
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) endDay++;
	}

	// 1일부터 끝날까지 순회하며 출력한다.
	INT y = ypos + 40;
	for (INT d = 1; d <= endDay; d++) {
		left = xpos + INT(yoil * gap);
		right = INT(left + gap);
		SetRect(&drt, left, y, right, y + lineGap);
		wsprintf(day, _T("%d"), d);

		// 클릭해 놓은 좌표이면 출력중에 날짜를 조사해 놓는다. 클릭 좌표는 즉시 무효화한다.
		if (click_pt.x != -1 && PtInRect(&drt, click_pt)) {
			click_y = year;
			click_m = month;
			click_d = d;
			click_pt.x = click_pt.y = -1;
		}

		// 오늘 날짜 배경색 강조
		if (year == today_y && month == today_m && d == today_d) {
			FillRect(hdc, &drt, todayBrush);
		}

		// 클릭한 날짜 사각형 테두리 그림
		if (year == click_y && month == click_m && d == click_d) {
			oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			oldPen = (HPEN)SelectObject(hdc, clickPen);
			Rectangle(hdc, drt.left, drt.top, drt.right, drt.bottom);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
		}

		// 날짜 출력
		SetTextColor(hdc, GetColorFromName(yoil == 0 ? _T("Red") : _T("Black")));
		SelectObject(hdc, yoil == 0 ? holyFont : mainFont[1]);
		TextOutVCenter(hdc, (left + right) / 2, y + lineGap / 2, day);

		yoil++;
		if (yoil == 7) {
			yoil = 0;
			y += lineGap;
		}
	}

	SelectObject(hdc, oldFont);
	DeleteObject(clickPen);
	DeleteObject(todayBrush);
}

LRESULT CCalendar::OnLButtonDown(WPARAM wParam, LPARAM lParam) {
	// 일단 클릭해야 키보드로 앞 뒤로 이동 가능하다.
	SetFocus(hBaseWnd);
	// 클릭한 날짜 알아내서 사각형을 표시한다.
	click_pt.x = LOWORD(lParam);
	click_pt.y = HIWORD(lParam);
	// 각 날짜의 영역을 외부에서 조사하기 어려워 출력 루틴에서 조사하도록 강제로 다시 그린다.
	// 날짜가 아닌 곳을 찍어도 이전 클릭 날짜를 유지한다. 
	InvalidateRect(hBaseWnd, NULL, FALSE);
	UpdateWindow(hBaseWnd);
	// 조사 후 다시 한번 더 그려야 한다. 새로 클릭한 날이 더 뒤쪽이면 앞쪽의 클릭한 날이 지워지지 않는다.
	// 앞의 무효화는 다시 그리며 조사만 하고 조사를 마친 후 새로 그려야 정확하다.
	InvalidateRect(hBaseWnd, NULL, FALSE);

	return FALSE;
}

LRESULT CCalendar::OnKeyDown(WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_UP:
		top_month--;
		if (top_month == 0) {
			top_month = 12;
			top_year--;
		}
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		top_month++;
		if (top_month == 13) {
			top_month = 1;
			top_year++;
		}
		InvalidateRect(hBaseWnd, NULL, FALSE);
		break;
	}
	return FALSE;
}

LRESULT CCalendar::OnMouseWheel(WPARAM wParam, LPARAM lParam) {
	// 휠을 굴린 후 키보드로도 조정할 수 있도록 포커스를 준다. 
	SetFocus(hBaseWnd);
	if ((SHORT)HIWORD(wParam) > 0) {
		SendMessage(hBaseWnd, WM_KEYDOWN, VK_UP, 0);
	} else {
		SendMessage(hBaseWnd, WM_KEYDOWN, VK_DOWN, 0);
	}
	return FALSE;
}

