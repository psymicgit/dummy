#include <stdlib.h>

#include "robot.h"

int main(int argc, char **argv)
{
    Robot robot;
    robot.Connect("127.0.0.1", 23);

    system("pause");
    return 0;
}