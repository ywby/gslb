#include <stdint.h>
#include <stdlib.h>  
#include <sys/types.h>  
#include <stdio.h>  
#include <sys/socket.h>  
#include <linux/in.h>  
#include <string.h>  
 
typedef struct packetHeader {
        uint32_t type;          //指令类型
        uint32_t dataLen;       //指令body长度
        uint32_t appID;         //APPID
        uint32_t userID;        //用户ID
}packetHeader;


#define DEBUG1	//1 号指令测试
 
int main()  
{  
	int cfd;   
	int recbytes;  
	int sin_size;  
	char buffer[1024]={0};      
	struct sockaddr_in s_add,c_add;   
	unsigned short portnum=808;    

	cfd = socket(AF_INET, SOCK_STREAM, 0);  
	if(-1 == cfd)  {  
		printf("socket fail ! \r\n");  
		return -1;  
	}  
	
	bzero(&s_add,sizeof(struct sockaddr_in));  
	s_add.sin_family=AF_INET;  
	s_add.sin_addr.s_addr= inet_addr("192.168.100.190");   
	s_add.sin_port=htons(portnum); 

	if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))  {  
		printf("connect fail !\r\n");  
		return -1;  
	}  

#ifdef DEBUG1
	char szbuffer[128] = "abcefgh";
	struct packetHeader header;
	header.type = htonl(1);
	header.dataLen = htonl(7);
	header.appID = htonl(1);
	header.userID = htonl(1);

	
	//==================request
	if(-1 == write(cfd,&header,sizeof(header))){
    	printf("write fail!\r\n");
    	return -1;
	}
	
	if(-1 == write(cfd,szbuffer,strlen(szbuffer))){
	}	
	if(-1 == (recbytes = read(cfd,buffer,1024)))  {  
		printf("read data fail !\r\n");  
		return -1;  
	} 

	//==================response
	char tmp[128] ;
	memcpy(tmp,buffer,sizeof(struct packetHeader)); 
	struct packetHeader *_header = (struct packetHeader *) tmp;	
	printf("read header :type = %d, datalen =%d, appid =%d,useid=%d\n",_header->type, _header->dataLen,_header->appID,_header->userID);  
	memcpy(tmp,buffer+sizeof(struct packetHeader),7); 
	printf("read body:data = %s\n", tmp);  
#endif 
	
	while(1){	
		usleep(1000);	
	}	
	close(cfd);   
	return 0;  
  
}  
