#include "apprunner.h"

Runner::Runner()
{
}

Runner::~Runner()
{
}

int Runner::Run(HINSTANCE hInstance, int nCmdShow)
{
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = sizeof(MainWindow);
   wcex.cbWndExtra = sizeof(MainWindow);
   wcex.hInstance = g_hInst;
   wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = NULL;
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

   if (!RegisterClassEx(&wcex))
   {
      MessageBox(NULL,
         _T("Call to RegisterClassEx failed!"),
         _T("Window Quote Pro"),
         NULL);

      return 1;
   }

   hWnd = CreateWindowEx(
      WS_EX_OVERLAPPEDWINDOW,
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      1000, 500,
      NULL,
      NULL,
      g_hInst,
      NULL
   );

   if (!hWnd)
   {
      MessageBox(NULL,
         _T("Call to CreateWindow failed!"),
         _T("Window Quote Pro"),
         NULL);

      return 1;
   }

   // The parameters to ShowWindow explained:
   // hWnd: the value returned from CreateWindow
   // nCmdShow: the fourth parameter from WinMain
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   // Main message loop:
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return (int)msg.wParam;
}

LRESULT CALLBACK Runner::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   if (WM_NCCREATE == message) {
      MainWindow* pMain = new MainWindow();
      SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pMain);
   }

   if (!hWnd) return FALSE;
   MainWindow* pWin = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
   if (pWin) return pWin->WndProcImpl(hWnd, message, wParam, lParam);
   return FALSE;
}
