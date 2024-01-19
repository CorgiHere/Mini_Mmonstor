#include "GameWindow.h"
#include <iostream>
#include "skill.cpp"

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
    doctor_intro_scene,
    choose_gender_scene,
    meet_doctor_scene,
    choose_egg_scene,
    itembackpack_scene,
    monsterbackpack_scene,
    beginnertown_scene,
    pharmacy_scene,
    itemshop_scene,
    town_scene,
    town2_scene,
    map1_scene,
    map2_scene,
    arena1_scene,
    final_arena_scene,
    battle_scene,
    game_over_scene,
    happy_end_scene
};
enum{
    music_on, music_off
};

enum{
    LINE1, LINE2
};

void
GameWindow::game_init()
{

    background = al_load_bitmap("./images/menu.png");
    chat_box = al_load_bitmap("./images/chatbox.png");
    music_button = al_load_bitmap("./images/music_button0.png");
    first_monster_water = al_load_bitmap("./images/monster_small_0_0.png");
    first_monster_fire = al_load_bitmap("./images/monster_small_1_0.png");
    first_monster_grass = al_load_bitmap("./images/monster_small_2_0.png");

    //icon = al_load_bitmap("./images/icon.png");
    //al_set_display_icon(display, icon);

    al_reserve_samples(3);

    sample = al_load_sample("./sounds/sound.wav");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());

}

void
GameWindow::game_play()
{
    srand(time(NULL));
    int msg = -1;
    game_reset();
    game_begin();

    while(msg != GAME_EXIT) msg = game_run();

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE) fprintf(stderr, "Game Terminated...");
    else fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}


GameWindow::GameWindow()
{
    if (!al_init()) show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    if(timer == NULL ) show_err_msg(-1);
    if (display == NULL || event_queue == NULL) show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("./fonts/VCR.ttf",36,0); // load small font
    font2 = al_load_ttf_font("./fonts/VCR.ttf",45,0); // load small font
    Medium_font = al_load_ttf_font("./fonts/VCR\.ttf",50,0); //load medium font
    Large_font = al_load_ttf_font("./fonts/VCR.ttf",72,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    game_init();
}

void
GameWindow::game_begin()
{

    // Character related
    ch_dir = RIGHT, ch_state = STAND;
    character_x = 0, character_y = 396;
    scene_state = first_scene;

    // Stories
    goback = enter_beginner_town_state = egg_chosen = No;
    choose_egg_state = EGG;
    doctor_intro_line_count = 2;
    meet_doctor_line_count = 4;

    // Arena related
    random_battle = game1_win = game2_win = arena1_win = final_arena_win = No;
    enemy1.monster_initial(water, 1, 20);
    enemy2.monster_initial(fire, 1, 50);
    random_monster_state = monster_left = 0;
    battle_state = DECIDE;
    Damage fight;

    draw_running_map();
    al_play_sample_instance(backgroundSound);
    al_start_timer(timer);

}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;
    if (!al_is_event_queue_empty(event_queue)) error = process_event();
    return error;
}

int
GameWindow::game_update()
{
    char temp[50];
    switch(scene_state){
        case first_scene:
            background = al_load_bitmap("./images/menu.png");
            sprintf(temp,"./images/music_button%i.png",music_state);
            music_button = al_load_bitmap(temp);
            break;
        case info_scene:
            background = al_load_bitmap("./images/info.png");
            break;
        case doctor_intro_scene:
            background = al_load_bitmap("./images/meet_doctor.png");
            break;
        case choose_gender_scene:
            if(choose_gender_state == non) background = al_load_bitmap("./images/chs_non.png");
            else if(choose_gender_state == male) background = al_load_bitmap("./images/chs_male.png");
            else background = al_load_bitmap("./images/chs_female.png");
            break;
        case meet_doctor_scene:
            chat_box = al_load_bitmap("./images/chatbox.png");
            char load[20];
            if(player_sex == male) {
                character = al_load_bitmap("./images/male.png");
                for(int i = 0; i < 3; i++){
                    sprintf(load,"./images/m%d.png",i);
                    ch_img[i] = al_load_bitmap(load);
                }
            }
            else {
                character = al_load_bitmap("./images/female.png");
                for(int i = 0; i < 3; i++){
                    sprintf(load,"./images/f%d.png",i);
                    ch_img[i] = al_load_bitmap(load);
                }
            }
            ch_width = al_get_bitmap_width(ch_img[0]);
            ch_height = al_get_bitmap_height(ch_img[0]);
            background = al_load_bitmap("./images/meet_doctor.png");
            break;
        case choose_egg_scene:
            chat_box = al_load_bitmap("./images/chat_box_empty.png");
            if(choose_egg_state == EGG){
                background = al_load_bitmap("./images/chs_egg.png");
            }else if(choose_egg_state == EGG1){
                background = al_load_bitmap("./images/chs_egg1.png");
            }else if(choose_egg_state == EGG2){
                background = al_load_bitmap("./images/chs_egg2.png");
            }else if(choose_egg_state == EGG3){
                background = al_load_bitmap("./images/chs_egg3.png");
            }
            break;
        case itembackpack_scene:
            //background = al_load_bitmap("./images/menu.png");
            break;
        case monsterbackpack_scene:
            //background = al_load_bitmap("./images/menu.png");
            break;
        case beginnertown_scene:
            background = al_load_bitmap("./images/beginner_town.png");
            break;
        case pharmacy_scene:
            //background = al_load_bitmap("./images/menu.png");
            break;
        case itemshop_scene:
            //background = al_load_bitmap("./images/menu.png");
            break;
        case map1_scene:
            background = al_load_bitmap("./images/map1.png");
            bush = al_load_bitmap("./images/bush.png");
            break;
        case town_scene:
            background = al_load_bitmap("./images/town.png");
            break;
        case arena1_scene:
            background = al_load_bitmap("./images/arena1.png");
            break;
        case town2_scene:
            background = al_load_bitmap("./images/town2.png");
            break;
        case map2_scene:
            //background = al_load_bitmap("./images/map2.png");
            break;
        case final_arena_scene:
            background = al_load_bitmap("./images/final_arena.png");
            break;
        case battle_scene:
            for(int i=0;i<3;i++){
                sprintf(temp,"./images/monster_%d_%d.png", MyMonster[i].monster_property(), MyMonster[i].monster_num());
                if(MyMonster[i].alive == Yes) player_monster[i] = al_load_bitmap(temp);
            }
            if(random_battle == Yes)sprintf(temp,"./images/monster_%d_%d.png", wild_monster.monster_property(), wild_monster.monster_num());
            else if(game1_win == No)sprintf(temp,"./images/monster_%d_%d.png", enemy1.monster_property(), enemy1.monster_num());
            else sprintf(temp,"./images/monster_%d_%d.png", enemy2.monster_property(), enemy2.monster_num());
            opponent_monster = al_load_bitmap(temp);
            background = al_load_bitmap("./images/battle.png");
            break;
        case game_over_scene:
            background = al_load_bitmap("./images/gameover.png");
            break;
        case happy_end_scene:
            background = al_load_bitmap("./images/win.png");
            break;
    }
    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    al_stop_sample_instance(backgroundSound);
}

