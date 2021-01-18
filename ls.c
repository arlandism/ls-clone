#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_NUM_FILES 1000

int print_dir(char* dirname){
  DIR *d = opendir(dirname);
  if (d == NULL)
  {
    printf("Couldn't open dir: %s\n", dirname);
    return (1);
  }
  struct dirent *entry;
  while ((entry = readdir(d)) != NULL)
  {
    printf("%s ", entry->d_name);
  }
  printf("\n");
  (void) closedir(d);
  return 0;
}

void print_file(){

}

int main(int argc, char **argv)
{
  char *dirname = "./";
  if (argc == 2)
  {
    dirname = argv[1];
    printf("argv: %s\n", argv[1]);
  }
  if (print_dir(dirname) != 0) {
    printf("Error printing dir: %s\n", dirname);
  };


  return 0;
}