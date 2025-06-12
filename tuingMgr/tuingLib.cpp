#include "framework.h"

#ifdef _DEBUG
VOID Trace(LPCTSTR format, ...) {
	TCHAR buf[1024];
	va_list marker;

	va_start(marker, format);
	_vstprintf_s(buf, Count(buf), format, marker);
	lstrcat(buf, _T("\r\n"));
	OutputDebugString(buf);
}

VOID WriteLog(LPCTSTR strLog, ...) {
	static INT count = 0;
	static LPCTSTR LogPath = _T("c:\\Temp\\TuingApp_Debug.txt");
	HANDLE hLog;
	DWORD dwWritten;
	TCHAR szLog[1024];
	TCHAR strLog2[1024];
	va_list marker;
	SYSTEMTIME st;

	va_start(marker, strLog);
	_vstprintf_s(szLog, Count(szLog), strLog, marker);

	if (count == 0) {
		hLog = CreateFile(LogPath, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	} else {
		hLog = CreateFile(LogPath, GENERIC_WRITE, 0, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	GetLocalTime(&st);
	_stprintf_s(strLog2, Count(strLog2), _T("카운터=%06d(%d:%d:%d:%d) %s\r\n"), count++,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, szLog);
	SetFilePointer(hLog, 0, NULL, FILE_END);
	WriteFile(hLog, strLog2, lstrlen(strLog2), &dwWritten, NULL);
	CloseHandle(hLog);
}
#else
VOID Trace(LPCTSTR format, ...) { ; }
VOID WriteLog(LPCTSTR strLog, ...) { ; }
#endif

VOID DrawBitmap(HDC hdc, INT x, INT y, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP oldBitmap;
	INT bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	oldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, oldBitmap);
	DeleteDC(MemDC);
}

VOID StretchBitmap(HDC hdc, INT x, INT y, INT w, INT h, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP oldBitmap;
	INT bx, by;
	BITMAP bit;

	SetStretchBltMode(hdc, HALFTONE);
	MemDC = CreateCompatibleDC(hdc);
	oldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	StretchBlt(hdc, x, y, w, h, MemDC, 0, 0, bx, by, SRCCOPY);

	SelectObject(MemDC, oldBitmap);
	DeleteDC(MemDC);
}

// 수직 중앙에 문자열을 출력한다. 수평은 hdc의 속성을 따른다.
VOID TextOutVCenter(HDC hdc, INT x, INT y, LPCTSTR str) {
	SIZE sz;
	GetTextExtentPoint32(hdc, str, lstrlen(str), &sz);
	TextOut(hdc, x, y - sz.cy / 2, str, lstrlen(str));
}

VOID FillRect(HDC hdc, INT left, INT top, INT right, INT bottom, HBRUSH brush) {
	RECT rt;
	SetRect(&rt, left, top, right, bottom);
	FillRect(hdc, &rt, brush);
}

// 배열을 순환하며 다음 첨자를 찾는다.
VOID moveNext(INT& now, INT size) {
	if (now == size - 1) {
		now = 0;
	} else {
		now++;
	}
}

// 배열을 순환하며 이전 첨자를 찾는다.
VOID movePrev(INT& now, INT size) {
	if (now == 0) {
		now = size - 1;
	} else {
		now--;
	}
}

VOID MySetImeMode(HWND hEdit, BOOL bHan) {
	HIMC hImc;
	hImc = ImmGetContext(hEdit);

	if (bHan == TRUE) {
		ImmSetConversionStatus(hImc, IME_CMODE_NATIVE, IME_SMODE_NONE);
	} else {
		ImmSetConversionStatus(hImc, 0, IME_SMODE_NONE);
	}

	ImmReleaseContext(hEdit, hImc);
}

SMonitorInfo arMonitor[10];
INT monitorCount;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	MONITORINFOEX mi;

	arMonitor[monitorCount].hMonitor = hMonitor;
	arMonitor[monitorCount].rcMonitor = *lprcMonitor;
	mi.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mi);
	arMonitor[monitorCount].rcWork = mi.rcWork;
	arMonitor[monitorCount].primary = (mi.dwFlags & MONITORINFOF_PRIMARY);
	monitorCount++;
	return TRUE;
}

INT GetMonitorOfWindow(HWND hWnd) {
	HMONITOR hMonitor;
	hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	for (INT i = 0;; i++) {
		if (arMonitor[i].hMonitor == hMonitor) {
			return i;
		}
	}

	return -1;
}

INT GetPrimaryMonitor() {
	for (INT i = 0; i < monitorCount; i++) {
		if (arMonitor[i].primary) {
			return i;
		}
	}

	return -1;
}

// 컨트롤 데이터
struct GrpBtnData {
	HBITMAP Bitmap[4];
	eState State;
};

LRESULT CALLBACK GrpBtnProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
static VOID DeleteAllBitmap(GrpBtnData* pData);
static BOOL IsPtOnMe(HWND hWnd, LPARAM lParam);

// 윈도우 클래스 등록을 도와주는 전역 객체
class GrpBtnRegister {
public:
	GrpBtnRegister() {
		WNDCLASS WndClass;

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = sizeof(LONG_PTR);
		WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = NULL;
		WndClass.hInstance = GetModuleHandle(NULL);
		WndClass.lpfnWndProc = GrpBtnProc;
		WndClass.lpszClassName = _T("GrpBtn");
		WndClass.lpszMenuName = NULL;
		WndClass.style = 0;

		RegisterClass(&WndClass);
	}
} GrpBtnRegisterObject;

