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

    }

    free(w);
    return 0;
}

/** \brief   extend the key
 *  \param   the string representation of key
 */
static void extend_key(char *key, char *w)
{
    int i;
    int j;
    for (i=0; i<4; i++)
        w[i] = get_word_from_str(key[i*4]);
    for (i=4,j=0; i<strlen(w); i++)
    {
        if (i%4 == 0)
        {
            w[i] = w[i-4] ^ T(w[i-1], j);
            j++;
        }
        else
        {
            w[i] = w[i-4] ^ w[i-1];
        }
    }
}

static int get_word_from_str(char *str)
{
    int one = get_int_from_char(str[0]);
    int two = get_int_from_char(str[1]);
    int three = get_int_from_char(str[2]);
    int four = get_int_from_char(str[3]);

    return one | two | three | four;
}

static int get_int_from_char(char c)
{
    int result = (int)c;
    return result & 0x000000FF;
}

/**
 * ��Կ��չ�е�T����
 */
static int T(int num, int round)
{
    int numArray[4];
    int i;
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1);              // ��ѭ��

    // �ֽڴ���
    for(i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);

    int result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}

/**
 * �������е�Ԫ��ѭ������stepλ
 */
static void leftLoop4int(int array[4], int step)
{
    int temp[4];
    int i;
    for(i = 0; i < 4; i++)
        temp[i] = array[i];

    int index = step % 4 == 0 ? 0 : step % 4;
    for(i = 0; i < 4; i++)
    {
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}

/**
 * ��һ��4�ֽڵ����ĵ�һ�����������ĸ��ֽ�ȡ����
 * ���һ��4��Ԫ�ص������������档
 */
static void splitIntToArray(int num, int array[4])
{
    int one = num >> 24;
    array[0] = one & 0x000000ff;
    int two = num >> 16;
    array[1] = two & 0x000000ff;
    int three = num >> 8;
    array[2] = three & 0x000000ff;
    array[3] = num & 0x000000ff;
}

/**
 * ������������S���л��Ԫ��
 */
static int getNumFromSBox(int index)
{
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}

/**
 * �������еĵ�һ������������Ԫ�طֱ���Ϊ
 * 4�ֽ����͵ĵ�һ�������������ֽڣ��ϲ���һ��4�ֽ�����
 */
static int mergeArrayToInt(int array[4])
{
    int one = array[0] << 24;
    int two = array[1] << 16;
    int three = array[2] << 8;
    int four = array[3];
    return one | two | three | four;
}

/**
 * ��ȡ�������ݵĵ�8λ����4��λ
 */
static int getLeft4Bit(int num)
{
    int left = num & 0x000000f0;
    return left >> 4;
}

/**
 * ��ȡ�������ݵĵ�8λ����4��λ
 */
static int getRight4Bit(int num)
{
    return num & 0x0000000f;
}

/** \brief      convert string to int array
 *
 * \param       string
 * \param       int array
 */
static void convert_to_int_array(char *str, int array[4][4])
{
    int k = 0;
    int i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<4; j++)
        {
            array[j][i] = get_int_from_char(str[k]);
            k++;
        }
    }
}

/** \brief        add the array and the round key
 *
 * \param          array[4][4]------the array will be added the round key
 * \param          w------the expanded key
 *
 */
static void add_round_key(int array[4][4], int round, char *w)
{
    int warray[4];
    int i,j;
    for (i=0; i<4; i++)
    {
        splitIntToArray(w[round*4 + i], warray);
        for (j=0; j<4; j++)
        {
            array[j][i] = array[j][i] ^ warray[j];
        }
    }
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

static int GF_mul(int n, int s)
{
    int result;
    switch(n)
    {
    case 1:
        result = s;
        break;
    case 2:
        result = GF_mul12(s);
        break;
    case 3:
        result = GF_mul13(s);
        break;
    case 4:
        result = GF_mul4(s);
        break;
    case 0x9:
        result = GF_mul9(s);
        break;
    case 0xb:
        result = GF_mul11(s);
        break;
    case 0xd:
        result = GF_mul13(s);
        break;
    case 0xe:
        result = GF_mul14(s);
        break;
    default:
        break;
    }

    return result;
}

static int GF_mul2(int s)
{
    int result = s << 1;
    int a7 = result & 0x00000100;

    if (a7 != 0)
    {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }
    return result;
}

static int GF_mul3(int s)
{
    return  GF_mul2(s)^s;
}

static int GF_mul4(int s)
{
    return GF_mul3(s)^s;
}

static int GF_mul8(int s)
{
    return GF_mul4(GF_mul4(s));
}

static int GF_mul9(int s)
{
    return GF_mul8(s)^s;
}

static int GF_mul11(int s)
{
    return GF_mul9(GF_mul2(s));
}

static int GF_mul12(int s)
{
    return GF_mul11(s) ^ s;
}

static int GF_mul13(int s)
{
    return GF_mul12(s)^s;
}

static int GF_mul14(int s)
{
    return GF_mul13(s) ^ s;
}
