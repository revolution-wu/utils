#include "Logging.h"

#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <sstream>
#include <thread>
namespace utils
{

__thread char t_time[64];
__thread time_t t_lastSeconds;

const int Logging::kMicroseconds;
int64_t Logging::microSeconds_ = 0;
const char*  Logging::basename_ = "deault";
Logging::LogLevel Logging::level_ = LOG_INFO;

Logging::put Logging::putFunc_ = defaultOutPut; 
Logging::flush Logging::flushPath_ = defaultFlush;



bool Logging::outPut(LogLevel level,
                const char* filename,
                int lineno,
                const char* func,
                const char* famat, ...)
{
  if(level < level_)
  {
    return false;
  }
  
  const char* slash = strrchr(filename, '/');
  if(slash)
  {
    basename_ = slash + 1;
  }
  else
  {
    basename_ = filename;
  }

  std::string strLine;
  makeLinePrefix(level,strLine);

  strLine += '[';
  strLine += func;
  strLine += ']';

  std::string strLogMsg; 
  va_list ap;
  va_start(ap, famat);

  int len = vsnprintf(NULL, 0, famat, ap);
  va_end(ap);

  if(static_cast<int>(strLogMsg.capacity()) < len + 1)
  {
    strLogMsg.resize(len + 1);
  }

  va_list aq;
  va_start(aq, famat);
  vsnprintf(const_cast<char*>(strLogMsg.data()), strLogMsg.capacity(), famat, aq);
  va_end(aq);
  std::string strLogForm;
  strLogForm.append(strLogMsg.c_str(),len);
  strLine += strLogForm;
  
  char buf[32];
  snprintf(buf, sizeof buf, "[%s:%d]", basename_, lineno);
  strLine += buf;
  strLine += '\n';

  putFunc_(strLine.c_str(), strLine.size());
  if(level == LOG_FATAL)
  {
    flushPath_();
    crash();
  }
  return true;
}

void Logging::setLogLevel(LogLevel level)
{
  level_ = level;
}

void Logging::setOutPut(void(*put)(const char* msg, int len))
{
  putFunc_ = put;
}

void Logging::defaultOutPut(const char* msg, int len)
{
  size_t n  = 0;
  while(true)
  {
    size_t nwriten = fwrite(msg +n, 1, len - n, stdout);  
    n += nwriten;
    if(nwriten == 0)
    {
      break;
    }
  }
}

void Logging::defaultFlush()
{
  fflush(stdout);
}

void Logging::setFlush(void(*flush)())
{
  flushPath_ = flush;
}

void Logging::makeLinePrefix(LogLevel level, std:: string& linePrefix)
{

  linePrefix.reserve(sizeof (t_time) + 64);
  formatTime();
  linePrefix += t_time;
  
  if(level == LOG_INFO)
  {
    linePrefix += "[INFO]";
  }
  else if(level == LOG_TRACE)
  {
    linePrefix += "[TRACE]";
  }
  else if(level == LOG_DEBUG) 
  {
    linePrefix += "[DEBUG]";
  }
  else if(level == LOG_DEBUG) 
  {
    linePrefix += "[DEBUG]";
  }
  else if(level == LOG_WARNING) 
  {
    linePrefix += "[WARNING]";
  }
  else if(level == LOG_ERROR) 
  {
    linePrefix += "[ERROR]";
  }
  else if(level == LOG_SYSERROR) 
  {
    linePrefix += "[SYSERROR]";
  }
  else if(level == LOG_FATAL)
  {
    linePrefix += "[FATAL]";
  }

  std::ostringstream threadId;
  threadId << std::this_thread::get_id();
  char buf[32];
  snprintf(buf, sizeof buf, "[%s]", threadId.str().c_str());
  linePrefix += buf;

}

void Logging::formatTime()
{
  now();
  time_t seconds = static_cast<time_t>(microSeconds_ / kMicroseconds);
  int microSeconds = microSeconds_ % kMicroseconds;
  if(t_lastSeconds != seconds)
  {
    t_lastSeconds = seconds;
    struct tm localtime;
    localtime_r(&seconds,&localtime );
    int len = snprintf(t_time, sizeof t_time, "[%04d-%02d-%02d %02d:%02d:%02d", 
           localtime.tm_year + 1900, localtime.tm_mon + 1, localtime.tm_mday,
           localtime.tm_hour, localtime.tm_min, localtime.tm_sec);
    assert(len == 20);(void)len;
  }
  snprintf(t_time + 20 ,sizeof (t_time) - 20, ".%06d]", microSeconds);
}

void Logging::now()
{
  struct timeval tm;
  gettimeofday(&tm, NULL);

  time_t seconds = tm.tv_sec;
 
  microSeconds_ = seconds * kMicroseconds + tm.tv_usec;
}

void Logging::crash()
{
  int* p = NULL;
  *p = 0;
}
}

