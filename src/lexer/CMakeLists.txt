add_paracl_library(
  lexer

  SOURCES
  generic-lexer.cpp
  lexer.cpp
  state-machine.cpp
  token.cpp

  LIBRARIES
  text

  TESTS
  lexer.cpp

  TOOL
  driver.cpp
)
