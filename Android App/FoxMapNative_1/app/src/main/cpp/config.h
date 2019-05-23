//
// Created by Andrey on 23.05.2019.
//

#ifndef FOXMAPNATIVE_1_CONFIG_H
#define FOXMAPNATIVE_1_CONFIG_H

namespace conf{
    const float door_dims[2] = {5, 1.5};
    const float eps = 1;
    const size_t dims_count = 2;
    const size_t rect_vertex_count = 6;
    const size_t path_dash_step = 5;
    const size_t max_id_length = 5;
    enum {DBMASTER, ASSET};
    const bool database_src = DBMASTER;
    enum {REAL_ROUTE_SEARCH, MOCK_ROUTE_SEARCH};
    const int route_search_src = REAL_ROUTE_SEARCH;
    const std::string steps_name = database_src == ASSET ? "Steps" : "Step";
    const char map_name_divider = '_';
}
#endif //FOXMAPNATIVE_1_CONFIG_H
