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
- [XSI IPC](#xsi-ipc)
	- [XSI IPC key](#xsi-ipc-key)
	- [제공 유틸리티](#제공-유틸리티)
	- [ftok](#ftok)
- [공유메모리](#공유메모리)
- [XSI 공유 메모리](#xsi-공유-메모리)
	- [IPC Flag](#ipc-flag)
	- [IPC 제어 명령](#ipc-제어-명령)
	- [shmget](#shmget)
	- [shmat](#shmat)
	- [shmdt](#shmdt)
	- [shmctl](#shmctl)
- [POSIX 공유메모리](#posix-공유메모리)
- [세마포어](#세마포어)
- [메시지큐](#메시지큐)


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


## XSI IPC
System V (System Five)에서 제공하는 IPC  
Key 획득 -> ID 획득 -> 자원 사용의 순서로 진행된다.  
IPC key	: IPC 자원에 접근하기 위한 해시 키     
IPC ID	: IPC key로 가져온 자원의 ID 값  
XSI IPC 자원들은 IPC key, IPC ID, 소유권자, 소유권한의 속성으로 이루어져있다.  
자원 획득 함수 사용시 생성을 진행하지만 이미 자원할당이 되어있다면 ID만을 알려준다.  
(Ubuntu 22.04 버전에서도 사용중인것으로 확인 되었다.)  


### XSI IPC key
일반적으로 IPC key는 유일해야한다.  
key_t 형으로 표현되며 32bit 또는 64bit 정수형이다.  
키를 사용하는 경우 외부에서 IPC 자원에 접근할 수 있다.  
키를 구한 후 semget, shmget, msgget 등을 호출하여 XSI IPC ID를 얻어올 수 잇다.  
임시로 사용되는 자원이라면 IPC_PRIVATE 매크로를 사용해 사설 IPC를 받아야 한다.  
사설 IPC의 키값은 0이며 호출할 때마다 ID가 랜덤생성된다.  
사설 IPC 또한 외부에서 접근 가능하다.  

### 제공 유틸리티
|유틸리티|설명|
|:--:|:--|
|ipcs	|IPC status</br>시스템의 XSI IPC 자원 리스트를 출력한다.|
|ipcrm	|IPC revmoe</br>시스템의 XSI IPC 자원을 제거한다.|
|lsipc	|list of IPC, XSI IPC 설정을 출력한다.|


### ftok
	#include <sys/types.h>
	#include <sys/ipc.h>
	key_t ftok(
		const char			*pathname,
		int				proj_id
	)
**Parameters**
- `const char *pathname`	: 실제로 존재하고 접속 가능한 파일 또는 디렉터리
- `int proj_id`			: 같은 path에 대해 구분하기 위한 값

**Return Value**
- `other`	: 성공
- `-1`		: 에러, errno 설정

**Description**  
System V IPC에서 사용할 key를 생성하는 함수  
매개변수값이 동일하다면 같은 key를 생성합니다.  


## 공유메모리
함수 콜 없이 주소번지에 직접 접근한다.  
입출력시 포인터 변수로 직접 접근하기 때문에 가장 빠른 성능을 보장한다.  
프로그래머가 배타적 접근을 보장해야한다.  
배타적 접근을 LOCK 매커니즘이라 하며 세마포어, 뮤텍스, rwlock, spinlock 등이 있다.  
8장 스레드 프로그래밍  

## XSI 공유 메모리
### IPC Flag
IPC 자원을 얻을 때 사용하는 설정이다.  
shmget, semget, msgget 메소드에서 동일하게 사용된다.  
|옵션	|설명|
|:--:	|:--|
|IPC_CREATE	| 해당 자원이 존재하지 않으면 생성한다.|
|IPC_EXCL	| 해당 자원이 존재한다면 에러(EEXIST)를 발생시킨다.|
|SHM_HUGETLB	| SHM에 Huge page를 사용한다. (리눅스에서만 지원)|
|접근권한	| 자원에 대한 접근 권한을 설정한다. ex)0660 |  

### IPC 제어 명령
IPC 자원에 대한 정보를 얻거나 수정할 때 사용하는 명령이다.  
shmctl, semctl, msgctl 메소드에서 동일하게 사용된다.  
|명령|설명|
|:--:|:--|
|IPC_RMID	| IPC 자원을 제거한다.|
|IPC_SET	| IPC 자원의 권한을 변경한다.|
|IPC_STAT	| 현재 공유메모리 정보(생성자, 생성 시각, 접근 권한 등)를 읽어 buf에 저장한다.|
|IPC_INFO	| 자원의 시스템 설정 값을 읽어온다.|
|SHM_LOCK	| 공유메모리의 세그먼트를 잠근다.|
|SHM_UNLOCK	| 공유메모리의 세그먼트를 잠금 해제한다.|  

### shmget
	#include <sys/ipc.h>
	#include <sys/shm.h>
	int shmget(
		key_t				key,
		int				size,
		int				shmflg
	)
**Parameters**
- `key_t	key`	: 키값
- `int		size`	: 공유메모리의 크기
- `int		shmflg`	: [옵션](#ipc-flag)

**Return Value**
- `other`	: id값
- `-1`		: 에러, errno설정

**Description**  
공유 메모리의 IPC ID를 얻는다.(없는경우 생성)  
공유 메모리의 크기가 너무 작은 경우 커널 설정에 따라 에러가 발생할 수 있다.  

### shmat
	#include <sys/ipc.h>
	#include <sys/shm.h>
	void* shmat(
		int				shmid,
		const void			*shmaddr,
		int				shmflg
	)
**Parameters**
- `int		shmid`		: id 값
- `const void	*shmaddr`
  - 매핑할 메모리 주소
  - null : 매핑되지 않은곳에 자동으로 붙인다.
- `int		shmflg`
  	메모리 매핑 옵
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
	#include <sys/ipc.h>
	#include <sys/shm.h>
	int shmdt(const void			*shmaddr)
**Parameters**
- `const void	*shmaddr`	: 공유메모리 주소

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
공유메모리와 연결을 해제한다.  


### shmctl
	#include <sys/ipc.h>
	#include <sys/shm.h>
	int shmctl(
		int			shmid,
		int			cmd,
		struct shmid_ds		*buf
	)
**Parameters**
- `int shmid`	: 공유메모리 id
- `int cmd`	: [제어 명령](#ipc-제어-명령)
- `struct shmid_ds *buf`	: 공유메모리 정보를 구하기 위한 버퍼포인터

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**  
공유 메모리를 조작(제거, 메타 데이터 획득)한다.  


## POSIX 공유메모리

## 세마포어


## 메시지큐




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
-->

