#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

bool vverbose = false;
char dBuff[1000];

void parseIP(const char *ipString);

int main(const int argc, const char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if ((strncmp(argv[i], "-d", sizeof("-d")) == 0) || (strncmp(argv[i], "--debug", sizeof("--debug")) == 0)) {
                setverbose(true);
            }
            if ((strncmp(argv[i], "-v", sizeof("-v")) == 0) || (strncmp(argv[i], "--verbose", sizeof("--verbose")) == 0)) {
                vverbose = true;
            }
        }

        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];
            if ((strncmp(arg, "-h", sizeof("-h")) == 0) || (strncmp(arg, "--help", sizeof("--help")) == 0)) {
                printf("\nCalculate CIDR IP Address\n\n%s <A.B.C.D/XY>\n\n", argv[0]);
                printf("[-h || --help]     Print this help message\n");
                printf("[-d || --debug]    Dispaly debug output\n");
                printf("[-v || --verbose]  List IP address of each host\n");
                printf("<A.B.C.D/XY>       CIDR IP address\n");

                return 0;
            }

            int periodCnt = 0, slashCnt = 0;
            for (size_t j = 0; j < strlen(arg); j++) {

                if ((arg[j] > 45) && (arg[j] < 58)) {
                    if (arg[j] == '.') {
                        periodCnt++;
                    }
                    if (arg[j] == '/') {
                        slashCnt++;
                    }
                    if ((j == strlen(arg) - 1) && (periodCnt == 3) && (slashCnt == 1)) {
                        printf("\n");
                       parseIP(arg);
                    }
                } else {
                    break;
                }
            }
        }
    }
}

