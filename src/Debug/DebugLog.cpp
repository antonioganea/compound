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


void DebugLog::fatal( const char* format, ... ){
    if ( LogLevel::Fatal > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Fatal]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
void DebugLog::error( const char* format, ... ){
    if ( LogLevel::Error > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Error]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
void DebugLog::warn ( const char* format, ... ){
    if ( LogLevel::Warn > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Warn]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
void DebugLog::info ( const char* format, ... ){
    if ( LogLevel::Info > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Info]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
void DebugLog::debug ( const char* format, ... ){
    if ( LogLevel::Debug > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Debug]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
void DebugLog::trace( const char* format, ... ){
    if ( LogLevel::Trace > outputLevel )
        return;
    va_list args;
    fprintf( stderr, "[Trace]" );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}
