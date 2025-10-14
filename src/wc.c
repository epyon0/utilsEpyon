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
unsigned long long lineCount = 0, wordCount = 0, byteCount = 0, charCount = 0;
bool verboseValue = false, byteFlag = false, charFlag = false, lineFlag = false, wordFlag = false;

int wc(FILE *fp);

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
                printf("\nDisplay line, word, and byte counts for each input stream\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]    Print this help message\n");
                printf("[-v || --verbose] Turn on verbose output\n");
                printf("[-c || --bytes]   Print the byte count, Default: %s\n", byteFlag ? "true" : "false");
                printf("[-m || --chars]   Print the character count, Default: %s\n", charFlag ? "true" : "false");
                printf("[-l || --lines]   Print the line count, Default: %s\n", lineFlag ? "true" : "false");
                printf("[-w || --words]   Print the word count, Default: %s\n", wordFlag ? "true" : "false");
                printf("<FILE>            File to process\n");
                printf("\nReturn Values:\n");
                printf("  0 = Success\n");
                printf("  1 = File inaccessible\n");
                printf("  2 = No file or input stream given\n");
                printf("  3 = Failed to open file\n");
                printf("  4 = Failed to close file\n");
                
                printf("\n");
                return 0;
            }

            if ((strncmp(arg, "-c", sizeof("-c")) == 0) || (strncmp(arg, "--bytes", sizeof("--bytes")) == 0)) {
                byteFlag = true;
                continue;
            }

            if ((strncmp(arg, "-m", sizeof("-m")) == 0) || (strncmp(arg, "--chars", sizeof("--chars")) == 0)) {
                charFlag = true;
                continue;
            }

            if ((strncmp(arg, "-l", sizeof("-l")) == 0) || (strncmp(arg, "--lines", sizeof("--lines")) == 0)) {
                lineFlag = true;
                continue;
            }

            if ((strncmp(arg, "-w", sizeof("-w")) == 0) || (strncmp(arg, "--words", sizeof("--words")) == 0)) {
                wordFlag = true;
                continue;
            }

            if ((strncmp(arg, "-v", sizeof("-v"))) || (strncmp(arg, "--verbose", sizeof("--verbose")))) {
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

    int rc = 0;
    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN", __FILE__, __LINE__, __FUNCTION__);
            rc = wc(stdin);
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
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            verboseValue = getverbose();
            setverbose(true);
            snprintf(dBuff, sizeof(dBuff), "Failed opening file \"%s\"", filename);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
            setverbose(verboseValue);
            exit(3);
        }
        
        rc = wc(fp);
        
        snprintf(dBuff, sizeof(dBuff), "Closing file \"%s\"", filename);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        int err = fclose(fp);
        if (err) {
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

int wc(FILE *fp) {
    return 0;
}