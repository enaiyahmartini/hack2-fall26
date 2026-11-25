cmake_minimum_required(VERSION 3.20)
project(hackathon C CXX)
set(CMAKE_CXX_STANDARD 17)


include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
 URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz)
FetchContent_MakeAvailable(raylib)


# ==========================================================
#  class demo: example.cpp
# ==========================================================
add_executable(example example.cpp)
target_link_libraries(example PRIVATE raylib)


# ==========================================================
#  HACKATHON: your program. List every .cpp file here.
#  Made a new class? Add its .cpp file to this list.
#  (Don't list .h files, only .cpp files.)
# ==========================================================
add_executable(main
   main.cpp
)
target_link_libraries(main PRIVATE raylib)


# Find libmusicbrainz5
find_package(PkgConfig REQUIRED)
pkg_check_modules(MB5 REQUIRED libmusicbrainz5)


# gemini 9:40 pm 11/24/26 was helping us fix linking the libraries together
# Add include directories and link libraries to your "main" executable
target_include_directories(main PRIVATE ${MB5_INCLUDE_DIRS})
target_link_libraries(main PRIVATE raylib musicbrainz5cc ${MB5_LIBRARIES})