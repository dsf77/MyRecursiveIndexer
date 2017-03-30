#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int totalCharacters;
int first = 0;
#define TRUE 0
#define FALSE 1


int getNumOfChars(char* fileName)
{
 int fp;
 int numberOfChar = 0;
 char buffer[200];
 int bytsRead;
 fp = open(fileName, O_RDONLY);
 if(fp != -1)
 {
  do
  {
   bytsRead = read(fp, buffer, 1);
   if(bytsRead != 0)
    numberOfChar++;
  }while(bytsRead != 0);
  close(fp);
  return numberOfChar;
 }
}





void openfile(char* fileName, char* string)
{
 //printf("Top of OpenFIle\n");
 int fd;
 int bytsRead;
 int numberOfChar;
 int i = 0;
 char buffer[1];
 fd = open(fileName, O_RDONLY);
 if(first == TRUE)
 {
  if(fd !=-1)
  {
   numberOfChar = getNumOfChars(fileName);
   totalCharacters = totalCharacters + numberOfChar;
   //printf("IN FIRST == TRUE\nTotalChar(%d)\nnumberofChar(%d)",totalCharacters,numberOfChar);
  }
 }
 if(first == FALSE)
 {
  //printf("IN FALSE\n");
  if(fd != -1)
  {
  //printf("File Open\n");
  do
  {
   bytsRead = read(fd, buffer, 1);
   strcat(string, buffer);
  }while(bytsRead != 0);
  //parse(string); ******************************THIS IS EACH FILE TO BE PARSED*****
  strcpy(string, "");
 }
 else
 {
  printf("UNABLE TO OPEN FILE\n");
 }
}
 close(fd);
 //printf("Leaving open file\n");
}


int print_dirs(const char *path, int recursive, char* string)
{
    //printf("Top of printdirs(%s)\n", path);
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
                print_dirs(full_name, 1, string);
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
     openfile(full_name, string); 
     //printf("After OpenFIle\n");
    }    
    /* Finalize resources. */
    }// while
    (void)closedir(dirp);
    return 0;
  }

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    if (argc < 2)
        return -1;
    
    print_dirs(argv[1], 1, NULL);
    first = FALSE;
    char *string = (char*)malloc(sizeof(char)*totalCharacters);
    print_dirs(argv[1],1,string);
    printf("SUCCESS\n");
    return 0;
}