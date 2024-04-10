#include "Plan.h"
#include "RBB.h"

const int MAX_DIST = 100000;

void Plan::Init(){
    // 做一些必要的初始化
    money = 0;
    berth_num = 0;
    goods_num = 0;
    robot_num = 0;
    boat_num = 0;
    boat_capacity = 0;
    frame_id = -1;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            gds_time[i][j] = -1;
            gds_val[i][j] = 0;
        }
    }
    robot.clear();
    boat.clear();
    berth.clear();
    robot_purchase_point.clear();
    boat_purchase_point.clear();
    delivery_point.clear();

    for(int i = 0; i < N; i ++){
        scanf("%s", grid[i]);
    }

    scanf("%d", &berth_num);
    for(int i = 0; i < berth_num; i ++)
    {
        int id,x,y,z,loading_speed;
        scanf("%d", &id);
        scanf("%d%d%d", &x, &y, &loading_speed);
        berth.push_back(Berth(x,y,loading_speed));
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);

    ProcessMap();

    printf("OK\n");
    fflush(stdout);
    if(MY_DEBUG)cerr<<"Init()::Init done"<<endl;
}

void Plan::ProcessMap()
{
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            if(grid[i][j] == 'R'){
                robot_map[i][j] = 2;
                boat_map[i][j] = 0;
                robot_purchase_point.push_back(make_pair(i, j));
            }
            else if(grid[i][j] == 'S'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 2;
                boat_purchase_point.push_back(make_pair(i, j));
            }
            else if(grid[i][j] == 'T'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 1;
                delivery_point.push_back(make_pair(i, j));
            }
            else if(grid[i][j]=='.'){
                robot_map[i][j] = 1;
                boat_map[i][j] = 0;
            }
            else if(grid[i][j]=='C'){
                robot_map[i][j] = 1;
                boat_map[i][j] = 1;
            }
            else if(grid[i][j]=='#'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 0;
            }
            else if(grid[i][j]=='>'){
                robot_map[i][j] = 1;
                boat_map[i][j] = 0;
            }
            else if(grid[i][j]=='c'){
                robot_map[i][j] = 1;
                boat_map[i][j] = 2;
            }
            else if(grid[i][j]=='B'){
                robot_map[i][j] = 1;
                boat_map[i][j] = 2;
            }
            else if(grid[i][j]=='~'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 2;
            }
            else if(grid[i][j]=='K'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 2;
            }
            else if(grid[i][j]=='*'){
                robot_map[i][j] = 0;
                boat_map[i][j] = 1;
            }
            else{
                robot_map[i][j] = -1;
                boat_map[i][j] = -1;
            }
        }
    }
    for(int i=0;i<N-1;i++){
        for(int j=0;j<N-1;j++){
            if(boat_map[i][j]>0 && boat_map[i+1][j]>0 && boat_map[i][j+1]>0 && boat_map[i+1][j+1]>0){
                boat_map_dual[i][j]=1;
            }
            else{
                boat_map_dual[i][j]=0;
            }
        }
    }

    // ofstream out("bmapdual.txt");
    // for(int i = 0; i < N-1; i ++){
    //     for(int j = 0; j < N-1; j ++){
    //         out << boat_map_dual[i][j];
    //     }
    //     out << endl;
    // }
    // ofstream out("bmap.txt");
    // for(int i = 0; i < N; i ++){
    //     for(int j = 0; j < N; j ++){
    //         out << boat_map[i][j];
    //     }
    //     out << endl;
    // }
    
}

void Plan::Input(){
    scanf("%d", &money);
    scanf("%d", &goods_num);
    for(int i = 0; i < goods_num; i ++)
    {
        int x, y, val;
        scanf("%d %d %d", &x, &y, &val);
        gds_time[x][y] = frame_id;
        gds_val[x][y] = val;
    }

    scanf("%d", &robot_num);
    if(robot_num<robot.size()){
        robot.erase(robot.begin()+robot_num,robot.end());
    }
    for(int i = 0; i < robot_num; i ++)
        scanf("%d %d %d %d", &robot[i].id, &robot[i].goods_num, &robot[i].pos.first, &robot[i].pos.second);

    scanf("%d", &boat_num);
    for(int i = 0; i < boat_num; i ++)
        scanf("%d %d %d %d %d %d\n", &boat[i].id, &boat[i].goods_num, &boat[i].pos.first, &boat[i].pos.second, &boat[i].dir, &boat[i].sys_status);
    char okk[100];
    scanf("%s", okk);
}

