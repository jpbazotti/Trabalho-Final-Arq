#include <iostream>
#include <fstream>
#include <string>
#include "fileSystemCreator.h"
#include "directory.h"
#include "file.h"
using namespace std;
int main(){
    FileSystem fs;
    FILE *file;
    fs.indexSize=8;
    fs.clusterSize=15;
    fs.indexStart=4;
    fs.rootStart=0;
    createFile(fs,"file.bin");
    int8 curDir=fs.rootStart;
    int startOffset=offSetCalc(fs.indexSize,fs.clusterSize,fs.rootStart);
    if((file = fopen("main.bin","r+"))){
        char dirName[9]="teste";
        char extName[4]="txt";
        fseek(file,startOffset ,SEEK_SET);
        createDir(file,curDir,dirName,fs.clusterSize,fs.indexSize);
        createFile(file,curDir,dirName,extName,fs.clusterSize,fs.indexSize);
        
        fseek(file,startOffset ,SEEK_SET);
        DIR(file);
    }
    return 0;
}