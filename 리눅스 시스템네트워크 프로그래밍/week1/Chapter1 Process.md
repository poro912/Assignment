# 프로세스

## 서론

## fork
### pid_t fork(void);
**Return Value**
- `other`	: 자식프로세스의 pid (부모프로세스일 경우 반환)
- `0`	: 성공  (자식프로세스일 경우 반환)
- `-1`	: 에러

**Description**
</br> 프로세스 복제
</br> 프로세스를 복제하여 병렬 실행한다.
</br> 이 때 모든 정적 정보를 복제하여 실행된다. (COW로 오버헤드 방지)
</br> 정적 정보 : 힙 메모리, 정적 메모리, IPC 자원 ID, 열린 파일 , 시그널 마스크 등

## exec
### int execl(const char *path, const char *arg, ... );
### int execlp(const char *file, const char *arg, ... );
### int execle(const char *path, const char *arg, ..., char *const envp[]);
### int execv(const char *path, char *const argv[]);
### int execvp(const char *file, char *const argv[]);
### int execve(const char *path, char *const argv[], char *const envp[]);
**Parametters**
- `const char *path`    : ``` ```
- `const char *file`	: ```
- `const char *arg`		: ```l```
- `chat const *argv[]`	: `v`
- `char *const envp[]`	: 

**Return Value**
- 

**Description**
</br> 실행 코드 복제 
</br> 현재 실행중인 프로세스를 새로운 프로세스로 교체한다.
</br> 기본적인 PID, PPID, 파일기술자 등 프로세스의 정보는 유지된다.


## POSIX
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>


## function_name
### declring_function
**Parametters**
- 

**Return Value**
- 

**Description**
</br>
