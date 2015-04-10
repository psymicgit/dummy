/*
    修改boss战聊天公告，把<击杀>这个暴力词汇替换成<镇压>和<降伏>
    Date: 2014-3-12 11:25:30
    @author by 洪坤安
*/

use ios_app_cfg;

REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_REPORT', '本次镇压【火麒麟】活动中，', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_BOSS_BE_KILLED', '世界BOSS【火麒麟】已被玩家【{玩家名字}】降伏，获得奖励:铜钱{x}.', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_RANK_ENCOURAGE', '第{xx}名【{玩家名字}】,奖励铜钱{M},境界点{N}', '');
REPLACE INTO `msgdefprop` VALUES ('0', 'BOSS_BATTLE_START', 'BOSS战已开始，请各位少侠火速前往麒麟洞镇压火麒麟', '');