void parseIP(const char *ipString) {
    snprintf(dBuff, sizeof(dBuff), "Parsing IP string: %s", ipString);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
    
    uint32_t tmpIP = 0;
    uint32_t snm = 0;
    uint32_t startIP = 0;
    uint32_t stopIP = 0;
    int octet1 = 0, octet2 = 0, octet3 = 0, octet4 = 0, netbit = 0, cnt = 0, pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

    for (size_t i = 0; i < strlen(ipString); i++) {
        if (ipString[i] == '.') {
            cnt++;

            switch(cnt) {
                case 1:
                    pos1 = (int)i;
                    break;
                case 2:
                    pos2 = (int)i;
                    break;
                case 3:
                    pos3 = (int)i;
                    break;
            }
        }
        if (ipString[i] == '/') {
            pos4 = (int)i;
        }
    }

    snprintf(dBuff, sizeof(dBuff), "Octet 1 end position: %d", pos1);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    snprintf(dBuff, sizeof(dBuff), "Octet 2 end position: %d", pos2);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    snprintf(dBuff, sizeof(dBuff), "Octet 3 end position: %d", pos3);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    snprintf(dBuff, sizeof(dBuff), "Octet 4 end position: %d", pos4);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
    
    char tmpBuf[4] = {'\0'};
    strncpy(tmpBuf, ipString, pos1);
    octet1 = atoi(tmpBuf);
    snprintf(dBuff, sizeof(dBuff), "Octet 1: %d", octet1);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    for (size_t i = 0; i < strlen(tmpBuf); i++) {
        tmpBuf[i] = '\0';
    }
    strncpy(tmpBuf, ipString + pos1 + 1, pos2 - pos1);
    octet2 = atoi(tmpBuf);
    snprintf(dBuff, sizeof(dBuff), "Octet 2: %d", octet2);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    for (size_t i = 0; i < strlen(tmpBuf); i++) {
        tmpBuf[i] = '\0';
    }
    strncpy(tmpBuf, ipString + pos2 + 1, pos3 - pos2);
    octet3 = atoi(tmpBuf);
    snprintf(dBuff, sizeof(dBuff), "Octet 3: %d", octet3);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    for (size_t i = 0; i < strlen(tmpBuf); i++) {
        tmpBuf[i] = '\0';
    }
    strncpy(tmpBuf, ipString + pos3 + 1, pos4 - pos3);
    octet4 = atoi(tmpBuf);    
    snprintf(dBuff, sizeof(dBuff), "Octet 4: %d", octet4);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
    
    for (size_t i = 0; i < strlen(tmpBuf); i++) {
        tmpBuf[i] = '\0';
    }
    strncpy(tmpBuf, ipString + pos4 + 1, strlen(ipString) - pos4);
    netbit = atoi(tmpBuf);
    snprintf(dBuff, sizeof(dBuff), "Network Bit: %d", netbit);
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    bool prevVerboseValue = getverbose();
    setverbose(true);

    if ((octet1 < 0) || (octet1 > 255)) {
        snprintf(dBuff, sizeof(dBuff), "Octet 1 is out of range [%d]", octet1);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }
    if ((octet2 < 0) || (octet2 > 255)) {
        snprintf(dBuff, sizeof(dBuff), "Octet 2 is out of range [%d]", octet2);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }
    if ((octet3 < 0) || (octet3 > 255)) {
        snprintf(dBuff, sizeof(dBuff), "Octet 3 is out of range [%d]", octet3);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }
    if ((octet4 < 0) || (octet4 > 255)) {
        snprintf(dBuff, sizeof(dBuff), "Octet 4 is out of range [%d]", octet4);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }
    if ((netbit < 0) || (netbit > 32)) {
        snprintf(dBuff, sizeof(dBuff), "Network mask is out of range [%d]", netbit);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }

    setverbose(prevVerboseValue);

    tmpIP += (uint32_t)octet1 << 24;
    tmpIP += (uint32_t)octet2 << 16;
    tmpIP += (uint32_t)octet3 << 8;
    tmpIP += (uint32_t)octet4;

    snprintf(dBuff, sizeof(dBuff), "Given IP: %d.%d.%d.%d", (uint8_t)(tmpIP>>24), (uint8_t)(tmpIP>>16), (uint8_t)(tmpIP>>8), (uint8_t)(tmpIP));
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    for (int i = 0; i <= netbit; i++) {
        snm <<= 1;
        snm++;
    }
    for (int i = netbit; i < 32; i++) {
        snm <<= 1;
    }

    snprintf(dBuff, sizeof(dBuff), "Subnet mask: %d.%d.%d.%d", (uint8_t)(snm>>24), (uint8_t)(snm>>16), (uint8_t)(snm>>8), (uint8_t)(snm));
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    startIP = tmpIP & snm;

    snprintf(dBuff, sizeof(dBuff), "Network ID: %d.%d.%d.%d", (uint8_t)(startIP>>24), (uint8_t)(startIP>>16), (uint8_t)(startIP>>8), (uint8_t)(startIP));
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    stopIP = tmpIP | ~snm;

    snprintf(dBuff, sizeof(dBuff), "Broadcast: %d.%d.%d.%d", (uint8_t)(stopIP>>24), (uint8_t)(stopIP>>16), (uint8_t)(stopIP>>8), (uint8_t)(stopIP));
    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

    char class = '\0';

    if (((uint8_t)octet1 | 0x7F) == 0x7F) {
        class = 'A';
    }
    if (((uint8_t)octet1 | 0x77) == 0xBF) {
        class = 'B';
    }
    if (((uint8_t)octet1 | 0x1F) == 0xDF) {
        class = 'C';
    }
    if (((uint8_t)octet1 | 0x0F) == 0xEF) {
        class = 'D';
    }
    if (((uint8_t)octet1 | 0x0F) == 0xFF) {
        class = 'E';
    }

    if (vverbose) {
        char tmpBuf[31] = {'\0'}, byte1[9] = {'\0'}, byte2[9] = {'\0'}, byte3[9] = {'\0'}, byte4[9] = {'\0'};
        
        strncpy(byte1, uint8tob((uint8_t)(tmpIP>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)(tmpIP>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)(tmpIP>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)tmpIP), sizeof(byte4));
        
        snprintf(tmpBuf, sizeof(tmpBuf), "Address:   %d.%d.%d.%d", (uint8_t)(tmpIP>>24), (uint8_t)(tmpIP>>16), (uint8_t)(tmpIP>>8), (uint8_t)(tmpIP));
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)(snm>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)(snm>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)(snm>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)snm), sizeof(byte4));
        
        snprintf(tmpBuf, sizeof(tmpBuf), "Netmask:   %d.%d.%d.%d", (uint8_t)(snm>>24), (uint8_t)(snm>>16), (uint8_t)(snm>>8), (uint8_t)(snm));
        printf("%-31s%s.%s.%s.%s\n",tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)((~snm)>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)((~snm)>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)((~snm)>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)(~snm)), sizeof(byte4));

        snprintf(tmpBuf, sizeof(tmpBuf), "Wildcard:  %d.%d.%d.%d", (uint8_t)((~snm)>>24), (uint8_t)((~snm)>>16), (uint8_t)((~snm)>>8), (uint8_t)((~snm)));
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)(startIP>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)(startIP>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)(startIP>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)startIP), sizeof(byte4));

        snprintf(tmpBuf, sizeof(tmpBuf), "Network:   %d.%d.%d.%d/%d", (uint8_t)(startIP>>24), (uint8_t)(startIP>>16), (uint8_t)(startIP>>8), (uint8_t)(startIP), netbit);
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)(stopIP>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)(stopIP>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)(stopIP>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)stopIP), sizeof(byte4));

        snprintf(tmpBuf, sizeof(tmpBuf), "Broadcast: %d.%d.%d.%d", (uint8_t)(stopIP>>24), (uint8_t)(stopIP>>16), (uint8_t)(stopIP>>8), (uint8_t)(stopIP));
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)((startIP+1)>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)((startIP+1)>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)((startIP+1)>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)(startIP+1)), sizeof(byte4));

        snprintf(tmpBuf, sizeof(tmpBuf), "Host Min:  %d.%d.%d.%d", (uint8_t)((startIP + 1)>>24), (uint8_t)((startIP + 1)>>16), (uint8_t)((startIP + 1)>>8), (uint8_t)((startIP + 1)));
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        for (size_t i = 0; i < strlen(tmpBuf); i++) {
            tmpBuf[i] = '\0';
            if ((i < strlen(byte1)) && (i < strlen(byte2)) && (i < strlen(byte3) && (i < strlen(byte4)))) {
                byte1[i] = '\0';
                byte2[i] = '\0';
                byte3[i] = '\0';
                byte4[i] = '\0';
            }
        }
        strncpy(byte1, uint8tob((uint8_t)((stopIP-1)>>24)), sizeof(byte1));
        strncpy(byte2, uint8tob((uint8_t)((stopIP-1)>>16)), sizeof(byte2));
        strncpy(byte3, uint8tob((uint8_t)((stopIP-1)>>8)), sizeof(byte3));
        strncpy(byte4, uint8tob((uint8_t)(stopIP-1)), sizeof(byte4));

        snprintf(tmpBuf, sizeof(tmpBuf), "Host Max:  %d.%d.%d.%d", (uint8_t)((stopIP - 1)>>24), (uint8_t)((stopIP - 1)>>16), (uint8_t)((stopIP - 1)>>8), (uint8_t)((stopIP - 1)));
        printf("%-31s%s.%s.%s.%s\n", tmpBuf, byte1, byte2, byte3, byte4);

        printf("Hosts/Net: %d\n", stopIP - startIP - 1);
        printf("Class:     %c\n", class);
    } else {
        printf("Address:   %d.%d.%d.%d\n",    (uint8_t)(tmpIP>>24),         (uint8_t)(tmpIP>>16),         (uint8_t)(tmpIP>>8),         (uint8_t)(tmpIP));
        printf("Netmask:   %d.%d.%d.%d\n",    (uint8_t)(snm>>24),           (uint8_t)(snm>>16),           (uint8_t)(snm>>8),           (uint8_t)(snm));
        printf("Wildcard:  %d.%d.%d.%d\n",    (uint8_t)((~snm)>>24),        (uint8_t)((~snm)>>16),        (uint8_t)((~snm)>>8),        (uint8_t)((~snm)));
        printf("Network:   %d.%d.%d.%d/%d\n", (uint8_t)(startIP>>24),       (uint8_t)(startIP>>16),       (uint8_t)(startIP>>8),       (uint8_t)(startIP), netbit);
        printf("Broadcast: %d.%d.%d.%d\n",    (uint8_t)(stopIP>>24),        (uint8_t)(stopIP>>16),        (uint8_t)(stopIP>>8),        (uint8_t)(stopIP));
        printf("Host Min:  %d.%d.%d.%d\n",    (uint8_t)((startIP + 1)>>24), (uint8_t)((startIP + 1)>>16), (uint8_t)((startIP + 1)>>8), (uint8_t)((startIP + 1)));
        printf("Host Max:  %d.%d.%d.%d\n",    (uint8_t)((stopIP - 1)>>24),  (uint8_t)((stopIP - 1)>>16),  (uint8_t)((stopIP - 1)>>8),  (uint8_t)((stopIP - 1)));
        printf("Hosts/Net: %d\n", stopIP - startIP - 1);
        printf("Class:     %c\n", class);
    }

    if (vverbose) {
        printf("Hosts:\n");
        for (register uint32_t i = startIP + 1; i < stopIP; i++) {
            printf("  %d.%d.%d.%d\n", (uint8_t)(i>>24), (uint8_t)(i>>16), (uint8_t)(i>>8), (uint8_t)(i));
        }
    }

    return ;
}