
#include <iostream>
#include <string>
#include <vector>

struct  Coordinate {
    int x;
    int y;
    int z;
};

struct Hall {
    int ID;
    Coordinate LeftTop;
    Coordinate RightBottom;
    bool Status;
};

struct Room :Hall {
    std::vector<Coordinate> Input;
    std::vector<int> Wight;
    std::vector<int> HallID;
    std::string Type;
};