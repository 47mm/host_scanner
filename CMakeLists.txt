project(host_scanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
find_package (Threads)
add_executable(host_scanner ./src/main.cc ./src/parser.cc ./src/connection.cc ./src/nc.cc)
target_link_libraries (host_scanner ${CMAKE_THREAD_LIBS_INIT})
