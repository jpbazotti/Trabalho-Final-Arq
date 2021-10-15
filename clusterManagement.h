#ifndef clusterManagement_h
#define clusterManagement_h
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
int offSetCalc(int8 indexSize,int8 clusterSize,int8 curDir);
int findAvailableCluster(FILE *file, int8 indexSize, int8 clusterSize);
int findNewCluster(FILE *file, int8 indexSize,int8 currentIndex);
int availableClusters(FILE *file, int8 indexSize);
int clusterInFile(FILE *file, int8 startCluster);
#endif