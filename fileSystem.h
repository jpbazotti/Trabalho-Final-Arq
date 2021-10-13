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

//Faz o comando CD e retorna o cluster atual no ponteiro clusterIndex
bool CD(char *names, FILE *dir, FileSystem fs, int8 *clusterIndex);

//retorna o nome do arquivo
char *getFileName(dirEntry Entry);

//retorna se o diretorio esta vazio ou nao
bool isDirEmpty(FILE *file);

//faz o commando DIR em um cluster somente
void DIR(FILE *file,int8 *clusterIndex, FileSystem fs);

//Valida o path
bool validPath(char *path);

//Quebra a string para encontrar o nome do arquivo
char *breakePath(char *path);

//faz o comando RM
bool RM(char *path, FILE *file, FileSystem fs, int8 *clusterIndex);

//faz o comando MOVE
bool MOVE(char *path1, char *path2, FILE *file, FileSystem fs, int8 *clusterIndex);

//cria novo diretorio em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
void createDir(FILE *file, int8 curDir, const char dirName[9], FileSystem fs);

//cria novo arquivo em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
void createFile(FILE *file, int8 curDir, const char fileName[9], FileSystem fs);

//renomea um arquivo 
bool rename(char *path, FILE *file, char *newFileName, FileSystem fs, int8 *clusterIndex);

//edita conteudo do arquivo
bool edit(char *path, FILE *file, char *content, FileSystem fs, int8 *clusterIndex);

#endif