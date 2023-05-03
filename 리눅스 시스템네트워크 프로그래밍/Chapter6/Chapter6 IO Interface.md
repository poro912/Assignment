<h1> I/O Interface </h1>

<h2> Index </h2>

- [서론](#서론)
- [파이프(pipe)](#파이프pipe)
	- [익명 파이프(unnamed pipe)](#익명-파이프unnamed-pipe)
	- [pipe](#pipe)
	- [popen](#popen)
	- [pclose](#pclose)
- [명명된 파이프 (FIFO)](#명명된-파이프-fifo)
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
	- [Byte Order Macro](#byte-order-macro)
- [유닉스 소켓 ()](#유닉스-소켓-)
	- [function\_name](#function_name)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)
- [TCP 소켓 (SOCK\_STREAM)](#tcp-소켓-sock_stream)
	- [socket](#socket-1)
	- [bind](#bind)
	- [struct sockaddr](#struct-sockaddr)
	- [struct sockaddr\_strage](#struct-sockaddr_strage)
	- [struct sockaddr\_in](#struct-sockaddr_in)
	- [struct sockaddr\_in6](#struct-sockaddr_in6)
	- [listen](#listen)
	- [accept](#accept)
	- [connect](#connect)
	- [send](#send)
	- [recv](#recv)
	- [close](#close)
	- [shutdown](#shutdown)
- [**Return Value**](#return-value-1)
	- [function\_name](#function_name-1)
- [**Parametters**](#parametters-1)
- [**Return Value**](#return-value-2)
- [UDP 소켓 ()](#udp-소켓-)
	- [function\_name](#function_name-2)
- [**Parametters**](#parametters-2)
- [**Return Value**](#return-value-3)
- [TCP 소켓과 UDP 소켓의 차이](#tcp-소켓과-udp-소켓의-차이)
- [TCP 소켓 기법](#tcp-소켓-기법)
	- [function\_name](#function_name-3)
- [**Parametters**](#parametters-3)
- [**Return Value**](#return-value-4)
- [sub-title](#sub-title)
	- [function\_name](#function_name-4)
- [**Parametters**](#parametters-4)
- [**Return Value**](#return-value-5)


## 서론
I/O Interface  
외부와 자료를 교환하기 위한 기능을 의미한다.  
파일, FIFO, 파이프, 소켓, mmap, 공유메모리, 세마포어, 메시지 큐 등이 포함된다.  
그중 파이프와 소켓에 대해 정리하였다.  



## 파이프(pipe)
익명 파이프(unnamed pipe) 와 명명된 파이프(named pipe)가 있다.  
읽기와 쓰기를 하는 프로세스가 존재한다.  
두 프로세스 모두가 열기를 진행할 때까지 블로킹에 걸리게 된다.  
이벤트 통지, 리다이렉션 같은 간단한 신호를 주고받는 용도로 사용된다.  

가장 쉽게 접할 수 있는 파이프는 리눅스에서의 '|' 문자이다.  
셸에서 익명파이프로 생성 되며 두 프로세스 사이의 출력과 입력을 연결한다. 


### 익명 파이프(unnamed pipe)
사용 후 pclose함수를 통해 닫아야한다.  
fclose나 close시 좀비프로세스가 생성될 가능성이 있다.  

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
단방향 통신으로 만들어지며, 양방향을 위해서는 두개의 파이프를 생성한다.  
filedes[0]	: 읽기용  
filedes[1]	: 쓰기용  


### popen
	FILE *popen(
		const char		*command,
		const char		*type
	)
**Parametters**
- `char *command` : 실행할 명령어
- `char *type`
  - "r" : 읽기모드 명령어가 표준출력으로 출력한 문자열을 읽을 수 있도록 함
  - "w" : 쓰기모드 명령어를 실행한 후 키보드로 데이터를 전송할 수 있도록 함
  
**Return Value**
- `other` : 파일포인터
- `NULL` : 생성 실패

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
상대 프로세스의 종료까지 대기한 후 종료되면 return 된다.  
서브셸의 안전한 종료를 보장하는 기능이 있어 좀비 프로세스 생성을 막아준다.  


## 명명된 파이프 (FIFO)
명명된 파이프는 FIFO라고도 불린다.  
외부에서 접근 가능한 이름(경로)이 있는 경우를 의미한다.  
파일 경로에 특정 접근권한을 갖는 FIFO를 생성한다.  
수신측은 O_RDONLY, 수신측은 O_WDONLY로 파일을 열어야 한다.  
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
- `mode_t mode` : 파이프 접근 권한  

  
**Return Value**
- `0`	: 성공
- `-1`	: 실패 errno 설정

**Description**  
이름이 phthname인 FIFO 특수 파일을 만든다.  
통신을 위해서는 송수신측이 열려있어야 한다.  
외부에서 접근이 가능하다.  
생성 이후 open 함수를 통해 접근한다.  
수신측은 O_RDONLY, 수신측은 O_WDONLY로 파일을 열어야 한다.  


### mkfifoat
	int mkfifoat(
		int			fd,
		const char		*path,
		mode_t			mode
	)
**Parametters**
- `int fd` : 디렉터리의 기술자
- `const char *path` : 파이프로 사용할 파일 이름 (상대경로로 해석)
- `mode_t mode` : 파이프 접근 권한  

**Return Value**
- `0`	: 성공
- `-1`	: 실패 errno 설정

**Description**  
fd가 가리키는 위치에서 상대경로로 파일을 연다.  
이 때 fd는 파일이 아닌 디렉터리이다.  
배번 바뀌는 임시 디렉터리에 FIFO를 생성할 때 유용하다.  
생성 이후 open 함수를 통해 접근한다.  
수신측은 O_RDONLY, 수신측은 O_WDONLY로 파일을 열어야 한다.  

**Example**
``` c++
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd;
    int ret = mkfifoat(fd, "myfifo", 0666);
    if (-1 == ret) {
        perror("mkfifoat error");
        return 1;
    }
    fd = open("myfifo", O_RDONLY);
    if(-1 == fd){
	perror("open error")
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
유닉스 도메인 소켓 : 유닉스 계열에서만 사용 가능한 소켓  
네트워크 도메인 소켓 : 네트워크 계층 내에 특정 네트워크 주소에 연결 가능한 소켓  
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
	| AF_UNIX	| 유닉스 도메인 소켓 (AF_LOCAL로도 가능) |
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
이 때 생성된 소켓은 빈 소켓이다.
bind 이후 입출력 인터페이스를 가질 수 있다.  
도메인, 소켓 타입, 프로토콜은 특정 조합에 따라 사용된다.


### 소켓 도메인
AF_접두어는 Address Family의 의미이다.  
PF_접두어를 사용하는 경우도 있으며, Protocol Family의 의미를 갖는다.  


### 소켓 타입
| SOCK_RAW	| 프로토콜 헤더를 포함한 원시적인 패킷을 볼 수있는 소켓을 사용</br>주로 모니터링 프로그램이나 패킷에 변경을 줄 때 사용한다.|
| :--: | :-- |
| SOCK_STREAM	| 연결 지향성 1:1 통신을 한다.</br>데이터 순서(byte order)와 재전송을 해준다.|
| SOCK_DGRAM	| 비연결 지향성 1:n 통신 방식이다.</br>초기 연결이나 해제, 제어에 관련된 절차가 없어 상대적으로 오버헤드가 적다.</br>일회성 데이터, 응답이 필요없는 작은 데이터 조각을 전송할 때 유리하다.|


### 소켓 프로토콜
IPPROTO_IP	: 지정된 소켓 타입에 따라 프로토콜을 자동으로 설정 (0값을 가짐)
IPPROTO_TCP	: TCP 프로토콜
IPPROTO_UDP	: UDP 프로토콜
IPPROTO_ICMP	: ICMP 프로토콜


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
원격 호스트와 데이터 통신할 때 꼭 지켜야하는 표현 규칙  
멀티바이트의 데이터를 표현할 때 사용하는 두 가지 표현법  
네트워크 통신의 경우 빅엔디안 방식을 사용한다.  

0x00124F80 숫자의 표현법
| | 빅엔디안 (Big Endian) | | 리틀 엔디안 (Little Endian) | |
| :--: | :--: | :--: | :--: | :--: |
|  | 낮은 주소에 큰 자릿수가 저장 | |높은 주소에 큰 자릿수가 저장 | |
|  | 숫자의 가장 큰 자릿수 부분부터 저장 | | 숫자의 가장 낮은 자릿수 부분부터 저장 | |
|  | 네트워크 통신 |  | 인텔 계열 CPU |  |
|  |  | 주소 |  |  |
|  | 0x80 | 0x0003 | 0x00 | little base |
|  | 0x4F | 0x0002 | 0x12 | |
|  | 0x12 | 0x0001 | 0x4F | |
| big base | 0x00 | 0x0000 | 0x80 |  |


### Byte Order Macro
	uint32_t htonl(uint32_t hostlong)
	uint16_t htons(uint16_t hostshort)
	uint32_t ntohl(uint32_t netlong)
	uint16_t ntohs(uint16_t netshort)
**Parametters**
- 해당 바이트오더로 변환할 값

**Return Value**
- 바이트오더에 맞게 변환된 숫자

**Description**  
각 바이트 오더에 맞는 숫자로 변환해준다.  
h : host	(Little Endian)  
n : network	(Big Endian)  
같은 방식을 사용하고 있는 경우라면 변환이 일어나지 않는다.  



## 유닉스 소켓 ()
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  



## TCP 소켓 (SOCK_STREAM)
데이터 전송 시 데이터의 경계를 보존하지 않는다.  
따로 어플리케이션 헤더를 만들거나 경계를 구분할 수 있는 구분자를 사용해야 한다.  
소켓에는 읽기채널과 쓰기채널이 존재하며 수신버퍼와 송신버퍼가 따로있다.  

server side	: socket -> bind -> listen  
client side	: socket (+ bind) -> connect  
active close	: close, shutdown 함수를 호출한 행위   
passive close	: 연결 종료 요청을 받아 close 함수를 호출하는 행위  
클라이언트 측에서도 bind가 일어난다.  
![](./img/TCP%20connect.png)

### [socket](#socket)
```cpp
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
```
TCP 소켓을 생성한다.


### bind
	int bind(
		int			sockfd,
		const struct sockaddr	*addr,
		socklen_t		addrlen
	)
**Parametters**
- `int sockfd`	: socket함수로 생성한 파일 기술자
- `struct sockaddr *addr`
  - 바인드 할 외부 인터페이스 정보 구조체
  - 아래는 sockadr_* 구조체이며 (struct sockaddr *) 타입으로 캐스팅해 사용
  - 빅엔디안으로 저장되어야 함
	|	|sockadr_* 구조체 명|
	| :--: | :--: |
	| AF_INET | struct sockaddr_in |
	| AF_INET6 | struct sockaddr_in6 |
	| AF_UNIX | struct sockaddr_un |
- `socklen_t addrlen`	: sockaddr 구조체의 크기

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정 

**Example**
``` cpp
	struct sockaddr_in saddr = {};
	listen_port = 8080;
	saddr.sin_family = AF_INET;		// IPv4
	saddr.sin_port = htons(listen_port);	// 빅엔디안 변환
	saddr.sin_addr.s_addr = INADDR_ANY;	// 로컬호스트의 모든 주소(0.0.0.0)
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	int ret = bind(sockfd,
		(struct sockaddr *) &saddr,
		sizeof(struct sockaddr_in));
```
**Description**  
소켓을 시스템에 부착하며, 외부로부터 연결점을 갖게된다.  
빈 소켓이 시스템 장치와 통신할 수 있도록 이름을 부여하는 과정이다.  
어떤 소켓 도메인을 사용하는지에 따라 세부 작업이 달라진다.  
클라이언트 측에서는 connect 과정에서 bind를 내포한다.  
connect를 통해 실행하는 경우 비어있는 포트를 순서대로 또는 랜덤으로 지정한다.  


### struct sockaddr
과거에 쓰였던 IPv4 전용 구조체  
sockaddr_in 구조체와 sockaddr_un을 감싸기위해 만들어졌다.  
```cpp
	struct sockaddr{
		sa_family_t		sa_family;	// Address family
		char			sa_data[];	// 가변길이 사용
	};
```


### struct sockaddr_strage
IPv4, IPv6를 모두 포함하는 새로운 구조체  
sockaddr_in6 구조체를 추가적으로 감싸기 위해 만들어졌다.  
```cpp
struct sockaddr_storage{
	sa_family_t	sa_family;
	// 정렬을 위한 패딩 
};
```


### struct sockaddr_in
``` cpp
	struct sockaddr_in{
		sa_family_t		sin_family;	// AF_INET
		in_port_t		sin_port;	// uint16_t
		struct in_addr		sin_addr;	// IPv4 32Bits
		char sin_zero[8];	// Not used (add padding)
	};
	struct in_addr{
		in_addr_t 		s_addr		// uint32_t
	};
```


### struct sockaddr_in6
``` cpp
	struct sockaddr_in6{
		sa_family_t		sin6_family;	// AF_INET6
		in_port_t		sin6_port;	// uint16_t
		uint32_t		sin6_flowinfo;	
		struct in6_addr		sin6_addr;	// IPv6 128Bits
		uint32_t		sin6_scope_id;
	};
	struct in6_addr{
		uint8_t			s6_addr[16];
	};
```


### listen
	int listen(
		int		socket,
		int		backlog
	)
**Parametters**
- `int socket` : bind된 파일 기술자
- `int backlog` : 연결 대기큐의 크기 (2의 제곱수를 주로 사용)

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정 

**Description**  
외부로부터 TCP 연결을 받아들일 수 있도록 연결큐를 만들어 대기한다.  
재호출 시 대기큐의 크기를 변경할 수 있다.  


### accept
	int accpet(
		int			sockfd, 
		struct sockaddr *restrict	address, 
		socklen_t *restrict	address_len
	)
**Parametters**
- `int sockfd`		: connect요청을 수락할 파일 기술자
- `struct sockaddr *restrict address`
  - 접속을 시도한 클라이언트의 주소 정보
  - sockadr_* 구조체, sockaddr 구조체로 캐스팅 해야 함
  - 필요 없다면 NULL로 지정
- `socklen_t *restrict address_len`
  - sockaddr 구조체의 크기
  - 필요 없다면 NULL로 지정

**Return Value**
- `other`	: 요청을 수락한 소켓 파일 기술자
- `-1`	: 에러, errno 설정 

**Description**  
클라이언트 측에서 보낸 접속 요청을 받아들인다.  
백로그에 여유가 있는 경우에 접속요청을 받아들인다.  
새로운 연결이 도착할 때 까지 블록상태로 대기한다.  
accept가 루프를 돌고있다면 매번 sockaddr 매개변수를 설정해서 넣어야 한다.  


### connect
	int connect(
		int			socket,
		cosnt struct socaddr	*address,
		socklen_t		address_len
	)
	
**Parametters**
- `int socket`			: socket함수(bind)로 생성한 파일 기술자
- `struct socaddr *address`	: sockadr_* 구조체, sockaddr 구조체로 캐스팅 해야 함
- `socklen_t address_len`	: sockaddr 구조체의 크기

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정 

**Description**  
listen상태의 서버에 연결을 시도한다.  
connect 성공 시 해당 소켓은 통신 가능상태가 되어 데이터를 주고받을 수 있다.  


### send
	ssize_t send(
		int			sockfd,
		const void		*buffer,
		size_t			length,
		int			flags
	)
	
**Parametters**
- `int sockfd` : 소켓 파일기술자
- `void *buffer` : 보낼 데이터의 버퍼
- `size_t length` : 버퍼에 담긴 데이터의 크기
- `int flags`
  - 작동 플래그
  	| 옵션 | 설명 |
	|:--:|:--|
	| 0		| write와 동일하게 동작 |
	| MSG_OOB	| 아웃오브밴드 데이터를 송신한다. |
	| MSG_NOSIGNAL	| 반대편 소켓 연결이 끊어졌을 대 SIGPIPE 시그널을 발생시키지 않는다.</br>EPIPE 에러 설정은 여전히 작동한다. |
	| MSG_DONTWAIT	| 1회성 넌블록킹 작동을 한다. |
	| MSG_EOR	| 레코드의 끝을 알리는 EOR을 지정한다. |
	| MSG_MORE	| 1회성으로 TCP_COR 옵션 기능을 사용한다.(리눅스전용)</br>데이터를 모아 MORE명령이 없는 전송시도가 있을 때 한번에 전송한다. |
	
**Return Value**
- `other`	: 데이터 복사에 성공한 바이트 크기
- `-1`	: 에러, errno 설정 

**ErrorCode**
| EAGIN | 넌블로킹으로 세팅된 소켓에서 소켓 버퍼에 공간이 없는 경우</br>보통의 경우 재전송시도를 함|
| :--: | :-- |
| EWOULDBLOCK	| 넌블로킹으로 세팅된 소켓에서 소켓 버퍼에 공간이 없는 경우</br>보통의 경우 재전송시도를 함 |
| EINTER	| 인터럽트가 발생하여 전송이 중단됨</br>보통의 경우 재전송시도를 함 |
| EPIPE		| 반대편 소켓의 연결이 끊어짐</br>보통의 경우 연결을 닫고 정리함 |
| ECONNRESET	| 반대편에서 연결을 강제로 끊음</br>보통의 경우 연결을 닫고 정리함 |

**Description**  
소켓으로 데이터를 송신한다.  
송신 데이터 크기와 복사에 성공한 크기가 다르다면 나머지부분은 계산해서 재전송해야한다.  
옵션이 없다면 write와 동일한 동작을 진행한다.



### recv
	ssize_t recv(
		int			sockfd,
		void			*buffer,
		size_t			length,
		int			flags
	)

**Parametters**
- `int sockfd` : 소켓 파일기술자
- `void *buffer` : 데이터를 저장할 버퍼
- `size_t length` : 데이터를 저장할 버퍼의 크기
- `int flags` 
  - 작동 플래그
  	| 옵션 | 설명 |
	|:--:|:--|
	| 0		| read와 동일하게 동작 |
	| MSG_OOB	| 아웃오브밴드 데이터를 수신함 |
	| MSG_PEEK	| recv가 성공한 뒤에도 소켓 수신 버퍼큐에서 데이터를 제거하지 않음 |
	| MSG_WAITALL	| 버퍼 크기가 다 채워질 때까지 대기한다.</br>시그널 개입이나 연결이 끊어진 경우엔 에러로 리턴 |
	| MSG_TRUNC	| recv 호출시 사용한 버퍼보다 큰 데이터를 수신해야 하는 경우 초과분을 삭제 |

**Return Value**
- `other`	: 데이터 수신에 성공한 바이트 크기
- `0`	: 상대측의 연결 종료요청
- `-1`	: 에러, errno 설정 

**Description**  
소켓으로부터 데이터를 송수신한다.  
리턴된 값이 length값과 같다면 남아있는 데이터가 있을 수 있다.  
나머지 데이터를 읽어와야 할 수 있으며 멀티플렉싱 기법을 주소 사용한다(I/O 7장 멀티플렉싱)  
옵션이 없다면 read와 동일한 동작을 진행한다.


### [close](../etc.md#close)
파일 디스크립터를 닫는다.  
해당 프로세스 내에서 소켓 ID를 닫는다.  

### shutdown
	int shutdown(
		int			socket,
		int			how
	)
**Parametters**
- `int socket`	: 닫을 소켓 파일기술자
- `int how`
  - 닫을 채널과 방법
	| 옵션 | 설명 |
	|:--:|:--:|
	| SHUT_RD	| 읽기 채널을 닫는다.</br>해당 소켓에 읽기 행동을 할 수 없다. |
	| SHUT_WR	| 쓰기 채널을 닫는다.</br>해당 소켓에 쓰기 행동을 할 수 없다.</br>명령이 성공하면 상대방에게 소켓을 닫기위한 신호(FIN)를 보낸다. |
	| SHUT_RDWR	| 소켓을 즉시 닫는다. |

**Return Value**
- 

**Description**  
소켓에 EOF신호를 보내 연결된 모든 소켓을 닫는다.  
TCP half-close	: 송수신 채널 중 하나만 닫는 행위, 몇몇 특수한 경우에만 사용된다.  

SHUT_RD	: 읽기 채널을 닫는것으로 수신 버퍼를 제거한다.  
상대편에서 오는 데이터를 모두 무시하고 송신할 데이터만 보내고 끊는 경우에 사용한다.  

SHUT_WD	: 쓰기 채널을 닫는것으로 송신 버퍼를 제거한다.  
상대편에게 더이상 보낼 데이터는 없으며, 수신할 데이터만 남은 경우 사용한다.  
상대방은 데이터를 다 보낸 뒤 recv를 하면 0이 리턴되어 close나 shutdown을 시도한다.  

SHUT_RDWR	: 소켓을 즉시 닫는다.  
close를 호출하는 경우 즉시 리턴되지만 연결이 바로해제되는 것이 아니다.  
전송되지 못한 데이터가 소켓 버퍼에 남아있다면 백그라운드에서 처리하고 연결을 끊기 때문이다.  


### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  


## UDP 소켓 ()
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

## TCP 소켓과 UDP 소켓의 차이

## TCP 소켓 기법
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  


## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

