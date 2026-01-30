#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            FillRect(hdc,&ps.rcPaint,(HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd,&ps);
            break;
        }
    }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int)
{
    // Classe de fenêtre
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "RectanglePedagogique";
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    RegisterClass(&wc);

    // Dimensions initiales
    int w=400, h=300;
    int x=300, y=200;

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,
        wc.lpszClassName,
        "Rectangle pédagogique",
        WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU), // Retire titre + boutons
        x, y, w, h,
        nullptr, nullptr, hInst, nullptr
    );

    // DWM pour enlever toute décoration mais garder le resize natif
    MARGINS margins = {-1}; // étend le client sur toute la fenêtre
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg,nullptr,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
