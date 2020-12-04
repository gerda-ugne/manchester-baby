#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "simulator.h"

int main()
{

    allocateMemory();
    fillStore("BabyTest1-MC.txt");
    //displayStore();

    runSimulator();
    freeMemory();

    return 0;
}