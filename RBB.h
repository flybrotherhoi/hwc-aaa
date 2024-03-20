#include<bits/stdc++.h>

#ifndef _RBB_H_
#define _RBB_H_

enum RobotAction{GET, PULL, RA_NOTHING};
enum RobotStatus{NotReady, ReadyToGo, GoingToGood, ReturnToBerth, Collision};
enum RobotMove{RIGHT, LEFT, UP, DOWN, STAND};
enum BoatAction{SHIP, GO, BA_NOTHING};

class Robot
{
public:
    int x, y, gx, gy, goods, goods_val;
    RobotStatus status;     // -1=not ready; 0=idle; 1=going to goods; 2=return to berth
    int sys_status; // 0=restore, 1=normal;
    int berth_id;
    int start_delay;
    bool is_out;
    RobotAction action_before_move; // 0=nothing, 1=get, 2=pull
    RobotMove action_move;    // 0=right, 1=left, 2=up, 3=down
    RobotAction action_after_move;  // 0=nothing, 1=get, 2=pull
    Robot() {
        x = 0;
        y = 0;
        goods = 0;
        status = NotReady;
        berth_id = -1;
        start_delay = 0;
    }
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

class Berth
{
public:
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int boat_id;
    std::vector<int> boat_lst;
    int robot_id;
    int goods,remain_goods;
    std::vector<unsigned int> fetch_time;
    std::vector<int> goods_val_lst;
    Berth(){
        x = 0;
        y = 0;
        transport_time = 0;
        loading_speed = 0;
        boat_id = -1;
        robot_id = -1;
        goods = 0;
    }
};

class Boat
{
public:
    int num, pos;
    int status;
    int goods, goods_val;
    int berth_id;
    int timer_wait;
    int times_between_berth;
    int MAX_TIMES_BETWEEN_BERTH;
    BoatAction action;
    Boat() {
        num = 0;
        pos = 0;
        status = 0;
        goods = 0;
    }
};


#endif