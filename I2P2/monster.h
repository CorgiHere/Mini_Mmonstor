#include "global.h"

typedef struct _status{
    double physical_attack;
    double physical_defence;
    double magical_attack;
    double magical_defence;
    double speed;
    double blood_full;
    string name;
}S;

class Monster{
public:
    // constructor
    monster();

    void monster_initial(int property, int num, int level);
    void identify_monster(int property, int num);
    void determine_initial_skill(int property, int num);
    void determine_monster_level(int level);

public:
    void determine_status(int present_blood, int level);
    int physical_attack_status();
    int magical_attack_status();
    int physical_defence_status();
    int magical_defence_status();
    int speed_status();
    int full_blood_status();
    string monster_name();
    int use_skill(int number);
    int monster_num();
    int monster_property();
    int monster_present_blood();
    int monster_level();
    int monster_weakness();
    int monster_strength();

    int alive = No;
    int _level;
    int _present_blood;

private:
    S _base_status;
    S _groth_status;
    int _monster_property;
    int _monster_num;
    int _owned_skill[4];
    int _weakness;
    int _strength;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
};
