#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED

#include "utility.h"

int encrypt_date(char *in, char *out, char *encrypt_key);

static void sub_bytes(int array[4][4]);
static void shift_rows(int array[4][4]);
static void mix_columns(int array[4][4]);


#endif // ENCRYPT_H_INCLUDED
