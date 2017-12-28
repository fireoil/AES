#include "utility.h"

/**
 * ��һ��4�ֽڵ����ĵ�һ�����������ĸ��ֽ�ȡ����
 * ���һ��4��Ԫ�ص������������档
 */
void splitIntToArray(int num, int array[4])
{
    int one = num >> 24;
    array[0] = one & 0x000000ff;
    int two = num >> 16;
    array[1] = two & 0x000000ff;
    int three = num >> 8;
    array[2] = three & 0x000000ff;
    array[3] = num & 0x000000ff;
}

int GF_mul(int n, int s)
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

int GF_mul2(int s)
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

int GF_mul3(int s)
{
    return  GF_mul2(s)^s;
}

int GF_mul4(int s)
{
    return GF_mul3(s)^s;
}

int GF_mul8(int s)
{
    return GF_mul4(GF_mul4(s));
}

int GF_mul9(int s)
{
    return GF_mul8(s)^s;
}

int GF_mul11(int s)
{
    return GF_mul9(GF_mul2(s));
}

int GF_mul12(int s)
{
    return GF_mul11(s) ^ s;
}

int GF_mul13(int s)
{
    return GF_mul12(s)^s;
}

int GF_mul14(int s)
{
    return GF_mul13(s) ^ s;
}

/**
 * ��ȡ�������ݵĵ�8λ����4��λ
 */
int getLeft4Bit(int num)
{
    int left = num & 0x000000f0;
    return left >> 4;
}

/**
 * ��ȡ�������ݵĵ�8λ����4��λ
 */
int getRight4Bit(int num)
{
    return num & 0x0000000f;
}

/** \brief   extend the key
 *  \param   the string representation of key
 */
void extend_key(char *key, char *w)
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

int get_word_from_str(char *str)
{
    int one = get_int_from_char(str[0]);
    int two = get_int_from_char(str[1]);
    int three = get_int_from_char(str[2]);
    int four = get_int_from_char(str[3]);

    return one | two | three | four;
}


/**
 * ��Կ��չ�е�T����
 */
int T(int num, int round)
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

int get_int_from_char(char c)
{
    int result = (int)c;
    return result & 0x000000FF;
}

/**
 * �������е�Ԫ��ѭ������stepλ
 */
void leftLoop4int(int array[4], int step)
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
 * ������������S���л��Ԫ��
 */
int getNumFromSBox(int index)
{
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}

/**
 * �������еĵ�һ������������Ԫ�طֱ���Ϊ
 * 4�ֽ����͵ĵ�һ�������������ֽڣ��ϲ���һ��4�ֽ�����
 */
int mergeArrayToInt(int array[4])
{
    int one = array[0] << 24;
    int two = array[1] << 16;
    int three = array[2] << 8;
    int four = array[3];
    return one | two | three | four;
}

/** \brief      convert string to int array
 *
 * \param       string
 * \param       int array
 */
void convert_to_int_array(char *str, int array[4][4])
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

