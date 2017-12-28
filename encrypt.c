#include "encrypt.h"

int encrypt_date(char *in, char *out, char *encrypt_key)
{
    int array[4][4];
    int key_length = strlen(encrypt_key);
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
    extend_key(encrypt_key, w);

    for (i=0; i<strlen(in); i++)
    {
        convert_to_int_array(in+i, array);
        add_round_key(array, 0, w);
        for (j=0; j<round_num; j++)
        {
            sub_bytes(array);
            shift_rows(array);
            mix_columns(array);
            add_round_key(array, j, w);
        }
        sub_bytes(array);
        shift_rows(array);
        add_round_key(array, round_num, w);
        convert_array_to_string(array, out);
    }

    free(w);
    return 0;
}


/** \brief   �ֽڴ���
 *
 * \param    4*4�ľ���int����
 *
 */
static void sub_bytes(int array[4][4])
{
    int i,j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<4; j++)
        {
            array[i][j] = getNumFromSBox(array[i][j]);
        }
    }
}

/** \brief    ����λ
 *
 * \param     4*4�ľ���int����
 *
 */
static void shift_rows(int array[4][4])
{
    int row_two[4], row_three[4], row_four[4];
    int i;
    for (i=0; i<4; i++)
    {
        row_two[i] = array[1][i];
        row_three[i] = array[2][i];
        row_four[i] = array[3][i];
    }
    leftLoop4int(row_two, 1);
    leftLoop4int(row_three, 2);
    leftLoop4int(row_four, 3);
    for (i=0; i<4; i++)
    {
        array[1][i] = row_two[i];
        array[2][i] = row_three[i];
        array[3][i] = row_four[i];
    }
}

/** \brief      �л��
 *
 * \param       4*4�ľ���int����
 *
 */
static void mix_columns(int array[4][4])
{
    int temp[4][4];
    int i,j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<4; j++)
        {
            temp[i][j] = array[i][j];
        }
    }

    for (i=0; i<4; i++)
    {
        for (j=0; j<4; j++)
        {
            array[i][j] = GF_mul(COL_MIX[i][0], temp[0][j]) ^ GF_mul(COL_MIX[i][1], temp[1][j]) ^ GF_mul(COL_MIX[i][2], temp[2][j]) ^ GF_mul(COL_MIX[i][3], temp[3][j]);
        }
    }
}


