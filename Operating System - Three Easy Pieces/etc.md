<h2> Index</h2>



LDE		제한적 직접 실행 
TLB





# 1장  
## 이론  
### 

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
