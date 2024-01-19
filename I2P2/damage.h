#include "global.h"
#include "monster.h"

class Damage
{
    public:
        damage();

        void damage_init(Monster player, Monster opponent);
        int fight_start();
        int who_go_first();
        int cause_damage(int skill_num, Monster *attacker, Monster *defender);
        int anyone_win();

        void opponent_decision_generate();
        void decision(int player_move);

        Monster _player, _opponent;
        int _player_move, _opponent_move;

};

extern Monster _player;
extern Monster _opponent;
extern int _player_move;
extern int _opponent_move;
