<h1> Signal</h1>

<h2> Index </h2>

- [서론](#서론)
- [시그널 행동 및 속성](#시그널-행동-및-속성)
- [표준 시그널 표](#표준-시그널-표)
- [시그널 마스크](#시그널-마스크)
	- [sigset\_t](#sigset_t)
	- [시그널 마스크 조작 함수](#시그널-마스크-조작-함수)
- [시그널 핸들러](#시그널-핸들러)
	- [sigaction](#sigaction)
	- [struct sigcation](#struct-sigcation)
	- [wait, waitpid](#wait-waitpid)
	- [wiat status 매크로](#wiat-status-매크로)
	- [waitid](#waitid)
- [프로세스 그룹과 세션](#프로세스-그룹과-세션)
	- [setpgid, getpgid](#setpgid-getpgid)
	- [setpgrp, getpgrp](#setpgrp-getpgrp)
	- [setsid, getsid](#setsid-getsid)
- [sub-title](#sub-title)
	- [function\_name](#function_name)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)



## 서론
시그널 처리는 외부 신호를 받아들이는 인터페이스 조작을 의미한다.  
일반적으로 예외 처리, 외부 개입, 이벤트 통지용으로 사용된다.  
그 중 표준 시그널이라 불리는 UNIX 시그널은 예외 상황, 새입을 위해 사용된다.  


예외 처리 : 프로세스에 치명적인 오류나 자원의 제한 수치 초과  
외부 개입 : 사용자가 프로세스에 개입하여 취소나 중단을 위해 시그널을 발생시키는 것  
이벤트 통지 : 몇몇 I/O의 송수신이 완료되었을 때 시그널을 발생시키는 것  

시그널이 발생하면 실행중인 코드는 시그널에 선점되어 시그널 우선적으로 실행된다.  
시그널처리는 특정 코드를 수행하는 방법과, 시그널을 무시하는 방법이 있다.  

시그널 처리는 다양한 범주를 포함한다.  
- 시그널 핸들러를 작성하는 것
- 시그널을 고의적으로 지연시키는 행위
- 시그널을 다른 프로세스에 전파하거나 송신하는 행위  
- 시그널에 의해 발생할 수 있는 다른 함수들의 에러처리  
  

## 시그널 행동 및 속성
| Term	| 프로세스를 종료시킨다	|
|:--|:--|
| Ign	| 아무런 작동을 하지 않는다.	|
| Core	| 프로세스를 종료시키며 코어를 덤프한다.	|
| Stop	| 포로세스를 정지시킨다.	|
| NoCatch	| 시그널 핸들러를 설치할 수 없다.	|
| NoIgn	| 시그널을 무시할 수 없다. (블록킹 불가)	|

## 표준 시그널 표
시그널은 시스템마다 다를 수 있다.  

![](./img/signal.jpg)  
![](./img/siganl2.jpg)  


## 시그널 마스크


### sigset_t
시그널 세트  
시그널 번호 개수만큼 공간이 할당되는 구조체이다.  

### 시그널 마스크 조작 함수
시그널 마스크를 조작하기위한 함수  
|||
|:--|:--|
|`int sigemptyset (sigset_t *set)`		|시그널 세트를 모두 1로 비운다.|
|`int sigfillset (sigset_t *set)`		| 시그널 세트를 모두 0으로 채운다.|
|`int sigaddset (sigset_t *set, int signum)`	| 특정 시그널 비트를 1로 변경한다.|
|`int sigdelset (sigset_t *set, int signum)`	| 특정 시그널 비트를 0으로 번경한다.|
|`int sigismember (sigset_t *set, int signo)`	| 시그널 번호에 해당하는 비트가 켜져있는지 확인한다.|


## 시그널 핸들러
전달된 시그널을 받을 때 실행되는 기능을 의미한다.  


### sigaction
	int stgactoin(
		int			sig,
		const struct sigcation 	*restrict act
	)

**Parametters**
- `int sig`	 : 대상 시그널 번호
- `sigcation *act`	: 작동과 속성이 저장된 정보 
  
**Return Value**  
- `0`	: 성공
- `-1`	: 에러, errno 설정 

**Description**  
SIGKILL, SIGSTOP은 대상으로 지정할 수 없다.


### struct sigcation
시그널 핸들러에대한 정보를 저장하는 구조체
```cpp
struct sigactoin{
	void 		(*sa_handler)(int);
	void 		(*sa_handler)(int, siginfo_t *, void *);
	sigset_t	sa_mask;
	int		sa_flags;
}
```
**Members**  
- `(*sa_handler)()`
  - 시그널 핸들러가 호출할 함수나 기본 행동(매크로)
  - 매크로는 SIG_DFL, SIG_IGN중 하나를 사용할 수 있다.
    - SIG_DFL	: 해당 시그널에 정해진 기본행동을 한다.
    - SIG_IGN	: 해당 시그널을 무시하여 아무것도 하지 않는다.
- `(*sa_handler)()` : 확장된 시그널 핸들러 사용시 호출할 함수
- `sa_mask` : 시그널 블록킹 마스크가 저장되는 시그널 세트
- `sa_flags` : 시그널 핸들러의 옵션 플래그
  |||
  |:--|:--|
  |SA_RESTART	| 시스템 콜 함수가 시그널 핸들러로 인해 중단된 경우 EINTR 없이 자동 재시작된다.</br>타임아웃이 존재하는 함수는 EINTR을 발생시킨다. |
  |SA_NOCLDSTOP	| 자식 프로세스의 정지에 대해 핸들러를 작동하지 않는다. |
  |SA_RESETHAND	| 시그널 핸들러를 일회용으로 설정한다. (SA_ONESHOT) |
  |SA_NODEFER	| 같은 시그널의 중복 수신을 허용한다. (SA_NOMASK) |
  |SA_SIGNFO	| 시그널 핸들러가 추가적인 시그널 정보를 저장하도록 지시한다. |
  |SA_ONSTACK	| 시그널 핸들러 실행 시 sigaltstack로 대체된 스택 공간을 사용하도록 한다. |


### wait, waitpid
	pid_t wait(int *status)
	pid_t waitpid(
		pid_t 		pid,
		int 		*status,
		int 		options)
**Parametters**
- `pid_t pid`	: 종료를 기다릴 프로세스의 pid 또는 프로세스 그룹 
  |||
  |:--|:--|
  | -1 		| 아무 자식 프로세스의 종료를 기다린다.</br>WAIT_ANY와 동일|
  | 양수	| 종료를 기다릴 자식 프로세스의 pid|
  | 0		| 현재 프로세스와 동일한 그룹의 ID내에 속한 자식프로세스				</br>WAIT_MYPGRP와 동일|
  |-1미만 음수	| 프로세스 그룹 ID가 지정된 음수의 절대값과 같은 pid의 자식 프로세스|
- `int *status`
  - 자식 프로세스의 상태값을 반환(NULL을 통해 무시가능)
  - [wait 매크로](#wiat-status-매크로)를 통해 값의 검사를 진행한다.
- `int *options`
  |||
  |:--|:--|
  | WNOHANG		| 준비된 자식 프로세스가 없다면 기다리지 않고 즉시 반환한다.|
  | WUNTRACED		| 종료되거나 정지된 자식 프로세스에 대해서 모두 보고한다.|
  

**Return Value**
- `-1` : 에러 
- `other` : 프로세스 ID

**Description**  
자식 프로세스의 결과값을 반환받기위해 사용한다.  
자식프로세스가 없는경우 ECHILD 에러코드가 설정된다.  

[좀비프로세스](../etc.md/#좀비-프로세스)를 방지하기위해 사용한다.  
루프구조를 만들어야 모든 좀비프로세스를 없앨 수 있다.  



### wiat status 매크로
<table><tr></tr>
<tr>
	<td rowspan=2>정상종료인 경우</td>
	<td>WIFEXITED(status)</td>
	<td>자식 프로세스가 종료한 경우 non-zero 반환</td>
</tr>
<tr>
	<td>WEXITSTATUS(status)</td>
	<td>WIFEXITED의 반환값이 non-zero인 경우에만사용</br>
	자식 프로세스의 종료값을 반환
	</td>
</tr>
<tr>
	<td rowspan=2>시그널로 종료된 경우</td>
	<td>WIFSIGNALED(status)</td>
	<td>자식 프로세스가 시그널로 종료된 경우 non-zero 반환</td>
</tr>
<tr>
	<td>WTRMSIG(status)</td>
	<td>WTRMSIG의 반환값이 non-zero인 경우에만사용</br>
	자식 프로세스가 수신한 종료 시그널 번호를 반환한다.</td>
</tr>
<tr>
	<td rowspan=2>정지된 경우</td>
	<td>WIFSTOPPED(status)</td>
	<td>자식 프로세스가 정지된 경우 non-zero 반환</br>
	waitpid에 WUNTRACED 옵션을 사용한 경우</td>
</tr>
<tr>
	<td>WSTOPSIG(status)</td>
	<td>WSTOPSIG의 반환값이 non-zero인 경우에만사용</br>
	자식 프로세스가 수신한 정지 시그널 번호를 반환한다.</td>
</tr>
</table>


### waitid
	int waitid(
		idtype_t	idtype,
		id_t		id,
		siginfo_t	*infop,
		int		option
	)
**Parametters**
- `idtype_t	idtype`	: 두번째 인수와 연계되어 작동한다.
  |||
  |:--|:--|
  | P_PID 		| id에 해당하는 자식 프로세스의 종료를 기다린다.|
  | P_PGID		| id에 지정한 프로세스 그룹에 속한 자식 프로세스의 종료를 기다린다.|
  | P_ALL		| 자식 프로세스 중 아무나 종료를 기다린다. id는 무시된다.|
- `id_t		id`	: 종료를 기다릴 프로세스 id
- `siginfo_t	*infop`	: 결과 구조체
  |||
  |:--|:--|
  | si_pid	| 자식 프로세스의 PID|
  | si_uid	| 자식 프로세스의 RUID |
  | si_signo	| 항상 SIGCHLD로 리턴한다.|
  | si_code	| 다음 코드 중 하나로 설정된다.</br>CLD_EXITED(정상 종료)</br>CLD_KILLED(시그널에 의한 종료)</br>CLD_DUMPED(시그널 종료, 코어 덤프됨)</br>CLD_TRAPPED(트랩됨)</br>CLD_STOPPED(정지됨)</br>CLD_CONTINUED(재개됨)|
  | si_stauts	| si_cdoe에 대한 부가 정보 상태값|
- `int		option`	: 옵션 (조합 가능)
  |||
  |:--|:--|
  | WNOHANG	| 준비된 자식 프로세스가 없다면 기다리지 않고 즉시 반환됨(넌블럭킹)|
  | WNOWAIT	| 자식 프로세스의 종료 상태를 읽고 남겨두어 다시 읽을 수 있게 한다.|
  | WSTOPPED	| 정지된 자식 프로세스에 대해서 보고한다.|
  | WCONTINUED	| 재개된 자식 프로세스에 대해서 보고한다.|
  | WEXITED	| 정상 종료된 자식 프로세스에 대해 보고한다.|

**Return Value**
- `0` : 성공 
- `-1` : 에러, errno 설정

**Description**
wait, waitpid 함수에서 확장된 시그널 처리를 지원하는 기능이 더해진 함수이다.  
자식프로세스가 없는경우 ECHILD 에러코드가 설정된다.  


## 프로세스 그룹과 세션
- [프로세스 그룹](../etc.md#프로세스-그룹-pgidprocess-group-id)
- [세션](../etc.md#세션-sidsession-id)


### setpgid
	int	setpgid(
		pid_t		pid,
		pid_t 		pgid);

**Parametters**
- `pid_t pid`	: 변경할 프로세스 ID
- `pid_t pgid`	: 소속하게될 프로세스 그룹

**Return Value**
- `0`	: 성공
- `-1`	: 에러, errno 설정

**Description**
프로세스 그룹을 변경한다.  
인수가 0일경우 현재 프로세스의 pid를 의미한다.  
setpid(0,0)을 사용하면 자신이 리더가 되며 새로운 프로세스 그룹을 만들게 된다.  
setpid(0,0)은 setpgrp()와 동일한 역할을 한다.


### setpgrp
	int	setpgrp(void);			// 

### setsid
	pid_t	setsid(void);


## get
	pid_t	getpgid(pid_t pid);			// 프로세스 그룹 획득
	pid_t	getpgrp(void);				//
	pid_t	getsid(pid_t pid);



## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**

 