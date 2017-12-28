#ifndef DECRYPT_H_INCLUDED
#define DECRYPT_H_INCLUDED
#include "utility.h"

int decrypt_date(char *in, char *out, char *decrypt_key);
static int get_num_from_SINV_BOX(int index);
static void de_sub_bytes(int array[4][4]);
static void right_loop_4int(int array[4], int step);
static void de_shift_rows(int array[4][4]);
static void de_mix_columns(int array[4][4]);
static void add_round_two_array(int arrayA[4][4], int arrayB[4][4]);
static void get_array_from_4W(int i, int array[4][4], char *w);


#endif // DECRYPT_H_INCLUDED
