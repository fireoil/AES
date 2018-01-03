#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "encrypt.h"
#include "decrypt.h"
// I will do myself to implement the AES algorithm
// I used many chinese language in code, sorry!
// Because I am a chinese. I love my country.

#define     NAME_LENGTH         256
#define     PATH_LENGTH         256

int read_key(char *keyfile, char *key);
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
    char input[NAME_LENGTH];
    char result[NAME_LENGTH];
    char deal_string[8];
    char path_name[PATH_LENGTH];
    char path[PATH_LENGTH];
    char path_r[PATH_LENGTH];
    char path_w[PATH_LENGTH];

    memset(input, 0, NAME_LENGTH);
    memset(result, 0, NAME_LENGTH);
    memset(deal_string, 0, 8);

    memset(path_name, 0, PATH_LENGTH);
    memset(path, 0, PATH_LENGTH);
    memset(path_r, 0, PATH_LENGTH);
    memset(path_w, 0, PATH_LENGTH);

    strcpy(path_name, argv[0]);
    char *p = strrchr(path_name, '/');
    int location = p - path_name;

    snprintf(path, location+1, "%s", path_name);

    strcat(path, "/");
    strcpy(path_r, path);
    strcpy(path_w, path);

#if 1
    argc = 3;
    argv[1] = "encrypt";
    argv[2] = "hello";

    char key[16] = "0123456789abcdef";
#endif // 0

    if (argc < 3)
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
        snprintf(input, strlen(argv[2]) + 1, "%s", argv[2]);
        strcat(result, argv[2]);
        strcat(result, ".result");
    }
    else if (argc == 4)
    {
        strcpy(input, argv[2]);
        strcpy(result, argv[3]);
    }

    strcat(path_r, input);
    strcat(path_w, result);

    // the two command below catch a problem which is very strange.
#if 1
    if ((fp_r = fopen(path_r, "r")) == NULL || (fp_w = fopen(path_w, "w")) == NULL)
    {
        goto end;
    }
#endif // 0
#if 1
    strcpy(deal_string, argv[1]);

    // get the string of input file
    int len_r = get_file_length(fp_r);
    if (len_r < 16)
    {
        goto end;
    }
    // test the function of get_file_length
    printf("the file length is %d.\n", len_r);

    if (strcmp(deal_string, "encrypt") == 0)
    {
        printf("encrypt\n");
        /* encrypt */
        //encrypt_date(char *in, char *out, char *encrypt_key);
        int len_once_time = len_r;
        while (len_r >= 0)
        {
            char buf[16];
            read(fd_r, buf, 16);

            len_r -= 16
        }
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
#endif
end:
    if (fp_r != NULL)
        fclose(fp_r);
    if (fp_w != NULL)
        fclose(fp_w);
    return 0;
}

static int get_file_length(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);
    return len;
}
