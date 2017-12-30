#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "encrypt.h"
#include "decrypt.h"
// I will do myself to implement the AES algorithm
// I used many chinese language in code, sorry!
// Because I am a chinese. I love my country.

#define     NAME_LENGTH         65535

int read_key(char *keyfile, int *round_num, uint8_t *key, uint8_t *s_box, uint8_t *inv_s_box);
static int get_file_length(FILE *fp);

/** \brief      主函数，根据输入的命令来加密或解密数据
 *
 * \param       参数个数
 * \param       参数字符串
 * \return      默认return 0，表示main函数工作正常。
 *
 */
int main(int argc, char *argv[])
{
    FILE *fp_r = NULL;
    FILE *fp_w = NULL;
    char result[NAME_LENGTH] = "";
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
    if (fp_r = fopen(argv[2], "r")) goto end;
    if (fp_w = fopen(result, "w")) goto end;

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
    fclose(fp_r);
    fclose(fp_w);
    return 0;
}

static int get_file_length(FILE *fp)
{
    char *pBuf;
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);
    return len;
}
