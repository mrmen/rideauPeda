#include <windows.h>
#include <windowsx.h>

#define CORNER 30

RECT hole = { 400, 300, 800, 600 };
POINT last;
bool dragging = false;

enum Mode { NONE, MOVE, RESIZE_TL, RESIZE_TR, RESIZE_BL, RESIZE_BR };
Mode mode = NONE;

// -------------------------------------------------

void UpdateRegion(HWND hwnd)
{
    RECT r;
    GetClientRect(hwnd, &r);

    HRGN full = CreateRectRgn(0, 0, r.right, r.bottom);
    HRGN cut  = CreateRectRgn(hole.left, hole.top, hole.right, hole.bottom);

    CombineRgn(full, full, cut, RGN_DIFF);
    SetWindowRgn(hwnd, full, TRUE);

    DeleteObject(cut);
}

bool InHole(int x, int y)
{
    return x > hole.left && x < hole.right &&
           y > hole.top  && y < hole.bottom;
}

bool InCornerTL(int x, int y)
{
    return abs(x - hole.left) < CORNER &&
           abs(y - hole.top)  < CORNER;
}

bool InCornerTR(int x, int y)
{
    return abs(x - hole.right) < CORNER &&
           abs(y - hole.top)   < CORNER;
}

bool InCornerBL(int x, int y)
{
    return abs(x - hole.left)   < CORNER &&
           abs(y - hole.bottom) < CORNER;
}

bool InCornerBR(int x, int y)
{
    return abs(x - hole.right)  < CORNER &&
           abs(y - hole.bottom) < CORNER;
}

// -------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            last = { x, y };

            if (InCornerTL(x, y))      mode = RESIZE_TL;
            else if (InCornerTR(x, y)) mode = RESIZE_TR;
            else if (InCornerBL(x, y)) mode = RESIZE_BL;
            else if (InCornerBR(x, y)) mode = RESIZE_BR;
            else if ((GetKeyState(VK_CONTROL) & 0x8000) && InHole(x, y))
                mode = MOVE;
            else
                mode = NONE;

            if (mode != NONE)
            {
                dragging = true;
                SetCapture(hwnd);
            }
            break;
        }

        case WM_MOUSEMOVE:
            if (dragging)
            {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                int dx = x - last.x;
                int dy = y - last.y;

                switch (mode)
                {
                    case MOVE:
                        OffsetRect(&hole, dx, dy);
                        break;

                    case RESIZE_TL:
                        hole.left += dx;
                        hole.top  += dy;
                        break;

                    case RESIZE_TR:
                        hole.right += dx;
                        hole.top   += dy;
                        break;

                    case RESIZE_BL:
                        hole.left   += dx;
                        hole.bottom += dy;
                        break;

                    case RESIZE_BR:
                        hole.right  += dx;
                        hole.bottom += dy;
                        break;

                    default: break;
                }

                last = { x, y };
                UpdateRegion(hwnd);
            }
            break;

        case WM_LBUTTONUP:
            dragging = false;
            mode = NONE;
            ReleaseCapture();
            break;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                PostQuitMessage(0);
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// -------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    RECT work;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &work, 0);

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "SpotlightOverlay";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,
        wc.lpszClassName,
        "Spotlight",
        WS_POPUP,
        work.left,
        work.top,
        work.right  - work.left,
        work.bottom - work.top,
        nullptr, nullptr, hInst, nullptr
    );

    ShowWindow(hwnd, SW_SHOW);
    UpdateRegion(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
