#ifndef fileSystemCreator_h
#define fileSystemCreator_h
#include <string>
#include <math.h>
#include "definitions.h"
using namespace std;

typedef struct
{
    int8 indexSize;
    int8 clusterSize;
    int8 indexStart;
    int8 rootStart;
} FileSystem;

void createFile(FileSystem fs, char *filename);
#endif