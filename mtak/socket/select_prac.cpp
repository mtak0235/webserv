#include <sys/select.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 30

int main(int ag, char **av)
{
    fd_set reads;
    fd_set temp;
    struct  timeval timeout;
    int result;
    char buf[BUF_SIZE];
    int str_len;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while (true)
    {
        temp = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temp, 0, 0, &timeout);
        if (result == -1)
        {
            std::cout << "select err" << std::endl;
            break;
        }
        else if (result == 0)
        {
            std::cout << "timeout err" << std::endl;
        }
        else
        {
            {
                if (FD_ISSET(0, &temp))
                {
                    str_len = read(0, (void *)buf, BUF_SIZE);
                    buf[str_len] = 0;
                    std::cout << "msg from console :" << buf << std::endl;
                }
            }
        }
    }
    return 0;
}