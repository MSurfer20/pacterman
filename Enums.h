#pragma once

enum EnityState
{
    STATE_ACTIVE,
    STATE_INACTIVE
};

enum GhostStateID
{
    HUNTING,
    VULNERABLE,
    RETREATING
};

enum Direction
{
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
    DIR_RIGHT
};

enum ObjectID
{
    ID_PLAYER,
    ID_GHOST,
    ID_WALL,
    ID_FRUIT,
    ID_DOT,
    ID_BLANK,
    ID_UNKNOWN
};
