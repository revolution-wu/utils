#include "../LogFile.h"
#include <libgen.h>
#include <string.h>
#include <iostream>

std::unique_ptr<utils::LogFile> g_file;
int main(int argc, char *argv[])
{
  char name[256];
  strncpy(name, argv[0], 255);
  g_file.reset(new utils::LogFile(basename(name), 200 * 1000));

  const char* str = "abcdefghijklmnopqrstuvwsyz10000000\n";
 
  time_t start = time(NULL);
  for(int i = 0; i < 1000000; ++i)
  {
    g_file->appendFile(str, strlen(str));

  }

  time_t end = time(NULL);
  std::cout << "diff " << (end - start) << std::endl;

  return 0;
}


