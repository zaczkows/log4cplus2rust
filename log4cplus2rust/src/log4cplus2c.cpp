#include "log4cplus2c.h"

#include <log4cplus/logger.h>

struct ForwardingAppender : log4cplus::Appender
{
    ForwardingAppender() {}

    ~ForwardingAppender() override { destructorImpl(); }

    void close() override {}

    void append(const log4cplus::spi::InternalLoggingEvent& /* event */) override {}
};

static log4cplus::SharedAppenderPtr s_appender;

void
add_rust_handler()
{
    using namespace log4cplus;

    if (!s_appender) {
        s_appender = log4cplus::SharedAppenderPtr(new ForwardingAppender());
        s_appender->setName(LOG4CPLUS_TEXT("CPP2RUST_APPENDER"));
    }

    LoggerList loggers = Logger::getCurrentLoggers();
    for (Logger& l : loggers) {
        l.addAppender(s_appender);
    }
}
