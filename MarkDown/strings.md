### [뒤로가기](../TreeVocaNote.md)

## 설명
- 입출력 시 문제가 될 수 있는 입력 데이터를 필터링 하기위한 함수 정의
- 공백 제거 및 두개 이상의 띄어쓰기에 대해 처리
### strings.h
```cpp
#include<string>
#include<algorithm>
using namespace std;

using std::string;

// 문자열 전처리
void StringProcess(string& target);

// 왼쪽 공백 제거 
inline string& ltrim(string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}
// 오른쪽 공백 제거 
inline string& rtrim(string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}
// 양쪽 공백 제거
inline string& trim(string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}
```

### StringProcess
```cpp
void StringProcess(string& target)
{
	// 두개 이상의 공백 전부 삭제
	while(target.find("  ") != string::npos)
	{
		target.replace(target.find("  "),2, " ");
	}
	
	// 좌우 공백 제거	
	trim(target);

	// 소문자 문자열로 변환
	transform(target.begin(), target.end(), target.begin(), ::tolower);

}
```