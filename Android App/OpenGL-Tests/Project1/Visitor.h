//
// Created by Andrey on 20.04.2019.
//

#ifndef FOXMAPNATIVE_1_VISITOR_H
#define FOXMAPNATIVE_1_VISITOR_H

class Room;
class Passage;
class Steps;

class Visitor {
public:
    virtual void visit(const Room&) = 0;
    virtual void visit(const Passage&) = 0;
    virtual void visit(const Steps&) = 0;
};

#endif //FOXMAPNATIVE_1_VISITOR_H
