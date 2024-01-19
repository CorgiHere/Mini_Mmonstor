#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED


#include <iostream>
#include <time.h>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "../algif5/src/algif.h"

using namespace std;

#define font_size 12
#define grid_width 40
#define grid_height 40

#define FIRST_LEVEL 100

#define window_width 1920
#define window_height 1080
#define field_width 1920
#define field_height 1080

#define Num_TowerType 5
#define Num_MonsterType 4
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)

/*TODO:*/
// Declare key_state
extern bool key_state[ALLEGRO_KEY_MAX];

enum{
    non, male, female
};
enum{
    No, Yes
};
enum{
    SKILL1, SKILL2, SKILL3, SKILL4, HEAL,CHANGE, BALL
};
enum{
    PLAYER, OPPONENT, NONE
};


#endif // GLOBAL_H_INCLUDED
