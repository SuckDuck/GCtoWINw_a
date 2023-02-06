#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <unistd.h>
#include <string.h>
#include <network.h>
#include <ogc/lwp_watchdog.h>
#include <ogcsys.h>

static u32 *xfb;
static GXRModeObj *rmode;


void Initialise() {
  
	VIDEO_Init();
	PAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}

s32 connectSocket(const char *hostn, u32 port) {
    int sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    //char buffer[1024];
    struct sockaddr_in connect_addr;    

    if (sock == INVALID_SOCKET) {
        printf("[err] Cannot create a socket.\n");
    } 
	else { 
        struct hostent *host = 0;
        host = net_gethostbyname(hostn);
        if (host == NULL) return -2;

        connect_addr.sin_family = AF_INET;
        connect_addr.sin_port = port;
		
        memcpy(&connect_addr.sin_addr, host->h_addr_list[0], host->h_length);
        if (net_connect(sock, (struct sockaddr*)&connect_addr, sizeof(connect_addr)) == -1) {
            net_close(sock);
            return -3;
        }
    }
return 0;
}

s32 connectSocket_ip(const char *ip, u32 port) {
    int sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    struct sockaddr_in connect_addr;    

    if (sock == INVALID_SOCKET) {
        printf("[err] Cannot create a socket.\n");
    } 
	else { 
        connect_addr.sin_family = AF_INET;
        connect_addr.sin_port = port;
        connect_addr.sin_addr.s_addr = inet_addr(ip);
        if (net_connect(sock, (struct sockaddr*)&connect_addr, sizeof(connect_addr)) == -1) {
            net_close(sock);
            return -3;
        }
    }

	char *getRequest = "GET /test/ HTTP/1.1\r\n\r\n";
    s32 bytesTransferred = net_send(sock, getRequest, strlen(getRequest), 0);
    if (bytesTransferred < 0) {
        printf("[err] Failed to send the GET request.\n");
        return -4;
    }
	return 0;
}

int main(int argc, char **argv) {
	Initialise();
    printf("\n \n");
	
	int ret = if_config("192.168.1.9", "255.255.255.0", "192.168.1.1",1,100);
	if (ret != 0) {
		printf("Error inicializando la red: %d\n", ret);
		return ret;
	}
	
	printf("google : %d \n",connectSocket("www.google.com",80));
	printf("local : %d \n",connectSocket_ip("192.168.1.8",5000));
	
	return 0;
}
