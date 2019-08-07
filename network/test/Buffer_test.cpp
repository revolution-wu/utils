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

BOOST_AUTO_TEST_CASE(testReadInt)
{
  Buffer buf;
  buf.append("HTTP");

  BOOST_CHECK_EQUAL(buf.readableBytes(), 4);
  BOOST_CHECK_EQUAL(buf.peekInt8(), 'H');
  int top16 = buf.peekInt16();
  printf("top16 = %d\n", top16);
  BOOST_CHECK_EQUAL(top16, 'H'*256 + 'T');
  BOOST_CHECK_EQUAL(buf.peekInt32(), top16*65536 + 'T'*256 + 'P');

  BOOST_CHECK_EQUAL(buf.readInt8(), 'H');
  BOOST_CHECK_EQUAL(buf.readInt16(), 'T'*256 + 'T');
  BOOST_CHECK_EQUAL(buf.readInt8(), 'P');
  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), 1024);

  buf.appendInt8(-1);
  buf.appendInt16(-2);
  buf.appendInt32(-3);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 7);
  BOOST_CHECK_EQUAL(buf.readInt8(), -1);
  BOOST_CHECK_EQUAL(buf.readInt16(), -2);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 4);
  BOOST_CHECK_EQUAL(buf.readInt32(), -3);
  buf.appendInt32(1);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 4);
  BOOST_CHECK_EQUAL(buf.readInt32(), 1);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);

  // -1在内存中存储的是11111111  11111111  11111111 11111111
  // 因此不存在大小端模式了，其他则不然
  int x = -1;
  buf.append(&x, sizeof(int));
  BOOST_CHECK_EQUAL(buf.readInt8(), -1);
  BOOST_CHECK_EQUAL(buf.readInt8(), -1);
  BOOST_CHECK_EQUAL(buf.readInt16(), -1);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 0);

  char y = 1;
  buf.append(&y, sizeof(char));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 1);
  BOOST_CHECK_EQUAL(buf.readInt8(), 1);

  short value = 1;
  buf.append(&value, sizeof(short));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 2);
  BOOST_CHECK_EQUAL(buf.readNoConvertInt16(), 1);
  int z = 1;
  buf.append(&z, sizeof(z));
  BOOST_CHECK_EQUAL(buf.readNoConvertInt32(), 1);
}

BOOST_AUTO_TEST_CASE(testBufferPrepend)
{
  Buffer buf;
  buf.append(std::string(200, 'y'));
  BOOST_CHECK_EQUAL(buf.readableBytes(), 200);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize-200);
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

  int x = 0;
  buf.prepend(&x, sizeof x);
  BOOST_CHECK_EQUAL(buf.readableBytes(), 204);
  BOOST_CHECK_EQUAL(buf.writeableBytes(), Buffer::kInitalSize-200);
  BOOST_CHECK_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend - 4);
}
