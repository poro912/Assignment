<h1> 파일 처리</h1>
<h2> Index </h2>

- [서론](#서론)
- [분류별 함수 목록](#분류별-함수-목록)
	- [저수준 함수](#저수준-함수)
	- [고수준 함수](#고수준-함수)
	- [그 외의 파일 관련 함수](#그-외의-파일-관련-함수)
- [원자적 실행](#원자적-실행)
- [저수준 파일처리](#저수준-파일처리)
- [고수준 파일처리](#고수준-파일처리)
- [원자성의 보장 방법](#원자성의-보장-방법)
	- [파일 입출력](#파일-입출력)
	- [파이프 입출력](#파이프-입출력)
- [파일 출력이 섞이지 않게하는 방법](#파일-출력이-섞이지-않게하는-방법)
- [동기화된 I/O](#동기화된-io)
	- [레이턴시 (latency)](#레이턴시-latency)
	- [수동 동기화](#수동-동기화)
	- [sync](#sync)
	- [fsync](#fsync)
	- [fdatasync](#fdatasync)
	- [자동 동기화](#자동-동기화)
	- [넌 블로킹](#넌-블로킹)
	- [비동기적 I/O](#비동기적-io)
	- [posix\_fadvise](#posix_fadvise)
- [FILE 구조체](#file-구조체)
	- [setvbuf](#setvbuf)
- [XDR](#xdr)
- [패딩](#패딩)
- [바이너리 데이터 입출력](#바이너리-데이터-입출력)
- [LFS](#lfs)


## 서론  
파일 입출력은 크게 저수준과 고수준으로 나뉘며 각 방식의 차이점, 고려해야할 점을 설명하였다.  
동기화 방법, 타 플랫폼 간 통신에서 발생할 수 있는 문제점 및 해결 방법 등도 함께 서술하였다.  
파일도 자원이기 때문에 누수가 발생할 수 있으므로 적절히 닫아야 하며,  
열고 닫는 행위에도 버퍼를 할당하므로 오버헤드를 발생 시킬 수 있다.  
하나의 파일에 여러 채널을 열면 순서 역전 등 문제가 발생할 수 있다.  

## 분류별 함수 목록  
### 저수준 함수  
<img src="img/%EC%A0%80%EC%88%98%EC%A4%80_1.jpg" width=500></img></br>
<img src="img/%EC%A0%80%EC%88%98%EC%A4%80_2.jpg" width=500></img>

### 고수준 함수  
<img src="img/%EA%B3%A0%EC%88%98%EC%A4%80.jpg" width=500></img>

### 그 외의 파일 관련 함수  
<img src="img/etc.jpg" width=500></img>


## 원자적 실행  
- 원자성 (atomicity) : 더 이상 쪼개질 수 없는 성질  
- 원자적 행위(atomic action)  
더 이상 나누어질 수 없는 하나의 행위  
도중에 중단될 수 없는 하나의 동작 단위  
8장 스레드프로그래밍 - 스레드


## 저수준 파일처리  
- 파일 기술자(file descriptor) 번호를 사용하여 입출력을 진행한다.  
- 유닉스 및 호환 계열에서만 사용 가능하다.  
- 비직관적인 함수 형태  
- 보편적으로 사용됨   
- pread, pwrite를 통해 원자적 실행을 보장한다.


## 고수준 파일처리  
- FILE 구조체를 사용하여 입출을 진행한다.   
- C 언어를 지원하는 모든 플랫폼에서 사용 가능  
- 직관적이고 사용이 편리한 함수 형태  
- 라이브러리 레벨의 버퍼링이 있음  
- 사용자 변수와 버퍼, 버퍼와 커널 간의 복사가 일어나기 때문에 메모리 대역폭을 비효율적으로 사용한다.  
- 따라서 응답성과 성능을 중시한다면 고수준 파일처리는 최소한으로 사용해야한다.


## 원자성의 보장 방법
멀티 스레드 환경에서 일반적인 입출력은 커서를 공유하여 사용하기 때문에 원자성을 보장할 수 없다.  

### 파일 입출력	
- pread, pwrite 함수를 사용한다.  
- 절대적 오프셋 주소를 사용하기 때문에 보장이 가능해진다.  
  
### 파이프 입출력
- PIPE_BUF 이내의 길이로 입출력을 진행한다.  
- 시스템에 따라 크기가 달라지며, 리눅스에서는 보통 4096byte 이다.  
- 페이지 단위에 영향을 받는 경우가 많아 4096의 배수가 사용된다.  


## 파일 출력이 섞이지 않게하는 방법  
1. 출력 함수의 원자적 실행을 보장한다.  
	- 저수준의 파일 처리를 사용해야한다.  
2. lock를 이용해 출력 과정을 보호한다.
	- 성능을 해칠 수 있다.
	- 간단하다.
3. 직렬화를 이용해 전문적으로 출력을 하는 프로세스나 스레드를 둔다.  
	- 설계 과정이 복잡하고 무거워진다.  
	- 신뢰성이 높고 응딥이 좋다.  
4. 메모리 맵파일(mmap)를 사용한다.
	- 5장 IPC - mmap


## 동기화된 I/O  
I/O 연산은 일반적으로 커널의 버퍼 캐시나 페이지 캐시를 거쳐 처리된다.  
프로세스가 파일에 데이터를 기록하면, 커널은 해당 데이터를 하나의 버퍼에 복사한 후 내부적인 대기열에 등록하고, 적절한 시점에 디스크에 기록한다.  
이렇게 되면 데이터는 버퍼에만 존재하고 아직 디스크에는 기록되지 않은 상태가 된다.  
몇 가지 예외적인 상황에서는 즉각적인 내용 반영이 필요할 수 있는데, 이때 동기화를 사용한다.  
동기화 사용 시 응답성과 시스템의 성능이 떨어질 가능성이 있다.

### 레이턴시 (latency)  
자극과 반응 사이의 시간  
물리적 반응에 대한 원인과 결과 간의 지연 시간

### 수동 동기화  
동기화 시점에 fsync, fdatasync 함수를 호출한다.  

### sync  
	void sync(void)
**Description**  
모든 버퍼를 디스크에 쓰도록 명령한다.  
모든 버퍼의 내용과 메타데이터를 디스크와 동기화한다.  
연산이 완료될 때까지 기다리지 않는다.  

### fsync  
	int fsync(int fd)
**Parameters**  
- ```int fd```	: 동기화할 파일디스크립터

**Return Value**  
- `0`	: 성공
- `-1`	: 실패

**Description**  
해당 디스크립터의 버퍼를 디스크에 쓴다.  
해당 fd의 내용과 메타데이터를 디스크와 동기화한다.  
연산이 완료될 때까지 기다린다.  

### fdatasync  
	int fdatasync(int fd)
**Parameters**  
- ```int fd```	: 동기화할 파일디스크립터

**Return Value**  
- `0`	: 성공
- `-1`	: 실패

**Description**  
해당 디스크립터의 버퍼를 디스크에 쓴다.  
해당 fd의 내용만 디스크와 동기화한다.  
연산이 완료될 때까지 기다린다.  

### 자동 동기화  
[open 함수](../etc.md#open)로 파일을 열 때 옵션 플래그를 설정한다.  
| 옵션		| 설명 |
| :---: 	| :--- |
| O_SYNC	| 쓰기가 완료될 때까지 대기 |
| O_DSYNC 	| 메타 데이터를 빼고 동기화 한다. |
| O_RSYNC 	| 읽기 작업에 대해서도 동기화된 I/O를 사용한다. |

### 넌 블로킹
- 함수 호출 시 제어권을 넘겨주지 않고 실행하는 방법
- 6장 I/O 인터페이스 - 네트워킹

### 비동기적 I/O  
- 10장 리얼타임 확장  

### posix_fadvise  
	int posix_fadvise(
		int		fd,
		off_t		offset,
		off_t		len,
		int		advice
	)

**Parameters**  
- `int	fd`	: 파일 Descriptor
- `off_t	offset`	: 시작 번호
- `off_t	len`		: 읽을 길이	(0이면 끝까지)
- `int	advice`		: 접근할 패턴 조언  
  
| 옵션	| 설명 |
| :---: | :--- |
| POSIX_FADV_NORMAL	| 열린 파일에 아무 조언도 주지 않는다. |
| POSIX_FADV_SEQUENTIAL	| 순차적으로 접근한다. |
| POSIX_FADV_RANDOM	| 지정된 데이터에 임의 순서로 접근한다. |
| POSIX_FADV_NOREUSE	| 데이터에 한번만 접근한다. |
| POSIX_FADV_WILLNEED	| 지정한 데이터에 곧 접근한다.</br>지정 영역을 페이지 캐시로 읽어들이는 논블록 동작을 개시한다.|
| POSIX_FADV_DONTNEED	| 지정한 데이터에 접근하지 않는다.</br>지정 영역과 연계된 캐싱 페이지를 해제 시도한다. |  

**Return Value**  
- `other`	: 에러번호
- `0`	: 성공

**Description**  
fd와 연결된 파일의 데이터 처리와 애플리케이션의 예상 동작에 대해 조언 한다.  
len이 0이라면 offset 부터 모든 데이터를 지정한다.  
5장 IPC - mmap  


## FILE 구조체  
파일의 정보를 담을수 있는 자료형  
파일의 위치, 버퍼의 사이즈 등 다양한 정보를 저장한다.  
파일 스트림	: FILE 구조체를 통해 얻어지는 파일 입출력 매개물  

### setvbuf  
	int setvbuf(
		FILE 		*stream, 
		char 		*buf, 
		int 		type, 
		size_t 		size
	)

**Parameters**  
- `FILE	*stream`	: 입출력 스트림
- `char	*buf`		: 사용할 버퍼
- `int	type`		: 버퍼링 옵션 
  - | 옵션		| 설명 |
  	| :---: 	| :--- |
	| _IONBF	| 사용된 버퍼가 없음 |
	| _IOFBF 	| buf를 버퍼로 사용하고 size를 버퍼 크기로 사용 |
	| _IOLBF	| 행 버퍼링 사용</br>행 버퍼링	: 줄바꿈, 버퍼 가득참, 입력 요청 시 버퍼를 비움 |
  - 행 버퍼링	: 줄바꿈, 버퍼 가득참, 입력 요청 시 버퍼를 비움
- `size_t	size`		: 버퍼의 크기

**Return Value**  
- `other`	: 에러번호
- `0`	: 성공

**Description**  
버퍼링 방식과 버퍼의 크기를 설정한다.  
buf 인자가 NULL 이라면 함수에 요청된 크기만큼 메모리를 할당하며 스트림 버퍼로 사용한다.  


## XDR
XDR(External Data Representation)  
기계나 운영체제에 상관없이 일관된 데이터 표현 방식을 제공하는 프로토콜이다.  
서로 다른 시스템 간 데이터를 교환할 때 발생할 수 있는 데이터 형식의 불일치나 바이트오더 등의 문제를 방지할 수 있다.  
주소 경계를 특정 단위의 배수로 정하는 규칙이다.  
빅엔디안을 사용한다. (낮은 주소부터 저장)  
유닛의 크기를 4바이트(32bit)로 권고한다.  


## 패딩  
구조체의 경우 경계를 맞추기 위해 패딩이 삽입되기도 한다.  
멤버 변수의 순서에따라 패딩의 크기와 위치가 달라질 수 있다.  
멀티 스레드나 시그널 프로그래밍에서 가짜 공유와 캐시미스를 줄일 수 있다.  
구조체 통신을 하는경우 바이트 단위 처리를 하는 언어와 호환성 유지를 위해서 묵시적인 패딩이 존재하는지 확인 후 통신 해야한다.  
구조체에 __attribute__((packed)) 키워드를 붙여 묵시적인 패딩을 제거할 수 있다.  
</br>
묵시적인 패딩	: 프로그램에서 크기를 맞춰주기 위해 자동으로 삽입하는 패딩  
명시적인 패딩	: 프로그래머가 크기를 명확하게 하기위해 명시적으로 삽입하는 패딩  
가짜공유	: 실제로 공유되고 있지 않은 캐시 데이터를 동기화하는 행위 또는 그로인해 발생하는 성능 하락  
Pack(압축)	: 패딩을 추가하지 않고 멤버 변수들을 채우는 것


## 바이너리 데이터 입출력   
팩화된 경우, 정렬되어 패딩이 발생하는 경우가 생길 수 있으므로 읽거나 쓸 때 정확한 위치 경계를 잡아야 한다.  
정적 캐스팅으로 데이터를 읽어오거나 사용할 때 버스 에러 문제점이 생길 수 있다.  
XDR 규약을 지켜야 한다.  


## LFS
LFS(Large File Summit/Support)	: 대용량 파일 지원  
32비트 시스템에서 2GB 이상의 어드레싱 공간을 사용하기위한 기술적 지원  
파일 시스템 포맷이 LFS를 지원해야하며, 라이브러리가 LFS를 지원해야 한다.  

- 32bit와 64bit 파일 관련 함수를 따로 사용하기
  - 기존의 함수와 64bit 함수를 병행하여 사용한다.
    - #define _LARGEFILE_SOURCE
    - #define _LARGFILE64_SOURCE
- 32bit 함수를 64bit로 모두 변환하기
  - 기존 함수의 수정 없이 컴파일 시 동일한 버전으로 변환된다.
    - #define _LARGEFILE_SOURCE
    - #define _FILE_OFFSET_BITS 64
