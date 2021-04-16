#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "Ground.h"
#include "Text.h"

SDL_Window* g_window = NULL;
SDL_Renderer* g_screen = NULL;
SDL_Event g_event;
BaseObject g_background;
BaseObject g_menu;
TTF_Font* font_point;

Mix_Chunk* gMusic = NULL;
Mix_Chunk* gJump = NULL;
Mix_Chunk* gDie = NULL;
Mix_Chunk* gNewHighPoint = NULL;

MainObject p_player;
Text pointText;
Ground p_ground;
ImpTimer fps_timer;

bool InitData()
{
    bool success = true;

    int ret = SDL_Init(SDL_INIT_VIDEO);
    if ( ret < 0 ) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
        success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
             success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, 255, 255 ,255 ,255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
        if (TTF_Init() == -1)
            return success;
        else
        {
            font_point = TTF_OpenFont("res//font//joystixmonospace.ttf", 18);
            if (font_point == NULL) success = false;
        }
        if ( SDL_Init(SDL_INIT_AUDIO) < 0 )
            success = false;
        else if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) success = false;
    }

    gMusic = Mix_LoadWAV( "res//sound//theme.wav" );
    if( gMusic == NULL ) success = false;
    gJump = Mix_LoadWAV( "res//sound//jump.wav" );
    if( gJump == NULL ) success = false;
    gDie = Mix_LoadWAV( "res//sound//dead.wav" );
    if( gDie == NULL ) success = false;
    gNewHighPoint = Mix_LoadWAV( "res//sound//newHighPoint.wav" );
    if( gNewHighPoint == NULL ) success = false;

    return success;
}

bool LoadMenuBackGround(std::string path, BaseObject& temp)
{
    bool ret = temp.LoadImg(path, g_screen);
    if ( ret == false) return false;
    return true;
}

void Close()
{
    Mix_FreeChunk( gJump ); gJump = NULL;
    Mix_FreeChunk( gDie ); gDie = NULL;
    Mix_FreeChunk( gMusic ); gMusic = NULL;
    Mix_FreeChunk( gNewHighPoint ); gNewHighPoint = NULL;
    g_background.Free();
    SDL_DestroyRenderer(g_screen); g_screen = NULL;
    SDL_DestroyWindow(g_window); g_window = NULL;
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
}

void LoadText(std::string str_content, int point_val, Text& temp, int xp, int yp)
{
    std::string point = std::to_string(point_val);
    str_content += point;
    temp.SetText(str_content);
    temp.LoadFromRenderText(font_point, g_screen);
    temp.RenderText(g_screen, xp, yp);
}

bool LoadMenu(std::string path, bool isDeadMenu = false, int point_val = NULL)
{
    if (LoadMenuBackGround(path, g_menu) == false) return false;
    Text menuPointText;
    menuPointText.SetColor();
    Text menuBestPointText;
    menuBestPointText.SetColor(255, 0, 0);
    while(true)
    {
        SDL_PollEvent(&g_event);
        if ( g_event.type == SDL_QUIT ) Close();
        if ( g_event.type == SDL_MOUSEBUTTONDOWN && g_event.button.button == SDL_BUTTON_LEFT )
        {
            g_menu.Free();
            menuPointText.Free();
            menuBestPointText.Free();
            break;
        }
        g_menu.Render(g_screen, NULL);

        if (isDeadMenu == true) LoadText("Your point: ", point_val, menuPointText,
                                         (SCREEN_WIDTH/2-menuPointText.getWidth()/2+10), (SCREEN_HEIGHT/2-50));
        if (point_val > SDLCommonFunc::getBestPoint()) //Load best point.
        {
            SDLCommonFunc::fixBestPoint(point_val);
            Mix_PlayChannel( -1, gNewHighPoint, 0 );
            LoadText("Your best point: ", point_val, menuBestPointText,
                      SCREEN_WIDTH/2-menuBestPointText.getWidth()/2+10, SCREEN_HEIGHT/2-10);
        }
        else LoadText("Your best point: ", SDLCommonFunc::getBestPoint(), menuBestPointText,
                      SCREEN_WIDTH/2-menuBestPointText.getWidth()/2+10, SCREEN_HEIGHT/2-10);

        SDL_RenderPresent(g_screen);
        menuPointText.Free();
        menuBestPointText.Free();
    }
    return true;
}

void SetFps(ImpTimer fps_timer)
{
    int real_imp_time = fps_timer.get_ticks();
    int time_one_frame = 1000/FRAME_PER_SECOND;
    if (real_imp_time<time_one_frame)
    {
        int delay_time = time_one_frame - real_imp_time;
        if (delay_time >= 0) SDL_Delay(delay_time);
    }
}

void GameLoop(bool& is_quit)
{
    fps_timer.start();
    while(SDL_PollEvent(&g_event) != 0)
    {
        if (g_event.type == SDL_QUIT)
        {
            is_quit = true;
            return;
        }
        if (p_player.jump(g_event, g_screen) == true)
            Mix_PlayChannel( -1, gJump, 0 );
    }
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderClear(g_screen);

    g_background.Render(g_screen, NULL);

    p_ground.MakeGround(g_screen);
    p_ground.HandleGround(g_screen);

    p_player.Update(p_ground.isGroundBelow(p_player.get_xPos(), p_player.get_widthFrame()));
    p_player.Show(g_screen);

    int point = p_ground.getPoint();
    LoadText("Point: ", point, pointText, 10, 5); //Load Point when playing

    if (p_player.get_Dead()==true)
    {
        Mix_PlayChannel( -1, gDie, 0 );
        pointText.Free();
        p_player.reset();
        p_ground.reset();
        if ( LoadMenu("res//deadmenu.png", true, point) == false )
        {
            is_quit = true;
            return;
        }
        p_ground.MakeStartGround(g_screen);
    }
    else
    {
        SDL_RenderPresent(g_screen);
        pointText.Free();
    }
    SetFps(fps_timer);
}

int main(int argc, char* argv[])
{
    srand(time(0));
    if (InitData() == false) return -1;

    Mix_PlayChannel( -1, gMusic, -1  );
    if ( LoadMenu("res//startmenu.png") == false ) return -1;

    if ( LoadMenuBackGround("res//background.png", g_background ) == false ) return -1;

    p_player.LoadImg("res//players//idle.png",g_screen);
    p_player.set_clips();

    p_ground.MakeStartGround(g_screen);

    pointText.SetColor();

    bool is_quit = false;
    while (!is_quit)
    {
        GameLoop(is_quit);
    }
    Close();
    return 0;
}
