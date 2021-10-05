#include <iostream>
#include <fstream>
#include <string>
#include "fileSystemCreator.h"
#include "fileSystem.h"
using namespace std;
int main()
{
    cout << "start \n";
    FileSystem fs;
    FILE *file;
    fs.indexSize = 8;
    fs.clusterSize = 15;
    fs.indexStart = 4;
    fs.rootStart = 0;
    char fileName[] = "file.bin";
    createFile(fs, fileName);
    int8 curDir = fs.rootStart;
    int startOffset = offSetCalc(fs.indexSize, fs.clusterSize, fs.rootStart);
    string Path = "/root";
    string command;
    string inputPath;
    string outputPath;
    if ((file = fopen(fileName, "r+")))
    {
        fseek(file, startOffset, SEEK_SET);
        while (1)
        {
            cout << "\n" << Path << " ";
            cin >> command;
            if (command.compare("CD") == 0|| command.compare("cd") == 0)
            {
                cin >> inputPath;
                char * convertedPath = new char[inputPath.size()+1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(CD(convertedPath,file,fs,&curDir)){
                    Path=inputPath;
                }
                gotoCluster(file, curDir, fs);

            }
            else if (command.compare("DIR") == 0|| command.compare("dir") == 0)
            {
                DIR(file);
                gotoCluster(file, curDir, fs);

            }
            else if (command.compare("MKDIR") == 0||command.compare("mkdir") == 0)
            {
                cin >> inputPath;
                if (inputPath.length() > 9 || inputPath.find(".") != std::string::npos || inputPath.find("/") != std::string::npos|| inputPath.find(" ") != std::string::npos)
                {
                    cout << "Nome de diretorio invalido\n";
                }
                else
                {
                    createDir(file, curDir, inputPath.c_str(), fs.clusterSize, fs.indexSize);
                    gotoCluster(file, curDir, fs);

                }
            }
            else if (command.compare("MKFILE")==0 || command.compare("mkfile")==0)
            {
                cin >> inputPath;
                string splitter = ".";
                string name;
                string extension;
                auto start = 0U;
                auto end = inputPath.find(splitter);
                name = inputPath.substr(start, end - start);
                start = end + splitter.length();
                end = inputPath.find(splitter, start);
                extension = inputPath.substr(start, end - start);
                cout << name << " " << extension << "\n";
                if (name.length() > 9 ||extension.length()>3|| inputPath.find("/") != std::string::npos || extension.compare("txt") != 0|| inputPath.find(" ") != std::string::npos)
                {
                    cout << "Nome de arquivo invalido";
                }
                else
                {
                    createFile(file, curDir, name.c_str(), extension.c_str(), fs.clusterSize, fs.indexSize);
                    gotoCluster(file, curDir, fs);
                }
            }else{
                char teste[]="/root/teste/a";
                if(validPath(teste))
                cout << "a";

                char teste2[]="/root/teste//a";
                if(validPath(teste2))
                cout << "b";
                //cout << "Comando nao reconhecido";
            }

        }

        return 0;
    }
}