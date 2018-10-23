//������ص�ͷ�ļ�  
#include<termios.h>    /*PPSIX �ն˿��ƶ���*/ 
#include<stdio.h>      /*��׼�����������*/  
#include<stdlib.h>     /*��׼�����ⶨ��*/  
#include<unistd.h>     /*Unix ��׼��������*/  
#include<sys/types.h>   
#include<sys/stat.h>     
#include<fcntl.h>      /*�ļ����ƶ���*/  
#include<errno.h>      /*����Ŷ���*/  
#include<string.h> 

#include "tty_com.h"
#include "log.h"
int ttyfd;

char head_up_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x00};
char head_down_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x01};
char foot_up_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x02};
char foot_down_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x03};
char leg_up_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x04};
char leg_down_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x05};
char lumbar_up_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x06};
char lumbar_down_buf[6]={0x5A,0x10,0x10,0x02,0x40,0x07};
char stop_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x0F};
char flat_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x10};
char antisnore_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x16};
char lounge_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x17};
char zero_gravity_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x13};
char incline_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x18};
char lounge_program_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x27};
char zero_gravity_program_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x23};
char incline_program_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x28};
char massage_on_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x51};
char wave_one_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x52};
char wave_two_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x53};
char wave_three_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x54};
char wave_four_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x55};
char full_body_one_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x56};
char full_body_two_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x57};
char massage_up_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x60};
char massage_down_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x61};
char massage_stop_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x6F};
char light_on_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x73};
char lights_on_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x73};
char light_off_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x74};
char lights_off_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x74};
char toggle_light_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x70};
char toggle_lights_buf[6]={0x5A,0x10,0x10,0x02,0x30,0x70};
char feedback_off_buf[6]={"None"};
char feedback_on_buf[6]={"None"};

char send_cmd_to_com[20]={};

//�궨��  
#define TRUE   0  
#define FALSE  -1  


/*******************************************************************
* ���ܣ�            ���ô�������λ��ֹͣλ��Ч��λ
* ��ڲ�����        fd         �����ļ�������
*                   speed      �����ٶ�
*                   flow_ctrl  ����������
*                   databits   ����λ   ȡֵΪ 7 ����8
*                   stopbits   ֹͣλ   ȡֵΪ 1 ����2
*                   parity     Ч������ ȡֵΪN,E,O,,S
*���ڲ�����         ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UARTx_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{

    int   i;
        int   speed_arr[] = {B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
        int   name_arr[]  = { 115200,  57600,  38400,  19200,  9600,  4800,  2400,  1200,  300 };

    struct termios options;

        /* tcgetattr(fd,&options)�õ���fdָ��������ز������������Ǳ�����options,
         * �ú���,�����Բ��������Ƿ���ȷ���ô����Ƿ���õȡ�
         * �����óɹ�����������ֵΪ0��������ʧ�ܣ���������ֵΪ1.
         */
    if( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");    
        return(FALSE); 
    }

    //���ô������벨���ʺ����������
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if  (speed == name_arr[i])
        {       
            cfsetispeed(&options, speed_arr[i]); 
            cfsetospeed(&options, speed_arr[i]);  
            printf("uart_speed=%d ", speed );   
        }
    }     

    //�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���
    options.c_cflag |= CLOCAL;
    //�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������
    options.c_cflag |= CREAD;

    //��������������
    switch(flow_ctrl)
    {
        case 0 ://��ʹ��������
            options.c_cflag &= ~CRTSCTS;
                break;   

        case 1 ://ʹ��Ӳ��������
                options.c_cflag |= CRTSCTS;
                break;
        case 2 ://ʹ�����������
                options.c_cflag |= IXON | IXOFF | IXANY;
                break;
    }
    printf("flow_ctrl=%d ",flow_ctrl);


    //��������λ
    options.c_cflag &= ~CSIZE; //����������־λ
    switch (databits)
    {  
        case 5    :
            options.c_cflag |= CS5;
            break;
        case 6    :
            options.c_cflag |= CS6;
            break;
        case 7    :    
            options.c_cflag |= CS7;
            break;
        case 8:    
            options.c_cflag |= CS8;
            break;  
        default:   
            fprintf(stderr,"Unsupported data size\n");
            return (FALSE); 
    }
    printf("databits=%d ",databits);


    //����У��λ
    switch (parity)
    {  
        case 'n':
        case 'N': //����żУ��λ��
            options.c_cflag &= ~PARENB; 
            options.c_iflag &= ~INPCK;  
            printf("parity=N ");  
            break; 
        case 'o':  
        case 'O'://����Ϊ��У��
            options.c_cflag |= (PARODD | PARENB); 
            options.c_iflag |= INPCK;             
            break; 
        case 'e': 
        case 'E'://����ΪżУ��
            options.c_cflag |= PARENB; 
            options.c_cflag &= ~PARODD;       
            options.c_iflag |= INPCK;       
            break;
        case 's':
        case 'S': //����Ϊ�ո�
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break; 
        default:  
            fprintf(stderr,"Unsupported parity\n");   
            return (FALSE); 
    } 


    // ����ֹͣλ 
    switch (stopbits)
    {  
        case 1:   
            options.c_cflag &= ~CSTOPB; 
            break; 
        case 2:   
            options.c_cflag |= CSTOPB; 
            break;
        default:   
            fprintf(stderr,"Unsupported stop bits\n"); 
            return (FALSE);
    }
    printf("stopbits=%d \n",stopbits);


    //�޸����ģʽ��ԭʼ������� /*Output*/
    options.c_oflag &= ~OPOST;

    //options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//�Ҽӵ� /*Input*/ 
    options.c_lflag &= ~(ISIG | ICANON);  

    //���õȴ�ʱ�����С�����ַ�
    options.c_cc[VTIME] = 0; /* ��ȡһ���ַ��ȴ�1*(1/10)s */  
    options.c_cc[VMIN] = 1;  /* ��ȡ�ַ������ٸ���Ϊ1 */

    //������??����������������ݣ����ǲ��ٶ�ȡ
    tcflush(fd,TCIFLUSH);

    //�������� (���޸ĺ��termios�������õ������У�
    if (tcsetattr(fd,TCSANOW,&options) != 0)  
    {
        perror("com set error!\n");  
        return (FALSE); 
    }

    return (TRUE); 
}