void
GameWindow::game_destroy()
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
    al_destroy_sample_instance(backgroundSound);
}

int
GameWindow::process_event()
{
    srand(time(NULL));
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;
    char temp[14];

    /*TODO:*/
    // process key board state
    switch(scene_state){
        case first_scene:
            if((event.keyboard.keycode == ALLEGRO_KEY_I && event.type == ALLEGRO_EVENT_KEY_DOWN) || mouse_in(643,641,1277,804) == 2){
                scene_state = info_scene;
                redraw = true;
            }else if((event.keyboard.keycode == ALLEGRO_KEY_ENTER && event.type == ALLEGRO_EVENT_KEY_DOWN) || mouse_in(643,455,1277,618) == 2){
                scene_state = doctor_intro_scene;
                redraw = true;
            }else if((event.keyboard.keycode == ALLEGRO_KEY_M && event.type == ALLEGRO_EVENT_KEY_DOWN) || mouse_in(643,828,1277,991) == 2){
                if(music_state == music_on) {
                    music_pause_position = al_get_sample_instance_position(backgroundSound);
                    al_stop_sample_instance(backgroundSound);
                    music_state = music_off;
                }else{
                    al_set_sample_instance_position(backgroundSound, music_pause_position);
                    al_play_sample_instance(backgroundSound);
                    music_state = music_on;
                }
                redraw = true;
            }

            break;
        case info_scene:
            if(event.keyboard.keycode == ALLEGRO_KEY_I && event.type == ALLEGRO_EVENT_KEY_DOWN){
                scene_state = first_scene;
                redraw = true;
            }
            break;
        case doctor_intro_scene:
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                    if(doctor_intro_line_count>0){
                        doctor_intro_line_count--;
                    }else{
                        scene_state = choose_gender_scene;
                        choose_gender_state = non;
                        sprintf(tmp, "");
                    }
                    redraw = true;
                }
            break;
        case choose_gender_scene:
            if(mouse_in(482,382,873,887)==2){
                choose_gender_state = male;
                redraw = true;
            }else if(mouse_in(1050,382,1442,887)==2){
                choose_gender_state = female;
                redraw = true;
            }
            if((choose_gender_state == female || choose_gender_state == male) && event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                scene_state = meet_doctor_scene;
                player_sex = choose_gender_state;
                redraw = true;
            }
            break;
        case meet_doctor_scene:
            if(egg_chosen == No){

                if(character_x < 1200){
                    ch_state = MOVE;
                    character_x += 5;
                    al_draw_bitmap(background,0,0,0);
                    ch_draw();
                    al_flip_display();
                }
                else if(ch_state != STAND){
                    ch_state = STAND;
                    ch_draw();
                    redraw = true;
                }
            }
            if(egg_chosen == No && character_x >= 1200 && event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                scene_state = choose_egg_scene;
                choose_egg_state = EGG;
                redraw = true;
            }
            if(goback){
                if(character_x > 0 - ch_width -150){
                    ch_state = MOVE;
                    ch_dir = LEFT;
                    character_x-=5;
                    al_draw_bitmap(background,0,0,0);
                    ch_draw();
                    al_flip_display();
                }else if(ch_state != STAND){
                    ch_state = STAND;
                    ch_draw();
                    scene_state = beginnertown_scene;
                    fighting_monster_code = 0;
                    redraw = true;
                    ch_dir = RIGHT;
                    character_x = 20, character_y = 350;
                    ch_draw();
                }
            }else if(egg_chosen == Yes ){
                if(event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
                    goback = Yes;
                }
            }
            break;
        case choose_egg_scene:
            if(mouse_in(349,75,684,475)==2){
                choose_egg_state = EGG1;
                redraw = true;
            }else if(mouse_in(793,75,1128,475)==2){
                choose_egg_state = EGG2;
                redraw = true;
            }else if(mouse_in(1236,75,1571,475)==2){
                choose_egg_state = EGG3;
                redraw = true;
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && (choose_egg_state == EGG1)){
                    MyMonster[0].monster_initial(grass, 0, FIRST_LEVEL);
                    egg_chosen = Yes;
                    scene_state = meet_doctor_scene;
                    monster_left++;
                    redraw = true;
            }else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && (choose_egg_state == EGG2)){
                    MyMonster[0].monster_initial(water, 0, FIRST_LEVEL);
                    egg_chosen = Yes;
                    scene_state = meet_doctor_scene;
                    monster_left++;
                    redraw = true;
            }else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && (choose_egg_state == EGG3)){
                    MyMonster[0].monster_initial(fire, 0, FIRST_LEVEL);
                    egg_chosen = Yes;
                    scene_state = meet_doctor_scene;
                    monster_left++;
                    redraw = true;
            }
            break;
        case itembackpack_scene:
            break;
        case monsterbackpack_scene:
            break;
        case beginnertown_scene:
            if(enter_beginner_town_state == No && event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                enter_beginner_town_state = Yes;
                redraw = true;
                al_draw_bitmap(background, 0, 0, 0);
                ch_draw();
                al_flip_display();
            }
            else if(enter_beginner_town_state == Yes){
                ox = character_x, oy = character_y;
                ch_move();
                if(!ch_in(-10,350,685,545) && !ch_in(660,235,765,910) && !ch_in(760,735,1930,910))
                    character_x = ox, character_y = oy;

                if(character_x >1910 && (character_y + ch_height - 20 < 910)&&(character_y + ch_height - 20 >  735)){
                    scene_state = map1_scene;
                    character_x = -5;
                    ch_draw();
                    redraw = true;
                }
            }
            break;
        case pharmacy_scene:
            break;
        case itemshop_scene:
            break;
        case map1_scene:
            ox = character_x, oy = character_y;
            if(random_monster_state != Yes)ch_move();
            if(random_monster_state == Yes){
                al_draw_bitmap(chat_box, 291, 548, 0);
                char toShow[50] = "Oops! You encountered a wild monster!";
                al_draw_text(Medium_font,al_map_rgb(255,255,255),410,660,ALLEGRO_ALIGN_LEFT,toShow);
                al_draw_text(Medium_font,al_map_rgb(0,0,0),407,658,ALLEGRO_ALIGN_LEFT,toShow);
                al_flip_display();
                if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                    random_battle = Yes;
                    random_monster_state = No;
                    arena1_win = No;
                    scene_state = battle_scene;
                    int val = MyMonster[fighting_monster_code].monster_level()-5-rand()%5;
                    val = val > 30 ? 30 :val;
                    val = val < 5 ? 5 : val;
                    wild_monster.monster_initial(rand()%3,rand()%2, val);
                    fight.damage_init(MyMonster[fighting_monster_code], wild_monster);
                    redraw = true;
                }
            }
            if(!ch_in(-ch_width + 10,715,1440,920) && !ch_in(670,240,850,790) &&
               !ch_in(1310,240,1440,790) && !ch_in(1440,350,1930,545) &&
               !ch_in(420,350,670,790) && !ch_in(850,240,900,700) && !ch_in(1205,240,1315,700) &&
               !ch_in(1440,350,1735,920) && !ch_in(850,520,1315,790))
                character_x = ox, character_y = oy;

            if(character_x <-ch_width + 15 &&(character_y + ch_height - 20 < 910)&&(character_y + ch_height - 20 >  735)){
                scene_state = beginnertown_scene;
                character_x = 1910;
                ch_draw();
                redraw = true;
            }

            if(character_x >1900 &&(character_y + ch_height - 20 < 545)&&(character_y + ch_height - 20 >  350)){
                scene_state = town_scene;
                character_x = -5;
                ch_draw();
                redraw = true;
            }
            if(dx < 20) dx += (character_x - ox) > 0 ? (character_x - ox) : -(character_x - ox);
            if(dy < 20) dy += (character_y - oy) > 0 ? (character_y - oy) : -(character_y - oy);
            if ((dx >= 20 || dy >= 20) &&(!ch_in(425,350,640,720) ||!ch_in(850,250,950,520) ||
                !ch_in(1210,250,1310,520) || !ch_in(850,510,1310,715) || !ch_in(1505,520,1730,900)))
                if(random_monster()) {
                    random_monster_state = Yes;
                    redraw = true;
                }
            if(dx >= 20) dx = 0;
            if(dy >= 20) dy = 0;
            break;
        case town_scene:
            ox = character_x, oy = character_y;
                ch_move();
                if(!ch_in(-ch_width,350,685,545) && !ch_in(685,230,790,905) && !ch_in(785,230,1315,410)
                   && !ch_in(785,745,1930,910) && !ch_in(1310,230,1425,905))
                    character_x = ox, character_y = oy;
            if(character_x <-ch_width + 15 &&(character_y + ch_height - 20 < 545)&&(character_y + ch_height - 20 >  350)){
                scene_state = map1_scene;
                character_x = 1910;
                ch_draw();
                redraw = true;
            }
            if(character_x >1900 &&(character_y + ch_height - 20 < 910)&&(character_y + ch_height - 20 >  745)){
                scene_state = arena1_scene;
                character_x = -5;
                ch_draw();
                redraw = true;
            }
            //
            break;
        case arena1_scene:
            ox = character_x, oy = character_y;
                ch_move();
            if(!ch_in(680,225,780,470) && !ch_in(950,255,1065,850) && !ch_in(680,395,950,575)
            && !ch_in(950,590,1930,760) && !ch_in(-10,745,1065,910) && !ch_in(1250,520,1320,700))
                character_x = ox, character_y = oy;

            if(character_x <-ch_width + 15 &&(character_y + ch_height - 20 < 910)&&(character_y + ch_height - 20 >  745)){
                scene_state = map1_scene;
                character_x = 1910;
                ch_draw();
                redraw = true;
            }
            if(character_x > 1250 && character_x < 1320 && character_y < 400 && game1_win == No){
                scene_state = battle_scene;
                fight.damage_init(MyMonster[fighting_monster_code], enemy1);
                redraw = true;
            }
            if(character_x >1910 &&(character_y + ch_height - 20 < 760)&&(character_y + ch_height - 20 >  590)&&game1_win == Yes){
                scene_state = town2_scene;
                character_x = -5;
                ch_draw();
                redraw = true;
            }
            break;
        case town2_scene:
            ox = character_x, oy = character_y;
                ch_move();
            if(!ch_in_x() && !ch_in(670,135,780,845) && !ch_in(-ch_width+10,590,685,760) && !ch_in(1025,660,1930,850))
                character_x = ox, character_y = oy;
            if(character_x <-ch_width + 15 &&(character_y + ch_height - 20 < 760)&&(character_y + ch_height - 20 >  590)){
                scene_state = arena1_scene;
                character_x = 1910;
                ch_draw();
                redraw = true;
            }
            if(character_x >1900 &&(character_y + ch_height - 20 > 660)&&(character_y + ch_height - 20 < 850)){
                scene_state = final_arena_scene;
                character_x = -5;
                ch_draw();
                redraw = true;
            }
            break;
        case final_arena_scene:
            ox = character_x, oy = character_y;
                ch_move();
            if(!ch_in(-ch_width+10,660,1930,850) && !ch_in(1250,550,1345,850))
                character_x = ox, character_y = oy;
            if(character_x > 1250 && character_x < 1345 &&(character_y + ch_height - 20 <  670 && game2_win == No)){
                scene_state = battle_scene;
                fight.damage_init(MyMonster[fighting_monster_code], enemy2);
                printf("%d %d\n",fight._player.physical_attack_status(),fight._opponent.physical_attack_status());
                redraw = true;
            }
            break;
            break;

       case battle_scene:
            //printf("%d\n", battle_state);
            int p_h, p_w, o_h, o_w;

            al_draw_bitmap(background,0,0,0);
            char temp[14], lv[10];
            sprintf(temp,"%d/%d",fight._player.monster_present_blood(), fight._player.full_blood_status());
            al_draw_text(Medium_font,WHITE,360,189,0,temp);
            al_draw_text(Medium_font,BLACK,357,187,0,temp);
            al_draw_text(Medium_font,WHITE,448,129,0, fight._player.monster_name().c_str());
            al_draw_text(Medium_font,BLACK,445,127,0, fight._player.monster_name().c_str());
            itoa(fight._player.monster_level(),temp,10);
            sprintf(lv, "Lv.%s",temp);
            al_draw_text(Medium_font,WHITE,600,189,0,lv);
            al_draw_text(Medium_font,BLACK,597,187,0,lv);


            sprintf(temp,"%d/%d",fight._opponent.monster_present_blood(), fight._opponent.full_blood_status());
            al_draw_text(Medium_font,WHITE,1175,189,0,temp);
            al_draw_text(Medium_font,BLACK,1172,187,0,temp);
            al_draw_text(Medium_font,WHITE,1263,129,0, fight._opponent.monster_name().c_str());
            al_draw_text(Medium_font,BLACK,1260,127,0, fight._opponent.monster_name().c_str());
            itoa(fight._opponent.monster_level(),temp,10);
            sprintf(lv, "Lv.%s",temp);
            al_draw_text(Medium_font,WHITE,1415,189,0,lv);
            al_draw_text(Medium_font,BLACK,1412,187,0,lv);


            p_h = al_get_bitmap_height(player_monster[fighting_monster_code]);
            p_w = al_get_bitmap_width(player_monster[fighting_monster_code]);
            o_h = al_get_bitmap_height(opponent_monster);
            o_w = al_get_bitmap_width(opponent_monster);
            al_draw_bitmap(player_monster[fighting_monster_code],540-p_w/2,570-p_h,0);
            al_draw_bitmap(opponent_monster,1354-o_w/2,570-o_h,ALLEGRO_FLIP_HORIZONTAL);

            if(battle_state == CHECK_BATTLE_RESULT){
                string t_win, t_lose;
                t_lose = ("Enemy\'s " + fight._opponent.monster_name());
                t_win = ("Your " + fight._player.monster_name());
                if(arena1_win == Yes) al_draw_text(font,BLACK,550,800,ALLEGRO_ALIGN_CENTRE,t_win.c_str());
                else al_draw_text(font,BLACK,550,800,ALLEGRO_ALIGN_CENTRE,t_lose.c_str());
                al_draw_text(font,BLACK,550,840,ALLEGRO_ALIGN_CENTRE," defeats ");
                t_lose = ("your " + fight._player.monster_name() + "!");
                t_win = ("enemy\'s " + fight._opponent.monster_name() + "!");
                if(arena1_win == Yes) al_draw_text(font,BLACK,550,880,ALLEGRO_ALIGN_CENTRE,t_win.c_str());
                else al_draw_text(font,BLACK,550,880,ALLEGRO_ALIGN_CENTRE,t_lose.c_str());
                al_draw_text(font,BLACK,550,920,ALLEGRO_ALIGN_CENTRE,"press \"Space\" to continue.");
                al_flip_display();
                if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                    if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                        if(random_battle == Yes){
                            if(arena1_win == Yes){
                                scene_state = map1_scene;
                                random_battle = No;
                            }else {
                                scene_state = game_over_scene;
                                random_battle = No;
                            }
                        }
                        else if(arena1_win == Yes) {
                            if(game1_win == No){
                                scene_state = arena1_scene;
                                game1_win = Yes;
                            }else{
                                game2_win = Yes;
                                scene_state = happy_end_scene;
                            }
                        }
                        else if (arena1_win == No) scene_state = game_over_scene;
                        arena1_win = No;
                        redraw = true;
                        battle_state = DECIDE;
                    }
            }
            else if(battle_state == DECIDE){;
                if(mouse_in(993,769,1375,909) == 2) battle_state = FIGHT;
                else if(mouse_in(1375,769,1761,909) == 2) battle_state = HEALING;
                else if(mouse_in(993,909,1375,1000) == 2) battle_state = CAPSULE;
                else if(mouse_in(1375,909,1761,1000) == 2)battle_state = RUN;
                al_draw_text(Medium_font,BLACK,550,870,ALLEGRO_ALIGN_CENTRE,"What do you want to do?");
                al_flip_display();
            }
            else if(battle_state == FIGHT){
                if(mouse_in(164, 724, 550, 870) == 2){
                    printf("SKILL1\n");
                    skill_click(SKILL1);
                }else if(mouse_in(550, 724, 963, 870) == 2){
                    printf("SKILL2\n");
                    skill_click(SKILL2);
                    if(battle_state != CHECK_BATTLE_RESULT) battle_state = DECIDE;
                }else if(mouse_in(164, 870, 550, 1000) == 2){
                    printf("SKILL3\n");
                    skill_click(SKILL3);
                    if(battle_state != CHECK_BATTLE_RESULT) battle_state = DECIDE;
                }else if(mouse_in(550, 870,  963, 1000) == 2){
                    printf("SKILL4\n");
                    skill_click(SKILL4);
                    if(battle_state != CHECK_BATTLE_RESULT) battle_state = DECIDE;
                }

                /*
                if(mouse_in(164, 724, 550, 870) == 2) skill_click(SKILL1);
                else if(mouse_in(550, 724, 963, 870) == 2) skill_click(SKILL2);
                else if(mouse_in(164, 870, 550, 1000) == 2) skill_click(SKILL3);
                else if(mouse_in(550, 870,  963, 1000) == 2) skill_click(SKILL4);
                if(battle_state == FIGHT) battle_state = DECIDE;
                */
                al_draw_bitmap(background,0,0,0);
                char temp[14];
                sprintf(temp,"%d/%d",fight._player.monster_present_blood(), fight._player.full_blood_status());
                al_draw_text(Medium_font,WHITE,360,189,0,temp);
                al_draw_text(Medium_font,BLACK,357,187,0,temp);
                al_draw_text(Medium_font,WHITE,448,129,0, fight._player.monster_name().c_str());
                al_draw_text(Medium_font,BLACK,445,127,0, fight._player.monster_name().c_str());
                itoa(fight._player.monster_level(),temp,10);
                sprintf(lv, "Lv.%s",temp);
                al_draw_text(Medium_font,WHITE,600,189,0,lv);
                al_draw_text(Medium_font,BLACK,597,187,0,lv);


                sprintf(temp,"%d/%d",fight._opponent.monster_present_blood(), fight._opponent.full_blood_status());
                al_draw_text(Medium_font,WHITE,1175,189,0,temp);
                al_draw_text(Medium_font,BLACK,1172,187,0,temp);
                al_draw_text(Medium_font,WHITE,1263,129,0, fight._opponent.monster_name().c_str());
                al_draw_text(Medium_font,BLACK,1260,127,0, fight._opponent.monster_name().c_str());
                itoa(fight._opponent.monster_level(),temp,10);
                sprintf(lv, "Lv.%s",temp);
                al_draw_text(Medium_font,WHITE,1415,189,0,lv);
                al_draw_text(Medium_font,BLACK,1412,187,0,lv);
                p_h = al_get_bitmap_height(player_monster[fighting_monster_code]);
                p_w = al_get_bitmap_width(player_monster[fighting_monster_code]);
                o_h = al_get_bitmap_height(opponent_monster);
                o_w = al_get_bitmap_width(opponent_monster);
                al_draw_bitmap(player_monster[fighting_monster_code],540-p_w/2,570-p_h,0);
                al_draw_bitmap(opponent_monster,1354-o_w/2,570-o_h,ALLEGRO_FLIP_HORIZONTAL);

                al_draw_text(font2,BLACK,210,807,0, skill_dictionary[(fight._player.use_skill(SKILL1))].name_skill.c_str());
                al_draw_text(font2,BLACK,575,807,0, skill_dictionary[(fight._player.use_skill(SKILL2))].name_skill.c_str());
                al_draw_text(font2,BLACK,210,950,0, skill_dictionary[(fight._player.use_skill(SKILL3))].name_skill.c_str());
                al_draw_text(font2,BLACK,575,950,0, skill_dictionary[(fight._player.use_skill(SKILL4))].name_skill.c_str());
                al_flip_display();
            }
            else if(battle_state == HEALING) battle_state = DECIDE;
            else if(battle_state == CAPSULE) battle_state = DECIDE;
            else if(battle_state == RUN) battle_state = DECIDE;
            break;
        case game_over_scene:
            al_draw_bitmap(background,0,0,0);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_begin();
                    scene_state = meet_doctor_scene;
                    redraw = true;
                }if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    printf("Game Over\n!");
                    exit(0);
                }
        break;
        case happy_end_scene:
            al_draw_bitmap(background,0,0,0);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_begin();
                    scene_state = meet_doctor_scene;
                    redraw = true;
                }if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    exit(0);
                }
        break;

    }

    if(event.keyboard.keycode >= ALLEGRO_KEY_1 && event.keyboard.keycode <= ALLEGRO_KEY_6
       && scene_state >= beginnertown_scene) {
        character_x = 1020, character_y = 720;
        if(event.keyboard.keycode == ALLEGRO_KEY_5 || event.keyboard.keycode == ALLEGRO_KEY_6 ) character_y = 650;
        if(event.keyboard.keycode == ALLEGRO_KEY_1  ) scene_state = beginnertown_scene;
        else if(event.keyboard.keycode == ALLEGRO_KEY_2  ) scene_state = map1_scene;
        else if(event.keyboard.keycode == ALLEGRO_KEY_3  ) scene_state = town_scene;
        else if(event.keyboard.keycode == ALLEGRO_KEY_4  ) scene_state = arena1_scene;
        else if(event.keyboard.keycode == ALLEGRO_KEY_5  ) scene_state = town2_scene;
        else if(event.keyboard.keycode == ALLEGRO_KEY_6  ) scene_state = final_arena_scene;
        redraw = true;
    }
    if(scene_state == choose_gender_scene){
        if( event.type == ALLEGRO_EVENT_KEY_DOWN &&
           (((event.keyboard.keycode >= 1 && event.keyboard.keycode <= 26)||
           (event.keyboard.keycode >= ALLEGRO_KEY_0 && event.keyboard.keycode <= ALLEGRO_KEY_9)||
           (event.keyboard.keycode >= ALLEGRO_KEY_PAD_0 && event.keyboard.keycode <= ALLEGRO_KEY_PAD_9))&&t != 16)||
           (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE&&t>=1)){
        if(t != 10){
            if(event.keyboard.keycode >= 1 && event.keyboard.keycode <= 26)
                    tmp[t++] = 'A' + event.keyboard.keycode - 1;
            else if(event.keyboard.keycode >= ALLEGRO_KEY_0 && event.keyboard.keycode <= ALLEGRO_KEY_9)
                    tmp[t++] = '0' + event.keyboard.keycode - ALLEGRO_KEY_0;
            else if(event.keyboard.keycode >= ALLEGRO_KEY_PAD_0 && event.keyboard.keycode <= ALLEGRO_KEY_PAD_9)
                    tmp[t++] = '0' + event.keyboard.keycode - ALLEGRO_KEY_PAD_0;
            redraw = true;
        }
        if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && t >= 1){
            tmp[t--] = '\0';
            redraw = true;
        }
        tmp[t] ='\0';
        player_name = tmp;
        }
    }
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
GameWindow::skill_click(int skill_num){
    printf("%d\n", skill_num);
    fight.decision(skill_num);
    fight.opponent_decision_generate();
    printf("OPPONENT hp first: %d\n",fight._opponent.monster_present_blood());
    printf("PLAYER hp first: %d\n",fight._player.monster_present_blood());
    printf("opponent:%d player:%d\n",fight._opponent_move, fight._player_move);
    if(fight.who_go_first() == PLAYER){
        printf("PLAYER go first\n");
        int d = fight.cause_damage(fight._player.use_skill(fight._player_move), &fight._player, &fight._opponent);
        printf("damage done: %d\n",d);
        if(fight._opponent.monster_present_blood() > d)
            fight._opponent.determine_status(fight._opponent.monster_present_blood()-d,fight._opponent.monster_level());
        else fight._opponent.determine_status(0, fight._opponent.monster_level());

        if(fight.anyone_win() == NONE){
            d = fight.cause_damage(fight._opponent.use_skill(fight._opponent_move), &fight._opponent, &fight._player);
            printf("damage done: %d\n",d);
            if(fight._player.monster_present_blood() > d)
                fight._player.determine_status(fight._player.monster_present_blood()-d, fight._player.monster_level());
            else fight._player.determine_status(0, fight._player.monster_level());

            if(fight.anyone_win() == OPPONENT){
                monster_left--;
                if(monster_left <= 0) battle_state = CHECK_BATTLE_RESULT;
                else {
                    MyMonster[fighting_monster_code].alive = No;
                    if(MyMonster[0].alive == Yes)fighting_monster_code=0;
                    else if(MyMonster[1].alive == Yes)fighting_monster_code=1;
                    else if(MyMonster[2].alive == Yes)fighting_monster_code=2;
                    fight.damage_init(MyMonster[fighting_monster_code], fight._opponent);
                }
            }
        }
        else if(fight.anyone_win() == PLAYER){
            printf("%d ",fight._player.monster_level());
            fight._player.determine_status(fight._player.monster_present_blood(),fight._player.monster_level());
            MyMonster[fighting_monster_code]._level += rand()%5+1;
            MyMonster[fighting_monster_code]._present_blood = MyMonster[fighting_monster_code].full_blood_status();
            printf("%d\n",fight._player.monster_level());
            battle_state = CHECK_BATTLE_RESULT;
            arena1_win = Yes;
        }
    }else if(fight.who_go_first() == OPPONENT){
        printf("OPPONENT go first\n");
        int d = fight.cause_damage(fight._opponent.use_skill(fight._opponent_move), &fight._opponent, &fight._player);
        printf("damage done: %d\n",d);
        if(fight._player.monster_present_blood() > d)
            fight._player.determine_status(fight._player.monster_present_blood()-d,fight._player.monster_level());
        else fight._player.determine_status(0, fight._player.monster_level());

        if(fight.anyone_win() == NONE){
            printf("NOW FOR PLAYER\n");
            d = fight.cause_damage(fight._player.use_skill(fight._player_move), &fight._player, &fight._opponent);
            printf("damage done: %d\n",d);
            if(fight._opponent.monster_present_blood() > d)
                fight._opponent.determine_status(fight._opponent.monster_present_blood()-d, fight._opponent.monster_level());
            else fight._opponent.determine_status(0, fight._opponent.monster_level());

            if(fight.anyone_win() == PLAYER){
                printf("%d ",fight._player.monster_level());
                fight._player.determine_status(fight._player.monster_present_blood(),fight._player.monster_level());
                MyMonster[fighting_monster_code]._level += rand()%5+1;
                MyMonster[fighting_monster_code]._present_blood = MyMonster[fighting_monster_code].full_blood_status();
                printf("%d\n",fight._player.monster_level());
                battle_state = CHECK_BATTLE_RESULT;
                arena1_win = Yes;
            }
        }else if(fight.anyone_win() == OPPONENT){
            monster_left--;
            if(monster_left <= 0) battle_state = CHECK_BATTLE_RESULT;
            else {
                MyMonster[fighting_monster_code].alive = No;
                if(MyMonster[0].alive == Yes)fighting_monster_code=0;
                else if(MyMonster[1].alive == Yes)fighting_monster_code=1;
                else if(MyMonster[2].alive == Yes)fighting_monster_code=2;
                fight.damage_init(MyMonster[fighting_monster_code], fight._opponent);
            }
        }
    }
    printf("OPPONENT hp left: %d\n",fight._opponent.monster_present_blood());
    printf("PLAYER hp left: %d\n",fight._player.monster_present_blood());
    printf("ya\n");
    redraw = true;
}

