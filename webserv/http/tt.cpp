#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

int main()
{
	char *argv[] = {"/Users/mtak/Desktop/check/webserv/www/bin",  NULL};
	char *envp[] = {"REQUEST_METHOD=GET", "SERVER_PROTOCOL=HTTP/1.1", "PATH_INFO=/i", NULL};

	char *a = (char*)malloc(16);

	pid_t pid;

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);

	write(fdIn, "qwer", 4);
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve("./cgi_tester", argv, envp);
	}
	else
	{
		char buf[1024] = {0, };
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		int ret = read(fdOut, buf, 1024);
		buf[ret] = 0;
		printf("%s\n", buf);
	}
	return 0;
}

/* 넘기는 데이터가 없는 cgi get 요청
GET /test_cgi/index.php HTTP/1.1
Host: localhost:8081
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="89", "Chromium";v="89", ";Not A Brand";v="99"
sec-ch-ua-mobile: ?0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*\/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:8081/test_cgi/
Accept-Encoding: gzip, deflate, br
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7
Cookie: SL_G_WPT_TO=en; SL_GWPT_Show_Hide_tmp=undefined; SL_wptGlobTipTmp=undefined

*/

/*넘기는 데이터가 있는  cgi post 요청
POST /test_cgi/post/action.php HTTP/1.1
Host: localhost:8081
Connection: keep-alive
Content-Length: 20
Cache-Control: max-age=0
sec-ch-ua: "Google Chrome";v="89", "Chromium";v="89", ";Not A Brand";v="99"
sec-ch-ua-mobile: ?0
Upgrade-Insecure-Requests: 1
Origin: http://localhost:8081
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*\/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:8081/test_cgi/post/form.html
Accept-Encoding: gzip, deflate, br
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7
Cookie: SL_G_WPT_TO=en; SL_GWPT_Show_Hide_tmp=undefined; SL_wptGlobTipTmp=undefined

name=glad&email=mtak
*/
