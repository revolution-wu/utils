/**************************************************************************
 *      FileName: Buffer_test.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年08月05日 星期一 22时13分54秒
 * Last Modified: 2019-08-05 22:13:54
 * 
 **************************************************************************/
#include "../buffer.h"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using net::Buffer;
BOOST_AUTO_TEST_CASE(testBufferAppend)
{
  Buffer buf;

  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), 1024);
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

  std::string str(200, 'r');
  buf.append(str);
  printf("buf writeableBytes() = %ld\n", buf.writeableBytes());
  BOOST_CHECK_EQUAL(buf.readableBytes(), 200);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize - str.size());
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

  const std::string str2 = buf.retrieveAsString(50);
  BOOST_CHECK_EQUAL(buf.readableBytes(), str.size() - str2.size());
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize - str.size());
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());
  BOOST_CHECK_EQUAL(str2, std::string(50, 'r'));

  buf.append(str);                                                                                                     
  BOOST_CHECK_EQUAL(buf.readableBytes(), 2*str.size() - str2.size());
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize - 2*str.size());
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());

  const std::string str3 =  buf.retrieveiAllAsString();                                                                      
  BOOST_CHECK_EQUAL(str3.size(), 350);                                                                                 
  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);                                                                           
  BOOST_CHECK_EQUAL(buf.writeableBytes(), 1024);                                                        
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);                                                    
  BOOST_CHECK_EQUAL(str3, std::string(350, 'r'));
}

BOOST_AUTO_TEST_CASE(testBufferGrow)
{
  Buffer buf;
  std::string str(408,'x');
  buf.append(str);
  BOOST_CHECK_EQUAL(buf.readableBytes(), str.size());
  BOOST_CHECK(buf.readableBytes());
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize - str.size());

  buf.retrieve(8);
  buf.append(std::string(1000,'y'));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 1400);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), buf.size() - (buf.readableBytes() + buf.prependableBytes()));
  BOOST_CHECK_EQUAL(buf.prependableBytes(), 8);
  BOOST_CHECK_GT(buf.capacityBuf(), buf.size());
  printf("buf. writeableBytes() = %ld\n", buf.writeableBytes());
  buf.append(std::string(200, 'z'));
  BOOST_CHECK_EQUAL(buf.writeableBytes(), buf.size() - (buf.readableBytes() + buf.prependableBytes()));
  buf.retrieveAll();
  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
  BOOST_CHECK_EQUAL(buf.writeableBytes(),  buf.size() - buf.prependableBytes());
  BOOST_CHECK_EQUAL(buf.prependableBytes(), 8);
  printf("buf .capacity = %ld\n", buf.capacityBuf());
  printf("buf .size = %ld\n", buf.size());

}

BOOST_AUTO_TEST_CASE(testBufferGrow1)
{
  Buffer buf;
  std::string str(1400, 'r');
  buf.append(str);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), 0);
}

BOOST_AUTO_TEST_CASE(testBufferInsideGrow)
{
  Buffer buf;
  buf.append(std::string(800, 'y'));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 800);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize-800);

  buf.retrieve(500);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 300);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize-800);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), buf.size() - (buf.readableBytes() + buf.prependableBytes()));
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend+500);

  buf.append(std::string(300, 'z'));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 600);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), buf.size() - (buf.readableBytes() + buf.prependableBytes()));
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}