void Plan::Output(){
    for(int i=0;i<robot_num;i++){
        if(robot[i].action_before_move == GET){
            printf("get %d\n", i);
        }
        else if(robot[i].action_before_move == PULL){
            printf("pull %d\n", i);
        }
        if(robot[i].action_move!=STAND){
            printf("move %d %d\n", i, robot[i].action_move);
        }
    }
    for(int i=0;i<boat_num;i++){
        if(boat[i].action==ROT_CLOCKWISE){
            printf("rot %d 0\n", i);
        }else if(boat[i].action==ROT_ANTICLOCKWISE){
            printf("rot %d 1\n", i);
        }else if(boat[i].action==SHIP){
            printf("ship %d\n", i);
        }else if(boat[i].action==DEPT){
            printf("dept %d\n", i);
        }else if(boat[i].action==BERTH){
            printf("berth %d\n", i);
        }
    }
}

void Plan::RobotRoutePlan(int rid, Position target){
    robot[rid].route.clear();
    // robot[rid].route_move.clear();
    robot[rid].p_route = 0;
    queue<Position> q;
    q.push(robot[rid].pos);
    int vis[N][N];
    memset(vis, 0, sizeof(vis));
    vis[robot[rid].pos.first][robot[rid].pos.second] = 1;
    int pre[N][N];
    memset(pre, -1, sizeof(pre));
    while(!q.empty()){
        Position now = q.front();
        q.pop();
        if(now == target){
            while(now != robot[rid].pos){
                robot[rid].route.push_back(now);
                now = make_pair(pre[now.first][now.second] / N, pre[now.first][now.second] % N);
            }
            reverse(robot[rid].route.begin(), robot[rid].route.end());
            break;
        }
        for(int i = 0; i < 4; i ++){
            int nx = now.first + rdx[i];
            int ny = now.second + rdy[i];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || vis[nx][ny] || robot_map[nx][ny] == 0) continue;
            vis[nx][ny] = 1;
            pre[nx][ny] = now.first * N + now.second;
            q.push(make_pair(nx, ny));
        }
    }
}

bool Plan::RobotRoutePlanForCollision(int rid, Position target){
    int temp_robot_map[N][N];
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            temp_robot_map[i][j] = robot_map[i][j];
        }
    }
    for(int i = 0; i < robot_num; i++){
        if(i == rid) continue;
        if(manhattan_distance(robot[i].pos, robot[rid].pos) <= 10){
            temp_robot_map[robot[i].pos.first][robot[i].pos.second] = 0;
        }
    }
    for(int i = 0; i < rid; i ++){
        if(robot[i].status == Collision){
            temp_robot_map[robot[i].pos.first][robot[i].pos.second] = 0;
            for(int j = robot[i].p_route; j < robot[i].route.size() && j<robot[i].p_route+50; j ++){
                temp_robot_map[robot[i].route[j].first][robot[i].route[j].second] = 0;
            }
        }
    }

    robot[rid].route.clear();
    robot[rid].p_route = 0;
    queue<Position> q;
    q.push(robot[rid].pos);
    int vis[N][N];
    memset(vis, 0, sizeof(vis));
    vis[robot[rid].pos.first][robot[rid].pos.second] = 1;
    int pre[N][N];
    memset(pre, -1, sizeof(pre));
    while(!q.empty()){
        Position now = q.front();
        q.pop();
        if(now == target){
            while(now != robot[rid].pos){
                robot[rid].route.push_back(now);
                now = make_pair(pre[now.first][now.second] / N, pre[now.first][now.second] % N);
            }
            reverse(robot[rid].route.begin(), robot[rid].route.end());
            break;
        }
        for(int i = 0; i < 4; i ++){
            int nx = now.first + rdx[i];
            int ny = now.second + rdy[i];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || vis[nx][ny] || temp_robot_map[nx][ny] == 0) continue;
            vis[nx][ny] = 1;
            pre[nx][ny] = now.first * N + now.second;
            q.push(make_pair(nx, ny));
        }
    }
    if(robot[rid].route[robot[rid].route.size()-1] != target){
        robot[rid].route.clear();
        return false;
    }else{
        return true;
    }
}

