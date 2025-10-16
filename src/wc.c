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
long lineCount = 0, wordCount = 0, byteCount = 0, charCount = 0;
bool verboseValue = false, byteFlag = false, charFlag = false, lineFlag = false, wordFlag = false;

int wc(FILE *fp);

int main(const int argc, const char *argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (!strncmp(argv[i], "-v", sizeof("-v")) || !strncmp(argv[i], "--verbose", sizeof("--verbose"))) {
                setverbose(true);
                break;
            }
        }
        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];
            snprintf(dBuff, sizeof(dBuff), "Processing argument: %s", arg);
            verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);

            if (!strncmp(arg, "-h", sizeof("-h")) || !strncmp(arg, "--help", sizeof("--help"))) {
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

            if (!strncmp(arg, "-c", sizeof("-c")) || !strncmp(arg, "--bytes", sizeof("--bytes"))) {
                byteFlag = true;
                continue;
            }

            if (!strncmp(arg, "-m", sizeof("-m")) || !strncmp(arg, "--chars", sizeof("--chars"))) {
                charFlag = true;
                continue;
            }

            if (!strncmp(arg, "-l", sizeof("-l")) || !strncmp(arg, "--lines", sizeof("--lines"))) {
                lineFlag = true;
                continue;
            }

            if (!strncmp(arg, "-w", sizeof("-w")) || !strncmp(arg, "--words", sizeof("--words"))) {
                wordFlag = true;
                continue;
            }

            if (!strncmp(arg, "-v", sizeof("-v")) || !strncmp(arg, "--verbose", sizeof("--verbose"))) {
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
    char ch, chPrev = '\0';
    int rc;

    // dont use fseek to get bytecount because of stdin
    while(fread(&ch, 1, 1, fp) == 1) {
        byteCount++;

        if (((ch & 0x80) == 0) || ((ch & 0xE0) == 0xC0) || ((ch & 0xF0) == 0xE0) || ((ch & 0xF8) == 0xF0)) {
            charCount++;
        }

        if (((chPrev == '\t') || (chPrev == ' ')) && ((ch != '\t') && (ch != ' '))) {
            wordCount++;
        }

        if (ch == '\n') {
            lineCount++;
        }

        chPrev = ch;
    }

    if (!charFlag && !byteFlag && !wordFlag && !lineFlag) {
            printf("% 6ld% 6ld% 6ld %s\n", lineCount, wordCount, charCount, filename);
    } else {
        if (lineFlag) {
            printf("% 6ld ", lineCount);
        }
        if (wordFlag) {
            printf("% 6ld ", wordCount);
        }
        if (charFlag) {
            printf("% 6ld ", charCount);
        }
        if (byteFlag) {
            printf("% 6ld ", byteCount);
        }
        printf("%s\n", filename);
    }

    return 0;
}