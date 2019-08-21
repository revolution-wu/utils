#ifndef __ASYNCLOGGING_H__
#define __ASYNCLOGGING_H__

#include "LogStream.h"
#include "Countdownlatch.h"
#include <vector>
#include <memory>
#include <thread>
#include <functional>
namespace utils
{

class AsyncLogging
{
 public:
  AsyncLogging(const std::string& basename, 
               size_t rollSize = 1024 * 1024 * 300,
               int flushInterval = 3);
  ~AsyncLogging()
  {
    if(running_)
    {
      stop();
    }
  }

  void start()
  {
    running_ = true;
    thread_.reset(new std::thread(std::bind(&AsyncLogging::threadFunc, this)));
    latch_.Wait();   
  }

  void stop()
  {
    running_ = false;
    thread_->join();
  }

  void append(const char* msg, int len);
 public:
  typedef FixedBuffer<kLargeBuffer> Buffer;  
  typedef std::vector<std::unique_ptr<Buffer>> vectorBuffers;
  typedef vectorBuffers::value_type BufferPtr;
 private:
  void threadFunc();
  
  bool                          running_;
  const std::string             basename_;
  const size_t                  rollSize_;
  const int                     flushInterval_;
  
  CountDownLatch                latch_;
  std::unique_ptr<std::thread>  thread_;
  std::mutex                    mutex_;
  std::condition_variable       cond_;

  BufferPtr                     currentBuffer_;     
  BufferPtr                     nextBuffer_;
  vectorBuffers                 buffers_;
};
}



#endif   /* __ASYNCLOGGING_H__ */
