#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>

namespace alpha
{
#ifdef ALPHA_DEBUG

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
            : m_path(path)
            , m_lineNumber(0)
        {
            //m_outStream.open(path.c_str(), std::ios_base::binary | std::ios_base::out);
            // remove the file if it already exists, so we can start a fresh log
            remove(path.c_str());
        }
        virtual ~Logger()
        {
            //m_outStream.close();
        }

        template<Severity severity, typename...Args>
        void print( Args...args )
        {
            // take write lock, and open file for appending only
            m_writeMutex.lock();
            m_outStream.open(m_path.c_str(), std::ios_base::app | std::ios_base::out);

            //if (m_outStream.open)
            {                
                m_outStream << this->GetLineNumber();

                switch (severity)
                {
                    case Severity::DEBUG:
                        m_outStream << "[  DEBUG  ] : ";
                        break;
                    case Severity::WARNING:
                        m_outStream << "[*WARNING*] : ";
                        break;
                    case Severity::ERROR:
                        m_outStream << "[!!ERROR!!] : ";
                        break;
                }

                //m_outStream << msg << std::endl;
                print_impl(args...);
            }
            m_outStream.close();

            m_writeMutex.unlock();
        }

    private:
        void print_impl()
        {
            //policy->write( get_logline_header() + log_stream.str() );
            //m_outStream << this.GetLineNumber();
            m_outStream << std::endl;
        }
         
        template<typename First, typename...Rest>
        void print_impl(First param1, Rest...rest)
        {
            m_outStream << param1;
            print_impl(rest...);    
        }

        std::string GetLineNumber()
        {
            std::stringstream number;

            number.str("");
            number.fill('0');
            number.width(7);
            number << m_lineNumber++ << " -- ";
            
            return number.str();
        }

        const std::string m_path;

        unsigned m_lineNumber;
        std::mutex m_writeMutex;
        
        std::ofstream m_outStream;
    };

    static Logger s_logger("alpha.log");

    #define LOG s_logger.print<Severity::DEBUG>
    #define LOG_ERR s_logger.print<Severity::ERROR>
    #define LOG_WARN s_logger.print<Severity::WARNING>


#else

    #define LOG(...)
    #define LOG_ERR(...)
    #define LOG_WARN(...)

#endif

}

#endif // LOGGER_H
