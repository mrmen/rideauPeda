#include <windows.h>
#include <windowsx.h>

#define BORDER 30
#define TOP_RESIZE 30

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // 🔥 SUPPRESSION TOTALE DU NON-CLIENT AREA
        case WM_NCCALCSIZE:
            if (wParam)
                return 0;

        case WM_NCHITTEST:
        {
            POINT pt = {
                GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam)
            };
            ScreenToClient(hwnd, &pt);

            RECT rc;
            GetClientRect(hwnd, &rc);

            // Coins
            if (pt.y < BORDER && pt.x < BORDER) return HTTOPLEFT;
            if (pt.y < BORDER && pt.x > rc.right - BORDER) return HTTOPRIGHT;
            if (pt.y > rc.bottom - BORDER && pt.x < BORDER) return HTBOTTOMLEFT;
            if (pt.y > rc.bottom - BORDER && pt.x > rc.right - BORDER) return HTBOTTOMRIGHT;

            // Bord haut
            if (pt.y < TOP_RESIZE) return HTTOP;

            // Déplacement
            return HTCAPTION;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PureBlackRect";
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "",
        WS_POPUP | WS_THICKFRAME, // resize conservé
        300, 200, 420, 300,
        nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
