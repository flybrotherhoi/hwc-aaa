#include<bits/stdc++.h>
using namespace std;

#ifndef _RBB_H_
#define _RBB_H_

#define MY_DEBUG 1

enum RobotAction{GET, PULL, RA_NOTHING};
enum RobotStatus{Ready, ToGoods, ToBerth, Collision};
enum RobotMove{RIGHT, LEFT, UP, DOWN, STAND};
enum BoatAction{ROT_CLOCKWISE, ROT_ANTICLOCKWISE, SHIP, DEPT, BERTH, BOAT_NOTHING};
enum BoatStatus{BReady, BToDelivery, BToBerth, BLoading, BDepting, BBerthing};

const int rdx[4]={0,0,-1,1};
const int rdy[4]={1,-1,0,0};

typedef pair<int,int> Position;

int inverse_dir(const int &dir);
int get_boat_dir(const Position &pos, const Position &next_pos);
Position get_core_pos(Position posDual, int dir);
Position get_dual_pos(Position posCore, int dir);
int get_boat_move(int dir, int next_dir);
RobotMove InverseRobotMove(const RobotMove &move);
int manhattan_distance(const Position &a, const Position &b);

class Robot
{
public:
    int id, goods_num, goods_val; 
    RobotStatus status;     // -1=not ready; 0=idle; 1=going to goods; 2=return to berth
    int sys_status; // 0=restore, 1=normal;
    int berth_id;
    Position pos, last_pos;
    Position target;
    bool has_target;
    bool has_goods;
    RobotAction action_before_move; // 0=nothing, 1=get, 2=pull
    int action_move;    // 0=right, 1=left, 2=up, 3=down
    RobotAction action_after_move;  // 0=nothing, 1=get, 2=pull
    vector<Position> route;
    unsigned int p_route;
    Robot() {}
    Robot(int startX, int startY) {
        pos = Position(startX, startY);
        last_pos = Position(-1, -1);
        status = Ready;
        has_goods = false;
        has_target = false;
        berth_id = -1;
        goods_num = 0;
    }
    int GetRobotMove(Position next_pos){
        for(int i=0;i<4;i++){
            if(pos.first+rdx[i]==next_pos.first && pos.second+rdy[i]==next_pos.second){
                return i;
            }
        }
        return 4;
    };
    void MoveNormal(){
        if(p_route<route.size()){
            if(MY_DEBUG) cerr<<"Robot "<<id<<" move to "<<route[p_route].first<<","<<route[p_route].second<<endl;
            Position next_pos = route[p_route];
            action_move = GetRobotMove(next_pos);
            last_pos = pos;
            pos = next_pos;
            p_route++;
        }
    }
};

class Boat
{
public:
    int id;
    int status, sys_status;
    // int x,y;
    Position pos;
    int dir;
    int goods_num, goods_val;
    int target_berth;
    BoatStatus boat_status;
    Position target;
    bool has_target;
    vector<Position> route;
    vector<int> route_move;
    unsigned int p_route;
    BoatAction action;
    // int action_move;
    Boat() {
        status = BReady;
        goods_num = 0;
        has_target = false;
    }
    Boat(int startX, int startY){
        pos = Position(startX, startY);
        status = BReady;
        goods_num = 0;
        has_target = false;
    }
    Position DualPos(){
        return get_dual_pos(pos, dir);
    }
    void MoveNormal(){
        // ofstream fout("log_boat_pos.txt", ios::app);
        if(p_route<route.size() && p_route<route_move.size()){
            // fout<<pos.first<<","<<pos.second<<"   dual pos:"<<DualPos().first<<","<<DualPos().second<<endl;
            Position next_pos = route[p_route];
            pos = next_pos;
            action = static_cast<BoatAction>(route_move[p_route]);
            p_route++;
        }
    }
};

class Berth
{
public:
    Position pos;
    int id;
    int loading_speed;
    int boat_id;
    std::set<int> boat_lst;
    int goods_num, goods_val, remain_goods;
    std::vector<unsigned int> fetch_time;
    std::vector<int> goods_val_lst;
    Berth(){
        pos = Position(-1,-1);
        loading_speed = 0;
        boat_id = -1;
        goods_num = 0;
        goods_val = 0;
    }
    Berth(int x, int y, int loading_speed){
        pos = Position(x, y);
        this->loading_speed = loading_speed;
        boat_id = -1;
        goods_num = 0;
        goods_val = 0;
    }
    void GoodsIn(int goods_val){
        if(goods_val>0){
            goods_num++;
            goods_val_lst.push_back(goods_val);
        }
    }
};


#endif