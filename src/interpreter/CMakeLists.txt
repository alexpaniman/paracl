add_paracl_library(
  interpreter

  SOURCES
  interpreter.cpp

  LIBRARIES
  lexer
  text
  graphviz
  parser

  TESTS
  interpreter.cpp

  TOOL
  driver.cpp
)
