/**************************************************************************
 *      FileName: eventloop_unittest.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年08月05日 星期一 14时09分29秒
 * Last Modified: 2019-08-05 14:09:29
 * 
 **************************************************************************/
#include "..//EventLoop.h"
#include <typeinfo>

//测试一个线程只可以创建一个EventLoop对象
//如果创建两个，则终止程序
int main(int argc, char *argv[])
{

  net::EventLoop loop;
  std::thread::id threadId = loop.getThreadId();
  std::cout << " threaadId: " << threadId << "\n";
  std::cout << " threadId type: " << typeid(threadId).name() << "\n";
  net::EventLoop loop1;
  return 0;
}
