#include <Arduino.h>
#include <Ethernet.h>
#pragma once

struct AkiEthernetConf{
    bool used;
    byte DeviceMAC[6];
    byte DeviceIP[4];
    byte DNSIP[4];
    byte SubnetIP[4];
    byte GatewayIP[4];
} AkiEthernetCfg;

class AkiEthernet{
    private:
        
    public:
        enum{OK, UnknowError, LinkOff, NoHardware, NoIP} status;
        bool begin();
        void loop();
};
extern AkiEthernet AkiNet;
