#include "directory.h""

bool DIR(FILE *dir){
  while(1){
    char *cache;
    fread(&cache, sizeof(int8), 1, dir);
    if(cache[0] == 28){
      break;
    }
    fseek(dir, 0, SEEK_CUR-1);
    fgets(&cache, sizeof(int8), 9, dir);
    cout << cache << ".";
    fgets(&cache, sizeof(int8), 4, dir);
    cout << cache << "\n";
    fseek(dir, 1, SEEK_CUR);
  }
}

void createDir(FILE *file, int8 curDir,char dirName[8],int8 clusterSize, int8 indexSize){
    int8 eof = 28;
    int dirOffset= 4 + pow(2,indexSize) + curDir * pow(2,clusterSize); 
    dirEntry newEntry;
    strcpy(newEntry.name,dirName);
    strcpy(newEntry.extension,"dir");
    newEntry.startCluster=findAvailableCluster(file,indexSize,clusterSize);
    fseek(file,dirOffset,SEEK_SET);
    int8 current;
    do{
        fread(&current,sizeof(int8),1,file);
    }while(current != 28);
    fseek(file,-1,SEEK_CUR);
    fwrite(&newEntry,sizeof(dirEntry),1,file);
    fwrite(&eof,sizeof(int8),1,file);
}