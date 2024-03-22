#include<bits/stdc++.h>
#include "Plan.h"
const int MY_DEBUG = 0;
const int see_log = 0;
const int analysis = 1;
vector<int> fetch_time;
long fetch_num, send_num;

const int robot_num=10;
const int berth_num=10;
const int n = 200;
const int MAX_DIST=1e5;
int dx[4]={0,0,-1,1};
int dy[4]={1,-1,0,0};
const int time_between_berth=500;
int WAIT_TIME=50;
const int MORE_WAIT=1;
// int MAX_TIMES_BETWEEN_BERTH=1;
bool robot_good[10];

RobotMove ReverseRobotMove(const RobotMove &move) {
    if(move == RIGHT) return LEFT;
    else if(move == LEFT) return RIGHT;
    else if(move == UP) return DOWN;
    else if(move == DOWN) return UP;
    return STAND;
};

bool diff_pair(const pair<int, int> &a, const pair<int, int> &b) {
    return a.first != b.first || a.second != b.second;
}

void Plan::ShipIn(int berth_id, int boat_id) {
    boat[boat_id].berth_id = berth_id;
    berth[berth_id].boat_lst.insert(boat_id);
    boat[boat_id].timer_wait = WAIT_TIME;
}

void Plan::ShipOut(int berth_id, int boat_id) {
    berth[berth_id].boat_lst.erase(boat_id);
    boat[boat_id].berth_id = -1;
    boat[boat_id].timer_wait = WAIT_TIME;
}

template<typename T>
void copy_map(T src[210][210], T dst[210][210]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            dst[i][j]=src[i][j];
        }
    }
};

void Plan::UpdateShortestDistToBerth(short temp_map[210][210],int id){
    for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
            shortest_dist_to_berth[id][j][k]=MAX_DIST;
        }
    }
    std::queue<std::pair<int,int>> q;
    int x=berth[id].x;
    int y=berth[id].y;
    q.push({x,y});
    shortest_dist_to_berth[id][x][y]=0;
    while(!q.empty()){
        auto [x,y]=q.front();
        q.pop();
        for(int k=0;k<4;k++){
            int nx=x+dx[k];
            int ny=y+dy[k];
            if(nx>=0 && nx<n && ny>=0 && ny<n && temp_map[nx][ny]==1 && shortest_dist_to_berth[id][nx][ny]>shortest_dist_to_berth[id][x][y]+1){
                shortest_dist_to_berth[id][nx][ny]=shortest_dist_to_berth[id][x][y]+1;
                q.push({nx,ny});
            }
        }
    }
}

void Plan::UpdateShortestDistToBerthAndParts(int id, int default_part){
    for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
            shortest_dist_to_berth[id][j][k]=MAX_DIST;
        }
    }
    std::queue<std::pair<int,int>> q;
    int x=berth[id].x;
    int y=berth[id].y;
    q.push({x,y});
    shortest_dist_to_berth[id][x][y]=0;
    while(!q.empty()){
        auto [x,y]=q.front();
        q.pop();
        for(int k=0;k<4;k++){
            int nx=x+dx[k];
            int ny=y+dy[k];
            if(nx>=0 && nx<n && ny>=0 && ny<n && map[nx][ny]==1 && shortest_dist_to_berth[id][nx][ny]>shortest_dist_to_berth[id][x][y]+1 && parts[nx][ny]==id){
                shortest_dist_to_berth[id][nx][ny]=shortest_dist_to_berth[id][x][y]+1;
                q.push({nx,ny});
            }
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            // 把不连通区域的点置为-1
            if(parts[i][j]==id && shortest_dist_to_berth[id][i][j]==MAX_DIST){
                parts[i][j]=default_part;
            }
        }
    }
}

void Plan::UpdateShortestDistToBerthAndParts(short temp_map[][210], int id){
    for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
            shortest_dist_to_berth[id][j][k]=MAX_DIST;
        }
    }
    std::queue<std::pair<int,int>> q;
    int x=berth[id].x;
    int y=berth[id].y;
    q.push({x,y});
    shortest_dist_to_berth[id][x][y]=0;
    while(!q.empty()){
        auto [x,y]=q.front();
        q.pop();
        for(int k=0;k<4;k++){
            int nx=x+dx[k];
            int ny=y+dy[k];
            if(nx>=0 && nx<n && ny>=0 && ny<n && temp_map[nx][ny]==1 && shortest_dist_to_berth[id][nx][ny]>shortest_dist_to_berth[id][x][y]+1 && parts[nx][ny]==id){
                shortest_dist_to_berth[id][nx][ny]=shortest_dist_to_berth[id][x][y]+1;
                q.push({nx,ny});
            }
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            // 把不连通区域的点置为-1
            if(parts[i][j]==id && shortest_dist_to_berth[id][i][j]==MAX_DIST){
                parts[i][j]=-1;
            }
        }
    }
}

bool Plan::RoutePlan(short tmap[210][210], int rid){
    return true;
}

void Plan::UpdateBerthRegion(int id){
    berth_region[id].clear();
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(parts[i][j]==id){
                berth_region[id].push_back(make_pair(i,j));
            }
        }
    }
}

void Plan::DivideParts(){
    /*
    将地图划分为10个各自连通的区域。
    ch[n][n]:   地图原始数据字符数据
    map[n][n]:  地图原始数据的二值化数据，可通行区域为1，不可通行区域为0
    parts[n][n]:地图划分的区域，-1表示不可通行区域，0-9表示各自的区域
    berth位置:  berth[i].x, berth[i].y
    shortest_dist_to_berth[10][210][210]: 存储每个泊位到每个点的最短距离
    */
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            if(ch[i][j] == '#')
                map[i][j] = 0;
            else if (ch[i][j] == '*')
                map[i][j] = 0;
            else
                map[i][j] = 1;
        }

    if(MY_DEBUG)cerr<<"map init done"<<endl;
    if(see_log){
        ofstream out("../LinuxRelease/log/map.txt");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                out<<map[i][j];
            }
            out<<endl;
        }
        out.close();
    }

    // 给每个泊位分配一个区域
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            parts[i][j]=-1;
        }
    }
    for(int i=0;i<berth_num;i++){
        int x=berth[i].x;
        int y=berth[i].y;
        for(int j=0;j<4;j++)for(int k=0;k<4;k++)parts[x+j][y+k]=i;
    }
    
    // for(int i=0;i<10;i++){
    //     for(int j=0;j<n;j++){
    //         for(int k=0;k<n;k++){
    //             shortest_dist_to_berth[i][j][k]=MAX_DIST;
    //         }
    //     }
    // }

    // // 用广度优先搜索计算每个泊位到每个点的最短距离
    // for(int i=0;i<berth_num;i++){
    //     if(MY_DEBUG)cerr<<"finding the shortest distance to berth "<<i<<endl;
    //     std::queue<std::pair<int,int>> q;
    //     q.push({berth[i].x,berth[i].y});
    //     shortest_dist_to_berth[i][berth[i].x][berth[i].y]=0;
    //     while(!q.empty()){
    //         auto [x,y]=q.front();
    //         q.pop();
    //         for(int k=0;k<4;k++){
    //             int nx=x+dx[k];
    //             int ny=y+dy[k];
    //             if(nx>=0 && nx<n && ny>=0 && ny<n && map[nx][ny]==1 && shortest_dist_to_berth[i][nx][ny]>shortest_dist_to_berth[i][x][y]+1){
    //                 shortest_dist_to_berth[i][nx][ny]=shortest_dist_to_berth[i][x][y]+1;
    //                 q.push({nx,ny});
    //             }
    //         }
    //     }
    // }
    for(int i=0;i<berth_num;i++){
        UpdateShortestDistToBerth(map,i);
    }

    if(MY_DEBUG)cerr<<"shortest distance to berth done"<<endl;
    // 分配parts,给map上所有为1的点分配一个距离最近的泊位
    int parts_num[10]={0};
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j]==1){
                int dist=MAX_DIST;
                int id=-1;
                for(int k=0;k<berth_num;k++){
                    if(shortest_dist_to_berth[k][i][j]<dist){
                        dist=shortest_dist_to_berth[k][i][j];
                        id=k;
                    }
                }
                if(dist!=MAX_DIST){
                    parts[i][j]=id;
                    parts_num[id]++;
                }
            }
        }
    }
    if(MY_DEBUG)cerr<<"parts assigned"<<endl;
    if(see_log){
        for(int i=0;i<berth_num;i++){
            ofstream out("../LinuxRelease/log/berth"+to_string(i)+"_dist.txt");
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    out<<setw(7)<<shortest_dist_to_berth[i][j][k];
                }
                out<<endl;
            }
            out.close();
        }
        ofstream out("../LinuxRelease/log/parts.txt");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if (parts[i][j]==-1) out<<'*';
                else out<<parts[i][j];
            }
            out<<endl;
        }
        out.close();
    }
    // copy the original parts and shortest_dist_to_berth
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            original_parts[i][j]=parts[i][j];
        }
    }
    for(int i=0;i<10;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                original_shortest_dist_to_berth[i][j][k]=shortest_dist_to_berth[i][j][k];
            }
        }
    }
}

