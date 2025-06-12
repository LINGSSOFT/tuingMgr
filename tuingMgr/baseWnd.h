#pragma once
class CBaseWnd {					// Base Window Class, management object
public:
	HWND hBaseWnd;					// window handle
	COLORREF BaseBackColor;			// Background Color
	HBRUSH BaseBackBrush;			// Background Brush
	HBITMAP BaseDblBuffer;			// Double Buffering bitmap
	BOOL delegateDispose = FALSE;	// Destroy status

	CBaseWnd();
	~CBaseWnd() { Dispose(); }
	VOID Dispose();

	virtual VOID Create(HWND hParent, DWORD style = WS_CHILD | WS_VISIBLE,
						INT child_id = 0, LPCTSTR title = _T(""));
	VOID SetBackColor(COLORREF color);

	virtual LRESULT OnMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnLButtonDblClk(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnUser1(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnUser2(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam) { return FALSE; }
	virtual VOID OnDraw(HDC hdc, INT width, INT height) { }
	
	static VOID DeleteMap();
	static map<HWND, CBaseWnd*>* pWinMap;	// Map object
	static LRESULT CALLBACK CBaseWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

// Base Widget/Tidget Class, management object 
class CBaseGet : public CBaseWnd {
public:
	wstring baseget_cls;				// widget/tiget member class
	wstring baseget_id;					// widget/tidget name, ussed INI section name
	BOOL baseget_visible;				// hide status with information

	virtual VOID LoadSetting() { ; }	// blank func
	virtual VOID SaveSetting() { ; }	// blank func
};
