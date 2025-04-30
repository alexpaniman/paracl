add_paracl_library(
  parser

  SOURCES
  parser.cpp

  LIBRARIES
  lexer
  text
  graphviz

  TESTS
  parser.cpp

  TOOL
  driver.cpp
)
