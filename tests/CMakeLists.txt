project(tests CXX)

enable_testing()
include_directories (..)

set(SRC 
  http_parser_tests
  http2_parser_tests
)

add_executable(tests ${SRC})
target_link_libraries(tests gtest pthread)
