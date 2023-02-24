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

