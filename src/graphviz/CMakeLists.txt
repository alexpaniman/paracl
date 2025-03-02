add_paracl_library(
  graphviz

  SOURCES
  graphviz.cpp
  ast-node.cpp
  attributes.cpp

  LIBRARIES
  lexer
  text
  parser

  TESTS
  graphviz.cpp

  TOOL
  driver.cpp
)
