
#include "framework.h"
#include "ThucHanh1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HMENU hmenupopup;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THUCHANH1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THUCHANH1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THUCHANH1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);  // Thêm menu
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   hmenupopup = LoadMenu(hInstance, MAKEINTRESOURCE( IDR_MENU2));   // Tạo menupopup
   HWND hWnd = CreateWindowW(szWindowClass, L"Thu Hien Vu <3", WS_OVERLAPPEDWINDOW,     // Thay đổi tiêu đề
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int hinh; // Lưu Id của hình cần vẽ
    static int xLeft, yTop, xRight, yBottom;
    static HDC hdc;
    static HPEN hpen = CreatePen(PS_SOLID, 3, RGB(255, 153, 153));
    // Để tạo menupopup
    static HMENU menu_netve;    // MenuPopup
    POINT point;
    // Để tạo timer
    static int time=0, gio, phut, giay; // Tổng thời gian, đơn vị: s
    static TCHAR hienthi[20];
    static int width, height;
    switch (message)
    {
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        break;
    case WM_CREATE:
        // Tạo timer
        SetTimer(hWnd, 1 /*Id*/, 1000 /*Chu kì (ms)*/, NULL /* Hàm xử lý khi tick */);
        break;
    case WM_TIMER:  // Được gọi mỗi chu kì khi tick
        gio = time / 3600;
        phut = (time % 3600) / 60;
        giay = (time % 3600) % 60;
        time += 1;
        // Tạo chuỗi để hiển thị
        wsprintf(hienthi, L"Time: %d:%d:%d", gio, phut, giay);    // Tạo chuỗi hienthi theo định dạng
        // Vẽ chuỗi ra màn hình bằng TextOut
        hdc = GetDC(hWnd);
        // Vẽ vào góc phải dưới nên phải lấy ra kích thước cửa sổ trong WM_SIZE
        TextOut(hdc, width-120, height-30, hienthi, 20);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_RBUTTONDOWN:    // Lấy tọa độ và hiển thị menupopup
        menu_netve = GetSubMenu(hmenupopup, 0); // Lấy 1 phần của 1 menu theo chỉ số
        // Lưu tạm tọa độ hiện tại vào point
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        ClientToScreen(hWnd, &point);   // kích hoạt vị trí point
        TrackPopupMenu(menu_netve, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);   // Hiển thị menupopup lên
        break;
    case WM_LBUTTONDOWN:    // Lưu lại tọa độ điểm đầu
        xLeft = LOWORD(lParam);
        yTop = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:      // Lưu tọa độ điểm cuối và vẽ
        // Lấy tọa độ điểm cuối
        xRight = LOWORD(lParam);
        yBottom = HIWORD(lParam);
        // Vẽ
        hdc = GetDC(hWnd);
        SelectObject(hdc, hpen);
        if (hinh == ID_TGV) // Vẽ tam giác vuông 32773
        {
            MoveToEx(hdc, xLeft, yTop, NULL);
            LineTo(hdc, xLeft, yBottom);
            MoveToEx(hdc, xLeft, yBottom, NULL);
            LineTo(hdc, xRight, yBottom);
            MoveToEx(hdc, xRight, yBottom, NULL);
            LineTo(hdc, xLeft, yTop);
        }
        else
        {
            MoveToEx(hdc, xLeft, yTop, NULL);
            LineTo(hdc, xRight, yBottom);
        }
        ReleaseDC(hWnd, hdc);
        break;
    case WM_CLOSE:  // Đóng chương trình
        if (MessageBox(NULL, L"Bạn muốn thoát không?", L"Thông báo", MB_YESNO | MB_ICONQUESTION) == IDYES)
            PostQuitMessage(1);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_Do:
                hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                break;
            case ID_Blue:
                hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
                break;
            case ID_Green:
                hpen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
                break;
            case ID_TGV:
            case ID_DT:
                hinh = wmId;
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        DeleteObject(hpen);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
