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
	- [mremap](#mremap)
	- [memory advice](#memory-advice)
	- [posix\_madvise](#posix_madvise)
- [Huge Page](#huge-page)
- [공유메모리](#공유메모리)
- [세마포어](#세마포어)
	- [세마포어와 뮤텍스](#세마포어와-뮤텍스)
	- [세마포어 vs 뮤텍스](#세마포어-vs-뮤텍스)
- [메시지큐](#메시지큐)
- [XSI IPC](#xsi-ipc)
	- [XSI IPC key](#xsi-ipc-key)
	- [제공 유틸리티](#제공-유틸리티)
	- [ftok](#ftok)
	- [IPC Get Flag](#ipc-get-flag)
	- [IPC Ctl Flag](#ipc-ctl-flag)
- [XSI 공유메모리](#xsi-공유메모리)
	- [shmget](#shmget)
	- [shmat](#shmat)
	- [shmdt](#shmdt)
	- [shmctl](#shmctl)
	- [struct shmid\_ds](#struct-shmid_ds)
- [XSI 세마포어](#xsi-세마포어)
	- [중요 값](#중요-값)
	- [semget](#semget)
	- [semop](#semop)
	- [semtimedop](#semtimedop)
	- [struct sembuf](#struct-sembuf)
	- [semctl](#semctl)
	- [union semun](#union-semun)
	- [struct semid\_ds](#struct-semid_ds)
	- [struct ipc\_perm](#struct-ipc_perm)
	- [struct seminfo](#struct-seminfo)
	- [wait-for-zero](#wait-for-zero)
	- [wait-for-zero의 4가지 이벤트](#wait-for-zero의-4가지-이벤트)
- [XSI 메시지 큐](#xsi-메시지-큐)
	- [메시지 큐 크기 확인](#메시지-큐-크기-확인)
	- [msgget](#msgget)
	- [struct msgbuf](#struct-msgbuf)
	- [msgsnd](#msgsnd)
	- [msgrcv](#msgrcv)
	- [msgctl](#msgctl)
	- [struct msqid\_ds](#struct-msqid_ds)
	- [struct msginfo](#struct-msginfo)
- [POSIX IPC](#posix-ipc)
- [POSIX 공유메모리](#posix-공유메모리)
	- [shm\_open](#shm_open)
	- [shm\_unlink](#shm_unlink)
- [POSIX 세마포어](#posix-세마포어)
	- [sem\_init](#sem_init)
	- [sem\_t \*sem\_open](#sem_t-sem_open)
	- [POSIX 세마포어 P,V 동작](#posix-세마포어-pv-동작)
	- [POSIX 세마포어 제거 동작](#posix-세마포어-제거-동작)
- [POSIX 메시지 큐](#posix-메시지-큐)
	- [메시지 큐 크기 확인](#메시지-큐-크기-확인-1)
	- [mq\_open](#mq_open)
	- [struct mq\_attr](#struct-mq_attr)
	- [POSIX 메시지 큐 삭제](#posix-메시지-큐-삭제)
	- [POSIX 메시지 큐 송수신](#posix-메시지-큐-송수신)
	- [POSIX 메시지 큐 이벤트 통지](#posix-메시지-큐-이벤트-통지)


## 서론
IPC(Inter Process Communication)  
프로세스 사이에서 통신을 가능하게 하는 메커니즘의 총칭  
파일, mmap, 세마포어, 공유메모리, 메시지 큐, 파이프, 소켓, 시그널 등의 모든 통신 기법이 이에 해당한다.  
공유 메모리, 메시지 큐, 소켓 순으로 속도가 빠르다.  


## mmap
mmap(memory mapped I/O)  
장치나 파일을 메모리와 대응시키는 기법  
복수의 프로세스가 같은 파일에 대해 mmap을 호출하면 가상주소는 달라도 물리적 주소는 동일하여 공유 효과를 가진다.  
rwx (read, write, execute) 프로텍션(권한)이 존재한다.  
공유 방식에 따라 공유 메모리맵, 사설 메모리맵으로 나뉜다.  
공유된 mmap을 사용할 때는 크리티컬 섹션 보호에 신경 써야 한다.  
메모리 해제 전 msync로 동기화를 마치고 해제하는 것이 좋다.  
대상 파일은 대응시킬 메모리의 크기보다 커야한다.  
만약 작은경우 ftruncate 함수를 사용해 늘린다.  
파일의 크기는 MMAP_SIZE 보다 커야한다.  

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
**Parameters**
- `void *start`
  - 가상 주소 시작 번지
  - 0으로 설정 시 자동 할당
- `size_t length`
  - 메모리의 크기
  - mmap의 최소 length 보다 커야함
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
**Parameters**
- `void *start`		: 메모리 시작 주소
- `size_t length`	: 해제할 길이

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 지정

**Description**  
현재 프로세스와 메모리의 연결을 끊는다.  
mmap 세그먼트를 삭제하지는 않는다.  
시스템의 모든 프로세스에서 사용을 종료한 경우에만 퇴출된다.  

### msync
	#include <sys/mman.h>
	int msync(
		void			*start,
		size_t			length,
		int			flags
	)
**Parameters**
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
	#include <sys/mman.h>
	int mprotect(void		*addr
		size_t			len
		int			prot
		int			pkey
	)
**Description**  
프로세스의 메모리 페이지에 대한 액세스 보호를 변경한다.  
페이지 경계에 정렬되어야 한다. (4096의 배수)  

### mremap
	#include <sys/mman.h>
	void *mremap(
		void			*old_address,
		size_t			old_size,
		size_t			new_size,
		int			flags
	)
**Description**  
기존 메모리매핑의 길이를 변경합니다.  
이 때 메모리의 위치는 이동될 수 있습니다.  


### memory advice
참조: [posix_fadvise](../week2/Chapter2%20File.md#posix_fadvise)  
BSD 표준과 POSIX 표준이 존재하며, 기능과 호환성면에서 차이가 있다.  
프로그래머의 의도에 따라 메모리를 어떻게 사용할지 힌트를 주는기능  

### posix_madvise  
	int posix_fadvise(
		void			*addr, 
		off_t			len, 
		int			advice
	)
**Parameters**  
- `void *addr`	: 조언할 메모리의 시작주소
- `off_t len`	: 조언할 메모리의 길이
- `int advice`  
	데이터 접근 조언
	| 옵션	| 설명 |
	| :---: | :--- |
	| POSIX_MADV_NORMAL	| 열린 메모리에 아무 조언도 주지 않는다.|
	| POSIX_MADV_SEQUENTIAL	| 순차적으로 접근한다. |
	| POSIX_MADV_RANDOM	| 지정된 메모리에 임의 순서로 접근한다.</br>TLB의 효율을 높여준다.|	
	| POSIX_MADV_WILLNEED	| 지정한 메모리에 곧 접근한다.</br>지정 영역을 페이지 캐시로 읽어들이는 논블록 동작을 개시한다.|
	| POSIX_FADV_DONTNEED	| 지정한 데이터에 접근하지 않는다.</br>지정 영역과 연계된 캐싱 페이지를 해제 시도한다. |  



## Huge Page
명시적으로 Huge Page를 사용하기 위해서는 커널 설정을 동반 해야한다.  
리눅스에서는 hugeadm 명령을 사용하여 설정을 변경할 수 있다.  
/proc/meminfo 파일의 HugePages_Total 속성을 참조하여 설정 여부를 알 수 있다.  
만약 설정되어있지 않다면 SIGBUG시그널이 발생하여 프로세스가 종료될 수 있다.  

THP(Transparent Huge Page)  
묵시적으로 일정 크기가 넘어가는 대용량 메모리맵에 대해 자동으로 Huge page를 적용하는 기능  
/sys/kernel/mm/transparent_hugepage/enabled 에서 설정을 볼 수 있다.  
소스 코드 수정 없이 Huge Page를 쓸 수 있게 해준다.  
THP 설정만으로 성능이 개선될 수 있다.  

| 속성	|설명|
| :--:	|:--|
| always	| THP 사용 |
| madvise	| madvise 함수를 적용한 mmap에 대해서만 사용 |
| never		| THP 사용안함 |



## 공유메모리
함수 콜 없이 주소번지에 직접 접근한다.  
입출력시 포인터 변수로 직접 접근하기 때문에 가장 빠른 성능을 보장한다.  

프로그래머가 배타적 접근을 보장해야한다.  
배타적 접근을 LOCK 매커니즘이라 하며 세마포어, 뮤텍스, rwlock, spinlock 등이 있다.  
8장 스레드 프로그래밍  



## 세마포어
초기 상호배제의 개념에서 만들어졌지만 현재는 동기화 및 락메커니즘 구현 도구를 가르키는 의미로 사용된다.  
공유 자원에 대한 교착상태를 해결하기 위한 기법이다.  

블록킹 모드로 작동하기 때문에 대기시간이 길어질 가능성이 있다.  
이를 해결하기위해 넌블록킹 모드에서 재시도 방식으로 코딩할 경우 CPU의 사용률이 치솟는 현상이 생긴다.  
타이머 작업 큐, semtimedop 사용으로 해결할 수 있다.  

P연산과 V 연산으로 이루어져있다.  
P연산	: 세마포어의 값을 감소시키는 연산 (wait, pend)  
V연산	: 세마포어의 값을 증가시키는 연산 (signal, post)  

|종류|특징|
|:--:|:--|
|카운팅		| 복수개의 자원 카운팅이 가능한 세마포어 |
|이진		| 1개의 자원 카운팅이 가능한 세마포어 |
|뮤텍스		| 자원의 독점을 가능하게 하는 락 |
|스핀락		| 문맥 교환을 막기위해 사용되는 매우 빠른 락 |
|Reader/Writer락	| 읽기 쓰기가 서로 다르게 적용되는 락 |  

락 메커니즘 : 8장 - 스레드 프로그래밍  


### 세마포어와 뮤텍스
카운팅 세마포어	: n개의 자원에 다수의 프로세스가 접근하는 것을 막아줌  
이진 세마포어	: 1개의 자원에 다수의 프로세스가 접근하는 것을 막아줌  
뮤텍스	: 자원의 독점적 사용권한을 부여하는 기능  
|		|XSI 세마포어|POSIX 세마포어|POSIX 뮤텍스|
|:--:		|:--|:--|:--|
| 최대 카운터		| 시스템 설정	| SEM_VALUE_MAX	| 1 |
| 독점적 소유권		| 불가능	| 불가능	| 가능 |
| 동작 취소 undo	| 가능		| 불가능	| 가능 |
| 타이머 설정		| 가능(비표준)	| 가능		| 가능 |


### 세마포어 vs 뮤텍스
세마포어는 동기화의 목적이고, 동기화를 위한 큐를 만드는 기능을 제공한다.  
뮤텍스는 독점적 사용권한 획득에 목적이 있으며, 소유권이 존재한다.  
일반적으로 뮤텍스가 세마포어보다 빠르다.  

동작 취소	: 잠금해제를 하지 않은 프로세스가 종료됐을 때, 데드락에 상태를 복구하는 기능  
뮤텍스의 데드락	: 중복으로 잠금을 한 경우, 뮤텍스를 획득한 스레드가 잠금 해제없이 종료한 경우 발생  



## 메시지큐
1 ~ 2KiB 이하의 짧은 메시지를 주고 받는데 매우 효율적인 통신 메커니즘  
모든 함수에서 스레드 안전을 만족한다.  
작은 메시지를 빈번하게 전송하는데 유리하다.  
수신측이 접속하지 않아도 데이터를 넣을 수 있다.  
큐가 가득차는 경우를 산정하여 프로그래밍 해야 한다.  
메시지 큐의 크기가 작아 송신속도가 빠르면 큐가 가득 차는 문제가 빈번히 발생한다.  
메시지 큐를 복수개로 만들어 사용하거나 수신측을 빠르게 처리할 수 있는 형태로 만들어야 한다.  


## XSI IPC
System V (System Five)에서 제공하는 IPC  
Key 획득 -> ID 획득 -> 자원 사용의 순서로 진행된다.  
IPC key	: IPC 자원에 접근하기 위한 해시 키  
IPC ID	: IPC key로 가져온 자원의 ID 값  

XSI IPC 자원들은 IPC key, IPC ID, 소유권자, 소유권한의 속성으로 이루어져있다.  
자원 획득 함수 사용시 생성을 진행하지만 이미 자원할당이 되어있다면 ID만을 알려준다.  
(Ubuntu 22.04 버전에서 사용중인것으로 확인 되었다.)  

### XSI IPC key
일반적으로 IPC key는 유일해야한다.  
key_t 형으로 표현되며 32bit 또는 64bit 정수형이다.  
키를 보유한 경우 외부에서 IPC 자원에 접근할 수 있다.  
키를 구한 후 semget, shmget, msgget 등을 호출하여 IPC ID를 얻어올 수 있다.  

임시로 사용되는 자원이라면 IPC_PRIVATE 매크로를 사용해 사설 IPC를 받아야 한다.  
사설 IPC의 키값은 0이며 호출할 때마다 ID가 랜덤생성된다.  
사설 IPC 또한 외부에서 접근 가능하다.  

### 제공 유틸리티
|유틸리티|설명|
|:--:|:--|
|ipcs	|IPC status</br>시스템의 XSI IPC 자원 리스트를 출력한다.|
|ipcrm	|IPC remove</br>시스템의 XSI IPC 자원을 제거한다.|
|lsipc	|list of IPC</br>XSI IPC 설정을 출력한다.|

### ftok
	#include <sys/types.h>
	#include <sys/ipc.h>
	key_t ftok(
		const char			*pathname,
		int				proj_id
	)
**Parameters**
- `const char *pathname`	: 실제로 존재하고 접속 가능한 파일 또는 디렉터리
- `int proj_id`			: 같은 path에 대해 구분하기 위한 값(번호)

**Return Value**
- `other`	: 성공
- `-1`		: 에러, errno 설정

**Description**  
System V IPC에서 사용할 key를 생성하는 함수  
매개변수값이 각각 동일하다면 같은 key를 생성합니다.  


### IPC Get Flag
IPC 자원을 얻을 때 사용하는 설정이다.  
shmget, semget, msgget 메소드에서 사용된다.  
|옵션	|설명|
|:--:	|:--|
|IPC_CREATE	| 해당 자원이 존재하지 않으면 생성한다.|
|IPC_EXCL	| 해당 자원이 존재한다면 에러(EEXIST)를 발생시킨다.|
|SHM_HUGETLB	| SHM에 Huge page를 사용한다. (리눅스에서만 지원)|
|접근권한	| 자원에 대한 접근 권한을 설정한다. ex)0660 |  


### IPC Ctl Flag
IPC 자원에 대한 정보를 얻거나 수정할 때 사용하는 명령이다.  
shmctl, semctl, msgctl 메소드에서 사용된다.  
|명령|설명|
|:--:|:--|
|IPC_RMID	| IPC 자원을 제거한다. |
|IPC_INFO	| 자원의 시스템 설정 값을 읽어온다. |
|IPC_SET	| IPC 자원의 권한을 변경한다. |
|IPC_STAT	| 현재 공유메모리 정보(생성자, 생성 시각, 접근 권한 등)를 읽어 buf에 저장한다. |
|SHM_LOCK	| 공유메모리의 세그먼트를 잠근다. |
|SHM_UNLOCK	| 공유메모리의 세그먼트를 잠금 해제한다. |  
|SETVAL		| 세마포어 세트 중 sumnum 위치의 세마포어 값을 초기화한다. |
|SETALL		| 세마포어 세트의 모든 세마포어를 한꺼번에 초기화한다. |
|GETVAL		| 세마포어 세트 중 semnum 위치의 세마포어 값을 리턴한다. |
|GETALL		| 세마포어 세트의 모든 세마포어를 한꺼번에 읽어온다. |
|GETNCNT	| 세마포어 세트 중 semnum 위치의 semnnt 정보를 리턴한다. |
|GETZCNT	| 세마포어 세트 중 semnum 위치의 semzcnt 정보를 리턴한다. |
|GETPID		| 세마포어 세트 중 semnum 위치의 sempid 값을 리턴한다. |


## XSI 공유메모리
	#include <sys/ipc.h>
	#include <sys/shm.h>


### shmget
	int shmget(
		key_t				key,
		int				size,
		int				shmflg
	)
**Parameters**
- `key_t key`	: 공유메모리 식별 키
- `int size`	: 공유메모리의 크기
- `int shmflg`	: [옵션](#ipc-get-flag)

**Return Value**
- `other`	: 공유메모리 식별 id
- `-1`		: 에러, errno설정

**Description**  
공유 메모리의 IPC ID를 얻는다.(없는경우 생성)  
공유 메모리의 크기가 너무 작은 경우 커널 설정에 따라 에러가 발생할 수 있다.  


### shmat
	void* shmat(
		int				shmid,
		const void			*shmaddr,
		int				shmflg
	)
**Parameters**
- `int shmid`		: 공유메모리 식별 id
- `const void *shmaddr`
  - 매핑할 메모리 주소
  - null : 매핑되지 않은공간에 자동으로 붙인다.
- `int shmflg`
  	메모리 매핑 옵션
	|옵션	|설명|
	|:--:	|:--|
	| SHM_RND	| 페이지주소를 반내림하여 경계에 맞춘다.|
	| SHM_RDONLY	| 읽기 전용으로 공유메모리에 연결한다.|
**Return Value**
- `other`	: 공유메모리 주소
- `-1`		: 에러, errno설정

**Description**  
지정한 공유메모리와 연결한다.  


### shmdt
	int shmdt(const void			*shmaddr)
**Parameters**
- `const void *shmaddr`	: 공유메모리 주소

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
공유메모리와 연결을 해제한다.  


### shmctl
	int shmctl(
		int				shmid,
		int				cmd,
		struct shmid_ds			*buf
	)
**Parameters**
- `int shmid`	: 공유메모리 식별 id
- `int cmd`	: [명령](#ipc-ctl-flag)
- `struct shmid_ds *buf`	: 공유메모리 정보를 저장하기위한 구조체

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
공유 메모리를 조작(제거, 메타 데이터 획득)한다.  


### struct shmid_ds
	struct shmid_ds {
		struct ipc_perm			shm_perm;		// 접근권한 
		int  				shm_segsz;		// 세그먼트의 크기(bytes)
		time_t				shm_atime;		// 마지막 접근 시간
		time_t				shm_dtime;		// 마지막 제거 시간
		time_t				shm_ctime;		// 마지막 변경 시간
		unsigned short			shm_cpid;		//생성자의 프로세스의 프로세스 id
		unsigned short			shm_lpid;		// 마지막으로 작동한 프로세스의 프로세스 pid
		short				shm_nattch;		// 현재 접근한 프로세스의 수
		
		unsigned short			shm_npages;		// 세그먼트의 크기(pages)
		unsigned long			*shm_pages;
		struct shm_desc			*attaches;		// 접근을 위한 기술자들 */
	}



## XSI 세마포어
	#include <sys/ipc.h>
	#include <sys/sem.h>


### 중요 값 
직접 접근은 불가능 하며, 관련 시스템 함수로만 참조 가능한 값
semval	: 현재 세마포어 값  
sempid	: 마지막으로 세마포어에 접근했던 프로세스의 PID  
semcnt	: 세마포어 카운트가 양수가 되기를 대기하는 프로세스의 개수  
semnzcnt	: 세마포어 카운트가 0이 되기까지 대기하는 프로세스의 개수  


### semget
	int semget(
		key_t				key,
		int				nsems,
		int				semflg
	)
**Parameters**
- `key_t key`	: 세마포어 식별 키
- `int nsems`	: 세마포어 자원의 갯수
- `int semflg`	: [옵션](#ipc-get-flag)

**Return Value**
- `other`	: 세마포어 식별 id
- `-1`	: 에러, errno 설정

**Description**  
세마포어 객체를 생성하고 열거나 기존에 생성된 객체를 연다.  


### semop
	int semop(
		int				semid,
		struct sembuf			*sops,
		size_t				nsops
	)
**Parameters**
- `int semid`		: 새마포어 식별 id
- `struct sembuf *sops`	: 세마포어 작동 버퍼 구조체의 주소
- `size_t nsops`	: 버퍼 구조체의 개수

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
세마포어 값을 변경한다.  


### semtimedop
	int semtimedop(
		int				semid, 
		struct sembuf 			*sops, 
		size_t 				nsops,
		const struct timespec 		*timeout
	)
**Parameters**
- `int semid`		: 세마포어 식별 id
- `struct sembuf *sops`	: 세마포어 작동 버퍼 구조체의 주소
- `size_t nsops`	: 버퍼 구조체의 개수
- `struct timespec *timeout`	: timeout 시간을 저장한 구조체

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
semop함수에 타임아웃 기능이 추가된 함수이다.  


### struct sembuf
	struct sembuf{
		unsigned short 			sem_num;		// 세마포어 세트의 개별 인덱스 번호, 0부터 시작 
		short				sem_op;			// 세마포어에 더할 값 (음수, 양수, 0)
		short				sem_flg;		// 작동옵션 플래그
	}
| 옵션		|설명|
| :--:		| :-- |
| SEM_UNDO	| 세마포어를 조작한 프로세스가 종료되었을 때 조작된 작업은 취소된다.</br>대기하던 다음 세마포어가 실행된다. |
| SEM_NOWAIT	| 사용 가능자원이 없는 경우 바로 에러를 리턴한다.(넌블로킹 실행) |


### semctl
	int semctl (
		int 				semid,
		int 				semnum,
		int 				cmd,
		...
		union semnum			
	)
**Parameters**
- `int semid`	: 세마포어 식별 id
- `int semnum`	: 인덱스
- `int cmd`	: [명령](#ipc-ctl-flag)

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
세마포어를 조작(제거, 메타 데이터 획득, 초기화)한다.  
cmd 값에 따라 가변인수의 사용 여부가 결정된다.  
네번째 인자는 세마포어 조작 공용체 semun 타입이다.  
semun 공용체는 사용자가 선언하여 사용하도록 되어있다.  
다만 호환성을 위해 선언 권고사항이 있으며 다음과 같다.  

### union semun
```cpp
	#include<sys/sem.h>
	#if defined(__GNU__LIBRARY__) && !(_SEM_SEMUM_UNDEFIND)
	/* union semun is defined by including <sys/sem.h> */
	#else
	/* according to X/OPEN we have to define it ourselves */
	union semun {
		int val;			/* Value for SETVAL */
		struct semid_ds *buf;		/* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;		/* Array for GETALL, SETALL */
		struct seminfo  *__buf;		/* Buffer for IPC_INFO(Linux-specific) */
	};
	#endif
```

### struct semid_ds
	struct semid_ds{
		struct ipc_perm			sem_perm;	// 권한 구조체
		time_t				sem_otime;	// 마지막 semop() 시간
		time_t				sem_ctime;	// 마지막 semctl() 변경시간 
		unsigned long int		sem_nsems;	// 세마포어 개수
	}

### struct ipc_perm
	struct ipc_perm{
		key_t				__key;		// key (리눅스 전용)
		uid_t				uid;		// 소유자 uid
		gid_t				gid;		// 소유자 gid
		uid_t				cuid;		// 생성자 uid
		gid_t				cgid;		// 생성자 gid
		unsigned short int		mode;		// 읽기/쓰기 권한
		unsigned short int		__seq;		// 시퀀스 번호 (리눅스 전용)
	}

### struct seminfo
	struct seminfo{
		int				semmap;		// 엔트리 맵 개수
		int				semmni;		// 최대 세트 개수
		int				semmns;		// 세마포어 최대 개수
		int				semmnu;		// undo 구조체 최대 개수
		int				semmsl;		// 한 세마포어세트 내의 최대 세마포어 개수
		int				semopm;		// semop 콜의 최대 값
		int				semume;		// 프로세스당 최대 undo 엔트리 개수
		int				semusz;		// undo 구조체가 사용하는 메모리 크기
		int				semvmx;		// 세마포어 값의 최대 값
		int				semaem;		// 프로세스 종료시 복구될 수 있는 undo의 최대 값
	}


### wait-for-zero
세마포어 호출 시 0을 더하도록 하면 wait-for-zero 라는 특수한 작동을 실행한다.  
기존 방식과 반대로 세마포어 값이 0이되면 깨어나게 된다.  
사용 가능한 자원이 없을 때 해야 하는 작업을 처리하는 경우 사용된다.  
로드 밸런싱 모니터링, 대기큐의 혼잡 방지 배리어 등의 용도에 사용된다.  


### wait-for-zero의 4가지 이벤트
1. 세마포어 값이 0이 되는경우
2. 대기하던 프로세스가 시그널을 받는 경우
   - 시스템 콜 실패
   - errno EINTR 설정
3. 세마포어 세트가 시스템에서 제거된 경우
   - 시스템 콜 실패
   - errno EIDRM 설정
4. 타임아웃 만료에 의해 깨어난 경우
   - 시스템 콜 실패
   - errno EAGAIN 설정


## XSI 메시지 큐
	#include <sys/ipc.h>
	#include <sys/msg.h>
메시지 타입을 선택할 수 있다.  
때문에 채널을 분리하거나 우선순위 용도로 사용된다.  

| 함수명 | 기능 |
| :--: | :-- |
| msgget	| 메시지 큐의 IPC ID를 얻는다. |
| msgsnd	| 메시지 큐에 데이터를 송신한다. |
| msgrcv	| 메시지 큐의 데이터를 수신한다. |
| msgctl	| 메시지 큐를 조작한다. |


### 메시지 큐 크기 확인
msgctl 메소드를 사용하거나 리눅스 명령을 통해 확인할 수 있다  
`sysctl -a | grep kernel.msg`  

### msgget
	int semget(
		key_t				key,
		int				msgflg
	)
**Parameters**
- `key_t key`	: 메시지 큐 식별 키
- `int msgflg`	: [옵션](#ipc-get-flag)

**Return Value**
- `other`	: 메시지 큐 식별 id
- `-1`	: 에러, errno 설정

**Description**  
메시지 객체를 생성하고 열거나 기존에 생성된 객체를 연다.  


### struct msgbuf
	struct msgbuf{
		long				mtype;		// 0 보다 큰 숫자
		char				mtext[size]	// 메시지가 들어갈 적당한 크기의 배열
	}
**Description**  
전송할 메시지를 담는 구조체

### msgsnd
	int msgsnd(
		int				msqid,
		const void			*msgp,
		size_t				msgsz,
		int				msgflg
	)
**Parameters**
- `int msqid`		: 메시지 큐 식별 id
- `const void *msgp`	: 송신할 데이터 (structg msgbuf를 캐스팅해서 전달)
- `size_t msgsz`	: 송신할 데이터 크기
- `int msgflg`
  - 메시지 플래그
  - IPC_NOWAIT	: 넌블럭킹 모드로 작동한다.

**Return Value**
- `0`	: 전송 성공
- `-1`	: 에러, errno 설정

**Description**  
메시지큐에 메시지를 송신한다.  
메시지크기가 너무 크거나 큐가 가득차면 실패한다.  
msgp 매개변수는 char형으로 사용하는게 아닌 struct msgbuf를 캐스팅해서 전달해야 한다.  

### msgrcv
	int msgrcv(
		int				msqid,
		void				*msgp,
		size_t				msgsz,
		long				msgtyp,
		int				msgflg
	)
**Parameters**
- `int msqid`		: 메시지 큐 식별 id
- `const void *msgp`	: 수신할 데이터
- `size_t msgsz`	: 수신할 데이터 크기
- `long msgtyp`
  - 수신받을 메시지 타입  

  | 값		| 의미 |
  | :--: 	| :-- |
  |양수		| 양수와 일치하는 메시지 타입만 수신한다. |
  |0		| 메시지 타입을 무시하고 큐에 있는 메시지를 입력된 순서대로 수신한다. |
  |음수		| 음수의 절대값과 같거나 작은 숫자를 순서대로 받는다. |

- `int msgflg`
  - 메시지 플래그  

  | 속성 	| 설명 |
  | :--: 	| :-- |
  | IPC_NOWAIT	| 넌블럭킹 모드로 작동한다. |
  | MSG_NOERROR | 수신 메시지가 MSGSZ 보다 클 때 에러 대신 초과부분을 잘라버린다. |
  | MSG_EXCEPT	| 메시지 타입 인수를 초과하는 메시지만 수신한다. |


**Return Value**
- `other`	: 수신한 메시지 크기
- `-1`	: 에러, errno 설정
  
**Description**  
메시지큐에 있는 메시지를 수신한다.  
아무 메시지도 없으면 곧바로 리턴된다.  

### msgctl
	int semctl (
		int 				msgid,
		int 				cmd,
		struct msqid_ds			*buf
	)
**Parameters**
- `int semid`	: 메시지 식별 id
- `int cmd`	: [명령](#ipc-ctl-flag)
- `struct msqid_ds *buf`
  - 결과를 받아올 버퍼
  - IPC_INFO 명령 시 msginfo를 msqid_ds로 캐스팅해야한다.

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
메시지 객체를 조작(제거, 메타 데이터 획득)한다.  


### struct msqid_ds
	struct msqid_ds{
		struct ipc_perm			msg_perm;
		time_t				msg_stime;	// 마지막 msgsnd() 호출시간
		time_t				msg_rtime;	// 마지막 msgrcv() 호출시간
		time_t				msg_ctime;	// 마지막 큐 변경(생성) 시간
		unsigned long			msg_cbytes;	// 현재 큐 바이트
		msgqnum_t			msg_qnum;	// 현재 큐 메시지 수
		msglen_t			msg_qbytes;	// 최대 큐 바이트
		pid_t				msg_lspid;	// 마지막으로 msgsnd() 호출한 pid
		pid_t				msg_lrpid;	// 마지막으로 msgrcv() 호출한 pid
	}
[struct ipc_perm](#struct-ipc_perm)



### struct msginfo
	struct msginfo{
		int				msgpool;
		int				msgmap;
		int				msgmax;		// 메시지 하나의 최대 크기
		int				msgmnb;		// 메시지 큐 한개의 용량
		int				msgmni;		// 생성 가능한 메시지 큐 최대 개수
		int				msgssz;
		int				msgtql;
		unsigned short			msgseg;
	}

**Description**  
주석이 없으면 사용되지 않거나 의미없는 설정이다.  


## POSIX IPC
저수준 파일 입출력과 구성이 비슷하여 XSI보다 직관적이다.  
확장성을 고려하여 기존의 구조와 비슷하게 구성된다.  
[posix](../etc.md#posix)  



## POSIX 공유메모리
	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <fcntl.h>
메모리의 일부를 파티션 공간처럼 만들어 파일을 만들고 페이지를 공유하는 방식  
mmap파일 시스템과 POSIX의 공유 방식이 비슷하여 상호 변환이 가능하다.  
/dev/shm 이라는 가상메모리 디스크 영역을 사용한다.  


### shm_open
	int shm_open(
		const char			*name,
		int				oflag,
		mode_t				mode
	)
**Parameters**
- `const char *name`	: 만들거나 열 공유메모리 객체명 ("/파일명" 의 형태)
- `int oflag`		: [파일열기 속성](../etc.md#open)
- `mode_t mode`		: 파일 접근권한

**Return Value**
- `other`	: 파일디스크립터
- `-1`		: 에러, errno 설정

**Description**  
공유메모리 객체를 생성하고 열거나 기존에 생성된 객체를 연다.  
생성 이후에 mmap 함수를 통해 공유메모리를 매핑한다.  
close 함수를 통해 매핑된 공유메모리를 닫는다.  
[mmap](#mmap-1)  
[close](../etc.md#close)  


### shm_unlink
	int shm_unlink(const char *name)
**Parameters**
- `const char *name`	: 공유메모리를 삭제할 객체명

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
공유 메모리를 삭제한다.  
unlink함수와 유사한 구조를 갖는다.  



## POSIX 세마포어
	#include<semaphore.h>  
한번에 +1 또는 -1만 연산할 수 있다.  
접근 방법에 따라 명명된 세마포어, 익명 세마포어로 나뉜다.  
무한 대기 상태가 발생할 수 있으므로 넌블럭킹 모드를 사용하는게 신뢰성을 높일 수 있다.  
 
명명된 세마포어는 외부에서 접근 가능한 인터페이스 경로를 가진다.  
명명된 세마포어는 이름을 알고 있다면 다른 프로세스에서도 접근 가능한 방식이다.  

익명 세마포어는 메모리 기반 세마포어라 불린다.  
해당 세마포어를 생성 및 초기화한 프로세스에서만 유효하다.  
외부에서 접근 불가능하다.  

|함수|설명|
| :--:		| :-- |
| sem_init	| 익명 세마포어를 생성 후 초기화 한다. |
| sem_destroy	| 익명 세마포어를 제거한다. |
| sem_open	| 명명된 세마포어를 생성 후 초기화 하거나 연다. |
| sem_close	| 명명된 세마포어의 연결을 해제한다. |
| sem_unlink	| 명명된 세마포어를 시스템에서 제거한다. |
| sem_wait	| 세마포어 값을 1 감소시킨다. |
| sem_trywait	| sem_wait에 넌블러킹 기능을 추가한다 |
| sem_timedwait	| sem_wait에 타임아웃 기능을 추가한다. |
| sem_post	| 세마포어 값을 1 증가시킨다. |
| sem_getvalue	| 세마포어 카운터 값을 읽어온다. |


### sem_init
	int sem_init(
		sem_t				*sem,
		int				pshard,
		unsigned int			value
	)
**Parameters**
- `sem_t *sem`	: 세마포어 객체를 저장할 포인터
- `int pshard`	: 공유 설정 플래그
- `unsigned int value`	: 세마포어 초기 값

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
익명 세마포어를 생성한다.


### sem_t *sem_open
	sem_t *sem_open(
		const char *name,
		int oflag
		...
	)
**Parameters**
- `const char *name`	: 
- `int oflag`	: 

**Return Value**
- `other`	: 세마포어 주소 반환
- `SEM_FAILED`	: 에러, errno 설정

**Description**  
세마포어를 생성하거나 이미 존재하는 세마포어를 연다.  
open 메소드와 사용방법이 동일하다.  


### POSIX 세마포어 P,V 동작
```cpp
int sem_wait(sem_t *sem);		// 블로킹 실행
int sem_trywait(sem_t *sem);		// 넌블로킹 실행
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);	// 타임아웃 실행
int sem_post(sem_t *sem);		// 세마포어 증가(잠금 해제)
struct timespec{
	time_t		tv_sec;		// 초
	long		tv_nsec;	// 나노초
}
```
**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
시그널 인터럽트로 깨어나면 EINTR 에러가 발생하며, 이를 꼭 해결해야한다.  
무한 대기 상태가 발생할 수 있으므로 넌블럭킹 모드를 사용하는게 신뢰성을 높일 수 있다.  
post시 EOVERFLOW 에러가 발생하면 계속 세마포어를 증가시키는 구간이 있는지 확인해야 한다.  
timespec는 절대시간에 대한 구조체로 유닉스 시간에 타임 아웃을 주고싶은 시간만큼 더하는 작업을 해야한다.  
ex) time(NULL) + 10;  


### POSIX 세마포어 제거 동작
```cpp
int sem_destroy(sem_t *sem);		// 익명 세마포어 제거
int sem_close(sem_t *sem);		// 명명된 세마포어 닫기
int sem_unlock(const char *name);	// 명명된 세마포어 제거
```

**Return Value**  
- `0`	: 성공
- `-1`	: 에러. errno 설정


## POSIX 메시지 큐
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <mqueue.h>
이벤트 통지 기능을 쓸 수 있다.  
데이터 수신 시 시그널 발생, 콜백 스레드 생성 등의 작동이 가능하다.  
비동기적 구조로 작동시킬 때 편리하다.  
가 도착했을 때 통지 기능을 이용한다. |

mqd_t	: 메시지 큐 기술자  
공유 메모리 기술자를 사용하여 처리함  
저수준 파일 처리와 동일한 메커니즘  
mq_overview 메뉴얼 참고  

### 메시지 큐 크기 확인
mq_open 시 큐의 크기를 지정할 수 있으며 이 크기는 커널 설정보다 클 수 없다.  
커널 설정을 확인하는 리눅스 명령이다.  
`sysctl -a | grep mqueue`  

| 함수명 | 기능 |
| :--: | :-- |
| mq_open	| 메시지 큐의 객체를 얻는다. |
| mq_close	| 메시지 큐를 닫는다. |
| mq_unlink	| 메시지 큐를 시스템에서 제거한다. |
| mq_send	| 메시지 큐에 데이터를 송신한다. |
| mq_timedsend	| mq_send에 타임아웃 기능이 추가된 함수이다. |
| mq_receive	| 메시지 큐로부터 데이터를 수신한다. |
| mq_timedreceive	| mq_receive에 타임아웃 기능이 추가된 함수이다. |
| mq_setattr	| 메시지 큐의 속성을 설정한다. |
| mq_getattr	| 메시지 큐의 속성을 읽어온다. |
| mq_notify	| 메시지 큐에 데이터
### mq_open
	mqd_t mq_open(
		const char			*naem,
		int				oflag,
		...
		mode_t				mode,
		struct mq_attr			attr
	)
**Parameters**
- `const char *name`	: 만들거나 열 메시지 큐 객체명 ("/파일명" 의 형태)
- `int oflag`		: [파일열기 속성](../etc.md#open)
- `mode_t mode`		: 파일 접근권한
- `struct mq_attr attr` : 메시지 큐 속성

**Return Value**
- `other`	: 파일디스크립터
- `-1`		: 에러, errno 설정

**Description**  
공유메모리 객체를 생성하고 열거나 기존에 생성된 객체를 연다.  
생성 이후에 mmap 함수를 통해 공유메모리를 매핑한다.  
close 함수를 통해 매핑된 공유메모리를 닫는다.  


### struct mq_attr
	struct mq_attr{
		long int			mq_flags;	// 메시지 큐 플래그 (O_NONBLOCK)	
		long int			mq_maxmsg;	// 메시지 큐의 최대 메시지 개수 제한
		long int			mq_msgsize;	// 메시지 1개의 최대 용량
		long int			mq_curmsgs;	// 현재 큐에 저장된 메시지 개수
	}
**Description**  
mq_flags값은 변경 불가능 하다.  
mq_flags 값은 oflag와 mq_setattr 함수에서 변경할 수 있다.  
mq_maxmsg와 mq_msgsize는 커널의 영향을 받으므로 크게 설정하면 에러가 발생한다.  


### POSIX 메시지 큐 삭제
	int mq_close(mqd_t);
	int mq_unlink(const char *name);
	
### POSIX 메시지 큐 송수신
``` cpp
int	mq_send		(mqd_t mqdes, const char *msg_ptr,	size_t msg_len, unsigned int msg_prio);
ssize_t	mq_receive	(mqd_t mqdes, char *msg_ptr,		size_t msg_len, unsigned int *msg_prio);
int	mq_timedsend	(mqd_t mqdes, const char *msg_ptr,	size_t msg_len, unsigned int msg_prio,
			const struct timespec *abs_timeout);
ssize_t	mq_timedreceive	(mqd_t mqdes, char *restrict msg_ptr, 	size_t msg_len, unsigned int *restrict msg_prio,
			const struct timespec *restrict abs_timeout);
```
**Parameters**
- `mqd_t mqdes`			: 메시지 큐 기술자
- `char *msg_ptr`		: 메시지
- `size_t msg_len`		: 메시지 길이
- `unsigned int msg_prio`	: 우선순위
- `const struct timespec *restrict abs_timeout`  
	: 타임아웃 객체 절대시간  
	timespec는 절대시간에 대한 구조체로 유닉스 시간에 타임 아웃을 주고싶은 시간만큼 더하는 작업을 해야한다.  
ex) time(NULL) + 10; 

**Return Value**
- `other`	: 수신 길이
- `0`		: 송신 성공
- `-1`		: 에러, errno 설정


### POSIX 메시지 큐 이벤트 통지
	int mq_notify(
		mqd_t 				mqdes,
		cosnt struct sigevent		*norification
	)
**Parameters**
- `mqd_t mqdes`		: 메시지 큐 기술자
- `cosnt struct sigevent *norification`	: 

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
메시지 최초 도착 시 sigevent구조체에 지정한 이벤트를 자동으로 실행한다.  
일회성 이벤트 핸들러 이므로 한번 호출 후 삭제된다.  
이벤트 통지를 위해서는 재호출이 필요하다.  
10장 - 리얼타임 시그널 확장 - sigevent  
