# 프로세스

## Index
- [서론](#서론)
- [fork](#fork)
- [exec](#exec)
- [posix](#posix)


## 서론
프로세스를 복제하는 전통적인 방법 fork-exec 방식의 단점을 보완하고자 나온 POSIX 프로세스에 대한 내용을 다뤘다.
</br> 멀티태스킹 구조에서 fork-exec 방식으로 구현할 경우 자원에 대한 복사가 이뤄져 오버헤드가 발생할 수 있다.
</br> fork에서는 힙 메모리, 정적 메모리, IPC 자원 ID, 열린 파일, 시그널 마스크 등 의 복사가 이뤄지며
</br> exec에서는 파일 디스크립터 정보의 복사가 발생하기 때문이다.
</br> 이를 더 효율적이며, 세밀하게 제어할 수 있는 POSIX 프로세스에 관해 기술하였다.
</br> popen, system
## fork
### pid_t fork(void);
**Return Value**
- `other`	: 자식프로세스의 pid (부모프로세스일 경우 반환)
- `0`	: 성공  (자식프로세스일 경우 반환)
- `-1`	: 실패

**Description**
</br> 프로세스 복제
</br> 프로세스를 복제하여 병렬 실행한다.
</br> 모든 정적 정보를 복제며 실행된다. (COW로 오버헤드 방지)
</br> 정적 정보 : 힙 메모리, 정적 메모리, IPC 자원 ID, 열린 파일 , 시그널 마스크 등

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
</br> 실행 코드 복제 
</br> 현재 실행중인 프로세스를 새로운 프로세스로 교체한다.
</br> 입력한 인자를 바탕으로 프로그램을 실행 한다.
</br> 기본적인 PID, PPID, 파일 디스크립터 등 프로세스의 정보는 유지된다.


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
</br> fork-exec 구조의 오버헤드를 방지하고 향상된 기능을 제공하는 함수
</br> 저수준 파일 처리, 세션과 프로세스 그룹, 시그널 처리, 스케줄링에 대한 처리가 포함되어있다.
</br> file_actions, attrp 인수를 이용하여 부모프로세스의 자원을 선택적으로 복제할 수 있다.


## posix_spawn_file_action_t 구조체
**Description**
</br> posix_spawn함수 호출 시 열거나 닫을 파일을 제어하는 구조체
</br> init 함수를 통해 무조건 초기화 후 사용해야 한다.
</br> 사용을 완료한 경우 메모리를 해제해야 메모리 누수가 생기지 않는다.
</br> EUID, 프로세스 한그룹, 기본 시그널 작동, 시그널 블록 마스크, 스케줄링 파라미터, 스케줄러

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
</br> posix_spawn_file_action_t 구조체를 초기화 한다.


### int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t	*file_actions)
</br> posix_spawn_file_action_t 구조체를 삭제한다.


### int posix_spawn_file_actions_addopen(
		posix_spawn_file_actions_t	*file_actions,
		int			fildes,
		const char *restrict	path,
		int			oflag,
		mode_t			mode)
</br> 자식 프로세스가 생성되면서 파일을 추가로 연다.


### int posix_spawn_file_actions_addclose(
		posix_spawn_file_actions_t	*file_actions,
		int 			fildes)
</br> 자식 프로세스가 생성되면서 파일을 닫는다.


### int posix_spawn_file_actions_adddup2(
		posix_spawn_file_actions_t	*file_actions,
		int 			fildes,
		int 			newfildes)
</br> 자식 프로세스가 생성되면서 파일기술자를 복제한다.


## posix_spawnatter_t 구조체
**Description**
</br> posix_spawn함수 호출 시 다양 속성을 제어하는 구조체
</br> init 함수를 통해 무조건 초기화 후 사용해야 한다.
</br> EUID, 프로세스 그룹, 기본 시그널 작동, 시그널 블록 마스크, 스케줄링 파라미터, 스케줄러

**Flags**
| 플래그 | 설명 |
| :---: | :--- |
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

### int posix_spawntter_init(posix_spawnatter_t *attr)
</br> posix_spawnatter_t 구조체를 초기화 한다.

### int posix_spawntter_destroy(posix_spawnatter_t *attr)
</br> posix_spawnatter_t 구조체를 삭제한다.

### int posix_spawntter_getflags(
		const posix_spawnatter_t *restrict 	attr,
		short *restrict		flags)
</br> posix_spawnatter_t 구조체에서 설정된 플래그 속성을 받는다.

### int posix_spawntter_setflags(
		posix_spawnatter_t 	*attr,
		short			flags)
</br> posix_spawnatter_t 구조체에 플래그를 설정한다.


## 프로세스 그룹 속성 조작

### int posix_spawntter_getgroup(
		const posix_spawntter_t *restrict 	attr,
		pid_t *restrict 	pgroup)
</br>

### int posix_spawntter_setgroup(
		const posix_spawntter_t *restrict 	attr,
		pid_t		 	pgroup)
</br>


## 시그널 속성 조작
### int posix_spawntter_getsigdefault(
		const posix_spawntter_t *restrict 	attr,
		sigset_t *restrict 	sigdefault)
</br>

### int posix_spawntter_setsigdefault(
		const posix_spawntter_t *restrict 	attr,
		const sigset_t *restrict 		sigdefault)
</br>

### int posix_spawntter_getsigmask(
		const posix_spawntter_t *restrict 	attr,
		sigset_t *restrict 	sigmask)
</br>

### int posix_spawntter_setsigmask(
		const posix_spawntter_t *restrict 	attr,
		const sigset_t *restrict 		sigmask)
</br>

## 스케줄링 속성 조작
### int posix_spawntter_getschedpolicy(
		const posix_spawntter_t *restrict 	attr,
		int *restrict 		schedpolicy)
</br>

### int posix_spawntter_setschedpolicy(
		const posix_spawntter_t *restrict 	attr,
		int 			schedpolicy)
</br>

### int posix_spawntter_getschedparam(
		const posix_spawntter_t *restrict 	attr,
		struct sched_param *restrict 		schedparam)
</br>

### int posix_spawntter_setschedparam(
		const posix_spawntter_t *restrict 	attr,
		const struct sched_param *restrict 	schedparam)
</br>


## posix_spawnattr_t 함수
### 
**Parametters**
- 

**Return Value**
- 

**Description**
</br>


## POSIX 매크로
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>
