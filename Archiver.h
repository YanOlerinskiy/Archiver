#pragma once

#include <iostream>

#include "Writer.h"

class Archiver {
public:
    void Parse(int argc, char** argv);
    void Archive(char* file_name, std::istream& input, std::ostream& output);
    void Unarchive(std::istream& input);

private:
    void Archive(char* file_name, std::istream& input, Writer& writer, bool not_last);
};