#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <string>

#include <stdint.h>
#include <string.h>
namespace utils
{

class Logging
{
public:
  enum LogLevel
  {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_SYSERROR,
    LOG_FATAL
  };

  //class SourceFile
  //{
  //public:
  //  template<int N>
  //    SourceFile(const char(&arr)[N])
  //    :data_(arr),
  //    size_(N - 1)
  //  {
  //    const char* slash = strrchr(arr, '/');
  //    if(slash)
  //    {
  //      data_ = slash + 1;
  //      size_ -= data_ - arr;
  //    }
  //  }
  //  explicit SourceFile(const char* filename)
  //    :data_(filename)
  //  {
  //    const char* slash = strrchr(filename, '/');
  //    if(slash)
  //    {
  //      data_ = slash + 1;
  //      size_ = strlen(data_);
  //    }
  //  }
  //private:
  //  const char* data_;
  //  int size_;
  //};
public:
  static bool outPut(LogLevel level, 
                     const char* filename,
                     int lineno,
                     const char* func, 
                     const char* famat, ...);
  static void setLogLevel(LogLevel level);
  static void setOutPut(void(*put)(const char*, int));
  static void setFlush(void(*flush)());
  typedef void (*put)(const char*, int);
  typedef void (*flush)();
public:
  static const int kMicroseconds = 1000 * 1000;
private:
  static void now();
  static void formatTime();
  static void crash();
  static void defaultOutPut(const char* msg, int len);
  static void defaultFlush();
  static void makeLinePrefix(LogLevel level, std::string& linePrefix);
  static int64_t microSeconds_;
  static LogLevel level_;
  static const char* basename_;
  static put putFunc_;
  static flush flushPath_;
};

#define LOG_TRACE(...) utils::Logging::outPut(utils::Logging::LOG_TRACE, __FILE__, __LINE__, \
                                              __func__, __VA_ARGS__)

#define LOG_DEBUG(...) utils::Logging::outPut(utils::Logging::LOG_DEBUG, __FILE__, __LINE__, \
                                              __func__, __VA_ARGS__)

#define LOG_INFO(...) utils::Logging::outPut(utils::Logging::LOG_INFO, __FILE__, __LINE__, \
                                             __func__, __VA_ARGS__) 

#define LOG_WARNING(...) utils::Logging::outPut(utils::Logging::LOG_WARNING, __FILE__, __LINE__, \
                                                __func__, __VA_ARGS__)

#define LOG_ERROR(...) utils::Logging::outPut(utils::Logging::LOG_ERROR, __FILE__, __LINE__, \
                                              __func__, __VA_ARGS__)

#define LOG_SYSERROR(...) utils::Logging::outPut(utils::Logging::LOG_SYSERROR, __FILE__, __LINE__, \
                                                 __func__, __VA_ARGS__)

#define LOG_FATAL(...) utils::Logging::outPut(utils::Logging::LOG_FATAL, __FILE__, __LINE__, \
                                              __func__, __VA_ARGS__)
}

#endif   /* __LOGGING_H__ */
