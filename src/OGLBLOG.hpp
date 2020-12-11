#ifndef OGLBLOG
#define OGLBLOG

/** A class for logging functions. */
class Log 
{
    public:
        /**
         *  Constructs a logger object.
         *  Note: Make this static
         */
        Log();

        void setDebug(bool debug_on);

        /**
         *  Prints an error message if debug mode is enabled.
         *  @param message - The message to be printed to cerr.
         */
        void e(const char* message);

        /**
         *  Prints a debug message if debug mode is enabled.
         *  @param message - The message to be printed to cout.
         */
        void d(const char* message);

        /**
         *  Prints a debug message to the log file if debug mode is enabled.
         *  Not thread-safe.
         *  TODO: Implement the log file writer.
         *  @param message - The message to be printed to the log file.
         */
        void f(const char* message);

    private:
        static const bool DEFAULT_DEBUG_MODE = true; // Default mode for debug
        bool debug; // Determines whether debug messages should be printed

};

#endif