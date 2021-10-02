#include "fileSystem.h"

void getFileName(dirEntry *Entry){
	//pastas sao arquivos, porem a sua extensao nao aparece

	if(strcmp(Entry->extension,"dir")==0){
		cout << Entry->name << "\n";
	}else{
		cout << Entry->name << "." << Entry->extension << "\n";
	}
}

void DIR(FILE *dir){
	while(1){
		dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, dir);
		if(Entry.name[0] == 28){
			break;
		}
		getFileName(&Entry);
	}
}

//bool remove(char* path){}

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize)
{
    int8 eof = 28;
    int dirOffset = offSetCalc(indexSize, clusterSize,curDir);
    dirEntry newEntry;
    strcpy(newEntry.name, dirName);
    strcpy(newEntry.extension, "dir");
    newEntry.startCluster = findAvailableCluster(file, indexSize, clusterSize);
    fseek(file, dirOffset, SEEK_SET);
    int8 current;
    do
    {
        fread(&current, sizeof(int8), 1, file);
    } while (current != 28);
    fseek(file, -1, SEEK_CUR);
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    fwrite(&eof, sizeof(int8), 1, file);
}
void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize)
{
        int8 eof = 28;
        int dirOffset = offSetCalc(indexSize, clusterSize,curDir);
        dirEntry newEntry;
        strcpy(newEntry.name, fileName);
        strcpy(newEntry.extension, "txt");
        newEntry.startCluster = findAvailableCluster(file, indexSize, clusterSize);
        fseek(file, dirOffset, SEEK_SET);
        int8 current;
        do
        {
            fread(&current, sizeof(int8), 1, file);
        } while (current != 28);
        fseek(file, -1, SEEK_CUR);
        fwrite(&newEntry, sizeof(dirEntry), 1, file);
        fwrite(&eof, sizeof(int8), 1, file);
    }