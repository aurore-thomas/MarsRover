#pragma once
#include <string>

#include "communication.hpp"

#ifdef _WIN32
    #include <winsock2.h>
#elif __unix__
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    #define SOCKET_ERROR -1
#else
    #error "Unknown"
#endif

using namespace std;

class MissionControl : public Communication
{
  private:
    // string AskCommand() const;
    // string DecodeResponse(const Packet& packet) const override;
    // Packet EncodeRequest() const override;

    
    public:
    MissionControl();
    // ~MissionControl();
    
    int LaunchServer() const; // ATTENTION, bon endroit ???
    void CloseServer(SOCKET serverSocket) const;
};


