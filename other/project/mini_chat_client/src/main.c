#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "defs.h"
#include "libs.h"
#include "string.h"

#define LISTEN_PORT 2233
#define BACKLOG     10

static unsigned int get_server_address()
{
    char buf[256];
    struct hostent *he;
    unsigned int ip;

    gethostname(buf, sizeof(buf));
    he = gethostbyname(buf);

    ip = *(unsigned int*)he->h_addr;

    //logI("length: %d", he->h_length); // print '4'
    logI("address list: %x", *(unsigned int*)he->h_addr_list[0]);
    logI("IP: %x", ip);

    return ip;
}

static void receive_data(int sockfd)
{
    int buf[256];
    int length;

    if (-1 == (length = recv(sockfd, buf, sizeof(buf), 0)))
        goto error;

    buf[length] = '\0';

    logI("receive string: %s", buf);
    logI("receive length: %d", length);

    return;

error:
    logI("receive error.");
}

int main(int argc, char **argv)
{
    int sockfd;
    int client_sockfd;

    struct sockaddr_in server_addr;

    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
        goto error;

    logI("Init socket.");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = get_server_address();

    mc_memset(&server_addr.sin_zero, 0, 8);

    if (-1 == connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
        goto error;

    logI("Connected.");

    receive_data(sockfd);

    mc_unused(argc);
    mc_unused(argv);

    return 0;

error:
    logE("there's error!. \r\n");
    return 1;
}
