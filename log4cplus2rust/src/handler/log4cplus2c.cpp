#include "log4cplus2c.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/logger.h>
#include <log4cplus/spi/loggingevent.h>

#include <list>

struct RustForwardingAppender : log4cplus::Appender
{
    RustForwardingAppender(log_callback& callback, void* callback_data)
      : m_callback(callback)
      , m_callback_data(callback_data)
    {}

    ~RustForwardingAppender() override { destructorImpl(); }

    void close() override {}

    void append(const log4cplus::spi::InternalLoggingEvent& event) override
    {
        auto& msg = event.getMessage();
        m_callback(m_callback_data, event.getLogLevel(), msg.c_str());
    }

    struct AppData
    {
        AppData(log4cplus::Logger& l, log4cplus::SharedAppenderPtr a)
          : logger(l)
          , appender(a)
        {}

        log4cplus::Logger& logger;
        log4cplus::SharedAppenderPtr appender;
    };

    void add_appender(AppData&& data) { m_appenders.emplace_back(data); }

  private:
    log_callback m_callback;
    void* m_callback_data;
    // Original log4cplus appender we are substituding for
    std::list<AppData> m_appenders;
};

static log4cplus::SharedAppenderPtr s_appender;

bool
add_rust_logger_handler(log_callback callback, void* callback_data)
{
    using namespace log4cplus;

    if (!callback) {
        return false;
    }

    RustForwardingAppender* rfa = nullptr;
    if (!s_appender) {
        rfa = new RustForwardingAppender(callback, callback_data);
        s_appender = log4cplus::SharedAppenderPtr(rfa);
        s_appender->setName(LOG4CPLUS_TEXT("CPP2RUST_APPENDER"));
    }

    if (!rfa || !s_appender) {
        return false;
    }

    LoggerList loggers = Logger::getCurrentLoggers();
    for (Logger& l : loggers) {
        SharedAppenderPtrList appenders = l.getAllAppenders();
        for (SharedAppenderPtr& a : appenders) {
            if (dynamic_cast<log4cplus::ConsoleAppender*>(a.get())) {
                // Remove current appender
                l.removeAppender(a);
                // Add ours
                rfa->add_appender(RustForwardingAppender::AppData(l, a));
                l.addAppender(s_appender);
            }
        }
    }

    return true;
}

void
remove_rust_logger_handler()
{}
