#include "framework.h"

map<HWND, CBaseWnd*>* CBaseWnd::pWinMap;		// static member declaration
CBaseWnd::CBaseWnd() {
	SetBackColor(RGB(0, 0, 0));
	BaseDblBuffer = nullptr;

	if (pWinMap == nullptr) {
		pWinMap = new map<HWND, CBaseWnd*>();
		WNDCLASS WndClass;

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = NULL;
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_TUINGMGR));
		WndClass.hInstance = GetModuleHandle(NULL);
		WndClass.lpfnWndProc = (WNDPROC)CBaseWnd::CBaseWndProc;
		WndClass.lpszClassName = _T("CBaseWnd");
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		RegisterClass(&WndClass);
	}
}

VOID CBaseWnd::Dispose() {
	if (BaseBackBrush != nullptr) DeleteObject(BaseBackBrush);
	if (BaseDblBuffer != nullptr) DeleteObject(BaseDblBuffer);

	if (pWinMap != nullptr)
		pWinMap->erase(hBaseWnd);
}

VOID CBaseWnd::Create(HWND hParent, DWORD style, INT child_id, LPCTSTR title) {
	CreateWindow(_T("CBaseWnd"), title, style,
		0, 0, 0, 0, hParent, (HMENU)(ULONG_PTR)child_id, g_hInst, this);
}

VOID CBaseWnd::SetBackColor(COLORREF color) {
	BaseBackColor = color;
	if (BaseBackBrush != nullptr) DeleteObject(BaseBackBrush);
	BaseBackBrush = CreateSolidBrush(color);
}

VOID CBaseWnd::DeleteMap() {
	delete pWinMap;
	pWinMap = nullptr;
}

LRESULT CBaseWnd::OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_CREATE:
		return OnCreate(wParam, lParam);
	case WM_DESTROY:
		return OnDestroy(wParam, lParam);
	case WM_SIZE:
		return OnSize(wParam, lParam);
	case WM_PAINT:
		return OnPaint(wParam, lParam);
	case WM_TIMER:
		return OnTimer(wParam, lParam);
	case WM_COMMAND:
		return OnCommand(wParam, lParam);
	case WM_CONTEXTMENU:
		return OnContextMenu(wParam, lParam);
	case WM_KEYDOWN:
		return OnKeyDown(wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(wParam, lParam);
	case WM_MOUSEMOVE:
		return OnMouseMove(wParam, lParam);
	case WM_LBUTTONUP:
		return OnLButtonUp(wParam, lParam);
	case WM_LBUTTONDBLCLK:
		return OnLButtonDblClk(wParam, lParam);
	case WM_MOUSEWHEEL:
		return OnMouseWheel(wParam, lParam);
	case WM_USER + 1:
		return OnUser1(wParam, lParam);
	case WM_USER + 2:
		return OnUser2(wParam, lParam);
	case WM_DROPFILES:
		return OnDropFiles(wParam, lParam);
	}

	LRESULT ret = DefWindowProc(hBaseWnd, iMessage, wParam, lParam);
	if (delegateDispose && iMessage == WM_NCDESTROY) delete this;

	return ret;
}

LRESULT CBaseWnd::OnPaint(WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HDC hMemDC;
	HBITMAP oldBit;
	RECT crt;

	GetClientRect(hBaseWnd, &crt);
	hdc = BeginPaint(hBaseWnd, &ps);

	if (BaseDblBuffer == nullptr) {
		BaseDblBuffer = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}

	hMemDC = CreateCompatibleDC(hdc);
	oldBit = (HBITMAP)SelectObject(hMemDC, BaseDblBuffer);
	FillRect(hMemDC, &crt, BaseBackBrush);
	OnDraw(hMemDC, crt.right, crt.bottom);
	SelectObject(hMemDC, oldBit);
	DeleteDC(hMemDC);
	DrawBitmap(hdc, 0, 0, BaseDblBuffer);
	EndPaint(hBaseWnd, &ps);

	return FALSE;
}

LRESULT CBaseWnd::OnSize(WPARAM wParam, LPARAM lParam) {
	if (BaseDblBuffer != nullptr) {
		DeleteObject(BaseDblBuffer);
		BaseDblBuffer = nullptr;
	}

	return FALSE;
}

LRESULT CALLBACK CBaseWnd::CBaseWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	CBaseWnd* pWin = nullptr;

	if (pWinMap->find(hWnd) != pWinMap->end()) pWin = (*pWinMap)[hWnd];

	if (pWin == nullptr) {
		if (iMessage == WM_GETMINMAXINFO) {
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
		}
		pWin = (CBaseWnd*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		(*pWinMap)[hWnd] = pWin;
		pWin->hBaseWnd = hWnd;
		Trace(_T("%d Regist Window. Map size = %d"), hWnd, pWinMap->size());
	}

	return pWin->OnMessage(iMessage, wParam, lParam);
}

