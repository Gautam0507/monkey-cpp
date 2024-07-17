add_library(ast STATIC ast.cpp)

target_include_directories(ast PUBLIC ../token)
target_include_directories(ast PUBLIC ../lexer)

target_link_libraries(ast token lexer)

