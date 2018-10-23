#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cJSON.h"  //��Ҫ�Ѹ�ͷ�ļ����ں������  ������Ҳ���size_t

char  url[100] = "https://";

void save_version(char * url)
{
	char str[80]; 
        memcpy(str, url, strlen(url));

	//char str[] = "https://alexa.topqizhi.com/dongle/20180906.bin";
	FILE *incaptureFp = fopen("/root/version","wb");

        char *token = strtok(str, "https://alexa.topqizhi.com/dongle/");

	while(token != NULL)
	{
		printf("%s\n", token);
		fwrite(token, 1, strlen(token), incaptureFp);
		break;	//token=strtok(NULL, ",");
        }
       fclose(incaptureFp);
}

//����JSON
char * parse_json(const char *filename)
{
 
    //���ļ��ж�ȡҪ������JSON����
    FILE *fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = (char*)malloc(len + 1);
    fread(data, 1, len, fp);
    fclose(fp);

    //����JSON����
    cJSON *root_json = cJSON_Parse(data);    //���ַ���������json�ṹ��
    if (NULL == root_json)
    {
        printf("error:%s\n", cJSON_GetErrorPtr());
        cJSON_Delete(root_json);
        return NULL;
    }
    //"name":"EVDI"
    cJSON *result_json = cJSON_GetObjectItem(root_json, "result");
    if (result_json != NULL)
    {
        char *result = cJSON_Print(result_json);    //��JSON�ṹ���ӡ���ַ����� ��Ҫ�Լ��ͷ�
        printf("result:%s\n", result);
        free(result);
    }

    cJSON *message_json = cJSON_GetObjectItem(root_json, "message");
    if (message_json != NULL)
    {
        char *message = cJSON_Print(message_json);    //��JSON�ṹ���ӡ���ַ����� ��Ҫ�Լ��ͷ�
        printf("message:%s\n", message);
        free(message);
    }

    cJSON *data_json = cJSON_GetObjectItem(root_json, "data");
    if (data_json != NULL)
    {
        char *data = cJSON_Print(data_json);    //��JSON�ṹ���ӡ���ַ����� ��Ҫ�Լ��ͷ�
        if(strcmp(data, "null"))
	{
        	printf("data:%s\n", data);
        
        	strncpy(url+8, data+1, strlen(data)-2);
        	free(data);
        	printf("url:%s\n",url);
		save_version(url);

        	return url;

        }else{
		return NULL;
	}
    }
    //"data":"..."
    //id
    //username
/*
    char *username = cJSON_Print(cJSON_GetObjectItem(data_json, "username"));
    printf("username:%s\n", username);
    free(username);
    //userpass
    char *userpass = cJSON_Print(cJSON_GetObjectItem(data_json, "userpass"));
    printf("userpass:%s\n", userpass);
    free(userpass);
    //version
    char *version = cJSON_Print(cJSON_GetObjectItem(data_json, "version"));
    printf("version:%s\n", version);
*/
    //free(version);
    //free(data);
}
//����JSON
void create_json()
{
    printf("----------------create json start-----------------------------\n");
    //��JSON
    cJSON *root_json = cJSON_CreateObject();
    cJSON_AddItemToObject(root_json, "name", cJSON_CreateString("EVDI"));
    cJSON *data_json = cJSON_CreateObject();
    cJSON_AddItemToObject(root_json, "data", data_json);
    //��ӵ���һ�ַ�ʽ:cJSON_AddNumberToObject(data_json, "id", 1);ͨ��Դ�뷢�ֽ����Ƕ�cJSON_AddItemToObject��define
    cJSON_AddItemToObject(data_json, "id", cJSON_CreateNumber(1));
    //��ӵ���һ�ַ�ʽ:cJSON_AddStringToObject(data_json, "username", "hahaya");
    cJSON_AddItemToObject(data_json, "username", cJSON_CreateString("hahaya"));
    cJSON_AddItemToObject(data_json, "userpass", cJSON_CreateString("123456"));
    cJSON_AddItemToObject(data_json, "version", cJSON_CreateString("1.0"));
    //��ӡJSON
    char *out = cJSON_Print(root_json);
    printf("%s\n", out);
    free(out);
    printf("----------------create json end-------------------------------\n");
}

/*
int main()
{
    parse_json("/tmp/page.json");
//    create_json();
    return 0;
}
*/

