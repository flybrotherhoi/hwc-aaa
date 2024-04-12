#include <bits/stdc++.h>
#include "Plan.h"
using namespace std;

int main(int argc, char *argv[])
{
    Plan plan;

    if(argc ==2){
        plan.max_robot_num = atoi(argv[1]);
        plan.max_boat_num = default_max_boat_num;
    }else if(argc==3){
        plan.max_robot_num = atoi(argv[1]);
        plan.max_boat_num = atoi(argv[2]);
    }

    plan.Init();
    while(scanf("%d", &plan.frame_id) != EOF)
    {
        plan.Input();
        plan.Process();
        puts("OK");
        fflush(stdout);
    }
    if(SAVE_LOG)plan.SaveLog();
    return 0;
}
