#include <string.h>
#include "fileSystem.h"
#include "fileSystemCreator.h"

char *getFileName(dirEntry *Entry)
{
    //pastas sao arquivos, porem a sua extensao nao aparece

    if (strcmp(Entry->extension, "dir") == 0)
    {
        return Entry->name;
    }
    else
    {
        return strcat(strcat(Entry->name, "."), Entry->extension);
    }
}

bool gotoCluster(FILE *file, int8 clusterIndex, FileSystem fs)
{
    int offset = offSetCalc(fs.indexSize, fs.clusterSize, clusterIndex);
    if (offset < offSetCalc(fs.indexSize, fs.clusterSize, fs.indexSize + 1))
    {
        fseek(file, offset, SEEK_SET);
        return true;
    }
    else
    {
        return false;
    }
}

bool gotoDir(char *name, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    while (1)
    {
        dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, file);
        if (Entry.name[0] == 28)
        {
            return false;
        }
        char *aux = getFileName(&Entry);
        if (strcmp(aux, name) == 0)
        {
            if (strcmp(Entry.extension, "dir") == 0)
            {
                *clusterIndex = Entry.startCluster;
                //fseek is now on CD function
                //fseek(dir, offSetCalc(fs.indexSize, fs.clusterSize, *clusterIndex), SEEK_SET);
                return true;
            }
        }
    }
}

bool CD(char *path, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    //Verifica se o path contem "root"
    char *name;
    name = strtok(path, "/");
    if (strcmp(name, "root") != 0)
    {
        //cout removido, usar retorno da funcao no lugar
        //cout << "Arquivo ou pasta nao encontrado.\n";
        return false;
    }
    //reseta o ponteiro para o diretorio root (necessario para o funcionamento da funcao)
    int8 index = fs.rootStart;
    int offset = offSetCalc(fs.indexSize, fs.clusterSize, index);
    fseek(file, offset, SEEK_SET);
    //Encontra a pasta a partir do path
    while (1)
    {
        name = strtok(NULL, "/");
        if (name == NULL)
        {
            *clusterIndex = index;
            return true;
            break;
        }
        if (!gotoDir(name, file, fs, &index))
        {
            //cout removido, usar retorno da funcao no lugar
            //cout << "Arquivo ou pasta nao encontrado.\n";
            return false;
            break;
        }
        //fseek movido para função separada
        //offset = offSetCalc(fs.indexSize, fs.clusterSize, index);
        //fseek(dir, offset, SEEK_SET);
    }
}

bool isDirEmpty(FILE *file)
{
    int8 first;
    while (1)
    {
        fread(&first, sizeof(int8), 1, file);
        if (first == 29)
        {
        }
        else if (first == 28)
        {
            return true;
            break;
        }
        else
        {
            return false;
            break;
        }
        fseek(file, sizeof(dirEntry) - 1, SEEK_CUR);
    }
}

void DIR(FILE *file, int8 *clusterIndex, FileSystem fs)
{
    if (isDirEmpty(file))
    {
        cout << "<vazio>\n";
    }
    else
    {
        gotoCluster(file, *clusterIndex, fs);
        dirEntry Entry;
        while (1)
        {
            fread(&Entry, sizeof(dirEntry), 1, file);
            if (Entry.name[0] == 28)
            {
                break;
            }
            if (Entry.name[0] != 29)
            {
                cout << getFileName(&Entry) << "\n";
            }
        }
    }
}

bool validPath(char *path)
{
    for (int i = 0; path[i] != '\0'; i++)
    {
        if (i > 0)
        {
            if (path[i] == '/' && path[i - 1] == '/')
            {
                return false;
                break;
            }
        }
        if (path[i] == '\0' && path[i - 1] == '/')
        {
            return false;
            break;
        }
    }
    if (path[0] != '/')
    {
        return false;
    }
    if (path[1] != 'r')
    {
        return false;
    }
    if (path[2] != 'o')
    {
        return false;
    }
    if (path[3] != 'o')
    {
        return false;
    }
    if (path[4] != 't')
    {
        return false;
    }
    return true;
}

char *breakePath(char *path)
{
    char *name;
    int j = 0;
    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            j = i;
        }
    }
    name = &(path[j + 1]);
    path[j] = '\0';
    return name;
}

