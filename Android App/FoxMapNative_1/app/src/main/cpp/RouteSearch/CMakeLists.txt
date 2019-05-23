cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR})

if (NOT TARGET Database)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Database RouteSearch/Bin)
endif()

add_library(RouteSearch SHARED
        DijkstrasAlgorithm.cpp
        GraphConstructor.cpp
        RouteSearch.cpp
        CMatrixGraph.cpp
        GraphConstructor.cpp

)

target_link_libraries(
        RouteSearch
)