void
GameWindow::draw_running_map()
{
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_bitmap(background, 0, 0, 0);
    if(scene_state == first_scene ){
        al_draw_bitmap(music_button,643,828,0);
    }else if(scene_state == doctor_intro_scene ){
        //al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(chat_box, 291, 548, 0);
        printf("%d",doctor_intro_line_count);
        typewriter(doctor_intro_line[doctor_intro_line_count]);
    }
    else if(scene_state == choose_gender_scene){
        //al_draw_bitmap(background, 0, 0, 0);
        al_draw_text(Large_font,al_map_rgb(0,0,0),961,233,ALLEGRO_ALIGN_CENTRE,tmp);
        al_draw_text(Large_font,al_map_rgb(255,255,255),965,235,ALLEGRO_ALIGN_CENTRE,tmp);
    }
    else if(scene_state == meet_doctor_scene){
        if(egg_chosen == No){
            //al_draw_bitmap(background, 0, 0, 0);
            ch_draw();
            if(character_x>=1200) {
                al_draw_bitmap(chat_box, 291, 548, 0);
                typewriter("Hi! " +player_name+"! Glad to meet you!Now choose your first mini monster!");
            }
        }else {
            ch_draw();
            al_draw_bitmap(chat_box, 291, 548, 0);
            typewriter("Oh! It is " +MyMonster[0].monster_name()+"! Great Choice! Take good care of it and HAVE FUN!");
        }

    }else if(scene_state == choose_egg_scene){
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(chat_box, 291, 548, 0);
        if(choose_egg_state == EGG)typewriter("Choose an egg and press ENTER to get your first monster.");
        else {
            type_straight("Choose an egg and press ENTER to get",LINE1);
            type_straight("your first monster.",LINE2);
        }
    }
    else if(scene_state == beginnertown_scene){
        if(enter_beginner_town_state == No){
            al_draw_bitmap(chat_box, 291, 548, 0);
            typewriter("Now you arrive a town! Please explore!");
        }
        else if(enter_beginner_town_state == Yes){
            al_draw_bitmap(background, 0, 0, 0);
            ch_draw();
        }
    }else if(scene_state == map1_scene||scene_state == town_scene||scene_state == arena1_scene||scene_state == town2_scene){
        al_draw_bitmap(background, 0, 0, 0);
        ch_draw();
    }

    /*TODO:*/
    // draw Hero
    al_flip_display();
}


