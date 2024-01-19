#include "playerinfo.h"

Playerinfo::playerinfo(int sex, string name, Monster first_monster){
    _sex = sex;
    _name = name;
    MyMonster[0] = first_monster;
}