void Plan::Init()
{
    for(int i = 0; i < n; i ++)
        scanf("%s", ch[i]);
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    if(MY_DEBUG)cerr<<"init start"<<endl;
    // 把泊位的位置初始化离岸最近的位置
    for(int i=0;i<berth_num;i++){
        int sea_line[4] = {0,0,0,0};
        // 看4条边上跟海相邻的点的个数
        // berth[i].x+=1;
        // berth[i].y+=1;
        for(int j=0;j<4;j++){
            if(berth[i].x-1 <0 || ch[berth[i].x-1][berth[i].y+j]=='*' || ch[berth[i].x-1][berth[i].y+j]=='#'){
                sea_line[0]++;
            }
        }
        for(int j=0;j<4;j++){
            if(berth[i].y+4 >=n || ch[berth[i].x+j][berth[i].y+4]=='*' || ch[berth[i].x+j][berth[i].y+4]=='#'){
                sea_line[1]++;
            }
        }
        for(int j=0;j<4;j++){
            if(berth[i].x+4 >=n || ch[berth[i].x+4][berth[i].y+j]=='*' || ch[berth[i].x+4][berth[i].y+j]=='#'){
                sea_line[2]++;
            }
        }
        for(int j=0;j<4;j++){
            if(berth[i].y-1 <0 || ch[berth[i].x+j][berth[i].y-1]=='*' || ch[berth[i].x+j][berth[i].y-1]=='#'){
                sea_line[3]++;
            }
        }
        int min_=10;
        int min_line=-1;
        if(MY_DEBUG){
            cerr<<"berth "<<i<<" sea line: ";
            for(int j=0;j<4;j++){
                cerr<<sea_line[j]<<' ';
            }cerr<<endl;
            cerr<<"berth "<<i<<" position: "<<berth[i].x<<","<<berth[i].y<<"--> ";
        }
        for(int j=0;j<4;j++){
            if(sea_line[j]<min_){
                min_=sea_line[j];
                min_line=j;
            }
        }
        if(min_line==0)berth[i].y+=2;
        if(min_line==1){
            berth[i].x+=2;
            berth[i].y+=3;
        }
        if(min_line==2){
            berth[i].x+=3;
            berth[i].y+=1;
        }
        if(min_line==3)berth[i].x+=1;
        if(MY_DEBUG)cerr<<berth[i].x<<","<<berth[i].y<<endl;
    }
    if(MY_DEBUG){
        cerr<<"berth position initialized"<<endl;
        for(int i=0;i<berth_num;i++){
            cerr<<"<"<<berth[i].x<<','<<berth[i].y<<"> ";
        }cerr<<endl;
    }

    // 打印transport time
    if(1 || MY_DEBUG){
        for(int i=0;i<berth_num;i++){
            cerr<<berth[i].transport_time<<' ';
        }cerr<<endl;
    }
    need_transfer_berth.clear();
    for(int i=0;i<berth_num;i++){
        for(int j=0;j<berth_num;j++){
            if(berth[i].transport_time>=berth[j].transport_time+500){
                need_transfer_berth.push_back(i);
                break;
            }
        }
    }

    for(int i=0;i<berth_num;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                shortest_dist_to_berth[i][j][k]=MAX_DIST;
            }
        }
    }

    // get the position of robots
    int tid=0;
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
        {
            if(ch[i][j] == 'A')
            {
                robot[tid].x = i;
                robot[tid].y = j;
                tid++;
            }
        }
    // initialize gds;
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
        {   
            gds[i][j] = 0;
            gds_time[i][j] = 0;
        }

    // assign berth to boat
    for(int i = 0; i < 5; i ++)
    {
        boat[i].status = 0;
        boat[i].num = 0;
        boat[i].goods = 0;
        boat[i].berth_id = -1;
        boat[i].timer_wait = WAIT_TIME;
    }

    if(see_log){
        ofstream out("../LinuxRelease/log/ch.txt");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                out<<ch[i][j];
            }
            out<<endl;
        }
        out.close();
    }
    DivideParts();

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(parts[i][j]!=-1){
                berth_region[parts[i][j]].push_back(make_pair(i,j));
            }
        }
    }
    if(MY_DEBUG)cerr<<"berth region assigned"<<endl;

    // assign robot to berth
    int flag[10];
    for(int i=0;i<10;i++)flag[i]=0;
    for(int i=0;i<robot_num;i++){
        int x=robot[i].x;
        int y=robot[i].y;
        robot[i].berth_id=-1;
        int nearest_berth = -1;
        int dist = MAX_DIST;
        for(int j=0;j<10;j++){
            if(flag[j]==0 && shortest_dist_to_berth[j][x][y]<dist){
                nearest_berth=j;
                dist=shortest_dist_to_berth[j][x][y];
            }
        }
        if(nearest_berth!=-1){
            robot[i].berth_id=nearest_berth;
            berth[nearest_berth].robot_id=i;
            flag[nearest_berth]++;
            robot_good[i]=true;
            //cerr<<"robot "<<i<<" is assigned to berth "<<nearest_berth<<", its position is "<<x<<","<<y <<" dist= "<<dist<<endl;
        }
        else{
            //cerr<<"robot "<<i<<" cannot be assigned to any berth"<<", its position is "<<x<<","<<y<<endl;
            robot_good[i]=false;
        }
    }

    if(MY_DEBUG)cerr<<"robot assigned to berth"<<endl;

    // plan the path for each robot
    for(int i=0;i<robot_num;i++){
        if(robot_good[i]==false)continue;
        if(MY_DEBUG)cerr<<"finding the path for robot "<<i<<endl;
        int x=robot[i].x;
        int y=robot[i].y;
        int id=robot[i].berth_id;
        int bx=berth[id].x;
        int by=berth[id].y;
        int dist=shortest_dist_to_berth[id][x][y];
        robot[i].status=NotReady;
        robot[i].sys_status=1;
        if(dist==MAX_DIST){
            if(MY_DEBUG)cerr<< "robot" <<i<<" cannot reach berth "<<id<<", robot coordinates: "<<x<<" "<<y<<endl;
            // exit(0);
        }
        int lastx=x;
        int lasty=y;
        pos_robot_path[i].clear();
        pos_robot_path[i].push_back(make_pair(x,y));
    
        while(x!=bx||y!=by){
            int no_way=0;
            for(int k=0;k<4;k++){
                int nx=x+dx[k];
                int ny=y+dy[k];
                int flag=0; // 不能走到别人的泊位点上
                for(int ii=0;ii<10;ii++){
                    if(nx==berth[ii].x && ny==berth[ii].y & ii!=robot[i].berth_id){
                        flag=1;
                        // if(MY_DEBUG)cerr<<"robot "<<i<<" get on the berth "<<ii<<endl;
                    }
                }
                // if(MY_DEBUG)cerr<<shortest_dist_to_berth[id][nx][ny]<<"  "<< dist << "  " << (nx>=0 && nx<n && ny>=0 && ny<n) << (nx!=lastx && ny!=lasty)<<endl;
                if(nx>=0 && nx<n && ny>=0 && ny<n &&shortest_dist_to_berth[id][nx][ny]<dist && flag==0 && (nx!=lastx || ny!=lasty)){
                    // if(MY_DEBUG)cerr<<"new step"<<endl;
                    robot_path[i].push_back(k);
                    pos_robot_path[i].push_back(make_pair(x,y));
                    lastx=x;
                    lasty=y;
                    x=nx;
                    y=ny;
                    dist--;
                    break;
                }
                if(k==3){
                    if(MY_DEBUG)cerr<<"no way to berth "<<id<<", robot "<<i<<endl;
                    int move = ReverseRobotMove(static_cast<RobotMove>(robot_path[i].back()));
                    robot_path[i].push_back(move);
                    lastx=x;
                    lasty=y;
                    x=x+dx[move];
                    y=y+dy[move];
                    pos_robot_path[i].push_back(make_pair(x,y));
                    dist++;
                }
            }
        }
    }

    //robot path conflicts detection
    for(int i=0;i<10;i++){
        if(robot_good[i]==false)continue;
        robot[i].start_delay=0;
        int conflict_flag=0;
        vector<int> conflict_id;
        for(int j=0;j<i;j++){
            if(robot_good[j]==false)continue;
            int t=0;
            while(t+robot[j].start_delay+1<pos_robot_path[i].size()&&t<pos_robot_path[j].size()&&
                diff_pair(pos_robot_path[i][t+robot[j].start_delay],pos_robot_path[j][t]) &&
                diff_pair(pos_robot_path[i][t+robot[j].start_delay+1],pos_robot_path[j][t]) ){
                t++;
            }
            if(t+robot[j].start_delay<pos_robot_path[i].size()-1&&t<pos_robot_path[j].size()){
                conflict_flag=1;
                conflict_id.push_back(j);
            }
        }
        if(MY_DEBUG)cerr<<"conflict flag: "<<conflict_flag<<endl;
        if(conflict_flag==1){
            int start_delay=MAX_DIST;
            for(int k=0;k<conflict_id.size();k++){
                if(start_delay>(robot[conflict_id[k]].start_delay+pos_robot_path[conflict_id[k]].size())){
                    start_delay=robot[conflict_id[k]].start_delay+pos_robot_path[conflict_id[k]].size();
                }
            }
            robot[i].start_delay=start_delay;
        }
    }
    if(MY_DEBUG){
        for(int i=0;i<10;i++){
            cerr<<"robot "<<i<<" start delay: "<<robot[i].start_delay<<endl;
        }
    }

    if(MY_DEBUG)cerr<<"init done"<<endl;
    if(see_log){
        ofstream out("../LinuxRelease/log/berth_goods.txt");
        out<<"init   ";
        for(int i=0;i<berth_num;i++){
            out<<berth[i].goods<<'\t';
        }
        out<<endl;
    }

    // boat init

    printf("OK\n");
    fflush(stdout);
}

void Plan::InitPlanB()
{
    for(int i = 0; i < n; i ++)
        scanf("%s", ch[i]);
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    if(MY_DEBUG)cerr<<"init start"<<endl;
    // get the position of robots
    int tid=0;
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
        {
            if(ch[i][j] == 'A')
            {
                robot[tid].x = i;
                robot[tid].y = j;
                tid++;
            }
        }
    // initialize gds;
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
        {   
            gds[i][j] = 0;
            gds_time[i][j] = 0;
        }

    // assign berth to boat
    for(int i = 0; i < 5; i ++)
    {
        boat[i].status = 0;
        boat[i].num = 0;
        boat[i].goods = 0;
        boat[i].berth_id = -1;
        boat[i].timer_wait = WAIT_TIME;
    }

    if(see_log){
        ofstream out("../LinuxRelease/log/ch.txt");
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                out<<ch[i][j];
            }
            out<<endl;
        }
        out.close();
    }
    DivideParts();

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(parts[i][j]!=-1){
                berth_region[parts[i][j]].push_back(make_pair(i,j));
            }
        }
    }

    if(MY_DEBUG)cerr<<"berth region assigned"<<endl;

    int flag[10];
    for(int i=0;i<10;i++)flag[i]=0;
    for(int i=0;i<10;i++){robot_good[i]=false;}
    int shortest_dist_to_robots[10][210][210];
    short temp_map[210][210];
    copy_map(map, temp_map);
    for(int i=0;i<10;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                shortest_dist_to_robots[i][j][k]=MAX_DIST;
            }
        }
        for(int j=0;j<i;j++){
            for(auto p:pos_robot_path[j]){
                temp_map[p.first][p.second]=0;
            }
        }
        shortest_dist_to_robots[i][robot[i].x][robot[i].y]=0;
        std::queue<std::pair<int,int>> q;
        q.push({robot[i].x,robot[i].y});
        pos_robot_path[i].clear();
        pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
        while(!q.empty()){
            auto [x,y]=q.front();
            q.pop();
            for(int k=0;k<4;k++){
                int nx=x+dx[k];
                int ny=y+dy[k];
                if(nx>=0 && nx<n && ny>=0 && ny<n && temp_map[nx][ny]==1 && shortest_dist_to_robots[i][nx][ny]>shortest_dist_to_robots[i][x][y]+1){
                    shortest_dist_to_robots[i][nx][ny]=shortest_dist_to_robots[i][x][y]+1;
                    q.push({nx,ny});
                    pos_robot_path[i].push_back(make_pair(nx,ny));
                }
            }
        }
        int nearest_berth = -1;
        int min_dist = MAX_DIST;
        for(int j=0;j<10;j++){
            if(shortest_dist_to_robots[i][berth[j].x][berth[j].y]<min_dist && flag[j]==0){
                min_dist=shortest_dist_to_robots[i][berth[j].x][berth[j].y];
                nearest_berth=j;
            }
        }
        if(nearest_berth!=-1){
            robot[i].berth_id=nearest_berth;
            flag[nearest_berth]++;
            berth[nearest_berth].robot_id=i;
            robot_good[i]=true;
        }
        else{
            robot[i].berth_id=-1;
            robot_good[i]=false;
        }
    }

    if(MY_DEBUG)cerr<<"robot assigned to berth"<<endl;

    // plan the path for each robot
    for(int i=0;i<robot_num;i++){
        if(robot_good[i]==false)continue;
        if(MY_DEBUG)cerr<<"finding the path for robot "<<i<<endl;
        int x=robot[i].x;
        int y=robot[i].y;
        int id=robot[i].berth_id;
        int bx=berth[id].x;
        int by=berth[id].y;
        int dist=shortest_dist_to_berth[id][x][y];
        robot[i].status=NotReady;
        robot[i].sys_status=1;
        if(dist==MAX_DIST){
            if(MY_DEBUG)cerr<< "robot" <<i<<" cannot reach berth "<<id<<", robot coordinates: "<<x<<" "<<y<<endl;
            // exit(0);
        }
        int lastx=x;
        int lasty=y;
        while(x!=bx||y!=by){
            int no_way=0;
            for(int k=0;k<4;k++){
                int nx=x+dx[k];
                int ny=y+dy[k];
                int flag=0; // 不能走到别人的泊位点上
                for(int ii=0;ii<10;ii++){
                    if(nx==berth[ii].x && ny==berth[ii].y & ii!=robot[i].berth_id){
                        flag=1;
                        // if(MY_DEBUG)cerr<<"robot "<<i<<" get on the berth "<<ii<<endl;
                    }
                }
                // if(MY_DEBUG)cerr<<shortest_dist_to_berth[id][nx][ny]<<"  "<< dist << "  " << (nx>=0 && nx<n && ny>=0 && ny<n) << (nx!=lastx && ny!=lasty)<<endl;
                if(nx>=0 && nx<n && ny>=0 && ny<n &&shortest_dist_to_berth[id][nx][ny]<dist && flag==0 && (nx!=lastx || ny!=lasty)){
                    // if(MY_DEBUG)cerr<<"new step"<<endl;
                    robot_path[i].push_back(k);
                    pos_robot_path[i].push_back(make_pair(x,y));
                    lastx=x;
                    lasty=y;
                    x=nx;
                    y=ny;
                    dist--;
                    break;
                }
                if(k==3){
                    if(MY_DEBUG)cerr<<"no way to berth "<<id<<", robot "<<i<<endl;
                    int move = ReverseRobotMove(static_cast<RobotMove>(robot_path[i].back()));
                    robot_path[i].push_back(move);
                    lastx=x;
                    lasty=y;
                    x=x+dx[move];
                    y=y+dy[move];
                    pos_robot_path[i].push_back(make_pair(x,y));
                    dist++;
                }
            }
        }
    }

    for(int i=0;i<10;i++)robot[i].start_delay=0;

    if(MY_DEBUG){
        for(int i=0;i<10;i++){
            cerr<<"robot "<<i<<" start delay: "<<robot[i].start_delay<<endl;
        }
    }

    if(MY_DEBUG)cerr<<"init done"<<endl;
    if(see_log){
        ofstream out("../LinuxRelease/log/berth_goods.txt");
        out<<"init   ";
        for(int i=0;i<berth_num;i++){
            out<<berth[i].goods<<'\t';
        }
        out<<endl;
    }

    // boat init

    printf("OK\n");
    fflush(stdout);
}

int Plan::Input()
{
    scanf("%d%d", &fid, &money);
    int num;
    scanf("%d", &num);
    for(int i = 0; i < num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        gds[x][y] = val;
        gds_time[x][y]=fid;
    }
    for(int i = 0; i < robot_num; i ++)
    {
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].sys_status);
    }
    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].berth_id);
    char okk[100];
    scanf("%s", okk);
    return fid;
}

void Plan::Output()
{
    for(int i=0;i<robot_num;i++){
        if(robot_good[i]==false)continue;
        if(robot[i].action_before_move==GET){
            printf("get %d\n", i);
        }
        if(robot[i].action_before_move==PULL){
            printf("pull %d\n", i);
        }
        if(robot[i].action_move!=STAND)
            printf("move %d %d\n", i, robot[i].action_move);
    }
    for(int i=0;i<5;i++){
        if(MY_DEBUG)cerr<<"boat "<<i<<" action: "<<boat[i].action<<' '<<boat[i].berth_id<<endl;
        if(boat[i].action==SHIP){
            printf("ship %d %d\n", i, boat[i].berth_id);
        }
        if(boat[i].action==GO){
            printf("go %d\n", i);
        }
    }
}

void Plan::SaveLog(){
    // output berth log
    {
        ofstream out;
        if(fid==1)
            out.open("../LinuxRelease/log/berth_log.txt",ios::out);
        else
            out.open("../LinuxRelease/log/berth_log.txt",ios::app);
        for(int i=0;i<berth_num;i++){
            out<<i<<' '<<berth[i].goods<<' '<<berth[i].boat_id<<' ';
            if(berth[i].fetch_time.size()!=0){
                out<<accumulate(berth[i].fetch_time.begin(),berth[i].fetch_time.end(),0.0)/berth[i].fetch_time.size()<<' ';
            }
            else out<<10000<<' ';
            out<<accumulate(berth[i].goods_val_lst.begin(),berth[i].goods_val_lst.end(),0.0)<<endl;
        }
    }

    // output boat log
    {
        ofstream out;
        if(fid==1)
            out.open("../LinuxRelease/log/boat_log.txt",ios::out);
        else
            out.open("../LinuxRelease/log/boat_log.txt",ios::app);
        for(int i=0;i<5;i++){
            out<<i<<' '<<boat[i].goods<<' '<<boat[i].berth_id<<' '<<boat[i].status<<' '<<boat[i].timer_wait<<' '<<boat[i].action<<endl;
        }
    }
}

void Plan::RobotDo()
{
    int robot_not_ready=0;
    std::set<short> robot_init_influenced_berth;
    std::set<pair<int,int>> robot_init_influenced_pos;
    if(fid<1000)    // 只有在前面的时间段里面才关心机器人NotReady的情况
        for(int i=0;i<robot_num;i++){
            if(robot_good[i]==false)continue;
            robot_not_ready += robot[i].status==RobotStatus::NotReady;
            if(robot[i].status==NotReady){
                for(auto p:pos_robot_path[i]){
                    robot_init_influenced_berth.insert(parts[p.first][p.second]);
                    robot_init_influenced_pos.insert(p);
                }
            }
        }
    if(MY_DEBUG)cerr<<"robot not ready: "<<robot_not_ready<<endl;
    for(int i=0;i<robot_num;i++){
        if(robot_good[i]==false)continue;   // 是被抛弃的机器人
        if(MY_DEBUG)cerr<<"robot "<<i<<" status: "<<robot[i].status<<endl;
        if(robot[i].start_delay>0){
            robot[i].status=NotReady;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            robot[i].start_delay--;
            continue;
        }
        if(robot[i].sys_status==0){
            if(robot[i].action_move!=RobotMove::STAND)robot_path[i].push_front(robot[i].action_move); // 先加上恢复状态保证程序不崩溃
            robot[i].status=RobotStatus::Collision;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            cerr<<"robot "<<i<<" is in collision, its (gx,gy)= "<< robot[i].gx<<", "<<robot[i].gy<<", its position= <"<<robot[i].x<<","<<robot[i].y<<">"<<endl;
            cerr<<"it's path is: ";
            for(int j=0;j<pos_robot_path[i].size();j++){
                cerr<<"("<<pos_robot_path[i][j].first<<','<<pos_robot_path[i][j].second<<") ";
            }
            cerr<<endl;
            if(parts[robot[i].x][robot[i].y]==robot[i].berth_id){
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
            }
            else{
                short temp_map[210][210];
                copy_map(map, temp_map);
                for(int j=0;j<10;j++){
                    for(auto p:pos_robot_path[j]){
                        temp_map[p.first][p.second]=0;
                    }
                } 
                UpdateShortestDistToBerth(temp_map,robot[i].berth_id);
                
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                UpdateShortestDistToBerth(map,robot[i].berth_id);
            }
            continue;
        }
        robot[i].action_move=STAND;
        robot[i].action_before_move=RA_NOTHING;
        robot[i].action_after_move=RA_NOTHING;
        auto it = robot_init_influenced_berth.find(robot[i].berth_id);
        if (robot[i].status==NotReady){
            // 还没初始化完成
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
            }
            else{
                robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                robot_path[i].pop_front();
                pos_robot_path[i].erase(pos_robot_path[i].begin());
            }
        }
        // else if (robot[i].status==ReturnToBerth && it==robot_init_influenced_berth.end()){
        else if (robot[i].status==ReturnToBerth){
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                if(robot[i].goods==1){
                    robot[i].action_before_move=PULL;
                    berth[robot[i].berth_id].goods++;
                    berth[robot[i].berth_id].goods_val_lst.push_back(robot[i].goods_val);
                    berth[robot[i].berth_id].fetch_time.push_back(pos_robot_path[i].size());
                    fetch_num++;
                }
                if(MY_DEBUG)cerr<<"planing the path for robot "<<i<<endl;
                // 在自己的地盘寻找目标货物
                int dist=MAX_DIST;
                float vvt = 0;
                for(int rx=0;rx<n;rx++)
                    for(int ry=0;ry<n;ry++){
                        // nvvt = 5.0*((1-fid/15000)/2+0.5)*gds[x][y]/(1.0*shortest_dist_to_berth[berth_id][x][y]);
                        if(parts[rx][ry]==robot[i].berth_id){
                            float nvvt;
                            nvvt = (gds[rx][ry]+1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry])/(1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry]);
                            if(gds[rx][ry]>0 && gds_time[rx][ry]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][rx][ry] && nvvt>vvt){
                                robot[i].gx=rx;
                                robot[i].gy=ry;
                                dist = shortest_dist_to_berth[robot[i].berth_id][rx][ry];
                                vvt = nvvt;
                            }
                        }
                }
                if(dist!=MAX_DIST){
                    if(MY_DEBUG)cerr<<"robot "<<i<<" going to goods: "<<robot[i].gx<<' '<<robot[i].gy<<", dist= "<<dist<<endl;
                    robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                    gds[robot[i].gx][robot[i].gy]=0;
                    robot_path[i].clear();
                    pos_robot_path[i].clear();
                    robot[i].status=GoingToGood;
                    // fetch_time.push_back(dist);
                    // plan the path for each robot
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].gx,robot[i].gy});
                    pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                    while(!q.empty()){
                        auto [x,y]=q.front();
                        q.pop_front();
                        if(x==robot[i].x&&y==robot[i].y)break;
                        for(int k=0;k<4;k++){
                            int nx=x+dx[k];
                            int ny=y+dy[k];
                            if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&
                                shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]
                                &&parts[nx][ny]==robot[i].berth_id)
                            {
                                q.push_back({nx,ny});
                                robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                pos_robot_path[i].push_back(make_pair(nx,ny));
                                break;
                            }
                        }
                    }
                    
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                    
                }
                else if(fid>400)
                { 
                    // 自己地盘没货了，出去找货
                    short temp_map[210][210];
                    copy_map(map,temp_map);
                    for(int ii=0;ii<robot_num;ii++){
                        if(robot_good[ii]==false || ii==i)continue;
                        for(int jj=0;jj<pos_robot_path[ii].size();jj++){
                            temp_map[pos_robot_path[ii][jj].first][pos_robot_path[ii][jj].second]=0;
                        }
                    }
                    // update shortest_dist_to_berth[i] using temp_map
                    UpdateShortestDistToBerth(temp_map, robot[i].berth_id);
                    int gx,gy;
                    float vvt=-1;
                    for(int ii=0;ii<n;ii++){
                        for(int jj=0;jj<n;jj++){
                            if(shortest_dist_to_berth[robot[i].berth_id][ii][jj]!=MAX_DIST && gds[ii][jj]>0 && 
                                gds_time[ii][jj]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][ii][jj] && temp_map[ii][jj]==1)
                            {
                                float nvvt = 1.0/(1.0*shortest_dist_to_berth[robot[i].berth_id][ii][jj]);// 找最近
                                if(nvvt>vvt){
                                    vvt=nvvt;
                                    gx=ii;
                                    gy=jj;
                                }
                            }
                        }
                    }
                    if(vvt==-1){
                        // 即使找最近的货物也找不到
                        // cerr<<"i am going out because i have no goods, but i didn't find."<<endl;
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].gx=gx;
                        robot[i].gy=gy;
                        robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                        gds[robot[i].gx][robot[i].gy]=0;
                        robot_path[i].clear();
                        pos_robot_path[i].clear();
                        robot[i].status=GoingToGood;
                        fetch_time.push_back(shortest_dist_to_berth[robot[i].berth_id][gx][gy]);
                        deque<pair<int,int>> q;
                        q.push_back({robot[i].gx,robot[i].gy});
                        pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                        while(!q.empty()){
                            auto [x,y]=q.front();
                            q.pop_front();
                            if(x==robot[i].x&&y==robot[i].y)break;
                            for(int k=0;k<4;k++){
                                int nx=x+dx[k];
                                int ny=y+dy[k];
                                if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&
                                    shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                                    q.push_back({nx,ny});
                                    robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                    pos_robot_path[i].push_back(make_pair(nx,ny));
                                    break;
                                }
                            }
                        }
                        std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                        p.first+=dx[robot_path[i].front()];
                        p.second+=dy[robot_path[i].front()];
                        if(robot_init_influenced_pos.find(p)==robot_init_influenced_pos.end()){
                            robot[i].action_move=STAND;
                        }
                        else{
                            robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                            robot_path[i].pop_front();
                        }
                        // update parts and shortest_dist_to_berth
                        copy_map(map,temp_map);
                        set<short> influenced_parts_set;
                        for(auto p:pos_robot_path[i]){
                            if(parts[p.first][p.second]!=-1 && parts[p.first][p.second]!=robot[i].berth_id){
                                // influenced_parts.push_back(parts[p.first][p.second]);
                                influenced_parts_set.insert(parts[p.first][p.second]);
                            }
                            temp_map[p.first][p.second]=0;
                            parts[p.first][p.second]=robot[i].berth_id;
                        }
                        //cerr<<"influenced parts size: "<<influenced_parts_set.size()<<" at frame "<<fid<<endl;
                        for(auto p:influenced_parts_set){
                        //    cerr<<"influenced parts: "<<p<<endl;
                            UpdateShortestDistToBerthAndParts(p, robot[i].berth_id);
                            UpdateBerthRegion(p);
                        }
                        UpdateShortestDistToBerthAndParts(robot[i].berth_id, robot[i].berth_id);
                        UpdateBerthRegion(robot[i].berth_id);
                        // cerr<<"influenced parts done."<<endl;
                        // cerr<< "i am going out, because i have no goods"<<endl;
                    }
                }
            }
            else{
                if(robot_path[i].size()!=0){
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                }
            }
        }
        else if (robot[i].status==GoingToGood){
            if(robot[i].x==robot[i].gx && robot[i].y==robot[i].gy){
                // 取到货
                robot[i].status=ReturnToBerth;
                if(robot[i].goods==0){
                    robot[i].action_before_move=GET;
                    gds[robot[i].gx][robot[i].gy]=0;
                    gds_time[robot[i].gx][robot[i].gy]=0;
                }
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
                if(MY_DEBUG)cerr<<"robot "<<i<<" return path to berth: ";
                for(auto p:robot_path[i]){
                    if(MY_DEBUG)cerr<<p<<' ';
                }if(MY_DEBUG)cerr<<endl;
            }
            else{
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
            }
        }
        if(MY_DEBUG)cerr<<"robot "<<i<<" action: "<<robot[i].action_before_move<<' '<<robot[i].action_move<<' '<<robot[i].action_after_move<<endl;
    }
}

void Plan::RobotDoNoDelay(){
    for(int i=0;i<robot_num;i++){
        std::set<pair<int,int>> robot_init_influenced_pos;
        if(fid<1000)    // 只有在前面的时间段里面才关心机器人NotReady的情况
            for(int ii=0;ii<robot_num;ii++){
                if(ii==i)continue;
                if(robot_good[ii]==false)robot_init_influenced_pos.insert(make_pair(robot[ii].x,robot[ii].y));
                if(robot[ii].status==NotReady){
                    for(auto p:pos_robot_path[ii]){
                        robot_init_influenced_pos.insert(p);
                    }
                }
            }
        if(robot_good[i]==false)continue;   // 是被抛弃的机器人
        if(MY_DEBUG)cerr<<"robot "<<i<<" status: "<<robot[i].status<<endl;
        if(robot[i].start_delay>0){
            robot[i].status=NotReady;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            robot[i].start_delay--;
            continue;
        }
        if(robot[i].sys_status==0){
            // if(robot[i].action_move!=RobotMove::STAND)robot_path[i].push_front(robot[i].action_move); // 先加上恢复状态保证程序不崩溃

            cerr<<"robot "<<i<<" is in collision, its (gx,gy)= "<< robot[i].gx<<", "<<robot[i].gy<<", its position= <"<<robot[i].x<<","<<robot[i].y<<">"<<endl;
            cerr<<"it's path is: ";
            for(int j=0;j<pos_robot_path[i].size();j++){
                cerr<<"("<<pos_robot_path[i][j].first<<','<<pos_robot_path[i][j].second<<") ";
            }
            cerr<<endl;
            cerr<<"its robot_path size is: "<<robot_path[i].size()<<endl;
            cerr<<"its status is: "<<robot[i].status<<endl;
            cerr<<"its berth id is: "<<robot[i].berth_id<<endl;

            robot[i].status=RobotStatus::Collision;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            if(parts[robot[i].x][robot[i].y]==robot[i].berth_id){
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
            }
            else{
                short temp_map[210][210];
                copy_map(map, temp_map);
                for(int j=0;j<10;j++){
                    for(auto p:pos_robot_path[j]){
                        temp_map[p.first][p.second]=0;
                    }
                } 
                UpdateShortestDistToBerth(temp_map,robot[i].berth_id);
                
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                UpdateShortestDistToBerth(map,robot[i].berth_id);
            }
            continue;
        }
        robot[i].action_move=STAND;
        robot[i].action_before_move=RA_NOTHING;
        robot[i].action_after_move=RA_NOTHING;
        if (robot[i].status==NotReady){
            // 还没初始化完成
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
            }
            else{
                robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                robot_path[i].pop_front();
                pos_robot_path[i].erase(pos_robot_path[i].begin());
            }
        }
        // else if (robot[i].status==ReturnToBerth && it==robot_init_influenced_berth.end()){
        else if (robot[i].status==ReturnToBerth){
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                if(robot[i].goods==1){
                    robot[i].action_before_move=PULL;
                    berth[robot[i].berth_id].goods++;
                    berth[robot[i].berth_id].goods_val_lst.push_back(robot[i].goods_val);
                    berth[robot[i].berth_id].fetch_time.push_back(pos_robot_path[i].size());
                    fetch_num++;
                }
                if(MY_DEBUG)cerr<<"planing the path for robot "<<i<<endl;
                // 在自己的地盘寻找目标货物
                int dist=MAX_DIST;
                float vvt = 0;
                for(int rx=0;rx<n;rx++)
                    for(int ry=0;ry<n;ry++){
                        // nvvt = 5.0*((1-fid/15000)/2+0.5)*gds[x][y]/(1.0*shortest_dist_to_berth[berth_id][x][y]);
                        if(parts[rx][ry]==robot[i].berth_id){
                            float nvvt;
                            nvvt = (gds[rx][ry]+1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry])/(1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry]);
                            if(gds[rx][ry]>0 && gds_time[rx][ry]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][rx][ry] && nvvt>vvt){
                                robot[i].gx=rx;
                                robot[i].gy=ry;
                                dist = shortest_dist_to_berth[robot[i].berth_id][rx][ry];
                                vvt = nvvt;
                            }
                        }
                }
                if(dist!=MAX_DIST){
                    if(MY_DEBUG)cerr<<"robot "<<i<<" going to goods: "<<robot[i].gx<<' '<<robot[i].gy<<", dist= "<<dist<<endl;
                    robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                    gds[robot[i].gx][robot[i].gy]=0;
                    robot_path[i].clear();
                    pos_robot_path[i].clear();
                    robot[i].status=GoingToGood;
                    // fetch_time.push_back(dist);
                    // plan the path for each robot
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].gx,robot[i].gy});
                    pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                    while(!q.empty()){
                        auto [x,y]=q.front();
                        q.pop_front();
                        if(x==robot[i].x&&y==robot[i].y)break;
                        for(int k=0;k<4;k++){
                            int nx=x+dx[k];
                            int ny=y+dy[k];
                            if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&
                                shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]
                                &&parts[nx][ny]==robot[i].berth_id)
                            {
                                q.push_back({nx,ny});
                                robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                pos_robot_path[i].push_back(make_pair(nx,ny));
                                break;
                            }
                        }
                    }
                    
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                    
                }
                else if(fid>400)
                { 
                    // 自己地盘没货了，出去找货
                    short temp_map[210][210];
                    copy_map(map,temp_map);
                    for(int ii=0;ii<robot_num;ii++){
                        if(robot_good[ii]==false || ii==i)continue;
                        for(int jj=0;jj<pos_robot_path[ii].size();jj++){
                            temp_map[pos_robot_path[ii][jj].first][pos_robot_path[ii][jj].second]=0;
                        }
                    }
                    // update shortest_dist_to_berth[i] using temp_map
                    UpdateShortestDistToBerth(temp_map, robot[i].berth_id);
                    int gx,gy;
                    float vvt=-1;
                    for(int ii=0;ii<n;ii++){
                        for(int jj=0;jj<n;jj++){
                            if(shortest_dist_to_berth[robot[i].berth_id][ii][jj]!=MAX_DIST && gds[ii][jj]>0 && 
                                gds_time[ii][jj]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][ii][jj] && temp_map[ii][jj]==1)
                            {
                                float nvvt = 1.0/(1.0*shortest_dist_to_berth[robot[i].berth_id][ii][jj]);// 找最近
                                if(nvvt>vvt){
                                    vvt=nvvt;
                                    gx=ii;
                                    gy=jj;
                                }
                            }
                        }
                    }
                    if(vvt==-1){
                        // 即使找最近的货物也找不到
                        // cerr<<"i am going out because i have no goods, but i didn't find."<<endl;
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].gx=gx;
                        robot[i].gy=gy;
                        robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                        gds[robot[i].gx][robot[i].gy]=0;
                        robot_path[i].clear();
                        pos_robot_path[i].clear();
                        robot[i].status=GoingToGood;
                        fetch_time.push_back(shortest_dist_to_berth[robot[i].berth_id][gx][gy]);
                        deque<pair<int,int>> q;
                        q.push_back({robot[i].gx,robot[i].gy});
                        pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                        while(!q.empty()){
                            auto [x,y]=q.front();
                            q.pop_front();
                            if(x==robot[i].x&&y==robot[i].y)break;
                            for(int k=0;k<4;k++){
                                int nx=x+dx[k];
                                int ny=y+dy[k];
                                if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&
                                    shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                                    q.push_back({nx,ny});
                                    robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                    pos_robot_path[i].push_back(make_pair(nx,ny));
                                    break;
                                }
                            }
                        }
                        std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                        p.first+=dx[robot_path[i].front()];
                        p.second+=dy[robot_path[i].front()];
                        if(robot_init_influenced_pos.find(p)==robot_init_influenced_pos.end()){
                            robot[i].action_move=STAND;
                        }
                        else{
                            robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                            robot_path[i].pop_front();
                        }
                        // update parts and shortest_dist_to_berth
                        copy_map(map,temp_map);
                        set<short> influenced_parts_set;
                        for(auto p:pos_robot_path[i]){
                            if(parts[p.first][p.second]!=-1 && parts[p.first][p.second]!=robot[i].berth_id){
                                // influenced_parts.push_back(parts[p.first][p.second]);
                                influenced_parts_set.insert(parts[p.first][p.second]);
                            }
                            temp_map[p.first][p.second]=0;
                            parts[p.first][p.second]=robot[i].berth_id;
                        }
                        //cerr<<"influenced parts size: "<<influenced_parts_set.size()<<" at frame "<<fid<<endl;
                        for(auto p:influenced_parts_set){
                        //    cerr<<"influenced parts: "<<p<<endl;
                            UpdateShortestDistToBerthAndParts(p, robot[i].berth_id);
                            UpdateBerthRegion(p);
                        }
                        UpdateShortestDistToBerthAndParts(robot[i].berth_id, robot[i].berth_id);
                        UpdateBerthRegion(robot[i].berth_id);
                        // cerr<<"influenced parts done."<<endl;
                        // cerr<< "i am going out, because i have no goods"<<endl;
                    }
                }
            }
            else{
                if(robot_path[i].size()!=0){
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                }
            }
        }
        else if (robot[i].status==GoingToGood){
            if(robot[i].x==robot[i].gx && robot[i].y==robot[i].gy){
                // 取到货
                robot[i].status=ReturnToBerth;
                if(robot[i].goods==0){
                    robot[i].action_before_move=GET;
                    gds[robot[i].gx][robot[i].gy]=0;
                    gds_time[robot[i].gx][robot[i].gy]=0;
                }
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
                if(MY_DEBUG)cerr<<"robot "<<i<<" return path to berth: ";
                for(auto p:robot_path[i]){
                    if(MY_DEBUG)cerr<<p<<' ';
                }if(MY_DEBUG)cerr<<endl;
            }
            else{
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
            }
        }
        if(MY_DEBUG)cerr<<"robot "<<i<<" action: "<<robot[i].action_before_move<<' '<<robot[i].action_move<<' '<<robot[i].action_after_move<<endl;
    }
}

void Plan::RobotDoNoDelayFlush(){
    // 在RobotDoNoDelay的基础上，加上地图分区的刷新
    for(int i=0;i<robot_num;i++){
        robot[i].is_out = false;
        for(auto p:pos_robot_path[i]){
            if(robot_good[i]==true and original_parts[p.first][p.second]!=robot[i].berth_id){
                robot[i].is_out = true;
                break;
            }
        }
    }
    int out_num = 0;
    for(int i=0;i<robot_num;i++){
        if(robot[i].is_out==true)out_num++;
    }
    if(out_num==0 || fid%2000==0){
        // cerr<<"flush at frame "<<fid<<endl;
        // restore original parts and shortest_dist_to_berth
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                parts[i][j]=original_parts[i][j];
            }
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    shortest_dist_to_berth[i][j][k]=original_shortest_dist_to_berth[i][j][k];
                }
            }
        }
    }

    for(int i=0;i<robot_num;i++){
        if(robot_good[i]==false)continue;   // 是被抛弃的机器人
        if(robot[i].sys_status==0){
            pos_robot_path[i].clear();
        }
    }

    for(int i=0;i<robot_num;i++){
        std::set<pair<int,int>> robot_init_influenced_pos;
        if(fid<1000)    // 只有在前面的时间段里面才关心机器人NotReady的情况
            for(int ii=0;ii<robot_num;ii++){
                if(ii==i)continue;
                if(robot_good[ii]==false)robot_init_influenced_pos.insert(make_pair(robot[ii].x,robot[ii].y));
                if(robot[ii].status==NotReady || robot[ii].status==Collision){
                    for(auto p:pos_robot_path[ii]){
                        robot_init_influenced_pos.insert(p);
                    }
                }
            }
        if(robot_good[i]==false)continue;   // 是被抛弃的机器人
        if(MY_DEBUG)cerr<<"robot "<<i<<" status: "<<robot[i].status<<endl;
        if(robot[i].start_delay>0){
            robot[i].status=NotReady;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            robot[i].start_delay--;
            continue;
        }
        if(robot[i].sys_status==0){ // 恢复冲突状态
            // cerr<<"robot "<<i<<" is in collision, its (gx,gy)= "<< robot[i].gx<<", "<<robot[i].gy<<", its position= <"<<robot[i].x<<","<<robot[i].y<<">"<<endl;
            // cerr<<"its berth position is: <"<<berth[robot[i].berth_id].x<<","<<berth[robot[i].berth_id].y<<">"<<endl;
            // cerr<<"its path is: ";
            // for(int j=0;j<pos_robot_path[i].size();j++){
            //     cerr<<"("<<pos_robot_path[i][j].first<<','<<pos_robot_path[i][j].second<<") ";
            // }
            // cerr<<endl;
            // cerr<< "its move action sequence size is: "<<robot_path[i].size();
            // // cerr<<"it's move action sequence is: ";
            // // for(int j=0;j<robot_path[i].size();j++){
            // //     cerr<<robot_path[i][j]<<' ';
            // // }
            // cerr<<endl;

            // if(robot[i].action_move!=RobotMove::STAND)robot_path[i].push_front(robot[i].action_move); // 先加上恢复状态保证程序不崩溃
            robot[i].status=RobotStatus::Collision;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;

            // if(original_parts[robot[i].x][robot[i].y]==robot[i].berth_id){
            //     // 在自己的区域，直接返回
            //     cerr<<"在自己区域，直接返回"<<endl;
            //     robot[i].status=ReturnToBerth;
            //     robot[i].action_move=STAND;
            //     robot_path[i].clear();
            //     pos_robot_path[i].clear();
            //     int dist=MAX_DIST;
            //     robot[i].gx = berth[robot[i].berth_id].x;
            //     robot[i].gy = berth[robot[i].berth_id].y;
            //     robot_path[i].clear();
            //     pos_robot_path[i].clear();
            //     deque<pair<int,int>> q;
            //     q.push_back({robot[i].x,robot[i].y});
            //     pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
            //     while(!q.empty()){
            //         auto [x,y]=q.front();
            //         q.pop_front();
            //         // if(x==robot[i].x&&y==robot[i].y)break;
            //         for(int k=0;k<4;k++){
            //             int nx=x+dx[k];
            //             int ny=y+dy[k];
            //             if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&original_shortest_dist_to_berth[robot[i].berth_id][nx][ny]<original_shortest_dist_to_berth[robot[i].berth_id][x][y]){
            //                 q.push_back({nx,ny});
            //                 robot_path[i].push_back(static_cast<RobotMove>(k));
            //                 pos_robot_path[i].push_back(make_pair(nx,ny));
            //                 break;
            //             }
            //         }
            //     };
            // }
            // else{
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                short temp_map[210][210];
                copy_map(map, temp_map);
                for(int j=0;j<10;j++){
                    if(j!=i)for(auto p:pos_robot_path[j]){
                        temp_map[p.first][p.second]=0;
                    }
                    if(robot[j].sys_status==0 && j!=i){
                        temp_map[robot[j].x][robot[j].y]=0;
                    }
                } 
                UpdateShortestDistToBerth(temp_map,robot[i].berth_id);

                if(shortest_dist_to_berth[robot[i].berth_id][robot[i].x][robot[i].y]!=MAX_DIST){
                    robot[i].gx = berth[robot[i].berth_id].x;
                    robot[i].gy = berth[robot[i].berth_id].y;
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].x,robot[i].y});
                    pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                    int t=0;
                    while(!q.empty() && t<1000){
                        auto [x,y]=q.front();
                        q.pop_front();
                        t++;
                        if(x==robot[i].gx&&y==robot[i].gy)break;
                        for(int k=0;k<4;k++){
                            int nx=x+dx[k];
                            int ny=y+dy[k];
                            if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                                q.push_back({nx,ny});
                                robot_path[i].push_back(static_cast<RobotMove>(k));
                                pos_robot_path[i].push_back(make_pair(nx,ny));
                                break;
                            }
                        }
                    }
                }
                UpdateShortestDistToBerth(map,robot[i].berth_id);

            // }
            continue;
        }
        robot[i].action_move=STAND;
        robot[i].action_before_move=RA_NOTHING;
        robot[i].action_after_move=RA_NOTHING;
        if (robot[i].status==NotReady){
            // 还没初始化完成
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                robot[i].status=ReturnToBerth;
                robot[i].action_move=STAND;
                robot_path[i].clear();
                pos_robot_path[i].clear();
            }
            else{
                robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                robot_path[i].pop_front();
                pos_robot_path[i].erase(pos_robot_path[i].begin());
            }
        }
        // else if (robot[i].status==ReturnToBerth && it==robot_init_influenced_berth.end()){
        else if (robot[i].status==ReturnToBerth){
            if(robot[i].x==berth[robot[i].berth_id].x && robot[i].y==berth[robot[i].berth_id].y){
                if(robot[i].goods==1){
                    robot[i].action_before_move=PULL;
                    berth[robot[i].berth_id].goods++;
                    berth[robot[i].berth_id].goods_val_lst.push_back(robot[i].goods_val);
                    berth[robot[i].berth_id].fetch_time.push_back(pos_robot_path[i].size());
                    fetch_num++;
                }
                if(MY_DEBUG)cerr<<"planing the path for robot "<<i<<endl;
                // 在自己的地盘寻找目标货物
                int dist=MAX_DIST;
                float vvt = 0;
                for(int rx=0;rx<n;rx++)
                    for(int ry=0;ry<n;ry++){
                        // nvvt = 5.0*((1-fid/15000)/2+0.5)*gds[x][y]/(1.0*shortest_dist_to_berth[berth_id][x][y]);
                        if(parts[rx][ry]==robot[i].berth_id){
                            float nvvt;
                            nvvt = (gds[rx][ry]+1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry])/(1.0*shortest_dist_to_berth[robot[i].berth_id][rx][ry]);
                            if(gds[rx][ry]>0 && gds_time[rx][ry]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][rx][ry] && nvvt>vvt){
                                robot[i].gx=rx;
                                robot[i].gy=ry;
                                dist = shortest_dist_to_berth[robot[i].berth_id][rx][ry];
                                vvt = nvvt;
                            }
                        }
                }
                if(dist!=MAX_DIST){
                    if(MY_DEBUG)cerr<<"robot "<<i<<" going to goods: "<<robot[i].gx<<' '<<robot[i].gy<<", dist= "<<dist<<endl;
                    robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                    gds[robot[i].gx][robot[i].gy]=0;
                    robot_path[i].clear();
                    pos_robot_path[i].clear();
                    robot[i].status=GoingToGood;
                    // fetch_time.push_back(dist);
                    // plan the path for each robot
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].gx,robot[i].gy});
                    pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                    while(!q.empty()){
                        auto [x,y]=q.front();
                        q.pop_front();
                        if(x==robot[i].x&&y==robot[i].y)break;
                        for(int k=0;k<4;k++){
                            int nx=x+dx[k];
                            int ny=y+dy[k];
                            if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&
                                shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]
                                &&parts[nx][ny]==robot[i].berth_id)
                            {
                                q.push_back({nx,ny});
                                robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                pos_robot_path[i].push_back(make_pair(nx,ny));
                                break;
                            }
                        }
                    }
                    
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                    
                }
                else if(fid>400)
                { 
                    // 自己地盘没货了，出去找货
                    short temp_map[210][210];
                    copy_map(map,temp_map);
                    for(int ii=0;ii<robot_num;ii++){
                        if(robot_good[ii]==false || ii==i)continue;
                        for(int jj=0;jj<pos_robot_path[ii].size();jj++){
                            temp_map[pos_robot_path[ii][jj].first][pos_robot_path[ii][jj].second]=0;
                        }
                    }
                    // update shortest_dist_to_berth[i] using temp_map
                    UpdateShortestDistToBerth(temp_map, robot[i].berth_id);
                    int gx,gy;
                    float vvt=-1;
                    for(int ii=0;ii<n;ii++){
                        for(int jj=0;jj<n;jj++){
                            if(shortest_dist_to_berth[robot[i].berth_id][ii][jj]!=MAX_DIST && gds[ii][jj]>0 && 
                                gds_time[ii][jj]+1000>fid+shortest_dist_to_berth[robot[i].berth_id][ii][jj] && temp_map[ii][jj]==1)
                            {
                                float nvvt = 1.0/(1.0*shortest_dist_to_berth[robot[i].berth_id][ii][jj]);// 找最近
                                if(nvvt>vvt){
                                    vvt=nvvt;
                                    gx=ii;
                                    gy=jj;
                                }
                            }
                        }
                    }
                    if(vvt==-1){
                        // 即使找最近的货物也找不到
                        // cerr<<"i am going out because i have no goods, but i didn't find."<<endl;
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].gx=gx;
                        robot[i].gy=gy;
                        robot[i].goods_val = gds[robot[i].gx][robot[i].gy];
                        gds[robot[i].gx][robot[i].gy]=0;
                        robot_path[i].clear();
                        pos_robot_path[i].clear();
                        robot[i].status=GoingToGood;
                        fetch_time.push_back(shortest_dist_to_berth[robot[i].berth_id][gx][gy]);
                        deque<pair<int,int>> q;
                        q.push_back({robot[i].gx,robot[i].gy});
                        pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                        while(!q.empty()){
                            auto [x,y]=q.front();
                            q.pop_front();
                            if(x==robot[i].x&&y==robot[i].y)break;
                            for(int k=0;k<4;k++){
                                int nx=x+dx[k];
                                int ny=y+dy[k];
                                if(nx>=0&&nx<n&&ny>=0&&ny<n&&temp_map[nx][ny]==1&&
                                    shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                                    q.push_back({nx,ny});
                                    robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                    pos_robot_path[i].push_back(make_pair(nx,ny));
                                    break;
                                }
                            }
                        }
                        std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                        p.first+=dx[robot_path[i].front()];
                        p.second+=dy[robot_path[i].front()];
                        if(robot_init_influenced_pos.find(p)==robot_init_influenced_pos.end()){
                            robot[i].action_move=STAND;
                        }
                        else{
                            robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                            robot_path[i].pop_front();
                        }
                        // update parts and shortest_dist_to_berth
                        copy_map(map,temp_map);
                        set<short> influenced_parts_set;
                        for(auto p:pos_robot_path[i]){
                            if(parts[p.first][p.second]!=-1 && parts[p.first][p.second]!=robot[i].berth_id){
                                // influenced_parts.push_back(parts[p.first][p.second]);
                                influenced_parts_set.insert(parts[p.first][p.second]);
                            }
                            temp_map[p.first][p.second]=0;
                            parts[p.first][p.second]=robot[i].berth_id;
                        }
                        //cerr<<"influenced parts size: "<<influenced_parts_set.size()<<" at frame "<<fid<<endl;
                        for(auto p:influenced_parts_set){
                        //    cerr<<"influenced parts: "<<p<<endl;
                            UpdateShortestDistToBerthAndParts(p, robot[i].berth_id);
                            UpdateBerthRegion(p);
                        }
                        UpdateShortestDistToBerthAndParts(robot[i].berth_id, robot[i].berth_id);
                        UpdateBerthRegion(robot[i].berth_id);
                        // cerr<<"influenced parts done."<<endl;
                        // cerr<< "i am going out, because i have no goods"<<endl;
                    }
                }
            }
            else{
                if(robot_path[i].size()!=0){
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                    }
                }
            }
        }
        else if (robot[i].status==GoingToGood){
            if(robot[i].x==robot[i].gx && robot[i].y==robot[i].gy){
                // 取到货
                robot[i].status=ReturnToBerth;
                if(robot[i].goods==0){
                    robot[i].action_before_move=GET;
                    gds[robot[i].gx][robot[i].gy]=0;
                    gds_time[robot[i].gx][robot[i].gy]=0;
                }
                int dist=MAX_DIST;
                robot[i].gx = berth[robot[i].berth_id].x;
                robot[i].gy = berth[robot[i].berth_id].y;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[robot[i].berth_id][nx][ny]<shortest_dist_to_berth[robot[i].berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
                if(MY_DEBUG)cerr<<"robot "<<i<<" return path to berth: ";
                for(auto p:robot_path[i]){
                    if(MY_DEBUG)cerr<<p<<' ';
                }if(MY_DEBUG)cerr<<endl;
            }
            else{
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
            }
        }
        if(MY_DEBUG)cerr<<"robot "<<i<<" action: "<<robot[i].action_before_move<<' '<<robot[i].action_move<<' '<<robot[i].action_after_move<<endl;
    }
}

void Plan::RobotDoGlobal(){
    for(int i=0;i<robot_num;i++){
        std::set<pair<int,int>> robot_init_influenced_pos;
        for(int ii=0;ii<i;ii++){
            // if(robot_good[ii]==false)robot_init_influenced_pos.insert(make_pair(robot[ii].x,robot[ii].y));
            for(auto p:pos_robot_path[ii]){
                robot_init_influenced_pos.insert(p);
            }
        }

        if(robot_good[i]==false)continue;
        if(MY_DEBUG)cerr<<"robot "<<i<<" status: "<<robot[i].status<<endl;
        if(robot[i].start_delay>0){
            robot[i].status=NotReady;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            robot[i].start_delay--;
            continue;
        }
        if(robot[i].sys_status==0){
            if(robot[i].action_move!=RobotMove::STAND)robot_path[i].push_front(robot[i].action_move); // 先加上恢复状态保证程序不崩溃
            robot[i].status=NotReady;
            robot[i].action_before_move=RA_NOTHING;
            robot[i].action_move=STAND;
            robot[i].action_after_move=RA_NOTHING;
            continue;
        }
        int berth_id = robot[i].berth_id;
        int x=robot[i].x;
        int y=robot[i].y;
        robot[i].action_move=STAND;
        robot[i].action_before_move=RA_NOTHING;
        robot[i].action_after_move=RA_NOTHING;
        if (robot[i].status==ReturnToBerth || robot[i].status==ReadyToGo || robot[i].status==NotReady){
            if(x==berth[berth_id].x && y==berth[berth_id].y){
                robot[i].status=ReadyToGo;
                if(robot[i].goods==1){
                    robot[i].action_before_move=PULL;
                    berth[berth_id].goods++;
                    fetch_num++;
                }
                if(MY_DEBUG)cerr<<"planing the path for robot "<<i<<endl;
                // plan the path to the nearest goods
                int dist=MAX_DIST;
                float vvt = 0;
                for(int rx=0;rx<n;rx++)
                    for(int ry=0;ry<n;ry++){
                        float nvvt;
                        // nvvt = 5.0*((1-fid/15000)/2+0.5)*gds[x][y]/(1.0*shortest_dist_to_berth[berth_id][x][y]);
                        nvvt = (gds[rx][ry]+1.0*shortest_dist_to_berth[berth_id][rx][ry])/(1.0*shortest_dist_to_berth[berth_id][rx][ry]);
                        if(gds[rx][ry]>0 && gds_time[rx][ry]+1000>fid+shortest_dist_to_berth[berth_id][rx][ry] && nvvt>vvt){
                            // 这里的1000是货物存在的最大时间
                            robot[i].gx=rx;
                            robot[i].gy=ry;
                            dist = shortest_dist_to_berth[berth_id][rx][ry];
                            vvt = nvvt;
                        }
                }
                if(dist!=MAX_DIST){
                    if(MY_DEBUG)cerr<<"robot "<<i<<" going to goods: "<<robot[i].gx<<' '<<robot[i].gy<<", dist= "<<dist<<endl;
                    gds[robot[i].gx][robot[i].gy]=0;
                    robot_path[i].clear();
                    pos_robot_path[i].clear();
                    robot[i].status=GoingToGood;
                    fetch_time.push_back(dist);
                    // plan the path for each robot
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].gx,robot[i].gy});
                    pos_robot_path[i].push_back(make_pair(robot[i].gx,robot[i].gy));
                    while(!q.empty()){
                        auto [x,y]=q.front();
                        q.pop_front();
                        if(x==robot[i].x&&y==robot[i].y)break;
                        for(int k=0;k<4;k++){
                            int nx=x+dx[k];
                            int ny=y+dy[k];
                            if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[berth_id][nx][ny]<shortest_dist_to_berth[berth_id][x][y]){
                                q.push_back({nx,ny});
                                robot_path[i].push_front(ReverseRobotMove(static_cast<RobotMove>(k)));
                                pos_robot_path[i].insert(pos_robot_path[i].begin(), make_pair(nx,ny));
                                break;
                            }
                        }
                    }
                    std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                    p.first+=dx[robot_path[i].front()];
                    p.second+=dy[robot_path[i].front()];
                    if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                        robot[i].action_move=STAND;
                    }
                    else{
                        robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                        robot_path[i].pop_front();
                        pos_robot_path[i].erase(pos_robot_path[i].begin());
                    }
                }
            }
            else{
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else if (robot_path[i].size()!=0){
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                    pos_robot_path[i].erase(pos_robot_path[i].begin());
                }
            }
        }
        else if (robot[i].status==GoingToGood){
            if(x==robot[i].gx && y==robot[i].gy){
                robot[i].status=ReturnToBerth;
                if(robot[i].goods==0){
                    robot[i].action_before_move=GET;
                    gds[robot[i].gx][robot[i].gy]=0;
                    gds_time[robot[i].gx][robot[i].gy]=0;
                }
                int dist=MAX_DIST;
                int near_berth_id = -1;
                for(int ii=0;ii<10;ii++){
                    if(shortest_dist_to_berth[ii][x][y]<dist){
                        dist = shortest_dist_to_berth[berth_id][x][y];
                        near_berth_id = ii;
                    }
                }
                robot[i].gx = berth[near_berth_id].x;
                robot[i].gy = berth[near_berth_id].y;
                robot[i].berth_id = near_berth_id;
                robot_path[i].clear();
                pos_robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                pos_robot_path[i].push_back(make_pair(robot[i].x,robot[i].y));
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[near_berth_id][nx][ny]<shortest_dist_to_berth[near_berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            pos_robot_path[i].push_back(make_pair(nx,ny));
                            break;
                        }
                    }
                }
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else{
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                    pos_robot_path[i].erase(pos_robot_path[i].begin());
                }
            }
            else{
                std::pair<int,int> p = make_pair(robot[i].x,robot[i].y);
                p.first+=dx[robot_path[i].front()];
                p.second+=dy[robot_path[i].front()];
                if(robot_init_influenced_pos.find(p)!=robot_init_influenced_pos.end()){
                    robot[i].action_move=STAND;
                }
                else if (robot_path[i].size()!=0){
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                    pos_robot_path[i].erase(pos_robot_path[i].begin());
                }
            }
        }
        if(MY_DEBUG)cerr<<"robot "<<i<<" action: "<<robot[i].action_before_move<<' '<<robot[i].action_move<<' '<<robot[i].action_after_move<<endl;
    }
}

void Plan::BoatDoPlain()
{
    // define the action for boats
    /*
    boat_capacity: 船的最大货物容量
    boat[i]: 第i艘船
    boat[i].status: 船的状态，-1表示刚开始，0表示空闲，1表示装货或者已经装好货
    boat[i].berth_id: 船停靠或要停靠的泊位，-1为虚拟点
    boat[i].timer_wait: 船停靠没货可装时的等待时间，最大时间为WAIT_TIME
    boat[i].action: 船的动作，SHIP表示停靠，GO表示离开，BA_NOTHING表示无动作
    berth[i]: 第i个泊位, berth[i].goods表示泊位i的货物数量, berth[i].boat_id表示停靠在泊位i的船的id

    */

    if(fid==1){
        // 第一帧特殊处理
        for(int i=0;i<5;i++){
            boat[i].status=-1;
            boat[i].berth_id = i*2;
            berth[boat[i].berth_id].boat_id = i;
            boat[i].timer_wait = WAIT_TIME;
            boat[i].status = 0;
            boat[i].action = SHIP;
        }
        return;
    }
    for(int i=0;i<5;i++){
        boat[i].action = BA_NOTHING;
        if(boat[i].berth_id!=-1 && (15000-fid)<= berth[boat[i].berth_id].transport_time+1){
            boat[i].status = 0;
            boat[i].action = GO;
            berth[boat[i].berth_id].boat_id = -1;
            boat[i].berth_id = -1;
            continue;
        }
        if(boat[i].status==1)
        {
            if(boat[i].berth_id==-1){   // 刚送完货
                //cerr<<"boat "<<i<<" 刚送完货"<<endl;
                boat[i].action = SHIP;
                boat[i].status=0;
                boat[i].berth_id = i*2;
                berth[boat[i].berth_id].boat_id = i;
                boat[i].goods = 0;
                boat[i].timer_wait = WAIT_TIME;
            }
            else if(boat[i].berth_id%2==0){ // 在第一个泊位
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait--;
                if(berth[boat[i].berth_id+1].goods+boat[i].goods>boat_capacity*0.9 || 
                        (boat[i].goods>=boat_capacity*0.5 && berth[boat[i].berth_id].goods<berth[boat[i].berth_id+1].goods) 
                        || boat[i].timer_wait==0)
                {
                    if(MY_DEBUG)cerr<<"boat to go "<<i<<"  "<<(berth[boat[i].berth_id].goods==0) << (berth[boat[i].berth_id+1].goods>boat_capacity-boat[i].goods) << (boat[i].goods>=boat_capacity*0.3)<<endl;
                    //cerr<<"boat "<<i<<" 要去它的第二个泊位"<<endl;
                    boat[i].action = SHIP;
                    boat[i].status=0;
                    berth[boat[i].berth_id].boat_id = -1;
                    boat[i].berth_id++;
                    berth[boat[i].berth_id].boat_id = i;
                    boat[i].timer_wait = WAIT_TIME;
                }
            }
            else if(boat[i].berth_id%2==1){ // 在第二个泊位
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait--;
                if(boat[i].timer_wait==0 || boat[i].goods>=boat_capacity*0.99){
                    if(MY_DEBUG)cerr<<"boat "<<i<<" go"<<endl;
                    //cerr<<"boat "<<i<<" 去虚拟点送货了"<<endl;
                    boat[i].action = GO;
                    boat[i].status=0;
                    berth[boat[i].berth_id].boat_id = -1;
                    boat[i].berth_id=-1;
                    boat[i].timer_wait = WAIT_TIME;
                }
            }
        }
    }
}

