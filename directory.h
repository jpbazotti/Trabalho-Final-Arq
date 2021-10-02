#ifndef directory_h
#define directory_h
#include <definitions.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "clusterManagement.h"

using namespace std;

typedef struct {
char name[8];
char extension[3];
int8 startCluster;
}dirEntry;


bool DIR(FILE *dir);
void createDir(FILE *file, int8 curDir,char dirName[8]);
#endif