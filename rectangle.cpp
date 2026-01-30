#include <windows.h>
#include <windowsx.h>

#define BORDER 30
#define TOP_RESIZE 30

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_NCCALCSIZE:
            if (wParam) return 0;

        case WM_NCHITTEST:
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hwnd, &pt);

            RECT rc;
            GetClientRect(hwnd, &rc);

            if (pt.y < BORDER && pt.x < BORDER) return HTTOPLEFT;
            if (pt.y < BORDER && pt.x > rc.right - BORDER) return HTTOPRIGHT;
            if (pt.y > rc.bottom - BORDER && pt.x < BORDER) return HTBOTTOMLEFT;
            if (pt.y > rc.bottom - BORDER && pt.x > rc.right - BORDER) return HTBOTTOMRIGHT;

            if (pt.y < TOP_RESIZE) return HTTOP;
            return HTCAPTION;
        }

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
{
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"RectanglePedagogique";
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"Rectangle pédagogique", // <- Wide string avec accents
        WS_POPUP | WS_THICKFRAME,
        300, 200, 420, 300,
        nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