LRESULT CALLBACK GrpBtnProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	GrpBtnData* pData;
	RECT crt;
	POINT pt;
	static eState CheckState;
	HWND hParent;

	pData = (GrpBtnData*)GetWindowLongPtr(hWnd, 0);
	switch (iMessage) {
	case WM_CREATE:
		pData = (GrpBtnData*)calloc(sizeof(GrpBtnData), 1);
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)pData);
		return FALSE;
	case GBM_SETIMAGE:
		DeleteAllBitmap(pData);
		pData->Bitmap[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LOWORD(wParam)));
		pData->Bitmap[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(HIWORD(wParam)));
		pData->Bitmap[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LOWORD(lParam)));
		pData->Bitmap[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(HIWORD(lParam)));
		InvalidateRect(hWnd, NULL, FALSE);
		return FALSE;
	case GBM_GETSTATE:
		return pData->State;
	case GBM_SETSTATE:
		if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) != 0) {
			ChangeState(hWnd, (eState)wParam);
		}
		return FALSE;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (pData->Bitmap[pData->State]) {
			DrawBitmap(hdc, 0, 0, pData->Bitmap[pData->State]);
		}
		EndPaint(hWnd, &ps);
		return FALSE;
	case WM_LBUTTONDOWN:
		// 푸시 버튼은 DOWN으로, 체크 버튼은 토클
		if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0) {
			ChangeState(hWnd, GB_DOWN);
		} else {
			CheckState = pData->State;
			ChangeState(hWnd, CheckState == GB_NORMAL ? GB_DOWN : GB_NORMAL);
		}
		SetCapture(hWnd);
		return FALSE;
	case WM_MOUSEMOVE:
		if (GetCapture() == hWnd) {
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0) {
				if (IsPtOnMe(hWnd, lParam)) {
					ChangeState(hWnd, GB_DOWN);
				} else {
					ChangeState(hWnd, GB_NORMAL);
				}
			} else {
				if (IsPtOnMe(hWnd, lParam)) {
					ChangeState(hWnd, CheckState == GB_NORMAL ? GB_DOWN : GB_NORMAL);
				} else {
					ChangeState(hWnd, CheckState);
				}
			}
		} else {
			for (hParent = hWnd; GetParent(hParent); hParent = GetParent(hParent)) { ; }
			if (GetForegroundWindow() != hParent) return FALSE;
			// 푸시 버튼만 핫 상태가 될 수 있다.
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0) {
				ChangeState(hWnd, GB_HOT);
				SetTimer(hWnd, 0, 50, NULL);
			}
		}
		return FALSE;
	case WM_LBUTTONUP:
		if (GetCapture() == hWnd) {
			ReleaseCapture();
			// 체크 버튼은 MOUSEMOVE의 최종 상태를 그대로 가진다.
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0) {
				ChangeState(hWnd, GB_NORMAL);
			}
			if (IsPtOnMe(hWnd, lParam)) {
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(
					GetWindowLongPtr(hWnd, GWLP_ID), GBN_CLICKED), (LPARAM)hWnd);
			}
		}
		return FALSE;
	case WM_TIMER:
		GetCursorPos(&pt);
		GetWindowRect(hWnd, &crt);
		if (PtInRect(&crt, pt) == FALSE) {
			KillTimer(hWnd, 0);
			ChangeState(hWnd, GB_NORMAL);
		}
		return FALSE;
	case WM_ENABLE:
		if (wParam == TRUE) {
			ChangeState(hWnd, GB_NORMAL);
		} else {
			ChangeState(hWnd, GB_DISABLE);
			KillTimer(hWnd, 0);
		}
		return FALSE;
	case WM_DESTROY:
		DeleteAllBitmap(pData);
		free(pData);
		return FALSE;
		// 팝업 메뉴 메시지를 부모에게 전달한다. 
	case WM_CONTEXTMENU:
		PostMessage(GetParent(hWnd), WM_CONTEXTMENU, wParam, lParam);
		return FALSE;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

VOID ChangeState(HWND hWnd, eState State) {
	GrpBtnData* pData;

	pData = (GrpBtnData*)GetWindowLongPtr(hWnd, 0);
	if (pData->State == State) return;

	pData->State = State;
	InvalidateRect(hWnd, NULL, FALSE);
}

static VOID DeleteAllBitmap(GrpBtnData* pData) {
	INT i;

	for (i = 0; i < 4; i++) {
		if (pData->Bitmap[i]) {
			DeleteObject(pData->Bitmap[i]);
		}
	}
}

static BOOL IsPtOnMe(HWND hWnd, LPARAM lParam) {
	POINT pt;
	RECT wrt;

	pt.x = (INT)(SHORT)LOWORD(lParam);
	pt.y = (INT)(SHORT)HIWORD(lParam);
	ClientToScreen(hWnd, &pt);
	GetWindowRect(hWnd, &wrt);
	return PtInRect(&wrt, pt);
}

// 실행 파일과 같은 위치에 INI 파일의 경로를 초기화한다. 
VOID CSetting::SetIniPath(LPCTSTR filename) {
	TCHAR* p;

	GetModuleFileName(NULL, iniPath, MAX_PATH);

	// 파일명을 전달하지 않았으면 실행 파일명에서 확장자만 ini로 변경한다. 
	if (filename == NULL) {
		p = _tcsrchr(iniPath, '.');
		*(p + 1) = 0;
		lstrcat(iniPath, _T("ini"));
	} else {
		p = _tcsrchr(iniPath, '\\');
		*(p + 1) = 0;
		lstrcat(iniPath, filename);
	}
}

INT CSetting::get(wstring section, LPCTSTR key, INT defValue) {
	if (useIni) {
		return GetPrivateProfileInt(section.c_str(), key, defValue, iniPath);
	} else {
		TCHAR regPath[MAX_PATH];
		HKEY hKey;
		DWORD dwDisp;
		INT Result;
		DWORD cbSize;

		Result = defValue;
		lstrcpy(regPath, regRoot);
		lstrcat(regPath, section.c_str());
		if (RegCreateKeyEx(hive, regPath, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_READ, NULL, &hKey, &dwDisp) == ERROR_SUCCESS) {
			if (dwDisp == REG_OPENED_EXISTING_KEY) {
				cbSize = sizeof(LONG);
				if (RegQueryValueEx(hKey, key, 0, NULL, (LPBYTE)&Result, &cbSize)
					!= ERROR_SUCCESS) {
					Result = defValue;
				}
			}
			RegCloseKey(hKey);
		}
		return Result;
	}
}

