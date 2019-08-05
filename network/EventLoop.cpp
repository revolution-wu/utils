/**************************************************************************
 *      FileName: EventLoop.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年08月05日 星期一 13时18分44秒
 * Last Modified: 2019-08-05 13:18:44
 * 
 **************************************************************************/
#include "EventLoop.h"
namespace net
{
__thread EventLoop* currentEventLoop = NULL;
EventLoop::EventLoop()
  :looping_(false),
  quit_(false),
  threadId_(std::this_thread::get_id())
{
  if(currentEventLoop)
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << " EventLoop this exits: " << this
              << " thread id: " << threadId_ << "\n";
    abort();
  }
  else
  {
    currentEventLoop = this;
  }
}

EventLoop::~EventLoop()
{
  quit_ = false;
  currentEventLoop = NULL;
}

void EventLoop::loop()
{
  assertThreadInLoop();
  looping_ = true;
  
}

void EventLoop::quit()
{
  quit_ = true;
}
}

