#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define GRN "\e[0;32m"
#define BLU "\e[0;34m"

#define reset "\e[0m"

#define DEFAULT_DIR "./"

int print_file(char* dirname, struct dirent* entry){
  int dirlen = strlen(dirname);
  int pathlen = strlen(entry->d_name);
  char path[dirlen + pathlen + 1];
  int cx = snprintf(path, dirlen + pathlen + 2, "%s/%s", dirname, entry->d_name);
  if (cx < 0 || cx >= dirlen + pathlen + 2) {
    printf("%d\n", cx);
    printf("Error getting pathname\n");
    return -1;
  }
  struct stat * fstat = malloc(sizeof(stat));
  if (stat(path, fstat) != 0) { //TODO: surely there is a better way to do this than make a syscall per file
    printf("Error reading: %s\n", path);
    free(fstat);
    return -1;
  }
  printf(BLU "%8lld %ld %s\n" reset, fstat->st_size, fstat->st_atimespec.tv_nsec, entry->d_name);
  free(fstat);
  return 1;
}

int print_dir(char* dirname){
  printf(GRN "%s " reset "contents:\n", dirname);
  DIR *d = opendir(dirname);
  if (d == NULL)
  {
    return (1);
  }
  struct dirent *entry;
  while ((entry = readdir(d)) != NULL)
  {
    print_file(dirname, entry);
  }
  printf("\n");
  (void) closedir(d);
  return 0;
}

int main(int argc, char **argv)
{
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      char *dirname = argv[i];
      if (print_dir(dirname) != 0)
      {
        printf("Error printing dir: %s\n", dirname);
      };
    }
  } else {
    print_dir(DEFAULT_DIR);
  }
  return 0;
}