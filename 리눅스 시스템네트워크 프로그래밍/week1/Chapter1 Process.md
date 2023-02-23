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
- `const char *file`	: 실행할 파일, 환경변수에서 경로 검색(파일 이름만 주면 환경변수에서 확인)
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
</br> 컴파일 시 FD_CLOEXEC 플래그를 통해 파일 디스크립터를 닫으며 실행할 수 있다.


## posix_spawn
### int posix_spawn(
	```
		pid_t *restrict			pid, 
		const char *restrict		path,
		const posix_spawn_file_actions_t	*file_actions,
		const posix_spawnattr_t		*restrict attrp,
		char *const			argv[restrict],
		char *const			envp[restrict]);
	```
### int posix_spawnp(
	```
		pid_t *restrict			pid, 
		const char *restrict		file,
		const posix_spawn_file_actions_t	*file_actions,
		const posix_spawnattr_t		*restrict attrp,
		char *const			argv[restrict],
		char *const			envp[restrict]);
	```
**Parametters**
- 

**Return Value**
- 

**Description**
</br> fork-exec 구조의 오버헤드 방지 및 향상된 기능을 제공하는 함수
</br> 저수준 파일 처리, 세션과 프로세스 그룹, 시그널 처리, 스케줄링에 대한 처리가 포함되어있다.

## posix_spawn_file_action_t 구조체
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>

## posix_spawnatter_t 구조체
### declring_function
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
