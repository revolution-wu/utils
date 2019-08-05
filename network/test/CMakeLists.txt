
add_executable(EventLoop_unittest EventLoop_unittest.cpp)
target_link_libraries(EventLoop_unittest network_shared)

add_executable(EventLoop_unittest2 EventLoop_unittest2.cpp)
target_link_libraries(EventLoop_unittest2 network_shared)

if(BOOSTTEST_LIBRARY)
  add_executable(Buffer_unittest Buffer_test.cpp)
  target_link_libraries(Buffer_unittest network_shared boost_unit_test_framework)
endif()

