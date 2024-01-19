#include"skill.h"
#include "global.h"

enum{
    affective, attackive
};

enum{
    physical, magical, neither
};

enum{
    water, fire, grass
};

const Skill skill_dictionary[30]={
    {water,attackive,physical,40,"Water Wall"}, //1
    {water,attackive,physical,40,"Waku Waku"}, //2
    {water,attackive,physical,70,"Water Pulse"}, //3
    {water,attackive,physical,80,"Extinguish"}, //4
    {water,attackive,magical,40,"Aqua Push"}, //5
    {water,attackive,magical,60,"Water Fall"}, //6
    {water,attackive,magical,80,"Aqua Tornado"}, //7
    {water,attackive,magical,90,"Tsnami"}, //8
    {water,attackive,physical,10,"Perfume"}, //9
    {water,attackive,magical,100,"Water Hell"}, //10
    {fire,attackive,physical,30,"Fire Ball"}, //11
    {fire,attackive,physical,40,"Fire Attack"}, //12
    {fire,attackive,physical,50,"Fire Spin"}, //13
    {fire,attackive,physical,90,"Hot Chamber"}, //14
    {fire,attackive,magical,40,"Friday Night"}, //15
    {fire,attackive,magical,450,"Scorching"}, //16
    {fire,attackive,magical,60,"Watch you burn"}, //17
    {fire,attackive,magical,80,"Flame"}, //18
    {fire,attackive,physical,100,"Ember"}, //19
    {fire,attackive,magical,100,"Frightening"}, //20
    {grass,attackive,physical,30,"Cactus Spine"}, //21
    {grass,attackive,physical,40,"Absorb"}, //22
    {grass,attackive,physical,50,"Bullet Corn"}, //23
    {grass,attackive,physical,60,"Cactus Palm"}, //24
    {grass,attackive,magical,20,"Leaf Blade"}, //25
    {grass,attackive,magical,30,"Aroma"}, //26
    {grass,attackive,magical,40,"Grass Tornado"}, //27
    {grass,attackive,magical,50,"Blooming"}, //28
    {grass,attackive,physical,90,"Marijuana"}, //29
    {grass,attackive,magical,90,"Sleeping Pollen"}, //30
    };
