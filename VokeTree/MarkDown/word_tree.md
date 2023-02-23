## [뒤로가기](../Readme.md)

## 설명
- 주요 함수들을 정의
- 데이터 가공 및 실행결과 출력
- word_tree.cc  : 출력을 담당하거나 사용자가 호출하게 되는 메소드 정의
- word_tree.control : 내부 데이터에 영향을 주거나 특정 알고리즘이 필요한 메소드 정의
- public 메소드 11개
- private 메소드 18개
## 구조체 및 클래스
* Node 구조체
	- 하나의 노드를 구성하는 구조체
	- 다음 노드의 주소, 갖고있는 문자, 뜻에 대한 정보를 저장한다.
	- 두개의 생성자로 구성되어있다. 
* WordDictionary 클래스
	- 내부 메커니즘 및 호출이 되는 메소드에대해 정의 되어있다.
	- 현재까지 저장된 단어의 수, 만들어진 노드의 수 에 대해 저장한다.
	- head 노드의 주소를 저장한다.

### Node

```cpp
typedef struct _NODE
{
	_NODE*		child;		// 하위 노드
	_NODE*		brother;	// 동급 노드
	char		letter;		// 문자
	bool		isWord;		// isExpress
	string		mean;		// definition
	
	// 생성자
	_NODE()
	{
		//cout << "Create Node\n";
		memset(this, 0x0, sizeof(struct _NODE));
	}
	_NODE(char letter)
	{
		//cout << "Create Node\tletter : " << letter << "\n";
		memset(this, 0x0, sizeof(struct _NODE));
		this->letter = letter;
	}
}Node;
```
### WordDictionary
```cpp
class WordDictionary
{
	private :
		// 저장된 단어의 수
		int countWord;
		// 저장된 노드의 수
		int countNode;
		// 헤드 노드
		Node* head;
}
```

