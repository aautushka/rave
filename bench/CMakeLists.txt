project(bench CXX)

include_directories (..)

set(SRC bench) 

add_executable(bench ${SRC})
target_link_libraries(bench benchmark pthread)
