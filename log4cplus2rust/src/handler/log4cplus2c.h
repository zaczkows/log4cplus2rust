#pragma once

#ifndef __LOG4CPLUS2C__H__
#define __LOG4CPLUS2C__H__

extern "C"
{
    typedef void (*log_callback)(void* data, const char* const name);

    void add_rust_logger_handler(log_callback callback, void* callback_data);
}

#endif // __LOG4CPLUS2C__H__
