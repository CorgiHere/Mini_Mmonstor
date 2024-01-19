#include "arena.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))

enum {
    first_scene,
    info_scene,
    choose_gender_scene,
    itembackpack_scene,
    medicinebackpack_scene,
    monsterbackpack_scene,
    beginnertown_scene,
    pharmacy_scene,
    itemshop_scene,
    skilladjust_scene,
    town_scene,
    map1_scene,
    map2_scene,
    arena1_scene,
    arena2_scene,
    finaltown_scene,
    finalarena_scene,
    hall_of_fame_scene
};
enum{
    music_on, music_off
};

void
arena::game_init()
{

    background = al_load_bitmap("./menu.png");

    //al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("growl.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("arena_bgm.mp3");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());
}

int
arena::game_play()
{
    int msg;

    srand(time(NULL));
    msg = -1;
    game_reset();
    game_begin();

    while(msg != GAME_EXIT)
    {
        msg = game_run();
    }

    return show_err_msg(msg);
}

int
arena::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    return result;
}


arena::arena()
{
    if (!al_init())
        show_err_msg(-1);
    printf("Game Initializing...\n");
    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);
    if(timer == NULL )
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    game_init();
}

void
arena::game_begin()
{
    music_state = music_on;
    draw_running_map();
    al_play_sample_instance(startSound);
    while(al_get_sample_instance_playing(startSound));

    al_play_sample_instance(backgroundSound);
    al_start_timer(timer);

}

int
arena::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
arena::game_update()
{
    switch(scene_state){
        case first_scene:
            background = al_load_bitmap("./menu.png");
        case info_scene:
            //printf("101010\n");
            background = al_load_bitmap("./menu.png");
        case choose_gender_scene:
            background = al_load_bitmap("./menu.png");
        case itembackpack_scene:
            background = al_load_bitmap("./menu.png");
        case medicinebackpack_scene:
            background = al_load_bitmap("./menu.png");
        case monsterbackpack_scene:
            background = al_load_bitmap("./menu.png");
        case beginnertown_scene:
            background = al_load_bitmap("./menu.png");
        case pharmacy_scene:
            background = al_load_bitmap("./menu.png");
        case itemshop_scene:
            background = al_load_bitmap("./menu.png");
        case skilladjust_scene:
            background = al_load_bitmap("./menu.png");
        case town_scene:
            background = al_load_bitmap("./menu.png");
        case map1_scene:
            background = al_load_bitmap("./menu.png");
        case map2_scene:
            background = al_load_bitmap("./menu.png");
        case arena1_scene:
            background = al_load_bitmap("./menu.png");
        case arena2_scene:
            background = al_load_bitmap("./menu.png");
        case finaltown_scene:
            background = al_load_bitmap("./menu.png");
        case finalarena_scene:
            background = al_load_bitmap("./menu.png");
        case hall_of_fame_scene:
            background = al_load_bitmap("./menu.png");
    }
    return GAME_CONTINUE;
}

void
arena::game_reset()
{
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
}

void
arena::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);

    //al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);
}

int
arena::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;


    al_wait_for_event(event_queue, &event);
    redraw = false;
    /*TODO:*/
    // process key board state

    if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;

    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    if(redraw) {
        // update each object in game
        instruction = game_update();

        // Re-draw map
        draw_running_map();
        redraw = false;
    }

    return instruction;
}

void
arena::draw_running_map()
{
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(100, 100, 100));

    /*TODO:*/
    // draw Hero
    al_flip_display();
}
