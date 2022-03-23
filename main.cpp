#include "Archiver.h"

int main(int argc, char* argv[]) {
    Archiver archiver;
    try {
        archiver.Parse(argc, argv);
    } catch(...) {
        std::cout << "Error" << std::endl;
    }
}
