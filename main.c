#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include <string.h>
#include "parse.h"

// for colorized feature
#define GRN "\e[0;32m"
#define BLU "\e[0;34m"
#define reset "\e[0m"

#define DEFAULT_DIR "./"

static program_options ops = {};

// CLI-aware print file function
void print_file_desc(struct stat *fstat, const char *filename)
{
  if (colorize_enabled(&ops))
  {
    printf(BLU "%8lld %ld %s\n" reset, fstat->st_size, fstat->st_atimespec.tv_nsec, filename);
  }
  else
  {
    printf("%8lld %ld %s\n", fstat->st_size, fstat->st_atimespec.tv_nsec, filename);
  }
}

void print_dir_desc(const char *dirname)
{
  if (colorize_enabled(&ops))
  {
    printf(GRN "%s " reset "contents:\n", dirname);
  }
  else
  {
    printf("%s contents:\n", dirname);
  }
}

int print_file(const char *dirname, const char *filename, int dirnamelen)
{
  int pathlen = strlen(filename);
  char pname[dirnamelen + pathlen + 1]; // output buffer for full path
  int dir_offset = strlcpy(pname, dirname, sizeof(pname));
  if (dir_offset >= sizeof(pname))
  {
    fprintf(stderr, "Directory name wouldn't fit in allocated space."); // defensive - technically shouldn't get here if pname is sized appropriately
    return 1;
  }

  int sep_offset = strlcpy(pname + dir_offset, "/", sizeof(pname)); // write separator
  if (sep_offset >= sizeof(pname))
  {
    fprintf(stderr, "Problem copying file separator");
    return 1;
  }

  int filename_offset = strlcpy(pname + dir_offset + sep_offset, filename, sizeof(pname));
  if (filename_offset >= sizeof(pname))
  {
    fprintf(stderr, "Problem copying filename");
    return 1;
  }

  struct stat fstat;
  if (stat(pname, &fstat) != 0)
  {
    printf("Error reading: %s\n", pname);
    return -1;
  }
  print_file_desc(&fstat, filename);
  return 1;
}

int print_dir(const char *dirname)
{
  print_dir_desc(dirname);
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

void print_dir_or_err(const char *dirname)
{
  if (print_dir(dirname) != 0)
  {
    printf("Error printing dir: %s\n", dirname);
  };
}

int main(int argc, char *argv[argc + 1])
{
  ops = parse_args(argc, argv);
  if (ops.rest_args_index < argc)
  { // Were all the supplied args CLI options?
    for (int i = ops.rest_args_index; i < argc; i++)
    {
      char *dirname = argv[i];
      print_dir_or_err(dirname);
    }
  }
  else
  {
    print_dir_or_err(DEFAULT_DIR);
  }
  return 0;
}