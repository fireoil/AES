#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// I will do myself to implement the AES algorithm

#define     name_length         65535

/**
 * 常量轮值表
 */
static const int Rcon[10] = { 0x01000000, 0x02000000,
                              0x04000000, 0x08000000,
                              0x10000000, 0x20000000,
                              0x40000000, 0x80000000,
                              0x1b000000, 0x36000000
                            };

/**
 * S盒
 */
static const int S[16][16] =  { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                                0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                                0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                                0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                                0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                                0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                                0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                                0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                                0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                                0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                                0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                                0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                                0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                                0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                                0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                                0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
                              };

/**
 * 逆S盒
 */
static const int S_INV[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
                                   0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
                                   0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
                                   0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
                                   0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
                                   0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
                                   0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
                                   0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
                                   0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
                                   0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
                                   0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
                                   0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
                                   0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
                                   0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
                                   0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
                                   0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
                                 };

/**
 * 列混合要用到的矩阵
 */
static const int COL_MIX[4][4] = { 2, 3, 1, 1,
                                   1, 2, 3, 1,
                                   1, 1, 2, 3,
                                   3, 1, 1, 2
                                 };

typedef struct _encrypt_key
{
    int *round_num;
    uint8_t *key;
    uint8_t *s_box;
} encrypt_key_t;

typedef struct _decrypt_key
{
    int *round_num;
    uint8_t *key;
    uint8_t *s_box;
} decrypt_key_t;

int read_key(char *keyfile, int *round_num, uint8_t *key, uint8_t *s_box, uint8_t *inv_s_box);
int encrypt_date(char *in, char *out, char *encrypt_key);
int decrypt_date(char *in, char *out, char *decrypt_key);

static void extend_key(char *key, char *w);
static void leftLoop4int(int array[4], int step);
static int T(int num, int round);
static void splitIntToArray(int num, int array[4]);
static int getNumFromSBox(int index);
static int mergeArrayToInt(int array[4]);
static int getLeft4Bit(int num);
static int getRight4Bit(int num);
static int get_int_from_char(char c);
static int get_word_from_str(char *str);
static void convert_to_int_array(char *str, int array[4][4]);
static void add_round_key(int array[4][4], int round, char *w);
static void sub_bytes(int array[4][4]);
static void shift_rows(int array[4][4]);
static void mix_columns(int array[4][4]);
static int GF_mul2(int s);
static int GF_mul3(int s);



/** \brief      主函数，根据输入的命令来加密或解密数据
 *
 * \param       参数个数
 * \param       参数字符串
 * \return      默认return 0，表示main函数工作正常。
 *
 */
int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    char result[name_length] = "";
    char deal_string[8] = "";
    int c = 0;

    if (argc == 1)
    {
        printf("please input the file which will be encrypted!\n");
        printf("e.g.   AES    encrypt     file_name\n");
        printf("e.g.   AES    decrypt     file_name\n");
        printf("e.g.   AES    encrypt     file_name    result\n");
        printf("e.g.   AES    decrypt     file_name    result\n");

        return 0;
    }
    else if (argc == 3)
    {
        strcat(result, argv[2]);
        strcat(result, ".result");
    }
    else if (argc == 4)
    {
        strcpy(result, argv[3]);
    }

    strcpy(deal_string, argv[1]);
    if(fp = fopen(argv[2], "r")) goto end;

    if (strcmp(deal_string, "encrypt") == 0)
    {
        printf("encrypt\n");
        /* encrypt */
    }
    else if (strcmp(deal_string, "decrypt") == 0)
    {
        printf("decrypt\n");
        /* decrypt */
    }
    else
    {
        printf("please input the command like that.\n");
        printf("e.g.   AES    encrypt     file_name\n");
        printf("e.g.   AES    decrypt     file_name\n");
        printf("e.g.   AES    encrypt     file_name    result\n");
        printf("e.g.   AES    decrypt     file_name    result\n");
    }

    // 每一步的结果都要测试，当然是直接设置好argv[]


end:
    fclose(fp);
    return 0;
}

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

        }
    }

    free(w);
    return 0;
}

int decrypt_date(char *in, char *out, char *decrypt_key)
{
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
 * 密钥扩展中的T函数
 */
static int T(int num, int round)
{
    int numArray[4];
    int i;
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1);              // 字循环

    // 字节代换
    for(i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);

    int result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}

/**
 * 将数组中的元素循环左移step位
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
 * 把一个4字节的数的第一、二、三、四个字节取出，
 * 入进一个4个元素的整型数组里面。
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
 * 根据索引，从S盒中获得元素
 */
static int getNumFromSBox(int index)
{
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}

/**
 * 把数组中的第一、二、三和四元素分别作为
 * 4字节整型的第一、二、三和四字节，合并成一个4字节整型
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
 * 获取整形数据的低8位的左4个位
 */
static int getLeft4Bit(int num)
{
    int left = num & 0x000000f0;
    return left >> 4;
}

/**
 * 获取整形数据的低8位的右4个位
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

/** \brief   字节代换
 *
 * \param    4*4的矩阵，int类型
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

/** \brief    行移位
 *
 * \param     4*4的矩阵，int类型
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

/** \brief      列混合
 *
 * \param       4*4的矩阵，int类型
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
