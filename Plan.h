#include<bits/stdc++.h>
#include "RBB.h"
const int robot_num=10;
const int berth_num=10;
const int n = 200;

class Plan
{
public:
    int money;
    int boat_capacity;
    int id;
    Robot robot[robot_num + 10];
    Berth berth[berth_num + 10];
    Boat boat[10];
    char ch[210][210];
    int gds[210][210];
    Plan() {}
    Plan(int robot_num, int berth_num, int N, int money, int boat_capacity, int id) {
   
        this -> money = money;
        this -> boat_capacity = boat_capacity;
        this -> id = id;
    }
    void Init()
    {
        for(int i = 1; i <= n; i ++)
            scanf("%s", ch[i] + 1);
        for(int i = 0; i < berth_num; i ++)
        {
            int id;
            scanf("%d", &id);
            scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
        }
        scanf("%d", &boat_capacity);
        char okk[100];
        scanf("%s", okk);
        

        printf("OK\n");
        fflush(stdout);
    }
    int Input()
    {
        scanf("%d%d", &id, &money);
        int num;
        scanf("%d", &num);
        for(int i = 1; i <= num; i ++)
        {
            int x, y, val;
            scanf("%d%d%d", &x, &y, &val);
        }
        for(int i = 0; i < robot_num; i ++)
        {
            int sts;
            scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
        }
        for(int i = 0; i < 5; i ++)
            scanf("%d%d\n", &boat[i].status, &boat[i].pos);
        char okk[100];
        scanf("%s", okk);
        return id;
    }
};