#pragma once

#ifndef __LOG4CPLUS2C__H__
#define __LOG4CPLUS2C__H__

extern "C"
{
    typedef void (*log_callback)(void* data, unsigned int level, const char* const msg);

    bool add_rust_logger_handler(log_callback callback, void* callback_data);
    void remove_rust_logger_handler();
}

#endif // __LOG4CPLUS2C__H__
