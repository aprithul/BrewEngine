#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include "Module.hpp"

namespace PrEngine
{
    class AudioManager: public Module
    {
        public:

            AudioManager(const std::string& name, Int_32 priority);
            ~AudioManager();

            void start() override;
            void update() override;
            void end() override;

            void load_music(const std::string& file_name);
            void load_audio_clip(const std::string& file_name);
            void play_music();
            void play_audio_clip();
            
        private:
            std::map<std::string , Mix_Music*> music_list;
            std::map<std::string , Mix_Chunk*> audio_clip_list;
    };
}

#endif
