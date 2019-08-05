#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <vector>
#include <sys/types.h>
#include <string>
#include <algorithm>
namespace base
{
class Buffer
{
 public:
  static const int kCheapPrepend = 8;
  static const int kInitalSize = 1024;
  Buffer()
    :buffer_(kCheapPrepend + kInitalSize),
    readedIndex_(kCheapPrepend),
    writenIndex_(kCheapPrepend)
  {}
  ~Buffer(){}
  
  const char* peek() const
  {
    return begin() + readedIndex_;
  }

  size_t readableBytes() const
  {
    return writenIndex_ - readedIndex_;
  }

  size_t writeableBytes() const
  {
    return buffer_.size() - writenIndex_;
  }

  size_t prependableBytes() const
  {
    return readedIndex_;
  }

  void swap(Buffer& rhs)
  {
    buffer_.swap(rhs.buffer_);
    std::swap(readedIndex_, rhs.readedIndex_);
    std::swap(writenIndex_, rhs.writenIndex_);
  }

  void retrieve(size_t len)
  {
    if(len < readableBytes())
    {
      readedIndex_ += len;
    }
    else
    {
      retrieveAll();
    }

  }

  void retrieveAll()
  {
    writenIndex_ = kCheapPrepend;
    readedIndex_ = kCheapPrepend;
  }

  //函数返回NULL,是没有收到数据，太大了
  std::string retrieveAsString( size_t len)
  {

      std::string recvString(peek(), len);
      retrieve(len);
      return recvString;
  }
  
  std::string retrieveiAllAsString()
  {
    return retrieveAsString(readableBytes());
  }
 
  void append(const char* data, size_t len)
  {
    ensureWritableBytes(len);
    std::copy(data, data + len, beginWriten());
    if(len <= writeableBytes())
    {
      writenIndex_ += len;
    }
  }

  void append(const std::string& newString)
  {
    append(newString.c_str(), newString.size());
  }

  void append(const void* data, size_t len)
  {
    append(static_cast<const char*>(data), len);
  }

  void prepend(const void* data, size_t len)
  {
    if(len <= prependableBytes())
    {
      readedIndex_ -= len;
      const char* src = static_cast<const char*>(data);

      std::copy(src, src + len, begin() + readedIndex_);
    }
  }

  void ensureWritableBytes(size_t len)
  {
    if(writeableBytes() < len)
    {
      makeSpace(len);
    }
  }

  char* beginWriten() 
  {
    return begin() + writenIndex_;
  }

  const char* beginWriten() const
  {
    return begin() + writenIndex_;
  }

  size_t readFd(int fd, int* err);
 private:
  void makeSpace(size_t len)
  {
    if(writeableBytes() + prependableBytes() < len + kCheapPrepend)
    {
      buffer_.resize(writenIndex_ + len);
      std::copy(begin() + readedIndex_, begin() + writenIndex_, begin() + kCheapPrepend);
      readedIndex_ = kCheapPrepend;
      writenIndex_ = readableBytes() + readedIndex_;
    }
    else
    {
      std::copy(begin() + readedIndex_, begin() + writenIndex_, begin() + kCheapPrepend);
      readedIndex_ = kCheapPrepend;
      writenIndex_ = readableBytes() + readedIndex_;
    }
  }
  const char* begin() const
  {
    return buffer_.data();
  }

  char* begin()
  {
    return &*buffer_.begin();
  }

  std::vector<char> buffer_;
  size_t readedIndex_;
  size_t writenIndex_;
  
};
}

#endif   /* __BUFFER_H__ */
