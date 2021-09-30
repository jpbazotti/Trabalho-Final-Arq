#include <definitions.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

boolean DIR(FILE *dir){
  while(1){
    char *cache;
    fread(&cache, sizeof(int8), 1, dir);
    if(cache[0] == 28){
      break;
    }
    fseek(dir, 0, SEEK_CUR-1);
    fread(&cache, sizeof(int8), 8, dir);
    cout << cache << ".";
    fread(&cache, sizeof(int8), 3, dir);
    cout << cache << "\n";
    fseek(dir, 1, SEEK_CUR);
  }
}
