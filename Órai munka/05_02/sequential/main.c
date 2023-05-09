#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Gömb", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 320, 240, 321, 240);
    SDL_RenderDrawLine(renderer, 320, 241, 321, 241);
    SDL_RenderDrawLine(renderer, 320, 239, 321, 239);
    SDL_RenderDrawLine(renderer, 319, 240, 319, 241);
    SDL_RenderDrawLine(renderer, 322, 240, 322, 241);

    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}