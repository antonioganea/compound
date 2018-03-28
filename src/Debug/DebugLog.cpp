#include "DebugLog.h"

#include <stdio.h>
#include <cstdarg>

LogLevel DebugLog::outputLevel = LogLevel::All;

void DebugLog::print( LogLevel level, const char* format, ... ) {
    if ( level > outputLevel )
        return;
    va_list args;
    switch( level ){
        case LogLevel::Fatal:
            fprintf( stderr, "[Fatal]" );
            break;
        case LogLevel::Error:
            fprintf( stderr, "[Error]" );
            break;
        case LogLevel::Warn:
            fprintf( stderr, "[Warning]" );
            break;
        case LogLevel::Info:
            fprintf( stderr, "[Info]" );
            break;
        case LogLevel::Debug:
            fprintf( stderr, "[Debug]" );
            break;
        case LogLevel::Trace:
            fprintf( stderr, "[Trace]" );
            break;
        default:
            break;
    }
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}



//#define DebugLog::fatal( ... ) DebugLog::print( LogLevel::Fatal, ... )
/*
static void fatal( const char* fmt... );
static void error( const char* fmt... );
static void warn ( const char* fmt... );
static void info ( const char* fmt... );
static void debug( const char* fmt... );
static void trace( const char* fmt... );
*/
