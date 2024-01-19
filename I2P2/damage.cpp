#include "damage.h"
#include "skill.cpp"
#include <time.h>


Damage::damage(){}

void
Damage::damage_init(Monster player, Monster opponent){
    _player = player;
    _opponent = opponent;
}

int
Damage::who_go_first(){
    if(_player.speed_status() >= _opponent.speed_status())return PLAYER;
    else return OPPONENT;
}

int
Damage::cause_damage(int skill_num, Monster *attacker, Monster *defender){
    double rate;
    double result;
    printf("done00");
    if(skill_dictionary[skill_num].type == physical){
        printf("done01\n");
        printf("%d %d\n",attacker->physical_attack_status(),defender->physical_defence_status());
        rate = (double)attacker->physical_attack_status()/(double)defender->physical_defence_status();
        printf("rate = %f\n",rate);
        result = rate * skill_dictionary[skill_num].power;
    }else {
        printf("done02\n");
        printf("%d %d\n",attacker->physical_attack_status(),defender->physical_defence_status());
        rate = (double)attacker->magical_attack_status()/(double)defender->magical_defence_status();
        printf("rate = %f\n",rate);
        result = rate * (double)skill_dictionary[skill_num].power;
    }
    printf("done03\n");
    if(defender->monster_weakness() == skill_dictionary[skill_num].property){
        result*= 1.2;
    }else if(defender->monster_strength() == skill_dictionary[skill_num].property){
        result *= 0.8;
    }
    printf("result = %f\n",result);
    return (int)result;
}

int
Damage::anyone_win(){
    if(_player.monster_present_blood()==0)return OPPONENT;
    else if(_opponent.monster_present_blood()==0)return PLAYER;
    return NONE;
}

void
Damage::opponent_decision_generate(){
    srand(time(NULL));
    _opponent_move = rand()%5;
}
void
Damage::decision(int player_move){
    _player_move = player_move;
}

//private:
//Monster _player, _opponent;
//int _player_move, _opponent_move;
//int _player_skill, _opponent_skill;

