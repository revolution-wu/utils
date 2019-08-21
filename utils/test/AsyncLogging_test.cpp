#include "../AsyncLogging.h"
#include "../Logging.h"
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
off_t kRollSize = 500*1000*1000;

utils::AsyncLogging* g_asyncLog = NULL;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->append(msg, len);

}

int64_t timer()
{
  struct timeval tm;  
  gettimeofday(&tm, NULL);

  return tm.tv_sec * 1000000 + tm.tv_usec;
}

void bench()
{
  utils::Logging::setOutPut(asyncOutput);

  int cnt = 0;
  const int kBatch = 1000;
  std::string empty = " ";
  std::string longStr(3000, 'X');
  longStr += " ";

  for (int t = 0; t < 30; ++t)
  {
    int64_t start = timer();
    for (int i = 0; i < kBatch; ++i)
    {
      LOG_INFO("Hello 0123456789 abcdefghijklmnopqrstuvwxyz %d",  cnt);
      ++cnt;
    }
    int64_t end = timer();

    double diff = static_cast<double>(end - start) / 1000000;
    printf("%f\n", diff * 1000000 /kBatch);
    struct timespec ts = { 0, 500*1000*1000  };
    nanosleep(&ts, NULL);

  }

}


int main(int argc, char* argv[])
{
  {
    // set max virtual memory to 2GB.
    size_t kOneGB = 1000*1024*1024;
    rlimit rl = { 2*kOneGB, 2*kOneGB  };
    setrlimit(RLIMIT_AS, &rl);

  }

  printf("pid = %d\n", getpid());

  char name[256] = { 0  };
  strncpy(name, argv[0], sizeof name - 1);
  utils::AsyncLogging log(::basename(name), kRollSize);
  log.start();
  g_asyncLog = &log;

  bool longLog = argc > 1;
  bench();

}

