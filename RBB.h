#include<bits/stdc++.h>

class Robot
{
public:
    int x, y, goods;
    int status;
    int mbx, mby;
    int berth_id;
    Robot() {
        x = 0;
        y = 0;
        goods = 0;
        status = 0;
        mbx = 0;
        mby = 0;
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