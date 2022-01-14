#ifndef EMPTY_OBJECT_HPP
#define EMPTY_OBJECT_HPP

#include "Object.hpp"

class EmptyObject : public Object {
public:
    EmptyObject();
    ~EmptyObject();

    void Render() override;
};
#endif