// 버퍼 크기는 128 이상 확보해야 한다. 그 이상은 API 함수를 직접 호출할 것
VOID CSetting::get(wstring section, LPCTSTR key, LPCTSTR defValue, LPTSTR retValue, INT nSize/* = 128*/) {
	if (useIni) {
		GetPrivateProfileString(section.c_str(), key, defValue, retValue, nSize, iniPath);
	} else {
		TCHAR regPath[MAX_PATH];
		HKEY hKey;
		DWORD dwDisp;
		DWORD cbSize;
		BOOL Result = FALSE;

		lstrcpy(regPath, regRoot);
		lstrcat(regPath, section.c_str());
		if (RegCreateKeyEx(hive, regPath, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_READ, NULL, &hKey, &dwDisp) == ERROR_SUCCESS) {
			if (dwDisp == REG_OPENED_EXISTING_KEY) {
				cbSize = nSize * sizeof(TCHAR);
				if (RegQueryValueEx(hKey, key, 0, NULL, (LPBYTE)retValue, &cbSize)
					== ERROR_SUCCESS) {
					Result = TRUE;
				}
			}
			RegCloseKey(hKey);
		}
		if (Result == FALSE) lstrcpy(retValue, defValue);
	}
}

// 실수 읽기 - 문자열을 읽은 후 실수로 변환하여 리턴한다. 
DOUBLE CSetting::getDouble(wstring section, LPCTSTR key, DOUBLE defValue) {
	TCHAR buf[128];
	get(section, key, _T(""), buf);
	if (lstrlen(buf) == 0) {
		return defValue;
	} else {
		TCHAR* endptr;
		return _tcstod(buf, &endptr);
	}
}

BOOL CSetting::set(wstring section, LPCTSTR key, INT value) {
	if (useIni) {
		// 정수는 문자열로 바꿔서 기록한다.
		TCHAR buf[64];
		wsprintf(buf, _T("%d"), value);
		WritePrivateProfileString(section.c_str(), key, buf, iniPath);
		return TRUE;
	} else {
		TCHAR regPath[MAX_PATH];
		HKEY hKey;
		DWORD dwDisp;
		BOOL Result = FALSE;

		lstrcpy(regPath, regRoot);
		lstrcat(regPath, section.c_str());
		if (RegCreateKeyEx(hive, regPath, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, &dwDisp) == ERROR_SUCCESS) {
			if (RegSetValueEx(hKey, key, 0, REG_DWORD, (LPBYTE)&value, sizeof(INT))
				== ERROR_SUCCESS) {
				Result = TRUE;
			}
			RegCloseKey(hKey);
		}
		return Result;
	}
}

BOOL CSetting::set(wstring section, LPCTSTR key, LPCTSTR value) {
	if (useIni) {
		WritePrivateProfileString(section.c_str(), key, value, iniPath);
		return TRUE;
	} else {
		TCHAR regPath[MAX_PATH];
		HKEY hKey;
		DWORD dwDisp;
		BOOL Result = FALSE;

		lstrcpy(regPath, regRoot);
		lstrcat(regPath, section.c_str());
		if (RegCreateKeyEx(hive, regPath, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE, NULL, &hKey, &dwDisp) == ERROR_SUCCESS) {
			if (RegSetValueEx(hKey, key, 0, REG_SZ, (LPBYTE)value,
				(lstrlen(value) + 1) * sizeof(TCHAR)) == ERROR_SUCCESS) {
				Result = TRUE;
			}
			RegCloseKey(hKey);
		}
		return Result;
	}
}

BOOL CSetting::setDouble(wstring section, LPCTSTR key, DOUBLE value) {
	TCHAR buf[128];
	_stprintf_s(buf, _T("%f"), value);
	return set(section, key, buf);
}

// 섹션 전체를 삭제한다.
VOID CSetting::deleteSection(wstring section) {
	if (useIni) {
		// 두번째 인수가 NULL이면 section을 모두 삭제한다.
		WritePrivateProfileString(section.c_str(), NULL, NULL, iniPath);
	} else {
		// 레지스트리 버전은 아직 미구현
	}
}

