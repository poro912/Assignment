<h2> Index</h2>

- [1장](#1장)
	- [이론](#이론)
		- [restrict 키워드](#restrict-키워드)
		- [POSIX](#posix)
		- [man명령어](#man명령어)
		- [파일 디스크립터](#파일-디스크립터)
		- [ID](#id)
		- [시그널](#시그널)
		- [IPC](#ipc)
	- [API](#api)
		- [system](#system)
		- [exit](#exit)
		- [kill](#kill)
		- [sleep](#sleep)
		- [pause](#pause)
		- [getpid](#getpid)
		- [getppid](#getppid)
		- [wait](#wait)
		- [waitpid](#waitpid)
		- [errno](#errno)
		- [strerror](#strerror)
		- [perror](#perror)
		- [strerror\_r](#strerror_r)
		- [dprintf](#dprintf)
		- [open](#open)
		- [close](#close)
		- [fcntl](#fcntl)
		- [signal](#signal)
	- [**Parameters**](#parameters)
	- [**Return Value**](#return-value)
		- [sigaction](#sigaction)
	- [**Parameters**](#parameters-1)
	- [**Return Value**](#return-value-1)
- [2장](#2장)
	- [이론](#이론-1)
		- [메타데이터](#메타데이터)
		- [File mode](#file-mode)
	- [API](#api-1)
		- [fopen](#fopen)
		- [fclose](#fclose)
		- [fdopen](#fdopen)
		- [fdclose](#fdclose)
		- [fread](#fread)
		- [fwrite](#fwrite)
		- [fflush](#fflush)
		- [dup](#dup)
		- [dup2](#dup2)
		- [lseek](#lseek)
- [3장](#3장)
	- [API](#api-2)
		- [memchr](#memchr)
		- [strchr](#strchr)
		- [strtok](#strtok)
- [5장](#5장)
	- [이론](#이론-2)
		- [물리메모리](#물리메모리)
		- [블록장치](#블록장치)
		- [가상메모리](#가상메모리)
		- [더티페이지](#더티페이지)
		- [스왑인](#스왑인)
		- [스왑아웃](#스왑아웃)
		- [file-backed 메모리](#file-backed-메모리)
	- [API](#api-3)
		- [ftruncate](#ftruncate)
- [6장](#6장)
	- [이론](#이론-3)
		- [TCP half close](#tcp-half-close)
		- [2MLS](#2mls)
	- [API](#api-4)
		- [readv](#readv)
		- [writev](#writev)
		- [struct timeval](#struct-timeval)
		- [struct timespec](#struct-timespec)
- [6장](#6장-1)
	- [이론](#이론-4)
		- [statefull](#statefull)
		- [stateless](#stateless)
- [9장](#9장)
	- [이론](#이론-5)
		- [0번 시그널](#0번-시그널)
		- [EINTR error](#eintr-error)
		- [좀비 프로세스](#좀비-프로세스)
		- [좀비프로세스의 해결 방법](#좀비프로세스의-해결-방법)
		- [부모 프로세스가 죽은 경우 자식 프로세스의 행동](#부모-프로세스가-죽은-경우-자식-프로세스의-행동)
		- [프로세스 그룹, PGID(Process Group ID)](#프로세스-그룹-pgidprocess-group-id)
		- [프로세스 그룹 리더](#프로세스-그룹-리더)
		- [세션, SID(Session ID)](#세션-sidsession-id)
		- [세션 리더](#세션-리더)
		- [제어 터미널](#제어-터미널)

# 1장  
## 이론  
### restrict 키워드
C 문법이며 C++에서는 지원하지 않는다.  
해당 포인터를 참조하는 객체가 하나임을 보장한다는 제한자이다.  

### POSIX 
POSIX	: portable operating system interface  
이식 가능 운영체제 인터페이스  
서로다른 UNIX OS의 공통 API를 정리하여 이식성이 높은 프로그램을 개발하기 위한 인터페이스 규격  
시스템 콜, 프로세스 환경, 파일과 디렉터리, 시스템 데이터베이스 tar 압축 포맷 등 다양한 분야가 있다.  

### man명령어
man (Manual)  
특정 명령이나 자원들의 메뉴얼을 출력해주는 명령어이다.  
총 9개의 영역으로 이뤄진다.  

```man [section num] 찾고싶은 명령어  ```
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


### 파일 디스크립터
| no | name | POSIX | stream |
| :---: | :---: | :---: | :---: | 
| 0 | standard input	| STDIN_FILENO | stdin|
| 1 | standard output	| STDOUT_FILENO| stdout|
| 2 | standard error	| STDERR_FILENO| stderr|


### ID
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


### 시그널 
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

### IPC
IPC(Inter Process Communication)  
프로세스간 데이터를 주고 받는 행위 또는 그에 대한 방법이나 경로  
Anonymous Pipe, Named Pipe, Message Queue, Shared Memory, Memory Map, Socket, Semaphore 방법이 존재한다.  


## API  
### system
	int system(const char *str)
**Parameters**
- `const char *str` : 셸에서 실행할 명령

**Return Value**
- 실행 결과를 반환한다. 
- `other`	: 프로그램 종료 후 반환받은 값
- `127`	: 자식프로세스에서 셸 실행 불가
- `0`	: 셸 사용 불가
- `-1`	: 명령 실패 (fork 불가, errno 설정됨)

**Description**  
전달받은 문자열을 셸에서 실행한다.  
부모프로세스는 실행이 정지되며, 시그널이 블로킹된다.


### exit
	void exit(int status)
**Parameters**
- `int status`	: 함수 종료 시 전달할 값
	- `EXIT_SUCCESS`	: 0 정상 종료
	- `EXIT_FAILURE`	: 1 비정상 종료

**Description**  
프로그램을 종료한다.  
종료시 버퍼를 모두 삭제하고 파일디스크립터를 모두 닫는다.


### kill
	int kill(pid_t pid, int sig)
**Parameters**
- `pid_t pid`	: 전송 대상 프로세스
	- `양수`	: 지정한 프로세스
	- `0`		: 같은 그룹의 프로세스
	- `-1`		: 전송할 수 있는 모든 프로세스
	- `음수`	: 절대값 프로세스가 속한 그룹 프로세스
- `int sig`		: 종료 시그널 번호
	```
		 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
		 2) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
		 3) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
		1)  SIGPIPE     14) SIGALRM     15) SIGTERM     17) SIGCHLD
		2)  SIGCONT     19) SIGSTOP     20) SIGTSTP     21) SIGTTIN
		3)  SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
	```

**Return Value**
- 시그널 전송 결과값 반환
- `other` : 실패 (확인 필요)
- `0`	: 성공
- `-1`	: 실패

**Description**  
대상 프로세스에 종료 시그널 전송


### sleep
	unsigned int sleep(unsigned int seconds)
**Parameters**
- `unsigned inst seconds`	: 대기 시간

**Return Value**
- `orthr` : 대기 중단으로 인해 남은 대기시간
- `0`	: 대기 완료

**Description**  
지정된 시간 또는 신호가 도착할 때까지 Sleep 상태가 된다.


### pause
	int pause(void)

**Return Value**
- `-1`	: 항상 -1 반환

**Description**  
시그널 수신 때 까지 프로세스 대기


### getpid
	pid_t getpid(void)

**Return Value**
- 현재 프로세스의 ID

**Description**  
현재프로세스의 ID값을 얻는 함수  
항상 실행된다.


### getppid
	pid_t getppid(void)

**Return Value**
- 부모 프로세스의 ID

**Description**  
부모프로세스의 ID값을 얻는 함수  
항상 실행된다.


### wait
	pid_t wait(int *statloc)
**Parameters**
- `int *statloc`	: 자식의 실행 결과를 매크로를 통해 알 수 있음 (정상종료 / 비정상종료)
  - 메크로 목록
  - `WIFEXITED(statloc)`	: 정상적으로 종료되었다면 non-zero
  - `WEXITSTATUS(statloc)`	: 정상종료 시 반환값을 반환
  - `WIFSIGNALED(statloc)`	: 신호로 인한 종료라면 참 반환
  - `WTERMSIG(statloc)`	: 종료하게한 신호의 번호를 반환
  - `WIFSTOPPED(statloc)`	: 정지되어있다면 참 반환
  - `WSTOPSIG(statloc)`	: 정지하게한 신호의 번호를 반환

**Return Value**
- `other`	: 프로세스 ID
- `-1` : 에러 (ECHILD 자식 없음, EINTR 시스템 콜 인터럽트)

**Description**  
 자식프로세스의 종료시 까지 대기  
 시그널을 받는경우  
 자식프로세스가 이미 종료된 상태인 경우  
 자식프로세스가 없는 경우  
 위의 네가지 경우 반환되어 함수가 종료된다.  


### waitpid
	pid_t waitpid(
		pid_t			pid,
		int			*statloc,
		int			options
	)
**Parameters**
- `pid_t pid`	: 대기할 자식 프로세스
  - `양수`	: 지정한 프로세스
  - `0`		: 같은 그룹의 프로세스
  - `-1`		: 임의의 자식 프로세스
  - `음수`	: 절대값 프로세스가 속한 그룹 프로세스
- `int *statloc`	: 자식의 실행 결과를 매크로를 통해 알 수 있음 (정상종료 / 비정상종료)
  - `WIFEXITED(statloc)`	: 정상적으로 종료되었다면 non-zero
  - `WEXITSTATUS(statloc)`	: 정상종료 시 반환값을 반환
  - `WIFSIGNALED(statloc)`	: 신호로 인한 종료라면 참 반환
  - `WTERMSIG(statloc)`	: 종료하게한 신호의 번호를 반환
  - `WIFSTOPPED(statloc)`	: 정지되어있다면 참 반환
  - `WSTOPSIG(statloc)`	: 정지하게한 신호의 번호를 반환

- `int options`	: 실행 시 사용할 옵션
  - `WCONTINUED`	: 중단되었다가 재개된 자식 프로세스의 상태를 받음
  - `WNOHANG`	: 호출자는 차단되지 않고 반환값으로 0을 받음
  - `WUNTRACED`	: 중단된 자식 프로세스의 상태를 받음
  - `0`	: Wait함수와 동일하게 실행됨

**Return Value**
- `other`	: 프로세스 ID
- `0`	: 이용할 수 있는 자식이 없음
- `-1`	: 에러 (ECHILD 자식 없음, EINTR 시스템 콜 인터럽트)

**Description**  
자식 프로세스를 기다린다.  
옵션에따라 차단을 방지하거나 자세한 설정이 가능하다.  


### errno
**Description**  
에러 발생시 발생 에러번호를 저장하는 전역변수  
thread마다 따로 관리되어 멀티스레드에 안전하다.  
0 이라면 정상 실행됨을 의미한다.  
요즘에는 전역 변수가 아닌 따로 관리한다. (정확한 정보 필요)  


### strerror
	char *strerror(int errnum)
**Parameters**
- `int errnum`	: 에러번호

**Return Value**
- 에러 메시지
- 에러번호에 해당하는 메시지를 리턴한다.
- `NULL`	: 해당하는 메시지 없음
- `"Unknown error nnn"`	: 해당하는 메시지 없음
 
**Description**  
해당하는 errnum의 문자열을 반환한다.


### perror
	void perror(const char *str)
**Parameters**
- `const char *str` : 에러 문자열 이전에 출력할 문자열

**Description**  
전역변수 errno의 값을 해석하여 해당하는 에러메시지를 표준 에러 출력 스트림에 출력한다.


### strerror_r
	int strerror_r(
		int			errnum, 
		char			*strerrbuf,
		size_t			buflen
	)
**Parameters**
- `int errnum`	: 에러 변호 
- `char *strerrbuf`	: 에러메시지를 저장할 버퍼
- `size_t buflen`	: 버퍼의 길이

**Return Value**
- `0`	: 성공
- `-1`	: 실패

**Description**  
시스템 에러 메시지를 문자열로 반환

### dprintf
	int dprintf(
		int			fd,
		const char		*format,
		...)  
**Description**  
일반적인 출력문장에 저수준의 출력 문장을 추가한형태

### open
```int open (const char *filename, int FLAGS[, mode_t mode])```  
**Parameters**
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
	| O_DSYNC 	| 메타 데이터를 빼고 동기화 한다. |
	| O_RSYNC 	| 읽기 작업에 대해서도 동기화된 I/O를 사용한다. |

- `mode_t mode`	: 파일 접근 권한 ex) 0644, 0777

**Return Value**
- `other`	: 파일디스크립터 값
- `-1`	: 실패

**Description**  
파일 열기 함수

### close
	int close(int fd)
**Parameters**  
- `int fd`	: 닫을 파일 디스크립터

**Return Value**  
- `0`	: 성공
- `-1`	: 에러

**Description**  
파일 디스크립터를 기반으로 파일을 닫는다.

### fcntl
	int fcntl(int fd, int cmd, ... )  
**Parameters**
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
열린 파일에 대해 제어함


### signal
```declring_function```    
**Parameters**
- 

**Return Value**
- 

**Description**

### sigaction
```declring_function```  
**Parameters**
- 

**Return Value**
- 

**Description**




# 2장  
## 이론
### 메타데이터
데이터에 대한 데이터  
많은 양의 데이터를 수집, 저장 및 분석할 수 있도록 일관된 방식으로 구조화된, 다른 데이터를 설명하는 데이터  
대량의 정보 가운데서 찾고 있는 정보를 효율적으로 찾아내서 이용하기위해 일정한 규칙에 따라 부여되는 데이터  
예) 사진의 촬영시간, 노출, 플래시 사용 여부, 해상도, 위치 등  


### File mode  
> **Warning**  
 w, w+, wb, wb+, w+b 모드는 기존 파일을 영구 삭제합니다.  

| 모드	| 설명 |
| :--	| :-- |
|  r	| 읽기</br>파일이 없는경우 실패 |
|  w	| 쓰기</br>파일이 없는경우 : 생성</br>파일이 있는경우 : 새로 만들기 |
|  a	| 추가쓰기</br>파일이 없는경우 : 생성</br>파일이 있는경우 : 끝에서부터 내용 추가  |
|  r+	| 읽기,쓰기</br>파일이 없는경우 : 생성</br>파일이 있는경우 : 덮어쓰기 |
|  w+	| 읽기,쓰기</br>파일이 없는경우 : 생성</br>파일이 있는경우 : 새로 만들기 |
|  a+	| 읽기,쓰기</br>파일이 없는경우 : 생성</br>파일이 있는경우 : 끝에서부터 내용 추가 |
|  t	| 텍스트모드 |
|  b	| 바이너리모드 |

## API  
### fopen
	FILE *fopen(
		const char		*filename,
		const char		*mode
	)
**Parameters**  
- `const char *filename`	: 파일 이름
- `const char *mode`	: 파일 열기 옵션
  
**Return Value**  
- `NULL`	: 실패
- `other`	: 파일 포인터

**Description**  
파일 이름을 기반으로 파일을 연다.  
> **Warning**  
 w, w+, wb, wb+, w+b 모드는 기존 파일을 영구 삭제합니다.  

### fclose
	int fclose(FILE *stream)
**Parameters**  
- `FILE *stream`	: 닫을 파일 스트림

**Return Value**   
- `0`	: 닫기 성공
- `EOF`	: 에러

**Description**  
파일을 닫는다.  
닫기 전 스트림과 연관된 버퍼를 모두 닫는다.  
2진 스트림이 닫히면 파일의 마지막까지 null문자로 채워진다.  

### fdopen
	FILE *fdopen(
		int			fd,
		char			*mode
	)
**Parameters**  
- `int fd`	: 파일 디스크립터
- `char *mode`	: 파일 열기 옵션
  
**Return Value**  
- `NULL`	: 실패
- `other`	: 파일 포인터

**Description**  
파일 디스크립터를 기반으로 열린 파일을 FILE 포인터로 연다.  
열기에 실패한 경우 close()를 이용해 fd를 닫아야 한다.  

### fdclose
	int fclose(
		FILE			*stream,
		int			*fdp
	)
**Parameters**  
- `FILE *stream`	: 닫을 파일 스트림
- `int *fdp`		: 반환받은 파일 기술자

**Return Value**   
- `0`	: 닫기 성공
- `EOF`	: 에러

**Description**  
파일 기술자를 제외하고 파일을 닫는다.  
스트림에 연결된 파일기술자가 없는 경우 fdp는 -1로 설정된다.  

### fread
	size_t fread(
		void			*buffer,
		size_t			size,
		size_t			count,
		FILE			*stream
	)

**Parameters**  
- `const void *buffer`	: 읽어온 데이터를 저장할 버퍼 주소
- `size_t size`		: 데이터 하나의 크기
- `size_t count`	: 읽어올 데이터 개수
- `FILE *stream`	: 데이터를 읽어올 스트림

**Return Value**  
- 읽기에 성공한 데이터의 수

**Description**  
streadm의 데이터를 buffer 에 (size * count) byte 만큼 읽어온다.  
ferror(), feof() 함수를 이용하여 읽기 오류와 파일의 끝을 확인한다.  
return 값과 count 값이 다르다면 에러가 발생한 것  

### fwrite  
	size_t fwrite (
		const void		*buffer,
		size_t			size,
		size_t			count,
		FILE			*stream
	)
**Parameters**  
- `const void *buffer`	: 저장할 데이터의 주소
- `size_t size`		: 데이터 하나의 크기
- `size_t count`	: 저장할 데이터 개수
- `FILE *stream`	: 데이터를 쓸 파일 포인터

**Return Value**  
- 파일에 실제로 저장된 데이터의 수

**Description**  
buffer의 데이터를 stream 에 (size * count) byte 만큼 씁니다.  
return 값과 count 값이 다르다면 에러

### fflush
	int fflush(FILE *stream)
**Parameters**  
- `FILE *stream`	: 대상 파일스트림

**Return Value**  
- `0`	: 성공
- `EOF`	: 에러

**Description**  
stream과 연관된 버퍼를 비웁니다.  
stream이 입력을 위해 열려있는 경우 ungetc 함수 효과를 취소합니다.  

### dup  
	int dup(int fd)
**Parameters**  
- `int fd`	: 복제할 파일 기술자

**Return Value**  
- `other`	: 파일기술자
- `-1`	: 에러

**Description**  
파일 기술자를 복제하여 반환한다.

### dup2  
	int dup2(int fd,int fd2)   
**Parameters**  
-  `int fd`	: 복제할 파일 기술자
-  `int fd2`	: 대상이 될 파일 기술자 번호

**Return Value**  
- `other`	: 파일기술자
- `-1`	: 에러

**Description**  
파일 기술자를 복제하여 반환한다.
새 파일 기술자의 값을 fd2로 지정한다.  
fd2가 이미 열려있다면 fd2를 닫은 후 복제하여 반환한다.  

### lseek
	off_t lseek(
		int			fd,
		off_t			offset,
		int			whence
	)  
**Parameters**  
- `int fd`	: 조정할 파일 디스크립터
- `off_t offset`	: 기준점으로부터 이동할 거리
- `int whence`		: 기준점
	| 기준점| 의미 |
	| :--:	| :-- |
	| SEEK_SET | (0)파일의 맨 앞 |
	| SEEK_CUR | (1)현재 SEEK 포인터 |
	| SEEK_END | (2)파일의 맨 끝 | 

**Return Value**  
- `other`	: 변환한 offset
- `-1`	: 에러

**Description**  
파일 디스크립터의 커서를 조정한다.  
현재 파일크기를 넘어 조정할 수 있다. (이 때 남은 부분은 NULL로 채워진다.)  



# 3장  
## API
### memchr
	void *memchr(
		const void 		*buf,
		int			c,
		size_t			count
	)
**Parameters**
- `const void *buf`	: 탐색 대상 메모리 블럭의 주소
- `int c`		: 찾을 값
- `size_t count`	: buf의 크기

**Return Value**  
- `other`	: 탐색결과 주소
- `NULL`	: 탐색 값 없음

**Description**  
메모리 블록에서 처음으로 일치하는 값을 찾아 주소를 반환한다.

### strchr
	char *strchr(
		const char 		*string,
		int			c
	)
**Parameters**
- `const char *string`	: 탐색 대상 문자열
- `int c`		: 찾을 값

**Return Value**
- `other`	: 탐색결과 주소
- `NULL`	: 탐색 값 없음

**Description**  
문자열에서 처음으로 일치하는 값을 찾아 주소를 반환한다.

### strtok
	char *strtok(
		char 			*string1,
		const char 		*string2
	)
**Parameters**
- `char *string1`	: 대상 문자열
- `const char *string2`	: 구분자 리스트

**Return Value**
- `other`	: 탐색결과 주소
- `NULL`	: 탐색 값 없음

**Description**  
구분자 리스트에 있는 값 중 하나가 string1에서 일치하면 구분자를 '\0'로 치환한다.  
최초 일치하는 하나의 구분자만 변환하기때문에 NULL 반환시까지 반복 수행해야 한다.  
원본 문자열은 검색을 통해 값이 바뀌기 때문에 유의해야한다.  



# 5장
## 이론
### 물리메모리

### 블록장치

### 가상메모리
물리 메모리보다 큰 프로그램을 적재하기 위해 물리 저장장치의 일부를 물리메모리 처럼 사용 하는 기법  
메모리 관리의 단순화, 메모리 용량 및 안정성 보장 등의 장점이 있다.  
이 때 저장되는 단위는 페이지이다.  


### 더티페이지
페이지 캐시의 내용이 저장장치의 내용과 다른 페이지  
읽기 쓰기 작업이 일어나는 경우 pagecache에 임시 저장된다.  
이 때 한번에 하드에 저장되는 것이 아닌 메모리에 유지되다 조건에 만족하면 파일에 쓰게 된다.  
메모리에 유지된 상태에서 저장장치의 내용과 다른 부분을 더티페이지라 한다.  


### 스왑인
페이지를 가상 메모리에서 물리 메모리로 가져오는 것  


### 스왑아웃
페이지를 물리 메모리에서 가상 메모리로 내보내는 것  


### file-backed 메모리
페이지 중 특정 경로에 이미 원본(백업본)이 존재하는 경우  
페이지 아웃 발생시 물리메모리에 있던 페이지는 백업을 위해 블록장치로 내려간다.  
읽기 전용 파일의 경우 이미 사본이 존재하기 때문에 페이지 아웃이 발생할 필요가 없다.  
위와같이 이미 파일의 백업본이 물리장치에 존재하는 경우를 file-backed 메모리라 부른다.  
실행파일, 동적라이브러리, 공용 데이터, 공용 이미지 파일 등이 이에 해당된다.  

## API
### ftruncate
	#include <unistd.h>
	#include <sys/types.h>
	int ftruncate(
		int			fd,
		off_t 			length
	)
**Parameters**
- `int fd`		: 크기를 변경할 파일 디스크립터
- `off_t length`	: 변경할 길이

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno설정

**Description**  
파일의 크기를 조정한다.
쓰기모드로 열린 fd의 파일 크기를 length 크기로 변경한다.  
length가 파일 크기보다 작으면 length 만큼의 크기로 파일 뒤쪽을 자른다.  
length가 파일 크기보다 크면 파일 크기를 변경하고 0x00으로 채운다.  


# 6장
## 이론
### TCP half close
파일 송신 중 클라이언트 측 프로그램은 데이터를 얼마나 받아야 할지 모르는 상황에 처한다.  
이 경우 EOF를 보내 연결을 끊어 데이터의 송신완료를 선언할 수 있다.  
하지만 해당 방법은 상대방이 전송하는 데이터를 다 받기 전에 연결이 종료될 가능성을 내포한다.  
때문에 서버의 출력 스트림만 종료하여 EOF신호를 보내 안정적으로 전송을 완료하기 위해 사용하는 기법이다.   

### 2MLS
MLS (Maximum Segment Lifetime)  
세그먼트의 최대 수명  
서그먼트가 네트워크 내에 존재할 수 있는 최대 시간  
주로 2분으로 정의된다.  
TIME_WAIT 간격을 결정한다.  

## API
### readv
소켓으로부터 벡터를 구성하여 데이터를 수신한다.  

### writev
소켓으로부터 벡터를 구성하여 데이터를 송신한다.  

### struct timeval
``` cpp
	struct timeval	{
		long		tv_sec;		// 초
		long		tv_usec;	// 마이크로초
	}
```
10E-6초(마이크로초) 까지 지정할 수 있다.

### struct timespec
``` cpp
	struct timespec{
		time_t		tv_sec;		// 초
		long		tv_nsec;	// 나노초
	}
```
10E-9초(나노초) 까지 지정할 수 있다.

# 6장
## 이론
### statefull
### stateless





# 9장
## 이론
### 0번 시그널
kill함수에 시그널 0을 지정시 해당 프로세스가 살아있는지 알려주는 용도로 사용된다.  
이는 kill 함수의 특별한 기능이다.  

### EINTR error 
느린 I/O 처리를 하거나 블로킹 모드에서 작동되는 함수에서 발생한다.  
타임아웃을 설정하는 기능이 있거나 소켓에 타임아웃이 설정된 경우에 EINTR를 리턴한다.  
accept, send, recv가 낮은 빈도로 EINTR 에러를 발생하여 데이터가 전송되지 않을 가능성이 있다.  
일반적으로 중단되었던 코드를 재시작 하도록 작성해야 한다.

### 좀비 프로세스
종료되어있지만, 점유한 자원이 풀어지지 않은 상태의 프로세스  
시스템의 모든 자원은 데이터와 메타 데이터로 이루어진다.  
프로세스가 종료되면 프로세스가 가진 이지지와 메타데이터를 모두 해제한다.  
부모 프로세스가 자식 프로세스의 종료값을 확인하지 않으면 PCB의 일부를 해제하지 않고 대기하게 된다.  
메타데이터의 일부인 PCB를 해제하는데 오랜 시간이 걸리거나 해제가 안된 경우를 좀비 프로세스라 한다.  


### 좀비프로세스의 해결 방법
자식 프로세스의 종료상태를 읽으면 좀비프로세스가 해결된다.  
wait, waitpid, waitid 함수로 자식 프로세스의 종료상태를 확인하면 운영체제는 좀비프로세스를 해제한다.  
자식프로세스의 종료상태를 읽을 필요 없는 경우 SIGCHILD 시그널을 무시하도록 시그널 핸들러를 설치하면 된다.  
SIGCHILD를 SIG_IGN로 설정한 후 핸들러를 설치하면 자식프로세스 종료 후 자동으로 PCB가 해제된다.
자식 프로세스가 정지된 경우에도 SIGCHILD 시그널을 전달한다.  
SA_NOCLDSTOP을 설정하면 자식 프로세스가 종료된 경우에만 시그널을 전달한다.  


### 부모 프로세스가 죽은 경우 자식 프로세스의 행동
부모 프로세스가 먼저 죽는경우
자식프로세스는 고아 프로세스가 된다.
최상위 프로세스인 init 프로세스가 고아 프로세스의 계 부모 프로세스가 되어 해제를 진행한다.  


### 프로세스 그룹, PGID(Process Group ID)
fork를 통해 생성되는 자식 프로세스들을 관리할 수 있도록 만들어진 그룹  
프롬프트에서 명령을 내리는 경우 자신의 PID와 동일한 PGID를 가지는 프로세스 그룹을 생성한다.  

setpgid를 통해 프로세스 그룹을 옮길 수 있다.  
다만 옮길 프로세스 그룹과 세션이 같아야한다는 조건이 있다.  
(팀)  

### 프로세스 그룹 리더
PID와 PGID가 같은 프로세스를 그룹 리더라고 한다.  
그룹 내의 다른 프로세스에게 시그널을 전파할 수 있다.  
자식 프로세스가 새롭게 그룹을 생성하여 리더가 될 수 있다.  

### 세션, SID(Session ID)
정의 : 사용자와 통신할 수 있는 일련의 프로세스들이 활동하는 논리적으로 구별되는 시스템 내의 가상공간  
세션에는 최소한 1개 이상의 프로세스 그룹이 존재하고 1개의 제어 터미널을 가진다.  

SID를 부여받은 프로세스로부터 파생된 프로세스는 모두 부모 프로세스의 SID를 물려받는다.  
SID를 추적하면 자식 프로세스의 기원을 추적할 수 있다.  
SID를 부여받은 프로세스는 터미널 입력을 받을 수 있다.  
이를 제어 터미널 이라 한다.  
(회사)

### 세션 리더
PID와 SID가 같은 프로세스를 세션 리더라고 한다.

### 제어 터미널
같은 세션 내에 있는 모든 프로세스가 돌려가면서 사용할 수 있는 터미널
fg, bg 명령어를 통해 여러 프로세스를 돌려가며 사용하는 것이 예가된다.
위의 상황에서는 셸 프로그램이 세션 리더가 된다.



<!-- 
## sub-title
### function_name
	#include <>
	[function]
**Parameters**
- 

**Return Value**
- 

**Description**  


### function_name
	a
**Parameters**
- .

**Return Value**
- .

**Description**




## trash

```cpp
	/*!
	* @brief	
	* @details	
	* @param[in]	
	* @return	
	* @retval	
	*/
```

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
-->
