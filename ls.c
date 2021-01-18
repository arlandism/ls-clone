#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define GRN "\e[0;32m"
#define BLU "\e[0;34m"

#define reset "\e[0m"

#define DEFAULT_DIR "./"

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
    printf(BLU "%s " reset, entry->d_name);
  }
  printf("\n");
  (void) closedir(d);
  return 0;
}

void print_file(){

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