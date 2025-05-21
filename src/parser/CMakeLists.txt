add_paracl_library(
  parser

  SOURCES
  parser.cpp

  LIBRARIES
  lexer
  text

  TESTS
  parser.cpp

  TOOL
  driver.cpp
)
