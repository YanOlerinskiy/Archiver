#include "Reader.h"

Reader::Reader(std::istream& input) : input_(input) {
    lst_ = 0;
    ptr_ = 8;
}

bool Reader::Read() {
    if (!input_.get(lst_)) {
        return 0;
    }
    ptr_ = 0;
    return 1;
}

bool Reader::ReadBit() {
    if (ptr_ == 8) {
        if (!Read()) {
            throw std::invalid_argument("Archive is incorrectly formatted");
        }
    }
    ++ptr_;
    return ((lst_ >> (ptr_ - 1)) & 1);
}

int64_t Reader::ReadBits(const int16_t x) {
    int64_t ans = 0;
    for (int16_t i = 0; i < x; ++i) {
        ans += (ReadBit() << i);
    }
    return ans;
}

void Reader::Reset() {
    input_.clear();
    input_.seekg(0);
    lst_ = 0;
    ptr_ = 8;
}

bool Reader::AllRead() {
    return ptr_ == 8 && !Read();
}

