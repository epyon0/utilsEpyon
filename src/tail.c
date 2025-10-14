#include <stdio.h>
#include <stdbool.h>
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
bool verboseValue, follow = false;
char filename[256] = {'\0'};
int lineLimit = 10;
int maxLine = 1024 * 200;

int tail(FILE *fp);

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
                printf("\nDisplay portion of the ending of a file\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]         Print this help message\n");
                printf("[-n || --lines] <INT>  Print the last <INT> lines, Default: %d\n", lineLimit);
                printf("[-f || --folow]        Append data as the file grows\n");
                printf("\nReturn Values:\n");
                printf("  0 = Success\n");
                printf("  1 = File not accessible\n");

                printf("\n");
                return 0;
            }

            if ((strncmp(arg, "-f", sizeof("-f")) == 0) || (strncmp(arg, "--follow", sizeof("--follow")) == 0)) {
                follow = true;
                continue;
            }

            if (((strncmp(arg, "-n", sizeof("-n")) == 0) || (strncmp(arg, "--lines", sizeof("--lines")) == 0)) && (i + 1 < argc)) {
                lineLimit = atoi(argv[i+1]);
                i++;
                continue;
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

    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN", __FILE__, __LINE__, __FUNCTION__);
            strncpy(filename, "STDIN", strlen("STDIN") + 1);
            int rc = tail(stdin);
            exit(rc);
        } else {
            verboseValue = getverbose();
            setverbose(true);
            verbose("No file or input stream given", __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(2);
        }
    } else {
        snprintf(dBuff, sizeof(dBuff), "Opening file \"%s\"", filename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

        FILE *fp = fopen(filename, "r");

        if (fp == NULL) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed opening file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(3);
        }

        int rc = tail(fp);

        snprintf(dBuff, sizeof(dBuff), "Closing file \"%s\"", filename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

        int err = fclose(fp);
        
        if (err != 0) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed closing file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(4);
        }
        exit(rc);
    }
}

int tail(FILE *fp) {
    char *strBuf[lineLimit + 1];
    char ch;
    int lineStart = 0;

    while (fgets(strBuf[lineLimit], maxLine, fp)) {
        // shift array strBuf left
        for (int i = 1; i <= lineLimit; i++) {
            strBuf[i - 1] = strBuf[i];
        }
    }

    // print strBuf array
    for (int i = 0; i <= lineLimit; i++) {
        printf("%s", strBuf[i]);
    }
    if(follow) {
        // keep reading file handler
        /*
        while(true) {
            fread(fp...)
            if rc == 1 {
            // print ch
            } else {
             do nothing
            }
        }*/
    }

    printf("\n");
    return 0;
}