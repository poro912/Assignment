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
- [sub-title](#sub-title)
	- [function\_name](#function_name)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)


## 서론
I/O Interface  
외부와 자료를 교환하기 위한 기능을 의미한다.  
파일, FIFO, 파이프, 소켓, mmap, 공유메모리, 세마포어, 메시지 큐 등이 포함된다.  
그중 IPC를 제외한 파이프와 소켓에 대해 정리하였다.  



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
단방향 통신을으로 만들어지며, 양방향을 위해서는 두개의 파이프를 생성한다.  
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



## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

