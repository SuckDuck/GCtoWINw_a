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


int main(int argc, char **argv) {
	Initialise();
    printf("\n \n");
	
	u32 botones;
	s8 left_stick_x;
	s8 left_stick_y;
	s8 right_stick_x;
	s8 right_stick_y;
	char temporal_stick[202];
	char lectura_botones[25];
	memset(lectura_botones, '0', 25);
	memset(temporal_stick, '0', 202);
	
	char request[100];
	//char *getRequest = "GET /set_joystick?joystick=100 HTTP/1.1\r\n\r\n";
	const char *ip = "192.168.1.8";
	int port = 5000;
	
	
	net_init();
	
	while (1){
	
		PAD_ScanPads();
		botones = PAD_ButtonsHeld(0);
		memset(lectura_botones, 0, 23);

		// -------- AÑADO LOS ESTADOS DE LOS BOTONES AL STRING --------- //
		
		for (int i = 0; i < 13; i++) {
		  if (botones & (1 << i)) {lectura_botones[i] = '1';}
		  else {lectura_botones[i] = '0';}
		}
		
		
		// ----------- LEO EL LEFT STICK Y LO AÑADO AL STRING ------------ //
		
		left_stick_x = PAD_StickX(0);
		sprintf(temporal_stick,"%d",left_stick_x + 500);
		lectura_botones[13] = temporal_stick[0];
		lectura_botones[14] = temporal_stick[1];
		lectura_botones[15] = temporal_stick[2];
		
		left_stick_y = PAD_StickY(0);
		sprintf(temporal_stick,"%d",left_stick_y + 500);
		lectura_botones[16] = temporal_stick[0];
		lectura_botones[17] = temporal_stick[1];
		lectura_botones[18] = temporal_stick[2];
		
		// ----------- LEO EL RIGHT STICK Y LO AÑADO AL STRING ------------ //
		
		right_stick_x = PAD_SubStickX(0);
		sprintf(temporal_stick,"%d",right_stick_x + 500);
		lectura_botones[19] = temporal_stick[0];
		lectura_botones[20] = temporal_stick[1];
		lectura_botones[21] = temporal_stick[2];
		
		right_stick_y = PAD_SubStickY(0);
		sprintf(temporal_stick,"%d",right_stick_y + 500);
		lectura_botones[22] = temporal_stick[0];
		lectura_botones[23] = temporal_stick[1];
		lectura_botones[24] = temporal_stick[2];
	
		
		
		// ------- IMPRIMO EL ESTADO FINAL DEL STRING EN PANTALLA ------------ //
		//printf(lectura_botones);
		sprintf(request, "GET /set_joystick?joystick=%s HTTP/1.1\r\n\r\n", lectura_botones);
		char *getRequest = request;
		printf("\n");


		// -------------- SI PRESIONO START, Y, R AL MISMO TIMPO SALGO DEL PROGRAMA --------------- //
		if((lectura_botones[12] == '1') & (lectura_botones[11] == '1') & (lectura_botones[5] == '1')){
			break;
		}
	
	
	
	
	
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

	

		if (sock == INVALID_SOCKET) {
			printf("socket error\n");
		} 
		s32 bytesTransferred = net_send(sock, getRequest, strlen(getRequest), 0);
		if (bytesTransferred < 0) {
			printf("[err] Failed to send the GET request.\n");
			return -4;
		}
		printf("send \n");
		net_close(sock);
		//sleep(10);
	}

	//printf("local : %d \n",connectSocket_ip(sock));
	
	return 0;
}
