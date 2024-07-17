add_library(repl STATIC repl.cpp)

target_include_directories(repl PRIVATE ../token )
target_include_directories(repl PRIVATE ../lexer ) 
target_include_directories(repl PRIVATE ../parser)
target_include_directories(repl PRIVATE ../ast)


target_link_libraries(repl token lexer parser ast)

