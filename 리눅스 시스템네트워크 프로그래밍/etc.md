## Index
### 이론
* [restrict 키워드](#restrict-키워드)
* [man](#man명령어)
* [파일 디스크립터](#파일-디스크립터)
* [ID](#id)
* [시그널](#시그널)
* [IPC](#ipc)
### 메소드
* [system](#system)
* [exit](#exit)
* [kill](#kill)
* [sleep](#sleep)
* [pause](#pause)
* [getpid](#getpid)
* [getppid](#getppid)
* [wait](#wait)
* [waitpid](#waitpid)
* [errno](#errno)
* [perror](#perror)
* [strerror](#strerror)
* [dprintf](#dprintf)
* [open](#open)
* [fcntl](#fcntl)
* [signal](#signal)
* [sigaction](#sigaction)

## restrict 키워드
C 문법이며 C++에서는 지원하지 않는다.
해당 포인터를 참조하는 객체가 하나임을 보장한다는 제한자이다.

## man명령어
man (Manual)
특정 명령이나 자원들의 메뉴얼을 출력해주는 명령어이다.  
총 9개의 영역으로 이뤄진다.  
### man [num] 찾고싶은 명령어  
| Section | description |
| :---: | :--- |
| 1 | 명령어(shell commands)"나 '실행 가능한 프로그램('Executable programs)|
| 2 | system call|
| 3 | c언어의 라이브러리 함수(코드들)|
| 4 | 장치나 특수파일, 드라이브|
| 5 | 환경설정 파일|
| 6 | 게임|
| 7 | 리눅스의 파일 표준이나 규칙, 프로토콜, 시그널|
| 8 | 관리자들이 사용하는 명령어나 데몬|
| 9 | 커널 루틴(프로그램 실행동안 계속 사용되는 코드들)과 커널 프로그래밍|

| Section | description |
| :---: | :--- |
| 1 | Indicates user commands and daemons. |
| 2 | Indicates system calls and kernel services. |
| 3 | Indicates subroutines. |
| 4 | Indicates special files, device drivers, and hardware. |
| 5 | Indicates configuration files. |
| 6 | Indicates games. |
| 7 | Indicates miscellaneous commands. |
| 8 | Indicates administrative commands and daemons. |


## 파일 디스크립터
| no | name | POSIX | stream |
| :---: | :---: | :---: | :---: | 
| 0 | standard input	| STDIN_FILENO | stdin|
| 1 | standard output	| STDOUT_FILENO| stdout|
| 2 | standard error	| STDERR_FILENO| stderr|

## ID
사용자, 권한등을 식별하기위해 OS에서 지정하는 번호
| ID | mean | 설명 |
|:---:|:---:|:---|
|UID | User		| 사용자 |
|GID | Group		| 사용자 그룹 |
|PID | Process          | 프로세스 |
|PPID| Parent Process   | 부모의 프로세스 |
|RUID| Real User        | 프로세스를 시작한 실제 사용자 |
|SUID| Saved set User   | ID 임시 저장, 원래사용자로 복귀 |
|EUID| Effective User   | 유효 사용자 ID |
|PGID| Process Group    | 하나 이상의 프로세스 (0 : 그룹 리더)|
|SID | Session          | 하나 이상의 프로세스 |
## 시그널 
``` 
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
13) SIGPIPE     14) SIGALRM     15) SIGTERM     16) SIGSTKFLT
17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU
25) SIGXFSZ     26) SIGVTALRM   27) SIGPROF     28) SIGWINCH
29) SIGIO       30) SIGPWR      31) SIGSYS      34) SIGRTMIN
35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4
39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12
47) SIGRTMIN+13 48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14
51) SIGRTMAX-13 52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10
55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6
59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX
```

## IPC
IPC(Inter Process Communication)
프로세스간 데이터를 주고 받는 행위 또는 그에 대한 방법이나 경로
</br> Anonymous Pipe, Named Pipe, Message Queue, Shared Memory, Memory Map, Socket, Semaphore 방법이 존재한다.

## system
### int system( const char *str )
**Parametters**
- `const char *str` : 셸에서 실행할 명령

**Return Value**
- 실행 결과를 반환한다. 
- `other`	: 프로그램 종료 후 반환받은 값
- `127`	: 자식프로세스에서 셸 실행 불가
- `0`	: 셸 사용 불가
- `-1`	: 명령 실패 (fork 불가, errno 설정됨)

**Description**
</br> 전달받은 문자열을 셸에서 실행한다.
</br> 부모프로세스는 실행이 정지되며, 시그널이 블로킹된다.


## exit
### void exit( int status )
**Parametters**
- `int status`	: 함수 종료 시 전달할 값
	- `EXIT_SUCCESS`	: 0 정상 종료
	- `EXIT_FAILURE`	: 1 비정상 종료

**Description**
</br> 프로그램을 종료한다.
</br> 종료시 버퍼를 모두 삭제하고 파일디스크립터를 모두 닫는다.


## kill
### int kill(pid_t pid, int sig)
**Parametters**
- `pid_t pid`	: 전송 대상 프로세스
	- `양수`	: 지정한 프로세스
	- `0`		: 같은 그룹의 프로세스
	- `-1`		: 전송할 수 있는 모든 프로세스
	- `음수`	: 절대값 프로세스가 속한 그룹 프로세스
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
</br> 대상 프로세스에 종료 시그널 전송


## sleep
### unsigned int sleep(unsigned int seconds)
**Parametters**
- `unsigned inst seconds`	: 대기 시간

**Return Value**
- `orthr` : 대기 중단으로 인해 남은 대기시간
- `0`	: 대기 완료

**Description**
</br> 지정된 시간 또는 신호가 도착할 때까지 Sleep 상태가 된다.


## pause
### int pause(void)

**Return Value**
- `-1`	: 항상 -1 반환

**Description**
</br> 시그널 수신 때 까지 프로세스 대기


## getpid
### pid_t getpid(void)

**Return Value**
- 현재 프로세스의 ID

**Description**
</br> 현재프로세스의 ID값을 얻는 함수
</br>항상 실행된다.


## getppid
### pid_t getppid(void)

**Return Value**
- 부모 프로세스의 ID

**Description**
</br> 부모프로세스의 ID값을 얻는 함수
</br>항상 실행된다.


## wait
### pid_t wait(int *statloc)
**Parametters**
- `int *statloc`	: 자식의 실행 결과를 매크로를 통해 알 수 있음 (정상종료 / 비정상종료)
  - 메크로 목록
  - `WIFEXITED(statloc)`	: 정상적으로 종료되었다면 non-zero
  - `WEXITSTATUS(statloc)`: 정상종료 시 반환값을 반환
  - `WIFSIGNALED(statloc)`: 신호로 인한 종료라면 참 반환
  - `WTERMSIG(statloc)`	: 종료하게한 신호의 번호를 반환
  - `WIFSTOPPED(statloc)`	: 정지되어있다면 참 반환
  - `WSTOPSIG(statloc)`	: 정지하게한 신호의 번호를 반환

**Return Value**
- `other`	: 프로세스 ID
- `-1` : 에러 (ECHILD 자식 없음, EINTR 시스템 콜 인터럽트)

**Description**
</br> 자식프로세스의 종료시 까지 대기
</br> 시그널을 받는경우
</br> 자식프로세스가 이미 종료된 상태인 경우
</br> 자식프로세스가 없는 경우
</br> 위의 네가지 경우 반환되어 함수가 종료된다.


## waitpid
### pid_t waitpid(pid_t pid, int *statloc, int options);
**Parametters**
- `pid_t pid`	: 대기할 자식 프로세스
  - `양수`	: 지정한 프로세스
  - `0`		: 같은 그룹의 프로세스
  - `-1`		: 임의의 자식 프로세스
  - `음수`	: 절대값 프로세스가 속한 그룹 프로세스
- `int *statloc`	: 자식의 실행 결과를 매크로를 통해 알 수 있음 (정상종료 / 비정상종료)
  - `WIFEXITED(statloc)`	: 정상적으로 종료되었다면 non-zero
  - `WEXITSTATUS(statloc)`: 정상종료 시 반환값을 반환
  - `WIFSIGNALED(statloc)`: 신호로 인한 종료라면 참 반환
  - `WTERMSIG(statloc)`	: 종료하게한 신호의 번호를 반환
  - `WIFSTOPPED(statloc)`	: 정지되어있다면 참 반환
  - `WSTOPSIG(statloc)`	: 정지하게한 신호의 번호를 반환

- `int options`	: 실행 시 사용할 옵션
  - `WCONTINUED`	: 중단되었다가 재개된 자식 프로세스의 상태를 받음
  - `WNOHANG`		: 호출자는 차단되지 않고 반환값으로 0을 받음
  - `WUNTRACED`	: 중단된 자식 프로세스의 상태를 받음
  - `0`	: Wait함수와 동일하게 실행됨

**Return Value**
- `other`	: 프로세스 ID
- `0`	: 이용할 수 있는 자식이 없
- `-1`	: 에러 (ECHILD 자식 없음, EINTR 시스템 콜 인터럽트)

**Description**
</br> 자식 프로세스를 기다린다.
</br> 옵션에따라 차단을 방지하거나 자세한 설정이 가능하다.


## errno
**Description**
</br> 에러 발생시 발생 에러코드를 저장하는 전역변수
</br> thread마다 따로리되어 멀티스레드에 안전하다.
</br> 0 이라면 정상 실행됨을 의미한다.


## strerror
### char *strerror(int errnum)
**Parametters**
- `int errnum`	: 에러번호

**Return Value**
- 에러 메시지
- 에러번호에 해당하는 메시지를 리턴한다.
- `NULL`	: 해당하는 메시지 없음
- `"Unknown error nnn"`	: 해당하는 메시지 없음
 
**Description**
</br> 해당하는 errnum의 문자열을 반환한다.


## perror
### void perror(const char* str)
**Parametters**
- `const char* str` : 에러 문자열 이전에 출력할 문자열

**Description**
</br> 전역변수 errno의 값을 해석하여 해당하는 에러메시지를 표준 에러 출력 스트림에 출력한다.


## strerror_r
### int strerror_r(int errnum, char *strerrbuf, size_t buflen)
**Parametters**
- `int errnum`	: 에러 변호 
- `char *strerrbuf`	: 에러메시지를 저장할 버퍼
- `size_t buflen`	: 버퍼의 길이

**Return Value**
- `0`	: 성공
- `-1`	: 실패

**Description**
</br> 시스템 에러 메시지를 문자열로 반환

## dprintf
### int dprintf( int fd, const char *format, ...)
**Description**
</br> 일반적인 출력문장에 저수준의 출력 문장을 추가한형태

## open
### int open (const char *filename, int FLAGS[, mode_t mode])
**Parametters**
- `const char *filename`	: 대상 파일 이름
- `int flags`	: 파일 열기 옵션 비트 or '|' 연산자로 연결 가능
  - | 옵션 | 설명 |
	| :---: | :--- |
	| O_RDONLY	| 읽기 전용 |
	| O_WRONLY	| 쓰기 전용 |
	| O_RDWR	| 읽기 쓰기 |
	| O_CREAT	| 해당 파일이 없으면 생성 |
	| O_EXCL	| 이미 파일이 있으면 열지 않음, O_CREAT와 함께 사용 |
	| O_TRUNC	| 기존 파일 내용을 모두 삭제 |
	| O_APPEND	| open 후에 쓰기 포인터가 파일 끝에 위치 |
	| O_NOCITTY	| 터미널 프로그램의 제어 터미널로 할당하지 않음 |
	| O_NONBLOCK	| 읽을 내용이 없으면 바로 복귀 |
	| O_SYNC	| 쓰기가 완료될 때까지 대기 |

- `mode_t mode`	: 파일 접근 권한 ex) 0644, 0777

**Return Value**
- `other`	: 파일디스크립터 값
- `-1`	: 실패

**Description**
</br> 파일 열기 함수

## fcntl
### int fcntl(int fd, int cmd, ... )
**Parametters**
- `int fd`	: 대상 파일 디스크립터
- `int cmd`	: 실행할 명령
  - F_SETFD
  - FD_CLOEXEC
  - F_DUPFD : 파일 디스크립터 복사
F_GETFD : 파일 디스크립터와 연관된 플래그 읽기
F_SETFD : 파일 디스크립터에 플래그 설정
F_GETFL : 열린 파일에 대한 플래그 읽기
F_SETFL : 열린 파일에 대한 플래그 설정
F_GETLK : 파일 잠금 확인
F_SETLK : 파일 잠금 및 해제
F_GETOWN : 프로세스 또는 프로세스 그룹 아이디 읽기
F_SETOWN : 프로세스 또는 프로세스 그룹 아이디 설정

**Return Value**
- `-1`	: 실패

**Description**
</br> 열린 파일에 대해 제어함


## signal
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>


## sigaction
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>


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