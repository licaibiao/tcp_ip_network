#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s $RCFBUFSIZE\n", argv[0]);
        goto error;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("create socket error=%d(%s)!!!\n", errno, strerror(errno));
        goto error;
    }

    // 查看系统默认的socket接收缓冲区大小
    int defRcvBufSize = -1;
    socklen_t optlen = sizeof(defRcvBufSize);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &defRcvBufSize, &optlen) < 0)
    {
        printf("getsockopt error=%d(%s)!!!\n", errno, strerror(errno));
        goto error;
    }
    printf("OS default udp socket recv buff size is: %d\n", defRcvBufSize);

    // 按照执行参数设置UDP SOCKET接收缓冲区大小
    int rcvBufSize = atoi(argv[1]);
    if (rcvBufSize <= 0)
    {
        printf("rcvBufSize(%d) <= 0, error!!!\n", rcvBufSize);
        goto error;
    }
    printf("you want to set udp socket recv buff size to %d\n", rcvBufSize);
    optlen = sizeof(rcvBufSize);
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvBufSize, optlen) < 0)
    {
        printf("setsockopt error=%d(%s)!!!\n", errno, strerror(errno));
        goto error;
    }
    printf("set udp socket(%d) recv buff size to %d OK!!!\n", sockfd, rcvBufSize);

    // 查看当前UDP SOCKET接收缓冲区大小
    int curRcvBufSize = -1;
    optlen = sizeof(curRcvBufSize);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &curRcvBufSize, &optlen) < 0)
    {
        printf("getsockopt error=%d(%s)!!!\n", errno, strerror(errno));
        goto error;
    }
    printf("OS current udp socket(%d) recv buff size is: %d\n",sockfd,curRcvBufSize);

    close(sockfd);

    exit(0);

error:
    if (sockfd >= 0)
        close(sockfd);
    exit(1);
}
