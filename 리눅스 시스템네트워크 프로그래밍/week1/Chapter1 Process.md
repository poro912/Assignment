# 프로세스

## Index
- [프로세스](#프로세스)
	- [Index](#index)
	- [서론](#서론)
	- [fork](#fork)
		- [pid\_t fork(void);](#pid_t-forkvoid)
	- [exec](#exec)
		- [int execl (const char \*path, const char \*arg, ... );](#int-execl-const-char-path-const-char-arg--)
		- [int execlp(const char \*file, const char \*arg, ... );](#int-execlpconst-char-file-const-char-arg--)
		- [int execle(const char \*path, const char \*arg, ..., char \*const envp\[\]);](#int-execleconst-char-path-const-char-arg--char-const-envp)
		- [int execv (const char \*path, char \*const argv\[\]);](#int-execv-const-char-path-char-const-argv)
		- [int execvp(const char \*file, char \*const argv\[\]);](#int-execvpconst-char-file-char-const-argv)
		- [int execve(const char \*path, char \*const argv\[\], char \*const envp\[\]);](#int-execveconst-char-path-char-const-argv-char-const-envp)
	- [posix\_spawn](#posix_spawn)
		- [int posix\_spawn(](#int-posix_spawn)
		- [int posix\_spawnp(](#int-posix_spawnp)
	- [posix\_spawn\_file\_action\_t 구조체](#posix_spawn_file_action_t-구조체)
		- [int posix\_spawn\_file\_actions\_init(posix\_spawn\_file\_actions\_t	\*file\_actions)](#int-posix_spawn_file_actions_initposix_spawn_file_actions_tfile_actions)
		- [int posix\_spawn\_file\_actions\_destroy(posix\_spawn\_file\_actions\_t	\*file\_actions)](#int-posix_spawn_file_actions_destroyposix_spawn_file_actions_tfile_actions)
		- [int posix\_spawn\_file\_actions\_addopen(](#int-posix_spawn_file_actions_addopen)
		- [int posix\_spawn\_file\_actions\_addclose(](#int-posix_spawn_file_actions_addclose)
		- [int posix\_spawn\_file\_actions\_adddup2(](#int-posix_spawn_file_actions_adddup2)
	- [posix\_spawnatter\_t 구조체](#posix_spawnatter_t-구조체)
		- [int posix\_spawnattr\_init(posix\_spawnatter\_t \*attr)](#int-posix_spawnattr_initposix_spawnatter_t-attr)
		- [int posix\_spawnattr\_destroy(posix\_spawnatter\_t \*attr)](#int-posix_spawnattr_destroyposix_spawnatter_t-attr)
		- [int posix\_spawnattr\_getflags(](#int-posix_spawnattr_getflags)
		- [int posix\_spawnattr\_setflags(](#int-posix_spawnattr_setflags)
	- [프로세스 그룹 속성 조작](#프로세스-그룹-속성-조작)
		- [int posix\_spawnattr\_getgroup(](#int-posix_spawnattr_getgroup)
		- [int posix\_spawnattr\_setgroup(](#int-posix_spawnattr_setgroup)
	- [시그널 속성 조작](#시그널-속성-조작)
		- [int posix\_spawnattr\_getsigdefault(](#int-posix_spawnattr_getsigdefault)
		- [int posix\_spawnattr\_setsigdefault(](#int-posix_spawnattr_setsigdefault)
		- [int posix\_spawnattr\_getsigmask(](#int-posix_spawnattr_getsigmask)
		- [int posix\_spawnattr\_setsigmask(](#int-posix_spawnattr_setsigmask)
	- [스케줄링 속성 조작](#스케줄링-속성-조작)
		- [int posix\_spawnattr\_getschedpolicy(](#int-posix_spawnattr_getschedpolicy)
		- [int posix\_spawnattr\_setschedpolicy(](#int-posix_spawnattr_setschedpolicy)
		- [int posix\_spawnattr\_getschedparam(](#int-posix_spawnattr_getschedparam)
		- [int posix\_spawnattr\_setschedparam(](#int-posix_spawnattr_setschedparam)

## 서론
프로세스를 복제하는 전통적인 방법 fork-exec 방식의 단점을 보완하고자 나온 POSIX 프로세스에 대한 내용을 다뤘다.  
멀티태스킹 구조에서 fork-exec 방식으로 구현할 경우 자원에 대한 복사가 이뤄져 오버헤드가 발생할 수 있다.  
fork에서는 힙 메모리, 정적 메모리, IPC 자원 ID, 열린 파일, 시그널 마스크 등 의 복사가 이뤄지며  
exec에서는 파일 디스크립터 정보의 복사가 발생하기 때문이다.  
이를 더 효율적이며, 세밀하게 제어할 수 있는 POSIX 프로세스에 관해 기술하였다.  

## fork
### pid_t fork(void);
**Return Value**
- `other`	: 자식프로세스의 pid (부모프로세스일 경우 반환)
- `0`	: 성공  (자식프로세스일 경우 반환)
- `-1`	: 실패

**Description**  
 프로세스 복제  
 프로세스를 복제하여 병렬 실행한다.  
 모든 정적 정보를 복제며 실행된다. (COW로 오버헤드 방지)  
 정적 정보 : 힙 메모리, 정적 메모리, IPC 자원 ID, 열린 파일 , 시그널 마스크 등  

## exec
### int execl (const char *path, const char *arg, ... );
### int execlp(const char *file, const char *arg, ... );
### int execle(const char *path, const char *arg, ..., char *const envp[]);
### int execv (const char *path, char *const argv[]);
### int execvp(const char *file, char *const argv[]);
### int execve(const char *path, char *const argv[], char *const envp[]);
**Parametters**
- `const char *path`    : 실행할 파일, 주소로 경로를 확인
- `const char *file`	: 실행할 파일, 환경변수에서 경로 탐색(파일 이름만 주면 환경변수에서 확인)
  - 접미사 `p : path enviroment`
- `const char *arg`	: 인자를 가변인자로 받음 (마지막은 NULL이 들어가야함)
  - 접미사 `l : list` 
- `chat const *argv[]`	: 인자를 배열로 받음 (마지막은 NULL이 들어가야함)
  - 접미사 `v : vector` 
- `char *const envp[]`	: 환경변수 목록
  - 접미사 `e : envitonmental path` 
  
**Return Value**
- 실행 성공 시 반환값 없음
- `-1`	: 실패

**Description**  

실행 코드 복제  
현재 실행중인 프로세스를 새로운 프로세스로 교체한다.  
입력한 인자를 바탕으로 프로그램을 실행 한다.  
기본적인 PID, PPID, 파일 디스크립터 등 프로세스의 정보는 유지된다.  


## posix_spawn
### int posix_spawn(
		pid_t *restrict			pid, 
		const char *restrict		path,
		const posix_spawn_file_actions_t	*file_actions,
		const posix_spawnattr_t *restrict 	attrp,
		char *const			argv[restrict],
		char *const			envp[restrict]);

### int posix_spawnp(
		pid_t *restrict			pid, 
		const char *restrict		file,
		const posix_spawn_file_actions_t	*file_actions,
		const posix_spawnattr_t *restrict	attrp,
		char *const			argv[restrict],
		char *const			envp[restrict]);
**Parametters**
- `pid_t *restrict pid` 		: 생성된 자식프로세스의 pid를 저장할 변수
- `const char *restrict path`		: 실행할 파일, 주소로 경로를 확인
- `const char *restrict file`		: 실행할 파일, 환경변수에서 경로 탐색
  - 접미사 `p : path enviroment`
- `const posix_spawn_file_actions_t *file_actions` : 열거나 닫을 파일을 제어하는 구조체
  - NULL : 파일 디스크립터를 전부 넘겨준다.
- `const posix_spawnattr_t attrp`	: 프로세스 그룹, 시그널 마스크, 스케줄링 정보 속성을 제어하는 구조체
- `char *const argv[restrict]`		: 인자를 배열로 받음 (마지막은 NULL이 들어가야함)
- `char *const envp[restrict]`		: 환경 변수 목록 (마지막은 NULL이 들어가야함)

**Return Value**
- `other`	: 에러 코드
- `127`	: 자식프로세스에서 셸 실행 불가
- `0`	: 셸 사용 불가
- `-1`	: 명령 실패 (fork 불가)

**Description**  
 fork-exec 구조의 오버헤드를 방지하고 향상된 기능을 제공하는 함수  
 저수준 파일 처리, 세션과 프로세스 그룹, 시그널 처리, 스케줄링에 대한 처리가 포함되어있다.  
 file_actions, attrp 인수를 이용하여 부모프로세스의 자원을 선택적으로 복제할 수 있다.  

## posix_spawn_file_action_t 구조체
**Description**  
 posix_spawn함수 호출 시 열거나 닫을 파일을 제어하는 구조체  
 init 함수를 통해 무조건 초기화 후 사용해야 한다.  
 사용을 완료한 경우 메모리를 해제해야 메모리 누수가 생기지 않는다.  
 EUID, 프로세스 한그룹, 기본 시그널 작동, 시그널 블록 마스크, 스케줄링 파라미터, 스케줄러  


**Common Parametters**
- `posix_spawn_file_actions_t *file_actions`
  - 속성을 변경할 객체의 주소
- `int fildes`	: file descriptor 번호
- `int oflag`	: 파일 열기 옵션 비트
- `mode_t mode`	: 파일 접근 권한 ex) 0644, 0777


**Common Return Value**
- `other`	: 에러번호
- `0`		: 성공


### int posix_spawn_file_actions_init(posix_spawn_file_actions_t	*file_actions)
posix_spawn_file_action_t 구조체를 초기화 한다.  
</br> 

### int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t	*file_actions)
posix_spawn_file_action_t 구조체를 삭제한다.  
</br>

### int posix_spawn_file_actions_addopen(
		posix_spawn_file_actions_t		*file_actions,
		int			fildes,
		const char *restrict	path,
		int			oflag,
		mode_t			mode)
자식 프로세스가 생성되면서 파일을 추가로 연다.  
</br>

### int posix_spawn_file_actions_addclose(
		posix_spawn_file_actions_t		*file_actions,
		int 			fildes)
자식 프로세스가 생성되면서 파일을 닫는다.  
</br>

### int posix_spawn_file_actions_adddup2(
		posix_spawn_file_actions_t		*file_actions,
		int 			fildes,
		int 			newfildes)
자식 프로세스가 생성되면서 파일기술자를 복제한다.  
</br>

## posix_spawnatter_t 구조체
**Description**  
 posix_spawn함수 호출 시 다양 속성을 제어하는 구조체  
 init 함수를 통해 무조건 초기화 후 사용해야 한다.  
 EUID, 프로세스 그룹, 기본 시그널 작동, 시그널 블록 마스크, 스케줄링 파라미터, 스케줄러  

**Flags**
| 플래그 | 설명 |
| :--- | :--- |
| POSIX_SPAWN_RESETIDS		| 자식 프로세스의 EUID를 부모 프로세스의 RUID로 설정한다. |
| POSIX_SPAWN_SETPGROUP		| 프로세스 그룹 관련 속성을 활성화한다. |
| POSIX_SPAWN_SETSIGDEF		| 기본 시그널 작동 속성을 활성화한다. |
| POSIX_SPAWN_SETSIGMASK	| 시그널 블록 마스크 속성을 활성화한다. |
| POSIX_SPAWN_SETSCHEDPARAM	| 스케줄링 파라미터 속성을 활성화한다. |
| POSIX_SPAWN_SETSCHEDULER	| 스케줄러 정책 속성을 활성화한다. |

**Common Parameters**
- `posix_spawnatter_t *attr`
  - 속성을 변경할 객체의 주소
- `short flags`	: 해당하는 플래그 조합

**Common Return Value**
- `other`	: 에러번호
- `0`		: 성공

### int posix_spawnattr_init(posix_spawnatter_t *attr)
posix_spawnatter_t 구조체를 초기화 한다.  
</br>

### int posix_spawnattr_destroy(posix_spawnatter_t *attr)
posix_spawnatter_t 구조체를 삭제한다.  
</br>

### int posix_spawnattr_getflags(
		const posix_spawnatter_t *restrict 	attr,
		short *restrict		flags)
posix_spawnatter_t 구조체에서 설정된 플래그 속성을 받는다.  
</br>

### int posix_spawnattr_setflags(
		posix_spawnatter_t 	*attr,
		short			flags)
posix_spawnatter_t 구조체에 플래그를 설정한다.  
</br>


## 프로세스 그룹 속성 조작
### int posix_spawnattr_getgroup(
		const posix_spawnattr_t *restrict 	attr,
		pid_t *restrict 	pgroup)
생성될 자식프로세스의 PGID를 받는다.  
</br>

### int posix_spawnattr_setgroup(
		const posix_spawnattr_t *restrict 	attr,
		pid_t		 	pgroup)
생성될 자식프로세스의 PGID를 변경한다.  
</br>


## 시그널 속성 조작
### int posix_spawnattr_getsigdefault(
		const posix_spawnattr_t *restrict 	attr,
		sigset_t *restrict 	sigdefault)
생성될 자식프로세스의 sigdefault 값을 얻는다.  
</br>

### int posix_spawnattr_setsigdefault(
		const posix_spawnattr_t *restrict 	attr,
		const sigset_t *restrict 		sigdefault)
생성될 자식프로세스의 sigdefault 값을 변경한다.  
</br>

### int posix_spawnattr_getsigmask(
		const posix_spawnattr_t *restrict 	attr,
		sigset_t *restrict 	sigmask)
생성될 자식프로세스의 시그널 마스크 값을 얻는다.  
</br>

### int posix_spawnattr_setsigmask(
		const posix_spawnattr_t *restrict 	attr,
		const sigset_t *restrict 		sigmask)
생성될 자식프로세스의 시그널 마스크 값을 변경한다.  
</br>

## 스케줄링 속성 조작
### int posix_spawnattr_getschedpolicy(
		const posix_spawnattr_t *restrict 	attr,
		int *restrict 		schedpolicy)
생성될 자식프로세스의 스케줄링 정책을 얻는다.  
</br>

### int posix_spawnattr_setschedpolicy(
		const posix_spawnattr_t *restrict 	attr,
		int 			schedpolicy)
생성될 자식프로세스의 스케줄링 정책을 변경한다.  
</br>

### int posix_spawnattr_getschedparam(
		const posix_spawnattr_t *restrict 	attr,
		struct sched_param *restrict 		schedparam)
생성될 자식프로세스의 스케줄 파라메터를 얻는다.  
</br>

### int posix_spawnattr_setschedparam(
		const posix_spawnattr_t *restrict 	attr,
		const struct sched_param *restrict 	schedparam)
생성될 자식프로세스의 스케줄 파라메터를 변경한다.  
</br>
