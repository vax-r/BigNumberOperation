#include "bn.h"

#include <stdio.h>
#include <stdlib.h>

#define TARGET 300

int main () {

    bn *a = bn_init(1);
    bn *b = bn_init(1);
    bn *c = bn_init(1);
    a->number[0] = 0;
    b->number[0] = 1;

    int i,j;
    char *tmp;
    for (i=2; i<=TARGET; i++) {
        bn_add(a, b, c);
        tmp = bn_to_string(c);
        printf("f[%d] = ", i);
        printf("%s\n", tmp);

        bn_free(a);
        a = b;
        b = c;
        c = bn_init(1);
    }

    bn_free(a);
    bn_free(b);
    bn_free(c);
    free(tmp);

    return 0;
}