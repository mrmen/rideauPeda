#include <windows.h>
#include <windowsx.h>  // Pour GET_X_LPARAM / GET_Y_LPARAM

#define BORDER 8
#define TOP_RESIZE 10
#define MIN_HEIGHT 120

// Fonction WndProc
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_NCHITTEST:
        {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hwnd, &pt);

            RECT rc;
            GetClientRect(hwnd, &rc);

            // Coins
            if (pt.y < BORDER && pt.x < BORDER) return HTTOPLEFT;
            if (pt.y < BORDER && pt.x > rc.right - BORDER) return HTTOPRIGHT;
            if (pt.y > rc.bottom - BORDER && pt.x < BORDER) return HTBOTTOMLEFT;
            if (pt.y > rc.bottom - BORDER && pt.x > rc.right - BORDER) return HTBOTTOMRIGHT;

            // Bord du haut
            if (pt.y < TOP_RESIZE) return HTTOP;

            // Drag central
            return HTCAPTION;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1)
                PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BorderlessWindow";
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "Rectangle Fenetre",
        WS_POPUP | WS_THICKFRAME,
        300, 200, 420, 300,
        nullptr, nullptr, hInstance, nullptr
    );

    // ---- MENU ----
    HMENU menu = CreateMenu();
    HMENU fileMenu = CreatePopupMenu();
    AppendMenu(fileMenu, MF_STRING, 1, "Quitter");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)fileMenu, "Fichier");
    SetMenu(hwnd, menu);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
