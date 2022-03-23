#include "Writer.h"

Writer::Writer(std::ostream& output) : output_(output) {
    cur_ = 0;
    ptr_ = 0;
}

void Writer::Write(char c) {
    output_ << c;
    output_.flush();
}

void Writer::WriteBit(bool bit) {
    cur_ |= (bit << ptr_);
    ++ptr_;
    if (ptr_ == 8) {
        Write(cur_);
        cur_ = 0;
        ptr_ = 0;
    }
}

void Writer::WriteSequence(int64_t x, int16_t sz) {
    for (int i = 0; i < sz; ++i) {
        WriteBit((x >> i) & 1);
    }
}

void Writer::Done() {
    if (ptr_ > 0) {
        Write(cur_);
    }
    cur_ = 0;
    ptr_ = 8;
}