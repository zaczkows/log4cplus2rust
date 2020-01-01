#pragma once

#ifndef __LOG4CPLUS2C__H__
#define __LOG4CPLUS2C__H__

extern "C"
{
    struct LogInfoDetails
    {
        unsigned int level;
        const char* logger;
        double timestamp;
        const char* file;
        const char* function;
        int line;
        const char* msg;
    };
    typedef void (*log_callback)(const LogInfoDetails* log_info);

    bool add_rust_logger_handler(log_callback callback);
    void remove_rust_logger_handler();
}

#endif // __LOG4CPLUS2C__H__
