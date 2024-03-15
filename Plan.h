#include<bits/stdc++.h>
#include "RBB.h"

#ifndef _PLAN_H_
#define _PLAN_H_
using namespace std;

class Plan
{
public:
    int money;
    int boat_capacity;
    Robot robot[10];
    Berth berth[10];
    std::deque<short> robot_path[10];
    short robot_path_len[10];
    std::vector<pair<int,int>> berth_region[10];
    Boat boat[10];
    char ch[210][210];
    short map[210][210];
    short parts[210][210];
    int shortest_dist_to_berth[10][210][210];
    unsigned int fid;
    int gds[210][210];
    int gds_time[210][210];
    Plan() {}
    void Init();
    int Input();
    void Output();
    void Process();
    void RobotDo();
    void BoatDo();
    void BerthDo();
};

#endif