#include<fcntl.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<netinet/in.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/wait.h>

void SigCatcher(int n)
{
pid_t pid;
int stat;
//pid = waitpid(-1,&stat,WNOHANG);
  wait3(NULL,WNOHANG,NULL);
}
void exitSysWithError(const char *call){
	fprintf(stderr, "Syscall %s failed with errno= %d: %s\n", call, errno,
	 strerror(errno));
	exit(-1);
}
void exitWithError(){
	printf("Please provide valid arguments\n");
	exit(-1);
}
void tcpserver(int port,struct sockaddr_in ina){
	printf("TCP SERVER\n");
	pid_t childpid;
	int sock;
	char buff[100]={'\0'};
	struct sockaddr_in cli;
	int s = socket(AF_INET, SOCK_STREAM,0);
	if (s == -1){
		printf("Socket connection failed\n");
		exitSysWithError("socket()");
	}
	int length = sizeof(ina);
	int b = bind(s, (struct sockaddr *)&ina , sizeof(ina));
	if(b == -1){
		printf("binding fails\n");
		exitSysWithError("bind()");
	}
	//printf("connection from port:%d\n",ntohs(ina.sin_port));
	int l = listen(s,5);
	if (l==-1){
		printf("listen fails\n");
		exitSysWithError("listen()");
	}
	getsockname(s,(struct sockaddr *)&ina, &length);
	if (port == 0)
		printf("Listening on port: %d\n",ntohs(ina.sin_port));
	signal(SIGCHLD,SigCatcher);
	//signal(SIGCHLD, SIG_IGN);
	while(1){
	int len  = sizeof(cli);
	sock = accept(s, (struct sockaddr *)&cli , &len);
	if (sock <0){
		printf("server accept failed\n");
	}
	if ((childpid = fork()) == 0){
		printf ("%s\n","Child created for dealing with client requests");
		close(s);

		while(1){
		int r = read(sock, (char*)&buff, sizeof(buff));
		if(r<1){
		//printf("read error\n");
		//exitSysWithError("read()");
		}
		buff[r]='\0';
		char *cmd = (char *)&buff;
		char stuff[1024]={'\0'};
		struct utsname buff1;
		errno =0; 
   		if(uname(&buff1)!=0) 
   		{ 
      			perror("uname doesn't return 0, so there is an error"); 
      			exitSysWithError("uname()"); 
   		} 
		if (strstr(cmd, "s") != NULL|| strstr(cmd,"kernel-name")!= NULL ||strstr(cmd,"-s") != NULL || strstr(cmd, "--kernel-name") !=NULL)
			{
				strcpy(stuff, buff1.sysname);
				printf("systemName=%s\n",buff1.sysname);
			}
		if (strstr(cmd, "n") != NULL|| strstr(cmd,"nodename")!= NULL ||strstr(cmd,"-n") != NULL || strstr(cmd, "--nodename") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.nodename); 
				}
				else{
				strcpy(stuff, buff1.nodename);
				printf("NodeName=%s\n",buff1.nodename);}
			}
		if (strstr(cmd, "r") != NULL|| strstr(cmd,"kernel-release")!= NULL ||strstr(cmd,"-r") != NULL || strstr(cmd, "--kernel-release") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.release); 
					}
				else{
				strcpy(stuff, buff1.release);
				printf("Release=%s\n",buff1.release);}
			}
		if (strstr(cmd, "v") != NULL|| strstr(cmd,"kernel-version")!= NULL ||strstr(cmd,"-v") != NULL || strstr(cmd, "--kernel-version") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.version); 
				}
				else{
				strcpy(stuff, buff1.version);
				printf("version=%s\n",buff1.version);}
			}
		if (strstr(cmd, "m") != NULL|| strstr(cmd,"--machine")!= NULL ||strstr(cmd,"-m") != NULL || strstr(cmd, "--machine") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.machine); 
					}
				else{
				strcpy(stuff, buff1.machine);
				printf("machine=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "p") != NULL|| strstr(cmd,"processor")!= NULL ||strstr(cmd,"-p") != NULL || strstr(cmd, "--processor") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("processor=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "i") != NULL|| strstr(cmd,"--hardware-platform")!= NULL ||strstr(cmd,"-i") != NULL || strstr(cmd, "--hardware-platform") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("platform=%s\n",buff1.machine);}
			}
	if ((strstr(cmd, "h") != NULL)|| (strstr(cmd,"help")!= NULL) ||
	(strstr(cmd,"-h") != NULL) || (strstr(cmd, "--help") !=NULL) ||
	 	(cmd == "h") || (cmd =="--help") || (cmd == "-h") || (cmd == "help"))
			{
			printf("in help");
			FILE *infile;
			long numbytes;
			infile = fopen("help.txt","r");
			fseek(infile, 0, SEEK_END);
			numbytes = ftell(infile);
			//printf("%ld",numbytes);
			fseek(infile, 0, SEEK_SET);
			//stuff = 
			//fread(stuff, sizeof(char), numbytes, infile);
			fread(stuff,1000,numbytes,infile);
			fclose(infile);
			printf("Help:%s",stuff);
			//fclose(infile);
		}
		if (strstr(cmd, "o") != NULL|| strstr(cmd,"operating-system")!= NULL ||strstr(cmd,"-o") != NULL || strstr(cmd, "--operating-system") !=NULL)
			{
			char *os = "GNU-Linux";
			if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,os); 
				}
			else{
			strcpy(stuff, os);
			printf("operating system=GNU-Linux\n");}
			}
		if (strstr(cmd, "asn") != NULL|| strstr(cmd,"-asn")!= NULL || 
		strstr(cmd,"all")!= NULL || strstr(cmd,"--all")!= NULL ||
		strstr(cmd,"-a")!= NULL || strstr(cmd,"a")!= NULL)
			{
				strcpy(stuff,buff1.sysname);
				strcat(stuff," ");
				strcat(stuff,buff1.nodename);
				strcat(stuff," ");
				strcat(stuff,buff1.release);
				strcat(stuff," ");
				strcat(stuff,buff1.version);
				strcat(stuff," ");
				strcat(stuff,buff1.machine);
				printf("ALL=%s\n",stuff);
			}
		if(cmd == NULL || cmd == "")
			{
				char *novalue = "Please provide valid commands";
				strcpy(stuff, novalue);
				//printf("%s\n",stuff);
			}
		int sendval = write(sock, stuff, sizeof(stuff));
		if(sendval < 1){}
		//exitSysWithError("write()");
		exit(0);
		}
	//close(sock);
	exit(0);
	}
	close(sock);
	}
