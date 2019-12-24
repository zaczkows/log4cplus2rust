
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

#include "example.h"

void
setup_logging()
{

    std::string log_props = "log4cplus.appender.CL=log4cplus::ConsoleAppender\n"
                            "log4cplus.appender.CL.ImmediateFlush=true\n"
                            "log4cplus.appender.CL.layout=log4cplus::PatternLayout\n"
                            "log4cplus.appender.CL.layout.ConversionPattern=[ %-5p] %m (%l)%n\n"
                            "log4cplus.logger.dex=DEBUG, CL\n"
                            "log4cplus.logger.iex=INFO,CL\n";
    std::istringstream props_stream(log_props);
    log4cplus::PropertyConfigurator configurator(props_stream);
    configurator.configure();
}

void
run_example()
{
    log4cplus::Logger dex = log4cplus::Logger::getInstance("dex");
    log4cplus::Logger iex = log4cplus::Logger::getInstance("iex");
    log4cplus::Logger uex = log4cplus::Logger::getInstance("uex");
    LOG4CPLUS_TRACE(dex, "TEST TRACE MSG");
    LOG4CPLUS_DEBUG(dex, "TEST DEBUG MSG");
    LOG4CPLUS_INFO(dex, "TEST INFO MSG");
    LOG4CPLUS_WARN(dex, "TEST WARN MSG");
    LOG4CPLUS_ERROR(dex, "TEST ERROR MSG");
    LOG4CPLUS_TRACE(iex, "TEST TRACE MSG");
    LOG4CPLUS_DEBUG(iex, "TEST DEBUG MSG");
    LOG4CPLUS_INFO(iex, "TEST INFO MSG");
    LOG4CPLUS_WARN(iex, "TEST WARN MSG");
    LOG4CPLUS_ERROR(iex, "TEST ERROR MSG");
}
