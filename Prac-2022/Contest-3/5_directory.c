#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

enum { MAX_LEN = 1024 };

void traverseDirectory(char *name, uid_t uid, off_t *totalSize)
{
    DIR *dir;
    struct dirent *ent;
    struct stat states;
    char path[MAX_LEN - 1];

    if ((dir = opendir(name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            sprintf(path, "%s/%s", name, ent->d_name);
            if (stat(path, &states) != -1) {
                if (S_ISREG(states.st_mode) && states.st_uid == uid) {
                    *totalSize += states.st_size;
                } else if (S_ISDIR(states.st_mode) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    traverseDirectory(path, uid, totalSize);
                }
            }
        }
        closedir(dir);
    }
}

int main(int argc, char **argv) 
{
    if (argc != 3) {
        printf("Usage: %s <directory> <uid>\n", argv[0]);
        return 1;
    }

    char *name = argv[1];
    uid_t uid = atoi(argv[2]);
    off_t totalSize = 0;

    traverseDirectory(name, uid, &totalSize);

    printf("Total size of files owned by user with UID %d: %ld bytes\n", uid, totalSize);

    return 0;
}