#include    "unp.h"

/*
struct sockaddr_in

functions:
socket()
htons()
inet_pton()
connect()
read()
fputs()

*/
int
main(int argc, char **argv)
{
    int                    sockfd, n;
    char                recvline[MAXLINE + 1];
    struct sockaddr_in    servaddr;
    struct sockaddr_in    clientAddr;
    socklen_t size=sizeof(clientAddr);

    if (argc != 2)
        err_quit("usage: a.out <IPaddress>");

    //Create a socket
    //AF_INET: Internet
    //SOCK_STREAM: stream type
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");
    //bzero(&clientAddr,sizeof(clientAddr));
    //clientAddr.sin_family=AF_INET;
    //clientAddr.sin_addr.s_addr=htons(INADDR_ANY);//get host address automatically
    //clientAddr.sin_port=htons(0);//random allocate a port
    //Bind(sockfd,&clientAddr,sizeof(clientAddr));

    //configure a servaddr_in object: sin_family, sin_port, sin_addr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(13);    /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    //connect to the daytime server
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");
    getsockname(sockfd,(SA*) &clientAddr,&size);
    printf("connected to server. local-%s:%d\n\r",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;    /* null terminate */
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error");
    }
    if (n < 0)
        err_sys("read error");

    exit(0);
}
