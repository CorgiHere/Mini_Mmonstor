#include "GameWindow.h"
int main(int argc, char *argv[])
{
    GameWindow *Mygame= new GameWindow();
    Mygame->game_play();

    delete Mygame;
    return 0;
}
//first_scene:�D���
//info_scene:�C����T
//choose_gender_scene:�﨤��ʧO
//meet_doctor_scene:�J��դh
//itembackpack_scene:�D��I�]
//medicinebackpack_scene:�ī~�I�]
//monsterbackpack_scene:�٦�I�]
//beginnertown_scene:�s�⫰��
//pharmacy_scene:�ħ�
//itemshop_scene:�D�㩱
//skilladjust_scene:���ޯ�ѤH
//town_scene:���~����
//map1_scene:���I�a��1
//map2_scene:���I�a��2
//arena1_scene:�D�]1
//arena2_scene:�D�]2
//finaltown_scene:���I����
//finalarena_scene:�׷��D�]
//hall_of_fame_scene:�W�H��
