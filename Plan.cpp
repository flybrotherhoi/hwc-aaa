#include<bits/stdc++.h>
#include "Plan.h"

const int robot_num=10;
const int berth_num=10;
const int n = 200;
const int MAX_DIST=1e5;
int dx[4]={0,0,-1,1};
int dy[4]={1,-1,0,0};
const int time_between_berth=500;
// in the map, '.' means empty, 'A' means robot, 'B' means berth(4x4),
// '#' means obstacle, '*' means ocean (same as obstacle which robot cannot pass through)
const int see_log=0;
const int WAIT_TIME=50;
const int MY_DEBUG=0;

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
        boat[i].pos = i;
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
    // initialize the map
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
    // for(int i=0;i<berth_num;i++){
    //     int x = berth[i].x;
    //     int y = berth[i].y;
    //     for(int j=0;j<4;j++)for(int k=0;k<4;k++)map[x+j][y+k]=i;
    // }
    
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
    
    for(int i=0;i<10;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                shortest_dist_to_berth[i][j][k]=MAX_DIST;
            }
        }
    }

    // 用广度优先搜索计算每个泊位到每个点的最短距离
    for(int i=0;i<berth_num;i++){
        if(MY_DEBUG)cerr<<"finding the shortest distance to berth "<<i<<endl;
        std::queue<std::pair<int,int>> q;
        q.push({berth[i].x,berth[i].y});
        shortest_dist_to_berth[i][berth[i].x][berth[i].y]=0;
        while(!q.empty()){
            auto [x,y]=q.front();
            q.pop();
            for(int k=0;k<4;k++){
                int nx=x+dx[k];
                int ny=y+dy[k];
                if(nx>=0 && nx<n && ny>=0 && ny<n && map[nx][ny]==1 && shortest_dist_to_berth[i][nx][ny]>shortest_dist_to_berth[i][x][y]+1){
                    shortest_dist_to_berth[i][nx][ny]=shortest_dist_to_berth[i][x][y]+1;
                    q.push({nx,ny});
                }
            }
        }
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

    // short parts_check[210][210];
    // for(int i=0;i<n;i++){
    //     memcpy(parts_check[i],parts[i],sizeof(short)*n);
    // }
    // if(MY_DEBUG)cerr<<"parts check start"<<endl;
    // 检查每个区域的连通性
    // for(int i=0;i<berth_num;i++){
    //     std::queue<std::pair<int,int>> q;
    //     int x=berth[i].x;
    //     int y=berth[i].y;
    //     q.push({x,y});
    //     int cnt=0;
    //     parts_check[x][y]=-1;
    //     while(!q.empty()){
    //         auto [x,y]=q.front();
    //         q.pop();
    //         cnt++;
    //         for(int k=0;k<4;k++){
    //             int nx=x+dx[k];
    //             int ny=y+dy[k];
    //             if(nx>=0&&nx<n&&ny>=0&&ny<n&&parts[nx][ny]==i){
    //                 parts_check[nx][ny]=-1;
    //                 q.push({nx,ny});
    //             }
    //         }
    //     }
    //     if(cnt!=parts_num[i]){
    //         // printf("berth %d is not connected, cnt=%d, should be %d\n",i, cnt, parts_num[i]);
    //         // exit(0);
    //     }
    // }
    
    if(MY_DEBUG)cerr<<"parts check done"<<endl;

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
        if(flag[parts[x][y]]==0){
            robot[i].berth_id=parts[x][y];
            berth[parts[x][y]].robot_id=i;
            flag[parts[x][y]]++;
            continue;
        }
        for(int j=0;j<10;j++){
            if(flag[j]==0 && shortest_dist_to_berth[j][x][y]!=MAX_DIST){
                robot[i].berth_id=j;
                flag[j]++;
                break;
            }
        }
    }
    if(MY_DEBUG)cerr<<"robot assigned to berth"<<endl;

    // plan the path for each robot
    vector<pair<int,int>> vec_robot_path[10]; // 为了检测冲突好遍历用的数组
    for(int i=0;i<robot_num;i++){
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
        robot_path_len[i]=dist;
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
                    vec_robot_path[i].push_back(make_pair(x,y));
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
                    vec_robot_path[i].push_back(make_pair(x,y));
                    dist++;
                }
            }
        }
    }

    //robot path conflicts detection
    for(int i=0;i<10;i++){
        robot[i].start_delay=0;
        int conflict_flag=0;
        vector<int> conflict_id;
        for(int j=0;j<i;j++){
            int t=0;
            while(t+robot[j].start_delay+1<vec_robot_path[i].size()&&t<vec_robot_path[j].size()&&
                diff_pair(vec_robot_path[i][t+robot[j].start_delay],vec_robot_path[j][t]) &&
                diff_pair(vec_robot_path[i][t+robot[j].start_delay+1],vec_robot_path[j][t]) ){
                t++;
            }
            if(t+robot[j].start_delay<vec_robot_path[i].size()-1&&t<vec_robot_path[j].size()){
                conflict_flag=1;
                conflict_id.push_back(j);
            }
        }
        if(MY_DEBUG)cerr<<"conflict flag: "<<conflict_flag<<endl;
        if(conflict_flag==1){
            // int start_delay = 1;
            // while(1){
            //     int conflict_flag=0;
            //     for(int j=0;j<i;j++){
            //         int jsd = robot[j].start_delay;
            //         int t=0;
            //         while(t+jsd-start_delay+1<vec_robot_path[i].size()-start_delay && t<vec_robot_path[j].size()
            //          && diff_pair(vec_robot_path[i][t+jsd-start_delay], vec_robot_path[j][t]) 
            //          && diff_pair(vec_robot_path[i][t+jsd-start_delay+1], vec_robot_path[j][t])){
            //             t++;
            //         }
            //         if(t+jsd-start_delay+1<vec_robot_path[i].size()-start_delay&&t<vec_robot_path[j].size()){
            //             conflict_flag=1;
            //         }
            //         // if(t<vec_robot_path[i].size()&&t<vec_robot_path[j].size()){
            //         //     conflict_flag=1;
            //         // }
            //     }
            //     if(conflict_flag==0){
            //         robot[i].start_delay=start_delay;
            //         break;
            //     }
            //     start_delay++;
                // if(MY_DEBUG)cerr<<"start delay: "<<start_delay<<endl;
            
            //}
            int start_delay=MAX_DIST;
            for(int k=0;k<conflict_id.size();k++){
                if(start_delay>(robot[conflict_id[k]].start_delay+vec_robot_path[conflict_id[k]].size())){
                    start_delay=robot[conflict_id[k]].start_delay+vec_robot_path[conflict_id[k]].size();
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
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[100];
    scanf("%s", okk);
    return fid;
}

void Plan::Output()
{
    for(int i=0;i<robot_num;i++){
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

void Plan::RobotDo()
{
        int robot_not_ready=0;
    for(int i=0;i<robot_num;i++){
        robot_not_ready += robot[i].status==RobotStatus::NotReady;
    }
    if(MY_DEBUG)cerr<<"robot not ready: "<<robot_not_ready<<endl;
    for(int i=0;i<robot_num;i++){
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
                if(robot_not_ready>0)continue;
                if(robot[i].goods==1){
                    robot[i].action_before_move=PULL;
                    berth[berth_id].goods++;
                }
                if(MY_DEBUG)cerr<<"planing the path for robot "<<i<<endl;
                // plan the path to the nearest goods
                int dist=MAX_DIST;
                float vvt = 0;
                for(int t=0;t<berth_region[berth_id].size();t++){
                    int x=berth_region[berth_id][t].first;
                    int y=berth_region[berth_id][t].second;
                    float nvvt;
                    nvvt = 5.0*gds[x][y]/(1.0*shortest_dist_to_berth[berth_id][x][y]);
                    if(gds[x][y]>0 && gds_time[x][y]+1000>fid+shortest_dist_to_berth[berth_id][x][y] && nvvt>vvt){
                        robot[i].gx=x;
                        robot[i].gy=y;
                        dist = shortest_dist_to_berth[berth_id][x][y];
                        vvt = nvvt;
                    }
                }
                if(dist!=MAX_DIST){
                    if(MY_DEBUG)cerr<<"robot "<<i<<" going to goods: "<<robot[i].gx<<' '<<robot[i].gy<<", dist= "<<dist<<endl;
                    robot_path[i].clear();
                    robot[i].status=GoingToGood;
                    // plan the path for each robot
                    deque<pair<int,int>> q;
                    q.push_back({robot[i].gx,robot[i].gy});
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
                                break;
                            }
                        }
                    }
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
                else robot[i].action_move=STAND;
            }
            else{
                if(robot_path[i].size()!=0){
                    robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                    robot_path[i].pop_front();
                }
            }
        }
        else if (robot[i].status==GoingToGood && robot_not_ready==0){
            if(x==robot[i].gx && y==robot[i].gy){
                robot[i].status=ReturnToBerth;
                if(robot[i].goods==0){
                    robot[i].action_before_move=GET;
                    gds[robot[i].gx][robot[i].gy]=0;
                    gds_time[robot[i].gx][robot[i].gy]=0;
                }
                int dist=MAX_DIST;
                robot[i].gx = berth[berth_id].x;
                robot[i].gy = berth[berth_id].y;
                robot_path[i].clear();
                deque<pair<int,int>> q;
                q.push_back({robot[i].x,robot[i].y});
                while(!q.empty()){
                    auto [x,y]=q.front();
                    q.pop_front();
                    // if(x==robot[i].x&&y==robot[i].y)break;
                    for(int k=0;k<4;k++){
                        int nx=x+dx[k];
                        int ny=y+dy[k];
                        if(nx>=0&&nx<n&&ny>=0&&ny<n&&map[nx][ny]==1&&shortest_dist_to_berth[berth_id][nx][ny]<shortest_dist_to_berth[berth_id][x][y]){
                            q.push_back({nx,ny});
                            robot_path[i].push_back(static_cast<RobotMove>(k));
                            break;
                        }
                    }
                }
                robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                robot_path[i].pop_front();
                if(MY_DEBUG)cerr<<"robot "<<i<<" return path to berth: ";
                for(auto p:robot_path[i]){
                    if(MY_DEBUG)cerr<<p<<' ';
                }if(MY_DEBUG)cerr<<endl;
            }
            else{
                robot[i].action_move=static_cast<RobotMove>(robot_path[i].front());
                robot_path[i].pop_front();
            }
        }
        if(MY_DEBUG)cerr<<"robot "<<i<<" action: "<<robot[i].action_before_move<<' '<<robot[i].action_move<<' '<<robot[i].action_after_move<<endl;
    }
}

void Plan::BoatDo()
{
    // define the action for boats
    if(fid==1){
        // 第一帧特殊处理
        for(int i=0;i<5;i++){
            boat[i].status=-1;
            boat[i].pos=i;
            boat[i].berth_id = i*2;
            berth[boat[i].berth_id].boat_id = i;
            boat[i].timer_wait = WAIT_TIME;
        }
    }
    for(int i=0;i<5;i++){
        boat[i].action = BA_NOTHING;
        if(boat[i].berth_id!=-1 && (15000-fid)<= berth[boat[i].berth_id].transport_time+20){
            boat[i].status = 1;
            boat[i].action = GO;
            boat[i].berth_id = -1;
            boat[berth[i].boat_id].berth_id = -1;
            continue;
        }
        if(boat[i].status==-1){
            boat[i].status = 0;
            boat[i].action = SHIP;
        }
        else if(boat[i].status==1){
            if(boat[i].berth_id==-1){
                boat[i].action = SHIP;
                boat[i].berth_id = i*2;
                berth[boat[i].berth_id].boat_id = i;
                boat[i].goods = 0;
            }
            else if(boat[i].berth_id%2==0){ // 还在第一个泊位
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait--;
                if(berth[boat[i].berth_id+1].goods>boat_capacity-boat[i].goods || boat[i].goods>=boat_capacity*0.49 || boat[i].timer_wait==0){
                    if(MY_DEBUG)cerr<<"boat to go "<<i<<"  "<<(berth[boat[i].berth_id].goods==0) << (berth[boat[i].berth_id+1].goods>boat_capacity-boat[i].goods) << (boat[i].goods>=boat_capacity*0.3)<<endl;
                    boat[i].action = SHIP;
                    boat[i].berth_id++;
                    berth[boat[i].berth_id].boat_id = i;
                }
            }
            else if(boat[i].berth_id%2==1){ // 在第二个泊位
                if(berth[boat[i].berth_id].goods==0) boat[i].timer_wait--;
                if(boat[i].timer_wait==0 || boat[i].goods>=boat_capacity*0.98){
                    if(MY_DEBUG)cerr<<"boat "<<i<<" go"<<endl;
                    boat[i].action = GO;
                    boat[i].berth_id=-1;
                    berth[boat[i].berth_id].boat_id = -1;
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
        if(boat_id!=-1){
            if(boat[boat_id].goods<boat_capacity & berth[i].goods>0){
                int load_num = min(berth[i].loading_speed, min(boat_capacity-boat[boat_id].goods, berth[i].goods));
                if(MY_DEBUG)cerr<<load_num<<' ';
                berth[i].goods-=load_num;
                boat[boat_id].goods+=load_num;
                boat[boat_id].timer_wait = WAIT_TIME;
            }else if(MY_DEBUG)cerr<<0<<' ';
        }
        else if(MY_DEBUG)cerr<<0<<' ';
    }
    if(MY_DEBUG)cerr<<endl;

    if(MY_DEBUG)cerr<<"boat goods: ";
    for(int i=0;i<5;i++){
        if(MY_DEBUG)cerr<<boat[i].goods<<' ';
    }
    if(MY_DEBUG)cerr<<endl;

    if(MY_DEBUG)cerr<<"berth goods: ";
    for(int i=0;i<10;i++){
        if(MY_DEBUG)cerr<<berth[i].goods<<' ';
    }
    if(MY_DEBUG)cerr<<endl;
}

void Plan::Process()
{
    RobotDo();
    BoatDo();
    BerthDo();
}