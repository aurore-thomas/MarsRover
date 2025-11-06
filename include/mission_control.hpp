#pragma once
#include <string>

#include "unix_socket.hpp"

using namespace std;

class MissionControl
{
  private:
    unsigned short port;
    UnixSocket server;
    bool LaunchServer();
    bool Bind(unsigned short port);
    int Accept();

  public:
    MissionControl(unsigned short port, UnixSocket& serverSocket);

    void setPort(unsigned short port);
    unsigned short getPort() const;
    void setServerSocket(UnixSocket& serverSocket);
    UnixSocket& getServerSocket();
};


