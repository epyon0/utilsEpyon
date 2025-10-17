#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char dBuff[1024] = {'\0'}, filename[256] = {'\0'};
bool verboseValue;

struct zipHeader {
    uint32_t magicNumber;
    uint16_t version;
    uint16_t compression;
    uint16_t modTime;
    uint16_t modDate;
    uint32_t crc;
    uint32_t compressSize;
    uint32_t uncompressSize;
    uint16_t filenameLen;
    uint16_t extrFieldLen;
    char * filename;
    char * extraField;
};

int zls(FILE *fp);

int main(const int argc, const char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (!strncmp(argv[i], "-v", sizeof("-v")) || !strncmp(argv[i], "--verbose", sizeof("--verbose"))) {
                setverbose(true);
                break;
            }
        }
        if (int i = 1; i < argc; i++) {
            const char *arg = argv[i];
            snprintf(dBuff, sizeof(dBuff), "Processing argument: %s", arg);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

            if (!strncmp(arg, "-h", sizeof("-h")) || !strncmp(arg, "--help", sizeof("--help"))) {
                printf("\nDisplay contents of a zip file (Supported: PKZIP, ...)\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]   Print this help message\n");

                printf("\nReturn Values:\n");
                printf("  0 = Success\n");
                printf("  1 = File inaccessible\n");
                printf("  2 = No file or input stream given\n");
                printf("  3 = Failed to open file\n");
                printf("  4 = Failed to close file\n");

                printf("\n");
                return(0);
            }

            if (!strncmp(arg, "-v", sizeof("-v")) || !strncmp(arg, "--verbose", sizeof("--verbose"))) {
                continue;
            }
            if (!access(arg, F_OK)) {
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

    int rc = 0;
    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN", __FILE__, __LINE__, __FUNCTION__);
            rc = zs(stdin);
            exit(rc);
        } else {
            verboseValue = getverbose();
            setverbose(true);
            verbose("No file or input stream given", __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            return(2);
        }
    } else {
        snprintf(dBuff, sizeof(dBuff), "Opening file \"%s\"", filename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed opening file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            return(3);
        }

        rc = zs(fp);

        snprintf(dBuff, sizeof(dBuff), "Closing file \"%s\"", filename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        int err = fclose(fp);
        if (err) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed closing file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            return(4);
        }
        exit(rc);
    }
}

int zls(FILE *fp) {

}