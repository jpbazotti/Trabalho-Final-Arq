#include <iostream>
#include <fstream>
#include <string>
#include "fileSystemCreator.h"
#include "fileSystem.h"
using namespace std;
int main()
{
    FileSystem fs;
    FILE *file;
    fs.indexSize = 8;
    fs.clusterSize = 15;
    fs.indexStart = 4;
    fs.rootStart = 0;
    createFile(fs, "file.bin");
    int8 curDir = fs.rootStart;
    int startOffset = offSetCalc(fs.indexSize, fs.clusterSize, fs.rootStart);
    if ((file = fopen("main.bin", "r+")))
    {
        char dirName[9] = "teste";
        char extName[4] = "txt";
        fseek(file, startOffset, SEEK_SET);
        createDir(file, curDir, dirName, fs.clusterSize, fs.indexSize);
        createFile(file, curDir, dirName, extName, fs.clusterSize, fs.indexSize);

        fseek(file, startOffset, SEEK_SET);
        cout << "0\n";
        DIR(file);
        fseek(file, startOffset, SEEK_SET);
        char arquivo1[8] = "root";
        arquivo1[0] = 'r';
        arquivo1[1] = 'o';
        arquivo1[2] = 'o';
        arquivo1[3] = 't';
        arquivo1[4] = 0;
        arquivo1[5] = 0;
        arquivo1[6] = 0;
        arquivo1[7] = 0;
        char arquivo2[8] = "teste";
        gotoDir(arquivo1, file, fs);
        gotoDir(arquivo2, file, fs);
        cout << "1\n";
        DIR(file);
        fread(&arquivo1, sizeof(dirEntry), 1, file);
        fseek(file, startOffset, SEEK_SET);
        char path[20] = "/root/teste";
        CD(path, file, fs);
        cout << "2\n";
        DIR(file);
    }
    
    return 0;
}