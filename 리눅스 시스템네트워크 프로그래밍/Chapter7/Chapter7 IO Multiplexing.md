<h1> I/O Multiplexing </h1>

<h2> Index </h2>

- [서론](#서론)
- [레벨/엣지 트리거](#레벨엣지-트리거)
- [select](#select)
	- [select와 pselect의 차이점](#select와-pselect의-차이점)
	- [fd\_set 구조체](#fd_set-구조체)
	- [fd\_set 변경 매크로](#fd_set-변경-매크로)
	- [select](#select-1)
	- [pselect](#pselect)
	- [select 시 필요 주요 변수](#select-시-필요-주요-변수)
- [poll](#poll)
	- [struct pollfd](#struct-pollfd)
	- [poll events](#poll-events)
	- [poll](#poll-1)
	- [ppoll](#ppoll)
- [epoll](#epoll)
	- [epoll\_create](#epoll_create)
	- [epoll\_ctl](#epoll_ctl)
	- [epoll\_event](#epoll_event)
	- [epoll\_data](#epoll_data)
	- [epoll\_wait](#epoll_wait)


## 서론
I/O 멀티플렉싱의 사전적 의미는 다중 입출력 통신이다.  
데이터 흐름을 하나로 가정하고 입출력을 설계, 구현하면 bottleneck으로 인한지연이 발생하게된다.  
bottleneck으로 인한 레이턴시를 최소화할 수 있는 것이 멀티 플랙싱 기법이다.  

통신시 넌블로킹 모드를 설정 후 모든 소켓을 순서대로 recv를 시도하는 방법도 가능하다.  
그러나 소켓 버퍼에 수신된 경우에만 recv를 시도하는 편이 더욱 효율적이다.  

select와 poll, epoll 함수가 지원된다.  
이외에도 /dev/poll, kqueue 등의 비표준 기술이 있다.  

select는 가장 큰 소켓 파일기술자 값이 클 수록 오버헤드가 발생한다.  
poll은 select보다 정교한 정교한 핸들링이 가능하다.  
poll은 500~1000개 이하의 통신 규모에서 사용한다.  
select와 pool은 레벨 트리거만을 지원한다.   
epoll은 레벨/엣지 트리거를 지원한다.  


## 레벨/엣지 트리거
엣지 트리거
- 상태가 변하는 순간을 기준으로 감지한다.  
- 버퍼에 데이터가 남아있다면 이벤트를 발생한다.  

레벨 트리거
- 상태의 변화가 어떤 일정한 수준을 넘었는지를 감지한다.  
- 버퍼에 데이터가 이전보다 더 많아질 경우에 이벤트를 발생한다.  
- TCP 통신 중 수신해야하는 데이터가 더 남아있는 경우 사용할 수 있다.  


## select
select와 pselect의 두가지 함수가 존재한다.  
select보다 pselect 함수를 주로 사용해야 한다.  

가장 큰 번호의 파일기술자에 의해 오버헤드가 발생한다.  
select 호출 시 인자로 가장 큰 파일 기술자 번호에 1을 더해 전달한다.  
이 인자는 내부 루프 횟수를 의미하며 이로인해 기술자 번호가 클수록 성능에 불이익이 생긴다.  
파일 기술자 번호 중 가장 큰 값을 직접 계산하여 넘겨줘야 한다.  
감시할 파일 기술자 리스트를 저장 및 관리해야 한다.  


### select와 pselect의 차이점  
타임 아웃 구조체  
- select - [struct timeval](../etc.md#struct-timeval)  
- pselect - [struct timespec](../etc.md#struct-timespec)  

블로킹 중 시그널 발생
- select 
  - 에러로 리턴
  - 추가적인 시그널 처리를 직접 코딩해야 함  
- pselect
  - 블록 처리
  - 시그널 마스크를 통해 시그널 지정 가능


### fd_set 구조체
감시할 이벤트에 파일디스크립터를 등록하는 구조체이다.  
파이프나 다른 장치의 이벤트 또한 감지할 수 있다.  
ex) readfds에 stdin을 사용해 키보드 입력 발생을 감지할 수 있다.  

|fd_set 인수	| 감지 가능 이벤트 |
| -- | -- |
| readfds	| 소켓 수신 버퍼에 데이터가 도착한 경우</br>소켓 수신 버퍼에 접속연결 해제요청이 발생한 경우</br>리스너 소켓에 새로운 접속이 있는 경우 |
| writefds	| 소켓 송신 버퍼에 빈공간이 생긴 경우</br>반대편에서 연결을 끊은 경우</br>TCP 스트림에 데이터를 전송 가능한 경우 |
| errorfds	| TCP의 OOB데이터가 수신된 경우 |

### fd_set 변경 매크로
	void	FD_ZERO	(fd_set * fdset);
	void	FD_CLR	(int fd, fd_set * fdset);
	void	FD_SET	(int fd, fd_set * fdset);
	int	FD_ISSET(int fd, fd_set * fdset);
|FD_ZERO	| fd_set을 초기화한다. |
| -- | -- |
|FD_SET		| fd_set에 파일기술자를 등록한다. |
|FD_CLR		| fd_set에 파일기술자를 해제한다. |
|FD_ISSET	| fd_set에 파일기술자가 등록되어있는지 확인한다. |

**Parameters**
- `int fd`		: 변경할 파일디스크립터 번호
- `fd_set * fdset`	: 변경을 저장할 fd_set 구조체

**Description**  
fd_set 구조체는 1024bit(128 Byte)크기를 갖는다.  
이벤트 발생시 파일 기술자에 해당하는 비트가 켜진상태로 반환된다.  
fd_set 구조체는 감시할 파일디스크립터를 알려주는 동시에 결과를 반환할 때 사용한다.  
select 함수를 재 호출 할 때 fd_set에 감시할 파일 디스크립터를 재설정해 넣어줘야 한다.  
select 함수가 성공적으로 리턴되었다면 파일 디스트립터를 FD_ISSET 매크로를 이용해서 확인해야한다.  


### select
	int select(
		int			nfds,
		fd_set	* restrict	readfds,
		fd_set	* restrict	writefds,
		fd_set	* restrict	errorfds,
		struct timeval	* restrict	timeout
	)
**Parameters**
- `int nfds`		: fd_set에 등록된 파일기술자 중 가장 큰 수 + 1
- `fd_set *readfds`	: 읽기 가능 이벤트
- `fd_set *writefds`	: 쓰기 가능 이벤트
- `fd_set *errorfds`	: 예외 상황 이벤트
- `timeval *timeout`	: [타임아웃 객체](../etc.md#struct-timeval)  

**Return Value**
- `ohter`	: 수신에 성공한 이벤트 수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 

**Description**   
nfds 인자에 fd_set에 등록된 파일기술자 중 가장 큰 수 + 1를 직접 계산해 넣어야 한다.  
이벤트 발생으로 리턴시 timeval 구조체에 남은 시간이 기록되어 반환된다.  
timeout 인자에 NULL 입력시 타임아웃 없이 진행된다.  
timeval 멤버에 0 입력시 바로 리턴되므로 주의해야한다.  


### pselect
	int pselect(
		int			nfds,
		fd_set	* restrict	readfds,
		fd_set	* restrict	writefds,
		fd_set	* restrict	errorfds,
		struct timespec	* restrict	timeout,
		const sigset_t	* restrict	sigmask
	)
**Parameters**
- `timespec *timeout` : [타임아웃 객체](../etc.md#struct-timespec)
- `sigset_t sigmask` : 감지할 시그널 마스크

**Return Value**
- `ohter`	: 수신에 성공한 이벤트 수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 

**Description**  
시그널 블록 마스크를 인수로 사용하여 함수 호출시 블록할 시그널을 지정할 수 있다.  
sigmask 인자를 NULL로 설정 시 select 와 동일한 효과를 갖는다.  


### select 시 필요 주요 변수
| 자료형 | 변수명 | 사용 처 |
| -- | -- | -- |
| fd_set| fds		| 이벤트 감시용 파일기술자 세트 |
| int	| fd_biggest	| 파일 기술자 세트 중 가장 큰 번호 |
| int	| fd_socket[]	| 시스템에서 관리하는 파일기술자 배열|
| int	| cnt_fd_socket	| fd_socket에서 관리하는 파일기술자 개수 |



## poll
select 함수의 복잡한 인수 리스트와 비효율적인 루프를 개선하기위해 만들어진 함수이다.  
poll 함수 또한 select 함수에 비해 큰 성능의 향상을 이룩하지는 못했다.  
시그널을 감시할 수 있는 ppoll 함수가 있으나 표준함수가 아니다.


### struct pollfd
``` cpp
struct pollfd{
	int			fd;		// 파일 디스크립터
	short			events;		// 요구된 이벤트
	short			revents;	// 반환된 이벤트
}
```
**Description**  
감시할 파일디스크립터의 정보를 등록하는 구조체  
revents에 감시 결과가가 저장되어 반환된다.  


### poll events
| 이벤트 명 | 설명 |
| :-- | :--|
| POLLIN	| 읽기 버퍼에 데이터가 있음</br>TCP 연결 요청이 들어옴 |
| POLLPRI	| TCP의 OOB 데이터가 감지됨 |
| POLLOUT	| 쓰기 버퍼가 사용가능한 상태가 됨</br>버퍼가 비워짐</br>넌블로킹 connect가 완료됨 |
| POLLERR	| 연결에 에러가 발생함 |
| POLLHUP	| 닫힌 연결에 쓰기 시도 감지 |
| POLLNVAL	| 무효한 파일기술자 지정 에러</br>연결되지 않은 파일기술자를 지정함 |


### poll
	int poll(
		struct pollfd 		fds[],
		nfds_t			nfds,
		int			timeout,
	)
**Parameters**
- `pollfd fds[]`	: 감시할 파일기술자와 이벤트 정보
- `nfds_t nfds`	: 감시할 파일기술자의 수 
- `int timeout`	: 타임아웃

**Return Value**
- `ohter`	: 수신에 성공한 파일디스크립터의 수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 

**Description**  
타임아웃 없이 진행하려면 -1을 넣어야 한다.  
타임아웃을 NULL로 지정한 경우 0으로 캐스팅돼 즉시 반환된다.  
select와 달리 구조체에 값을 다시 집어넣을 필요가 없다.  


### ppoll
	int ppoll(
		struct pollfd 		* fds,
		nfds_t			nfds,
		const struct timespeec 	* timeout_ts,
		const sigset_t 		* sigmask
	)
**Parameters**
- `timespec *timeout` : [타임아웃 객체](../etc.md#struct-timespec)
- `sigset_t *sigmask` : 시그널 마스크

**Return Value**
- `ohter`	: 수신에 성공한 파일디스크립터의 수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 

**Description**  
sigmask 인자를 NULL로 설정 시 poll과 동일한 효과를 갖는다.  



## epoll
엣지트리거 기능의 지원이 추가되었다.  
메모리 복사의 부담이 많이 줄어들었다.  
statefull 함수로 파일기술자 정보를 내부적으로 저장한다.  
파일 기술자의 등록, 해제, 변경하는 함수와 이벤트를 감시하는함수가 분리되었다.  
이벤트가 발생한 파일기술자만 반환하기때문에 루프를 돌며 검사할 필요가 없다.

### epoll_create
	int epoll_create (int size)
	int epoll_create1 (int flags)
**Parameters**
- `int size`	: 등록할 수 있는 파일기술자의 개수
- `int flags`	: 플래그 (EPOLL_CLOSEXEC)

**Return Value**
- `other` : epoll 파일 기술자
- `-1`	: 에러, errno 설정 

**Description**  
epoll 파일기술자를 생성하여 반환한다.  
epoll 파일기술자는 epoll을 구분할 수 있는 정보로 다른 파일기술자들과는 다르다.  
size값은 무시되고 동적으로 메모리를 관리되는 방식으로 변경되었다.  
size에는 0 이상의 값을 넣어야 한다.  
close()를 사용해 epoll을 제거한다.  

EPOLL_CLOSEXEC exec 계열 함수 실행 시 자동으로 파일 기술자를 닫는다.


### epoll_ctl
	int epoll_ctl(
		int			epfd,
		int			op,
		int			fd,
		struct epoll_event 	* event
	)
**Parameters**
- `int epfd`	: epoll 파일 기술자
- `int op`	: 조작할 작업
  |EPOLL_CTL_ADD	| 파일 기술자와 이벤트를 등록한다.	|
  | :--:		|:--:|
  |EPOLL_CTL_DEL	| 파일 기술자의 정보를 제거한다.	|
  |EPOLL_CTL_MOD	| 파일 기술자의 이벤트를 교체한다.	|
- `int fd`	: 대상 파일기술자
- `epoll_event *event`	: 이벤트 구조체
  

**Return Value**
- `0` : 실행 성공
- `-1` : 에러, errno 설정 

**Description**  
감시할 파일기술자의 이벤트를 등록, 변경, 제거한다.


### epoll_event
```cpp
struct epoll_event{
	uint32_t		events;
	epoll_data_t		data;
}__attribute__((__packed__));
```
**Parameters**
- `uint32_t events` : 감시 이벤트
  | EPOLLIN		| 읽기 버퍼에 데이터가 있다.	|
  | -- | -- |
  | EPOLLPRI	| 우선순위 데이터를 사용한다.(TCP의 OOB)	|
  | EPOLLOUT	| 쓰기 버퍼가 사용가능한 상태가 됨</br>버퍼가 비워짐</br>넌블로킹 connect가 완료됨	|
  | EPOLLERR	| 연결에 에러가 발생함	|
  | EPOLLHUP	| 닫힌 연결에 쓰기 시도 감지	|
  | EPOLLONESHOT	| 이벤트 감시를 일회용으로 사용한다.	|
  | EPOLLET		| 이벤트를 엣지 트리거로 작동시킨다.	|
- `epoll_data_t data` : 감시대상 정보를 갖는 union 자료형

**Description**  
EPOLLONESHOT의 경우 한번 감지된 파일기술자의 이벤트 마스크를 비활성화 시킨다.  
epoll_ctl로 마스크를 재설정시 까지 이벤트를 감시하지 않는다.  

이외에도 EPOLLRDNORM, EPOLLRDBAND, EPOLLWRNORM, EPOLLWRBAND 등의 다양한 이벤트가 존재한다.  

### epoll_data
```cpp
typedef union epoll_data{
	void			*ptr;
	int			fd;
	uint32_t		u32;
	uint64_t		u64;
}epoll_data_t;
```
**Description**  
감시대상의 정보를 공용체에 저장한다.


### epoll_wait
	epoll_wait(
		int			epfd,
		struct epoll_event	*events,
		int			maxevents,
		int			timeout
	)
	epoll_pwait(
		int			epfd,
		struct epoll_event	*events,
		int			maxevents,
		int			timeout,
		const sigset_t		*sigmask
	)
**Parameters**
- `int epfd`	: epoll 파일디스크립터
- `epoll_event *events` : 이벤트를 수신받을 epoll_event 배열
- `int maxevents` : 수신받을 수 있는 이벤트의 최대 개수
- `int timeout` : 타임아웃
- `sigset_t *sigmask` : 시그널 마스크

**Return Value**
- `ohter`	: 수신에 성공한 파일디스크립터 개수
- `0`	: timeout 발생시 까지 이벤트 없음
- `-1`	: 에러, errno 설정 
  
**Description**  
파일기술자의 이벤트 수신 여부를 확인한다.  
