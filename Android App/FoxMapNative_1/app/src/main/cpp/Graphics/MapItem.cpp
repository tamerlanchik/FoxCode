#include "MapItem.h"
using namespace gls;
size_t Room::size = 0;
size_t Room::count = 0;
size_t Passage::size = 0;
size_t Passage::count_ = 0;

void Passage::ConvertRect2Lines(std::vector<Point>& v) {

}

std::vector<Point> Passage::GeneratePlaceholders(const std::vector<Passage> & v) {
    std::vector<Point> res;
    res.reserve(2*(v.size()-1));
    // условие: обьекты идут по порядку.
    for(size_t i = 0; i < v.size() - 1; i++){

    }
    return res;
}