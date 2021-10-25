#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int ag, char **av)
{
    char *filename = "data.txt";
    int fd;
    pid_t pid;
    struct flock filelock;

    pid = fork();
    if (pid == 0)
    {
        printf("자식 : \"부모 프로세스를 잠시 기다리겠습니다.\"\n");
        sleep(1);

        printf("자식 : \"파일 내용 수정할거임\"\n");
        fd = open(filename, O_RDWR|O_CREAT, 0666);
        filelock.l_type = F_WRLCK;
        filelock.l_whence = SEEK_SET;
        filelock.l_start = 0;
        filelock.l_len = 4;

        if (fcntl(fd,F_SETLKW, &filelock) == -1)
        {
            printf("자식 : 레코드 잠금에 실패해서 hello를 쓰지 못했다.");
        } else {
            write(fd, av[1], 5);
            printf("자식 : hello 쓰기 완료\n");
        }

        filelock.l_type = F_WRLCK;
        filelock.l_whence = SEEK_SET;
        filelock.l_start = 6;
        filelock.l_len = 7;

        if (fcntl(fd,F_SETLKW, &filelock) == -1)
        {
            printf("자식 : 레코드 잠금에 실패해서 world를 쓰지 못했다.");
        } else {
            write(fd, "world", 5);
            printf("자식 : world 쓰기 완료\n");
        }
        close(fd);
    } else if (pid == -1)
        printf("자식 프로세스 생성 실패\n");
    else
    {
        printf("부모 : 파일을 열고 레코드를 잠금하겠습니다.\n");
        fd = open(filename, O_RDWR, 0666);
        filelock.l_type = F_RDLCK;
        filelock.l_whence = SEEK_SET;
        filelock.l_start = 6;
        filelock.l_len = 7;

        if (fcntl(fd, F_SETLK, &filelock) != -1)
        {
            printf("부모 : 잠금에 성공했으며, 3초간 대기합니다\n");
            sleep(3);
        }
        close(fd);
    }
}