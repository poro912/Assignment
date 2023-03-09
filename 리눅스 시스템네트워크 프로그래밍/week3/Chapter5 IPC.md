<h1> IPC </h1>

<h2> Index </h2>

- [서론](#서론)
- [mmap](#mmap)
	- [장점](#장점)
	- [더티페이지](#더티페이지)
	- [file-backed 메모리](#file-backed-메모리)
	- [shard mmap](#shard-mmap)
	- [privatge mmap](#privatge-mmap)
	- [mmap](#mmap-1)
	- [munmap](#munmap)
	- [msync](#msync)
	- [mprotect](#mprotect)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)
	- [mremap](#mremap)
- [**Parametters**](#parametters-1)
- [**Return Value**](#return-value-1)
	- [memory advice](#memory-advice)
- [공유메모리](#공유메모리)
- [세마포어](#세마포어)
- [메시지큐](#메시지큐)
- [sub-title](#sub-title)
	- [function\_name](#function_name)
- [**Parametters**](#parametters-2)
- [**Return Value**](#return-value-2)


## 서론
IPC(Inter Process Communication)  
프로세스 사이에서 통신을 가능하게 하는 메커니즘의 총칭  
파일, mmap, 세마포어, 공유메모리, 메시지 큐, 파이프, 소켓, 시그널 등의 모든 통신 기법이 이에 해당한다.  
공유 메모리, 메시지 큐, 소켓 순으로 속도가 빠르다.  


## mmap
mmap(memory mapped I/O)
장치나 파일을 메모리와 대응시키는 기법  
복수의 프로세스가 같은 파일에 대해 mmap을 호출하면 가상주소는 달라도 실제로 가리키는 물리적 주소는 동일하게 되어 공유의 효과를 가진다.  
rwx (read, write, execute) 프로텍션(권한)이 존재한다.  
공유 방식에 따라 공유 메모리맵, 사설 메모리맵으로 나뉜다.  
공유된 mmap을 사용할 때는 크리티컬 섹션 보호에 신경 써야 한다.  
메모리 해제 전 msync로 동기화를 마치고 해제하는 것이 좋다.  


### 장점 
스레드 안전을 만족한다.  
대응된 메모리 맵은 포인터로 접근하므로 사용이 쉽다.  
시스템 호출을 통하지 않고도 파일의 내용에 접근할 수 있다.  
메모리와 파일 사이의 동기화는 운영체제가 담당하므로 편리하다.  
대응된 메모리 맵의 크기를 넘어서는 경우 파일에 영향을 주지 않는다.  
일반 파일보다 더 오랜 시간 더티 페이지로 유지될 가능성이 커서 I/O 처리가 빈번한 파일의 경우 사용에 유리하다.  


### 더티페이지
페이지 캐시의 내용이 저장장치의 내용과 다른 페이지  
읽기 쓰기 작업이 일어나는 경우 pagecache에 임시 저장된다.  
이 때 한번에 하드에 저장되는 것이 아닌 메모리에 유지되다 조건에 만족하면 파일에 쓰게 된다.  
메모리에 유지된 상태에서 저장장치의 내용과 다른 부분을 더티페이지라 한다.  


### file-backed 메모리
페이지 중 특정 경로에 이미 원본이 존재하는 경우  
페이지 아웃 발생시 물리메모리에 있던 페이지는 백업을 위해 블록장치로 내려간다.  
읽기 전용 파일의 경우 이미 사본이 존재하기 때문에 페이지 아웃이 발생할 필요가 없다.  
위와같이 이미 파일의 백업본이 물리장치에 존재하는 경우를 file-backed 메모리라 부른다.  
실행파일, 동적라이브러리, 공용 데이터, 공용 이미지 파일 등이 이에 해당된다  


### shard mmap
공유 메모리맵  
다수 프로세스가 메모리를 공유하는 상태  
모든 프로세스가 변경된 내용을 같이 보게된다.  
한 프로세스가 mmap을 변경하면 동일한 mmap을 보는 다른 모든 프로세스도 변경된 내용을 보게 된다.  
단 동기화에는 시간차가 있을 수 있어 원본파일을 직접 읽을때는 최신의 데이터가 아닐 수 있다.  
항상 원본 파일이 블록 장치에 존재하기 때문에 스왑 아웃의 대상이 되지 않는다.  
직접 write 함수를 사용하는 것보다 성능이 좋다.  


### privatge mmap 
사설 메모리맵  
읽기 전용의 경우 메모리 공유에 목적이 있다.  
쓰기가 가능한 경우 메모리 공유의 목적 없이 초기값의 의미만 갖고있다.  
내용을 변경하더라도 대응했던 파일에는 아무런 변화가 생기지 않는다.  
쓰기 발생 전까지 공유를 진행하며, 쓰기가 발생하면 COW가 발생하여 추가적인 메모리를 요구한다.  
쓰기 발생 시 쓰기를 실행한 프로세스의 전용 페이지로 전환된다.  
내용 변경 이후 프로세스 고유의 메모리 공간이 되므로 스왑 아웃이 가능한 페이지로 전환된다.  
메모리가 낭비되거나 성능의 이점이 사라질 수 있다.  
mlock, madvise 등을 이용해 스왑 아웃을 금지하여 성능을 향상시킬 수 있다.  


### mmap
	#include <sys/mman.h>
	void * mmap(
		void 			*start,
		size_t			length,
		int			prot,
		int			flags,
		int			fd,
		off_t			offset
	)
**Parametters**
- `void *start`		: 가상 주소 시작 번지 (0으로 설정 시 자동 할당)
- `size_t length`	: 메모리의 크기 (mmap의 최소 length 보다 커야함)
- `int prot`  
  	메모리 보호 권한 설정 플래그  
	파일 기술자와 맞추거나 낮은 권한을 줄 것을 권장  
	| 플래그 | 의미 |
	| :--: | :-- |
	| PROTO_READ	| 읽기 가능 |
	| PROTO_WRITE	| 쓰기 가능 |
	| PROTO_EXEC	| 실행 가능 |
	| PROTO_NONE	| 접근 불가 |
- `int flags`  
	작동에 관련된 설정 플래그  
	| 플래그 | 의미 |
	| :--: | :-- |
	| MAP_SHARED	 | 공유 가능한 메모리맵으로 지정 |
	| MAP_PRIVATE	 | 사설 메모리 맵으로 지정 |
	| MAP_FIXED	 | 원하는 시작 번지를 지정할 때 사용 |
	| MAP_ANONYMOUS	 | 임시로 장치와 연결되지 않은 익명 mmap을 사용한다.</br>특정 주소에 페이지를 끼워 넣거나 순서대로 배열할 경우 사용</br>과거 코드와의 호환성을위해 fd는 -1로 설정한다. |
	| MAP_HUGETLB	 | 대용량 데이터를 처리할 수 있다.</br>커널 설정이 필요하다. |
	| MAP_LOCKED	 | 페이지 락을 이용한다.</br>mlock와 동일하다.</br>호환되지 않는 경우가 있어 mlock을 권장한다. |
	| MAP_UNINITIALIZED	 | 익명 mmap으로 할당된 공간을 초기화 하지 않는다.</br>성능을 중시하는 경우에 유용 |
- `int fd`		: 파일 디스크립터
- `off_t offset`	: sysconf(_SC_PAGE_SIZE)에서 반환한 페이지 크기의 배수(보통 0으로 지정)

**Return Value**
- `other`	: 할당된 메모리 주소
- `MAP_FAILED`	: 메모리 할당 실패, errno 지정

**Description**  
fd로 지정된 파일을 메모리 주소로 대응 시킨다.


### munmap
	#include <sys/mman.h>
	int munmap(
		void			*start,
		size_t			length
	)
**Parametters**
- `void *start`		: 메모리 시작 주소
- `size_t length`	: 해제할 길이

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 지정

**Description**  
현재 프로세스와 메모리의 연결을 끊는다.  
mmap 세그먼트를 삭제하지는 않는다.  
(시스템의 모든 프로세스에서 사용을 종료한 경우에만 퇴출된다.)  

### msync
	#include <sys/mman.h>
	int msync(
		void			*start,
		size_t			length,
		int			flags
	)
**Parametters**
- `void *start`		: 동기화할 메모리 시작 주소
- `size_t length`	: 동기화할 길이
- `int flags`  
	동기화 방법 설정 플래그  
	| 플래그 | 의미 |
	| :--: | :-- |
	| MS_ASYNC 	| 즉시 리턴</br>비동기로 동기화 진행</br>동기화는 예약될 뿐 보장할 수 없다. |
	| MS_SYNC 	| 동기화 종료시까지 대기</br>동기화는 바로 진행되며, 리턴과 동시에 완료가 보장된다. |
	| MS_INVALIDATE 	| 메모리에 쓰인 값을 무효화</br>파일에서 데이를 다시 로딩하여 덮어쓴다. |
**Return Value**
- `0`	: 성공
- `-1`	: 에러	errno 설정

**Description**  
start부터 length 길이만큼 동기화 한다.


### mprotect
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

### mremap
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  


### memory advice

## 공유메모리

## 세마포어

## 메시지큐


## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

