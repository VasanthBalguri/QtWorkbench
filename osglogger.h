#ifndef OSGLOGGER_H
#define OSGLOGGER_H

#include <osgDB/ReadFile>
#include <fstream>
#include <boost/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>

class OsgLogger : public osg::NotifyHandler{
public:
    OsgLogger(const std::string& file_path)
    {
        _log.open(file_path.c_str());
    }

    virtual ~OsgLogger(){_log.close();}

    virtual void notify(osg::NotifySeverity severity,const char* msg)
    {
        std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _log<<'['<<current_time<<']'<<msg;
        _log<<'['<<time_fmt(boost::chrono::timezone::local, "%H:%M:%S") <<system_clock::now()<<']\n';
    }

protected:
    std::ofstream _log;
};
#endif // OSGLOGGER_H
