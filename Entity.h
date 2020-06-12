#pragma once
#include <bitset>
#include "Vector2D.h"

class Entity
{
protected:
    Vector2D pos;
    std::bitset<8> state;
    char image;

public:
    Vector2D getPos(void);
    void setPos(Vector2D v);
    std::bitset<8> getState(void);
    void setState(std::bitset<8> s);
    char getImage(void);
    void setImage(char i);
    Entity();
    Entity(Vector2D v, char i);
};