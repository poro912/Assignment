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
	- [setpgid](#setpgid)
	- [setpgrp](#setpgrp)
	- [setsid](#setsid)
	- [get 함수](#get-함수)
- [데몬 프로세스](#데몬-프로세스)
	- [데몬 프로세스의 생성 방법](#데몬-프로세스의-생성-방법)
	- [daemon](#daemon)
- [시그널 블록 마스크](#시그널-블록-마스크)
	- [pause](#pause)
	- [sigprocmask](#sigprocmask)
	- [sigpending](#sigpending)
	- [sigsuspend](#sigsuspend)
- [시그널 안전, 원자적 실행](#시그널-안전-원자적-실행)
	- [시그널에서 전역 변수 사용의 2가지 조건](#시그널에서-전역-변수-사용의-2가지-조건)
- [멀티 스레드 환경의 시그널처리](#멀티-스레드-환경의-시그널처리)
	- [pthread\_sigmask](#pthread_sigmask)
	- [pthread\_kill](#pthread_kill)
	- [sigwait](#sigwait)
- [대체 시그널 스택 (SA\_ONSTACK)](#대체-시그널-스택-sa_onstack)
	- [sigaltstack](#sigaltstack)



## 서론
시그널처리는 외부 신호를 받아들이는 인터페이스 조작을 의미한다.  
예외 처리, 외부 개입, 이벤트 통지용으로 사용된다.  
표준 시그널이라 불리는 UNIX 시그널은 예외 상황 개입을 위해 사용된다.  


예외 처리 : 프로세스에 치명적인 오류나 자원의 제한 수치 초과  
외부 개입 : 사용자가 프로세스에 개입, 취소나 중단을 위해 시그널을 발생시키는 것  
이벤트 통지 : 몇몇 I/O의 송수신이 완료되었을 때 시그널을 발생시키는 것  

시그널 발생 시 실행중인 코드는 시그널에 선점되어 시그널을 우선적으로 실행한다. 
시그널처리 : 특정 코드를 수행하는 방법, 시그널을 무시하는 방법이 있다.  

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
	int	setpgrp(void);
자신을 프로세스 그룹 리더로 만든다  

### setsid
	pid_t	setsid(void);
새로운 세션을 연다  
자신을 프로세스 그룹 리더로 만든다.  


### get 함수
	pid_t	getpgid(pid_t pid);		// 포로세스 그룹 id 획득
	pid_t	getpgrp(void);			// 프로세스 그룹 id  획득  
	pid_t	getsid(pid_t pid);		// 프로세스 세션 id 획득


## 데몬 프로세스
터미널 세션과 연결되어 있지 않은 백그라운드 프로세스  
데몬 프로세스는 독립된 세션을 따로 만들기 때문에 로그인 셸을 종료해도 살아남는다.  

### 데몬 프로세스의 생성 방법
1. setsid()	세션을 새로 연다
2. 고아 프로세스로 만든다. 제어터미널은 가지지 않는다.
3. 작업 디렉터리를 "/"로 변경
   - 다른 장치의 디렉터리 점유를 막는다.
4. stdin.stdout,strerr을 닫는다.
   - 입출력으로 인한 프로세스 STOP 상태를 방지한다.

### daemon
	int daemon(
		int 		nochdir, 
		int 		noclose)
**Parametters**
- `int nochdir` : chdir 과정을 할 것인가
  - `0` : "/"로 작업디렉터리를 변경한다.
  - `other` : 변경하지 않는다.
- `int noclose` : 
  - `0` : stdin, stdout, stderr을 /dev/null로 리다이렉트 한다.
  - `other` : 리다이렉트 하지 않는다.

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
현재 프로세스를 데몬 프로세스로 만든다.


## 시그널 블록 마스크
시그널로부터 보호되어야 하는 코드구간에 적용한다.  
특정 코드 구간에서 도착된 시그널을 지연시켜 비동기적 요소가 개입하지 못하도록 막는다.  

블록마스크에서 막은 시그널은 프로세스에 전달하지 않고 마스크에 머무른다.

지연된(pending) 처리 : 막힌 시그널을 프로그래머가 원하는 특정 시점에 처리하는 것  
지연된 처리 시 동일한 시그널은 하나만 보존한다.  
시그널 핸들러는 최대 2회 실행된다.(현재 실행중, 추가로 도착한 시그널)  
블록되었던 시그널의 처리순서는 알 수 없다.  

### pause
	int pause(void)

**Return Value**  
- `-1`	: errno 설정 

**Description**  
아무 시그널이 도착할 때 까지 기다림  
errno가 EINTR이라면 정상 실행  


### sigprocmask
	int sigprocmask(
		int 		how,
		const sigset_t *restrict 	set,
		sigset_t *restrict 		oset
	)
**Parametters**
- `int how`	: 시그널 블록 마스크의 행위를 지정  
  |||
  | :-- | :-- |  
  | SIG_BLOCK | 현재 시그널 블록 마스크에 추가한다.|  
  | SIG_UNBLOCK| 현재 시그널 블록 마스크에서 제거한다.|
  |SIG_SETMASK| 블록 마스크를 교체한다.|

- `sigset_t *set`
  - 지정할 시그널 블록 마스크
- `sigset_t *oset`
  - 변경하기 전 옛날 시그널 마스크 값을 반환
  - `NULL` 지정 시 무시됨

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
프로세스의 전역 시그널 마스크를 조정한다.  
블록마스크로 지정된 시그널은 지연되어 처리되지 않는다.  
블록마스크가 변경되거나 해제되어 시그널이 블록되지 않으면 즉시 프로세스에 전달된다.  

스레드 안전 함수가 아니다.  
pthread_sigmask 함수를 사용하여 스레드 안전 상태로 처리한다.

### sigpending
	int sigpending (sigset_t *set);
**Parametters**
- `sigset_t *set` : 블록된 시그널

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
지연된 시그널을 읽어온다.  
시그널 마스크로인해 블록된 시그널을 반환한다.

시그널을 확인할 뿐 변경을 하지 않는다.  
수동으로 작업을 처리하려면 해당 시그널 핸들러를 SIG_IGN로 변경해야한다.  


### sigsuspend
	int sigsuspend(const sigset_t *mask)
**Parametters**
- `sigset_t *mask` : 

**Return Value**
- `-1`	: errno 설정 

**Description**  
전역마스크를 mask 값으로 임시 교체한다.  
mask되지 않은 시그널이 도착할 때까지 현재 프로세스의 실행을 중지한다.  
pause 함수와 달리 특정 시그널에 대해서만 블록킹 상태로 둘 수 있다.  
실행 종료 시 원래 시그널 마스크로 복원된다.  
errno가 EINTR이라면 정상 실행  



## 시그널 안전, 원자적 실행
시그널도 원자적 실행에 영향을 받는다.  
시그널 핸들러가 전역변수를 다루면 에일리어싱 되거나 캐쉬된 경우, 최적화 과정에 영향을 받는다.  

### 시그널에서 전역 변수 사용의 2가지 조건
	volatie sig_atomic_t gi_counter;
1. `volatile`로 선언해야한다.
   * SUSv2부터는 선언이 포함되어있는 경우도 있다.
2. `sig_atomic_t`로 선언해야한다.



## 멀티 스레드 환경의 시그널처리
시그널은 프로세스 단위로 전달된다.  
멀티 스레드 환경에서 어떤 스레드에 전달된 것인지 혼란을 야기할 수 있다.  

1. sigcation과 시그널 마스크를 설치하는 방법
   * 스레드별로 시그널 블록마스크를 조절하는 방법
   * 프로세스 종료, 디버깅 목적에서 사용된다.
   * 스레드에 외부적인 조작을 가할때는 IPC와 조건 변수를 통한 개입이 효과적이다.
2. 대부분의 시그널을 블록, 지연된 시그널 처리를 전담하는 방법
   * sigwait을 이용해 지연된 시그널을 확인하고 처리하는 방법
   * 동기적으로 처리하여 뮤텍스, 조건변수와 결합 사용해도 안전히 수행할 수 있다.


비동기적인 시그널 작업을 목적으로 하는것이 아니다.  
최소한의 종료 시그널을 제외한 다른 시그널의 개입을 막아 작동이 방해받지 않아야 한다.  
시그널의 비동기 특성을 활용하는 작업은 대부분 스레드로 처리 가능하다.  


### pthread_sigmask
 	int pthread_sigmask(
		int		how, 
		const sigset_t *restrict	netmask, 
		sigset_t *restrict		oldmask);
	
**Parametters**
- `int how` : 시그널 블록 마스크의 행위를 지정  
  |||
  | :-- | :-- |  
  | SIG_BLOCK | 현재 시그널 블록 마스크에 추가한다.|  
  | SIG_UNBLOCK| 현재 시그널 블록 마스크에서 제거한다.|
  |SIG_SETMASK| 블록 마스크를 교체한다.|
- `sigset_t *netmask`
  - 지정할 시그널 블록 마스크 
- `sigset_t *oldmask`
  - 변경하기 전 옛날 시그널 마스크 값을 반환
  - `NULL` 지정 시 무시됨

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
단일 스레드 환경에서는 [sigprocmask](#sigprocmask)와 동일한 역할을 수행한다.  
스레드는 생성될 때 호출한 스레드의 시그널 마스크를 상속받는다.  
main 함수에서 pthread_sigmask를 호출하여 사용할 시그널을 미리 블록하도록 한다.  
`sigcation`을 통한 시그널은 모든 쓰레드에서 공유된다.  
`pthread_sigmask`는 현재 스레드에만 적용된다.  

### pthread_kill
 	int pthread_kill(
		pthread_t	thread, 
		int		signo);
**Parametters**
- `pthread_t thread` : 스레드를 전송할 대상 TID
- `int signo` : 전송할 시그널

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
특정 스레드에 시그널을 전달한다.  
TID는 프로세스 내에서만 의미가 있다.  
다른 프로세스의 스레드에는 영향을 끼칠 수 없다.  

### sigwait
 	int sigwait(
		const sigset_t *restrict	set,
		int *restrict	sig);
**Parametters**
- `sigset_t * set` : 기다릴 시그널 집합
- `int * sig` : 발생한 시그널 번호

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  
수신된 시그널 번호를 리턴하며 블록된 시그널을 제거한다.  
[sigpending](#sigpending)과 비슷한 역할을 한다.



## 대체 시그널 스택 (SA_ONSTACK)
SA_ONSTACK 플래그를 사용할 때 사용된다.  
핸들러가 실행할 때 별도의 대체 스택 공간을 사용하도록 한다.  
대체 공간은 `sigaltstack`로 미리 지정해줘야한다.  

스택 오버플로우로 인해 SIGSEGV 시그널 발생 시 SIGSEGV 핸들러가 실행될 공간도 부족하여 동작을 기대할 수 없다.  
이는 디버깅에 필요한 어떤 추가 정보도 남길 수 없게 된다.  
힙 영역에서 디버깅에 필요한 정보를 남기기위한 기법이다.  
가 사용할 수 없다.</br>이전 시그널 스택을 백업하는 경우에만 시스템



### sigaltstack
 	int sigaltstack(
		const stack_t *restrict		ss,
		stack_t *restrict		oss
	)

	typedef struct sigltstack{
		void *		ss_sp;
		size_t		ss_size;
		int		ss_flags;
	} stack_t;
**Parametters**
- `stack_t *ss` : 새로 할당할 시그널 스택 정보
- `stack_t *oss` :기존 시그널 스택 정보 

**Return Value**
- `0` : 성공
- `-1`	: 에러, errno 설정 

**Description**  

스택 크기의 최소값 `MINSIGSTKDZ`이상을 충족해야한다.  
스택의 권장 크기 값 `SIGSTKSZ` 일반적으로 8~32KB 사이의 값이다.  
- ss_flag
  - 사용자는 SS_DISABLE 플래그만 지정 가능하다.
  - |||
    | :-- | :-- |  
    | SS_ONSTACK | 대체 시그널 스택이 사용중이다.</br>사용중인 시그널 스택은 병경할 수 없다.</br>사용자이 보고하는 용도로 설정된다. |  
    | SS_DISABLE | 대체 시그널 스택을 비활성화 시킨다.</br>플래그 설정 성공 시 시그널 스택은 유저 스택을 사용하는 방식으로 되돌아간다. |

 