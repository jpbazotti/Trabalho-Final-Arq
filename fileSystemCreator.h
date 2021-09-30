#ifndef fileSystemCreator_h
#define fileSystemCreator_h
#include <string>
#include <math.h>
using namespace std;

typedef struct {
unsigned char indexSize;
unsigned char clusterSize;
unsigned char indexStart;
unsigned char rootStart;
}FileSystem;

void createFile(FileSystem fs, string filename);
#endif