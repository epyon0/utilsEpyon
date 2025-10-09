#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char dBuff[1024] = {'\0'};
char filename[256] = {'\0'};
bool verboseValue, bytes = true;
int length = 10 * 1024 * 1024;

int split(FILE *fp);

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
                printf("\nSplit files or input into chunks\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]        Print this help message\n");
                printf("[-v || --verbose]     Turn on verbose output\n");
                printf("[-b || --bytes] <INT> Put <INT> bytes per output file, Default: %s\n", btos(length));
                printf("[-l || --lines] <INT> Put <INT> lines per output file\n");
                printf("<FILE>                File to process\n");
                printf("\nReturn Values:\n");
                printf("  0 = Success\n");
                printf("  1 = File not accessible\n");
                printf("  2 = No input file given\n");
                printf("  3 = Failed opening file\n");
                printf("  4 = Failed closing file\n");
                printf("  5 = Invalid byte length\n");
                printf("  6 = Invalid line length\n");
                printf("  7 = Failed writing file\n");

                printf("\n");
                return 0;
            }

            if (((strncmp(arg, "-l", sizeof("-l")) == 0) || (strncmp(arg, "--lines", sizeof("--lines")) == 0)) && ((i + 1) < argc)) {
                length = atoi(argv[i+1]);
                bytes = false;
                if (length <= 0) {
                    verboseValue = getverbose();
                    setverbose(true);
                    snprintf(dBuff, sizeof(dBuff), "Invalid line length: %d", length);
                    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                    setverbose(verboseValue);
                    exit(6);    
                }
                i++;
                continue;
            }

            if (((strncmp(arg, "-b", sizeof("-b")) == 0) || (strncmp(arg, "--bytes", sizeof("--bytes")) == 0)) && ((i + 1) < argc)) {
                length = atoi(argv[i+1]);
                bytes = true;
                if (length <= 0) {
                    verboseValue = getverbose();
                    setverbose(true);
                    snprintf(dBuff, sizeof(dBuff), "Invalid byte length: %d", length);
                    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                    setverbose(verboseValue);
                    exit(5);
                }
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
                exit(1);
            }
        }
    }

    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN", __FILE__, __LINE__, __FUNCTION__);
            strncpy(filename, "STDIN", strlen("STDIN") + 1);
            int rc = split(stdin);
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

        FILE *fp = fopen(filename, "rb");

        if (fp == NULL) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed opening file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(3);
        }

        int rc = split(fp);

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

int split(FILE *fp) {
    int rc;
    char buff;
    uint64_t count = 0;
    int lenCount = 0;
    char tmpFilename[256] = {'\0'};

    while (true) {
        rc = fread(&buff, 1, 1, fp);
        if (rc < 1) {
            break;
        }

        snprintf(tmpFilename, 256, "%s.%04d", filename, lenCount);
        snprintf(dBuff, sizeof(dBuff), "Output file \"%s\"", tmpFilename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

        snprintf(dBuff, sizeof(dBuff), "Opening file \"%s\"", tmpFilename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        FILE *outfp = fopen(tmpFilename, "w");
        if (outfp == NULL) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed opening file \"%s\"", tmpFilename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(3);
        }

        if (bytes) {
            for (int i = 0; i < length; i++) {
                rc = fwrite(&buff, 1, 1, outfp);
                if (rc != 1) {
                    verboseValue = getverbose();
                    setverbose(true);
                    snprintf(dBuff, sizeof(dBuff), "Failed writing file \"%s\"", tmpFilename);
                    verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                    setverbose(verboseValue);
                    exit(7);
                }
            }
            lenCount++;
        }
        rc = fclose(outfp);
        if (rc != 0) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed closing file \"%s\"", tmpFilename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(4);
        }
        count++;
    }

    return(0);
}