/*******************************************************************
* ���ƣ�           UART0_Recv
* ���ܣ�           ���մ�������
* ��ڲ�����       fd          :�ļ�������    
*                  rcv_buf     :���մ��������ݴ���rcv_buf�������??*                  data_len    :һ֡���ݵĳ���
* ���ڲ�����       ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UARTx_Recv(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;

    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);

    time.tv_sec = 10;
    time.tv_usec = 0;

    //ʹ��selectʵ�ִ��ڵĶ�·ͨ��
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel)
    {
            len = read(fd,rcv_buf,data_len);
        printf("I am right!(version1.2) len = %d fs_sel = %d\n",len,fs_sel);  
            return len;
    }
    else
    {
        printf("Sorry,I am wrong!\n");  
            return FALSE;
    }     
}

/*******************************************************************
* ���ƣ�            UART0_Send
* ���ܣ�            ��������
* ��ڲ�����         fd         :�ļ�������    
*                   send_buf    :�??�Ŵ��ڷ�������
*                   data_len    :һ֡���ݵĸ��??
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UARTx_Send(int fd, char *send_buf,int data_len)
{
    int len = 0;

    len = write(fd,send_buf,data_len);
    if (len == data_len ){
        return len;
    }     
    else{
        tcflush(fd,TCOFLUSH);
        return FALSE;
    }
}


/*****************************************************************
* ���ܣ�         �򿪴��ڲ����ش����豸�ļ�����
* ��ڲ�����     fd:�ļ�������   port:���ں�(ttyS0,ttyS1,ttyS2)
* ���ڲ�����     ��ȷ����Ϊ1�????�󷵻�Ϊ0
*****************************************************************/
int UARTx_Open(int fd, const char * ttyX)
{
    //fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(ttyX, O_RDWR | O_NOCTTY | O_NDELAY);

    if(fd == FALSE)
    {
        perror("open_port: Unable to open /dev/ttyS0 -");
        return(FALSE);
    }

    //�жϴ��ڵ�״̬�Ƿ�Ϊ����״̬
    if( fcntl(fd,F_SETFL,0)<0 ){
        printf("fcntl failed! \n");
        return(FALSE);
    }     
    else{
        printf("fcntl=%d \n",fcntl(fd, F_SETFL,0));
    }

    //�����Ƿ�Ϊ�??���豸    
    if(0 == isatty(STDIN_FILENO) ){
        printf("standard input is not a terminal device \n");
        return(FALSE);
    }
    else{
        printf("isatty success!\n");
    }
    printf("fd->open=%d \n",fd);

    //UARTx_Set(fd,115200,0,8,1,'N');
    //UARTx_Set(fd,57600,0,8,1,'N');

    return (fd);
}


/******************************************************
* ���ƣ�    main
*******************************************************************/

