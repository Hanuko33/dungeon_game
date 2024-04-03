#ifndef WINDOW_H
#define WINDOW_H

#ifndef STUB_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400
#define WINDOW_INFO_SIZE 200

extern SDL_Renderer *renderer;
extern SDL_Window *main_window;

extern int window_width;
extern int window_height;

void draw_rectangle(int x,int y,int w,int h, SDL_Color c0, SDL_Color c1, SDL_Color c2, SDL_Color c3);

SDL_Texture* load_texture(const char * texture_name);

int init_sdl();
void clear_window();
unsigned int color(int r, int g, int b, int a);
#define GAME_WINDOW 512
#define PANEL_WINDOW 512
#define STATUS_LINE 32
#endif // WINDOW_H
