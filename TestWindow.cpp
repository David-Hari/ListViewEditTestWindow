// TestWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TestWindow.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
CHAR szTitle[MAX_LOADSTRING];
CHAR szWindowClass[MAX_LOADSTRING];
HWND mainWindow;
HWND listView;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    EditBoxProc(HWND, UINT, WPARAM, LPARAM);

WNDPROC originalEditBoxProc;


int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringA(hInstance, IDC_TESTWINDOW, szWindowClass, MAX_LOADSTRING);

    INITCOMMONCONTROLSEX init;
    init.dwSize = sizeof(INITCOMMONCONTROLSEX);
    init.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES | ICC_LISTVIEW_CLASSES |
        ICC_COOL_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS |
        ICC_NATIVEFNTCTL_CLASS | ICC_STANDARD_CLASSES | ICC_LINK_CLASS;
    InitCommonControlsEx(&init);

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEXA);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINDOW));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEA(IDC_TESTWINDOW);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExA(&wcex);

    hInst = hInstance; // Store instance handle in our global variable

    mainWindow = CreateWindowA(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        500, 500, 400, 400, NULL, NULL, hInstance, NULL);
    listView = CreateWindowExA(0, WC_LISTVIEWA, "", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
        0, 0, 400, 400, mainWindow, NULL, hInstance, NULL);

    LVCOLUMN lvColumn = { 0 };
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
    lvColumn.pszText = (LPSTR)"Column";
    lvColumn.cx = 100;
    ListView_InsertColumn(listView, 0, &lvColumn);

    // Add an item to the ListView
    LVITEM lvItem = { 0 };
    lvItem.mask = LVIF_TEXT;
    lvItem.pszText = (LPSTR)"Edit Me";
    lvItem.iItem = 0;
    ListView_InsertItem(listView, &lvItem);

    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (hWnd == mainWindow) {
        switch (message) {
        case WM_NOTIFY:
            if (((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT) {
                NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
                HWND hEdit = ListView_GetEditControl(plvdi->hdr.hwndFrom);
                if (hEdit) {
                    // It works without the following line
                    originalEditBoxProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)EditBoxProc);
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK EditBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return CallWindowProcA(originalEditBoxProc, hWnd, message, wParam, lParam);
}
