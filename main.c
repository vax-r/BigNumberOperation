#include "bn.h"

#include <stdio.h>
#include <stdlib.h>

#define TARGET 100

int main () {

    bn *a = bn_init(1);
    bn *b = bn_init(1);
    bn *c = bn_init(1);
    a->number[0] = 0;
    b->number[0] = 1;

    int i;
    printf("Verification of add operation\n");
    for (i=2; i<=TARGET; i++) {
        bn_add(a, b, c);
        printf("f[%d] = %s, size of the number: %d\n", i, bn_to_string(c), c->size);

        bn_free(a);
        a = b;
        b = c;
        c = bn_init(1);
    }

    printf("----------------------------------------\n");

    printf("Verification of sub operation\n");

    printf("f[%d] = %s, size of the number: %u\n", TARGET, bn_to_string(b), b->size);
    printf("f[%d] = %s, size of the number: %u\n", TARGET-1, bn_to_string(a), a->size);
    
    for (i=TARGET-2; i>0; i--) {
        bn_sub(b, a, c);
        printf("f[%d] = %s, size of the number: %u\n", i, bn_to_string(c), c->size);
        
        bn_free(b);
        b = a;
        a = c;
        c = bn_init(1);
    }
    
    bn_free(a);
    bn_free(b);
    bn_free(c);

    return 0;
}