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

        gotoCluster(file, curDir, fs);
        cout << "1\n";
        DIR(file);
        gotoCluster(file, curDir, fs);
        char arquivo2[6] = "teste";
        gotoDir(arquivo2, file, fs, &curDir);
        gotoCluster(file, curDir, fs);
        cout << "2\n";
        DIR(file);
        gotoCluster(file, curDir, fs);
        char path1[20] = "/root";
        cout << "3\n";
        CD(path1, file, fs, &curDir);
        gotoCluster(file, curDir, fs);
        cout << "4\n";
        DIR(file);
        gotoCluster(file, curDir, fs);
        
        char path2[20] = "/root/teste";
        cout << "5\n";
        CD(path2, file, fs, &curDir);
        gotoCluster(file, curDir, fs);
        cout << "6\n";
        DIR(file);
        gotoCluster(file, curDir, fs);

        char path3[20] = "/root/tes";
        cout << "7\n";
        CD(path3, file, fs, &curDir);
        gotoCluster(file, curDir, fs);
        cout << "8\n";
        DIR(file);
        gotoCluster(file, curDir, fs);
        cout << curDir;
    }
    
    return 0;
}