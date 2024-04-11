#include "RBB.h"

RobotMove InverseRobotMove(const RobotMove &move) {
    if(move == RIGHT) return LEFT;
    else if(move == LEFT) return RIGHT;
    else if(move == UP) return DOWN;
    else if(move == DOWN) return UP;
    return STAND;
};

int manhattan_distance(const Position &a, const Position &b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int inverse_dir(const int &dir){
    // 0=right, 1=left, 2=up, 3=down
    if(dir==0) return 1; 
    if(dir==1) return 0;
    if(dir==2) return 3;
    if(dir==3) return 2;
    return -1;
}

int get_boat_dir(const Position &pos, const Position &next_pos){
    // 0=right, 1=left, 2=up, 3=down
    assert(manhattan_distance(pos, next_pos)==1);
    if(pos.first==next_pos.first){
        if(pos.second<next_pos.second) return 0;    // right
        else return 1;  // left
    }
    else{
        if(pos.first<next_pos.first) return 3;  // up
        else return 2;  // down
    }
}

Position get_core_pos(Position posDual, int dir){
    if(dir==0) return make_pair(posDual.first, posDual.second);
    if(dir==1) return make_pair(posDual.first+1, posDual.second+1);
    if(dir==2) return make_pair(posDual.first+1, posDual.second);
    if(dir==3) return make_pair(posDual.first, posDual.second+1);
    return make_pair(-1,-1);
}

Position get_dual_pos(Position posCore, int dir){
    if(dir==0) return make_pair(posCore.first, posCore.second);
    if(dir==1) return make_pair(posCore.first-1, posCore.second-1);
    if(dir==2) return make_pair(posCore.first-1, posCore.second);
    if(dir==3) return make_pair(posCore.first, posCore.second-1);
    return make_pair(-1,-1);
}

int get_boat_move(int dir, int next_dir){
    // 0=clockwise, 1=anticlockwise, 2=front
    if(dir==0){
        if(next_dir==0) return 2;
        if(next_dir==1) return -1;  // error
        if(next_dir==2) return 1;
        if(next_dir==3) return 0;
    }else if(dir==1){
        if(next_dir==0) return -1;  // error
        if(next_dir==1) return 2;
        if(next_dir==2) return 0;
        if(next_dir==3) return 1;
    }else if(dir==2){
        if(next_dir==0) return 0;
        if(next_dir==1) return 1;
        if(next_dir==2) return 2;
        if(next_dir==3) return -1;  // error
    }else if(dir==3){
        if(next_dir==0) return 1;
        if(next_dir==1) return 0;
        if(next_dir==2) return -1;  // error
        if(next_dir==3) return 2;
    }
    return -1;
}

bool check_pos_regular(int map_size, Position pos){
    return pos.first>=0 && pos.first<map_size && pos.second>=0 && pos.second<map_size;
    // return true;
}

bool check_pos_regular(int map_size, int first, int second){
    return first>=0 && first<map_size && second>=0 && second<map_size;
}

void avoid_boat_dual(int temp_map[][200], int map_size, Position dual_pos, int dir){
    Position temp_pos{-1,-1};
    for(int i=-1;i<=2;i++){
        temp_pos.first = dual_pos.first+rdx[dir]*i;
        temp_pos.second = dual_pos.second+rdy[dir]*i;
        if(check_pos_regular(map_size, temp_pos)){
            temp_map[temp_pos.first][temp_pos.second] = 0;
        }
        if(dir==0 || dir==1){
            temp_pos.first = temp_pos.first-1;
            if(check_pos_regular(map_size, temp_pos)){
                temp_map[temp_pos.first][temp_pos.second] = 0;
            }
            temp_pos.first = temp_pos.first+2;
            if(check_pos_regular(map_size, temp_pos)){
                temp_map[temp_pos.first][temp_pos.second] = 0;
            }
        }
        else{
            temp_pos.second = temp_pos.second-1;
            if(check_pos_regular(map_size, temp_pos)){
                temp_map[temp_pos.first][temp_pos.second] = 0;
            }
            temp_pos.second = temp_pos.second+2;
            if(check_pos_regular(map_size, temp_pos)){
                temp_map[temp_pos.first][temp_pos.second] = 0;
            }
        }
    }
}

void avoid_boat(int temp_map[][200], Boat boat){
    Position pos = boat.pos;
    int dir = boat.dir;
    for(int i=0;i<3;i++){
        if(temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i]==1){
            temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i]=0;
        }
        if(dir==0){
            if(temp_map[pos.first+rdx[dir]*i+1][pos.second+rdy[dir]*i]==1){
                temp_map[pos.first+rdx[dir]*i+1][pos.second+rdy[dir]*i]=0;
            }
        }else if(dir==1){
            if(temp_map[pos.first+rdx[dir]*i-1][pos.second+rdy[dir]*i]==1){
                temp_map[pos.first+rdx[dir]*i-1][pos.second+rdy[dir]*i]=0;
            }
        }else if(dir==2){
            if(temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i+1]==1){
                temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i+1]=0;
            }
        }else if(dir==3){
            if(temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i-1]==1){
                temp_map[pos.first+rdx[dir]*i][pos.second+rdy[dir]*i-1]=0;
            }
        }
    }
}

bool check_pos_boat_valid(int temp_map[][200], Position pos){
    int x = pos.first;
    int y = pos.second;
    for(int i=0;i<4;i++){
        if(temp_map[x+rdx[i]][y+rdy[i]]==0){
            return false;
        }
    }
    return true;
}