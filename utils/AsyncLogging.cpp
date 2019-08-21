
#include "AsyncLogging.h"
#include "LogFile.h"
#include <chrono>
namespace utils
{
AsyncLogging::AsyncLogging(const std::string& basename,
                           size_t rollSize,
                           int flushInterval)
                          : running_(false),
                            basename_(basename),
                            rollSize_(rollSize),
                            flushInterval_(flushInterval),
                            latch_(1),
                            currentBuffer_(new Buffer),
                            nextBuffer_(new Buffer),
                            buffers_()
                                                      
{
  currentBuffer_->beZero();
  nextBuffer_->beZero();
  buffers_.reserve(16);
}

void AsyncLogging::append(const char* msg, int len)
{
  std::unique_lock<std::mutex> lock(mutex_);
  if(currentBuffer_->avail() > len)
  {
    currentBuffer_->append(msg, len);
  }
  else
  {
    buffers_.push_back(std::move(currentBuffer_));
    if(nextBuffer_)
    {
      currentBuffer_ = std::move(nextBuffer_);
    }
    else
    {
      currentBuffer_.reset(new Buffer);
    }

    currentBuffer_->append(msg, len);
    cond_.notify_one();
  }
}

void AsyncLogging::threadFunc()
{
  if(!running_)
  {
    return;
  }

  latch_.CountDown();

  BufferPtr newBuffer1 (new Buffer);
  BufferPtr newBuffer2 (new Buffer);
  LogFile outPutFile(basename_, rollSize_, false);
  newBuffer1->beZero();
  newBuffer2->beZero();
  vectorBuffers thisBuffer;
  thisBuffer.reserve(16);
  while(running_)
  {
    assert(newBuffer1->length() == 0);
    assert(newBuffer2->length() == 0);
    assert(thisBuffer.empty());

    {
      std::unique_lock<std::mutex> lock(mutex_);
      if(buffers_.empty())
      {
        cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
      }
      buffers_.push_back(std::move(currentBuffer_));
      currentBuffer_ = std::move(newBuffer1);
      thisBuffer.swap(buffers_);
      if(!nextBuffer_)
      {
        nextBuffer_ = std::move(newBuffer2); 
      }
    }

    if(thisBuffer.empty())
    {
      return;
    }

    if(thisBuffer.size() > 25)
    {
      thisBuffer.erase(thisBuffer.begin() + 2, thisBuffer.end());
    }

    for(const auto& it :thisBuffer)
    {
      outPutFile.appendFile(it->data(), it->length());
    }

    if(thisBuffer.size() > 2)
    {
      thisBuffer.resize(2);
    }

    if(!newBuffer1)
    {
      newBuffer1 = std::move(thisBuffer.back());
      thisBuffer.pop_back();
      newBuffer1->reset();
    }
    if(!newBuffer2)
    {
      newBuffer2 = std::move(thisBuffer.back());
      thisBuffer.pop_back();
      newBuffer2->reset();
    }
    
    thisBuffer.clear();
    outPutFile.flush();
  }
outPutFile.flush();
}
}
