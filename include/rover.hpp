#pragma once

#include <string>

class Rover : public Communication{
  private:
    string orientation;
    Vector<int> position;
        
    string getOrientation() const;
    void setOrientation(const string& newOrientation);
    
    Coordinates getPosition() const;
    void setPosition(const Coordinates& newPosition);
    
  public:
    Rover(int x, int y, const string& orientation);
    Response ExecuteCommand(const string& command);

    //string DecodeResponse(const Packet& packet) const override;
    // Packet EncodeRequest() const override;

};

