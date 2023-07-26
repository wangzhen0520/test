#include <stdio.h>
#include <string.h>
#include <cstdlib>

void hexdump(const unsigned char *p, unsigned int len)
{
    int buf_size = 1024;
    char *buf = (char *)calloc(buf_size, sizeof(char));
    if (buf == NULL) {
        return;
    }
    snprintf(buf, buf_size, "Dumping %u bytes from %p:\r\n", len, p);
    const unsigned char *c = p;
    unsigned int size = len;
    unsigned int i;
    while (size > 0) {
        snprintf(buf + strlen(buf), buf_size - strlen(buf), "%p   ", c);
        for (i = 0; i < 16; i++) {
            if (i < size) {
                snprintf(buf + strlen(buf), buf_size - strlen(buf), "%02x ", c[i]);
            } else {
                snprintf(buf + strlen(buf), buf_size - strlen(buf), "   ");
            }
        }

        for (i = 0; i < 16; i++) {
            if (i < size) {
                snprintf(buf + strlen(buf), buf_size - strlen(buf), "%c", c[i] >= 32 && c[i] < 127 ? c[i] : '.');
            } else {
                snprintf(buf + strlen(buf), buf_size - strlen(buf), " ");
            }
        }
        snprintf(buf + strlen(buf), buf_size - strlen(buf), "\r\n");
        c += 16;
        if (size <= 16) {
            break;
        }
        size -= 16;
    }
    printf("%s\n", buf);

    free(buf);
}

void hexdump2(const unsigned char *p, unsigned int len)
{
    int buf_size = len * 3 + 1;
    char *buf = (char *)calloc(buf_size, sizeof(char));
    for (int i = 0; i < len; i++) {
        snprintf(buf + strlen(buf), buf_size - strlen(buf), "%02x ", p[i]);
    }
    printf("Dumping %u bytes: %s\n", len, buf);
    free(buf);
}