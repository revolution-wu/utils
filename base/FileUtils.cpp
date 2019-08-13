#include "FileUtils.h"
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define strcut timespec int64_t

__thread char t_errnorbuf[512];
namespace FileUtils
{
ReadSmallFile::ReadSmallFile(const char* name)
  :fd_(open(name, O_RDONLY | O_CLOEXEC))
{
  buf_[0] = '\0';
  if(fd_ < 0)
  {
    err_ = errno;
  }
}

ReadSmallFile::~ReadSmallFile()
{
  if(fd_ > 0)
    close(fd_);
}

int ReadSmallFile::readToString(int maxsize,
                                std::string* content,
                                int64_t* filesize,
                                int64_t* modifytime,
                                int64_t* createtime)
{
  int err = err_;
  if(fd_ > 0)
  {
    content->clear();
    if(filesize)
    {
      struct stat filebuf;

      if(::fstat(fd_, &filebuf) == 0)
      {
        if(S_ISDIR(filebuf.st_mode))
        {
          err_ = EISDIR;
        }
        if(S_ISREG(filebuf.st_mode))
        {
          *filesize = filebuf.st_size;
          content->reserve(static_cast<int>(std::min(static_cast<int64_t>(maxsize), *filesize)));
        }
        if(modifytime)
        {
          *modifytime = filebuf.st_mtime;
        }
        if(createtime)
        {
          *createtime = filebuf.st_ctime;
        }
      }
      else
      {
        err = errno;
      }
    }

    while(content->size() < static_cast<size_t>(maxsize))
    {
      size_t nread = std::min(static_cast<size_t>(maxsize) - content->size(), sizeof buf_);
      ssize_t n = read(fd_, buf_, nread);
      if(n > 0)
      {
        content->append(buf_, n);
      }
      else
      {
        if(n < 0)
          err = errno;
        break;
      }
    }
  }
  return err;
}

int ReadSmallFile::readToBuffer(int* size)
{
  int err = err_;
  if(fd_ > 0)
  {
    ssize_t n = ::pread(fd_, buf_, sizeof buf_ - 1, 0);
    if(n > 0)
    {
      if(size)
      {
        *size = n;
      }
      buf_[n] = '\0';
    }
    else
    {
      err = errno;
    }
  }
  return err;
}

AppendFile::AppendFile(const char* name)
  :fp_(fopen(name, "ae")),
  writenBytes_(0)
{
  assert(fp_);
  setbuffer(fp_, buf_, sizeof buf_);
}

AppendFile::~AppendFile()
{
  if(fp_)
  {
    fclose(fp_);
  }
}

void AppendFile::append(const char* line, size_t len)
{
  size_t n = write(line, len);
  size_t remain = len - n;
  while(remain > 0)
  {
    size_t x = write(line + n, remain);
    if(x > 0)
    {
      n += x;
      remain -= x;
    }
    if(x == 0)
    {
      int err = ferror(fp_);
      fprintf(stderr,"append file faild %s\n", strerror_r(err,t_errnorbuf, sizeof t_errnorbuf));
      break;
    }
  }
  writenBytes_ += n;
}

size_t AppendFile::write(const char* line, size_t len)
{
  return fwrite(line, 1, len, fp_);
}

void AppendFile::fflush()
{
  ::fflush(fp_);
}

}
