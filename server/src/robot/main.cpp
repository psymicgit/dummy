///<------------------------------------------------------------------------------
//< @file:   src\robot\main.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 11:11:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "robotmgr.h"

int main(int argc, char **argv)
{
	RobotMgr robotMgr;
	robotMgr.init();
	robotMgr.start();
	return 0;
}