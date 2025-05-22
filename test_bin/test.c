// test.c
#include <fcntl.h>
int main() {
    open("secret.txt", O_RDONLY);
    return 0;
}

