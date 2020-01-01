#include "log4cplus2c.h"

#include <log4cplus/consoleappender.h>
#include <log4cplus/logger.h>
#include <log4cplus/spi/loggingevent.h>

#include <list>
#include <mutex>

struct RustForwardingAppender : log4cplus::Appender
{
    RustForwardingAppender(log_callback& callback)
      : m_callback(callback)
    {}

    ~RustForwardingAppender() override { destructorImpl(); }

    void close() override {}

    void append(const log4cplus::spi::InternalLoggingEvent& event) override
    {
        LogInfoDetails details;
        details.level = event.getLogLevel();
        details.logger = event.getLoggerName().c_str();
        details.timestamp = static_cast<double>(event.getTimestamp().usec()) / 1000000.0;
        details.file = event.getFile().c_str();
        details.function = event.getFunction().c_str();
        details.line = event.getLine();
        details.msg = event.getMessage().c_str();
        m_callback(&details);
    }

    struct AppData
    {
        AppData(log4cplus::Logger& l, log4cplus::SharedAppenderPtr a)
          : logger(l)
          , appender(a)
        {}

        log4cplus::Logger logger;
        log4cplus::SharedAppenderPtr appender;
    };

    void add_appender(AppData&& data) { m_appenders.emplace_back(data); }

    std::list<AppData>& appenders() { return m_appenders; }

  private:
    log_callback m_callback;
    // Original log4cplus appender we are substituting for
    std::list<AppData> m_appenders;
};

static std::mutex s_mutex;
static log4cplus::SharedAppenderPtr s_appender;

bool
add_rust_logger_handler(log_callback callback)
{
    using namespace log4cplus;

    std::lock_guard<std::mutex> _guard(s_mutex);
    if (!callback) {
        return false;
    }

    RustForwardingAppender* rfa = nullptr;
    if (!s_appender) {
        rfa = new RustForwardingAppender(callback);
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
{
    std::lock_guard<std::mutex> _guard(s_mutex);
    if (!s_appender) {
        return;
    }

    for (RustForwardingAppender::AppData& app_data :
         static_cast<RustForwardingAppender*>(s_appender.get())->appenders()) {
        app_data.logger.addAppender(app_data.appender);
        app_data.logger.removeAppender(s_appender);
    }
}
