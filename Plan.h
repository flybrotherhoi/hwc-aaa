#include<bits/stdc++.h>
#include "RBB.h"

#ifndef _PLAN_H_
#define _PLAN_H_
using namespace std;

const int N = 200;
const int robot_price=2000;
const int boat_price=8000;
const int default_max_robot_num=16;
const int default_max_boat_num=2;
class Plan
{
public:
    int money, berth_num, goods_num, robot_num, boat_num, boat_capacity;
    int frame_id;
    int p_delivery_point;
    char grid[N+10][N+10];
    int robot_map[N][N];
    int boat_map[N][N];
    int boat_map_dual[N][N];
    int gds_time[N][N];
    int gds_val[N][N];
    int max_robot_num;
    int max_boat_num;
    vector<array<array<int,N>,N>> distance_to_berth;
    int shortest_distance_to_berth[N][N];
    int nearest_berth[N][N];
    vector<Robot> robot;
    vector<Boat> boat;
    vector<Berth> berth;
    vector<Position> robot_purchase_point;
    vector<Position> boat_purchase_point;
    vector<Position> delivery_point;
    set<int> started_berth_set;
    set<int> unstarted_berth_set;
    vector<set<int>> delivery_berth_set;
    unsigned int p_robot_purchase_point;
    unsigned int p_boat_purchase_point;
    Plan() {max_robot_num=default_max_robot_num; max_boat_num=default_max_boat_num;}
    void Init();
    void ProcessMap();
    void GetBoatDualMap(int temp_map[][N], int temp_map_dual[][N]);
    void Input();
    void Output();

    void Process();
    
    void RobotDo();
    void BoatDo();
    void BoatDoBindingBerth();
    void BoatDoBindingDelivery();
    void BerthDo();

    void RobotRoutePlan(int rid, Position target);
    bool RobotRoutePlanForCollision(int rid, Position target);
    void BoatRoutePlan(int bid,  Position target);
    void BoatRoutePlanContinue(int bid, Position target);
    void BoatRoutePlanAStar(int bid,  Position target);
    void BoatRoutePlanDijkstra(int bid, Position target);
    bool BoatRoutePlanForCollision(int bid,  Position target);

    void RobotFindGoods(int rid);
    void RobotFindBerth(int rid);

    bool BoatFindBerth(int bid);
    bool BoatFindDelivery(int bid);

    void ShipIn(int boat_id, int berth_id);
    void ShipOut(int boat_id, int berth_id);

    void BuyBoat();
    
    void Summary();
    void SaveLog();

    int BoatInMainRoad(int bid);
};

#endif