void Plan::BoatDoGreedy()
{
    /*
    boat_capacity: 船的最大货物容量
    boat[i]: 第i艘船
    boat[i].status: 船的状态，-1表示刚开始，0表示移动中，1表示在装货或刚在虚拟点完成送货, 2表示泊位外等待（目前没用到）
    boat[i].pos: 船的位置
    boat[i].berth_id: 船停靠或要停靠的泊位，-1为虚拟点
    boat[i].timer_wait: 船停靠没货可装时的等待时间，最大时间为WAIT_TIME，
                        还有一个影响等待时间的地方是BerthDo中给船装货后，目前是加上一个值，并且不超过WAIT_TIME
    boat[i].action: 船的动作，SHIP表示停靠，GO表示离开，BA_NOTHING表示无动作
    berth[i]: 第i个泊位, berth[i].goods表示泊位i的货物数量, berth[i].boat_id表示停靠在泊位i的船的id
    MAX_TIMES_BETWEEN_BERTH: 最大泊位间转移次数
    WAIT_TIME: 设定的等待时间

    输入：当前泊位货物和船的状态，以及当前帧数
    输出：保存在船的动作中（boat[i].action），在Output中会统一输出，action为ship时需要设定好berth_id

    */
    // define the action for boats
    if(fid==1){
        // 第一帧特殊处理，任务书里说的船一开始的状态输入无效，全在虚拟点
        for(int i=0;i<5;i++){
            boat[i].status=-1;
            boat[i].pos=i;
            boat[i].berth_id = i*2;
            berth[boat[i].berth_id].boat_id = i;
            boat[i].timer_wait = WAIT_TIME;
            boat[i].times_between_berth = 0;
        }
    }
    for(int i=0;i<5;i++){
        boat[i].action = BA_NOTHING;
        if(boat[i].berth_id!=-1 && (15000-fid)<= berth[boat[i].berth_id].transport_time+1){
            // at last we should all go
            boat[i].status = 0;
            boat[i].action = GO;
            berth[boat[i].berth_id].boat_id = -1;
            boat[i].berth_id = -1;
            boat[i].times_between_berth = 0;
            boat[i].timer_wait = WAIT_TIME;
            continue;
        }
        if(boat[i].status==-1){
            boat[i].status = 0;
            boat[i].action = SHIP;
        }
        else if(boat[i].status==1){ //完成或装货状态
            if(boat[i].berth_id==-1){   // 刚送完货
                
                boat[i].goods = 0;
                boat[i].times_between_berth = 0;

                // 寻找最多货物的泊位
                int best_berth = -1;
                int max_goods = -1;
                for(int ii=0;ii<berth_num;ii++){
                    if(berth[ii].goods>max_goods && berth[ii].boat_id==-1){
                        max_goods = berth[ii].goods;
                        best_berth = ii;
                    }
                }
                // ------------------

                // 已找到并绑定
                if(best_berth!=-1){
                    boat[i].berth_id = best_berth;
                    berth[best_berth].boat_id = i;
                    boat[i].action = SHIP;
                    boat[i].status = 0;
                    boat[i].timer_wait = WAIT_TIME;
                }
            }
            else{
                // 正在装货
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait-=1; // 泊位没货，耐心减1,这个值可以调
                if(boat[i].goods>=boat_capacity*0.99){  //装货量达到95%，出发
                    
                    boat[i].action = GO;
                    boat[i].status = 0;
                    berth[boat[i].berth_id].boat_id = -1;
                    boat[i].berth_id= -1;
                    boat[i].times_between_berth = 0;
                    boat[i].timer_wait = WAIT_TIME;
                }
                else{
                    // 没装够货，找一下当前没船停靠的最多货物的泊位
                    int best_berth = -1;
                    int most_goods = -1;
                    for(int ii=0;ii<berth_num;ii++){
                        if(berth[ii].goods>most_goods && berth[ii].boat_id==-1){
                            most_goods = berth[ii].goods;
                            best_berth = ii;
                        }
                    }
                    assert(best_berth!=-1);

                    // 如果找到了，并且当前泊位没货，且到达目标泊位后能到装货量的80%，就出发
                    if(((boat[i].timer_wait<=0 && boat[i].goods<boat_capacity*0.8) || 
                        (boat[i].timer_wait<=45) && boat[i].goods<boat_capacity*0.5 && most_goods+boat[i].goods>boat_capacity*0.6)
                        && boat[i].times_between_berth<boat[i].MAX_TIMES_BETWEEN_BERTH){
                        boat[i].action = SHIP;  
                        berth[boat[i].berth_id].boat_id = -1;
                        boat[i].berth_id= best_berth;
                        berth[boat[i].berth_id].boat_id = i;
                        boat[i].times_between_berth++;
                        boat[i].status = 0;
                        boat[i].timer_wait = WAIT_TIME;
                    }
                    else{
                        // 如果等待时间超过了，并且达到了最大的泊位间转移次数，就出发
                        if(boat[i].timer_wait<=0 && boat[i].times_between_berth>=boat[i].MAX_TIMES_BETWEEN_BERTH){
                            boat[i].action = GO;
                            boat[i].status = 0;
                            berth[boat[i].berth_id].boat_id = -1;
                            boat[i].berth_id= -1;
                            boat[i].times_between_berth = 0;
                            boat[i].timer_wait = WAIT_TIME;
                        }
                    }
                }
            }
        }
    }
}

void Plan::BerthDo()
{
    // berth action
    if(MY_DEBUG)cerr<<"berth load num: ";
    for(int i=0;i<berth_num;i++){
        int boat_id = berth[i].boat_id;
        if(boat_id!=-1 && boat[boat_id].status==1 && boat[boat_id].berth_id==i && berth[i].goods>0){
            int load_num = min(berth[i].loading_speed, berth[i].goods);
            if(MY_DEBUG)cerr<<load_num<<' ';
            berth[i].goods-=load_num;
            boat[boat_id].goods+=load_num;
            for(int ii=0;ii<load_num;ii++){
                berth[i].goods_val_lst.erase(berth[i].goods_val_lst.begin());
            }
            send_num += load_num;
            boat[boat_id].timer_wait = WAIT_TIME;
        }
        else if(MY_DEBUG)cerr<<0<<' ';
    }
    
    // cerr<< "berth goods val: ";
    // for(int i=0;i<berth_num;i++){
    //     cerr<<accumulate(berth[i].goods_val_lst.begin(),berth[i].goods_val_lst.end(),0.0)<<' ';
    // }cerr<<endl;
    // if(MY_DEBUG)cerr<<endl;

    // cerr<<"berth goods num: ";
    // for(int i=0;i<berth_num;i++){
    //     cerr<<berth[i].goods<<' ';
    // }cerr<<endl;

    if(MY_DEBUG)cerr<<"boat goods: ";
    for(int i=0;i<5;i++){
        if(MY_DEBUG)cerr<<boat[i].goods<<' ';
    }
    if(MY_DEBUG)cerr<<endl;

    // if(fid%1000==0){
    //     cerr<<"berth goods: ";
    //     for(int i=0;i<10;i++){
    //         cerr<<berth[i].goods<<'\t';
    //     }
    //     cerr<<endl;
    //     cerr<<"berth boats: ";
    //     for(int i=0;i<10;i++){
    //         cerr<<berth[i].boat_id<<'\t';
    //     }
    //     cerr<<endl;
    //     cerr<<"boats goods: ";
    //     for(int i=0;i<10;i++){
    //         if(berth[i].boat_id!=-1)cerr<<boat[berth[i].boat_id].goods<<'\t';
    //         else cerr<<0<<'\t';
    //     }
    //     cerr<<endl;
    //     cerr<<"---------------------------------------------------------------"<<endl;
    // }

}

void Plan::BoatDoGreedyMoreMore(){
        /*
    boat_capacity: 船的最大货物容量
    boat[i]: 第i艘船
    boat[i].status: 船的状态，-1表示刚开始，0表示移动中，1表示在装货或刚在虚拟点完成送货, 2表示泊位外等待（目前没用到）
    boat[i].pos: 船的位置
    boat[i].berth_id: 船停靠或要停靠的泊位，-1为虚拟点
    boat[i].timer_wait: 船停靠没货可装时的等待时间，最大时间为WAIT_TIME，
                        还有一个影响等待时间的地方是BerthDo中给船装货后，目前是加上一个值，并且不超过WAIT_TIME
    boat[i].action: 船的动作，SHIP表示停靠，GO表示离开，BA_NOTHING表示无动作
    berth[i]: 第i个泊位, berth[i].goods表示泊位i的货物数量, berth[i].boat_id表示停靠在泊位i的船的id
    MAX_TIMES_BETWEEN_BERTH: 最大泊位间转移次数
    WAIT_TIME: 设定的等待时间

    输入：当前泊位货物和船的状态，以及当前帧数
    输出：保存在船的动作中（boat[i].action），在Output中会统一输出，action为ship时需要设定好berth_id

    */
    // define the action for boats
    // if(fid==1){
    //     // 第一帧特殊处理，任务书里说的船一开始的状态输入无效，全在虚拟点
    //     for(int i=0;i<5;i++){
    //         boat[i].status=-1;
    //         boat[i].pos=i;
    //         boat[i].berth_id = i*2;
    //         // berth[boat[i].berth_id].boat_id = i;
    //         ShipIn(boat[i].berth_id,i);
    //         boat[i].times_between_berth = 0;
    //     }
    // }

    if(fid==1){
        // 第一帧特殊处理，任务书里说的船一开始的状态输入无效，全在虚拟点
        // 初始化时去最近的泊位
        for(int i=0;i<5;i++){
            boat[i].status=-1;
            int min_transport_time = 100000;
            for(int ii=0;ii<10;ii++){
                if(berth[ii].transport_time<min_transport_time && berth[ii].boat_lst.size()==0){
                    boat[i].berth_id = ii;
                    min_transport_time = berth[ii].transport_time;
                }
            }
            if(boat[i].berth_id==-1){
                for(int ii=0;ii<10;ii++){
                    if(berth[ii].boat_lst.size()==0){
                        boat[i].berth_id = ii;
                        break;
                    }
                }
            }
            ShipIn(boat[i].berth_id,i);
            boat[i].times_between_berth = 0;
        }
    }
    for(int i=0;i<5;i++){
        boat[i].action = BA_NOTHING;
        if(boat[i].berth_id!=-1 && (15000-fid)<= berth[boat[i].berth_id].transport_time+1){
            // at last we should all go
            boat[i].status = 0;
            boat[i].action = GO;
            ShipOut(boat[i].berth_id,i);
            boat[i].go_times++;
            boat[i].times_between_berth = 0;
            continue;
        }
        if(boat[i].status==-1){
            boat[i].status = 0;
            boat[i].action = SHIP;
        }
        else if(boat[i].status==1){ //完成或装货状态
            if(boat[i].berth_id==-1){   // 刚送完货
                boat[i].goods = 0;
                boat[i].times_between_berth = 0;

                // 寻找最多货物的泊位
                int best_berth = -1;
                int max_goods = -1;
                for(int ii=0;ii<berth_num;ii++){
                    int remain_goods = berth[ii].goods;
                    for(auto b:berth[ii].boat_lst){
                        remain_goods -= boat_capacity-boat[b].goods;
                    }
                    if(remain_goods>max_goods){
                        max_goods = remain_goods;
                        best_berth = ii;
                    }
                }
                // ------------------

                // 已找到并绑定
                if(best_berth!=-1){
                    ShipIn(best_berth,i);
                    boat[i].action = SHIP;
                    boat[i].status = 0;
                }
            }
            else{
                // 正在装货
                berth[boat[i].berth_id].boat_id = i;
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait-=1; // 泊位没货，耐心减1,这个值可以调
                if(boat[i].goods>=boat_capacity*0.99){  //装货量达到95%，出发
                    boat[i].action = GO;
                    boat[i].status = 0;
                    ShipOut(boat[i].berth_id,i);
                    boat[i].go_times++;
                    boat[i].times_between_berth = 0;
                }
                else{
                    // 没装够货，找一下当前没船停靠的最多货物的泊位
                    int best_berth = -1;
                    int max_goods = -1;
                    for(int ii=0;ii<berth_num;ii++){
                        int remain_goods = berth[ii].goods;
                        for(auto b:berth[ii].boat_lst){
                            remain_goods -= boat_capacity-boat[b].goods;
                        }
                        if(remain_goods>max_goods){
                            max_goods = remain_goods;
                            best_berth = ii;
                        }
                    }
                    assert(best_berth!=-1);

                    // 如果找到了，并且当前泊位没货，且到达目标泊位后能到装货量的80%，就出发
                    if((boat[i].timer_wait<=0) 
                        // || (boat[i].timer_wait<=45) && boat[i].goods<boat_capacity*0.5 && max_goods+boat[i].goods>boat_capacity*0.9)
                        && boat[i].times_between_berth<boat[i].MAX_TIMES_BETWEEN_BERTH 
                        && (15000-berth[best_berth].transport_time-500-30)>fid){
                        boat[i].action = SHIP;
                        berth[boat[i].berth_id].boat_id = -1;
                        berth[boat[i].berth_id].boat_lst.erase(berth[boat[i].berth_id].boat_lst.begin());
                        ShipIn(best_berth,i);
                        boat[i].times_between_berth++;
                        boat[i].status = 0;
                        boat[i].timer_wait = WAIT_TIME;
                    }
                    else{
                        // 如果等待时间超过了，并且达到了最大的泊位间转移次数，就出发
                        if(boat[i].timer_wait<=0 && boat[i].times_between_berth>=boat[i].MAX_TIMES_BETWEEN_BERTH){
                            boat[i].action = GO;
                            boat[i].status = 0;
                            ShipOut(boat[i].berth_id,i);
                            boat[i].go_times++;
                            boat[i].times_between_berth = 0;
                            boat[i].timer_wait = WAIT_TIME;
                        }
                    }
                }
            }
        }
    }
}

