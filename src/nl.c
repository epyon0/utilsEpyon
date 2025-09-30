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
        int count = 0;

        while (true) {
            fread(&ch, 1, 1, stdin);

            if (count == 0) {
                printf("%d ", count);
            }

            printf("%c", ch);
            
            if (ch == '\n') {
                printf("%d ", count);
            }

            count++;
        }
    }
}