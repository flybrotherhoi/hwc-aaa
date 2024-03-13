#include <bits/stdc++.h>
#include "Plan.h"
using namespace std;

int main()
{
    Plan plan{};
    plan.Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = plan.Input();
        for(int i = 0; i < robot_num; i ++)
            printf("move %d %d\n", i, rand() % 4);
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
