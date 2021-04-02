//
//  FrameRateRegulatorComponent.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef LOGGER
#define LOGGER

//#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Utils.hpp"

namespace PrEngine{


    typedef enum
    {
        LOGTYPE_ERROR,
        LOGTYPE_WARNING,    
        LOGTYPE_GENERAL

    }LogType;


    typedef enum
    {
        LOGLEVEL_MAX,
        LOGLEVEL_MODERATE,
        LOGLEVEL_MIN
    }LogLevel;


    // Logging functions
    extern const Char_8* log_file_path;
    extern Bool_8 log_enabled;
    extern LogLevel log_level;
    extern void LOG(LogType type = LOGTYPE_GENERAL, const std::string& log_text="", const std::string& log_text_2="", const std::string& log_text_3="",const std::string& log_text_4="",const std::string& log_text_5="",const std::string& log_text_6="",const std::string& log_text_7="",const std::string& log_text_8="",const std::string& log_text_9="",const std::string& log_text_10="");
    extern void close_log_file();
    extern void set_log_file_path(const Char_8* log_file_path);
    //extern PrLog logError;
}


#endif