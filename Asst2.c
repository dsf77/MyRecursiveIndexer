#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int print_dirs(const char *path, int recursive)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;

    /* Check input parameters. */
    if (!path)
        return -1;
    path_len = strlen(path);  

    if (!path || !path_len || (path_len > _POSIX_PATH_MAX))
        return -1;

    /* Open directory */
    dirp = opendir(path);
    if (dirp == NULL)
        return -1;

    while ((direntp = readdir(dirp)) != NULL)
    {
        /* For every directory entry... */
      if(direntp->d_type == DT_DIR)
      {
        struct stat fstat;
        char full_name[_POSIX_PATH_MAX + 1];

        /* Calculate full name, check we are in file length limts */
        if ((path_len + strlen(direntp->d_name) + 1) > _POSIX_PATH_MAX)
            continue;

        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
            strcat(full_name, "/");
        strcat(full_name, direntp->d_name);

        /* Ignore special directories. */
        if ((strcmp(direntp->d_name, ".") == 0) ||
            (strcmp(direntp->d_name, "..") == 0))
            continue;

        /* Print only if it is really directory. */
        if (stat(full_name, &fstat) < 0)
            continue;
        if (S_ISDIR(fstat.st_mode))
        {
            //printf("%s\n", full_name);
            if (recursive)
                print_dirs(full_name, 1);
        }// small if
      }// if
    else
    {
     struct stat fstat;
     char full_name[_POSIX_PATH_MAX + 1];

     /* Calculate full name, check we are in file length limts */
     if ((path_len + strlen(direntp->d_name) + 1) > _POSIX_PATH_MAX)
         continue;
     
     strcpy(full_name, path);
     if (full_name[path_len - 1] != '/')
        strcat(full_name, "/");
     strcat(full_name, direntp->d_name);
     
     /* call open file to parse through */
     openfile(full_name); 
    
    }    
    /* Finalize resources. */
    }// while
    (void)closedir(dirp);
    return 0;
  }

void openfile(char* fileName)
{
 int fd;
 fd = open(fileName, O_RDONLY);
 if(fd != -1)
    // tokenize(fd);
 else
    // Throw Errno
 close(fd);
}


/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    if (argc < 2)
        return -1;

    print_dirs(argv[1], 1);
    printf("SUCCESS\n");
    return 0;
}