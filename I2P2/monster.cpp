#include "monster.h"
#include "global.h"
#include "skill.cpp"
#include <iostream>
#include <time.h>

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))

/*typedef struct _status{
    double physical_attack;
    double physical_defence;
    double magical_attack;
    double magical_defence;
    double speed;
    double blood_full;
    string name;
}S;*/

const S Monster_dictionary[3][4][2]={//[屬性][編號][固有/成長]
    {
        {{50,40,60,70,60,50,"Sea Spirit"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,"Jellicora"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}}
    },
    {
        {{50,40,60,70,60,50,"Evil Fire"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,"Emberflame"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}}
    },
    {
        {{50,40,60,70,60,50,"Bushii"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,"Reddish Sprite"},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}},
        {{50,40,60,70,60,50,""},{3,2,4,4,3,6}}
    }
};


void
Monster::identify_monster(int property, int num){
    _monster_num = num;
    _monster_property = property;
    if(property == water){
        _weakness = grass;
        _strength = fire;
    }
    else if(property == fire){
        _weakness = water;
        _strength = grass;
    }
    else {
        _weakness = fire;
        _strength = water;
    }
}

void
Monster::determine_initial_skill(int property, int num){
    srand(time(NULL));
    int t=0;
    for(int i=0;i<4;i++){
        int tmp = rand() ;
        t=0;
        for(int j=0;j<i;j++){
            if(tmp%10 + property*10 == _owned_skill[j]){
                t=1;
                i--;
            }
        }
        if(t==0)_owned_skill[i] = tmp%10 + property*10;
    }
}

void
Monster::monster_initial(int property, int num, int level){
    alive = Yes;
    identify_monster(property, num);
    determine_initial_skill(property, num);
    _level = level;
    _base_status = Monster_dictionary[property][num][0];
    _groth_status = Monster_dictionary[property][num][1];
    _present_blood = full_blood_status();
}

void
Monster::determine_status(int present_blood, int level){
    _level = level;
    _present_blood = present_blood;
}

Monster::monster(){
}

string
Monster::monster_name(){
    return _base_status.name;
}

int
Monster::physical_attack_status(){
    return _level*_groth_status.physical_attack + _base_status.physical_attack;
}

int
Monster::magical_attack_status(){
    return _level*_groth_status.magical_attack + _base_status.magical_attack;
}

int
Monster::physical_defence_status(){
    return _level*_groth_status.physical_defence + _base_status.physical_defence;
}

int
Monster::magical_defence_status(){
    return _level*_groth_status.magical_defence + _base_status.magical_defence;
}

int
Monster::speed_status(){
    return _level*_groth_status.speed + _base_status.speed;
}

int
Monster::full_blood_status(){
    return _level*_groth_status.blood_full + _base_status.blood_full;
}

int
Monster::monster_level(){
    return _level;
}

int
Monster::use_skill(int number){
    return _owned_skill[number];
}

int
Monster::monster_num(){
    return _monster_num;
}

int
Monster::monster_property(){
    return _monster_property;
}

int
Monster::monster_present_blood(){
    return _present_blood;
}

int
Monster::monster_weakness(){
    return _weakness;
}

int
Monster::monster_strength(){
    return _strength;
}
