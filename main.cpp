#include "Archiver.h"

int main(int argc, char* argv[]) {
    Archiver archiver;
    try {
        archiver.Parse(argc, argv);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << std::endl;
    } catch (const std::runtime_error& error) {
        std::cout << "Runtime error: " << error.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error" << std::endl;
    }
}
