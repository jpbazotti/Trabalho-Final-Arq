#ifndef clusterManagement_h
#define clusterManagement_h
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int offSetCalc(int8 indexSize,int8 clusterSize,int8 curDir);
int findAvailableCluster(FILE *file, int8 indexSize, int8 clusterSize);
int findNewCluster(FILE *file, int8 indexSize, int8 clusterSize,int8 currentIndex);
#endif