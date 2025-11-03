#ifdef _WIN32
#include "common/win_socket.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

bool WinSocket::Init() {
    WSADATA wsaData;
    return (WSAStartup(MAKEWORD(2,2), &wsaData) == 0);
}

void WinSocket::Cleanup() { WSACleanup(); }

bool WinSocket::Create() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return (sock != INVALID_SOCKET);
}

bool WinSocket::Bind(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return bind(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool WinSocket::Listen(int backlog) {
    return listen(sock, backlog) == 0;
}

int WinSocket::Accept() {
    return (int)accept(sock, nullptr, nullptr);
}

bool WinSocket::Connect(const std::string& host, unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
    return connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool WinSocket::Send(Packet& packet) {
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

bool WinSocket::Receive(Packet& packet) {
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

#endif
