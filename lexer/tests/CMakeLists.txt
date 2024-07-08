enable_testing()

add_executable(
  lexer_test
  lexer_test.cpp
)

target_include_directories(lexer_test PRIVATE ../)

target_link_libraries(
  lexer_test
  GTest::gtest_main
  lexer
  token
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
