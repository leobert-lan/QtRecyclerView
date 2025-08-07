cmake_minimum_required(VERSION 3.5)

project(QtRecyclerView)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(QtRecyclerView
    src/main.cpp
    src/RecyclerView.cpp
    src/LinearLayoutManager.cpp
        src/GridLayoutManager.cpp
        src/GridLayoutManager.h
        src/RecyclerCachePool.h
        src/AbstractLayoutManager.h
        src/LinearLayoutManager.cpp
        src/myviewholder.cpp
        src/RecyclerViewNotifier.h
)

target_include_directories(QtRecyclerView PRIVATE src)
target_link_libraries(QtRecyclerView Qt6::Widgets)
