#include "Logger.hpp"

namespace PrEngine{


    const Char_8* LogTypeStrings[] = {"ERROR", "WARNING", "LOG"};
    std::ofstream log_file;
    const Char_8* log_file_path;
    Bool_8 log_enabled = true;
    LogLevel log_level = LOGLEVEL_MIN;



    void close_log_file()
    {
        log_file.close();
    }

    void set_log_file_path(const Char_8* log_file_path)
    {
        PrEngine::log_file_path = log_file_path;
    }

  

    void LOG(LogType type, const std::string& log_text, const std::string& log_text_2,const std::string& log_text_3,const std::string& log_text_4,const std::string& log_text_5,const std::string& log_text_6,const std::string& log_text_7,const std::string& log_text_8,const std::string& log_text_9,const std::string& log_text_10)
    {

        if(!log_enabled)
            return;
        
        std::string log_extra_info = "";    // depending on log level, this string may or may not contain progressively more text

        switch (log_level)
        {
        case LOGLEVEL_MAX:
            break;
        case LOGLEVEL_MODERATE:
            break;
        case LOGLEVEL_MIN:
            break;        
        }

        switch (type)
        {
        case LOGTYPE_GENERAL:
            std::cout<<log_extra_info<<log_text<<log_text_2<<log_text_3<<log_text_4<<log_text_5<<log_text_6<<log_text_7<<log_text_8<<log_text_9<<log_text_10<<std::endl;
            break;
        case LOGTYPE_WARNING:
            std::cout<< "\033[1;33m" << "Warning:"<<"\033[0m"<<log_extra_info<<log_text<<log_text_2<<log_text_3<<log_text_4<<log_text_5<<log_text_6<<log_text_7<<log_text_8<<log_text_9<<log_text_10<<std::endl;
            break;
        case LOGTYPE_ERROR:
            std::cout<< "\033[1;31m" << "Error:"<<"\033[0m"<<log_extra_info<<log_text<<log_text_2<<log_text_3<<log_text_4<<log_text_5<<log_text_6<<log_text_7<<log_text_8<<log_text_9<<log_text_10<<std::endl;
            break;
        }

        /*if(log_file)
        {    
            if(log_file.is_open())
                log_file<<LogTypeStrings[type]<<": "<<log_text<<log_text_2<<log_text_3<<log_text_4<<log_text_5<<log_text_6<<log_text_7<<log_text_8<<log_text_9<<log_text_10;
        }*/
    }

}