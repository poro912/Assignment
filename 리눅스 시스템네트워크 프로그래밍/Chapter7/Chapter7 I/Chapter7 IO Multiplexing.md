<h1> ChapterName </h1>

<h2> Index </h2>

- [서론](#서론)
- [select](#select)
	- [select와 pselect의 차이점](#select와-pselect의-차이점)
	- [fd\_set](#fd_set)
	- [fd\_set 변경 매크로](#fd_set-변경-매크로)
	- [select](#select-1)
	- [pselect](#pselect)
	- [select 시 필요 변수](#select-시-필요-변수)
- [poll](#poll)
	- [function\_name](#function_name)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)
- [epoll](#epoll)
	- [function\_name](#function_name-1)
- [**Parametters**](#parametters-1)
- [**Return Value**](#return-value-1)
- [sub-title](#sub-title)
	- [function\_name](#function_name-2)
- [**Parametters**](#parametters-2)
- [**Return Value**](#return-value-2)


## 서론
I/O 멀티플렉싱의 사전적 의미는 다중 입출력 통신이다.  
데이터 흐름을 하나로 가정하고 설계, 구현하면 bottleneck으로 인한지연이 발생하게된다.  
bottleneck으로 인한 레이턴시를 최소화할 수 있는 것이 멀티 플랙싱 기법이다.  

통신시 넌블로킹 모드를 설정 후 순서대로 recv를 시도하는 방법도 가능하다.  
그러나 소켓 버퍼에 수신된 경우에만 recv를 시도하는 편이 더욱 효율적이다.  

select와 poll, epoll 함수가 지원된다.  
select는 가장 큰 소켓 파일기술자 값이 클 수록 오버헤드가 발생한다.  
poll은 select보다 정교한 정교한 핸들링이 가능하다.  
poll은 500~1000개 이하의 통신 규모에서 사용한다.  
select와 pool은 레벨 트리거만을 지원한다.  
epoll은 레벨/엣지 트리거를 지원한다.  

이외에도 /dev/poll, kqueue 등의 비표준 기술이 있다.  



## select
select와 pselect의 두가지 함수가 존재한다.
select보다 pselect 함수를 주로 사용해야 한다. 
가장 큰 번호의 파일기술자에 의해 오버헤드가 발생한다.  
select 호출 시 인자로 가장 큰 파일 기술자 번호에 1을 더해 전달한다.  
이 인자는 내부 루프 횟수를 의미하며 이로인해 기술자 번호가 클수록 성능에 불이익이 생긴다.  
감시할 파일 기술자 리스트를 저장 및 관리해야 한다.
파일 기술자 번호 중 가장 큰 값을 직접 계산하여 넘겨줘야 한다.



### select와 pselect의 차이점  
타임 아웃 구조체  
- select - [struct timeval](../../etc.md#struct-timeval)  
- pselect - [struct timespec](../../etc.md#struct-timespec)  

블로킹 중 시그널 발생
- select - 에러 리턴, 추가적인 시그널 처리 코딩을 해야 함  
- pselect - 시그널 블록 마스크를 설정 가능  


### fd_set
파이프나 다른 장치의 이벤트 또한 감지할 수 있다.  
ex) readfds에 stdin을 사용해 키보드 입력 발생을 감지할 수 있다.  

|fd_set 인수	| 감지 가능 이벤트 |
| -- | -- |
| readfds	| 소켓 수신 버퍼에 데이터가 도착한 경우</br>소켓 수신 버퍼에 접속연결 해제요청이 발생한 경우</br>리스너 소켓에 새로운 접속이 있는 경우 |
| writefds	| 소켓 송신 버퍼에 빈공간이 생긴 경우</br>반대편에서 연결을 끊은 경우</br>TCP 스트림에 데이터를 전송 가능한 경우 |
| errorfds	| TCP의 OOB데이터가 수신된 경우 |

### fd_set 변경 매크로
	void	FD_ZERO(fd_set * fdset);
	void	FD_CLR(int fd, fd_set * fdset);
	void	FD_SET(int fd, fd_set * fdset);
	int	FD_ISSET(int fd, fd_set * fdset);
|FD_ZERO	| set을 초기화한다. |
| -- | -- |
|FD_SET		| set에 파일기술자를 등록한다.|
|FD_CLR		| set에 파일기술자를 해제한다.|
|FD_ISSET	| set에 파일기술자가 등록되어있는지 확인한다.|

**Parametters**
- `int fd`		: 변경할 파일디스크립터 번호
- `fd_set * fdset`	: 변경을 저장할 fd_set 구조체

**Description**  
fd_set 구조체는 1024bit(128 Byte)크기를 갖는다.  
fd_set 구조체는 감시할 파일디스크립터를 알려주는 동시에 결과를 반환할 때 사용한다.  
이벤트가 발생시 파일 기술자에 해당하는 비트가 켜진상태로 반환된다.  
select 함수가 성공적으로 리턴되었다면 입력한 파일 디스트립터를 FD_ISSET 매크로를 이용해서 확인해야한다.  
select 함수를 재 호출 할 때 fd_set을 재설정해 넣어줘야 한다.  


### select
	int select(
		int			nfds,
		fd_set	* restrict	readfds,
		fd_set	* restrict	writefds,
		fd_set	* restrict	errorfds,
		struct timeval	* restrict	timeout
	)
**Parametters**
- `int		nfds`		: fd_set에 등록된 파일기술자 중 가장 큰 수 + 1
- `fd_set *	readfds`	: 읽기 가능 이벤트
- `fd_set *	writefds`	: 쓰기 가능 이벤트
- `fd_set *	errorfds`	: 예외 상황 이벤트
- `struct timeval *	timeout`	: [타임아웃 객체](../../etc.md#struct-timeval)  

**Return Value**
- `ohter`	: 수신에 성공한 파일디스크립터의 수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 

**Description**   
nfds 인자에 fd_set에 등록된 파일기술자 중 가장 큰 수 + 1를 직접 계산해 넣어야 한다.  
이벤트 발생으로 리턴시 timeval 구조체에 남은 시간이 기록되어 반환된다.  
timeout 인자에 NULL 입력시 타임아웃 없이 진행된다.  
timeval 구조체에 0 입력시 바로 리턴되므로 주의해야한다.  

### pselect
	int pselect(
		int			nfds,
		fd_set	* restrict	readfds,
		fd_set	* restrict	writefds,
		fd_set	* restrict	errorfds,
		struct timespec	* restrict	timeout,
		const sigset_t	* restrict	sigmask
	)
**Parametters**
- `const sigset_t sigmask` : 감지할 시그널 마스크
  
**Description**  
시그널 블록 마스크를 인수로 사용하여 함수 호출시 블록할 시그널을 지정할 수 있다.  
sigmask 인자를 NULL로 설정 시 select 와 동일한 효과를 갖는다.  

### select 시 필요 변수
| fd_set| fds		| 이벤트 감시용 파일기술자 세트 |
| -- | -- | -- |
| int	| fd_biggest	| 파일 기술자 세트 중 가장 큰 번호 |
| int	| fd_socket[]	| 시스템에서 관리하는 파일기술자 배열|
| int	| cnt_fd_socket	| fd_socket에서 관리하는 파일기술자 개수 |



## poll
select 함수의 복잡한 인수 리스트와 비효율적인 루프 구조를 개선하기위해 만들어진 함수이다.
poll 함수 또한 select 함수에 비해 큰 성능의 향상을 이룩하지는 못했다.  

시그널을 감시할 수 있는 ppoll 함수가 있으나 표준함수가 아니다.

### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  



## epoll
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

