#include <windows.h>
#include <windowsx.h>

#define BORDER 30

RECT rect = {300, 200, 700, 500};
POINT last;
bool dragging = false;

enum Mode { NONE, MOVE, RESIZE_TL, RESIZE_TR, RESIZE_BL, RESIZE_BR };
Mode mode = NONE;

bool InCornerTL(int x, int y){ return abs(x-rect.left)<BORDER && abs(y-rect.top)<BORDER; }
bool InCornerTR(int x, int y){ return abs(x-rect.right)<BORDER && abs(y-rect.top)<BORDER; }
bool InCornerBL(int x, int y){ return abs(x-rect.left)<BORDER && abs(y-rect.bottom)<BORDER; }
bool InCornerBR(int x, int y){ return abs(x-rect.right)<BORDER && abs(y-rect.bottom)<BORDER; }
bool InRect(int x, int y){ return x>rect.left && x<rect.right && y>rect.top && y<rect.bottom; }

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
            last = {x,y};

            if (InCornerTL(x,y)) mode = RESIZE_TL;
            else if (InCornerTR(x,y)) mode = RESIZE_TR;
            else if (InCornerBL(x,y)) mode = RESIZE_BL;
            else if (InCornerBR(x,y)) mode = RESIZE_BR;
            else if (InRect(x,y)) mode = MOVE;
            else mode = NONE;

            if(mode!=NONE){ dragging=true; SetCapture(hwnd); }
            break;
        }

        case WM_MOUSEMOVE:
            if(dragging)
            {
                int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
                int dx = x-last.x, dy = y-last.y;
                switch(mode)
                {
                    case MOVE: OffsetRect(&rect, dx, dy); break;
                    case RESIZE_TL: rect.left+=dx; rect.top+=dy; break;
                    case RESIZE_TR: rect.right+=dx; rect.top+=dy; break;
                    case RESIZE_BL: rect.left+=dx; rect.bottom+=dy; break;
                    case RESIZE_BR: rect.right+=dx; rect.bottom+=dy; break;
                    default: break;
                }
                last={x,y};
                InvalidateRect(hwnd,NULL,TRUE);
            }
            break;

        case WM_LBUTTONUP:
            dragging=false; mode=NONE;
            ReleaseCapture();
            break;

        case WM_KEYDOWN:
            if(wParam==VK_ESCAPE) PostQuitMessage(0);
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc=BeginPaint(hwnd,&ps);
            FillRect(hdc,&ps.rcPaint,(HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd,&ps);
            break;
        }

        case WM_DESTROY: PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int)
{
    WNDCLASS wc={};
    wc.lpfnWndProc=WndProc;
    wc.hInstance=hInst;
    wc.lpszClassName="RectanglePedagogique";
    wc.hbrBackground=CreateSolidBrush(RGB(0,0,0));

    RegisterClass(&wc);

    HWND hwnd=CreateWindowEx(
        0,                  // Pas de WS_THICKFRAME
        wc.lpszClassName,
        "Rectangle pédagogique",
        WS_POPUP,           // <- juste WS_POPUP pour fenêtre totalement noire
        rect.left, rect.top,
        rect.right-rect.left,
        rect.bottom-rect.top,
        nullptr,nullptr,hInst,nullptr
    );

    ShowWindow(hwnd,SW_SHOW);

    MSG msg;
    while(GetMessage(&msg,nullptr,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
