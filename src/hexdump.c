#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#define STDIN       0x20
#define ASCII       0x10
#define HEXADECIMAL 0x08
#define DECIMAL     0x04
#define OCTAL       0x02
#define BINARY      0x01

char dBuff[1024*2];
uint8_t outputFlag = HEXADECIMAL;
char ascii[255][4];
unsigned int len = 8;
char file[1024] = {'\0'};

void dumpFile(char ch, uint64_t pos);

int main(const int argc, const char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if ((strncmp(argv[i], "-v", sizeof("-v")) == 0) || (strncmp(argv[i], "--verbose", sizeof("--verbose")) == 0)) {
                setverbose(true);
                break;
            }
        }
        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];

            if ((strncmp(arg, "-h", sizeof("-h")) == 0) || (strncmp(arg, "--help", sizeof("--help")) == 0)) {
                printf("\nDump file contents\n\n%s <FILE>\nOR\ncat <FILE> | %s -i\n\n", argv[0], argv[0]);
                printf("[-h || --help]        Print this help message\n");
                printf("[-v || --verbose]     Turn on verbose output\n");
                printf("[-b || --bin]         Toggle binary output, default: %s\n", ((BINARY & outputFlag) == BINARY) ? "true" : "false");
                printf("[-x || --hex]         Toggle hexadecimal output, default: %s\n", ((HEXADECIMAL & outputFlag) == HEXADECIMAL) ? "true" : "false");
                printf("[-o || --oct]         Toggle octal output, default: %s\n", ((OCTAL & outputFlag) == OCTAL) ? "true" : "false");
                printf("[-d || --dec]         Toggle decimal output, default: %s\n", ((DECIMAL & outputFlag) == DECIMAL) ? "true" : "false");
                printf("[-a || --ascii]       Toggle ASCII output, default: %s\n", ((ASCII & outputFlag) == ASCII) ? "true" : "false");
                printf("[-i || --stdin]       Read file from STDIN instead of parameter\n");
                printf("[-l || --len] <INT>   Number of bytes displayed before wrapping to next line, default: %u\n", len);
                printf("[-f || --file] <FILE> Path to file\n");

                return 0;
            }

            if ((strncmp(arg, "-b", sizeof("-b")) == 0) || (strncmp(arg, "--bin", sizeof("--bin")) == 0)) {
                verbose("Setting BINARY Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= BINARY;
                snprintf(dBuff, sizeof(dBuff), "BINARY Flag: %s", ((BINARY & outputFlag) == BINARY) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }
            if ((strncmp(arg, "-o", sizeof("-o")) == 0) || (strncmp(arg, "--oct", sizeof("--oct")) == 0)) {
                verbose("Setting OCTAL Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= OCTAL;
                snprintf(dBuff, sizeof(dBuff), "OCTAL Flag: %s", ((OCTAL & outputFlag) == OCTAL) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }
            if ((strncmp(arg, "-d", sizeof("-d")) == 0) || (strncmp(arg, "--dec", sizeof("--dec")) == 0)) {
                verbose("Setting DECIMAL Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= DECIMAL;
                snprintf(dBuff, sizeof(dBuff), "DECIMAL Flag: %s", ((DECIMAL & outputFlag) == DECIMAL) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }
            if ((strncmp(arg, "-x", sizeof("-x")) == 0) || (strncmp(arg, "--hex", sizeof("--hex")) == 0)) {
                verbose("Setting HEXADECIMAL Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= HEXADECIMAL;
                snprintf(dBuff, sizeof(dBuff), "HEXADECIMAL Flag: %s", ((HEXADECIMAL & outputFlag) == HEXADECIMAL) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }
            if ((strncmp(arg, "-a", sizeof("-a")) == 0) || (strncmp(arg, "--ascii", sizeof("--ascii")) == 0)) {
                verbose("Setting ASCII Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= ASCII;
                snprintf(dBuff, sizeof(dBuff), "ASCII Flag: %s", ((ASCII & outputFlag) == ASCII) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }

            if ((strncmp(arg, "-i", sizeof("-i")) == 0) || (strncmp(arg, "--stdin", sizeof("--stdin")) == 0)) {
                verbose("Setting STDIN Flag", __FILE__, __LINE__, __FUNCTION__);
                outputFlag ^= STDIN;
                snprintf(dBuff, sizeof(dBuff), "STDIN Flag: %s", ((STDIN & outputFlag) == STDIN) ? "true" : "false");
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                continue;
            }

            if (((strncmp(arg, "-l", sizeof("-l")) == 0) || (strncmp(arg, "--len", sizeof("--len")) == 0)) && (i+1 < argc)) {
                len = atoi(argv[i+1]);
                snprintf(dBuff, sizeof(dBuff), "Setting LENGTH: %d", len);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                i++;
                continue;
            }

            if (((strncmp(arg, "-f", sizeof("-f")) == 0) || (strncmp(arg, "--file", sizeof("--file")) == 0)) && (i+1 < argc)) {
                strncpy(file, argv[i+1], sizeof(file));
                i++;
                continue;
            }
        }
    }

    if ((ASCII & outputFlag) == ASCII) {
            for (int i = 33; i < 127; i++) {
        char tmpStr[4];
        snprintf(tmpStr, sizeof(tmpStr), " %c ", i);
        strncpy(ascii[i], tmpStr, sizeof(ascii[i]));
    }
    for (int i = 128; i <= 255; i++) {
        char tmpStr[4];
        snprintf(tmpStr, sizeof(tmpStr), " %c ", i);
        strncpy(ascii[i], tmpStr, sizeof(ascii[1]));
    }
    strncpy(ascii[0],   "NUL", 4);
    strncpy(ascii[1],   "SOH", 4);
    strncpy(ascii[2],   "STX", 4);
    strncpy(ascii[3],   "ETX", 4);
    strncpy(ascii[4],   "EOT", 4);
    strncpy(ascii[5],   "ENQ", 4);
    strncpy(ascii[6],   "ACK", 4);
    strncpy(ascii[7],   "BEL", 4);
    strncpy(ascii[8],   " BS", 4);
    strncpy(ascii[9],   "TAB", 4);
    strncpy(ascii[10],  " LF", 4);
    strncpy(ascii[11],  " VT", 4);
    strncpy(ascii[12],  " FF", 4);
    strncpy(ascii[13],  " CR", 4);
    strncpy(ascii[14],  " SO", 4);
    strncpy(ascii[15],  " SI", 4);
    strncpy(ascii[16],  "DLE", 4);
    strncpy(ascii[17],  "DC1", 4);
    strncpy(ascii[18],  "DC2", 4);
    strncpy(ascii[19],  "DC3", 4);
    strncpy(ascii[20],  "DC4", 4);
    strncpy(ascii[21],  "NAK", 4);
    strncpy(ascii[22],  "SYN", 4);
    strncpy(ascii[23],  "ETB", 4);
    strncpy(ascii[24],  "CAN", 4);
    strncpy(ascii[25],  " EM", 4);
    strncpy(ascii[26],  "SUB", 4);
    strncpy(ascii[27],  "ESC", 4);
    strncpy(ascii[28],  " FS", 4);
    strncpy(ascii[29],  " GS", 4);
    strncpy(ascii[30],  " RS", 4);
    strncpy(ascii[31],  " US", 4);
    strncpy(ascii[32],  "SPC", 4);
    strncpy(ascii[127], "DEL", 4);
    }

    char ch;
    if ((STDIN & outputFlag) == STDIN) {
        uint64_t count = 0;
        verbose("Reading from STDIN", __FILE__, __LINE__, __FUNCTION__);
        while(read(STDIN_FILENO, &ch, 1) > 0) {
            dumpFile(ch, count);
            count++;
        }
    } else {
        if (file[0] != '\0') {
            snprintf(dBuff, sizeof(dBuff), "Reading from FILE: %s", file);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

            if (access(file, F_OK) == 0) {
                FILE *fptr;
                long flen;

                fptr = fopen(file, "rb");
                fseek(fptr, 0, SEEK_END);
                flen = ftell(fptr);
                rewind(fptr);

                for (int i = 0; i < flen; i++) {
                    fread(&ch, 1, 1, fptr);
                    dumpFile(ch, i);
                }

                fclose(fptr);

            } else {
                snprintf(dBuff, sizeof(dBuff), "Error reading from FILE: %s", file);
                bool prevVerboseValue = getverbose();
                setverbose(true);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                setverbose(prevVerboseValue);
            }
        }
    }

    printf("\n");
    return 0;
}

void dumpFile(char ch, uint64_t pos) {
    if (pos == 0) {
        printf("0x%016llX: ", (unsigned long long)pos);
    }
    if ((pos % len == 0) && (pos != 0)) {
        printf("\n");
        printf("0x%016llX: ", (unsigned long long)pos);
    }
    if ((HEXADECIMAL & outputFlag) == HEXADECIMAL) {
        printf("[0x%02X]", (unsigned char)ch);
    }
    if ((OCTAL & outputFlag) == OCTAL) {
        printf("[0o%03o]", (unsigned char)ch);
    }
    if ((DECIMAL & outputFlag) == DECIMAL) {
        printf("[%03u]", (unsigned char)ch);
    }
    if ((BINARY & outputFlag) == BINARY) {
        printf("[0b%s]", uint8tob((unsigned char)ch));
    }
    if ((ASCII & outputFlag) == ASCII) {
        printf("[%s]", ascii[(unsigned char)ch]);
    }

    printf("  ");
}