bool RM(char *path, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    char *pathBackup = new char;
    strcpy(pathBackup, path);
    cout << path;
    int8 index = *clusterIndex;
    int8 indexAux = *clusterIndex;
    //if dir, check if removable

    if (CD(pathBackup, file, fs, &indexAux))
    {
        cout << "dir";
        gotoCluster(file, indexAux, fs);
        if (!isDirEmpty(file))
        {
            cout << "full";
            return false;
        }
    }
    else
    {
        cout << "file";
    }
    char *name = breakePath(path);
    bool rm = true;
    gotoCluster(file, index, fs);
    cout << name;
    cout << path;
    if (CD(path, file, fs, &index))
    {
        cout << "my god part 2";
        gotoCluster(file, index, fs);
        //find name in dir
        dirEntry Entry;
        while (1)
        {
            fread(&Entry, sizeof(dirEntry), 1, file);
            char *entryName = getFileName(&Entry);
            if (strcmp(entryName, name) == 0)
            {

                //remove entry from dir
                int8 char29 = 29;
                fseek(file, (int)-sizeof(dirEntry), SEEK_CUR);
                fwrite(&char29, sizeof(int8), 1, file);
                //remove from index
                const int8 empty = 0;
                int8 currentIndex = 0;
                int cluster = Entry.startCluster + 4;
                fseek(file, cluster, SEEK_SET);
                while (currentIndex != 255)
                {
                    fread(&currentIndex, sizeof(int8), 1, file);
                    cluster = (currentIndex + 4);
                    fseek(file, -1, SEEK_CUR);
                    fwrite(&empty, sizeof(int8), 1, file);
                    if (currentIndex != 255)
                    {
                        fseek(file, cluster, SEEK_SET);
                    }
                }
                break;
            }
            if (Entry.name[0] == 28)
            {
                rm = false;
                break;
            }
        }
    }
    else
    {
        rm = false;
    }
    gotoCluster(file, *clusterIndex, fs);
    return rm;
}

bool MOVE(char *path1, char *path2, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    char *name = breakePath(path1);
    bool mv = true;

    int8 index1 = *clusterIndex;
    int8 index2 = *clusterIndex;
    if (CD(path1, file, fs, &index1) && CD(path2, file, fs, &index2))
    {
        gotoCluster(file, index1, fs);
        //find name in dir
        dirEntry Entry;
        while (1)
        {
            fread(&Entry, sizeof(dirEntry), 1, file);
            char *entryName = getFileName(&Entry);
            if (strcmp(entryName, name) == 0)
            {
                //remove entry from path1
                int8 char29 = 29;
                fseek(file, (int)-sizeof(dirEntry), SEEK_CUR);
                fwrite(&char29, sizeof(int8), 1, file);
                break;
            }
            else if (Entry.name[0] == 28)
            {
                mv = false;
                break;
            }
        }
        //create new entry in path2
        gotoCluster(file, index2, fs);
        while (1)
        {
            int8 auxChar;
            int8 char28 = 28;
            fread(&auxChar, sizeof(int8), 1, file);
            if (auxChar == 29)
            {
                fseek(file, (int)-sizeof(int8), SEEK_CUR);
                fwrite(&Entry, sizeof(dirEntry), 1, file);
            }
            if (auxChar == 28)
            {
                fseek(file, (int)-sizeof(int8), SEEK_CUR);
                fwrite(&Entry, sizeof(dirEntry), 1, file);
                fwrite(&char28, sizeof(int8), 1, file);
            }
            fseek(file, sizeof(dirEntry) - sizeof(int8), SEEK_CUR);
        }
    }
    else
    {
        mv = false;
    }
    return mv;
}

void createDir(FILE *file, int8 curDir, const char dirName[9], FileSystem fs)
{
    int8 eof = 28;
    int dirOffset = offSetCalc(fs.indexSize, fs.clusterSize, curDir);
    dirEntry newEntry;
    strcpy(newEntry.name, dirName);
    strcpy(newEntry.extension, "dir");
    newEntry.startCluster = findAvailableCluster(file, fs.indexSize, fs.clusterSize);
    fseek(file, dirOffset, SEEK_SET);
    int8 current;
    do
    {
        //acha espaco para colocar o novo diretorio
        fread(&current, sizeof(int8), 1, file);
    } while ((current != 28) && (current != 29));
    fseek(file, -1, SEEK_CUR);
    //escreve o objeto de entrada do diretorio
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    if (current == 28)
    {
        //escreve novo final de arquivo se o espaco disponivel estava no final do arquivo
        fwrite(&eof, sizeof(int8), 1, file);
    }
}

void createFile(FILE *file, int8 curDir, const char fileName[9], FileSystem fs)
{
    int8 eof = 28;
    int dirOffset = offSetCalc(fs.indexSize, fs.clusterSize, curDir);
    dirEntry newEntry;
    strcpy(newEntry.name, fileName);
    strcpy(newEntry.extension, "txt");
    newEntry.startCluster = findAvailableCluster(file, fs.indexSize, fs.clusterSize);
    fseek(file, dirOffset, SEEK_SET);
    int8 current;
    do
    {
        //acha espaco para colocar o novo arquivo
        fread(&current, sizeof(int8), 1, file);
    } while ((current != 28) && (current != 29));
    fseek(file, -1, SEEK_CUR);
    //escreve o objeto de entrada do diretorio
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    if (current == 28)
    {
        //escreve novo final de arquivo se o espaco disponivel estava no final do arquivo
        fwrite(&eof, sizeof(int8), 1, file);
    }
}

