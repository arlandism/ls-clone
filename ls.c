#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include <string.h>

#define GRN "\e[0;32m"
#define BLU "\e[0;34m"

#define reset "\e[0m"

#define DEFAULT_DIR "./"

int print_file(const char* dirname, const char* filename, int dirnamelen){
  int pathlen = strlen(filename);
  char pname[dirnamelen + pathlen + 1]; // output buffer for full path
  int dir_offset = strlcpy(pname, dirname, sizeof(pname));
  if (dir_offset >= sizeof(pname)) {
    fprintf(stderr, "Directory name wouldn't fit in allocated space."); // defensive - technically shouldn't get here if pname is sized appropriately
    return 1;
  }

  int sep_offset = strlcpy(pname + dir_offset, "/", sizeof(pname)); // write separator
  if (sep_offset >= sizeof(pname)) {
    fprintf(stderr, "Problem copying file separator");
    return 1;
  }

  int filename_offset = strlcpy(pname + dir_offset + sep_offset, filename, sizeof(pname));
  if (filename_offset >= sizeof(pname)) {
    fprintf(stderr, "Problem copying filename");
    return 1;
  }

  struct stat * fstat = malloc(sizeof(stat));
  if (stat(pname, fstat) != 0) {
    printf("Error reading: %s\n", pname);
    free(fstat);
    return -1;
  }
  printf(BLU "%8lld %ld %s\n" reset, fstat->st_size, fstat->st_atimespec.tv_nsec, filename);
  free(fstat);
  return 1;
}

int print_dir(const char* dirname){
  printf(GRN "%s " reset "contents:\n", dirname);
  DIR *d = opendir(dirname);
  if (d == NULL)
  {
    return (1);
  }
  struct dirent *entry;
  int dirlen = strlen(dirname);
  while ((entry = readdir(d)) != NULL)
  {
    print_file(dirname, entry->d_name, dirlen);
  }
  printf("\n");
  closedir(d);
  return 0;
}

void print_dir_or_err(const char* dirname) {
      if (print_dir(dirname) != 0)
      {
        printf("Error printing dir: %s\n", dirname);
      };
}

int main(int argc, char* argv[argc + 1])
{
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      char *dirname = argv[i];
      print_dir_or_err(dirname);
    }
  } else {
      print_dir_or_err(DEFAULT_DIR);
  }
  return 0;
}