int GameWindow::mouse_in(int x1, int y1, int x2, int y2){//rectangle
    if( event.mouse.x > x1 && event.mouse.x < x2 && event.mouse.y > y1 && event.mouse.y < y2){
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            return 2;
        return 1;
    }
    return 0;
}
int GameWindow::mouse_in_circle(int x, int y, int r){//circle
    if(((event.mouse.x-x)*(event.mouse.x-x) + (event.mouse.y-y)*(event.mouse.y-y)) < r * r){
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            return 2;
        return 1;
    }
    return 0;
}
void GameWindow::ch_draw(){
    if(event.type == ALLEGRO_EVENT_TIMER)
        if(event.timer.source == timer){
            ch_anime++;
            ch_anime%=ch_anime_time;
        }
    if(ch_state == STAND )
    {
        if( ch_dir == LEFT )
            al_draw_bitmap(ch_img[0], character_x, character_y, ALLEGRO_FLIP_HORIZONTAL);
        else if( ch_dir == RIGHT )
            al_draw_bitmap(ch_img[0], character_x, character_y, 0);

    }
    else if(ch_state == MOVE){
        if( ch_dir == LEFT ){
            if( ch_anime < ch_anime_time/2 )
                al_draw_bitmap(ch_img[1], character_x, character_y, ALLEGRO_FLIP_HORIZONTAL); // change image when jumping and (press L or R)

            else
                al_draw_bitmap(ch_img[2], character_x, character_y, ALLEGRO_FLIP_HORIZONTAL);
        }
        else if( ch_dir == RIGHT ){
            if( ch_anime < ch_anime_time/2 )
                al_draw_bitmap(ch_img[1], character_x, character_y, 0);
            else
                al_draw_bitmap(ch_img[2], character_x, character_y, 0);
        }
    }
    if(MyMonster[0].monster_property() == water && scene_state == meet_doctor_scene && egg_chosen == Yes){
        al_draw_bitmap(first_monster_water, character_x+120, character_y+5, ALLEGRO_FLIP_HORIZONTAL);
    }else if(MyMonster[0].monster_property() == fire && scene_state == meet_doctor_scene && egg_chosen == Yes){
        al_draw_bitmap(first_monster_fire, character_x+120, character_y, ALLEGRO_FLIP_HORIZONTAL);
    }else if(MyMonster[0].monster_property() == grass && scene_state == meet_doctor_scene && egg_chosen == Yes){
        al_draw_bitmap(first_monster_grass, character_x+120, character_y+5, ALLEGRO_FLIP_HORIZONTAL);
    }
}
void GameWindow::type_straight(char* toShow, int LINE){
    if(LINE == LINE1){
        al_draw_text(Medium_font,al_map_rgb(255,255,255),410,660,ALLEGRO_ALIGN_LEFT,toShow);
        al_draw_text(Medium_font,al_map_rgb(0,0,0),407,658,ALLEGRO_ALIGN_LEFT,toShow);
    }else if(LINE2){
        al_draw_text(Medium_font,al_map_rgb(255,255,255),410,730,ALLEGRO_ALIGN_LEFT,toShow);
        al_draw_text(Medium_font,al_map_rgb(0,0,0),407,728,ALLEGRO_ALIGN_LEFT,toShow);
    }
}

