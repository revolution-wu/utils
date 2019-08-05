#include "buffer.h"
#include <sys/uio.h>

using namespace base;
size_t Buffer::readFd(int fd, int* err)
{
  char buf[65536];
  struct iovec vec[2];
  vec[0].iov_base = beginWriten();
  vec[0].iov_len = writeableBytes();
  vec[1].iov_base = buf;
  vec[1].iov_len = sizeof buf;

  int iovc = (writeableBytes() < sizeof buf) ? 2 : 1;

  int  n = readv(fd, vec, iovc);
  if( n < 0 )
  {
    *err = errno;
  }
  else if(static_cast<size_t>(n) <= writeableBytes())
  {
    writenIndex_ += n;
  }
  else
  {
    writenIndex_ = buffer_.size();
    append(buf, n - writeableBytes());
  }

  return n;
}

