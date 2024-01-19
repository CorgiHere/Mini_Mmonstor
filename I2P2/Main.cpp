#include "GameWindow.h"
int main(int argc, char *argv[])
{
    GameWindow *Mygame= new GameWindow();
    Mygame->game_play();

    delete Mygame;
    return 0;
}
//first_scene:主選單
//info_scene:遊戲資訊
//choose_gender_scene:選角色性別
//meet_doctor_scene:遇到博士
//itembackpack_scene:道具背包
//medicinebackpack_scene:藥品背包
//monsterbackpack_scene:夥伴背包
//beginnertown_scene:新手城鎮
//pharmacy_scene:藥局
//itemshop_scene:道具店
//skilladjust_scene:換技能老人
//town_scene:中途城鎮
//map1_scene:探險地圖1
//map2_scene:探險地圖2
//arena1_scene:道館1
//arena2_scene:道館2
//finaltown_scene:終點城鎮
//finalarena_scene:終極道館
//hall_of_fame_scene:名人堂
