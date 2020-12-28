#include <Arduino.h>
#include <Ethernet.h>
#pragma once

struct
{
    bool isUpdated;
    bool used = false;
    bool isStaticIPUsed;
    byte DeviceMAC[6];
    IPAddress DeviceIP;
    IPAddress DNSIP;
    IPAddress SubnetIP;
    IPAddress GatewayIP;
} AkiEthernetCfg;

class AkiEthernet
{
private:
    void WipeCFG();
    void PrintCFG();
    void UpdateIP();
    enum NetStatus
    {
        OK,
        LinkOff,
        NoHardware,
        NoIP
    };
public:
    NetStatus status();
    uint8_t begin(bool Wipe);
    void loop();
};
extern AkiEthernet AkiNet;
