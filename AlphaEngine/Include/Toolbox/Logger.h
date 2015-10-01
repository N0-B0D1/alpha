#ifndef LOGGER_H
#define LOGGER_H

/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <memory>
#include <mutex>

#include <chrono>
#include <ctime>

namespace alpha
{
#ifdef ALPHA_DEBUG

    enum Severity
    {
        SDEBUG,
        SWARNING,
        SERROR,
    };

    class Logger
    {
    public:
        explicit Logger(const std::string & path)
            : m_path(path)
        {
            // remove the file if it already exists, so we can start a fresh log
            remove(path.c_str());
        }
        virtual ~Logger() { }

        template<Severity severity, typename...Args>
        void print( Args...args )
        {
            // take write lock, and open file for appending only
            m_writeMutex.lock();
            m_outStream.open(m_path.c_str(), std::ios_base::app | std::ios_base::out);

            {
                std::string time = this->GetTimestamp();
                print_val(time);

                std::string level;
                switch (severity)
                {

                    case Severity::SDEBUG:
                        level = " [  DEBUG  ] : ";
                        break;
                    case Severity::SWARNING:
                        level = " [*WARNING*] : ";
                        break;
                    case Severity::SERROR:
                        level = " [!!ERROR!!] : ";
                        break;
                }

                print_val(level);
                print_impl(args...);
            }
            m_outStream.close();

            m_writeMutex.unlock();
        }

    private:
        /**
         * Print the given string to the log file and stdout.
         */
        template<typename value>
        void print_val(value output)
        {
            m_outStream << output;
            std::cout << output;
        }

        void print_impl()
        {
            // finish with a newline, so it logs cleanly
            m_outStream << std::endl;
            std::cout << std::endl;
        }
         
        template<typename First, typename...Rest>
        void print_impl(First param1, Rest...rest)
        {
            print_val<First>(param1);
            print_impl(rest...);    
        }

        std::string GetTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);

            struct tm * timeinfo;
            timeinfo = std::localtime(&now_time);

            char buf[10];
            std::strftime(buf, 10, "%H:%M:%S", timeinfo);

            return std::string(buf);
        }

        const std::string m_path;
        std::mutex m_writeMutex;        
        std::ofstream m_outStream;
    };

    static Logger s_logger("alpha.log");

    #define LOG s_logger.print<Severity::SDEBUG>
    #define LOG_ERR s_logger.print<Severity::SERROR>
    #define LOG_WARN s_logger.print<Severity::SWARNING>

#else

    #define LOG(...)
    #define LOG_ERR(...)
    #define LOG_WARN(...)

#endif

}

#endif // LOGGER_H