## Public 메소드
### [Code](./word_tree_public.md)
* [WordDictionary](./word_tree_public.md#creater)
	- 생성자 
* [Add](./word_tree_public.md#add)
	- 단어와 의미 추가
* [Remove](./word_tree_public.md#remove)
	- 단어 삭제
* [Clear](./word_tree_public.md#clear)
	- 트리 초기화
* [ReplaceWord](./word_tree_public.md#replaceword)
	- 단어 철자 수정
* [ReplaceMean](./word_tree_public.md#replacemean)
	- 단어 의미 수정
* [Print](./word_tree_public.md#printsingle)
	- 단일 단어 탐색 및 출력
* [Print](./word_tree_public.md#printall)
	- 저장된 모든 단어 탐색 및 출력
* [SimilarPrint](./word_tree_public.md#similarprint)
	- 유사 단어 모두 탐색 및 출력
* [Tree](./word_tree_public.md#tree)
	- 트리 상태 가시화 출력
* [Info](./word_tree_public.md#info)
	- 트리의 정보 출력
* [PrintAll](./word_tree_public.md#printallinfo)
	- 트리에 대한 모든 정보 출력

## Private 메소드
### [Code](./word_tree_private.md)
* [Find](./word_tree_private.md#find)
* [Find](./word_tree_private.md#find)
    - 조건에 해당하는 노드 탐색
* [Save](./word_tree_private.md#save)
	- 데이터 삽입
* [Insert](./word_tree_private.md#insert)
	- 탐색 및 삽입
* [MakeNodes](./word_tree_private.md#makenode)
	- 서브 트리 생성
* [SaveMean](./word_tree_private.md#savemean)
	- 의미 저장
* [AddRightNode](./word_tree_private.md#addrightnode)
	- 형제노드로 트리 잇기
* [AddBelowNode](./word_tree_private.md#addbelownode)
	- 자식노드로 트리 잇기
* [Deleteable](./word_tree_private.md#deleteable)
	- 노드가 지워질 수 있는 상태인지 반환
* [DeleteNode](./word_tree_private.md#deletenode)
	- 노드 삭제
* [DeleteMean](./word_tree_private.md#deletemean)
	- 노드에 저장된 의미 삭제
* [DeleteWord](./word_tree_private.md#deletenode)
	- 단어 삭제
* [ModifyWord](./word_tree_private.md#modifyword)
	- 단어 철자 변경
* [ModifyMean](./word_tree_private.md#modifymean)
	- 단어 의미 변경
* [DFSDelete](./word_tree_private.md#dfsdelete)
	- 단어 삭제를 위한 DFS 메소드
* [DFSTree](./word_tree_private.md#dfstree)
	- 트리 상태 출력을 위한 DFS 메소드
* [DFSPrint](./word_tree_private.md#dfsprint)
	- 단어 의미 출력을 위한 DFS 메소드
* [DFSClear](./word_tree_private.md#dfsclear)
	- 모든 단어를 삭제하기위한 DFS 메소드

### word_tree.h
```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<cstring>
#include<list>

#include"strings.h"

#define SUCCESS	1
#define FAIL	0

using namespace std;

typedef struct _NODE
{
	_NODE*	child;
	_NODE*	brother;
	char	letter;	
	bool	isWord;	// isExpress
	string	mean;	// definition
	
	// 생성자
	_NODE()
	{
		//cout << "Create Node\n";
		memset(this, 0x0, sizeof(struct _NODE));
	}
	_NODE(char letter)
	{
		//cout << "Create Node\tletter : " << letter << "\n";
		memset(this, 0x0, sizeof(struct _NODE));
		this->letter = letter;
	}
}Node;

class WordDictionary
{
	private :
		// 저장된 단어의 수
		int countWord;
		// 저장된 노드의 수
		int countNode;
		// 헤드 노드
		Node* head;

		// 문자열이 저장될 수 있는 위치를 확인
		Node* Find(	const string &target, int& n);
		Node* Find(
				const string&		target,
				int&	 		n,
				Node*& 			parent);

		// 문자열을 저장
		bool Save(
				Node * 			destination,
				Node *			parent,
				const string& 		target,
				const string& 		mean,
				int& 			n);

		// 단어 삽입
		bool Insert(
				const string&		target,
				const string&		mean);

		// 노드리스트를 생성
		Node* MakeNodes(
				const string& 		target, 
				const string& 		mean);

		// 노드에 의미를 저장
		bool SaveMean(
				Node *			destination,
				const string&		mean,
				bool			replace = false);

		// 형제 노드로 붙임
		void AddRightNode(
				Node * 			destination,
				Node *			parent,
				const string&		target, 
				const string&		mena);

		// 자식 노드로 붙임
		void AddBelowNode(
				Node* 			destination, 
				const string& 		target, 
				const string& 		mean);
				
		// 노드 삭제 가능여부 반환
		bool Deleteable(Node* target);

		// 노드 삭제
		bool DeleteNode(Node* target);

		// 의미 삭제
		bool DeleteMean(Node* target);

		// 단어 삭제
		bool DeleteWord(string target);

		// 단어 수정(삭제 후 생성)
		bool ModifyWord(string oldWord,string newWord);

		// 의미 수정
		bool ModifyMean(string target, string mean);

		// 대상 삭제
		bool DFSDelete(
				Node* 			now,
				const string& 		target,
				int			n = 0);

		// 전체 탐색
		void DFSTree(Node* now,int n);

		// 전체 출력
		void DFSPrint(Node* now, string& stack);

		// 전체 삭제
		void DFSClear(Node * node);

	public :
		// 생성자
		WordDictionary();

		// 단어 추가
		bool Add(string target, string mean);

		// 단어 삭제
		bool Remove(string target);

		// 전체 삭제
		void Clear();	
		// 단어 변경
		void ReplaceWord(string oldWord, string newWord);

		// 의미 변경
		void ReplaceMean(string word, string mean);

		// 해당 단어 출력
		void Print(string target);

		// 전체 단어 출력
		void Print();

		// 유사단어 전체 출력
		void SimilarPrint(string target);

		// 트리 출력
		void Tree();

		// 저장 정보 출력
		void Info();

		// 모든 정보 출력
		void PrintAll();
};
```

### [위로가기](#설명)
### [뒤로가기](../Readme.md)

