#include <vector>
#include <list>
#include <time.h>
#include "global.h"
#include "GameWindow.h"

enum{
    win, lose
};

class arena:public GameWindow{

public:
    // constructor
    arena();

    // each process of scene
    void game_init();
    void game_reset();
    int game_play();
    void game_begin();

    int game_run();
    int game_update();

    int show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw_running_map();

    // process of updated event
    int process_event();

    int mouse_in(int x1, int y1, int x2, int y2);
    int mouse_in_circle(int x, int y, int r);

public:
    bool initial = true;
    int music_state;
    int music_pause_position;
    int result;

private:
    ALLEGRO_BITMAP *background = NULL;

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
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

    int mouse_x, mouse_y;

    bool redraw = false;
    bool mute = false;
};
