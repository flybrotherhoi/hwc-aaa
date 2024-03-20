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
    std::vector<pair<int,int>> pos_robot_path[10];
    short robot_path_len[10];
    std::vector<pair<int,int>> berth_region[10];
    Boat boat[10];
    char ch[210][210];
    short map[210][210];
    short dynamic_map[210][210];
    short parts[210][210];
    int shortest_dist_to_berth[10][210][210];
    unsigned int fid;
    int gds[210][210];
    int gds_time[210][210];
    Plan() {}
    void Init();
    void InitPlanB();
    int Input();
    void DivideParts();
    void Output();
    void Process();
    bool RoutePlan(short tmap[210][210], int id);
    void RobotDo();
    void RobotDoNoDelay();
    void RobotDoGlobal();
    void BoatDoPlain();
    void BoatDoGreedy();
    void BoatDoGreedyMore();
    void BoatDoGreedyMoreMore();
    void BerthDo();
    void Summary();
    void UpdateShortestDistToBerth(short temp_map[210][210],int id);
    void UpdateShortestDistToBerthAndParts(short temp_map[210][210],int id);
    void UpdateShortestDistToBerthAndParts(int id);
    void UpdateBerthRegion(int id);
    void SaveLog();
};

#endif