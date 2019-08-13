#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <string>
#include <sys/types.h>
namespace FileUtils
{
class ReadSmallFile
{
public:
  ReadSmallFile(const char* name);
  ~ReadSmallFile();

  // return errno
  int readToString(int maxsize, 
                   std::string* content,
                   int64_t* filesize,
                   int64_t* modifitime,
                   int64_t* createtime);
  // return errno
  int readToBuffer(int* size);
  const char* data() const { return buf_; }
  int readFile(int maxsize,
               std::string* content,
               int64_t* filesize = NULL,
               int64_t* modifitime = NULL,
               int64_t* createtime = NULL)
  {
    int err = err_;
    err = readToString(maxsize, content, filesize, modifitime, createtime);
    return err;
  }
private:
  char buf_[64 * 1024];
  int err_;
  int fd_;
};

class AppendFile
{
public:
  AppendFile(const char* name);
  ~AppendFile();
  size_t write(const char* line, size_t len);
  void append(const char* line, size_t len);
  void fflush();

  size_t writenBytes() const { return writenBytes_; }
private:
  char buf_[64 * 1024];
  FILE* fp_;
  off_t writenBytes_;
};
}



#endif   /* __FILEUTILS_H__ */
