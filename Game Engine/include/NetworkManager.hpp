#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

// platform detection

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN64)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM != PLATFORM_WINDOWS

    #if PLATFORM == PLATFORM_WINDOWS
        #include <winsock2.h>
    #elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <fcntl.h>
        #include <errno.h>
    #endif

    #if PLATFORM == PLATFORM_WINDOWS // need to be tested on windows machine
        #pragma comment( lib, "wsock32.lib" )
        #endif

    #include "Module.hpp"
    #include "Logger.hpp"

    #define HEADER_SIZE 12
    #define PAYLOAD_SIZE 512

    namespace PrEngine
    {

        struct Packet   //524 bytes
        {
            uint32_t protocl_id;
            uint16_t sequence_number;
            uint16_t ack;
            uint32_t bit_filed;
            Char_8 data[PAYLOAD_SIZE];
        };


        class Address
        {
        public:

            sockaddr_in address;
            Address();
            
            Address( unsigned Char_8 a, 
                    unsigned Char_8 b, 
                    unsigned Char_8 c, 
                    unsigned Char_8 d, 
                    unsigned short port );

            // not defined yet
            unsigned Int_32 GetAddress() const;
            unsigned Char_8 GetA() const;
            unsigned Char_8 GetB() const;
            unsigned Char_8 GetC() const;
            unsigned Char_8 GetD() const;
            unsigned short GetPort() const;

        };

        class Socket
        {
            public:

                Socket();

                ~Socket();

                Bool_8 Open( unsigned short port );

                void Close();

                Bool_8 IsOpen() const;

                Bool_8 Send( const Address & destination, 
                        const Packet* packet, 
                        Int_32 size );

                Int_32 Receive( Address & sender, 
                            Packet* packet, 
                            Int_32 size );
                Int_32 handle;


            private:

        };



        class NetworkManager : public Module
        {
            public:
                NetworkManager(const std::string& name, Int_32 priority);
                ~NetworkManager();

                void start() override;
                void update() override;
                void end() override;

            private:
                const Int_32 port = 30001;
                Socket socket;
        };
    }

        #endif

    #endif