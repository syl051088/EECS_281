// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#pragma once

struct Mode {
    int sModeN;
    bool vMode;
    bool sMode;
    bool mMode;

    Mode():sModeN{0}, vMode{false}, sMode{false}, mMode{false} { }
};