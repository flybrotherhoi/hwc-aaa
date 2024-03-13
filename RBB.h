#include<bits/stdc++.h>
enum RobotAction{GET, PULL};
enum RobotStatus{NotReady, ReadyToGo, GoingToGood, ReturnToBerth};
enum RobotMove{RIGHT, LEFT, UP, DOWN, STAND};

class Robot
{
public:
    int x, y, gx, gy, goods;
    RobotStatus status;     // -1=not ready; 0=idle; 1=going to goods; 2=return to berth
    int sys_status; // 0=restore, 1=normal;
    int berth_id;
    RobotAction action_before_move; // 0=nothing, 1=get, 2=pull
    RobotMove action_move;    // 0=right, 1=left, 2=up, 3=down
    RobotAction action_after_move;  // 0=nothing, 1=get, 2=pull
    Robot() {
        x = 0;
        y = 0;
        goods = 0;
        status = NotReady;
        berth_id = -1;
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
    int robot_id;
    Berth(){
        x = 0;
        y = 0;
        transport_time = 0;
        loading_speed = 0;
        boat_id = -1;
        robot_id = -1;
    }
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
};

class Boat
{
public:
    int num, pos, status;
    int goods;
    int berth_id[10];
    Boat() {
        num = 0;
        pos = 0;
        status = 0;
        goods = 0;
        memset(berth_id, -1, sizeof(berth_id));
    }
};