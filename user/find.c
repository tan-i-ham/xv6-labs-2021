#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file_name)
{
    char buf[512], *p;
    int fd; // file descriptor
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", file_name);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", file_name);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        if (strcmp(de.name, file_name) == 0)
        {
            fprintf(2, "%s/%s\n", path, file_name);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) // DIRSIZ means the maximum length of a file or directory name
        {
            printf("find: target too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            /// Skip the current and parent directories
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            // printf("de.name %s\n", de.name);
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE)
            {
                if (strcmp(de.name, file_name) == 0)
                {
                    printf("%s\n", buf);
                }
            }
            else if (st.type == T_DIR)
            {
                find(buf, file_name);
            }
        }

        break;
    }
    close(fd);
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: find files \n");
        exit(0);
    }
    else
    {
        if (strcmp(argv[1], ".") == 0 && strcmp(argv[2], "..") == 0)
        {
            printf("not support find . ..\n");
            exit(0);
        }
        find(argv[1], argv[2]); // skip `find` arg
    }

    exit(0);
}