void Plan::RobotFindGoods(int rid){
    robot[rid].has_target = false;
    robot[rid].has_goods = false;
    robot[rid].target = make_pair(-1, -1);
    int distance[N][N];
    memset(distance, 0x3f, sizeof(distance));
    distance[robot[rid].pos.first][robot[rid].pos.second] = 0;
    queue<Position> q;
    q.push(robot[rid].pos);
    float val = -1;
    while(!q.empty()){
        Position now = q.front();
        q.pop();
        for(int i = 0; i < 4; i ++){
            int nx = now.first + rdx[i];
            int ny = now.second + rdy[i];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || distance[nx][ny] <= distance[now.first][now.second]+1 || robot_map[nx][ny] <= 0) continue;
            distance[nx][ny] = min(distance[now.first][now.second] + 1, distance[nx][ny]);
            q.push(make_pair(nx, ny));
            if(gds_time[nx][ny] + 1000 - frame_id > distance[nx][ny] && 1.0*gds_val[nx][ny]/distance[nx][ny] > val){
                robot[rid].has_target = true;
                robot[rid].target = make_pair(nx, ny);
                // val = gds_val[nx][ny];
                val = 1.0*gds_val[nx][ny]/distance[nx][ny];
                robot[rid].goods_val = val;
            }
        }
    }
    if(robot[rid].has_target){
        gds_val[robot[rid].target.first][robot[rid].target.second] = 0;
    }
}

void Plan::RobotFindBerth(int rid){
    robot[rid].has_target = false;
    robot[rid].target = make_pair(-1, -1);
    robot[rid].berth_id = -1;
    int distance[N][N];
    memset(distance, 0x3f, sizeof(distance));
    distance[robot[rid].pos.first][robot[rid].pos.second] = 0;
    queue<Position> q;
    q.push(robot[rid].pos);
    while(!q.empty()){
        Position now = q.front();
        q.pop();
        for(int i = 0; i < 4; i ++){
            int nx = now.first + rdx[i];
            int ny = now.second + rdy[i];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || distance[nx][ny] <= distance[now.first][now.second] + 1 || robot_map[nx][ny] <= 0) continue;
            distance[nx][ny] = distance[now.first][now.second] + 1;
            q.push(make_pair(nx, ny));
            if(grid[nx][ny] == 'B'){
                robot[rid].has_target = true;
                robot[rid].target = make_pair(nx, ny);
                break;
            }
        }
        if(robot[rid].has_target) break;
    }
    for(int i = 0; i < berth_num; i ++){
        if(manhattan_distance(berth[i].pos,robot[rid].target)<=3){
            robot[rid].berth_id = i;
            break;
        }
    }

    // for test
    robot[rid].berth_id = 1;
    robot[rid].has_target = true;
    robot[rid].target = berth[robot[rid].berth_id].pos;
    assert(robot[rid].berth_id != -1 && robot[rid].has_target);
}

