#include<iostream>
#include "global.h"
#include "monster.h"

class playerinfo{
public:
    playerinfo();

    void inputname(string name);
    void inputsex(int sex);

public:
    int playersex();
    int playername();
    Monster My_Monster(int num);

private:
    int sex;
    string name;
    Monster MyMonster[3];
} ;
