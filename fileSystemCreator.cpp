#include "fileSystemCreator.h"
using namespace std;

void createFile(FileSystem fs,string filename){
    FILE *file;
    int8 full =255;
    int8 empty =0;
    int8 eof = 28;
    if((file = fopen("main.bin","wb"))){
        fwrite(&fs,sizeof(FileSystem),1,file);
        for(int i=0;i<pow(2,8);i++){
             fwrite(&empty,sizeof(int8),1,file);
        }
        for(int i=0;i<pow(2,23);i++){
             fwrite(&empty,sizeof(int8),1,file);
        }
        fseek(file,fs.indexStart,SEEK_SET);
        fwrite(&full,sizeof(int8),1,file);
        fseek(file,fs.indexStart+pow(2,fs.indexSize),SEEK_SET);
        fwrite(&eof,sizeof(int8),1,file);
        fclose(file);
    }
}