<h1> 텍스트 처리</h1>
<h2> Index </h2>
 
- [서론](#서론)
- [정규 표현식](#정규-표현식)
	- [함수 목록](#함수-목록)
	- [regcomp](#regcomp)
	- [regmatch\_t](#regmatch_t)
	- [regexec](#regexec)
	- [regerror](#regerror)
- [**Parametters**](#parametters)
	- [regfree](#regfree)
- [**Parametters**](#parametters-1)
- [문자열 관련 함수](#문자열-관련-함수)



## 서론
텍스트 처리함수는 입력 함수와 출력 형태를 결정하는 함수들로 이루어져 있다.  
문자열을 한 바이트씩 읽어 처리하는 방법은 빠르지만, 형태가 변할 경우 재작성 하는 등의 문제가 발생할 수 있다.  
이를 해결하기 위해 문자열 패턴을 분석하여 처리하는 정규 표현식을 소개하였다.  

## 정규 표현식
특정한 규칙을 가진 문자열의 집합을 표현하는 데 사용하는 형식 언어  
텍스트 편집기와 프로그래밍 언어에서 문자열의 검색과 치환을 위해 지원하고있다.  
텍스트 처리, 데이터 검증, 데이터 스크래핑, 데이터 랭글링, 파싱, 문법강조 시스템 등의 작업에 사용된다.  
정규 표현식의 복잡도와 설계에 따라 컴퓨터 자원을 과도하게 소비할 수 있다.  
BSD 방식, POSIX 방식, PCRE 방식 등이 있다.  
[정규 표현식-위키백과](https://ko.wikipedia.org/wiki/%EC%A0%95%EA%B7%9C_%ED%91%9C%ED%98%84%EC%8B%9D)


### 함수 목록
|함수	|기능|
|:--:	|:--|
|regcomp	| 정규 표현식 패턴 문자열 패턴 버퍼로 컴파일 |
|regexec	| 컴파일된 패턴 버퍼를 검색할 문자열에 적용 |
|regfree	| 패턴 버퍼에 할당된 메모리를 해제 |
|regerror	| 에러 보고 |

### regcomp  
	int regcomp(
		regex_t *restrict		preg
		const char * restrict		pattern
		int				cflags
	)
**Parametters** 
- `regex_t *restrict preg`	: 패턴 버퍼 저장 공간
- `const char * restrict pattern`	: 정규식 표현 문자열
- `int cflags`	: 옵션 ('|'로 연결 가능)
	| flag		| 의미 |
	| :--:		| :-- |
	| REG_EXTENDED	| POSIX 확장 정규 표현식 문법을 적용 |
	| REG_ICASE	| 대소문자 구분을 무시 |
	| REG_NOSUB	| 서브 스트링 기능을 무시</br>괄호를 떼내는 작업을 하지 않는다. |
	| REG_NEWLINE	| 줄 바꾸기 문자를 특수 줄의 끝 문자로 처리한다.</br>(생략시 줄바꿈 문자는 다른 문자처럼 처리) |  
 
**Return Value**  
- `other`	: 에러코드
- `0`	: 성공

**Description**  
패턴 버퍼로 컴파일 한다.  
컴파일 : 데이터를 수집, 처리하여 다른 형태로 만들어내는 과정  
정규 표현식 사용을 위해 기계가 읽기 위한 형태로 데이터를 수집하고 공간을 확보한다.

### regmatch_t
	typedef struct {
		regoff_t rm_so;
		regoff_t rm_eo;
	} regmatch_t;

**Member**  
- `regoff_t rm_so`	: 매칭에 성공한 시작 오프셋
- `regoff_t rm_eo`	: 매칭에 성공한 끝 오프셋

**Description**  
매칭된 문자열의 오프셋 위치를 저장하기위한 구조체  
두개의 정수형으로 이루어져있다.  

### regexec  
	int regexec(
		const regx_t *restrict		preg,
		const char *restrict		string,
		size_t				nmatch,
		regmatch_t			pmatch[restrict],
		int				eflags
	)
**Parametters** 
- `const regx_t *restrict	preg`	: 
- `const char *restrict	string`	: 
- `size_t	nmatch`	: 
- `regmatch_t 	pmatch[restrict]`	: 
- `int	eflags`	: 


**Return Value**  
-   ㅁ

**Description**  

### regerror  
	int regerror(
		int errcode,
		const regex_t *restrict preg,
		char *restrict errbuf,
		size_t errbuf_size
	)
**Parametters** 
- 
**Return Value**  
-   ㅁ

**Description**  

### regfree  
	int regerror(
		int errcode,
		const regex_t *restrict preg,
		char *restrict errbuf,
		size_t errbuf_size
	)
**Parametters** 
- 
**Return Value**  
-   ㅁ

**Description**  

## 문자열 관련 함수 
|함수	|기능|
|:--:	|:--|
|stpcpy		| 문자열 복사 후 마지막 위치의 주소값을 넘겨준다. |
|stpncpy	| 최대값을 지정할 수 있는 stpcpy 함수 |
|strnlen	| 최대값을 지정할 수 있는 strlen 함수 |
|strndup	| 최대값을 지정할 수 있는 strdup 함수 |
|strerror_r	| strerror의 재진입 가능한 쓰레드 안전 버전 |
|getline	| 행 단위 파일 읽기 |
|getdelim	| 구분자 단위 파일 읽기 |