void Plan::BoatDoMinTravel(){
    /*
    boat_capacity: 船的最大货物容量
    boat[i]: 第i艘船
    boat[i].status: 船的状态，-1表示刚开始，0表示移动中，1表示在装货或刚在虚拟点完成送货, 2表示泊位外等待（目前没用到）
    boat[i].pos: 船的位置
    boat[i].berth_id: 船停靠或要停靠的泊位，-1为虚拟点
    boat[i].timer_wait: 船停靠没货可装时的等待时间，最大时间为WAIT_TIME，
                        还有一个影响等待时间的地方是BerthDo中给船装货后，目前是加上一个值，并且不超过WAIT_TIME
    boat[i].action: 船的动作，SHIP表示停靠，GO表示离开，BA_NOTHING表示无动作
    berth[i]: 第i个泊位, berth[i].goods表示泊位i的货物数量, berth[i].boat_id表示停靠在泊位i的船的id
    MAX_TIMES_BETWEEN_BERTH: 最大泊位间转移次数
    WAIT_TIME: 设定的等待时间

    输入：当前泊位货物和船的状态，以及当前帧数
    输出：保存在船的动作中（boat[i].action），在Output中会统一输出，action为ship时需要设定好berth_id

    */
    // define the action for boats
    // if(fid==13455){
    //     ofstream out("boat_berth_13455.txt",ios::out);
    //     for(int i=0;i<5;i++){
    //         out<<"boat id"<<i<<" status: "<<boat[i].status<<" berth id: "<<boat[i].berth_id<<endl;
    //         out<<"  timer_wait: "<<boat[i].timer_wait<<endl;
    //         out<<"  goods: "<<boat[i].goods<<endl;
    //         out<<"  capacity: "<<boat_capacity<<endl;
    //         out<<"  times_between_berth: "<<boat[i].times_between_berth<<endl;
    //     }
    //     for(int i=0;i<10;i++){
    //         out<<"berth "<<i<<": "<<berth[i].boat_id<<" berth goods: "<<berth[i].goods<<" berth transport_time: "<<berth[i].transport_time<<endl;
    //         out<<"  berth boat list: ";
    //         for(auto b:berth[i].boat_lst){
    //             out<<b<<' ';
    //         }
    //         out<<endl;
    //     }
    // }

    if(fid==1){
        // 第一帧特殊处理，任务书里说的船一开始的状态输入无效，全在虚拟点
        // 初始化时去最近的泊位
        for(int i=0;i<5;i++){
            boat[i].status=-1;
            int min_transport_time = 100000;
            for(int ii=0;ii<10;ii++){
                if(berth[ii].transport_time<min_transport_time && berth[ii].boat_lst.size()==0){
                    boat[i].berth_id = ii;
                    min_transport_time = berth[ii].transport_time;
                }
            }
            if(boat[i].berth_id==-1){
                for(int ii=0;ii<10;ii++){
                    if(berth[ii].boat_lst.size()==0){
                        boat[i].berth_id = ii;
                        break;
                    }
                }
            }
            ShipIn(boat[i].berth_id,i);
            boat[i].times_between_berth = 0;
        }
    }
    for(int i=0;i<5;i++){
        boat[i].action = BA_NOTHING;
        if(boat[i].berth_id!=-1 && (15000-fid)<= berth[boat[i].berth_id].transport_time+1){
            // at last we should all go
            boat[i].status = 0;
            boat[i].action = GO;
            ShipOut(boat[i].berth_id,i);
            boat[i].go_times++;
            boat[i].times_between_berth = 0;
            continue;
        }
        if(boat[i].status==-1){
            boat[i].status = 0;
            boat[i].action = SHIP;
        }
        else if(boat[i].status==1){ //完成或装货状态
            if(boat[i].berth_id==-1){   // 刚送完货
                boat[i].goods = 0;
                boat[i].times_between_berth = 0;
                // 寻找最多货物的泊位
                int best_berth = -1;
                int max_goods = -1;
                for(int ii=0;ii<berth_num;ii++){
                    int remain_goods = berth[ii].goods;
                    for(auto b:berth[ii].boat_lst){
                        remain_goods -= boat_capacity-boat[b].goods;
                    }
                    if(remain_goods>max_goods && 
                        find(need_transfer_berth.begin(),need_transfer_berth.end(),ii)==need_transfer_berth.end())
                    {
                        max_goods = remain_goods;
                        best_berth = ii;
                    }
                }
                // ------------------

                // 已找到并绑定
                if(best_berth!=-1){
                    ShipIn(best_berth,i);
                    boat[i].action = SHIP;
                    boat[i].status = 0;
                }
            }
            else{
                // 正在装货
                berth[boat[i].berth_id].boat_id = i;
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait-=1; // 泊位没货，耐心减1,这个值可以调
                if(boat[i].goods>=boat_capacity*0.95){  //装货量达到95%，出发
                    // 找去虚拟点的中转泊位，若存在，则去中转，否则直接出发
                    std::vector<int> transfer_berth;
                    transfer_berth.clear();
                    for(int ii=0;ii<berth_num;ii++){
                        if(berth[ii].transport_time+500<berth[boat[i].berth_id].transport_time){
                            transfer_berth.push_back(ii);
                        }
                    }

                    if(transfer_berth.size()==0){
                        boat[i].action = GO;
                        boat[i].status = 0;
                        ShipOut(boat[i].berth_id,i);
                        boat[i].go_times++;
                        boat[i].times_between_berth = 0;
                        boat[i].timer_wait = WAIT_TIME;
                    }
                    else{
                        ShipOut(boat[i].berth_id,i);
                        boat[i].action = SHIP;
                        boat[i].status = 0;
                        int min_transfer_time = 100000;
                        for(auto b:transfer_berth){
                            if(berth[b].transport_time<min_transfer_time){
                                min_transfer_time = berth[b].transport_time;
                                boat[i].berth_id = b;
                            }
                        }
                        ShipIn(boat[i].berth_id,i);
                    }
                }
                else{
                    // 没装够货，找一下当前没船停靠的最多货物的泊位
                    int best_berth = -1;
                    int max_goods = -1;
                    int remain_goods[10] = {0};
                    for(int ii=0;ii<berth_num;ii++){
                        remain_goods[ii] = berth[ii].goods;
                        for(auto b:berth[ii].boat_lst){
                            remain_goods[ii] -= boat_capacity-boat[b].goods;
                        }
                        // if(fid==13455)cerr<<"flag 4 "<<ii<<" done"<<endl;
                        if(remain_goods[ii]>max_goods){
                            max_goods = remain_goods[ii];
                            best_berth = ii;
                        }
                    }
                    if(best_berth==-1)cerr<<"没找着有货且可去的泊位"<<endl;
                    if((boat[i].timer_wait<=0) 
                        // || (boat[i].timer_wait<=45) && boat[i].goods<boat_capacity*0.5 && max_goods+boat[i].goods>boat_capacity*0.9)
                        && boat[i].times_between_berth<boat[i].MAX_TIMES_BETWEEN_BERTH )
                    { 
                        ShipOut(boat[i].berth_id,i);
                        ShipIn(best_berth,i);
                        boat[i].action = SHIP;
                        boat[i].times_between_berth++;
                        boat[i].status = 0;
                    }
                    else{
                        // 如果等待时间超过了，并且达到了最大的泊位间转移次数，就出发
                        if(boat[i].timer_wait<=0 && boat[i].times_between_berth>=boat[i].MAX_TIMES_BETWEEN_BERTH){
                            // 如果有更近的中转泊位，就去货最多的中转泊位
                            std::vector<int> transfer_berth;
                            for(int ii=0;ii<berth_num;ii++){
                                if(berth[ii].transport_time+500<berth[boat[i].berth_id].transport_time){
                                    transfer_berth.push_back(ii);
                                }
                            }

                            if(transfer_berth.size()==0){
                                ShipOut(boat[i].berth_id,i);
                                boat[i].action = GO;
                                boat[i].go_times++;
                                boat[i].status = 0;
                                boat[i].times_between_berth = 0;
                            }
                            else{
                                ShipOut(boat[i].berth_id,i);
                                boat[i].action = SHIP;
                                boat[i].status = 0;
                                max_goods = -1;
                                for(auto b:transfer_berth){
                                    if(remain_goods[b]>max_goods){
                                        max_goods = remain_goods[b];
                                        boat[i].berth_id = b;
                                    }
                                }
                                ShipIn(boat[i].berth_id,i);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Plan::UpdateBoatStrategy(){
    // if(fid==1){
    //     WAIT_TIME = 1;
    //     boat[0].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[1].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[2].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[3].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[4].MAX_TIMES_BETWEEN_BERTH = 10;
    // }
    // if(fid==3000){
    //     WAIT_TIME = 1;
    //     boat[0].MAX_TIMES_BETWEEN_BERTH = 1;
    //     boat[1].MAX_TIMES_BETWEEN_BERTH = 1;
    //     boat[2].MAX_TIMES_BETWEEN_BERTH = 1;
    //     boat[3].MAX_TIMES_BETWEEN_BERTH = 1;
    //     boat[4].MAX_TIMES_BETWEEN_BERTH = 1;
    // }
    // if(fid==11000){
    //     WAIT_TIME = 1;
    //     boat[0].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[1].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[2].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[3].MAX_TIMES_BETWEEN_BERTH = 10;
    //     boat[4].MAX_TIMES_BETWEEN_BERTH = 10;
    // }
    WAIT_TIME=1;
    int all_boat_last_time = 0;
    int times_num[10];
    for(int i=0;i<10;i++)times_num[i] = 0;
    for(int i=0;i<5;i++){
        for(int j=0;j<=min(boat[i].go_times,9);j++){
            times_num[j]++;
        }
    }
    if(times_num[1]<=2){
        for(int i=0;i<5;i++){
            if(boat[i].go_times==1){
                boat[i].order_status = TIMES_ONE_FIRST;
                // cerr<<"boat "<<i<<" 2"<<endl;
            }
        }
    }
    if(times_num[2]<=1){
        for(int i=0;i<5;i++){
            if(boat[i].go_times==2){
                boat[i].order_status = TIMES_TWO_FIRST;
                // cerr<<"boat "<<i<<" 2"<<endl;
            }
        }
    }

    for(int i=0;i<5;i++){
        if(boat[i].go_times==0){
            boat[i].MAX_TIMES_BETWEEN_BERTH = 4;
        }
        else if(boat[i].go_times==1 && boat[i].order_status==TIMES_ONE_FIRST){
            boat[i].MAX_TIMES_BETWEEN_BERTH = 2;
            // cerr<<"boat "<<i<<" 2"<<endl;
        }
        else if(boat[i].go_times==2 && boat[i].order_status==TIMES_TWO_FIRST){
            boat[i].MAX_TIMES_BETWEEN_BERTH = 2;
        }
        else if(boat[i].go_times<=3){
            boat[i].MAX_TIMES_BETWEEN_BERTH = 1;
        }
        else{
            boat[i].MAX_TIMES_BETWEEN_BERTH = 10;
            all_boat_last_time+=1;
        }
    }
    if(all_boat_last_time==5){
        WAIT_TIME = 50;
    }
}

void Plan::Process()
{
    if(need_transfer_berth.size()==0){
        RobotDoNoDelay();
        UpdateBoatStrategy();
        BoatDoGreedyMoreMore();
        // BoatDoMinTravel();
        BerthDo();
    }
    else{
        RobotDoNoDelay();
        UpdateBoatStrategy();
        BoatDoMinTravel();
        BerthDo();
    }
}

void Plan::Summary()
{
    if(analysis){
        ofstream out("../LinuxRelease/summary.txt");
        out<<"fetch num: "<<fetch_num<<endl;
        out<<"send num: "<<send_num<<endl;
        out<<"fetch time: avg="<<accumulate(fetch_time.begin(),fetch_time.end(),0.0)/fetch_time.size()<<endl;
        out<<endl;
        out.close();
        cerr<<"fetch num: "<<fetch_num<<endl;
        cerr<<"send num: "<<send_num<<endl;
        cerr<<"fetch time: avg="<<accumulate(fetch_time.begin(),fetch_time.end(),0.0)/fetch_time.size()<<endl;
        cerr<<"fetch time: max="<<*max_element(fetch_time.begin(),fetch_time.end())<<endl;
        cerr<<"fetch time: min="<<*min_element(fetch_time.begin(),fetch_time.end())<<endl;
    }
}
