cmake_minimum_required(VERSION 3.4.1)

add_library(RouteSearch SHARED
        DijkstrasAlgorithm.cpp
        GraphConstructor.cpp
        RouteSearch.cpp
)

target_link_libraries(
        RouteSearch
)