add_paracl_library(
  interpreter

  SOURCES
  interpreter.cpp

  LIBRARIES
  lexer
  text
  parser

  TESTS
  interpreter.cpp

  TOOL
  driver.cpp
)
