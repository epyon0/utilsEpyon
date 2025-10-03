#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../lib/libEpyon/misc.h" // git clone git@github.com:epyon0/libEpyon.git

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

char dBuff[1024] = {'\0'};
char filename[256] = {'\0'};
bool encode = true, prevVerboseValue, rfc3501 = false, url = false;
int wrapLimit = 76;

char *processData(FILE *fp, char *output);

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
                printf("\nEncode or decode base64 (RFC 4648)\n\n%s <FILE>\nOR\ncat <FILE> | %s\n\n", argv[0], argv[0]);
                printf("[-h || --help]        Print this help message\n");
                printf("[-v || --verbose]     Turn on verbose output\n");
                printf("[-d || --decode]      Decode data instead of encoding\n");
                printf("[-w || --wrap] <INT>  Wrap encoded lines after <INT> character, Default: %d\n", wrapLimit);
                printf("[--rfc-3501]          RFC 3501 encoding for IMAP mailbox names\n");
                printf("[--url]               RFC 4648 URL and filename safe encoding\n");
                printf("<FILE>                File to encode/decode\n");
                printf("\nReturn Values:\n\n");
                printf("  0 = Success\n");
                printf("  1 = File inaccessible\n");
                printf("  2 = No file or input stream given\n");
                printf("  3 = Invalid Wrap Limit\n");

                printf("\n");
                return 0;
            }

            if (strncmp(arg, "--rfc-3501", sizeof("--rfc-3501")) == 0) {
                rfc3501 = true;
                continue;
            }

            if (strncmp(arg, "--url", sizeof("--url")) == 0) {
                url = true;
                continue;
            }

            if ((strncmp(arg, "-d", sizeof("-d")) == 0) || (strncmp(arg, "--decode", sizeof("--decode")) == 0)) {
                encode = false;
                continue;
            }

            if (((strncmp(arg, "-w", sizeof("-w")) == 0) || (strncmp(arg, "--wrap", sizeof("--wrap")) == 0)) && i+1 < argc) {
                wrapLimit = atoi(argv[i+1]);
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
                prevVerboseValue = getverbose();
                setverbose(true);
                snprintf(dBuff, sizeof(dBuff), "Filename \"%s\" is not accessible", arg);
                verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
                setverbose(prevVerboseValue);
                return(1);
            }
        }
    }

    char *output = malloc(sizeof(char));

    if (filename[0] == '\0') {
        verbose("Filename not given", __FILE__, __LINE__, __FUNCTION__);
        if (!isatty(0)) {
            verbose("Detected STDIN", __FILE__, __LINE__, __FUNCTION__);
            char *b64string = processData(stdin, output);
            printf("%s", b64string);
        } else {
            prevVerboseValue = getverbose();
            setverbose(true);
            verbose("No file or input stream given", __FILE__, __LINE__, __FUNCTION__);
            setverbose(prevVerboseValue);
            exit(2);
        }
    } else {
        FILE *fp = fopen(filename, "rb");
        char *b64string = processData(fp, output);
        fclose(fp);
        printf("%s", b64string);
    }
}

char *processData(FILE *fp, char *output) {
    if (wrapLimit < 1) {
        prevVerboseValue = getverbose();
        setverbose(true);
        snprintf(dBuff, sizeof(dBuff), "Invalid wrap limit of: %d", wrapLimit);
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        setverbose(prevVerboseValue);
        exit(3);
    }

    size_t count = 1, rc;
    char lookup[64];
    char padding = '=';
    uint32_t buff;

    for (int i = 48; i < 58; i++) {
        lookup[4+i] = (char)i;
    }
    for (int i = 65; i < 91; i++) {
        lookup[-65+i] = (char)i;
    }
    for (int i = 97; i < 123; i++) {
        lookup[-71+i] = (char)i;
    }

    if (rfc3501) {
        verbose("RFC 3501", __FILE__, __LINE__, __FUNCTION__);
        lookup[62] = '+';
        lookup[63] = ',';
        padding = ' ';
    } else {
        verbose("RFC 4846", __FILE__, __LINE__, __FUNCTION__);
        snprintf(dBuff, sizeof(dBuff), "URL Encoding: %s", url ? "true" : "false");
        verbose(dBuff, __FILE__, __LINE__, __FUNCTION__);
        if (url) {
            lookup[62] = '-';
            lookup[63] = '_';
        } else {
            lookup[62] = '+';
            lookup[63] = '/';
        }
    }

    while (true) {
        rc = fread(&buff, 1, 3, fp);

        if (rc < 3) { // do stuff if 2 or 1 to..
            break;
        }

        if (count % 4 == 0) {
            buff = (0x00FFFFFF & buff);
            output = realloc(output, count * sizeof(char) + 1);
            output[(sizeof(char) * count) - 1] = lookup[(0x3F & buff)];
            count++;
            continue;
        } else if (count % 3 == 0) {
            output = realloc(output, count * sizeof(char));
            output[(sizeof(char) * count) - 1] = lookup[((0xFC0 & buff)>>6)];
            count++;
            continue;
        } else if (count % 2 == 0) {
            output = realloc(output, count * sizeof(char));
            output[(sizeof(char) * count) - 1] = lookup[((0x3F000 & buff)>>12)];
            count++;
            continue;
        } else {
            output = realloc(output, count * sizeof(char));
            output[(sizeof(char) * count) - 1] = lookup[((0xFC0000 & buff)>>18)];
            count++;
            continue;
        }
    }

    if (rc == 1) {
        output = realloc(output, (count * sizeof(char)) + 3);
        output[sizeof(output) - 3] = padding;
        output[sizeof(output) - 2] = padding;
    }

    if (rc == 2) {
        output = realloc(output, (count * sizeof(char)) + 2);
        output[sizeof(output) - 2] = padding;
    }

    output[sizeof(output) - 1] = '\0';
    return output;
}