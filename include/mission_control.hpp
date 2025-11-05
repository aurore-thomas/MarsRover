#pragma once
#include <string>

#include "common/unix_socket.hpp"

using namespace std;

class MissionControl
{
  private:
    unsigned short port;
    UnixSocket server;
    bool LaunchServer();

  public:
    MissionControl(unsigned short port, UnixSocket& serverSocket);

    void setPort(unsigned short port);
    unsigned short getPort() const;
    void setServerSocket(UnixSocket& serverSocket);
    UnixSocket& getServerSocket();
};


