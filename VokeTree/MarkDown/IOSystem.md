### [뒤로가기](../Readme.md)

## 설명
- 사용자 입력처리를 담당
- CLI환경에서 사용할 수 있는 도움말 등의 기능

### 메소드
* [DictionaryIOSystem](#iosystem)
	- IOSystem 가동
* [DictionaryHelp](#help)
	- 사용할 수 있는 명령어 리스트 출력
* [DictionaryAdd](#add)
	- Add 메소드 호출
* [DictionaryExample](#example)
	- 예시 함수 실행

### IOSystem.h
```cpp
#include<iostream>
#include"word_tree.h"

using namespace std;

// 사전 사용자 입출력 시스템
void DictionaryIOSystem	(WordDictionary& wd);
// 도움말 출력 
void DictionaryHelp		();
// 내용 추가
void DictionaryAdd		(WordDictionary& wd);
// 예제
void DictionaryExample	(WordDictionary& wd);
```


### iosystem
```cpp
// 사전 사용자 입출력 시스템
void DictionaryIOSystem(WordDictionary& wd)
{
	char commend = 0;
	while(true)
	{
		DictionaryHelp();
		cout << "Enter the commend : ";
		cin >> commend;
		switch(commend)
		{
			case 'Q' :
			case 'q' : return;
			case 'T' : 
			case 't' : 
						wd.Tree();
						break;
			case 'A' :
			case 'a' :
					   DictionaryAdd(wd);
					   break;
			default : 
					   break;
		}
	}
}
```

### help
```cpp
// 도움말 출력
void DictionaryHelp()
{
	cout << "a\t: add\t\td\t:delete\n";
	cout << "t\t: tree\n";
	cout << "q\t: exit\n";
}
```

### add
```cpp
// 내용 추가
void DictionaryAdd(WordDictionary& wd)
{
	string word;
	string mean;
	
	cout << "!q : exit";
	cout << "Enter the word : ";
	getline(cin, word);
	if(0 == word.compare("!q"))
		return;	
	cout << "Enter the definition\n";
	getline(cin, mean);
	if(0 == mean.compare("!q"))
		return;

	if(	//wd.Add(word)
		wd.Add(word,mean)
		)
	{
		cout << "Add Successed\n";
	}
	else
	{
		cout << "Add Failed";
	}
}
```

### example
```cpp
// 예제
void DictionaryExample(WordDictionary & wd)
{
	// wd.Add(string(""),string(""));
	wd.Print();
	wd.Add(string("  hello  "),string("안녕하세요"));
	wd.Add(string("App"),	string("application의 준말"));
	wd.Add(string("Apple"),	string("사과"));
	wd.Add(string("application"),string("1.지원\n2.적용, 응용"));

	wd.Add(string("craw"),	string("새의 모이주머니"));
	wd.Add(string("crawl"),	string("기어가다"));
	wd.Add(string("creep"),	string("살금살금 움직이다"));
	wd.Add(string("create"), string("1.창조하다\n2.(느낌이나 인상을)자아내다"));
	wd.Add(string("cry"),	string("울다"));
	wd.Add(string("crown"),	string("왕관"));
	
	wd.Add(string("WIN"),	string("이기다"));
	wd.Add(string("wint"),	string("날개"));


	wd.Add(string("How   Are   You"),string("1.안녕하십니까?\n2.처음 뵙겠습니다."));
	wd.Add(string("  hello  "),string("안녕하세요"));
	wd.Print(string("hello"));
	wd.Print();
	wd.Search(("hello"));
	wd.SimilarPrint("a");
	wd.SimilarPrint("app");
	wd.Tree();
	wd.Remove(string("hello"));
	wd.Remove(string("hello"));	
	wd.Remove(string("app"));
	wd.Tree();
	wd.Remove(string("create"));
	wd.Info();
	wd.PrintAll();

	wd.Print("wint");
	wd.ReplaceWord("wint","wing");
	wd.Tree();
	wd.Print("wint");
	wd.Print("wing");
	wd.ReplaceMean("wing","1.날개\n2.날개를 펴다");
	wd.Print("wing");
	wd.Print();
	wd.Clear();
	wd.PrintAll();

	return;
}
```
### [위로가기](#설명)
### [뒤로가기](../Readme.md)