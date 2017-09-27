cmake_minimum_required(VERSION 3.7)
project(lab2)

set(CMAKE_CXX_STANDARD 11)
find_package( Threads REQUIRED )  # Multithreading package required

add_executable(quicksort src/quicksort.cpp)
target_link_libraries(quicksort Threads::Threads)    # link with threads library

add_executable(pi src/pi.cpp)
target_link_libraries(pi Threads::Threads)

add_executable(integration src/integration.cpp)
target_link_libraries(integration Threads::Threads)
