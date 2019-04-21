//
// Created by Andrey on 20.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEM_H
#define FOXMAPNATIVE_1_MAPITEM_H
#include "Visitor.h"

class MapItem {
private:
    float vertices_[4];  //from top-left corner clockwise
    float color_[4];     //RGBA
    bool is_active_;
    bool is_clickable;
public:
    virtual void Accept(Visitor&) = 0;
};

class Room : public MapItem {
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

class Passage : public MapItem {
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

class Steps : public MapItem {
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

#endif //FOXMAPNATIVE_1_MAPITEM_H
