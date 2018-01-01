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

/** \brief      ��������������������������ܻ��������
 *
 * \param       ��������
 * \param       �����ַ���
 * \return      Ĭ��return 0����ʾmain��������������
 *
 */
int main(int argc, char *argv[])
{
    FILE *fp_r;
    FILE *fp_w;
    char input[NAME_LENGTH] = "";
    char result[NAME_LENGTH] = "";
    char deal_string[8] = "";
    int c = 0;
    char path_name[PATH_LENGTH];
    char path[PATH_LENGTH];
    char path_r[PATH_LENGTH];
    char path_w[PATH_LENGTH];
    memset(path, 0, PATH_LENGTH);
    memset(path_r, 0, PATH_LENGTH);
    memset(path_w, 0, PATH_LENGTH);
    int i;

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

    if ((fp_r = fopen(path_r, "r")) == NULL || (fp_w = fopen(path_w, "w")) == NULL)
        goto end;
#if 0
    strcpy(deal_string, argv[1]);

    // get the string of input file
    int len_r = get_file_length(fp_r);
    // test the function of get_file_length
    printf("the file length is %d.\n", len_r);

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
#endif
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
