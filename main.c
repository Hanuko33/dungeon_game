#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "window.h"
#include "text.h"

struct
{
    int going_right;
    int x;
    int y;
    int sel_x;
    int sel_y;
    int sel;
    int hp;
    int money;
    int ammo;
#define P_SIZE 25
} p;

enum textures
{
    TXT_PLAYER_R,
    TXT_PLAYER_L,
    TXT_SELECT,
    TXT_ZOMBIE,
    TXT_SHOP,
    // MUST BE AT THE END
    TXT_MAX
};

SDL_Texture * textures[TXT_MAX];

void load_textures()
{
    textures[TXT_PLAYER_R] = load_texture("playerr.png");
    textures[TXT_PLAYER_L] = load_texture("playerl.png");
    textures[TXT_SELECT] = load_texture("select.png");
    textures[TXT_ZOMBIE] = load_texture("zombie.png");
    textures[TXT_SHOP] = load_texture("shop.png");
}
struct zombie
{
    int x;
    int y;
    int counter;
};

struct zombie*zombies[16];

void update_zombie(struct zombie *z)
{
    if (z->x == p.x && z->y == p.y)
        z->counter=0;
    if (z->counter < 15)
        z->counter++;
    else
    {
        if (z->x != p.x)
        {
            if (z->x > p.x)
                z->x-=25;
            else
                z->x+=25;
        }
        if (z->y != p.y)
        {
            if (z->y > p.y)
                z->y-=25;
            else
                z->y+=25;
        }
        z->counter=0;
    }

    SDL_Rect img_rect = {z->x, z->y, P_SIZE, P_SIZE};
    SDL_RenderCopy(renderer, textures[TXT_ZOMBIE], NULL, &img_rect);
}

void update_zombies()
{
    for (int i = 0; i<16; i++)
    {
        if (zombies[i] != NULL)
        {
            update_zombie(zombies[i]);
        }
        else
        {
            int random = rand() % 1000;
            if (random == 50)
            {
                zombies[i] = calloc(1, sizeof(struct zombie));
                zombies[i]->y = (rand() % 10) * 25;
            }
       }
    }
}

void draw_bg()
{
    draw_rectangle(
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            (SDL_Color){0,0,0,0},
            (SDL_Color){0,0,0,0},
            (SDL_Color){0,0,255,100},
            (SDL_Color){0,0,255,100}
            );
    draw_rectangle(
            WINDOW_WIDTH,
            0,
            WINDOW_INFO_SIZE,
            WINDOW_HEIGHT,
            (SDL_Color){0,255,0,100},
            (SDL_Color){0,0,0,0},
            (SDL_Color){0,255,0,100},
            (SDL_Color){0,0,0,0}
            );
}
void draw()
{
    SDL_Rect img_rect2 = {600, 300, P_SIZE, P_SIZE};
    SDL_RenderCopy(renderer, textures[TXT_SHOP], NULL, &img_rect2);
    
    SDL_Rect img_rect = {p.x, p.y, P_SIZE, P_SIZE};
    SDL_RenderCopy(renderer, p.going_right ? textures[TXT_PLAYER_R] : textures[TXT_PLAYER_L], NULL, &img_rect);
    char text[128];
    sprintf(text, "Select mode: %s", p.sel ? "yes" : "no");
    write_text(WINDOW_WIDTH, 0, text, (SDL_Color){255,255,255,255}, 12, 24);

    sprintf(text, "Hp: %d", p.hp);
    write_text(WINDOW_WIDTH, 24, text, (SDL_Color){255,255,255,255}, 12, 24);
    
    sprintf(text, "Money: %d", p.money);
    write_text(WINDOW_WIDTH, 24*2, text, (SDL_Color){255,255,255,255}, 12, 24);
    
    sprintf(text, "Ammo: %d", p.ammo);
    write_text(WINDOW_WIDTH, 24*3, text, (SDL_Color){255,255,255,255}, 12, 24);
    if (p.sel)
    {
        SDL_Rect img_rect = {p.sel_x, p.sel_y, P_SIZE, P_SIZE};
        SDL_RenderCopy(renderer, textures[TXT_SELECT], NULL, &img_rect);
    }
}
void handle_key(int key)
{
    switch (key)
    {
        case SDLK_LEFT:
            if (p.sel_x>0)
                p.sel_x-=25;
            break;
        case SDLK_RIGHT:
            if (p.sel_x<WINDOW_WIDTH-P_SIZE)
                p.sel_x+=25;
            break;
        case SDLK_UP:
            if (p.sel_y>0)
                p.sel_y-=25;
            break;
        case SDLK_DOWN:
            if (p.sel_y<WINDOW_HEIGHT-P_SIZE)
                p.sel_y+=25;
            break;
        case SDLK_a:
            if (p.x>0) 
            {
                p.going_right=0;
                p.x-=25;
            }
            break;
        case SDLK_d:
            if (p.x<WINDOW_WIDTH-P_SIZE)
            {
                p.going_right=1;
                p.x+=25;
            }
            break;
        case SDLK_w:
            if (p.y>0)
                p.y-=25;
            break;
        case SDLK_s:
            if (p.y<WINDOW_HEIGHT-P_SIZE)
                p.y+=25;
            break;
        case SDLK_ESCAPE:
            SDL_Quit();
            exit(0);
            break;
        case SDLK_RETURN:
            if (p.sel && p.ammo)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (zombies[i] != NULL)
                    {
                        if (zombies[i]->x == p.sel_x && zombies[i]->y == p.sel_y)
                        {
                            free(zombies[i]);
                            zombies[i] = NULL;
                            p.hp+=100;
                            p.money+=1;
                        }  
                    } 
                }
                p.ammo--;
                if (p.sel_x == p.x && p.sel_y == p.y)
                {
                    p.hp-=99;
                }
            }
            p.sel^=1;
            break;
        case SDLK_SPACE:
            if (p.x == 600 && p.y == 300)
            {
                if (p.money)
                {
                    p.ammo+=4;
                    p.money--;
                }
            }
    }
}
void game_logic()
{
    for (int i = 0; i < 16; i++)
    {
        if (zombies[i] != NULL)
        {
            if (zombies[i]->x == p.x && zombies[i]->y == p.y)
            {
                p.hp--;
            }
        }
    }
    if (p.hp<1)
    {
        SDL_Quit();
        printf("You lost!\n");
        exit(0);
    }
}
int main()
{
    srand(time(NULL));
    p.ammo=5;
    p.hp=100;
    p.x=250;
    for (int i = 0; i < 16; i++)
    {
        zombies[i] = NULL;
    }

    zombies[0] = calloc(1, sizeof(struct zombie));

    SDL_Event event;
    init_sdl();
    if (load_font())
        printf("FAILED TO LOAD FONT!!!\n");
    load_textures();
    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {
                SDL_Quit(); 
                return 0;
            }
            if (event.type==SDL_KEYDOWN)
                handle_key(event.key.keysym.sym);
        }
        if (!p.sel)
        {
            p.sel_x = p.x;
            p.sel_y = p.y;
        }
        clear_window();
        draw_bg();
        update_zombies();
        draw();
        game_logic();
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
}
        
