#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>

namespace alpha
{
//#ifdef DEBUG

    enum Severity
    {
        DEBUG,
        WARNING,
        ERROR,
    };

    class Logger
    {
    public:
        explicit Logger(const std::string & path)
            : m_lineNumber(0)
        {
            m_outStream.open(path.c_str(), std::ios_base::binary | std::ios_base::out);
        }
        virtual ~Logger()
        {
            m_outStream.close();
        }

        template<Severity severity>
        void print(const std::string & msg)
        {
            m_writeMutex.lock();

            m_outStream << this->GetLineNumber();

            switch (severity)
            {
                case Severity::DEBUG:
                    m_outStream << "<DEBUG>   : ";
                    break;
                case Severity::WARNING:
                    m_outStream << "<WARNING> : ";
                    break;
                case Severity::ERROR:
                    m_outStream << "<ERROR>   : ";
                    break;
            }

            m_outStream << msg << std::endl;;

            m_writeMutex.unlock();
        }

    private:
        std::string GetLineNumber()
        {
            std::stringstream number;

            number.str("");
            number.fill('0');
            number.width(7);
            number << m_lineNumber++ << " -- ";
            
            return number.str();
        }

        unsigned m_lineNumber;
        std::mutex m_writeMutex;
        
        std::ofstream m_outStream;
    };

    static Logger s_logger("alpha.log");

    #define LOG s_logger.print<Severity::DEBUG>
    #define LOG_ERR s_logger.print<Severity::ERROR>
    #define LOG_WARN s_logger.print<Severity::WARNING>

/*
#else

    #define LOG (...)
    #define LOG_ERR (...)
    #define LOG_WARN (...)

#endif
*/
}

#endif // LOGGER_H
