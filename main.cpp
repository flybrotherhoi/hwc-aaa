#include <bits/stdc++.h>
#include "Plan.h"
using namespace std;

// const int n = 200;
// const int robot_num = 10;
// const int berth_num = 10;
// const int N = 210;

// Robot robot[robot_num + 10];
// Berth berth[berth_num + 10];
// Boat boat[10];

// int money, boat_capacity, id;
// char ch[N][N];
// int gds[N][N];
// void Init()
// {
//     for(int i = 1; i <= n; i ++)
//         scanf("%s", ch[i] + 1);
//     for(int i = 0; i < berth_num; i ++)
//     {
//         int id;
//         scanf("%d", &id);
//         scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
//     }
//     scanf("%d", &boat_capacity);
//     char okk[100];
//     scanf("%s", okk);
//     printf("OK\n");
//     fflush(stdout);
// }

// int Input()
// {
//     scanf("%d%d", &id, &money);
//     int num;
//     scanf("%d", &num);
//     for(int i = 1; i <= num; i ++)
//     {
//         int x, y, val;
//         scanf("%d%d%d", &x, &y, &val);
//     }
//     for(int i = 0; i < robot_num; i ++)
//     {
//         int sts;
//         scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
//     }
//     for(int i = 0; i < 5; i ++)
//         scanf("%d%d\n", &boat[i].status, &boat[i].pos);
//     char okk[100];
//     scanf("%s", okk);
//     return id;
// }

int main()
{
    Plan plan{};
    plan.Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = plan.Input();
        for(int i = 0; i < robot_num; i ++)
            printf("move %d %d\n", i, rand() % 4);
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
