#ifndef DEBUGLOG_H
#define DEBUGLOG_H

enum LogLevel { Off, Fatal, Error, Warn, Info, Debug, Trace, All };

class DebugLog
{
    public:
        static void print( LogLevel level, const char* format, ... );
        static void fatal( const char* fmt... );
        static void error( const char* fmt... );
        static void warn ( const char* fmt... );
        static void info ( const char* fmt... );
        static void debug( const char* fmt... );
        static void trace( const char* fmt... );

        static LogLevel outputLevel;
    protected:
    private:
        DebugLog();
};

#endif // DEBUGLOG_H
