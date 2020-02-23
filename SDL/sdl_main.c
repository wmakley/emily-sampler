#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int main(int argc, const char **argv) {
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const char *imagePath = "hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath);
    if (bmp == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //First clear the renderer
    SDL_RenderClear(renderer);
    //Draw the texture
    SDL_RenderCopy(renderer, tex, NULL, NULL);

    // Draw a red line!
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < WINDOW_WIDTH; ++i) {
        SDL_RenderDrawPoint(renderer, i, i);
    }
    SDL_RenderPresent(renderer);

    //Update the screen
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }
    }

    printf("Success!?\n");

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
