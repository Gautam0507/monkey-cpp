add_library(parser STATIC parser.cpp)

target_include_directories(parser PRIVATE ../ast)
target_include_directories(parser PRIVATE ../lexer)
target_include_directories(parser PRIVATE ../token)

target_link_libraries(parser token lexer ast)
 add_subdirectory(./tests)
