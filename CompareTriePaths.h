#pragma once

#include "TriePath.h"

struct CompareTriePaths {
    bool operator()(const TriePath& a, const TriePath& b) const;
};