#pragma once

struct Mode {
    int sModeN;
    bool vMode;
    bool sMode;
    bool mMode;

    Mode():sModeN{0}, vMode{false}, sMode{false}, mMode{false} { }
};