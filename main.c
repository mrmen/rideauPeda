#include <SDL.h>
#include <stdbool.h>

#define HANDLE_SIZE 10

typedef enum {
    NONE,
    MOVE,
    RESIZE_NW,
    RESIZE_NE,
    RESIZE_SE,
    RESIZE_SW
} Action;

bool point_in_rect(int x, int y, SDL_Rect *r) {
    return x >= r->x && x <= r->x + r->w &&
           y >= r->y && y <= r->y + r->h;
}

Action detect_action(int mx, int my, SDL_Rect *r) {
    if (abs(mx - r->x) < HANDLE_SIZE && abs(my - r->y) < HANDLE_SIZE)
        return RESIZE_NW;
    if (abs(mx - (r->x + r->w)) < HANDLE_SIZE && abs(my - r->y) < HANDLE_SIZE)
        return RESIZE_NE;
    if (abs(mx - (r->x + r->w)) < HANDLE_SIZE && abs(my - (r->y + r->h)) < HANDLE_SIZE)
        return RESIZE_SE;
    if (abs(mx - r->x) < HANDLE_SIZE && abs(my - (r->y + r->h)) < HANDLE_SIZE)
        return RESIZE_SW;

    if (point_in_rect(mx, my, r))
        return MOVE;

    return NONE;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Rectangle SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect rect = {300, 200, 200, 150};

    bool running = true;
    Action action = NONE;
    int last_x = 0, last_y = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                last_x = e.button.x;
                last_y = e.button.y;
                action = detect_action(last_x, last_y, &rect);
            }

            if (e.type == SDL_MOUSEBUTTONUP) {
                action = NONE;
            }

            if (e.type == SDL_MOUSEMOTION && action != NONE) {
                int dx = e.motion.x - last_x;
                int dy = e.motion.y - last_y;

                if (action == MOVE) {
                    rect.x += dx;
                    rect.y += dy;
                } else {
                    if (action == RESIZE_NW) {
                        rect.x += dx;
                        rect.y += dy;
                        rect.w -= dx;
                        rect.h -= dy;
                    }
                    if (action == RESIZE_NE) {
                        rect.y += dy;
                        rect.w += dx;
                        rect.h -= dy;
                    }
                    if (action == RESIZE_SE) {
                        rect.w += dx;
                        rect.h += dy;
                    }
                    if (action == RESIZE_SW) {
                        rect.x += dx;
                        rect.w -= dx;
                        rect.h += dy;
                    }
                }

                last_x = e.motion.x;
                last_y = e.motion.y;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
