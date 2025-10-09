#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char dBuff[1024] = {'\0'};
bool verboseValue, follow = false;
char filename[256] = {'\0'};

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
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION);

            if ((strncmp(arg, "-h", sizeof("-h")) == 0) || (strncmp(arg, "--help", sizeof("--help")) == 0)) {
                printf("\nDisplay portion of the ending of a file\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]   Print this help message\n");
                printf("\nReturn Values:\n");
                printf("  0 = Success\n");
                printf("  1 = File not accessible\n");

                printf("\n");
                return 0;
            }




            if ((strncmp(arg, "-v", sizeof("-v")) == 0) || (strncmp(arg, "--verbose", sizeof("--verbose")) == 0)) {
                continue;
            }
            if (access(arg, F_OK) == 0) {
                strncpy(filename, arg, sizeof(filename));
                snprintf(dBuff, sizeof(dBuff), "Filename \"%s\" is accessible", arg);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            } else {
                verboseValue = getverbose();
                setverbose(true);
                snprintf(dBuff, sizeof(dBuff), "Filename \"%s\" is not accessible", arg);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                setverbose(verboseValue);
                return(1);
            }
        }
    }

}