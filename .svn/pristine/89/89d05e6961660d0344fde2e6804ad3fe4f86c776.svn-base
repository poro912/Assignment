#include"word_tree.h"

// 문자열이 저장될 수 있거나 해당 문자열의 위치를 반환
// 반환값
// 주소값	: 저장 가능 또는 동일한 문자열 위치
// NULL		: 에러
Node*	WordDictionary :: Find(
		const string&	target,
		int&	 		n,
		Node*& 			parent
		)
{
	Node* temp = head;
	parent = NULL;
	n = 0;

	// 아무 문자열도 안들어온다면
	if(target.compare("") == 0)
		return NULL;

	// 위치 탐색
	while(NULL != temp->child || NULL != temp->brother)
	{
		// 같은 문자가 아니라면
		// 오른쪽 확인
		if(target[n] != temp->letter)
		{
			// 오른쪽에 노드가 없다면
			if(NULL == temp->brother){ break; }
			// 오른쪽이 나보다 큰 문자라면
			if(target[n] < temp->brother->letter){ break; }
			// 탐색 노드를 오른쪽으로 이동
			parent = temp;
			temp = temp->brother;
		}
		// 같은 문자라면
		// 아래쪽 확인
		else
		{
			// 남은 문자가 없다면 (노드가 이미 만들어짐, 같은 단어가 이미 있음)
			if( n >= target.length() ) { break; }
			// 아래 노드가 없다면
			if(NULL == temp->child) { break; }
			
			// 탐색 노드를 아래로 이동 및 다음 문자로 이동	
			++n;
			// if( n >= target.length()) break;
			parent = temp;
			temp = temp->child;
				
		}
	}
	return temp;
}

Node*	WordDictionary :: Find(
		const string&	target,
		int&	 		n	
		)
{
	Node* temp;
	return Find(target,n,temp);
}

bool WordDictionary :: Insert(
	const string &	word,
	const string &	mean)
{
	Node* destination;
	Node* parent;
	int n = 0;
	bool ret = FAIL;	
	
	do
	{
		// 빈 문자열이라면 종료
		if(0 == word.length()) {ret = FAIL; break;}

		// 저장을 시작함
		// 저장 위치 탐색
		destination = Find(word, n, parent);
		if(NULL == destination) 
		{
			cout<<"Find Fail\n"; 
			ret = FAIL;
			break;
		}

		// 사전에 등록함
		ret = Save(destination, parent, word, mean, n);
	}while(false);
	return ret;
}

// 문자열을 저장함

bool	WordDictionary :: Save(
	Node*			destination,
	Node*			parent,
	const string&	word,	
	const string&	mean,	
	int&			n)
{
	// 모든 글자노드가 존재 한다면
	if(n == word.length())
	{
		// 뜻 저장
		return SaveMean(destination, mean);
	}
	
	// destination의 글자와 현재 글자가 다르다면
	if(word[n] != destination->letter)
	{
		AddRightNode(destination, parent, word.substr(n) ,mean);
		//AddRightNode(parent, word.substr(n) ,mean);
		return SUCCESS;
	}
	// destination의 글자와 현재 글자가 같다면
	else
	{
		// 남은 글자가 있다면
		if(n + 1 < word.length())
		{
			// 아래쪽에 저장 
			AddBelowNode(destination,word.substr(n + 1),mean);
			return SUCCESS;
		}
		else
		{
			return SaveMean(destination,mean);
		}
	}
	return FAIL;
}

// 노드 리스트 생성
Node*	WordDictionary :: MakeNodes(const string& target, const string& mean)
{
	Node head;
	Node* now = NULL;
	Node* temp = NULL;
	now = &head;

	for(auto i : target)
	{
		temp = new Node(i);
		now->child = temp;
		now = temp;
		++this->countNode;
	}

	if(	NULL == temp )
		return NULL;
	// 마지막 노드에 정의 저장
	SaveMean(temp,mean);

	// target이 빈 문자열이라면 null 반환
	return head.child;
}

bool	WordDictionary :: SaveMean(
	Node*			destination,
	const string&	mean,
	bool			replace)
{
	// 뜻이 저장되어있지 않다면
	if(!destination->isWord)
	{
		destination->mean = mean;
		destination->isWord = true;
		++this->countWord;
		return SUCCESS;
	}
	// 수정모드라면
	if(replace) {
		destination->mean = mean;
		return SUCCESS;
	}
	return FAIL;
}


// 현재의 오른쪽에 노드리스트 붙이기
void	WordDictionary :: AddRightNode(
	Node* 			destination, 
	Node*			parent,
	const string& 	target, 
	const string& 	mean)
{
	//cout << "오른쪽에 노드 연결\n";
	Node* temp;
	temp = MakeNodes(target, mean);
	
	
	if(temp->letter < destination->letter)
	{
		if(parent->child == destination)
		{
			temp->brother = destination;
			parent->child = temp;
		}
		else
		{
			temp->brother = destination;
			parent->brother = temp;
		}
	}
	else
	{
		temp->brother = destination->brother;
		destination->brother = temp;
	}

}

