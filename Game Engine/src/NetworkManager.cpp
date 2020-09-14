#if PLATFORM != PLATFORM_WINDOWS
#include "NetworkManager.hpp"

namespace PrEngine
{
    bool InitializeSockets()
    {
        #if PLATFORM == PLATFORM_WINDOWS
        WSADATA WsaData;
        return WSAStartup( MAKEWORD(2,2), 
                            &WsaData ) 
            == NO_ERROR;
        #else
            return true;
        #endif
    }

    void ShutdownSockets()
    {
        #if PLATFORM == PLATFORM_WINDOWS
        WSACleanup();
        #endif
    }

    Socket::Socket()
    {
        InitializeSockets();

        // create a socket
        handle = socket( AF_INET, 
                         SOCK_DGRAM, 
                         IPPROTO_UDP );

        if ( handle <= 0 )
        {
            LOG( LOGTYPE_ERROR,  "failed to create socket" );
        }
        else
            LOG(LOGTYPE_GENERAL, "Socket handle: ", std::to_string(handle));

    }


    Socket::~Socket()
    {
        ShutdownSockets();

        #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        close( handle );
        #elif PLATFORM == PLATFORM_WINDOWS
        closesocket( handle );
        #endif
    }

    bool Socket::Open(unsigned short port)
    {
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( (unsigned short) port );


        if ( bind( handle, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
        {
            
            LOG( LOGTYPE_ERROR, "failed to bind socket : ", std::to_string(errno), std::string(" | "),std::string(strerror(errno)));
            return false;
        }

        #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

        int nonBlocking = 1;
        if ( fcntl( handle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
        {
            printf( "failed to set non-blocking\n" );
            return false;
        }

        #elif PLATFORM == PLATFORM_WINDOWS

            DWORD nonBlocking = 1;
            if ( ioctlsocket( handle, 
                            FIONBIO, 
                            &nonBlocking ) != 0 )
            {
                printf( "failed to set non-blocking\n" );
                return false;
            }

        #endif

        return true;
    }


    bool Socket::Send(const Address& destination, const Packet* packet, int size)
    {
        int sent_bytes = sendto( handle, packet, size, 0, 
                                (sockaddr*)&(destination.address), sizeof(sockaddr_in) );

        if ( sent_bytes != size )
        {
            LOG( LOGTYPE_ERROR, "failed to send packet" );
            return false;
        }
        else
        {
            LOG( LOGTYPE_GENERAL, "packet sent");
        }
        
    }



    int Socket::Receive(Address &sender, Packet* packet, int size)
    {
        return -1;
    }

    int Socket::Receive(Address & sender, Packet * packet, int size)
    {
        #if PLATFORM == PLATFORM_WINDOWS
        typedef int socklen_t;
        #endif

        sockaddr_in from;
        socklen_t fromLength = sizeof( from );

        int bytes = recvfrom( handle, 
                            packet, 
                            size,
                            0, 
                            (sockaddr*)&from, 
                            &fromLength );

        if ( bytes <= 0 )
            return 0;

        unsigned int from_address = 
            ntohl( from.sin_addr.s_addr );

        unsigned int from_port = 
            ntohs( from.sin_port );
        
        return bytes;
    }



    //////////////??ADDRESS/////////

    Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
    {
         unsigned int _address = ( a << 24 ) | 
                                ( b << 16 ) | 
                                ( c << 8  ) | 
                                d;

        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl( _address );
        address.sin_port = htons( port );
    }

    Address::Address()
    {
        
    }

    NetworkManager::NetworkManager(const std::string& name, int priority) :Module(name,priority)
    {
    }

    NetworkManager::~NetworkManager()
    {
        
    }

    void NetworkManager::start()
    {
        if ( !socket.Open( port ) )
        {
            printf( "failed to create socket!\n" );
            return;
        }
        // send a packet
        Packet packet;
        packet.protocl_id = 1100;
        packet.sequence_number = 1;
        packet.ack = 0;
        packet.bit_filed = 0x00;
        packet.data[0] = 'H';
        packet.data[1] = 'E';
        packet.data[2] = 'L';
        packet.data[3] = 'L';
        packet.data[4] = 'O';
        packet.data[5] = '\0';
        std::cout<<"sending: "<<sizeof( packet )<<" bytes"<<std::endl;
        socket.Send( Address(127,0,0,1,port), &packet, sizeof( packet ) );
    }


    void NetworkManager::update()
    {
        
        Address sender;
        Packet packet;
        int bytes_read = socket.Receive( sender, 
                            &packet, 
                            sizeof( packet ) );
        //if ( !bytes_read )
        if( bytes_read > 96)
        {
            LOG(LOGTYPE_GENERAL, "Data received! ",std::to_string(bytes_read));
            std::cout<<packet.data<<std::endl;
            std::cout<<std::endl;
        }
            
        // process packet
    }

    void NetworkManager::end()
    {

    }


}
#endif