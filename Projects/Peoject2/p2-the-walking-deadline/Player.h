#pragma once

struct Player {
public:
    uint32_t quiver_capacity;
    bool isAlive;

    Player(): quiver_capacity{0}, isAlive{true} { }
};