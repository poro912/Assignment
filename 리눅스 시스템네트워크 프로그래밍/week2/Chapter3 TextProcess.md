<h1> 텍스트 처리</h1>

<h2> Index </h2>

- [서론](#서론)
	- [regcomp](#regcomp)
	- [regexec](#regexec)
	- [regerror](#regerror)
	- [regfree](#regfree)
- [문자열 관련 함수](#문자열-관련-함수)



## 서론


|함수	|기능|
|:--:	|:--|
|regcomp	| 정규 표현식 패턴 문자열 패턴 버퍼로 컴파일 |
|regexec	| 컴파일된 패턴 버퍼를 검색할 문자열에 적용 |
|regfree	| 패턴 버퍼에 할당된 메모리를 해제 |
|regerror	| 에러 보고 |

### regcomp  
```
int regcmop(
	regex_t *restrict		preg
	const char * restrict		pattern
	int				cflags)
```
-   ㅁ

**Return Value**  
-   ㅁ

**Description**  


### regexec  
```
int regexec(
	const regx_t *restrict		preg,
	const char *restrict		string,
	size_t				nmatch,
	regmatch_t			pmatch[restrict],
	int				eflags)
```  
**Return Value**  
-   ㅁ

**Description**  

### regerror  
```
int regerror(
	int errcode,
	const regex_t *restrict preg,
	char *restrict errbuf,
	size_t errbuf_size)
```  
**Return Value**  
-   ㅁ

**Description**  

### regfree  
```
int regerror(
	int errcode,
	const regex_t *restrict preg,
	char *restrict errbuf,
	size_t errbuf_size)
```  
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



