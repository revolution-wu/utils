/**************************************************************************
 *      FileName: EventLoop_unittest2.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年08月05日 星期一 15时08分02秒
 * Last Modified: 2019-08-05 15:08:02
 * 
 **************************************************************************/
#include <iostream>
#include "../EventLoop.h"

net::EventLoop g_loop;

//测试一个EventLoop对象只能属于创建该对象的线程
//如果在其他线程调用，则终止程序
void func()
{
  std::cout << " threa id: " << std::this_thread::get_id() << "\n";
  g_loop.loop();
}
int main(int argc, char *argv[])
{
  std::thread t1(func);  
  t1.join();
  return 0;
}
