### [뒤로가기](./word_tree.md)

### word_tree.cc
```cpp
#include "word_tree.h"

// 기본 생성자
WordDictionary :: WordDictionary() : countWord(0), countNode(0) ,head(new Node){	}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### Add
```cpp
// 단어 추가 
bool	WordDictionary :: Add(string word, string mean)
{
	bool ret = FAIL;
	StringProcess(word);
	StringProcess(mean);
	
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ단어추가ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	cout << "expression\t: \"" << word << "\"\n";
	cout << mean << "\n";
	
	ret = Insert(word, mean);
	if(ret)
	{
		cout << "추가 성공\n";
	}
	else
	{
		cout << "추가 실패\n";
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
	return ret;
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### remove
```cpp
// 단어 삭제
bool	WordDictionary :: Remove(string target)
{
	bool ret = false;
	cout << __FUNCTION__ << " Execute" << "\n";
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ단어삭제ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	cout << "target\t: " << target << "\n";
	ret = DeleteWord(target);

	if(ret)
	{
		cout << "삭제 성공\n";
	}
	else
	{
		cout << "삭제 실패\n";
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
	return ret;
}

```

###### [뒤로가기](./word_tree.md#public-메소드)
#### clear
```cpp
void	WordDictionary :: Clear()
{
	if(NULL != head->brother)
	{
		countNode = 1;
		countWord = 0;

		DFSClear(head->brother);
		head->brother = NULL;
	}
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### replaceword
```cpp
void 	WordDictionary :: ReplaceWord(string oldWord, string newWord)
{
		
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ단어변경ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	if(ModifyWord(oldWord,newWord))
	{
		cout << "변경성공\n";
	}
	else
	{
		cout << "변경실패\n";
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### replacemean
```cpp
void 	WordDictionary :: ReplaceMean(string word, string mean)
{

	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ의미변경ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	if(ModifyMean(word,mean))
	{
		cout << "변경성공\n";
	}
	else
	{
		cout << "변경실패\n";
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### printsingle
```cpp
// 뜻 출력
void	WordDictionary :: Print(string target)
{
	Node* destination;
	int n;
	StringProcess(target);

	destination = Find(target, n);
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ단어검색ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	do
	{	
		if( NULL == destination ) { break; }
		if( !destination->isWord ) { break; }
		if( n + 1 != target.length() ) { break; }
		if( destination->letter != target.back()) {break;}
		cout << "단어\t: " << target;
		cout << "\n" << destination->mean;

		cout << "\nㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
		return;
	}while(false);

	cout << "단어를 찾을 수 없습니다.\n";
	cout << "\nㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
	return;
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### printall
```cpp
// 전체 출력
void	WordDictionary :: Print()
{
	string temp = string("");

	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ전체단어ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	if(NULL != head){

		if(NULL == this->head->brother)
		{
			cout << "등록된 단어가 없습니다.\n";
		}
		else 
		{
			DFSPrint(head->brother, temp);
		}
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### similarprint
```cpp
// 유사 단어 전부 출력
void	WordDictionary :: SimilarPrint(string target)
{
	Node* destination;
	string temp;
	int n;
	
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ유사단어ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	cout << "target\t: " << target << "\n";
	destination = Find(target, n);
	temp = target.substr(0, n+1);

	if(destination->isWord)
	{
		cout << temp << "\t : ";
		cout << destination->mean << "\n";
	}
	if(NULL == destination->child)
	{
		cout << "유사 단어 없음\n";
	}
	else
	{
		this->DFSPrint(destination->child, temp);
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### tree
```cpp
// 트리 출력
void	WordDictionary :: Tree()
{
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡTREEㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	if(NULL != head->brother)
	{	
		DFSTree(head->brother, 0);
	}
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### info
```cpp
// 현재 정보 출력
void	WordDictionary :: Info()
{
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡ트리정보ㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	cout << "저장된 단어 수\t: " << countWord << "\n";
	cout << "생성된 노드 수\t: " << countNode << "\n";
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

###### [뒤로가기](./word_tree.md#public-메소드)
#### printallinfo
```cpp
void	WordDictionary :: PrintAll()
{
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ전체정보ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n";
	Info();
	Tree();
	Print();
	cout << "ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n\n";
}
```

### [위로가기](#word_tree.cc)
### [뒤로가기](./word_tree.md)
