
사용자, 권한등을 식별하기위해 OS에서 지정하는 번호
| ID | mean | 설명 |
|:---:|:---:|:---|
|UID | User				| 사용자 |
|GID | Group			| 사용자 그룹 |
|PID | Process          | 프로세스 |
|PPID| Parent Process   | 부모의 프로세스 |
|RUID| Real User        | 프로세스를 시작한 실제 사용자 |
|SUID| Saved set User   | ID 임시 저장, 원래사용자로 복귀 |
|EUID| Effective User   | 유효 사용자 ID |
|PGID| Process Group    | 하나 이상의 프로세스 |
|SID | Session          | 하나 이상의 프로세스 |
||||

## system
### int system( const char *str )
**Parametters**
- `const char *str` : 셸에서 실행할 명령

**Return Value**
- 실행 결과를 반환한다. 
- `0`	: 셸 사용 불가
- `-1`	: 명령 실패 (fork 불가)
- `127`	: 자식프로세스에서 셸 실행 불가
- `other`	: 프로그램 종료 후 반환받은 값

**Description**
</br>전달받은 문자열을 셸에서 실행한다.


## exit
### void exit( int status )
**Parametters**
- int status	: 함수 종료 시 전달할 값
	- `EXIT_SUCCESS`	: 0 정상 종료
	- `EXIT_FAILURE`	: 1 비정상 종료

**Description**
</br> 프로그램을 종료한다.
</br> 종료시 버퍼를 모두 삭제하고 파일디스크립터를 모두 닫는다.


## kill
### int kill(pid_t pid, int sig)
**Parametters**
- `pid_t pid`	: 대상 프로세스
	- `양수`	: 지정한 프로세스에 전송
	- `0`		: 같은 그룹의 프로세스에 전송
	- `-1`		: 전송할 수 있는 모든 프로세스에 전송
	- `음수`	: 절대값 프로세스가 속한 그룹 프로세스에 전송
- `int sig`		: 종료 시그널 번호
	```
		 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
		 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
		 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
		13) SIGPIPE     14) SIGALRM     15) SIGTERM     17) SIGCHLD
		18) SIGCONT     19) SIGSTOP     20) SIGTSTP     21) SIGTTIN
		22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
	```

**Return Value**
- 시그널 전송 결과값 반환
- `0`	: 성공
- `-1`	: 실패

**Description**
</br> 종료 시그널 대상 프로세스에 전송


```cpp
	int kill(pid_t pid, int sig)
	/*!
	* @brief	종료 시그널 대상 프로세스에 전송
	* @details	
	* @param[in]	int pid	대상 프로세스
					0		같은 그룹에 있는 프로세스에 전송
					-1		전송할 수 있는 모든 프로세스에 전송
					양수	지정한 프로세스에 전송
					음수	절대값 프로세스 그룹이 속한 프로세스에 전송
	* @param[in]	int	sig		시그널 번호
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
13) SIGPIPE     14) SIGALRM     15) SIGTERM     17) SIGCHLD
18) SIGCONT     19) SIGSTOP     20) SIGTSTP     21) SIGTTIN
22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
	* @return		시그널 전송 결과값 반환
	* @retval		0	성공
	* @retval		-1	실패 
	*/
```
sleep()
pause()


getpid()
getppid()
waitpid()

open()
fcntl()

perror()
strerror_r()
dprintf()

sigaction()



***

```cpp
	/*!
	* @brief	
	* @details	
	* @param[in]	
	* @return	
	* @retval	
	*/
```

## function_name
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>

## trash
***
```cpp
int system( const char *str )
	/*!
	* @brief 		
	* @details		해당 프로세스가 종료될 때까지 대기한다.
	* @param[in]	const char * str	쉘에서 실행할 명령어
	* @return		실행 결과를 리턴한다.
	* @retval		0	셸 사용 불가
	* @retval		-1	명령 실패 (fork 불가)
	* @retval 		127	자식프로세스에서 셸 실행 불가
	* @retval		other	프로그램이 반환한 값 
	*/
```

```cpp
void exit( int status )
	/*!
	* @brief		프로그램을 종료한다.
	* @details		버퍼를 모두 삭제하고 열린 파일을 모두 닫는다.
	* @param[in]	int status	함수 종료시 전달할 값
					EXIT_SUCCESS 0	정상 종료
					EXIT_FAILURE 1	비정상 종료
	*/
```