VOID EnsureVisible(HWND hWnd) {
	RECT wrt, irt;
	GetWindowRect(hWnd, &wrt);

	// 일부라도 겹치는 영역이 있는지 조사한다.
	BOOL visible = FALSE;
	for (INT i = 0; i < monitorCount; i++) {
		IntersectRect(&irt, &wrt, &arMonitor[i].rcWork);
		if (IsRectEmpty(&irt) == FALSE) {
			visible = TRUE;
			break;
		}
	}

	// 전혀 없으면 주모니터로 강제 이동시킨다.
	if (visible == FALSE) {
		for (INT i = 0; i < monitorCount; i++) {
			if (arMonitor[i].primary) {
				SetWindowPos(hWnd, NULL, arMonitor[i].rcMonitor.left + 100,
					arMonitor[i].rcMonitor.top + 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				return;
			}
		}
	}
}

// X11, 웹 표준색
sNamedColor arStandardColor[] = {
	{_T("IndianRed"),				0xcd5c5c},
	{_T("DarkSalmon"),				0xe9967a},
	{_T("LightCoral"),				0xf08080},
	{_T("Salmon"),					0xfa8072},
	{_T("OrangeRed"),				0xff4500},
	{_T("Red"),						0xff0000},
	{_T("Crimson"),					0xdc143c},
	{_T("FireBrick"),				0xb22222},
	{_T("DarkRed"),					0x8b0000},
	{_T("MediumVioletRed"),			0xc71585},
	{_T("Pink"),					0xffc0cb},
	{_T("LightPink"),				0xffb6c1},
	{_T("HotPink"),					0xff69b4},
	{_T("DeepPink"),				0xff1493},
	{_T("PaleVioletRed"),			0xdb7093},
	{_T("DarKkhaki"),				0xbdb76b},
	{_T("Khaki"),					0xf0e68c},
	{_T("PaleGoldenRod"),			0xeee8aa},
	{_T("LightGoldenRodYellow"),	0xfafad2},
	{_T("LightYellow"),				0xffffe0},
	{_T("LemonChiffon"),			0xfffacd},
	{_T("Yellow"),					0xffff00},
	{_T("Gold"),					0xffd700},
	{_T("PapayaWhip"),				0xffefd5},
	{_T("Moccasin"),				0xffe4b5},
	{_T("PeachPuff"),				0xffdab9},
	{_T("Cyan"),					0x00ffff},
	{_T("Aqua"),					0x00ffff},
	{_T("AquaMarine"),				0x7fffd4},
	{_T("Turquoise"),				0x40e0d0},
	{_T("MediumTurquoise"),			0x48d1cc},
	{_T("DarkTurquoise"),			0x00ced1},
	{_T("CadetBlue"),				0x5f9ea0},
	{_T("SlateGray"),				0x708090},
	{_T("LightCyan"),				0xe0ffff},
	{_T("PaleTurquoise"),			0xafeeee},
	{_T("PowderBlue"),				0xb0e0e6},
	{_T("LightSteelBlue"),			0xb0c4de},
	{_T("SteelBlue"),				0x4682b4},
	{_T("LightBlue"),				0xadd8e6},
	{_T("SkyBlue"),					0x87ceeb},
	{_T("LightSkyBlue"),			0x87cefa},
	{_T("DeepSkyBlue"),				0x00bfff},
	{_T("CornflowerBlue"),			0x6495ed},
	{_T("RoyalBlue"),				0x4169e1},
	{_T("MediumSlateBlue"),			0x7b68ee},
	{_T("DodgerBlue"),				0x1e90ff},
	{_T("Blue"),					0x0000ff},
	{_T("MediumBlue"),				0x0000cd},
	{_T("DarkBlue"),				0x00008b},
	{_T("Navy"),					0x000080},
	{_T("MidnightBlue"),			0x191970},
	{_T("LightSalmon"),				0xffa07a},
	{_T("Orange"),					0xffa500},
	{_T("DarkOrange"),				0xff8c00},
	{_T("Coral"),					0xff7f50},
	{_T("Tomato"),					0xff6347},
	{_T("MediumGpringGreen"),		0x00fa9a},
	{_T("SpringGreen"),				0x00ff7f},
	{_T("PaleGreen"),				0x98fb98},
	{_T("GreenYellow"),				0xadff2f},
	{_T("ChartReuse"),				0x7fff00},
	{_T("LawnGreen"),				0x7cfc00},
	{_T("Lime"),					0x00ff00},
	{_T("LightGreen"),				0x90ee90},
	{_T("YellowGreen"),				0x9acd32},
	{_T("LimeGreen"),				0x32cd32},
	{_T("MediumSeaGreen"),			0x3cb371},
	{_T("DarkSeaGreen"),			0x8fbc8f},
	{_T("ForestGreen"),				0x228b22},
	{_T("Seagreen"),				0x2e8b57},
	{_T("Green"),					0x008000},
	{_T("OliveDrab"),				0x6b8e23},
	{_T("Olive"),					0x808000},
	{_T("DarkOliveGreen"),			0x556b2f},
	{_T("DarkGreen"),				0x006400},
	{_T("MediumAquaMarine"),		0x66cdaa},
	{_T("Turquoise"),				0x40e0d0},
	{_T("LightSeaGreen"),			0x20b2aa},
	{_T("DarkCyan"),				0x008b8b},
	{_T("Teal"),					0x008080},
	{_T("Lavender"),				0xe6e6fa},
	{_T("Thistle"),					0xd8bfd8},
	{_T("Plum"),					0xdda0dd},
	{_T("Violet"),					0xee82ee},
	{_T("Fuchsia"),					0xff00ff},
	{_T("Magenta"),					0xff00ff},
	{_T("Orchid"),					0xda70d6},
	{_T("MediumOrchid"),			0xba55d3},
	{_T("DarkOrchid"),				0x9932cc},
	{_T("BlueViolet"),				0x8a2be2},
	{_T("DarkViolet"),				0x9400d3},
	{_T("MediumPurple"),			0x9370db},
	{_T("SlateBlue"),				0x6a5acd},
	{_T("Purple"),					0x800080},
	{_T("DarkMagenta"),				0x8b008b},
	{_T("DarkSlateBlue"),			0x483d8b},
	{_T("Indigo"),					0x4b0082},
	{_T("HoneyDew"),				0xf0fff0},
	{_T("MintCream"),				0xf5fffa},
	{_T("Azure"),					0xf0ffff},
	{_T("AliceBlue"),				0xf0f8ff},
	{_T("GhostWhite"),				0xf8f8ff},
	{_T("WhiteSmoke"),				0xf5f5f5},
	{_T("LavenderBlush"),			0xfff0f5},
	{_T("MistyRose"),				0xffe4e1},
	{_T("AntiqueWhite"),			0xfaebd7},
	{_T("SeaShell"),				0xfff5ee},
	{_T("Snow"),					0xfffafa},
	{_T("White"),					0xffffff},
	{_T("Beige"),					0xf5f5dc},
	{_T("Linen"),					0xfaf0e6},
	{_T("OldLace"),					0xfdf5e6},
	{_T("FloralWhite"),				0xfffaf0},
	{_T("Ivory"),					0xfffff0},
	{_T("Gainsboro"),				0xdcdcdc},
	{_T("LightGrey"),				0xd3d3d3},
	{_T("Silver"),					0xc0c0c0},
	{_T("DarkGray"),				0xa9a9a9},
	{_T("Gray"),					0x808080},
	{_T("DimGray"),					0x696969},
	{_T("DarkSlateGray"),			0x2f4f4f},
	{_T("LightSlateGray"),			0x778899},
	{_T("SlateGray"),				0x708090},
	{_T("CornSilk"),				0xfff8dc},
	{_T("BlanchedAlmond"),			0xffebcd},
	{_T("Bisque"),					0xffe4c4},
	{_T("NavajoWhite"),				0xffdead},
	{_T("Wheat"),					0xf5deb3},
	{_T("SandyBrown"),				0xf4a460},
	{_T("GoldenRod"),				0xdaa520},
	{_T("DarkGoldenRod"),			0xb8860b},
	{_T("Peru"),					0xcd853f},
	{_T("Chocolate"),				0xd2691e},
	{_T("Maroon"),					0x800000},
	{_T("SaddleBrown"),				0x8b4513},
	{_T("Brown"),					0xa52a2a},
	{_T("Sienna"),					0xa0522d},
	{_T("Darkred"),					0x8b0000},
	{_T("BurlyWood"),				0xdeb887},
	{_T("Tan"),						0xd2b48c},
	{_T("RosyBrown"),				0xbc8f8f},
	{_T("Black"),					0x000000},
	{_T(""),						0x000000},		// end mark
};

// 이름으로부터 색상을 찾아 준다. 없으면 검정색을 리턴한다.
COLORREF GetColorFromName(LPCTSTR name) {
	for (INT i = 0; ; i++) {
		if (lstrlen(arStandardColor[i].name) == 0) break;
		if (lstrcmpi(arStandardColor[i].name, name) == 0) {
			INT c = arStandardColor[i].color;
			return RGB(GetBValue(c), GetGValue(c), GetRValue(c));
		}
	}

	return RGB(0, 0, 0);
}

// 팝업을 부모와 겹치지 않는 영역으로 자동 이동
// wx, wy는 팝업의 좌상단 좌표이며 이 위치에 맞춘다.
VOID MovePopupToSide(HWND hDlg, INT wx, INT wy) {
	POINT pt;
	RECT wrt, drt;
	INT x, y;
	const INT GAP = 5;

	// 메인 윈도우가 있는 모니터를 알아낸다.
	INT nowMonitor = GetMonitorOfWindow(mainWindow.hBaseWnd);

	GetCursorPos(&pt);
	GetWindowRect(mainWindow.hBaseWnd, &wrt);
	GetWindowRect(hDlg, &drt);
	if (mainWindow.bVertical) {
		// 모니터의 중점보다 마우스가 더 왼쪽에 있으면 대화상자를 오른쪽에 표시한다.
		if ((arMonitor[nowMonitor].rcMonitor.right +
			arMonitor[nowMonitor].rcMonitor.left) / 2 > pt.x) {
			x = wrt.right + GAP;
		}
		else {
			x = wrt.left - (drt.right - drt.left) - GAP;
		}
		y = wy;

		// 대화상자 밑볕이 모니터보다 더 아래쪽이면 위로 올려 준다.
		if (y + (drt.bottom - drt.top) > arMonitor[nowMonitor].rcWork.bottom) {
			y = arMonitor[nowMonitor].rcWork.bottom - (drt.bottom - drt.top);
		}

		// 상단이 모니터를 벗어나면 아래로 내린다.
		if (y < arMonitor[nowMonitor].rcMonitor.top) {
			y = arMonitor[nowMonitor].rcMonitor.top;
		}
	} else {
		// 수평쪽도 논리는 같다.
		if ((arMonitor[nowMonitor].rcMonitor.bottom +
			arMonitor[nowMonitor].rcMonitor.top) / 2 > pt.y) {
			y = wrt.bottom + GAP;
		} else {
			y = wrt.top - (drt.bottom - drt.top) - GAP;
		}
		x = wx;

		if (x + (drt.right - drt.left) > arMonitor[nowMonitor].rcWork.right) {
			x = arMonitor[nowMonitor].rcWork.right - (drt.right - drt.left);
		}

		if (x < arMonitor[nowMonitor].rcMonitor.left) {
			x = arMonitor[nowMonitor].rcMonitor.left;
		}
	}

	// 대화상자의 위치만 옮기며 Z순서는 그대로 유지한다. 아니면 AlwaysOnTop 속성이 바뀐다. 
	SetWindowPos(hDlg, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// 클래스명으로 초기화 정보 배열의 위치를 찾는다. 
INT FindWidgetClsIdx(LPCTSTR cls) {
	for (INT wi = 0; ; wi++) {
		if (arWidgetInfo[wi].cls == _T("")) return -1;
		if (arWidgetInfo[wi].cls == cls) return wi;
	}

	return -1;
}

INT FindWidgetNameIdx(LPCTSTR name) {
	for (INT wi = 0; ; wi++) {
		if (arWidgetInfo[wi].cls == _T("")) return -1;
		if (arWidgetInfo[wi].name == name) return wi;
	}
	
	return -1;
}

INT FindTidgetClsIdx(LPCTSTR cls) {
	for (INT ti = 0; ; ti++) {
		if (arTidgetInfo[ti].cls == _T("")) return -1;
		if (arTidgetInfo[ti].cls == cls) return ti;
	}

	return -1;
}

INT FindTidgetNameIdx(LPCTSTR name) {
	for (INT ti = 0; ; ti++) {
		if (arTidgetInfo[ti].cls == _T("")) return -1;
		if (arTidgetInfo[ti].name == name) return ti;
	}

	return -1;
}

// st 날짜의 절대 초를 구한다. 2000년 1월 1일 자정 기준
DWORD MyGetAbsSecond(SYSTEMTIME st) {
	INT64 i64;
	FILETIME fst;

	SystemTimeToFileTime(&st, &fst);
	i64 = (((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	// 초단위로 환산하고 2000년 1월 1일 자정 기준으로 바꾼다.
	i64 = i64 / 10000000 - (INT64)145731 * 86400;

	return (DWORD)i64;
}

// 절대초를 시스템 타임으로 바꾼다.
VOID MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME& st) {
	INT64 i64;
	FILETIME fst;

	i64 = (Abs + (INT64)145731 * 86400) * 10000000;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
}

// 지정한 영역의 중앙에 종횡비 맞추어 이미지 출력
VOID DrawImageCenter(HDC hdc, Image* pImage, INT width, INT height, BOOL erase, HBRUSH backBrush) {
	Graphics G(hdc);
	INT iw, ih;
	INT l, t, r, b, tsize;

	iw = pImage->GetWidth();
	ih = pImage->GetHeight();
	if (iw < width && ih < height) {
		l = width / 2 - iw / 2;
		t = height / 2 - ih / 2;
		r = l + iw;
		b = t + ih;
	} else {
		// 이미지의 종횡비가 더 크면 : 화면이 세로로 길쭉할 때
		if ((DOUBLE)iw / ih > (DOUBLE)width / height) {
			l = 0;
			r = width;
			// 화면 폭의 비율만큼 그림의 폭도 줄인다.
			tsize = INT(ih * width / (DOUBLE)iw);
			t = height / 2 - tsize / 2;
			b = t + tsize;
		} else {
			t = 0;
			b = height;
			tsize = INT(iw * height / (DOUBLE)ih);
			l = width / 2 - tsize / 2;
			r = l + tsize;
		}
	}

	G.DrawImage(pImage, l, t, r - l, b - t);

	// 이미지 바깥을  배경색으로 채운다. 
	if (erase) {
		FillRect(hdc, 0, 0, width, t, backBrush);
		FillRect(hdc, 0, 0, l, height, backBrush);
		FillRect(hdc, 0, b, width, height, backBrush);
		FillRect(hdc, r, t, width, height, backBrush);
	}
}

// 회전된 이미지를 위쪽 방향으로 돌린다. 2, 4, 5, 6번 상태도 있어 좀 더 면밀한 연구가 필요하다. 
VOID FixImageOrientation(Image* pImage) {
	UINT size, count;
	// 1번이 정방향이며 이 경우는 exif에 방향 정보가 기록되지 않는다. 
	WORD orientation = 1;
	PropertyItem* arPro;
	pImage->GetPropertySize(&size, &count);

	arPro = (PropertyItem*)malloc(size);
	pImage->GetAllPropertyItems(size, count, arPro);
	for (INT i = 0; i < (INT)count; i++) {
		if (arPro[i].id == PropertyTagOrientation) {
			orientation = *(WORD*)arPro[i].value;
		}
	}

	switch (orientation) {
	case 6:
		pImage->RotateFlip(Rotate90FlipNone);
		break;
	case 3:
		pImage->RotateFlip(Rotate180FlipNone);
		break;
	case 8:
		pImage->RotateFlip(Rotate270FlipNone);
		break;
	}
}

// 현재 모니터를 가득 채운다. 
VOID makeFullScreen(HWND hWnd) {
	SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	INT nowMonitor = GetMonitorOfWindow(hWnd);
	RECT rt = arMonitor[nowMonitor].rcMonitor;
	SetWindowPos(hWnd, HWND_NOTOPMOST, rt.left, rt.top,
		rt.right - rt.left, rt.bottom - rt.top, SWP_FRAMECHANGED);
}

VOID CopyClipboardText(LPCTSTR content) {
	HGLOBAL hmem;
	TCHAR* ptr;

	hmem = GlobalAlloc(GHND, (lstrlen(content) + 1) * sizeof(TCHAR));
	ptr = (TCHAR*)GlobalLock(hmem);
	memcpy(ptr, content, (lstrlen(content) + 1) * sizeof(TCHAR));
	GlobalUnlock(hmem);
	if (OpenClipboard(mainWindow.hBaseWnd)) {
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hmem);
		CloseClipboard();
	} else {
		GlobalFree(hmem);
	}
}

VOID TrimTrailZero(TCHAR* num) {
	if (_tcschr(num, '.') != NULL) {
		TCHAR* p;
		for (p = num + lstrlen(num) - 1; *p == '0'; p--) { ; }
		if (*p == '.') {
			*p = 0;
		} else {
			*(p + 1) = 0;
		}
	}
}

// 그래픽 스크롤 바 구현 코드
#define IsHorz (GetWindowLong(hWnd,GWL_STYLE) & GSS_HORZ)
struct GrpScrlData {
	INT Max, Min;
	INT Pos;
	INT TSize;
	INT TGap;
	COLORREF BackColor;
	COLORREF ThumbColor;
};

LRESULT CALLBACK GrpScrlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
VOID GetThumbRect(HWND hWnd, RECT* trt);
VOID PosFromPixel(HWND hWnd, INT Pixel);

class GrpScrlRegister {
public:
	GrpScrlRegister() {
		WNDCLASS WndClass;

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 4;
		WndClass.hbrBackground = NULL;
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = NULL;
		WndClass.hInstance = GetModuleHandle(NULL);
		WndClass.lpfnWndProc = GrpScrlProc;
		WndClass.lpszClassName = _T("GrpScrl");
		WndClass.lpszMenuName = NULL;
		WndClass.style = 0;

		RegisterClass(&WndClass);
	}
} GrpScrlRegisterObject;

LRESULT CALLBACK GrpScrlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc, MemDC;
	HBITMAP MemBit, hOldBmp;
	PAINTSTRUCT ps;
	GrpScrlData* pData;
	RECT crt, trt;
	POINT pt;
	static INT offset;
	static HPEN hPen[5];
	INT i;
	HBRUSH BackBrush, ThumbBrush, OldBrush;

	pData = (GrpScrlData*)(LONG_PTR)GetWindowLong(hWnd, 0);
	switch (iMessage) {
	case WM_CREATE:
		pData = (GrpScrlData*)calloc(sizeof(GrpScrlData), 1);
		SetWindowLong(hWnd, 0, (LONG)(LONG_PTR)pData);
		pData->TSize = 20;
		pData->TGap = 2;
		// hPen 배열이 정적이므로 두 인스턴스 생성시 따로 만들 필요가 없다.
		if (hPen[0] == NULL) {
			for (i = 0; i < 5; i++) {
				hPen[i] = CreatePen(PS_SOLID, 1, RGB(i * 32 + 32, i * 32 + 32, i * 32 + 32));
			}
		}
		pData->BackColor = GetSysColor(COLOR_BTNFACE);
		pData->ThumbColor = RGB(255, 255, 255);
		return FALSE;
	case GSM_SETRANGE:
		pData->Min = static_cast<INT>(wParam);
		pData->Max = static_cast<INT>(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
	case GSM_GETRANGEHI:
		return pData->Max;
	case GSM_GETRANGELOW:
		return pData->Min;
	case GSM_SETPOS:
		pData->Pos = static_cast<INT>(wParam);
		pData->Pos = max(pData->Min, min(pData->Max, pData->Pos));
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
	case GSM_GETPOS:
		return pData->Pos;
	case GSM_SETTSIZE:
		GetClientRect(hWnd, &crt);
		pData->TSize = static_cast<INT>(wParam);
		if (IsHorz) {
			pData->TSize = max(4, min(crt.right - 10, pData->TSize));
		} else {
			pData->TSize = max(4, min(crt.bottom - 10, pData->TSize));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
	case GSM_GETTSIZE:
		return pData->TSize;
	case GSM_SETTGAP:
		GetClientRect(hWnd, &crt);
		pData->TGap = static_cast<INT>(wParam);
		if (IsHorz) {
			pData->TGap = max(0, min(crt.bottom / 2 - 2, pData->TGap));
		} else {
			pData->TGap = max(0, min(crt.right / 2 - 2, pData->TGap));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
	case GSM_GETTGAP:
		return pData->TGap;
	case GSM_SETBACKCOLOR:
		pData->BackColor = (COLORREF)wParam;
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
	case GSM_SETTHUMBCOLOR:
		pData->ThumbColor = (COLORREF)wParam;
		InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;
		// 크기가 바뀌면 썸의 모양도 범위 점검을 다시 해야 한다.
	case WM_SIZE:
		SendMessage(hWnd, GSM_SETTSIZE, pData->TSize, 0);
		SendMessage(hWnd, GSM_SETTGAP, pData->TGap, 0);
		return FALSE;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		MemDC = CreateCompatibleDC(hdc);
		MemBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		hOldBmp = (HBITMAP)SelectObject(MemDC, MemBit);

		BackBrush = CreateSolidBrush(pData->BackColor);
		FillRect(MemDC, &crt, BackBrush);
		DeleteObject(BackBrush);
		if (IsHorz) {
			for (i = 0; i < 5; i++) {
				SelectObject(MemDC, hPen[i]);
				MoveToEx(MemDC, crt.left, crt.bottom / 2 - 2 + i, NULL);
				LineTo(MemDC, crt.right, crt.bottom / 2 - 2 + i);
			}
		} else {
			for (i = 0; i < 5; i++) {
				SelectObject(MemDC, hPen[i]);
				MoveToEx(MemDC, crt.right / 2 - 2 + i, crt.top, NULL);
				LineTo(MemDC, crt.right / 2 - 2 + i, crt.bottom);
			}
		}
		GetThumbRect(hWnd, &trt);
		SelectObject(MemDC, GetStockObject(BLACK_PEN));
		ThumbBrush = CreateSolidBrush(pData->ThumbColor);
		OldBrush = (HBRUSH)SelectObject(MemDC, ThumbBrush);
		RoundRect(MemDC, trt.left, trt.top, trt.right, trt.bottom, 10, 10);
		DeleteObject(SelectObject(MemDC, OldBrush));

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		SelectObject(MemDC, hOldBmp);
		DeleteObject(MemBit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		return FALSE;
	case WM_LBUTTONDOWN:
		GetThumbRect(hWnd, &trt);
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		if (PtInRect(&trt, pt)) {
			if (IsHorz) {
				offset = trt.left - LOWORD(lParam);
			} else {
				offset = trt.top - HIWORD(lParam);
			}
			SetCapture(hWnd);
		} else {
			if (IsHorz) {
				PosFromPixel(hWnd, LOWORD(lParam) - pData->TSize / 2);
			} else {
				PosFromPixel(hWnd, HIWORD(lParam) - pData->TSize / 2);
			}
			SendMessage(GetParent(hWnd), IsHorz ? WM_HSCROLL : WM_VSCROLL,
				MAKEWPARAM(SB_THUMBPOSITION, 0), (LPARAM)hWnd);
		}
		return FALSE;
	case WM_MOUSEMOVE:
		if (GetCapture() == hWnd) {
			GetClientRect(hWnd, &crt);
			if (IsHorz) {
				PosFromPixel(hWnd, (INT)(SHORT)LOWORD(lParam) + offset);
			} else {
				PosFromPixel(hWnd, (INT)(SHORT)HIWORD(lParam) + offset);
			}
			// 위치는 보내지 않음. 32비트이므로 직접 조사할 것
			SendMessage(GetParent(hWnd), IsHorz ? WM_HSCROLL : WM_VSCROLL,
				MAKEWPARAM(SB_THUMBTRACK, 0), (LPARAM)hWnd);
		}
		return FALSE;
	case WM_LBUTTONUP:
		if (GetCapture() == hWnd) {
			ReleaseCapture();
			SendMessage(GetParent(hWnd), IsHorz ? WM_HSCROLL : WM_VSCROLL,
				MAKEWPARAM(SB_THUMBPOSITION, 0), (LPARAM)hWnd);
		}
		return FALSE;
	case WM_DESTROY:
		if (hPen[0]) {
			for (i = 0; i < 5; i++) {
				DeleteObject(hPen[i]);
				hPen[0] = NULL;
			}
		}
		free(pData);
		return FALSE;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

VOID GetThumbRect(HWND hWnd, RECT* trt) {
	GrpScrlData* pData;
	RECT crt;
	INT x, y;

	pData = (GrpScrlData*)(LONG_PTR)GetWindowLong(hWnd, 0);
	GetClientRect(hWnd, &crt);
	if (IsHorz) {
		x = MulDiv(pData->Pos, crt.right - pData->TSize, pData->Max - pData->Min);
		SetRect(trt, x, crt.top + pData->TGap, x + pData->TSize, crt.bottom - pData->TGap);
	} else {
		y = MulDiv(pData->Pos, crt.bottom - pData->TSize, pData->Max - pData->Min);
		SetRect(trt, crt.left + pData->TGap, y, crt.right - pData->TGap, y + pData->TSize);
	}
}

// 화면상의 ScrPos에 해당하는 범위값으로 변경한다.
VOID PosFromPixel(HWND hWnd, INT Pixel) {
	GrpScrlData* pData;
	RECT crt;
	INT width;

	pData = (GrpScrlData*)(LONG_PTR)GetWindowLong(hWnd, 0);
	GetClientRect(hWnd, &crt);
	if (IsHorz) {
		width = crt.right - pData->TSize;
	} else {
		width = crt.bottom - pData->TSize;
	}

	pData->Pos = MulDiv(Pixel, pData->Max - pData->Min, width);
	pData->Pos = max(pData->Min, min(pData->Max, pData->Pos));
	InvalidateRect(hWnd, NULL, TRUE);
}

VOID DrawOwnerItem(LPARAM lParam, sNamedColor arColor[]) {
	HBRUSH bkBrush, Brush, oldBrush;
	LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;

	// 선택 배경 그림
	if (lpdis->itemState & ODS_SELECTED) {
		bkBrush = CreateSolidBrush(RGB(0, 0, 255));
		SetTextColor(lpdis->hDC, RGB(255, 255, 255));
	} else {
		bkBrush = CreateSolidBrush(RGB(255, 255, 255));
		SetTextColor(lpdis->hDC, RGB(0, 0, 0));
	}
	FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);
	DeleteObject(bkBrush);

	// 색상 그림
	Brush = CreateSolidBrush(arColor[lpdis->itemID].color);
	oldBrush = (HBRUSH)SelectObject(lpdis->hDC, Brush);
	Rectangle(lpdis->hDC, lpdis->rcItem.left + 2, lpdis->rcItem.top + 1,
		lpdis->rcItem.left + 20, lpdis->rcItem.bottom - 1);
	SetBkMode(lpdis->hDC, TRANSPARENT);
	TextOutSz(lpdis->hDC, lpdis->rcItem.left + 25, lpdis->rcItem.top + 2,
		arColor[lpdis->itemID].name);
	SelectObject(lpdis->hDC, oldBrush);
	DeleteObject(Brush);
}

// 확장자를 추출하고 소문자로 바꾼 후 포인터를 리턴한다.
LPCTSTR GetFileExtension(LPCTSTR path, TCHAR* ext) {
	lstrcpy(ext, PathFindExtension(path));
	_tcslwr_s(ext, MAX_PATH);
	return ext;
}

// 앱바를 등록한다.
BOOL AppBarRegister(HWND hWnd, UINT uCallBack) {
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
	abd.uCallbackMessage = uCallBack;
	return static_cast<BOOL>(SHAppBarMessage(ABM_NEW, &abd));
}

// 앱바를 해제한다.
BOOL AppBarUnRegister(HWND hWnd) {
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
	return static_cast<BOOL>(SHAppBarMessage(ABM_REMOVE, &abd));
}

// 앱바의 위치를 지정한다. 모니터를 조정했으면 결정한 모니터를 리턴한다.
// uEdge:고정할 위치. nPixel:앱바의 두께, bSetPos:위치 확정 여부
INT AppBarSetPos(HWND hWnd, INT nMonitor, UINT uEdge, UINT nPixel, BOOL move) {
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;

	// 모니터를 전달하지 않았으면 현재 윈도우가 있는 모니터에 표시
	if (nMonitor == -1) {
		nMonitor = GetMonitorOfWindow(hWnd);
	}

	// 없는 모니터이면 주 모니터에 도킹
	if (nMonitor >= monitorCount) {
		nMonitor = GetPrimaryMonitor();
	}

	// 사용 가능한 스크린 영역을 조사한다.
	abd.uEdge = uEdge;
	CopyRect(&abd.rc, &arMonitor[nMonitor].rcMonitor);
	SHAppBarMessage(ABM_QUERYPOS, &abd);
	Trace(_T("QueryPos %d,%d-%d,%d"), abd.rc.left, abd.rc.top, abd.rc.right, abd.rc.bottom);

	// 원하는 영역을 계산하여 설정한다.
	switch (uEdge) {
	case ABE_LEFT:
		abd.rc.right = abd.rc.left + nPixel;
		break;
	case ABE_RIGHT:
		abd.rc.left = abd.rc.right - nPixel;
		break;
	case ABE_TOP:
		abd.rc.bottom = abd.rc.top + nPixel;
		break;
	case ABE_BOTTOM:
		abd.rc.top = abd.rc.bottom - nPixel;
		break;
	}

	// 엡바 영역의 위치를 확정한다. 이때 도킹된 메인 윈도우가 워크 에리어 안쪽으로 잠시 들어온다. 
	// 도킹할 때 TOOLWIDNOW 확장 스타일을 주면 해결된다. 
	SHAppBarMessage(ABM_SETPOS, &abd);

	// 실제로 윈도우를 이동시킨다. 여러번 이동할 필요 없어 ABN_POSCHANGED에서만 실제로 이동한다. 
	if (move) {
		MoveWindow(hWnd, abd.rc.left, abd.rc.top,
			abd.rc.right - abd.rc.left, abd.rc.bottom - abd.rc.top, TRUE);
	}

	return nMonitor;
}
