#ifndef fileSystem_h
#define fileSystem_h
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "clusterManagement.h"
#include "fileSystemCreator.h"
using namespace std;
typedef struct
{
    char name[9];
    char extension[4];
    int8 startCluster;
} dirEntry;

//Vai para o cluster de numero clusterIndex
bool gotoCluster(FILE *file, int8 clusterIndex, FileSystem fs);

//Entra na pasta com o nome que foi passado, se ela existir no diretorio atual. Retorna o cluster atual no ponteiro clusterIndex
bool gotoDir(char *name, FILE *dir, FileSystem fs, int8 *clusterIndex);

//Faz o comando cd e retorna o cluster atual no ponteiro clusterIndex
bool CD(char *names, FILE *dir, FileSystem fs, int8 *clusterIndex);

//retorna o nome do arquivo
void getFileName(dirEntry Entry);

//faz o commando DIR em um cluster somente
void DIR(FILE *dir);

//faz o comando RM
bool remove(char *name);

//cria novo diretorio em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
void createDir(FILE *file, int8 curDir,const char dirName[9], int8 clusterSize, int8 indexSize);
//cria novo arquivo em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
void createFile(FILE *file, int8 curDir, const char fileName[9],const char extensionName[4], int8 clusterSize, int8 indexSize);

#endif