void Plan::RobotDo(){
    // 先扫一遍有无碰撞的，如果有，重新规划路线
    for(int i=0;i<robot_num;i++){
        if(robot[i].last_pos==robot[i].pos && robot[i].has_target && robot[i].status!=Collision){
            robot[i].last_status = robot[i].status;
            robot[i].status = Collision;
        }
    }

    for(int i = 0; i < robot_num; i ++){
        if(robot[i].status==Collision){
            if(MY_DEBUG)cerr << "Robot " << i << " status is Collision" << endl;
            RobotRoutePlanForCollision(i, robot[i].target);
        }
    }
    for(int i=0;i<robot_num;i++){
        if(robot[i].status==Collision && robot[i].route.size()>0){
            robot[i].last_pos = Position(-1, -1);
            robot[i].status = robot[i].last_status;
        }
    }

    for(int i = 0; i < robot_num; i ++){
        robot[i].last_pos = robot[i].pos;
        robot[i].action_move = STAND;
        robot[i].action_before_move = RA_NOTHING;
        if(robot[i].status==Ready){
            // find a good
            RobotFindGoods(i);
            if(robot[i].has_target){
                if(MY_DEBUG)cerr << "Robot " << i << " find goods at " << robot[i].target.first << "," << robot[i].target.second << endl;
                RobotRoutePlan(i, robot[i].target);
                robot[i].status = ToGoods;
                robot[i].MoveNormal();
            }
            else{
                if(MY_DEBUG)cerr<<"Robot "<<i<<" has no target"<<endl;
            }
        }
        else if(robot[i].status==ToGoods){
            if(MY_DEBUG)cerr << "Robot " << i << " is moving to goods, target_position is "<<robot[i].target.first<<","<<robot[i].target.second << endl;
            if(MY_DEBUG)cerr<<"Robot "<<i<<" is at "<<robot[i].pos.first<<","<<robot[i].pos.second<<endl;
            if(robot[i].pos == robot[i].target){
                robot[i].action_before_move = GET;
                RobotFindBerth(i);
                if (robot[i].has_target){
                    robot[i].status = ToBerth;
                    RobotRoutePlan(i, robot[i].target);
                }
            }
            robot[i].MoveNormal();
        }
        else if(robot[i].status==ToBerth){
            if(MY_DEBUG)cerr << "Robot " << i << " is moving to berth, target_position is "<<robot[i].target.first<<","<<robot[i].target.second << endl;
            if(MY_DEBUG)cerr<<"Robot "<<i<<" is at "<<robot[i].pos.first<<","<<robot[i].pos.second<<endl;
            if(robot[i].pos == robot[i].target){
                if(robot[i].goods_num>0){
                    robot[i].action_before_move = PULL;
                    berth[robot[i].berth_id].GoodsIn(robot[i].goods_val);
                }
                RobotFindGoods(i);
                if(robot[i].has_target){
                    robot[i].status = ToGoods;
                    RobotRoutePlan(i, robot[i].target);
                    robot[i].MoveNormal();
                }
                else{
                    robot[i].status = Ready;
                }
            }
            else{
                robot[i].MoveNormal();
            }
        }
    }
}

void Plan::BoatRoutePlan(int bid, Position target){
    boat[bid].route.clear();
    boat[bid].route_move.clear();
    boat[bid].p_route = 0;
    queue<Position> q;
    queue<int> qdir;
    // q.push(boat[bid].DualPos());
    qdir.push(boat[bid].dir);
    int visDual[N][N];
    memset(visDual, 0, sizeof(visDual));
    Position init_dual_pos = boat[bid].DualPos();
    Position real_start_point(init_dual_pos.first+rdx[boat[bid].dir], init_dual_pos.second+rdy[boat[bid].dir]);
    q.push(real_start_point);
    visDual[init_dual_pos.first][init_dual_pos.second] = 1;
    visDual[real_start_point.first][real_start_point.second] = 1;
    int preDual[N][N];
    memset(preDual, -1, sizeof(preDual));
    preDual[real_start_point.first][real_start_point.second] = init_dual_pos.first * N + init_dual_pos.second;
    // int dir = boat[bid].dir;
    // if(MY_DEBUG)cerr << "Boat " << bid << " route plan from " << boat[bid].pos.first << "," << boat[bid].pos.second << " to " << target.first << "," << target.second << endl;
    while(!q.empty()){
        Position now = q.front();
        // if(MY_DEBUG)cerr << "Now at " << now.first << "," << now.second << endl;
        q.pop();
        int curr_dir = qdir.front();
        qdir.pop();
        Position target_dual_pos = get_dual_pos(target, curr_dir);    // 动态调整目标点，因为船只最后需要核心点到达
        if(now == target_dual_pos){
            while(now != init_dual_pos){
                boat[bid].route.push_back(now);
                now = make_pair(preDual[now.first][now.second] / N, preDual[now.first][now.second] % N);
            }
            reverse(boat[bid].route.begin(), boat[bid].route.end());
            int curr_dir = boat[bid].dir;
            // boat[bid].route_move.push_back(get_boat_move(curr_dir, get_boat_dir(boat[bid].pos, boat[bid].route[0])));
            // curr_dir = get_boat_dir(boat[bid].pos, boat[bid].route[0]);
            for(int i = 0; i < boat[bid].route.size()-1; i++){
                int next_dir = get_boat_dir(boat[bid].route[i], boat[bid].route[i+1]);
                boat[bid].route_move.push_back(get_boat_move(curr_dir, next_dir));
                curr_dir = next_dir;
            }
            boat[bid].route_move.push_back(SHIP);
            break;
        }
        else{
            for(int i = curr_dir; i < curr_dir+4; i++){
                int nx, ny, ndir;
                nx = now.first + rdx[i%4];
                ny = now.second + rdy[i%4];
                int idir = inverse_dir(curr_dir);
                ndir = i%4;
                int frontx,fronty;
                frontx = nx + rdx[ndir];
                fronty = ny + rdy[ndir];
                if(ndir==idir || nx < 0 || nx >= N-1 || ny < 0 || ny >= N-1 || frontx<0 || frontx>=N-1 ||
                    fronty<0 || fronty>=N-1 || visDual[nx][ny] || boat_map_dual[nx][ny] <= 0) continue;
                visDual[nx][ny] = 1;
                preDual[nx][ny] = now.first * N + now.second;   // 这个只是个编码，应该不影响
                q.push(Position(nx, ny));
                qdir.push(ndir);
            }
        }
    }
    // ofstream out("boat_route.txt");
    // out<<boat[bid].DualPos().first<<" "<<boat[bid].DualPos().second<<endl;
    // for(int i = 0; i < boat[bid].route.size(); i ++){
    //     out << boat[bid].route[i].first << " " << boat[bid].route[i].second << endl;
    // }
    // ofstream out2("boat_route_move.txt");
    // for(int i = 0; i < boat[bid].route_move.size(); i ++){
    //     out2 << boat[bid].route_move[i] << endl;
    // }
}

