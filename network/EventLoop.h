#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include <iostream>
#include <thread>
#include <boost/noncopyable.hpp>
namespace net
{
class EventLoop:public boost::noncopyable
{
 public:
  EventLoop();
  ~EventLoop();
  
  bool isThreadInLoop()
  {
    return threadId_ == std::this_thread::get_id(); 
  }

  void assertThreadInLoop()
  {
    if(!isThreadInLoop())
    {
      std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << " " 
                << " current thread id: " << threadId_ << "this: " << this << std::endl;
      abort();
    }
  }
  
  void loop();
  void quit();
  const std::thread::id getThreadId() const { return threadId_; }
 private:
  bool looping_;
  bool quit_;
  const std::thread::id threadId_;  
};
}




#endif   /* __EVENTLOOP_H__ */
