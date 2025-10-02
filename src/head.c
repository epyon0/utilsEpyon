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

char dBuff[1024] = {'\0'};
char filename[256] = {'\0'};
int linelimit = 10;
bool prevVerboseValue;

int processData(FILE *fp);

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
                printf("\nDisplay portion of the begining of a file\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]        Print this help message\n");
                printf("[-v || --verbose]     Turn on verbose output\n");
                printf("[-n || --lines] <INT> Print the first <INT> lines, Default: %d\n", linelimit);
                printf("<FILE>                File to process\n");
                printf("\nReturn Codes:\n");
                printf("  0 = Success\n");
                printf("  1 = File inaccessible\n");
                printf("  2 = No file or input stream given\n");
                printf("  3 = Invalid line limit given\n");

                printf("\n");
                return 0;
            }

            if (((strncmp(arg, "-n", sizeof("-n")) == 0) || (strncmp(arg, "--lines", sizeof("--lines")) == 0)) && (i+1 < argc)) {
                linelimit = atoi(argv[i+1]);
                i++;
                continue;
            }

            if ((strncmp(argv[i], "-v", sizeof("-v")) == 0) || (strncmp(argv[i], "--verbose", sizeof("--verbose")) == 0)) {
                continue;
            }
            if (access(arg, F_OK) == 0) {
                strncpy(filename, arg, sizeof(filename));
                snprintf(dBuff, sizeof(dBuff), "Filename \"%s\" is accessible\n", arg);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            } else {
                snprintf(dBuff, sizeof(dBuff), "Filename \"%s\" is not accessible\n", arg);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                return(1);
            }
        }
    }

    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN from source other than terminal", __FILE__, __LINE__, __FUNCTION__);
            int rc = processData(stdin);
            if (rc != 0) {
                return(rc);
            }
        } else {
            prevVerboseValue = getverbose();
            setverbose(true);
            verbose("No file or input stream given", __FILE__, __LINE__, __FUNCTION__);
            setverbose(prevVerboseValue);
            return(2);
        }
    } else {
        FILE *fp = fopen(filename, "r");
        int rc = processData(fp);
        fclose(fp);
        if (rc != 0) {
            return(rc);
        }
    }
}

int processData(FILE *fp) {
    if (linelimit < 1) {
        prevVerboseValue = getverbose();
        setverbose(true);
        snprintf(dBuff, sizeof(dBuff), "Invalid line limit of: %d", linelimit);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        setverbose(prevVerboseValue);
        return(3);
    }

    char ch;
    int currLine = 0;
    while (currLine < linelimit) {
        fread(&ch, 1, 1, fp);
        printf("%c", ch);
        if (ch == '\n') {
            currLine++;
        }
    }

    return 0;
}