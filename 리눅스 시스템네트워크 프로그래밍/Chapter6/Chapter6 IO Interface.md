<h1> I/O Interface </h1>

<h2> Index </h2>

- [서론](#서론)
- [파이프(pipe)](#파이프pipe)
	- [익명 파이프(unnamed pipe)](#익명-파이프unnamed-pipe)
	- [pipe](#pipe)
	- [popen](#popen)
	- [pclose](#pclose)
- [명명된 파이프](#명명된-파이프)
	- [mkfifo](#mkfifo)
	- [mkfifoat](#mkfifoat)
- [소켓(Socket)](#소켓socket)
	- [도메인에 의한 분류](#도메인에-의한-분류)
	- [타입에 의한 분류](#타입에-의한-분류)
	- [socket](#socket)
	- [소켓 도메인](#소켓-도메인)
	- [소켓 타입](#소켓-타입)
	- [소켓 프로토콜](#소켓-프로토콜)
	- [소켓의 주요 조합](#소켓의-주요-조합)
- [Byte Order](#byte-order)
- [sub-title](#sub-title)
	- [function\_name](#function_name)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)


## 서론
I/O Interface  
외부와 자료를 교환하기 위한 기능을 의미한다.  
파일, FIFO, 파이프, 소켓, mmap, 공유메모리, 세마포어, 메시지 큐 등이 포함된다.  
그중 파이프와 소켓에 대해 정리하였다.  



## 파이프(pipe)
익명 파이프(unnamed pipe) 와 명명된 파이프(named pipe)가 있다.  
읽기와 쓰기를 하는 프로세스 모두가 열기를 하지 못하면 블록킹에 걸리게 된다.  
이벤트 통지, 리다이렉션 같은 간단한 신호를 주고받는 용도로 사용된다.  

가장 쉽게 접할 수 있는 파이프는 '|' 문자이다.  
셸에서 익명파이프로 생성 되며 두 프로세스 사이의 출력과 입력을 연결한다. 


### 익명 파이프(unnamed pipe)
사용 후 pclose함수를 통해 닫아야한다.  
닫지 않는다면 좀비프로세스가 생성된다.  

popen 함수는 command에 해당하는 명령을 실행하며 fork 한다.  
보안에 취약하다는 단점이 있다.  


### pipe
	int pipe(int filedes[2]);
**Parametters**
-  `int filedes[2]` : 읽기, 쓰기 파일디스크립터를 반환받을 배열

**Return Value**
- `0` : 성공
- `-1` : 실패

**Description**  
성공시 2개의 파일 기술자를 생성하여 배열로 반환한다.  
fork를 통해 부모, 자식 프로세스간 데이터를 교환할 때 사용된다.  
단방향 통신을으로 만들어지며, 양방향을 위해서는 두개의 파이프를 
생성한다.  
filedes[0]	: 읽기용  
filedes[1]	: 쓰기용  


### popen
	FILE *popen(
		const char		*command,
		const char		*type);
**Parametters**
- `char *command` : 실행할 명령어
- `char *type`
  - "r" : 읽기모드 명령어가 표준출력으로 출력한 문자열을 읽을 수 있도록 함
  - "w" : 쓰기모드 명령어를 실행한 후 키보드로 데이터를 전송할 수 있도록 함
  
**Return Value**
- `NULL` : 생성 실패
- `other` : 파일포인터

**Description**  
command에 해당하는 명령을 실행하며 fork 한다.  
읽기모드와 쓰기모드로 생성 가능하다.  
FILE * 타입을 반환하지만 pclose로 닫아야 한다.  
외부 요인에 의해 크래킹될 수 있어 보안에 취약하다.  


### pclose
	int pclose(FILE *stream)
**Parametters**
- `FILE *stream` : 닫을 파일 스트림

**Return Value**
- `othre` : 성공
- `-1`	: 오류 발생 errno 설정

**Description**  
할당된 파이프 스트림을 해제한다.  
상대 프로세스의 종료를 대기한 후 종료되면 return 된다.  



## 명명된 파이프
명명된 파이프는 FIFO라고도 불린다.  
외부에서 접근 가능한 이름(경로)이 있는 경우를 의미한다.  
pathname의 파일 경로에 mode 접근권한을 갖는 FIFO를 생성한다.  
open 함수를 통해 파이프에 접근 가능하다.  
read, write를 사용해 읽고 쓸수 있다.  
FIFO를 닫으면 수신측은 EOF가 수신된다.  
송신측 프로세스가 여러개라면 모든 프로세스가 닫아야 EOF가 송신된다.  

### mkfifo
	int mkfifo(
		const char		*pathname,
		mode_t			mode
	)
**Parametters**
- `const char *pathname` : 파이프로 사용할 파일 이름
- `mode_t mode`
  - 파이프 접근 권한  
	| 속성 | 의미 |
	| :--: | :--: |
	| O_RDONLY | 수신측 |
	| O_WDONLY | 송신측 |
  
**Return Value**
- `0`	: 성공
- `-1`	: 실패 errno 설정

**Description**  
이름이 phthname인 FIFO 특수 파일을 만든다.  
통신을 위해서는 양쪽 끝이 열려있어야 한다.  
외부에서 접근이 가능하다.  

### mkfifoat
	int mkfifoat(
		int			fd,
		const char		*path,
		mode_t			mode
	)
**Parametters**
- `int fd` : 디렉터리의 기술자
- `const char *path` : 파이프로 사용할 파일 이름 (상대경로로 해석)
- `mode_t mode`
  - 파이프 접근 권한  
	| 속성 | 의미 |
	| :--: | :--: |
	| O_RDONLY | 수신측 |
	| O_WDONLY | 송신측 |

**Return Value**
- `0`	: 성공
- `-1`	: 실패 errno 설정

**Description**  
fd가 가리키는 위치에서 상대경로로 파일을 연다.  
이 때 fd는 파일이 아닌 디렉터리이다.  
배번 바뀌는 임시 디렉터리에 FIFO를 생성할 때 유용하다.  

**Example**
``` c++
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open(".", O_RDONLY);
    int ret = mkfifoat(fd, "myfifo", 0666);
    if (ret == -1) {
        perror("mkfifoat");
        return 1;
    }
    printf("Named pipe created successfully\n");
    close(fd);
    return 0;
}
```


## 소켓(Socket)
I/O 인터페이스중 가장 많이 사용된다.  
대부분의 운영체제가 BSD 소켓 인터페이스를 따르고 있다.  
통신 가능한 장치에 입출력이 가능한 파일을 탑재하는 형태를 띄고있다.  
파이프 입출력과 동일하게 read, write, close를 사용한다.  


### 도메인에 의한 분류
도메인 : 소켓으로 통신할 수 있는 범위의 분류  
유닉스 도메인 소켓 : 유닉스 계열에서만 사용 가능  
네트워크 도메인 소켓 : 네트워크 계층 내에 특정 네트워크 주소에 연결 가능  
| 도메인		| 유닉스 소켓	| 네트워크 소켓			|
| :--: | :--: | :--: |
| 외부 인터페이스	| 파일 경로	| 네트워크 주소(IP,port 등)	|
| 원격지 연결		| 불가능	| 가능				|
| 유닉스 계열 외 OS 지원| 불가능	| 가능				|


### 타입에 의한 분류
데이터그램 소켓 : 사용자가 정한 데이터그램 단위로 전송  
스트림 소켓 : 통로를 연결해 데이터를 전송  
|			| 데이터그램 소켓	| 스트림 소켓		|
| :--:			| :--:			| :--:			|
| 데이터 경계 		| 파일 경로		| 보존 안 됨		|
| 데이터 크기 제한 	| 보존 됨		| 없음			|
| 데이터 순서 		| 보존 안 됨		| 보존됨		|
| 연결 과정 		| 필요 없음 (1:n 통신)	| 필요함 (1:1 통신)	|
| 신뢰 			| 낮음 (유실 시 복구 X)	| 높음 (데이터 유실시 재전송) |


### socket
	int socket(
		int			domain,
		int			type,
		int			protocol
	)
**Parametters**
- `int domain` 
	| AF_UNIX	| 유닉스 도메인 소켓(AF_LOCAL로도 가능) |
	| :--: | :-- |
	| AF_INET	| IPv4 네트워크 도메인 소켓 |
	| AF_INET6	| IPv6 네트워크 도메인 소켓 |
- `int type` 
	| SOCK_STREAM	| 스트림 소켓 |
	| :--: | :-- |
	| SOCK_DGRAM	| 데이터그램 소켓 |
	| SOCK_RAW	| raw 소켓 |
- `int protocol`
	| IPPROTO_IP	| IP 프로토콜 사용 |
	| :--: | :-- |
	| IPPROTO_TCP	| TCP 프로토콜 사용 |
	| IPPROTO_UDP	| UDP 프로토콜 사용 |
	| IPPROTO_ICMP	| ICMP 프로토콜 사용 |
**Return Value**
- `other`	: 파일기술자
- `-1`		: 실패, errno 설정

**Description**  
소켓을 생성한다.  
이 때 생성된 소켓은 빈 소켓으로 bind를 해야 입출력 인터페이스를 가질 수 있다.  
도메인, 소켓 타입, 프로토콜은 특정 조합에 따라 사용된다.


### 소켓 도메인
AF_접두어는 Address Family의 의미이다.  
PF_접두어를 사용하는 경우도 있으며, Protocol Family의 의미를 갖는다.  


### 소켓 타입
| SOCK_STREAM	| 프로토콜 헤더를 포함한 원시적인 패킷을 볼 수있는 소켓을 사용</br>주로 모니터링 프로그램이나 패킷에 변경을 줄 때 사용한다.|
| :--: | :-- |
| SOCK_DGRAM	| 연결 지향성 1:1 통신을 한다.</br>데이터 순서(byte order)와 재전송을 해준다.|
| SOCK_RAW	| 비연결 지향성 1:n 통신 방식이다.</br>초기 연결이나 해제, 제어에 관련된 절차가 없어 상대적으로 오버헤드가 적다.</br>일회성 데이터, 응답이 필요없는 작은 데이터 조각을 전송할 때 유리하다.|


### 소켓 프로토콜
IPPROTO_IP : 지정된 소켓 타입에 따라 프로토콜을 자동으로 설정


### 소켓의 주요 조합

<table>
<tr>	<td align="center"> Domain </td>
	<td align="center"> Type </td>
	<td align="center"> Protocol </td>
</tr>
<tr>	<td rowspan=3>AF_UNIX</td>
		<td> SOCK_STREAM </td>
		<td> 0</td>
	<tr>	<td> SOCK_DGRAM </td>
		<td> 0</td>
	</tr>
	<tr>	<td> SOCK_RAW </td>
		<td> 0, IPPROTO_ICMP</td>
	</tr>
</tr>
<tr>	<td rowspan=3>AF_INET</td>
		<td> SOCK_STREAM </td>
		<td> 0, IPPROTO_TCP</td>
	<tr>	<td> SOCK_DGRAM </td>
		<td> 0, IPPROTO_UDP</td>
	</tr>
	<tr>	<td> SOCK_RAW </td>
		<td> IPPROTO_RAW, IPPROTO_ICMP, IPPROTO_UDP,</br> IPPROTO_TCP </td>
	</tr>
</tr>
<tr>	<td rowspan=3>AF_INET6</td>
		<td> SOCK_STREAM </td>
		<td> 0, IPPROTO_TCP</td>
	<tr>	<td> SOCK_DGRAM </td>
		<td> 0, IPPROTO_UDP</td>
	</tr>
	<tr>	<td> SOCK_RAW </td>
		<td> IPPROTO_RAW, IPPROTO_ICMPV6, IPPROTO_ICMP,</br> IPPROTO_UDP, IPPROTO_TCP</td>
	</tr>
</tr>
</table>



## Byte Order

## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

