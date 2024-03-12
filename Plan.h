#include<bits/stdc++.h>
#include "RBB.h"
const int robot_num=10;
const int berth_num=10;
const int n = 200;
int dx[4]={0,0,1,-1};
int dy[4]={1,-1,0,0};
// in the map, '.' means empty, 'A' means robot, 'B' means berth(4x4),
// '#' means obstacle, '*' means ocean (same as obstacle which robot cannot pass through)

class Plan
{
public:
    int money;
    int boat_capacity;
    int id;
    Robot robot[robot_num];
    Berth berth[berth_num];
    short robot_path[robot_num][210][210];
    short robot_path_len[robot_num];
    short robot_status[robot_num];
    Boat boat[10];
    char ch[210][210];
    short map[210][210];
    short parts[210][210];
    int gds[210][210];
    Plan() {}
    Plan(int robot_num, int berth_num, int N, int money, int boat_capacity, int id) {
   
        this -> money = money;
        this -> boat_capacity = boat_capacity;
        this -> id = id;
    }
    void Init()
    {
        for(int i = 1; i <= n; i ++)
            scanf("%s", ch[i] + 1);
        for(int i = 0; i < berth_num; i ++)
        {
            int id;
            scanf("%d", &id);
            scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
        }
        scanf("%d", &boat_capacity);
        char okk[100];
        scanf("%s", okk);
        // get the position of robots
        int tid=0;
        for(int i = 1; i <= n; i ++)
            for(int j = 1; j <= n; j ++)
            {
                if(ch[i][j] == 'A')
                {
                    robot[id].x = i;
                    robot[id].y = j;
                    id++;
                }
            }

        // assign robot to berth using naive distance
        for(int i = 0; i < robot_num; i ++)
        {
            int dist = 1e9;
            int berth_id = -1;
            for(int j = 0; j < berth_num; j ++)
            {
                int tmp = abs(robot[i].x - berth[j].x) + abs(robot[i].y - berth[j].y);
                if(tmp < dist)
                {
                    dist = tmp;
                    berth_id = j;
                }
            }
            robot[i].berth_id = berth_id;
            berth[berth_id].robot_id = i;
        }

        // assign berth to boat
        tid = 0;
        for(int i = 0; i < 5; i ++)
        {
            boat[i].pos = i;
            boat[i].status = 0;
            boat[i].num = 0;
            boat[i].goods = 0;
            for(int j = 0; j < 2; j ++)
            {
                boat[i].berth_id[j] = tid;
                berth[tid].boat_id = i;
                tid++;
            }
        }

        // initialize the map
        for(int i = 1; i <= n; i ++)
            for(int j = 1; j <= n; j ++)
            {
                if(ch[i][j] == '#')
                    map[i][j] = 0;
                else if (ch[i][j] == '*')
                    map[i][j] = -1;
                else
                    map[i][j] = 1;
            }
        for(int i=0;i<berth_num;i++){
            int x = berth[i].x;
            int y = berth[i].y;
            for(int j=0;j<4;j++)for(int k=0;k<4;k++)map[x+j][y+k]=i;
        }
        
        // 给每个泊位分配一个区域
        memset(parts,-1,sizeof(parts));
        for(int i=0;i<berth_num;i++){
            int x=berth[i].x;
            int y=berth[i].y;
            for(int j=0;j<4;j++)for(int k=0;k<4;k++)parts[x+j][y+k]=i;
        }
        int shortest_dist_to_berth[10][210][210];
        memset(shortest_dist_to_berth,0x3f,sizeof(shortest_dist_to_berth));
        // 用广度优先搜索计算每个泊位到每个点的最短距离
        for(int i=0;i<berth_num;i++){
            std::queue<std::pair<int,int>> q;
            q.push({berth[i].x,berth[i].y});
            shortest_dist_to_berth[i][berth[i].x][berth[i].y]=0;

            while(!q.empty()){
                auto [x,y]=q.front();
                q.pop();
                for(int k=0;k<4;k++){
                    int nx=x+dx[k];
                    int ny=y+dy[k];
                    if(nx>=1&&nx<=n&&ny>=1&&ny<=n&&map[nx][ny]&&shortest_dist_to_berth[i][nx][ny]>shortest_dist_to_berth[i][x][y]+1){
                        shortest_dist_to_berth[i][nx][ny]=shortest_dist_to_berth[i][x][y]+1;
                        q.push({nx,ny});
                    }
                }
            }
        }
        // 分配parts,给map上所有为1的点分配一个距离最近的泊位
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(map[i][j]==1){
                    int dist=1e9;
                    int id=-1;
                    for(int k=0;k<berth_num;k++){
                        if(shortest_dist_to_berth[k][i][j]<dist){
                            dist=shortest_dist_to_berth[k][i][j];
                            id=k;
                        }
                    }
                    parts[i][j]=id;
                }
            }
        }
        // 检查每个区域的连通性
        for(int i=0;i<berth_num;i++){
            std::queue<std::pair<int,int>> q;
            int x=berth[i].x;
            int y=berth[i].y;
            q.push({x,y});
            int cnt=0;
            while(!q.empty()){
                auto [x,y]=q.front();
                q.pop();
                cnt++;
                for(int k=0;k<4;k++){
                    int nx=x+dx[k];
                    int ny=y+dy[k];
                    if(nx>=1&&nx<=n&&ny>=1&&ny<=n&&parts[nx][ny]==i){
                        parts[nx][ny]=-1;
                        q.push({nx,ny});
                    }
                }
            }
            if(cnt!=16){
                printf("berth %d is not connected\n",i);
                exit(0);
            }
        }

        printf("OK\n");
        fflush(stdout);
    }
    int Input()
    {
        scanf("%d%d", &id, &money);
        int num;
        scanf("%d", &num);
        for(int i = 1; i <= num; i ++)
        {
            int x, y, val;
            scanf("%d%d%d", &x, &y, &val);
        }
        for(int i = 0; i < robot_num; i ++)
        {
            int sts;
            scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
        }
        for(int i = 0; i < 5; i ++)
            scanf("%d%d\n", &boat[i].status, &boat[i].pos);
        char okk[100];
        scanf("%s", okk);
        return id;
    }
};