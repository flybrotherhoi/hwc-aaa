#include <bits/stdc++.h>
#include "Plan.h"
using namespace std;

int main(int argc, char *argv[])
{
    Plan plan{};
    plan.Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = plan.Input();
        plan.Process();
        plan.Output();
        plan.SaveLog();
        puts("OK");
        fflush(stdout);
    }
    // plan.Summary();

    return 0;
}
