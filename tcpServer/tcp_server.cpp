#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT 5100

int main(int argc, char **argv)
{
    int ssock, csock; //fd
    socklen_t clen;
    int n;
    struct sockaddr_in servaddr, cliaddr;
    char mesg[BUFSIZ];

    if((ssock = socket(AF_INET, SOCK_STREAM,0)) < 0) //AF_INET = 인터넷 SOCK_STREAM tcp 0 : 키보드 1 모니터 2 에러
    {
        perror ("socket()");    //printf 와 같은 기능(버퍼를 안거침)
        return -1;
    }
int option = 1;
setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));   //바로 재사용

memset(&servaddr, 0, sizeof(servaddr));     //servaddr 주소체개 
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr =htonl(INADDR_ANY);    //all local ip 할당
servaddr.sin_port = htons(TCP_PORT);            //프로세스에 포트 변호 할당 

if(bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)     //socket 정보를 os에 알려줌
{
    perror ("bind()");
    return -1;
}

if(listen(ssock, 8) < 0)            //버퍼역활 client 수 8개
{
    perror ("listen()");
    return -1;
}

clen = sizeof(cliaddr);
do
{
    csock = accept(ssock, (struct sockaddr *)&cliaddr, &clen);          //함수 실행 client가 접속할때 까지 대기중 client정보가 넘어오고 cliaddr저장
    
    inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFSIZ);
    printf("Client is connected : %s\n", mesg);

    if((n = read(csock, mesg, BUFSIZ)) <= 0)
        perror("read()");

    printf("Received data : %s", mesg);

    if (write(csock, mesg, n) <= 0)
        perror("write()");

    close(csock);
} while (strncmp(mesg, "q",1));

close(ssock);

return 0;

}