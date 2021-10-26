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
    string inputPath2;
    char *convertedPath;
    char *convertedPath2;
    char *convertedCurrentPath;
    if ((file = fopen(fileName, "r+")))
    {
        fseek(file, startOffset, SEEK_SET);
        while (1)
        {
            cout << "\n"
                 << Path << " ";
            cin >> command;
            if (command.compare("CD") == 0 || command.compare("cd") == 0)
            {
                cin >> inputPath;
                convertedPath = new char[inputPath.size() + 1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(convertedPath[inputPath.size()-1] == '/'){
                    convertedPath[inputPath.size()-1] = '\0';
                }
                if (CD(convertedPath, file, fs, &curDir))
                {
                    Path = inputPath;
                }
                else
                {
                    cout << "Erro: arquivo nao encontrado";
                }
                gotoCluster(file, curDir, fs);
            }
            else if (command.compare("DIR") == 0 || command.compare("dir") == 0)
            {
                DIR(file, &curDir, fs);
                gotoCluster(file, curDir, fs);
            }
            else if (command.compare("MKDIR") == 0 || command.compare("mkdir") == 0)
            {
                cin >> inputPath;
                if (inputPath.length() > 9 || inputPath.find(".") != std::string::npos || inputPath.find("/") != std::string::npos || inputPath.find(" ") != std::string::npos || inputPath.find("<") != std::string::npos || inputPath.find(">") != std::string::npos)
                {
                    cout << "Erro: nome de diretorio invalido\n";
                }
                else
                {
                    if(!createDir(file, curDir, inputPath.c_str(), fs)){
                        cout << "Erro: sem espaco\n";
                    }
                    gotoCluster(file, curDir, fs);
                }
            }
            else if (command.compare("MKFILE") == 0 || command.compare("mkfile") == 0)
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
                if (name.length() > 9 || extension.length() > 3 || inputPath.find("/") != std::string::npos || extension.compare("txt") != 0 || inputPath.find(" ") != std::string::npos || inputPath.find("<") != std::string::npos || inputPath.find(">") != std::string::npos)
                {
                    cout << "Nome de arquivo invalido\n";
                }
                else
                {
                    if(!createFile(file, curDir, name.c_str(), fs)){
                        cout << "Erro: sem espaco\n";
                    }
                    gotoCluster(file, curDir, fs);
                }
            }
            else if (command.compare("RM") == 0 || command.compare("rm") == 0)
            {
                cin >> inputPath;
                convertedPath = new char[inputPath.size() + 1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(convertedPath[inputPath.size()-1] == '/'){
                    convertedPath[inputPath.size()-1] = '\0';
                }
                if (validPath(convertedPath))
                {
                    if(RM(convertedPath, file, fs, &curDir)){
                        if(Path.find(inputPath) != std::string::npos){
                            Path = "/root";
                            curDir = fs.rootStart;
                            cout << "Aviso: enviado para o root\n";
                        }
                    }
                    else
                    {
                        cout << "Erro: arquivo nao encontrado/pasta nao vazia\n";
                    }
                }
                else
                {
                    cout << "Erro: caminho mal formatado\n";
                }
                gotoCluster(file, curDir, fs);

            }
            else if (command.compare("RENAME") == 0 || command.compare("rename") == 0)
            {
                cin >> inputPath;
                cin >> inputPath2;
                convertedPath = new char[inputPath.size() + 1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(convertedPath[inputPath.size()-1] == '/'){
                    convertedPath[inputPath.size()-1] = '\0';
                }

                convertedPath2 = new char[inputPath2.size() + 1];
                copy(inputPath2.begin(), inputPath2.end(), convertedPath2);
                convertedPath2[inputPath2.size()] = '\0';
                if(convertedPath2[inputPath2.size()-1] == '/'){
                    convertedPath2[inputPath2.size()-1] = '\0';
                }

                convertedCurrentPath = new char[Path.size() + 1];
                copy(Path.begin(), Path.end(), convertedCurrentPath);
                convertedCurrentPath[Path.size()] = '\0';
                if(convertedCurrentPath[Path.size()-1] == '/'){
                    convertedCurrentPath[Path.size()-1] = '\0';
                }
                if (validPath(convertedPath))
                {
                    if (!rename(convertedPath, file, convertedPath2, fs, &curDir))
                    {
                        cout << "Erro: erro nos argumentos / arquivo nao encontrado\n";
                    }
                    CD(convertedCurrentPath, file, fs, &curDir);
                    gotoCluster(file, curDir, fs);
                }
                else
                {
                    cout << "Erro: caminho mal formatado\n";
                }
            }
            else if (command.compare("MOVE") == 0 || command.compare("move") == 0)
            {
                cin >> inputPath;
                cin >> inputPath2;
                convertedPath = new char[inputPath.size() + 1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(convertedPath[inputPath.size()-1] == '/'){
                    convertedPath[inputPath.size()-1] = '\0';
                }

                convertedPath2 = new char[inputPath2.size() + 1];
                copy(inputPath2.begin(), inputPath2.end(), convertedPath2);
                convertedPath2[inputPath2.size()] = '\0';
                if(convertedPath2[inputPath2.size()-1] == '/'){
                    convertedPath2[inputPath2.size()-1] = '\0';
                }

                convertedCurrentPath = new char[Path.size() + 1];
                copy(Path.begin(), Path.end(), convertedCurrentPath);
                convertedCurrentPath[Path.size()] = '\0';
                if(convertedCurrentPath[Path.size()-1] == '/'){
                    convertedCurrentPath[Path.size()-1] = '\0';
                }
                if (validPath(convertedPath) && validPath(convertedPath2))
                {
                    if (!MOVE(convertedPath,convertedPath2,file , fs, &curDir))
                    {
                        cout << "Erro: erro nos argumentos / arquivo nao encontrado\n";
                    }
                    CD(convertedCurrentPath, file, fs, &curDir);
                    gotoCluster(file, curDir, fs);
                }
                else
                {
                    cout << "Erro: caminho mal formatado\n";
                }
            }
            else if (command.compare("EDIT") == 0 || command.compare("edit") == 0)
            {
                cin >> inputPath;
                getline(cin,inputPath2);
                convertedPath = new char[inputPath.size() + 1];
                copy(inputPath.begin(), inputPath.end(), convertedPath);
                convertedPath[inputPath.size()] = '\0';
                if(convertedPath[inputPath.size()-1] == '/'){
                    convertedPath[inputPath.size()-1] = '\0';
                }

                convertedPath2 = new char[inputPath2.size() + 1];
                copy(inputPath2.begin(), inputPath2.end(), convertedPath2);
                convertedPath2[inputPath2.size()] = '\0';
                if(convertedPath2[inputPath2.size()-1] == '/'){
                    convertedPath2[inputPath2.size()-1] = '\0';
                }

                convertedCurrentPath = new char[Path.size() + 1];
                copy(Path.begin(), Path.end(), convertedCurrentPath);
                convertedCurrentPath[Path.size()] = '\0';
                if(convertedCurrentPath[Path.size()-1] == '/'){
                    convertedCurrentPath[Path.size()-1] = '\0';
                }
                cout << convertedPath2;
                if (validPath(convertedPath))
                {
                    if (!edit(convertedPath,file ,convertedPath2, fs, &curDir))
                    {
                        cout << "Erro: erro nos argumentos / arquivo nao encontrado\n";
                    }
                    CD(convertedCurrentPath, file, fs, &curDir);
                    gotoCluster(file, curDir, fs);
                }
                else
                {
                    cout << "Erro: caminho mal formatado\n";
                }
            }
            else{
                cout << "Erro: comando nao existente\n";
            }
            cin.clear();
            fflush(stdin);
        }
        return 0;
    }
}