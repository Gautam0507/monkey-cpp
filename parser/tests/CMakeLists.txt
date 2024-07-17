enable_testing()

add_executable(parserTest parser_test.cpp)

target_include_directories(parserTest PRIVATE ../ ../../lexer)

target_link_libraries(parserTest lexer parser GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(parserTest)
