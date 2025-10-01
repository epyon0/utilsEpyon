#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

int main(void) {
    if (!isatty(0)) {
        char ch;
        size_t count = 1;
        bool firstRun = true;
        size_t rc;

        while (true) {
            rc = fread(&ch, 1, 1, stdin);

            if (rc < 1) {
                break;
            }

            if (firstRun) {
                printf("%6lld  ", (long long int)count);
                firstRun = false;
                count++;
            }

            printf("%c", ch);
            
            if (ch == '\n') {
                printf("%6lld  ", (long long int)count);
                count++;
            }
        }

        printf("\n");
    }
}