void GameWindow::typewriter(string currentText){
    char toShow[100];
    int changeLine;
    for(int i = 39; i > 0; i--)
        if(currentText[i] == ' ' ||currentText[i] == '.' ||currentText[i] == '!' ||currentText[i] == '~' ||currentText[i] == ',') {
            changeLine = i;
            break;
        }
    if(textIndex == 200) {textIndex = 0; lastCharTime = currentTime = al_get_time();}
    while(textIndex < currentText.length()){
        currentTime = al_get_time();
        if(currentTime - lastCharTime > typetime && textIndex < currentText.length()) {
            textIndex++;
            lastCharTime = currentTime;
        }
        if(textIndex<=changeLine){
            strcpy(toShow,currentText.substr(0,textIndex).c_str());
            al_draw_text(Medium_font,al_map_rgb(255,255,255),410,660,ALLEGRO_ALIGN_LEFT,toShow);
            al_draw_text(Medium_font,al_map_rgb(0,0,0),407,658,ALLEGRO_ALIGN_LEFT,toShow);
            if(textIndex == changeLine) for(int i = changeLine; i > 0; toShow[i--] = '\0');
            al_flip_display();
            if(textIndex == changeLine && currentText[changeLine + 1] == ' ')textIndex++;
        }else if(textIndex>changeLine && textIndex < currentText.length()){
            toShow[textIndex-changeLine-1] = currentText[textIndex];
            al_draw_text(Medium_font,al_map_rgb(255,255,255),410,730,ALLEGRO_ALIGN_LEFT,toShow);
            al_draw_text(Medium_font,al_map_rgb(0,0,0),407,728,ALLEGRO_ALIGN_LEFT,toShow);
            al_flip_display();
        }
        if(textIndex == currentText.length() - 1) textIndex = 200;
    }
}

