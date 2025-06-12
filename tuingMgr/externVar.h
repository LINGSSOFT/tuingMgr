#pragma once
extern HINSTANCE g_hInst;
extern CMainWnd mainWindow;
extern CBaseTray trayWindow;
extern HWND hOptionPopup;
extern SMonitorInfo arMonitor[10];
extern INT monitorCount;
extern HFONT mainFont[6];
extern eLang lang;
extern STuinggetInfo arWidgetInfo[];
extern vector<CTuingGet*> arWidget;
extern STuingjetInfo arTidgetInfo[];
extern vector<CTuingJet*> arTidget;
extern sNamedColor arStandardColor[];
extern CSetting setting;
