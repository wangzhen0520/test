#if 0
#include "test_others.h"

#include <iostream>
#include "SDL2/SDL.h"

#include "action.h"

using namespace std;

void test_sdl()
{
#define test
#ifdef test
    return;
#else
    cout << __FILE__ << ":" << __func__ << endl;
#endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Window *window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "window creation failed" << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if (!surface) {
        cout << "surface creation failed" << SDL_GetError() << std::endl;
        return;
    }
    SDL_Rect rec;
    rec.x = 700;
    rec.y = 10;
    rec.w = 20;
    rec.h = 20;

    while (1) {
        SDL_FillRect(surface, &rec, SDL_MapRGB(surface->format, 180, 10, 140));

        rec.x += 6;
        rec.y += 2;
        rec.x = rec.x > 800 ? 0 : rec.x;
        rec.y = rec.y > 600 ? 0 : rec.y;

        SDL_FillRect(surface, &rec, SDL_MapRGB(surface->format, 10, 200, 120));

        SDL_UpdateWindowSurface(window);
        SDL_Delay((1.0 / 60) * 1000);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

LTC_REGISTER_ACTION(ACTION_OTHERS, test_sdl);
#endif