void GameWindow::ch_move(){
    if(key_state[ALLEGRO_KEY_W]){
        ch_state = MOVE;
        character_y -= 5;
    }if(key_state[ALLEGRO_KEY_A]){
        ch_state = MOVE;
        ch_dir = LEFT;
        character_x -= 5;
    }if(key_state[ALLEGRO_KEY_S]){
        ch_state = MOVE;
        character_y += 5;
    }if(key_state[ALLEGRO_KEY_D]){
        ch_state = MOVE;
        ch_dir = RIGHT;
        character_x += 5;
    }if(!key_state[ALLEGRO_KEY_W] && !key_state[ALLEGRO_KEY_A] &&
        !key_state[ALLEGRO_KEY_S] && !key_state[ALLEGRO_KEY_D] && ch_state != STAND){
        ch_state = STAND;
    }
    al_draw_bitmap(background, 0, 0, 0);
    ch_draw();
    if(scene_state == map1_scene){
        al_draw_bitmap(bush,0,0,0);
    }
    al_flip_display();
}

int GameWindow:: ch_in(int x1, int y1,int x2, int y2){
    if( character_x >= x1 && character_x <= x2 &&
        character_y + ch_height >= y1 + 35 && character_y + ch_height <= y2-20 ) return 1;
    else return 0;
}
int GameWindow:: ch_in_x(){
    if(((character_x + character_y + ch_height-25 >=1155)&&
       (character_x + character_y + ch_height-25 <=1845)&&
       (character_x - (character_y + ch_height-25) >=190)&&
       (character_x - (character_y + ch_height-25) <=350))||
       ((character_x + character_y + ch_height-25 >=1155)&&
       (character_x + character_y + ch_height-25 <=1270)&&
       (character_x - (character_y + ch_height-25) >=190)&&
       (character_x - (character_y + ch_height-25) <=925)))
        return 1;
    else return 0;
}
int GameWindow::random_monster(){
    srand((int)al_get_time());
    int p = rand() % 100;
    if(p < 20) return 1;
    else return 0;
}