//void * send_data_to_com_thread(void * data)
void send_data_to_com( int fd)
{
	//int fd;                            //�ļ�������  
    	int err;                           //���ص��ú�����״̬  
    	int len;


    	//fd = UART0_Open(fd,argv[1]); //�򿪴��ڣ������ļ�������  
	//fd = UARTx_Open(fd); //�򿪴��ڣ������ļ�������  

/*
	fd = UARTx_Open(fd, "/dev/ttyS0");

    	if(FALSE == fd){
        	exit(1);
	}
*/
	/*
	���ô��ڲ�????    err = UARTx_Set(fd,57600,0,8,1,'N');  
    	if (FALSE == err){
        	printf("Set Port Error\n");
        	exit(1);
    	}
	*/


	//while(1){
	//	sleep(1);
		printf("send data:%s\n",send_cmd_to_com);
		if(!strcmp(send_cmd_to_com, "head up")){
			UARTx_Send(fd, head_up_buf, strlen(send_cmd_to_com));
		}else if(!strcmp(send_cmd_to_com, "lights on")){
			len=UARTx_Send(fd, lights_on_buf, 6);
			if(len>0){
				LOGD("send data len:%d\n",len);
			}
		}else if(!strcmp(send_cmd_to_com, "lights off")){
			UARTx_Send(fd, lights_off_buf, strlen(send_cmd_to_com));
		}else if(!strcmp(send_cmd_to_com, "stop")){
			UARTx_Send(fd, stop_buf, strlen(send_cmd_to_com));
		}else if(!strcmp(send_cmd_to_com, "head down")){
			UARTx_Send(fd, head_down_buf, strlen(send_cmd_to_com));
		}else if(!strcmp(send_cmd_to_com, "foot up")){
                        UARTx_Send(fd, foot_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "foot down")){
                        UARTx_Send(fd, foot_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "foot up")){
                        UARTx_Send(fd, foot_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "leg up")){
                        UARTx_Send(fd, leg_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "leg down")){
                        UARTx_Send(fd, leg_down_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "lumbar up")){
                        UARTx_Send(fd, lumbar_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "lumbar down")){
                        UARTx_Send(fd, lumbar_down_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "flat")){
                        UARTx_Send(fd, flat_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "antisnore")){
                        UARTx_Send(fd, antisnore_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "lounge")){
                        UARTx_Send(fd, lounge_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "zero gravity")){
                        UARTx_Send(fd, zero_gravity_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "incline")){
                        UARTx_Send(fd, incline_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "lounge program")){
                        UARTx_Send(fd, lounge_program_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "zero gravity program")){
                        UARTx_Send(fd, zero_gravity_program_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "incline program")){
                        UARTx_Send(fd, incline_program_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "massage on")){
                        UARTx_Send(fd, massage_on_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "massage up")){
                        UARTx_Send(fd, massage_up_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "massage down")){
                        UARTx_Send(fd, massage_down_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "massage stop")){
                        UARTx_Send(fd, massage_stop_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "wave one")){
                        UARTx_Send(fd, wave_one_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "wave two")){
                        UARTx_Send(fd, wave_two_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "wave three")){
                        UARTx_Send(fd, wave_three_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "wave four")){
                        UARTx_Send(fd, wave_four_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "full body one")){
                        UARTx_Send(fd, full_body_one_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "full body two")){
                        UARTx_Send(fd, full_body_two_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "light on")){
                        UARTx_Send(fd, light_on_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "light off")){
			UARTx_Send(fd, light_off_buf, strlen(send_cmd_to_com));
		}else if(!strcmp(send_cmd_to_com, "toggle light")){
                        UARTx_Send(fd, toggle_light_buf, strlen(send_cmd_to_com));
                }else if(!strcmp(send_cmd_to_com, "toggle lights")){
                        UARTx_Send(fd, toggle_lights_buf, strlen(send_cmd_to_com));
                }else{
			printf(".");
		}
	//}
}

/*
int main(int argc, char **argv)  
//int main(void)  
{  
    int fd;                            //�ļ�������  
    int err;                           //���ص��ú�����״̬  
    int len;                          
    int i;  

    char rcv_buf[256];         
    char send_buf[20]="uartx_test\n";  

    //fd = UART0_Open(fd,argv[1]); //�򿪴��ڣ������ļ�������  
    //fd = UARTx_Open(fd); //�򿪴��ڣ������ļ�������  
    fd = UARTx_Open(fd,argv[1]);
    if(FALSE == fd){        
        exit(1);    
    }

    //���ô��ڲ��??    err = UARTx_Set(fd,57600,0,8,1,'N');  
    if (FALSE == err){  
        printf("Set Port Error\n"); 
        exit(1); 
    }
   
    if(!strcmp(argv[2], "Head Up"))
    {
	printf("5A1010024000\\n size:%d\n",sizeof("5A1010024000\n"));
	memcpy(send_buf, "5A1010024000\n", sizeof("5A1010024000\n"));
    }


    for( i = 0; i < 10; i++ )  
    {  
        len = UARTx_Send(fd, send_buf, strlen(send_buf));  
        if(len > 0)  
            printf(" %d send data successful len=%d \n",i,len);  
        else  
            printf("send data failed!\n"); 
    }

    len = 0 ;
    while(1) 
    {
        len = read(fd, rcv_buf, 256);  

        if(len > 0){
            rcv_buf[len] = '\n';
            UARTx_Send(fd,rcv_buf,len+1);
            printf("len=%d uart_RX_ok \n", len); 

            len = 0 ;
            memset(rcv_buf, 0, sizeof(rcv_buf) );
        } 
        //sleep(2); 
    } 

        printf("Close...\n");
    close(fd);   

    return 0;
}
*/
