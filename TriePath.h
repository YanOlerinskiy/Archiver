#pragma once

#include <cstdint>

struct TriePath {
    int64_t path_;
    int16_t sz_;
    int16_t symbol_;
    TriePath();
    TriePath(int64_t path, int16_t sz, int16_t symbol);
};