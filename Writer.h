#pragma once

#include <ostream>

class Writer {
public:
    Writer(std::ostream& output);

    void WriteBit(bool bit);
    void WriteSequence(int64_t x, int16_t sz);

    void Done();

private:
    void Write(char c);

    std::ostream& output_;
    char cur_;
    int16_t ptr_;
};
