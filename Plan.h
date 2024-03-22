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
    std::vector<pair<int,int>> berth_region[10];
    std::vector<int> need_transfer_berth;
    int nearest_berth;
    Boat boat[10];
    char ch[210][210];
    short map[210][210];
    short parts[210][210];
    short original_parts[210][210];
    int shortest_dist_to_berth[10][210][210];
    int original_shortest_dist_to_berth[10][210][210];
    unsigned int fid;
    int gds[210][210];
    int gds_time[210][210];

    Plan() {
        for(int i=0;i<10;i++)
            berth[i].boat_lst.clear();
    }
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
    void RobotDoNoDelayFlush();
    void BoatDoPlain();
    void BoatDoGreedy();
    void BoatDoGreedyMoreMore();
    void BoatDoMinTravel();
    void ShipIn(int boat_id, int berth_id);
    void ShipOut(int boat_id, int berth_id);
    void UpdateBoatStrategy();
    void BerthDo();
    void Summary();
    void UpdateShortestDistToBerth(short temp_map[210][210],int id);
    void UpdateShortestDistToBerthAndParts(short temp_map[210][210],int id);
    void UpdateShortestDistToBerthAndParts(int id, int default_part);
    void UpdateBerthRegion(int id);
    void SaveLog();
};

#endif