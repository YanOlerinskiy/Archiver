#include "TriePath.h"
#include "Constants.h"

TriePath::TriePath() : path_(0), sz_(0), symbol_(NO_SYMBOL) {}

TriePath::TriePath(int64_t path, int16_t sz, int16_t symbol) : path_(path), sz_(sz), symbol_(symbol) {}
