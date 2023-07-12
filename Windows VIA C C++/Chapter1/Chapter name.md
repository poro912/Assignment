<h1> Error Handling </h1>

<h2> Index </h2>

- [서론](#서론)
- [반환 자료형에 따른 함수 해석](#반환-자료형에-따른-함수-해석)
- [함수 실패 시 매커니즘](#함수-실패-시-매커니즘)
	- [GetLastError](#getlasterror)
	- [에러코드](#에러코드)
- [함수의 성공](#함수의-성공)
- [에러 처리 확인 유틸리티](#에러-처리-확인-유틸리티)
	- [FormatMessage](#formatmessage)
- [**Parametters**](#parametters)
- [**Return Value**](#return-value)
- [sub-title](#sub-title)
	- [function\_name](#function_name)
- [**Parametters**](#parametters-1)
- [**Return Value**](#return-value-1)


## 서론
윈도우 함수 호출 시 전달된 인자가 유효하지 않거나 해당 기능을 수행할 수 없으면 실패를 반환한다.  
함수 실패여부와 원인을 알아내는 과정이 반드시 필요하다.
발생할 가능성이 있는 모든 에러 코드를 32비트 숫자로 정의해 두었다.



## 반환 자료형에 따른 함수 해석
| 자료형 	| 실패했을 때 값 |
| :-- 		| :--  |
| VOID		| 실패하지 않는다.</br>(아주 적은 수의 윈도우 함수만이 VOID 반환형을 갖는다.)	|
| BOOL		| 실패시 0을 반환한다.</br>성공시에는 0이 아닌값을 반환한다.</br>성공여부를 확인하기위해 FALSE인지 아닌지 확인해야한다.	|
| HANDLE	| 실패시 대개 NULL을 반환한다.</br>성공 시 유효한 오브젝트 핸들을 반환한다.</br>몇몇함수는 INVALID_HANDLE_VALUE(-1)을 반환한다.</br>플랫폼 SDK 문서에 반환값에 대해 명확하게 기술되어있다.	|
| PVOID		| 실패시 NULL을 반환한다.</br>성공시 PVOID가 데이터를 저장하고있는 메모리 주소를 가리킨다.	|
| LONG/DWORD	| 보통 개수를 반환한다.</br>특수한 이유로 개수를 반환하지 못한다면 0이나 -1을 반환한다.</br>잠재적인 에러를 미연에 방지하기위해 플랫폼 SDK문서를 주의 깊게 살펴봐야한다.	|



## 함수 실패 시 매커니즘
함수 실패시 함수 호출 스레드의 스레드 지역저장소(thread local storage)에 적절한 에러코드를 저장한다.  
이를 통해 여러 스레드가 동작하더라도 상호간에 영향을 미치지 않게된다.  

### GetLastError
	DWORD GetLastError();

**Return Value**
- 발생 에러코드

**Description**  
가장 최근에 호출된 함수의 에러코드를 가져온다.


### 에러코드
	MessageId
	MessageText
	ErroeCode
**Description**  
WinError.h 헤더파일에 정의된다.
윈도우의 에러코드는 메시지ID, 텍스트, 에러코드로 구성된다.


## 함수의 성공
서로다른 성공 이유가 있는 함수들이 존재한다.  
에러코드저장과 동일한 메커니즘을 활용하여 이를 구분할 수 있다.  
플랫폼 SDK 문서상에 GetLastError를 이용할 것을 명확하게 기술되어있다. 

## 에러 처리 확인 유틸리티
Visual Studio 내에 포함된 디버거는 Watch 창을 통해 에러코드와 메시지를 확인할 수 있다.  
Visual Studio 내에 포함된 Error Lookup 유틸리티를 활용하면 에러코드에 해당하는 메시지를 쉽게 확인할 수 있다.  


### FormatMessage
	DWORD FormatMessage(
		

	)
**Parametters**
- 

**Return Value**
- 

**Description**  
에러코드를 메시지 텍스트로 변환해주는 함수


## sub-title
### function_name
	[function]
**Parametters**
- 

**Return Value**
- 

**Description**  

