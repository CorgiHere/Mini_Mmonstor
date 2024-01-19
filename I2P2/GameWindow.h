#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED+

#include <vector>
#include <list>
#include <time.h>
#include "global.h"
#include "damage.h"
#include <string>

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7


enum{
    EGG, EGG1, EGG2, EGG3
};
enum{
    DECIDE, FIGHT, HEALING, CAPSULE, RUN, CHECK_BATTLE_RESULT
};

enum{
    MOVE, STAND
};
enum{
    LEFT,RIGHT
};
// clock rate
const float FPS = 60;

class GameWindow
{
public:
    // constructor
    GameWindow();

    // each process of scene
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();

    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw_running_map();

    void skill_click(int skill_num);
    // process of updated event
    int process_event();

    int mouse_in(int x1, int y1, int x2, int y2);
    int mouse_in_circle(int x, int y, int r);
    void keyboard_press(ALLEGRO_EVENT event);
    void ch_draw();
    void typewriter(string currentText);
    void type_straight(char* toShow, int LINE);
    void ch_move();
    int ch_in(int x1, int y1,int x2, int y2);
    int ch_in_x();
    int random_monster();

public:
    bool initial = true;
    int msg;
    int game1_win = No;
    int game2_win = No;
    int random_battle = No;
    int random_monster_state = No;
    int battle_state = DECIDE;
    int arena1_win = No;
    int final_arena_win = No;
    int scene_state;
    int choose_gender_state;
    int choose_egg_state = 0;
    int music_state;
    int music_pause_position;
    int egg_chosen;
    int goback = No;
    int enter_beginner_town_state = No;
    int t=0;
    char tmp[20];

    int player_sex;
    string player_name = "www";
    Monster MyMonster[3];
    int fighting_monster_code;
    int monster_left;

    Monster enemy1;
    Monster enemy2;
    Monster wild_monster;
    double typetime = 0.01; //default 0.025
    int character_x;
    int character_y;
    int ox;
    int oy;
    int dx = 0;
    int dy = 0;
    int ch_height;
    int ch_width;
    int ch_anime = 0;
    int ch_anime_time = 30;
    int ch_dir = RIGHT;
    int ch_state = STAND;
    int textIndex =  0;
    double lastCharTime = 0;
    double currentTime = 0;
    string doctor_intro_line[3]={"First, tell me something about you! ","Here is where monster trainers get their first mini monster!","Welcome to the home of mini monsters~ "};
    int doctor_intro_line_count = 3-1;
    const string meet_doctor_line[4];
    int meet_doctor_line_count=4;
    const string beginnertown_line[7];
    int beginnertown_line_count=7;
    const string town_line[2];
    int town1_line_count=2;
    int town2_line_count=2;
    const string arena1_line[3];
    int arena1_line_count=3;
    const string lose_line;
    const string win_line;
    Damage fight;

private:
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *chat_box = NULL;
    ALLEGRO_BITMAP *music_button;
    ALLEGRO_BITMAP *bush = NULL;
    ALLEGRO_BITMAP *first_monster_water = NULL;
    ALLEGRO_BITMAP *first_monster_fire = NULL;
    ALLEGRO_BITMAP *first_monster_grass = NULL;
    ALLEGRO_BITMAP *character = NULL;
    ALGIF_ANIMATION *fire_atk = NULL;
    ALGIF_ANIMATION *water_atk = NULL;
    ALGIF_ANIMATION *grass_atk = NULL;
    ALLEGRO_BITMAP *player_monster[3] ;
    ALLEGRO_BITMAP *opponent_monster = NULL;

    ALLEGRO_BITMAP *ch_img[3];
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *font2 = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;

    bool redraw = false;
    bool mute = false;
};



#endif // MAINWINDOW_H_INCLUDED

