#include "fileSystemCreator.h"
using namespace std;

void createFile(FileSystem fs,string filename){
    FILE *file;
    unsigned char full =255;
    unsigned char empty =0;
    if((file = fopen("main.bin","wb"))){
        fwrite(&fs,sizeof(FileSystem),1,file);
        for(int i=0;i<pow(2,8);i++){
             fwrite(&empty,sizeof(unsigned char),1,file);
        }
        for(int i=0;i<pow(2,23);i++){
             fwrite(&empty,sizeof(unsigned char),1,file);
        }
        fseek(file,fs.indexStart,SEEK_SET);
        fwrite(&full,sizeof(unsigned char),1,file);
    }
}