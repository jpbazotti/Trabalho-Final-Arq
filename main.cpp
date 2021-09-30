#include <iostream>
#include <fstream>
#include <string>
#include "fileSystemCreator.h"
using namespace std;
int main(){
    FileSystem fs;
    fs.indexSize=8;
    fs.clusterSize=15;
    fs.indexStart=4;
    fs.rootStart=0;
    createFile(fs,"file.bin");
    return 0;
}