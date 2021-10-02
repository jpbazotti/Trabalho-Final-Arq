#ifndef file_h
#define file_h
#include <definitions.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "directory.h"
#include "clusterManagement.h"
using namespace std;
void createFile(FILE *file, int8 curDir,char fileName[9],char extensionName[4],int8 clusterSize, int8 indexSize);

#endif