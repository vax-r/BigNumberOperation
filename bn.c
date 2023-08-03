#include "bn.h"

#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#define CLZ(num) _Generic((num), \
    unsigned int: __builtin_clz(num), \
    unsigned long: __builtin_clzl(num), \
    unsigned long long: __builtin_clzll(num))

// big number version of __builtin_clz
// count the number of zero of src
// note that the unit is BLOCK_LEN
static inline int bn_clz(const bn *src)
{
    int num_of_zero = 0, i;
    for (i = src->size-1; i>=0; i--) {
        if (src->number[i]) {
            n_type zero = 0;
            num_of_zero += (CLZ(src->number[i]) / CLZ(zero));
        } else {
            num_of_zero += 1;
        }
    }
    return num_of_zero;
}

bn *bn_init (size_t size)
{
    bn *new_num = malloc(sizeof(bn));
    if (!new_num) {
        printf("failed to create memory for big number\n");
        exit(1);
    }
    new_num->number = malloc(sizeof(n_type) * size);
    if (!new_num->number) {
        printf("failed to create memory for numbers\n");
        exit(1);
    }
    memset(new_num->number, 0, sizeof(n_type) * size);
    new_num->size = size;
    new_num->sign = 0;
    return new_num;
}

void bn_free (bn *num)
{
    if (!num)
        return;
    free(num->number);
    free(num);
}

void bn_resize (bn *num ,int new_size)
{
    if (num->size == new_size)
        return;
    num->number = realloc(num->number, new_size * sizeof(n_type));
    if (!num->number) {
        printf("failed to realloc\n");
        exit(1);
    }
    if (new_size > num->size)
        memset(num->number + num->size, 0, sizeof(n_type) * (new_size - num->size));
    num->size = new_size;
}

void bn_add (bn *a, bn *b, bn *c)
{
    // c = a + b
    unsigned int new_size = MAX(a->size, b->size) + 1;
    bn_resize(c, new_size);
    
    n_type carry = 0;
    int i;
    for (i=0; i<new_size; i++) {
        n_type tmp1 = i < a->size ? a->number[i] : 0;
        n_type tmp2 = i < b->size ? b->number[i] : 0;
        carry += (tmp1 + tmp2);
        if (carry >= BASE) {
            c->number[i] = (carry - BASE);
            carry = 1;
        }
        else {
            c->number[i] = carry;
            carry = 0;
        }
    }

    if (!c->number[c->size - 1] && c->size > 1)
        bn_resize(c, c->size - 1);
}

// c = a - b
// note that |a| > |b|
void bn_sub(const bn *a, const bn *b, bn *c)
{
    unsigned int new_size = MAX(a->size, b->size);
    bn_resize(c, new_size);

    n_type carry = 0;
    int i;
    for (i=0; i<new_size; i++) {
        n_type tmp1 = i < a->size ? a->number[i] : 0;
        n_type tmp2 = i < b->size ? b->number[i] : 0;
        if (carry == 1)
            tmp1 -= 1;
        if (tmp1 < tmp2) {
            c->number[i] = (n_type) BASE - tmp2 + tmp1;
            carry = 1;
        } else {
            c->number[i] = (n_type) tmp1 - tmp2;
            carry = 0;
        }
    }

    new_size = bn_clz(c);
    if (new_size == c->size)
        new_size -= 1;
    bn_resize(c, c->size - new_size);
}

char *string_resize(char *str, int len)
{
    // remove the prefix zeroes in str
    if (str[0] != '0')
        return str;
    int count = 0;
    for (count=0; str[count] != '\0' && str[count] == '0'; count++);
    int new_len = len - count;
    char *new_str = malloc(sizeof(char) * new_len);
    strncpy(new_str, &str[count], new_len);
    new_str[new_len-1] = '\0';
    free(str);
    return new_str;
}

char *bn_to_string (bn *num)
{
    int str_len = num->size * BLOCK_LEN + 1;
    char *num_str = malloc(sizeof(char) * str_len);
    memset(num_str, '\0', str_len-1);
    char *p = num_str;
    int i,j,k;
    n_type block_num;
    j = 0, k=0;
    block_num = num->number[j];
    for (i=str_len-2; i>=0; i--) {
        num_str[i] = (block_num%10) + '0';
        block_num /= 10;
        k++;
        if (k%BLOCK_LEN == 0) {
            j++;
            block_num = num->number[j];
        }
    }
    num_str = string_resize(num_str, str_len);
    return num_str;
}