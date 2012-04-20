#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "defs.h"
#include "libs.h"
#include "string.h"
    
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in addr = {AF_INET, htons(2233), htonl(0)};

    memset(&addr.sin_zero, 0, 8);

    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) 
        goto error;
    if (-1 == bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)))
        goto error;
    if (-1 == listen(sockfd, 10))
        goto error;

    while (1) {
        int new_sockfd = accept(sockfd, NULL, NULL);
        char msg[20] = "Server: Hi.";
        send(new_sockfd, msg, strlen(msg) + 1, 0);
        close(new_sockfd);
    }

    close(sockfd);
    return 0;

error:
    logE("there's error");
    return 1;
}
