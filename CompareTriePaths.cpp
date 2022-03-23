#include "CompareTriePaths.h"

bool CompareTriePaths::operator()(const TriePath &a, const TriePath &b) const {
    return a.sz_ == b.sz_ ? a.symbol_ > b.symbol_ : a.sz_ > b.sz_;
}
