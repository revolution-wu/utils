#ifndef __BUFFER_H__
#define __BUFFER_H__
#include "Endian.h"
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <string.h>
namespace net
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

  void retriveInt8()
  {
    retrieve(sizeof (int8_t));
  }

  void retriveInt16()
  {
    retrieve(sizeof (int16_t));
  }

  void retriveInt32()
  {
    retrieve(sizeof (int32_t));
  }

  void retriveInt64()
  {
    retrieve(sizeof (int64_t));
  }

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
 
  int8_t peekInt8() const
  {
    assert(readableBytes() >= sizeof (int8_t));
    int8_t net8 = *peek();
    return net8;
  }

  int16_t peekInt16() const
  {
    assert(readableBytes() >= sizeof (int16_t));
    int16_t net16 = 0;
    memcpy(&net16, peek(), sizeof (int16_t));
    return sockets::netToHost16(net16);
  }

  int32_t peekInt32() 
  {
    assert(readableBytes() >= sizeof (int32_t));
    int32_t net32 = 0;
    memcpy(&net32, peek(), sizeof (int32_t));
    return sockets::netToHost32(net32);

  }

  int64_t peekInt64() const
  {
    assert(readableBytes() >= sizeof (int64_t));
    int64_t net64 = 0;
    memcpy(&net64, peek(), sizeof (int64_t));
    return sockets::netToHost64(net64);
  }
  
  int8_t readInt8()
  {
    int8_t result = peekInt8(); 
    retriveInt8();
    return result;
  }

  int16_t readInt16()
  {
    int16_t result = peekInt16();
    retriveInt16(); 
    return result;
  }

  int32_t readInt32()
  {
    int32_t result = peekInt32();
    retriveInt32(); 
    return result;
  }

  int64_t readInt64()
  {
    int64_t result = peekInt64();
    retriveInt64(); 
    return result;
  }


  int8_t readNoConvertInt8()
  {
    assert(readableBytes() >= sizeof (int8_t));
    int8_t result = 0;
    memcpy(&result, peek(), sizeof(int8_t));
    retriveInt8();
    return result;
  }

  int16_t readNoConvertInt16()
  {
    assert(readableBytes() >= sizeof (int16_t));
    int16_t result = 0;
    memcpy(&result, peek(), sizeof(int16_t));
    retriveInt16();
    return result;
  }

  int32_t readNoConvertInt32()
  {
    assert(readableBytes() >= sizeof (int32_t));
    int32_t result = 0;
    memcpy(&result, peek(), sizeof(int32_t));
    retriveInt32();
    return result;
  }
  
  int64_t readNoConvertInt64()
  {
    assert(readableBytes() >= sizeof (int64_t));
    int64_t result = 0;
    memcpy(&result, peek(), sizeof(int64_t));
    retriveInt64();
    return result;
  }

  void append(const char* data, size_t len)
  {
    ensureWritableBytes(len);
    std::copy(data, data + len, beginWriten());
    //for(size_t i = 0; i < len; ++i)
    //{
    //  buffer_.push_back(data[i]);
    //}

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

  void appendInt8(int8_t host8)
  {
    append(&host8, sizeof host8);
  }

  ///
  /// append int16_t using Edian
  void appendInt16(int16_t host16)
  {
    int16_t to16 = sockets::hostToNet16(host16);    
    append(&to16, sizeof to16);
  }

  void appendInt32(int32_t host32)
  {
    int32_t to32 = sockets::hostToNet32(host32);    
    append(&to32, sizeof to32);
  }

  void appendInt64(int64_t host64)
  {
    int64_t to64 = sockets::hostToNet32(host64);    
    append(&to64, sizeof to64);
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

  void prependInt8(int8_t prependData)
  {
    prepend(&prependData, sizeof(int8_t));
  }

  void prependInt16(int16_t prependData)
  {
    int be16 = sockets::hostToNet16(prependData);
    prepend(&be16, sizeof(int16_t));
  }

  void prependInt32(int32_t prependData)
  {
    int be32 = sockets::hostToNet32(prependData);
    prepend(&be32, sizeof(int32_t));
  }

  void prependInt64(int64_t prependData)
  {
    int be64 = sockets::hostToNet64(prependData);
    prepend(&be64, sizeof (int64_t));
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

  size_t capacityBuf() const
  {
    return buffer_.capacity();
  }
  
  size_t size() const
  {
    return buffer_.size();
  }

  size_t readFd(int fd, int* err);
 private:
  void makeSpace(size_t len)
  {
    if(writeableBytes() + prependableBytes() < len + kCheapPrepend)
    {
      size_t readable = readableBytes();
      buffer_.resize(writenIndex_ + len);
      std::copy(begin() + readedIndex_, begin() + writenIndex_, begin() + kCheapPrepend);
      readedIndex_ = kCheapPrepend;
      writenIndex_ = readable + readedIndex_;
    }
    else
    {
      size_t readable = readableBytes();
      std::copy(begin() + readedIndex_, begin() + writenIndex_, begin() + kCheapPrepend);
      readedIndex_ = kCheapPrepend;

      writenIndex_ = readable + readedIndex_;
      //这样计算的话,readableBytes() = writenIndex_ - readedIndex_ 不会等于一开始的readableBytes()
      //因为readedIndex_ 已经改为kCheapPrepend了, 把prependBytes() - kCheapPrepend 的大小也加上了;
      //writenIndex_ = readedIndex_ + readableBytes();
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
