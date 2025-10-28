#ifndef PREPROCESOR_HH
#define PREPROCESOR_HH

#include <string>

/**
 * @brief Reads the entire content of a file into a string.
 *
 * @param filename The name (path) of the file to read, typically from argv[1].
 * @return std::string The content of the file. Returns an empty string
 * if the file cannot be opened.
 */
std::string readFileAndPreprocess(const char* filename);

#endif // PREPROCESOR_HH