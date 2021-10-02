#include "directory.h"

void DIR(FILE *dir){
	while(1){
		dirEntry Entry;
		fgets(Entry.name, 9, dir);
		if(Entry.name[0] == 28){
			break;
		}
		fgets(Entry.extension, 4, dir);
		cout << Entry.name << "." << Entry.extension << "\n";
	}
}

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