bool Plan::BoatFindBerth(int bid){
    boat[bid].target_berth = -1;
    int min_dis = 0x3f3f3f3f;
    for(int i = 0; i < berth_num; i ++){
        int dis = manhattan_distance(boat[bid].pos, berth[i].pos);
        if(dis < min_dis){
            min_dis = dis;
            boat[bid].target_berth = i;
        }
    }
    if(boat[bid].target_berth != -1){
        // 进行一次直接的bfs，确定目标点
        Position target = berth[boat[bid].target_berth].pos;
        queue<Position> q;
        q.push(boat[bid].pos);
        int distance[N][N];
        memset(distance, 0x3f, sizeof(distance));
        distance[boat[bid].pos.first][boat[bid].pos.second] = 0;
        while(!q.empty()){
            Position now = q.front();
            q.pop();
            if(now == target || ((grid[now.first][now.second] == 'K' || grid[now.first][now.second] == 'B') && manhattan_distance(now, target) <= 8)){
                boat[bid].target = now;
                break;
            }
            for(int i = 0; i < 4; i ++){
                int nx = now.first + rdx[i];
                int ny = now.second + rdy[i];
                if(nx < 0 || nx >= N || ny < 0 || ny >= N || distance[nx][ny] <= distance[now.first][now.second] + 1 || boat_map[nx][ny] <= 0) continue;
                distance[nx][ny] = distance[now.first][now.second] + 1;
                q.push(make_pair(nx, ny));
            }
        }
        boat[bid].has_target = true;
        return true;
    }
    return false;
}

bool Plan::BoatFindDelivery(int bid){
    boat[bid].has_target = false;
    boat[bid].target = make_pair(-1, -1);
    int min_dis = 0x3f3f3f3f;
    for(int i=0;i<delivery_point.size();i++){
        int dis = manhattan_distance(boat[bid].pos, delivery_point[i]);
        if(dis < min_dis){
            min_dis = dis;
            boat[bid].target = delivery_point[i];
        }
    }
    if(boat[bid].target != make_pair(-1, -1)){
        boat[bid].has_target = true;
        return true;
    }
    return false;
}

