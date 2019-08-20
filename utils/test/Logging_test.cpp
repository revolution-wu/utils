#include "../Logging.h"
#include "../LogFile.h"
#include <time.h>
#include <memory>
int  g_total;
FILE* g_file;
std::unique_ptr<utils::LogFile> g_logFile;

void dummyOutput(const char* msg, int len)
{
  g_total += len;
  if (g_file)
  {
    fwrite(msg, 1, len, g_file);

  }
  else if (g_logFile)
  {
    g_logFile->appendFile(msg, len);

  }

}

void bench(const char* type)
{
  utils::Logging::setOutPut(dummyOutput);
  time_t start = time(NULL);
  g_total = 0;

  int n = 1000*1000;
  std::string empty = " ";
  std::string longStr(3000, 'X');
  longStr += " ";
  for (int i = 0; i < n; ++i)
  {
    LOG_INFO(" hijklmnopqrstuvwxyz %d", i);

  }
  time_t end = time(NULL);
  double seconds = end - start;
  printf("%12s: %f seconds, %d bytes, %10.2f msg/s, %.2f MiB/s\n",
         type, seconds, g_total, n / seconds, g_total / seconds / (1024 * 1024));

}
int main(int argc, char *argv[])
{

  char buffer[64*1024];

  g_file = fopen("/dev/null", "w");
  setbuffer(g_file, buffer, sizeof buffer);
  bench("/dev/null");
  fclose(g_file);

  g_file = fopen("/tmp/log", "w");
  setbuffer(g_file, buffer, sizeof buffer);
  bench("/tmp/log");
  fclose(g_file);

  g_file = NULL;
  g_logFile.reset(new utils::LogFile("test_log_st", 500*1000*1000, false));
  bench("test_log_st");

  g_logFile.reset(new utils::LogFile("test_log_mt", 500*1000*1000, true));
  bench("test_log_mt");
  g_logFile.reset();
  return 0;
}
