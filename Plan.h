#include<bits/stdc++.h>
#include "RBB.h"

#ifndef _PLAN_H_
#define _PLAN_H_
using namespace std;

const int N = 200;
const int robot_price=2000;
const int boat_price=8000;
class Plan
{
public:
    int money, berth_num, goods_num, robot_num, boat_num, boat_capacity;
    int frame_id;
    char grid[N+10][N+10];
    int robot_map[N][N];
    int boat_map[N][N];
    int boat_map_dual[N][N];
    int gds_time[N][N];
    int gds_val[N][N];
    vector<Robot> robot;
    vector<Boat> boat;
    vector<Berth> berth;
    vector<Position> robot_purchase_point;
    vector<Position> boat_purchase_point;
    vector<Position> delivery_point;
    Plan() {}
    void Init();
    void ProcessMap();
    void Input();
    void Output();

    void Process();
    
    void RobotDo();
    void BoatDo();
    void BerthDo();

    void RobotRoutePlan(int rid, Position target);
    bool RobotRoutePlanForCollision(int rid, Position target);
    void BoatRoutePlan(int bid,  Position target);
    bool BoatRoutePlanForCollision(int bid,  Position target);

    void RobotFindGoods(int rid);
    void RobotFindBerth(int rid);

    bool BoatFindBerth(int bid);
    bool BoatFindDelivery(int bid);

    void ShipIn(int boat_id, int berth_id);
    void ShipOut(int boat_id, int berth_id);
    
    void Summary();
    void SaveLog();
};

#endif