void Plan::BoatDo(){
    for(int i = 0; i < boat_num; i ++){
        if(boat[i].sys_status==1){
            if(MY_DEBUG)cerr << "Boat " << i << " is in system status 1" << endl;
            boat[i].action = BOAT_NOTHING;
            continue; // 恢复状态，不做任何操作
        }
        if(boat[i].status == BReady && boat[i].sys_status == 0){
            if(MY_DEBUG)cerr << "Boat " << i << " is ready" << endl;
            if(BoatFindBerth(i)){
                if(MY_DEBUG)cerr << "Boat " << i << " find berth at " << boat[i].target.first << "," << boat[i].target.second << endl;
                BoatRoutePlan(i, boat[i].target);
                if(MY_DEBUG)cerr << "Boat " << i << " route plan done" << endl;
                ShipIn(boat[i].target_berth, i);
                boat[i].status = BToBerth;
                boat[i].MoveNormal();
            }
        }
        else if(boat[i].status == BToBerth && boat[i].sys_status == 0){
            if(MY_DEBUG)cerr << "Boat " << i << " is moving to berth" << endl;
            if(boat[i].pos == boat[i].target){
                boat[i].action = BERTH;
                boat[i].status = BLoading;
            }
            else{
                boat[i].MoveNormal();
            }
        }
        else if(boat[i].status == BToDelivery && boat[i].sys_status == 0){
            if(boat[i].has_target==false){
                BoatFindDelivery(i);
                BoatRoutePlan(i, boat[i].target);
            }
            if(boat[i].pos == boat[i].target){
                if(BoatFindBerth(i)){
                    BoatRoutePlan(i, boat[i].target);
                    boat[i].status = BToBerth;
                    boat[i].MoveNormal();
                }
            }
            else{
                boat[i].MoveNormal();
            }
        }
        else if(boat[i].status == BLoading && boat[i].sys_status == 2){
            berth[boat[i].target_berth].boat_id = i;
            boat[i].action = BOAT_NOTHING;
            if(berth[boat[i].target_berth].goods_num==0 || boat[i].goods_num>=boat_capacity-berth[boat[i].target_berth].loading_speed){
                ShipOut(boat[i].target_berth, i);
                boat[i].action = DEPT;
                boat[i].status = BToDelivery;
                boat[i].has_target = false;
            }
        }
    }
}

void Plan::BerthDo(){
    for(int i = 0; i < berth_num; i ++){
        if(berth[i].goods_num > 0 && berth[i].boat_id != -1 && boat[berth[i].boat_id].sys_status==2){
            int loading_num = min(berth[i].loading_speed, min(berth[i].goods_num, boat_capacity - boat[berth[i].boat_id].goods_num));
            berth[i].goods_num -= loading_num;
            boat[berth[i].boat_id].goods_num += loading_num;
            if(berth[i].goods_num < 0) berth[i].goods_num = 0;
        }
    }
}

void Plan::ShipIn(int berth_id, int boat_id) {
    boat[boat_id].target_berth = berth_id;
    berth[berth_id].boat_lst.insert(boat_id);
}

void Plan::ShipOut(int berth_id, int boat_id) {
    berth[berth_id].boat_lst.erase(boat_id);
    boat[boat_id].target_berth = -1;
}

void Plan::Process(){

    // cerr<<robot_num<<" "<<robot_num<<endl;
    // for(int i = 0; i < robot_num; i ++)
    //     printf("move %d %d\n", i, rand() % 4);

    // for(int i = 0; i < boat_num; i ++){
    //     int status = abs(rand()) % 2;
    //     if(status == 1)
    //         printf("ship %d\n", i);
    //     else
    //         printf("rot %d %d\n", i, rand() % 2);
    //     }

    RobotDo();
    // if(MY_DEBUG)cerr<<"Robot done"<<endl;
    BoatDo();
    // if(MY_DEBUG)cerr<<"Boat done"<<endl;
    BerthDo();
    // if(MY_DEBUG)cerr<<"Berth done"<<endl;

    if(money >= robot_price && robot_num <= 5){
        money-=robot_price;
        printf("lbot %d %d\n", robot_purchase_point[0].first, robot_purchase_point[0].second);
        robot.push_back(Robot(robot_purchase_point[0].first, robot_purchase_point[0].second));
    }

    if(money >= boat_price && boat_num <= 1){
        money-=boat_price;
        printf("lboat %d %d\n", boat_purchase_point[0].first, boat_purchase_point[0].second);
        boat.push_back(Boat(boat_purchase_point[0].first, boat_purchase_point[0].second));
    }

    if(MY_DEBUG){
        // Summary();
    }
    Output();
}

void Plan::Summary(){
    cerr<<"Summary"<<endl;
    cerr<<"Frame "<<frame_id<<endl;
    cerr<<"Money: "<<money<<endl;
    cerr<<"Robot: "<<robot_num<<endl;
    cerr<<"Berth: "<<berth_num<<endl;
    for(int i=0;i<berth_num;i++){
        cerr<<"Berth "<<i<<" at "<<berth[i].pos.first<<","<<berth[i].pos.second<<"  goods_num: "
            <<berth[i].goods_num<<"  goods_val: "<<berth[i].goods_val<<endl;
    }
}