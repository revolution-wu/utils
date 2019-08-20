add_executable(config_test Config_test.cpp)
target_link_libraries(config_test utils)

add_executable(LogFile_test LogFile_test.cpp)
target_link_libraries(LogFile_test utils)

add_executable(logging_test Logging_test.cpp)
target_link_libraries(logging_test utils)
