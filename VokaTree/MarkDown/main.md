[뒤로가기](../Readme.md)

## 설명
- 주요 함수를 순차적으로 실행시킴
- 환경 설정

### main.h
```cpp
#include<iostream>
#include"IOSystem.h"

using namespace std;

#define SUCCESS	1	// 성공
#define FAIL	0	// 실패
```

### main.cc
```cpp
#include "main.h"
int main(void)
{
	// 사전 클래스 생성
	WordDictionary wd;
	
	// 예제 삽입 및 동작 실행
	DictionaryExample(wd);

	// 입출력 시스템 가동
	DictionaryIOSystem(wd);
	return 0;
}

```
### [위로가기](#설명)
### [뒤로가기](../Readme.md)