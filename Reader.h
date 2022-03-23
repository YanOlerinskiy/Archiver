#pragma once

#include <istream>

class Reader {
public:
    Reader(std::istream& input);

    bool ReadBit();
    int64_t ReadBits(const int16_t x);

    bool AllRead();

    void Reset();
private:
    bool Read();

    std::istream& input_;
    char lst_;
    int ptr_;
};