bool rename(char *path, FILE *file, char *newFileName, FileSystem fs, int8 *clusterIndex)
{
    char *oldName = breakePath(path);
    char *oldExtension;
    char *newName;
    char *newExtension;
    int8 index = *clusterIndex;
    if (CD(path, file, fs, &index))
    {
        gotoCluster(file, index, fs);
        dirEntry replaceEntry;
        dirEntry oldEntry;
        oldName = strtok(oldName, ".");
        oldExtension = strtok(NULL, ".");
        newName = strtok(newFileName, ".");
        newExtension = strtok(NULL, ".");
        //checa se extensao anterior eh igual a nova
        if (strlen(newName) < 9)
        {
            strcpy(replaceEntry.name, newName);
            if (!newExtension && !oldExtension)
            {
                strcpy(replaceEntry.extension, "dir");
                oldExtension = new char;
                strcpy(oldExtension, "dir");
            }
            else if (!newExtension || !oldExtension)
            {
                return false;
            }
            else if (strcmp(newExtension, "txt") == 0 && strcmp(oldExtension, "txt") == 0)
            {
                strcpy(replaceEntry.extension, "txt");
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        do
        {
            //achar a entrada para substituir e substitui
            fread(&oldEntry, sizeof(dirEntry), 1, file);
            if (strcmp(oldEntry.name, oldName) == 0 && strcmp(oldEntry.extension, oldExtension) == 0)
            {
                replaceEntry.startCluster = oldEntry.startCluster;
                fseek(file, -14, SEEK_CUR);
                fwrite(&replaceEntry, sizeof(dirEntry), 1, file);
                return true;
                break;
            }
        } while (oldEntry.name[0] != 28);
        return false;
    }
    else
    {
        gotoCluster(file, *clusterIndex, fs);
        return false;
    }
    return true;
}
bool edit(char *path, FILE *file, char *content, FileSystem fs, int8 *clusterIndex)
{
    char *name = breakePath(path);
    int8 index = *clusterIndex;
    dirEntry entry;
    dirEntry current;
    if (CD(path, file, fs, &index))
    {
        //vai para pasta do arquivo
        gotoCluster(file, index, fs);
        strcpy(entry.name, strtok(name,"."));
        strcpy(entry.extension, "txt");
        entry.startCluster = 0;
        do
        {
            //checa se arquivo esta na pasta
            fread(&current, sizeof(dirEntry), 1, file);
            if (strcmp(entry.name, current.name) == 0 && strcmp(entry.extension, current.extension) == 0)
            {
                entry.startCluster = current.startCluster;
                break;
            }
        } while (current.name[0] != 28);
        if (entry.startCluster == 0)
        {   
            
            //nao achou arquivo na pasta
            return false;
        }
        else
        {
            //reseta indexamento
            int cluster = 4 + entry.startCluster;
            const int8 empty = 0;
            int8 currentIndex = 0;
            fseek(file, cluster, SEEK_SET);
            while (currentIndex != 255)
            {
                fread(&currentIndex, sizeof(int8), 1, file);
                cluster = (currentIndex + 4);
                fseek(file, -1, SEEK_CUR);
                if (currentIndex != 255)
                {
                    fwrite(&empty, sizeof(int8), 1, file);
                    fseek(file, cluster, SEEK_SET);
                }
            }
            //vai para cluster do arquivo
            int bytesWritten=0;
            gotoCluster(file,entry.startCluster,fs);
            currentIndex=entry.startCluster;
            //checa se string comeca e termina com parenteses
            if (content[0]=='"' && content[strlen(content)-1]=='"')
            {
                int8 eof = 28;
                int i=1;
                while (content[i]!='"')
                {
                    
                    fwrite(&content[i],1,1,file);
                    bytesWritten++;
                    if(bytesWritten==pow(2,fs.clusterSize)){
                        currentIndex=findNewCluster(file,fs.indexSize,fs.clusterSize,currentIndex);
                        gotoCluster(file,currentIndex,fs);
                        bytesWritten=0;
                    }
                    i++;
                }
                fwrite(&eof,1,1,file);
                return true;
            }else{
                return false;
            }
            
        }
    }
    else
    {
        //nao conseguiu entrar na pasta do arquivo
        gotoCluster(file, *clusterIndex, fs);
        return false;
    }
}