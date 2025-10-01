#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

char dBuff[1024] = {'\0'};
bool encode = true;
int wrapLimit = 76;

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
            snprintf(dBuff, sizeof(dBuff), "Processing argument: %s", arg);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

            if ((strncmp(arg, "-h", sizeof("-h")) == 0) || (strncmp(arg, "--help", sizeof("--help")) == 0)) {
                printf("\nEncode or decode base64\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]        Print this help message\n");
                printf("[-v || --verbose]     Turn on verbose output\n");
                printf("[-d || --decode]      Decode data instead of encoding\n");
                printf("[-w || --wrap] <INT>  Wrap encoded lines after <INT> character, Default: %d\n", wrapLimit);
                printf("<FILE>                File to encode/decode\n");

                printf("\n");
                return 0;
            }

            if ((strncmp(arg, "-d", sizeof("-d")) == 0) || (strncmp(arg, "--decode", sizeof("--decode")) == 0)) {
                encode = false;
                continue;
            }

            if (((strncmp(arg, "-w", sizeof("-w")) == 0) || (strncmp(arg, "--wrap", sizeof("--wrap")) == 0)) && i+1 < argc) {
                wrapLimit = atoi(argv[i+1]);
                continue;
            }

            if ((strncmp(arg, "-v", sizeof("-v")) == 0) || (strncmp(arg, "--verbose", sizeof("--verbose")) == 0)) {
                continue;
            }
        }
    }
}