//close(s);
}

void udpserver(int port,struct sockaddr_in ina){
	struct sockaddr_in cli;
	char buffer[100]={'\0'};
	pid_t pid[50];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	int s = socket(AF_INET, SOCK_DGRAM,0);
	if (s==-1){
		printf("socket failed udp\n");
		exitSysWithError("socket()");
	}
	int b = bind (s, (struct sockaddr *)&ina, sizeof(ina));
	if(b==-1){
		printf("bindfailed\n");
		exitSysWithError("bind()");
	}
	int length = sizeof(ina);
	getsockname(s, (struct sockaddr *)&ina , &length);
	if (port == 0)
		printf("connection from port:%d\n",ntohs(ina.sin_port));
	int len;
	len = sizeof(cli);
	int n = recvfrom(s,(char *)buffer,100,0,(struct sockaddr *)&cli, &len);
	if(n<0){
		printf("received/n");
		exitSysWithError("recvfrom()");
	}
	//printf("received by client");
	buffer[n] = '\0';
	char *cmd = (char *)&buffer;
	char stuff[100]={'\0'};
	struct utsname buff1;
	errno =0; 
   	if(uname(&buff1)!=0) 
   	{ 
      		perror("uname doesn't return 0, so there is an error"); 
      		exitSysWithError("uname()"); 
   	} 
	if (strstr(cmd, "s") != NULL|| strstr(cmd,"kernel-name")!= NULL ||strstr(cmd,"-s") != NULL || strstr(cmd, "--kernel-name") !=NULL)
			{
				strcpy(stuff, buff1.sysname);
				printf("systemName=%s\n",buff1.sysname);
			}
		if (strstr(cmd, "n") != NULL|| strstr(cmd,"nodename")!= NULL ||strstr(cmd,"-n") != NULL || strstr(cmd, "--nodename") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.nodename); 
				}
				else{
				strcpy(stuff, buff1.nodename);
				printf("NodeName=%s\n",buff1.nodename);}
			}
		if (strstr(cmd, "r") != NULL|| strstr(cmd,"kernel-release")!= NULL ||strstr(cmd,"-r") != NULL || strstr(cmd, "--kernel-release") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.release); 
					}
				else{
				strcpy(stuff, buff1.release);
				printf("Release=%s\n",buff1.release);}
			}
		if (strstr(cmd, "v") != NULL|| strstr(cmd,"kernel-version")!= NULL ||strstr(cmd,"-v") != NULL || strstr(cmd, "--kernel-version") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.version); 
				}
				else{
				strcpy(stuff, buff1.version);
				printf("version=%s\n",buff1.version);}
			}
		if (strstr(cmd, "m") != NULL|| strstr(cmd,"--machine")!= NULL ||strstr(cmd,"-m") != NULL || strstr(cmd, "--machine") !=NULL)
			{
				if(stuff != NULL)
					{
					strcat(stuff," ");
					strcat(stuff,buff1.machine); 
					}
				else{
				strcpy(stuff, buff1.machine);
				printf("machine=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "p") != NULL|| strstr(cmd,"processor")!= NULL ||strstr(cmd,"-p") != NULL || strstr(cmd, "--processor") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("processor=%s\n",buff1.machine);}
			}
		if (strstr(cmd, "i") != NULL|| strstr(cmd,"--hardware-platform")!= NULL ||strstr(cmd,"-i") != NULL || strstr(cmd, "--hardware-platform") !=NULL)
			{
				if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,buff1.machine); 
				}
				else{
				strcpy(stuff, buff1.machine);
				printf("platform=%s\n",buff1.machine);}
			}
	if ((strstr(cmd, "h") != NULL)|| (strstr(cmd,"help")!= NULL) ||
	(strstr(cmd,"-h") != NULL) || (strstr(cmd, "--help") !=NULL) ||
	 	(cmd == "h") || (cmd =="--help") || (cmd == "-h") || (cmd == "help"))
			{
			printf("in help");
			FILE *infile;
			long numbytes;
			infile = fopen("help.txt","r");
			fseek(infile, 0, SEEK_END);
			numbytes = ftell(infile);
			//printf("%ld",numbytes);
			fseek(infile, 0, SEEK_SET);
			//stuff = 
			//fread(stuff, sizeof(char), numbytes, infile);
			fread(stuff,1000,numbytes,infile);
			fclose(infile);
			printf("Help:%s",stuff);
			//fclose(infile);
		}
		if (strstr(cmd, "o") != NULL|| strstr(cmd,"operating-system")!= NULL ||strstr(cmd,"-o") != NULL || strstr(cmd, "--operating-system") !=NULL)
			{
			char *os = "GNU-Linux";
			if(stuff != NULL)
				{
				strcat(stuff," ");
				strcat(stuff,os); 
				}
			else{
			strcpy(stuff, os);
			printf("operating system=GNU-Linux\n");}
			}
		if (strstr(cmd, "asn") != NULL|| strstr(cmd,"-asn")!= NULL || 
		strstr(cmd,"all")!= NULL || strstr(cmd,"--all")!= NULL ||
		strstr(cmd,"-a")!= NULL || strstr(cmd,"a")!= NULL)
			{
				strcpy(stuff,buff1.sysname);
				strcat(stuff," ");
				strcat(stuff,buff1.nodename);
				strcat(stuff," ");
				strcat(stuff,buff1.release);
				strcat(stuff," ");
				strcat(stuff,buff1.version);
				strcat(stuff," ");
				strcat(stuff,buff1.machine);
				printf("ALL=%s\n",stuff);
			}
		if(cmd == NULL || cmd == "")
			{
				char *novalue = "Please provide valid commands";
				strcpy(stuff, novalue);
				//printf("%s\n",stuff);
			}
	//pthread_mutex_unlock(&lock);
	//printf("Client requested command:%s\n",buffer);
	int sendval = sendto(s,(const char *)stuff, sizeof(stuff),MSG_CONFIRM,(const struct sockaddr *)&cli,len);
	 if (sendval < 0)
		exitSysWithError("sendto()");
	close(s);

	}

int main(int argc, char** argv)
{

	int opt;
	char *portname;
	int portnumber;
	portnumber = 0;
	
	while ((opt = getopt(argc, argv, "p:t:u:")) != -1){
		switch(opt){
		case 'p':
			for(int i=0; i<argc; i++){
				if(strcmp(argv[i],"-port") == 0)
					portnumber =(int)atol(argv[i+1]);
			}
			//printf("portnumber%d",portnumber);
			break;
		case 't':
			portname= "tcp";
			//printf("IN TCP portname%s\n",portname);
			break;
		case 'u':
			portname = "udp";
			//printf("portname%s\n",portname);
			break;
		}
	}
	//printf("portnumber outside%d\n",portnumber);
	struct servent *sp;
	struct sockaddr_in ina;
	ina.sin_family = AF_INET;
	ina.sin_addr.s_addr = htonl(INADDR_ANY);
	ina.sin_port = htons(portnumber);
	if(portname == "tcp"){
		tcpserver(portnumber,ina);
	}
	else if (portname == "udp") {
		udpserver(portnumber,ina);
	}
}