// 현재의 아래쪽에 노드리스트 붙이기
void	WordDictionary :: AddBelowNode(
	Node* 			destination, 
	const string& 	target, 
	const string& 	mean)
{
	
	//cout << "아래쪽에 노드 연결\n";
	Node* temp;
	temp = MakeNodes(target, mean);

	destination->child = temp;
}

// 단어 삭제 
bool	WordDictionary :: DeleteWord(string target)
{
	Node * temp;
	Node * parent;
	int n;

	temp = Find(target,n,parent);

	if(n == target.length())
	{
		temp = parent;
		--n;
	}

	// 탐색 결과 동일한 문자열이 아니라면
	if(n + 1 != target.length() || !temp->isWord) {return FAIL;}
	//if(target.back() != temp->letter) {return FAIL;}
	// 해당 문자를 삭제한다.
	DFSDelete(head,target);

	if(temp != Find(target,n) || !temp->isWord) {return SUCCESS;}

	return FAIL;
}
// false	삭제 불가
// true		삭제 가능
bool	WordDictionary :: DFSDelete(
		Node*			now,
		const string& 	target,
		int 			n)
{
	Node* temp;
	
	do
	{
		// 내가 타겟 이라면
		if(target[n] == now->letter)	{break;}

		temp = now->brother;
		// 형제가 없다면
		if(NULL == temp)		{ return FAIL;}
		// 형제 탐색
		// 형제에게서 삭제가 일어나지 않았다면
		if(!DFSDelete(temp, target, n))	{ return FAIL; }
		// 형제가 삭제 불가능하면
		if(!Deleteable(temp))	{ return FAIL; }

		//형제를 삭제한다.
		now->brother = temp->brother;
		DeleteNode(temp);
		return FAIL;
	}while(false);

	// 내가 타겟이라면
	// 내가 마지막 문자라면?
	if( n + 1 == target.length() )
	{
		// 의미를 지워버림
		DeleteMean(now);
		return SUCCESS;
	}

	temp = now->child;
	
	// 아이가 없다면
	if(NULL == temp) { return FAIL; }
	
	// 아이를 탐색함
	// 아이에게서 삭제가 일어나지 않으면
	if(!DFSDelete(temp,target,n+1)){ return FAIL; }

	if(!Deleteable(temp)){return FAIL;}
	
	// 아이의 형제를 직속 아이로 편입
	now->child = temp->brother;

	// 아이 삭제	
	return DeleteNode(temp);
}

// 대상이 갖고있는 뜻을 삭제함
bool	WordDictionary :: DeleteMean(Node* target)
{
	if(NULL == target)	{ return FAIL; }
	if(!target->isWord)	{ return FAIL; }
	
	target->isWord = false;
	--countWord;
	return SUCCESS;
}

// 의미가 저장안된경우, 아이가 없는경우에만 삭제됨
bool	WordDictionary :: Deleteable(Node* target)
{
	if(NULL == target)			{ return false; }
	// if(NULL != target->brother)	{ return false; }
	if(NULL != target->child)	{ return false; }
	if(target->isWord)			{ return false; }

	return true;
}

// 노드 삭제	
bool	WordDictionary :: DeleteNode(Node* target)
{
	// 삭제 불가능
	if(!Deleteable(target)){ return FAIL; }

	// 삭제 가능
	delete target;
	--countNode;

	return SUCCESS;
}

bool	WordDictionary :: ModifyWord(string oldWord, string newWord)
{
	Node* temp;
	int n;
	temp = Find(oldWord, n);
	
	// 같은 단어가 아니거나 단어가 아니라면 종료
	if(n + 1 != oldWord.length() || !temp->isWord) {return FAIL;}

	// 새로운 단어 생성
	if(!Insert(newWord,temp->mean)) {return FAIL;}
	// 기존 단어 삭제
	if(!DeleteWord(oldWord)) 
	{
		DeleteWord(newWord);
		return FAIL;
	};
	
	return SUCCESS;
}
bool	WordDictionary :: ModifyMean(string target, string mean)
{
	Node * temp;
	int n;
	temp = Find(target,n);

	// 같은 단어가 아니거나 단어가 아니라면 종료
	if(n + 1 != target.length() || !temp->isWord){return FAIL;}
	// 의미만 바꿈
	temp->mean = mean;

	return SUCCESS;
}
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

void 	WordDictionary :: DFSClear(Node* node)
{
	if(NULL != node->child)
	{
		DFSClear(node->child);
	}
	if(NULL != node->brother)
	{
		DFSClear(node->brother);
	}
	delete node;
}

















