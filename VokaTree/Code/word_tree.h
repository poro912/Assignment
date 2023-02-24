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
		Node*	Find(
				const string&	target,
				int&	 		n,
				Node*& 			parent);
		Node* Find(const string &target, int& n);
	
		// 단어 삽입
		bool Insert(
				const string&	target,
				const string&	mean);
		// 문자열을 저장
		bool Save(
				Node * 			destination,
				Node *			parent,
				const string& 	target,
				const string& 	mean,
				int& 			n);
		// 노드리스트를 생성
		Node* MakeNodes(
				const string& 	target, 
				const string& 	mean);
		// 노드에 의미를 저장
		bool SaveMean(
				Node *			destination,
				const string&	mean,
				bool			replace = false);
		// 형제 노드로 붙임
		void AddRightNode(
				Node * 			destination,
				Node *			parent,
				const string& 	target, 
				const string& 	mena);
		// 자식 노드로 붙임
		void AddBelowNode(
				Node* 			destination, 
				const string& 	target, 
				const string& 	mean);
		// 단어 삭제
		bool DeleteWord(string target);
		// 의미 삭제
		bool DeleteMean(Node* target);
		// 노드 삭제
		bool DeleteNode(Node* target);
		// 노드 삭제 가능여부 반환
		bool Deleteable(Node* target);
		// 단어 수정(삭제 후 생성)
		bool ModifyWord(string oldWord,string newWord);
		// 의미 수정
		bool ModifyMean(string target, string mean);
		// 전체 탐색
		void DFSTree(Node* now,int n);
		// 전체 출력
		void DFSPrint(Node* now, string& stack);
		// 대상 삭제
		bool DFSDelete(
				Node* 			now,
				const string& 	target,
				int				n = 0);
		void DFSClear(Node * node);
	public :
		// 생성자
		WordDictionary();
		// 단어 추가
		bool Add(string target, string mean);
		// 단어 삭제
		bool Remove(string target);
		// 해당 단어 출력
		void Print(string target);
		// 전체 단어 출력
		void Print();
		// 유사단어 전체 출력
		void SimilarPrint(string target);
		// 해당 단어가 존재하는지 확인
		bool Search(string target);
		// 가장 유사한 단어 출력
		string SimilarSearch(string target);
		// 단어 변경
		void ReplaceWord(string oldWord, string newWord);
		// 의미 변경
		void ReplaceMean(string word, string mean);
		// 트리 출력
		void Tree();
		// 저장 정보 출력
		void Info();
		void PrintAll();
		// 전체 삭제
		void Clear();
		
};
