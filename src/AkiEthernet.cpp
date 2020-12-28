#include "AkiEthernet.h"
#include <AkiConfigurator.h>

AkiEthernet AkiNet=AkiEthernet();

uint8_t AkiEthernet::begin(bool Wipe)
{
    if (Wipe == true)
    {
        WipeCFG();
    }
    else
    {
        if(AkiEthernetCfg.used==true)
        {
            Serial.println("Begining AkiEthernet");

            if (AkiEthernetCfg.isStaticIPUsed == true)
            {
                Ethernet.begin(AkiEthernetCfg.DeviceMAC, AkiEthernetCfg.DeviceIP, AkiEthernetCfg.DNSIP, AkiEthernetCfg.GatewayIP, AkiEthernetCfg.SubnetIP);
            }
            else
            {
                Ethernet.begin(AkiEthernetCfg.DeviceMAC);
            }
            PrintCFG();
        }
    }
}

void AkiEthernet::loop()
{
    if (AkiEthernetCfg.used == true)
    {
        if (AkiEthernetCfg.isStaticIPUsed == true)
        {
            static unsigned long int i;
            if (millis() - i == 50000)
            {
                Ethernet.maintain();
            }
        }
    }
}

void AkiEthernet::UpdateIP(){
    AkiEthernetCfg.DeviceIP=Ethernet.localIP();
    AkiEthernetCfg.DNSIP=Ethernet.dnsServerIP();
    AkiEthernetCfg.GatewayIP=Ethernet.gatewayIP();
    AkiEthernetCfg.SubnetIP=Ethernet.subnetMask();
}

AkiEthernet::NetStatus AkiEthernet::status()
{
    UpdateIP();
    if(Ethernet.hardwareStatus()==EthernetNoHardware)
    {
        return(AkiEthernet::NoHardware);
    }else if (Ethernet.linkStatus()==EthernetLinkStatus::LinkOFF)
    {
        return(AkiEthernet::LinkOff);
    }else
    {
        return(AkiEthernet::OK);
    }
}

void AkiEthernet::PrintCFG(){
    UpdateIP();
    Serial.print("Status:");
    Serial.println(status());
    Serial.print("---MAC Adress");
    for (uint8_t i = 0; i < 6; i++)
    {
        Serial.print(':');
        Serial.print(AkiEthernetCfg.DeviceMAC[i], HEX);
    }
    Serial.println();
    Serial.print("---IP:");
    if (AkiEthernetCfg.isStaticIPUsed == 0)
    {
        Serial.println("DHCP");
    }
    else
    {
        Serial.println("Static");
    }
    Serial.print("------Device IP:");
    Serial.println(AkiEthernetCfg.DeviceIP);
    Serial.print("------Gateway IP:");
    Serial.println(AkiEthernetCfg.GatewayIP);
    Serial.print("------DNS Server IP:");
    Serial.println(AkiEthernetCfg.DNSIP);
    Serial.print("------Subnet IP:");
    Serial.println(AkiEthernetCfg.SubnetIP);
}

void AkiEthernet::WipeCFG(){
    Serial.println("Ethernet configuration start");
    char str[19];
    for (uint8_t att = 0, slen = 0;; att++)
    {
        char buf[5] = "0x"; //for mac
        //set input string length
        if (att == 0)
        {
            slen = 1;
            Serial.println("Enter 1 if Ethernet is used, else 0:");
        }
        else if (att == 1)
        { //MAC
            Serial.println("Enter Ethernet MAC adress");
            slen = 17;
        }
        else if (att == 2)
        { // Is IP Adress static
            slen = 1;
            Serial.println("Enter 1 if DHCP not used, 0 if used");
        }
        else if (att == 3 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            slen = 15;
            Serial.println("Enter device IP Adress");
        }
        else if (att == 4 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            slen = 15;
            Serial.println("Enter DNS Server Adress");
        }
        else if (att == 5 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            slen = 15;
            Serial.println("Enter Gateway IP Adress");
        }
        else if (att == 6 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            slen = 15;
            Serial.println("Enter Subnet IP Adress");
        }
        else
        {
            break;
        }
        strcpy(str,"");
        for (uint8_t i = 0; i < slen;)
        {
            if (Serial.available() > 0)
            {
                str[i] = Serial.read();
                i++;
            }
        }
        if (att == 0)
        {
            if(str[0] == '0')
            {
                AkiEthernetCfg.used=false;
                break;
            }else
            {
                AkiEthernetCfg.used=true;
                AkiCfg.addCfg((void*)&AkiEthernetCfg, &AkiEthernetCfg.isUpdated, (char*)"Ethernet Configuration", sizeof(AkiEthernetCfg));
            }
            
        }
        if (att == 1)
        {
            str[17] = ':';
            for (uint8_t i = 0, j = 0, k = 0; i <= slen; i++)
            {
                if (str[i] != ':')
                {
                    buf[j + 2] = str[i];
                    if (j == 1)
                    {
                        j = 0;
                    }
                    else
                    {
                        j++;
                    }
                }
                else
                {
                    AkiEthernetCfg.DeviceMAC[k] = strtol(buf, NULL, 0);
                    k++;
                }
            }
            Serial.print("MAC Adress");
            for (uint8_t i = 0; i < 6; i++)
            {
                Serial.print(':');
                Serial.print(AkiEthernetCfg.DeviceMAC[i], HEX);
            }
            Serial.println();
        }
        else if (att == 2)
        {
            strcpy(buf, "");
            buf[0] = str[0];
            AkiEthernetCfg.isStaticIPUsed = atoi(buf);
            Serial.print("Static IP:");
            Serial.println(AkiEthernetCfg.isStaticIPUsed);
        }
        else if (att == 3 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            str[15]='\0';
            AkiEthernetCfg.DeviceIP.fromString(str);
            Serial.print("IP Adress:");
            Serial.println(AkiEthernetCfg.DeviceIP);
        }
        else if (att == 4 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            str[15]='\0';
            AkiEthernetCfg.DNSIP.fromString(str);
            Serial.print("DNS IP Adress:");
            Serial.println(AkiEthernetCfg.DNSIP);
        }
        else if (att == 5 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            str[15]='\0';
            AkiEthernetCfg.GatewayIP.fromString(str);
            Serial.print("Gateway Adress:");
            Serial.println(AkiEthernetCfg.GatewayIP);
        }
        else if (att == 6 and AkiEthernetCfg.isStaticIPUsed == 1)
        {
            str[15]='\0';
            AkiEthernetCfg.SubnetIP.fromString(str);
            Serial.print("Subnet Mask:");
            Serial.println(AkiEthernetCfg.SubnetIP);
        }
    }
}