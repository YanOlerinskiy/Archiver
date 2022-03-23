#pragma once

#include <cstdint>
#include <cstddef>

const int16_t FILENAME_END = 256;
const int16_t ONE_MORE_FILE = 257;
const int16_t ARCHIVE_END = 258;
const int16_t NO_SYMBOL = 259;
const size_t MAX_NODES = 1000; // it is actually 260 + 259 = 519