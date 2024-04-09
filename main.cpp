#include <bits/stdc++.h>
#include "Plan.h"
using namespace std;

int main(int argc, char *argv[])
{
    Plan plan;
    plan.Init();
    while(scanf("%d", &plan.frame_id) != EOF)
    {
        plan.Input();
        plan.Process();
        puts("OK");
        fflush(stdout);
    }
    return 0;
}
