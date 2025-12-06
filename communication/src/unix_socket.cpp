#include "unix_socket.hpp"

#ifndef _UNIX_
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

UnixSocket::UnixSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
}

UnixSocket::~UnixSocket() {
    close(sock);
}

bool UnixSocket::Send(PacketSerializer& packet) {
    std::vector<uint8_t> buffer = packet.SerializePacket();

    uint32_t packetSize = static_cast<uint32_t>(buffer.size());
    int sent = send(sock, reinterpret_cast<const char*>(&packetSize), sizeof(packetSize), 0);
    if (sent != sizeof(packetSize))
        return false;

    int totalSent = 0;
    const char* data = reinterpret_cast<const char*>(buffer.data());

    while (totalSent < static_cast<int>(packetSize)) {
        int bytes = send(sock, data + totalSent, packetSize - totalSent, 0);
        if (bytes <= 0)
            return false;
        totalSent += bytes;
    }

    return true;
}

bool UnixSocket::Receive(PacketSerializer& packet) {
    uint32_t packetSize = 0;
    int received = recv(sock, reinterpret_cast<char*>(&packetSize), sizeof(packetSize), 0);
    if (received <= 0 || received != sizeof(packetSize))
        return false;

    std::vector<uint8_t> buffer(packetSize);
    int totalReceived = 0;

    while (totalReceived < static_cast<int>(packetSize)) {
        int bytes = recv(sock,
                        reinterpret_cast<char*>(buffer.data()) + totalReceived,
                        packetSize - totalReceived, 0);
        if (bytes <= 0)
            return false;
        totalReceived += bytes;
    }

    try {
        packet = packet.DeserializePacket(buffer);
    } catch (const std::string& e) {
        return false;
    }

    return true;
}


void UnixSocket::setSock(int socket) {
    sock = socket;
}
int UnixSocket::getSock() const {
    return sock;
}

#endif
