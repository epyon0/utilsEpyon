#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

char dBuff[1024] = {'\0'};

int main(const int argc, const char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if ((strncmp(argv[i], "-v", sizeof("-v")) == 0) || (strncmp(argv[i], "--verbose", sizeof("--verbose")) == 0)) {
                setverbose(true);
                break;
            }
        }
        if (isatty(0)) {
            verbose("STDIN is from terminal", __FILE__, __LINE__, __FUNCTION__);
        } else {
            verbose("STDIN is NOT from terminal", __FILE__, __LINE__, __FUNCTION__);
        }
        if (isatty(1)) {
            verbose("STDOUT is to terminal", __FILE__, __LINE__, __FUNCTION__);
        } else {
            verbose("STDOUT is NOT to terminal", __FILE__, __LINE__, __FUNCTION__);
        }
        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];
            snprintf(dBuff, sizeof(dBuff), "Processing argument: %s", arg);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

            if ((strncmp(arg, "-h", sizeof("-h")) == 0) || (strncmp(arg, "--help", sizeof("--help")) == 0)) {
                printf("\nDisplay portion of the begining of a file\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]    Print this help message\n");
                printf("[-v || --verbose] Turn on verbose output\n");
                printf("<FILE>            File to process\n");

                return 0;
            }
        }
    }
}