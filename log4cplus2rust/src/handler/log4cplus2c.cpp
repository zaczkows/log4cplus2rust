#include "log4cplus2c.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/logger.h>

struct ForwardingAppender : log4cplus::Appender
{
    ForwardingAppender(log_callback& callback, void* callback_data)
      : m_callback(callback)
      , m_callback_data(callback_data)
    {}

    ~ForwardingAppender() override { destructorImpl(); }

    void close() override {}

    void append(const log4cplus::spi::InternalLoggingEvent& /* event */) override
    {
        if (m_callback) {
            m_callback(m_callback_data, "blah");
        }
    }

  private:
    log_callback m_callback;
    void* m_callback_data;
};

static log4cplus::SharedAppenderPtr s_appender;

void
add_rust_logger_handler(log_callback callback, void* callback_data)
{
    using namespace log4cplus;

    if (!s_appender) {
        s_appender = log4cplus::SharedAppenderPtr(new ForwardingAppender(callback, callback_data));
        s_appender->setName(LOG4CPLUS_TEXT("CPP2RUST_APPENDER"));
    }

    LoggerList loggers = Logger::getCurrentLoggers();
    for (Logger& l : loggers) {
        SharedAppenderPtrList appenders = l.getAllAppenders();
        for (SharedAppenderPtr& a : appenders) {
            if (dynamic_cast<log4cplus::ConsoleAppender*>(a.get())) {
                auto name = a->getName();
                // Remove current appender
                l.removeAppender(name);
                // Add ours
                l.addAppender(s_appender);
            }
        }
    }
}
