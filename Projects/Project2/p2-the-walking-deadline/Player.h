// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#pragma once

struct Player {
    uint32_t quiver_capacity;
    bool isAlive;

    Player(): quiver_capacity{0}, isAlive{true} { }

    void refill(uint32_t capacity) {
        quiver_capacity = capacity;
    }   
};