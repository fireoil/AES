#include "decrypt.h"
int decrypt_date(char *in, char *out, char *decrypt_key)
{
    int array[4][4];
    int key_length = strlen(decrypt_key);
    int i,j;
    char *w;
    const int block_num = 4;
    int k_num;
    int round_num;
    switch(key_length)
    {
    case 16:
        k_num = 4;
        round_num = 10;
        break;
    case 24:
        k_num = 6;
        round_num = 12;
        break;
    case 32:
        k_num = 8;
        round_num = 14;
        break;
    default:
        break;
    }

    w = (char *)malloc(block_num*(round_num+1)*4);
    extend_key(decrypt_key, w);

    int wArray[4][4];
    for (i=0; i<strlen(in); i++)
    {
        convert_to_int_array(in+i, array);
        add_round_key(array, round_num, w);
        for(j=round_num-1; j>=1; j--)
        {
            de_sub_bytes(array);
            de_shift_rows(array);
            de_mix_columns(array);
            get_array_from_4W(j, wArray, w);
            de_mix_columns(wArray);
            add_round_two_array(array, wArray);
        }
        de_sub_bytes(array);
        de_shift_rows(array);
        add_round_key(array, 0, w);
        convert_array_to_string(array, out);
    }

    free(w);
    return 0;
}
static int get_num_from_SINV_BOX(int index)
{
    int row = getLeft4Bit(index);
    int column = getRight4Bit(index);
    return S_INV[row][column];
}
static void de_sub_bytes(int array[4][4])
{
    int i,j;
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
            array[i][j] = get_num_from_SINV_BOX(array[i][j]);
        }
}

static void right_loop_4int(int array[4], int step)
{
    int temp[4];
    int i, index;
    for (i=0; i<4; i++)
    {
        temp[i] = array[i];
    }
    index = step % 4 == 0 ? 0:step%4;
    index = 3 - index;
    for (i=3; i>=0; i--)
    {
        array[i] = temp[index];
        index--;
        index = index == -1?3 : index;
    }
}

static void de_shift_rows(int array[4][4])
{
    int row_two[4], row_three[4], row_four[4];
    int i;
    for (i=0; i<4; i++)
    {
        row_two[i] = array[1][i];
        row_three[i] = array[2][i];
        row_four[i] = array[3][i];
    }
    right_loop_4int(row_two, 1);
    right_loop_4int(row_three, 2);
    right_loop_4int(row_four, 3);
    for(i=0; i<4; i++)
    {
        array[1][i] = row_two[i];
        array[2][i] = row_three[i];
        array[3][i] = row_four[i];
    }
}
static void de_mix_columns(int array[4][4])
{
    int temp[4][4];
    int i,j;
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
            temp[i][j] = array[i][j];
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        {
            array[i][j] = GF_mul(de_col_mix[i][0], temp[0][j]) ^ GF_mul(de_col_mix[i][1], temp[1][j]) ^ GF_mul(de_col_mix[i][2], temp[2][j]) ^ GF_mul(de_col_mix[i][3], temp[3][j]);
        }
}

static void add_round_two_array(int arrayA[4][4], int arrayB[4][4])
{
    int i,j;
    for ( i=0; i<4; i++ )
        for ( j=0; j<4; j++ )
            arrayA[i][j] ^= arrayB[i][j];
}

static void get_array_from_4W(int i, int array[4][4], char *w)
{
    int index, j;
    int col_one[4], col_two[4], col_three[4], col_four[4];
    index = i*4;
    splitIntToArray(w[index], col_one);
    splitIntToArray(w[index+1], col_two);
    splitIntToArray(w[index+2], col_three);
    splitIntToArray(w[index+3], col_four);

    for (j=0; j<4; j++)
    {
        array[j][0] = col_one[j];
        array[j][1] = col_two[j];
        array[j][2] = col_three[j];
        array[j][3] = col_four[j];
    }
}
