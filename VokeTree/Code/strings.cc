#include "strings.h"

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

