#include <iostream>

using namespace std;

/**
 *  Throws an error with the given information if something doesn't work out right.
 *  @param error    - The character string describing the error.
 *  @param source   - The filename of the code that failed.
 *  @param line     - The line number of the failing code.
 *  @param log      - The extra log data for the failure, if applicable.
 */
inline void BubbleError(string error, string source, int line, char log[])
{
    string logData(log);
    std::cout << "Error thrown: " << error << std::endl << "Source: " << source << ", line " << line << std::endl << logData;
    throw new std::exception();
}