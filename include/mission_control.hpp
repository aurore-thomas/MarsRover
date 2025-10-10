#pragma once
#include <string>

#include "communication.hpp"

using namespace std;

class MissionControl : public Communication
{
  private:
    string AskCommand() const;
    // string DecodeResponse(const Packet& packet) const override;
    // Packet EncodeRequest() const override;


  public:
    MissionControl();

};


