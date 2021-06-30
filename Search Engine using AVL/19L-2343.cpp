#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include <sstream>

using namespace std;

int Tol_words = 0; //global variable to calculate number of word in Index

int max(int rh, int lh) // global func to find max height from both child node
{
	return (rh < lh) ? lh : rh;
}

template <typename Q>
class Stack		// stack implementation
{
private:
	class node
	{
	public:
		Q data;
		node* next;
	};
	node* head; // top pointer 
public:
	Stack()
	{
		head = NULL;
	}

	void Push(Q d)
	{
		node* obj = new node;
		obj->data = d;
		obj->next = head;
		head = obj;
	}

	bool isEmpty() //checks if stack is empty
	{
		if (head == NULL)
			return true;
		return false;
	}

	void pop()
	{
		if (!isEmpty())
		{
			node* del = head;
			head = head->next;
			delete del;
		}
	}

	Q peek()
	{
		if (!isEmpty())
		{
			return head->data;
		}
	}

	void print()
	{
		node* trav = head;
		while (trav != NULL)
		{
			cout << trav->data;
			cout << endl;
			trav = trav->next;
		}
	}

	~Stack()
	{
		while (!isEmpty())
		{
			pop();
		}
	}
};


template <typename T>
class List
{
private:
	class node
	{
	public:

		T* data;
		node* next;
		node* prev;
		node()
		{
			next = NULL;
			prev = NULL;
		}
		~node()				// For T as a class memeber the node destructor will destroy it's dynamic object 
		{
			if (data != NULL)
			{
				delete data;
			}
		}
	};
	node* head;
	node* tail;
	int size;

public:

	List()
	{
		head = new node;
		tail = new node;
		head->prev = NULL;
		tail->next = NULL;
		head->next = tail;
		tail->prev = head;
		this->size = 0;		//total number of data items in the list 
	}

	List(const List &obj1) // copy constructor
	{
		head = new node;
		tail = new node;
		head->prev = NULL;
		tail->next = NULL;
		head->next = tail;
		tail->prev = head;

		this->size = 0;
		if (obj1.head->next != tail)
		{
			node* cur = obj1.head->next;
			while (cur != obj1.tail)
			{
				this->insertAtEnd(cur->data);
				cur = cur->next;
			}

		}
	}


	List& operator=(const List &obj3)
	{
		while (this->head->next != tail)
		{
			DeleteAtStart();
		}

		head = new node;
		tail = new node;
		head->prev = NULL;
		tail->next = NULL;
		head->next = tail;
		tail->prev = head;

		this->size = 0;
		if (obj3.head->next != tail)
		{
			node* cur = obj3.head->next;
			while (cur != obj3.tail)
			{
				this->insertAtEnd(cur->data);
				cur = cur->next;
			}

		}
		return *this;
	}

	class Iterator
	{
	private:

		node* cur;

	public:

		Iterator(node* obj = NULL)
		{
			cur = obj;
		}

		Iterator(Iterator & obj1)
		{
			cur = obj1.cur;
		}

		Iterator& operator++()
		{
			cur = cur->next;
			return *this;
		}

		Iterator& operator--()
		{
			cur = cur->prev;
			return *this;
		}

		bool operator==(const Iterator& rhs)const
		{
			if (rhs.cur == this->cur)
			{
				return true;
			}
			return false;
		}

		bool operator!=(const Iterator& rhs)const
		{
			if (rhs.cur != this->cur)
			{
				return true;
			}
			return false;
		}

		Iterator& operator=(const Iterator& rhs)
		{
			this->cur = rhs.cur;
			return *this;
		}

		T& operator*()
		{
			return *cur->data;
		}

		void del_node()
		{
			node* del = cur;
			cur = cur->next;
			cur->prev = del->prev;
			del->prev->next = cur;
			delete del;
			del = NULL;
		}
	};

	Iterator begin()
	{
		Iterator it(head->next);
		return it;
	}

	Iterator end()
	{
		Iterator itr(tail);
		return itr;
	}

	void print()
	{
		Iterator prn;
		for (prn = begin(); prn != end(); ++prn)
		{
			cout << *(prn);
		}
		cout << endl << endl;
	}



	void insertAtStart(T* element)
	{
		node* temp = new node;
		temp->data = element;
		temp->next = head->next;
		temp->prev = head;
		head->next->prev = temp;
		head->next = temp;
		temp = NULL;
		size++;     // increment of total number of data elements
	}

	void insertAtEnd(T* element)
	{
		node* tempo = new node;
		tempo->data = element;

		tempo->next = tail;
		tempo->prev = tail->prev;

		tail->prev->next = tempo;
		tail->prev = tempo;

		size++;
	}

	void DeleteAtStart()
	{
		if (head->next != tail)
		{
			node* save = head->next;
			head->next->next->prev = head;
			head->next = head->next->next;

			delete save;
			size--;
		}
	}

	void DeleteAtEnd()
	{
		if (head->next != tail)
		{
			node* del_end = tail->prev;

			del_end->prev->next = tail;
			tail->prev = del_end->prev;

			del_end->next = NULL;
			del_end->prev = NULL;

			delete del_end;
			size--;
		}
	}

	bool SearchTerm(string term, Iterator &itr)    // a  function for term info that checks if term already exist in index or not 
	{
		node* search = head->next;
		for (int i = 0; i < size; i++)
		{
			if (search->data->Termcmp(term) == true)
			{
				Iterator it(search);
				itr = it;
				return true;
			}
			search = search->next;
		}
		return false;
	}

	bool SearchDoc(string docs, Iterator &itr2)     // a function for Doc type that checks if Doc already exist in List or not 
	{
		node* search = head->next;
		for (int i = 0; i < size; i++)
		{
			if (search->data->Search_Doc(docs) == true)
			{
				Iterator it(search);
				itr2 = it;
				return true;
			}
			search = search->next;
		}
		return false;
	}

	bool isEmpty()
	{
		if (head->next == tail)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	~List()
	{

		while (head->next != tail)
		{
			DeleteAtEnd();
		}
		delete head;
		delete tail;
		head = NULL;
		tail = NULL;
	}
};


class DOC_info
{
private:
	string Doc_id;
	int term_freq;
	int points;				// a variable that checks how many "Query" terms does the document has  

public:

	DOC_info()
	{
		Doc_id = "";	     //empties the string with no character
		term_freq = 0;
		points = 0;
	}

	bool Search_Doc(string doc)
	{
		if (this->Doc_id == doc)
		{
			return true;
		}
		return false;
	}

	DOC_info(string id, int freq)
	{
		this->Doc_id = id;
		this->term_freq = freq;
		points = 0;
	}

	DOC_info operator=(DOC_info& obj)
	{
		this->Doc_id = obj.Doc_id;
		this->term_freq = obj.term_freq;
		return *this;
	}

	void inc_freq()
	{
		this->term_freq++;
	}

	friend ostream & operator << (ostream &out, DOC_info &c)
	{
		out << " ==> " << c.Doc_id << " Tf:" << c.term_freq;
		out << endl << endl;
		return out;
	}

	string returnID()
	{
		return Doc_id;
	}

	friend class Search_engine;
};


class Term_info
{
private:
	string term;
	List<DOC_info> l1;

public:

	Term_info(string term = "")
	{
		this->term = term;
	}

	bool Termcmp(string temp) //compares if term already exist in index
	{
		if (this->term == temp)
		{
			return true;
		}
		return false;
	}
	Term_info operator=(Term_info& obj)
	{
		this->term = obj.term;
		return *this;
	}

	bool SearchDoc(string id, List<DOC_info>::Iterator &temp)
	{
		List<DOC_info>::Iterator check = l1.begin();
		while (check != l1.end())
		{
			if ((*check).returnID() == id)
			{
				temp = check;
				return true;
			}
			check++;
		}
		return false;
	}

	bool operator <(const Term_info& obj2)
	{
		return (this->term < obj2.term);
	}

	bool operator >(const Term_info& obj2)
	{
		return (this->term > obj2.term);
	}

	bool operator <(const string s1)
	{
		return (this->term < s1);
	}

	bool operator >(const string s3)
	{
		return (this->term > s3);
	}


	bool operator ==(const Term_info& obj2)
	{
		return (this->term == obj2.term);
	}

	friend ostream & operator << (ostream &out, Term_info &c)
	{
		out << c.term;
		c.l1.print();
		out << endl << endl;
		return out;
	}


	~Term_info()
	{

	}

	friend class Search_engine;
};

template<typename R>
class Avl
{
private:
	struct Stdnode // Avl node
	{
		R* data;
		int height;
		Stdnode* lchild;
		Stdnode* rchild;

		Stdnode(R* data, Stdnode* lchild = NULL, Stdnode* rchild = NULL, int height = 0)
		{
			this->data = data;
			this->lchild = lchild;
			this->rchild = rchild;
			this->height = height;
		}

		~Stdnode()
		{
			if (data != NULL)
			{
				delete data;
			}
		}

	} *root;  // Avl have one data member



	int getheight(Stdnode* ptr)
	{
		if (ptr == NULL)
			return -1;
		else
			return ptr->height;
	}

	int balf(Stdnode* ptr)
	{
		return getheight(ptr->lchild) - getheight(ptr->rchild);
	}

	bool updateHeight(Stdnode* ptr)
	{
		int old_h = getheight(ptr);
		int new_h = max(getheight(ptr->lchild), getheight(ptr->rchild)) + 1;
		ptr->height = new_h;
		return (old_h != new_h); // if false no changes made
	}

	bool getAccess(const R* key, Stdnode*& ptr, Stack<Stdnode*> & ancestors)
	{
		ptr = root;

		while (ptr != NULL && ptr->data != key){
			ancestors.Push(ptr);
			if ((*ptr->data)>(*key))
				ptr = ptr->lchild;
			else
				ptr = ptr->rchild;
		}

		return (ptr != NULL);
	}

	int childCount(Stdnode * ptr)
	{
		if (ptr == NULL || (ptr->lchild == NULL && ptr->rchild == NULL))
		{
			return 0;
		}
		else if (ptr->lchild != NULL && ptr->rchild != NULL)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}




	bool rr(Stdnode * ptr, Stdnode * pptr){
		//right rotate around ptr
		Stdnode* cptr = ptr->lchild;

		if (cptr == NULL)
			return false;
		else{
			//what is the node itself is the root
			//promote cptr
			if (pptr == NULL){
				root = cptr;
			}
			else{
				if (ptr == pptr->lchild)
					pptr->lchild = cptr;
				else
					pptr->rchild = cptr;
			}


			//adjust children
			Stdnode * rc_cptr = cptr->rchild;
			cptr->rchild = ptr;
			ptr->lchild = rc_cptr;

			//update heights of ptr and cptr
			updateHeight(ptr);
			updateHeight(cptr);

			return true;
		}
	}


	bool rl(Stdnode* ptr, Stdnode*pptr)
	{
		//left rotate around ptr

		Stdnode * cptr = ptr->rchild;

		if (cptr == NULL)
			return false;
		else{
			if (pptr == NULL){
				root = cptr;
			}
			else{
				if (ptr == pptr->lchild)
					pptr->lchild = cptr;
				else
					pptr->rchild = cptr;
			}


			Stdnode * lc_cptr = cptr->lchild;
			cptr->lchild = ptr;
			ptr->rchild = lc_cptr;

			//update heights of ptr and cptr
			updateHeight(ptr);
			updateHeight(cptr);
			return true;
		}

	}

public:

	Avl() // constructor
	{
		this->root = NULL;
	}


	void copycons(Stdnode* root, int level) // helper recursively deepcopy data
	{
		if (root == NULL)
		{
			return;
		}
		if (level == 0)
		{
			this->insert(root->data);
		}

		else if (level > 0)
		{
			copycons(root->lchild, level - 1);
			copycons(root->rchild, level - 1);
		}
	}

	Avl(const Avl<R> &obj)		 //copy constructor 
	{
		if (obj.root == NULL)
		{
			this->root = NULL;
		}
		else
		{
			//using level order traversal to copy from one avl to another

			int h = getheight(obj.root);
			for (int i = 0; i <= h; i++)
			{
				copycons(obj.root, i);
			}
		}
	}




	void insert(R* data)
	{
		Stack<Stdnode*> ancestor;
		Stdnode* ptr = NULL;
		if (getAccess(data, ptr, ancestor)) // if data is already there
		{
			return;
		}
		else
		{
			if (ancestor.isEmpty())
			{
				root = new Stdnode(data, NULL, NULL, 0);
			}
			else
			{
				Stdnode* pptr = ancestor.peek();
				if (*(data) < *(pptr->data))
				{
					pptr->lchild = new Stdnode(data, NULL, NULL, 0);
				}
				else
				{
					pptr->rchild = new Stdnode(data, NULL, NULL, 0);
				}
			}


			//insertion has taken place at leaf node now checking validitiy of Avl

			while (!ancestor.isEmpty())
			{
				Stdnode* check = ancestor.peek();
				ancestor.pop();

				//update the height to check of imbalancing 
				updateHeight(check);

				if (balf(check) > 1 || balf(check) < -1) // true if imbalance
				{
					Stdnode* pcheck = ancestor.isEmpty() ? NULL : ancestor.peek();

					if (balf(check) == -2 && balf(check->rchild) == -1)
					{
						//right-right
						rl(check, pcheck);
					}

					else if (balf(check) == +2 && balf(check->lchild) == +1)
					{
						//left-left
						rr(check, pcheck);
					}

					else if (balf(check) == -2 && balf(check->rchild) == +1)
					{
						//right-left
						rr(check->rchild, check);//make it a right-right imbalance
						rl(check, pcheck);
					}
					else if (balf(check) == +2 && balf(check->lchild) == -1)
					{
						//left-right
						rl(check->lchild, check);//make it a left-left imbalance
						rr(check, pcheck);
					}
				}
				// when a correction is done no need to go above so clear stack 
			}

		}
	}

	bool searchterm(string term, Term_info* &ptr)
	{
		Stdnode* search = root;

		while (search != NULL && search->data->Termcmp(term) == false)
		{
			if (*(search->data) < term)
			{
				search = search->rchild;
			}
			else
			{
				search = search->lchild;
			}
		}

		if (search != NULL)
		{
			ptr = search->data;
			return true;
		}
		else
			return false;
	}

	bool searchrec(Stdnode* trav, R* key)
	{
		if (trav == NULL)
		{
			return false;
		}
		if (*(trav->data) == *(key))
		{
			return true;
		}
		else
		{
			*(trav->data) > *(key) ? searchrec(trav->lchild, key) : searchrec(trav->rchild, key);
		}
	}

	bool search(R* data)
	{
		Stdnode *trav = root;
		return searchrec(trav, data);
	}



	int callcount(Stdnode* node, int &count)
	{
		if (node == NULL)
		{
			return count;
		}

		callcount(node->lchild, count);
		count++;
		callcount(node->rchild, count);
	}

	int NoofNodes()
	{
		int count = 0;
		//using in order trav to count no of nodes
		Stdnode* ptr = root;
		return callcount(ptr, count);
	}

	void destroy(Stdnode* node)
	{
		if (node)
		{
			destroy(node->lchild);
			destroy(node->rchild);
			delete node;
		}
	}

	~Avl() //destructor
	{
		Stdnode* des = root;
		destroy(des);
	}
};

class Search_engine
{
private:

	Avl<Term_info> index;   // Index as a List 

public:
	Search_engine()
	{
	
	}

	void Create_Index(string arr[], int n)
	{
		string file;		// to store file name
   
		for (int j = 0; j < n; j++)
		{
			file = arr[j];
			Add_Doc_to_Index(file);
		}

	}


	void Add_Doc_to_Index(string filename)
	{
		ifstream infile;
		string word;

		infile.open(filename);
		while (!infile.eof())
		{
			Term_info * ptr = NULL;


			getline(infile, word, ' ');
			if (index.searchterm(word, ptr) == false)		 //word not found 
			{
				Term_info* obj = new Term_info(word);
				DOC_info *objr = new DOC_info(filename, 1);
				obj->l1.insertAtEnd(objr);
				index.insert(obj);
			}
			else
			{
				List<DOC_info>::Iterator index = (*ptr).l1.begin();
				if ((*ptr).SearchDoc(filename, index) == true)  // if Doc file opened already exist in the list 
				{
					(*index).inc_freq();
				}
				else
				{
					DOC_info *objr2 = new DOC_info(filename, 1);
					(*ptr).l1.insertAtEnd(objr2);
				}

			}
		}
		infile.close();
	}


	void Search_Document(string query)
	{
		stringstream ss(query);
		string terms;

		List<DOC_info> Result; //resulting union list 

		while (getline(ss, terms, ' '))
		{
			Term_info* ptr = NULL;
			if (index.searchterm(terms, ptr) == false)
			{
				cout << terms << " does not exist in document \n";
			}
			else
			{

				List<DOC_info>::Iterator itr = (*ptr).l1.begin();

				List<DOC_info>::Iterator index;

				while (itr != (*ptr).l1.end())
				{
					DOC_info *obj = new DOC_info(*itr);

					if (Result.SearchDoc(obj->Doc_id, index) == true) // if Doc is found combine it's freq with preexisting doc and store in result
					{
						(*index).term_freq = (*index).term_freq + (*itr).term_freq;
						itr++;
						(*index).points++;

					}
					else										// if Doc doesnot exist in the resulting list
					{
						Result.insertAtEnd(obj);
						itr++;
						obj->points++;								//if Doc has the Term word add it's points

					}
				}
			}

		}
		/*Result.print(); */

		List<DOC_info>::Iterator Rank;	// moves through the array

		List<DOC_info>::Iterator Max;   // stores Max 

		List<DOC_info> Ranked_List;    //List to store Ranked Documents 


		while (!Result.isEmpty()) //until the resulting list is not empty
		{
			Rank = Result.begin();		//set Rank to start
			Max = Rank;					//set Max the first node
			Rank++;						//for the comparsion of first and second node

			for (Rank; Rank != Result.end(); ++Rank)
			{
				if ((*Max).points < (*Rank).points)  // Rule 1 : Doc containing largest Query terms
				{
					Max = Rank;
				}
				else if ((*Max).points == (*Rank).points)		//if Query terms are same 
				{
					if ((*Max).term_freq < (*Rank).term_freq)	//Rule 2 : Doc containing largest collective frequency
					{
						Max = Rank;
					}
					else if ((*Max).term_freq == (*Rank).term_freq)		// if Collective frequency is same
					{
						if ((*Max).Doc_id >(*Rank).Doc_id)				//Rule 3 : Alphabetically more bigger term (for strings terminology Doc1 is less than Doc2 so we work opposite)
						{
							Max = Rank;
						}
					}

				}
			}
			DOC_info *obj4 = new DOC_info(*Max);
			Ranked_List.insertAtEnd(obj4);
			Max.del_node(); // delete max
		}

		Ranked_List.print();
	}

	~Search_engine()
	{

	}

};


int main()
{

	string Files[5] = { "Doc1.txt", "Doc2.txt", "Doc3.txt", "Doc4.txt", "Doc5.txt" };
	{
		Search_engine obj;
		obj.Create_Index(Files, 5);
		
		/*obj.print();*/


		string Query_term;


		cout << "============================================\n";
		cout << "=     ___                _                 =\n";
		cout << "=    / __|___  ___  __ _| |___             =\n";
		cout << "=   | (_ / _ \\/ _ \\/ _` |   -_)            =\n";
		cout << "=    \\___\\___/\\___/\\__, |_\\___|            =\n";
		cout << "=                  |___/                   =\n";
		cout << "============================================\n";

		cout << "Enter :  ";
		getline(cin, Query_term);


		cout << "\n\nRanked List : \n\n";
		obj.Search_Document(Query_term);
	}

		

	system("pause");
}






//Stack linear data structure follow particular order operation perform LIFO FILO fundamental 
//stack push equivalent insert pop delete recent element queue list done one end other basic 
//enqueue rear dequeue start front linked consist series nodes adjacent memory node contain link
//successor call next last cell point nullptr large amount input access time prohibitive tree define 
//several way natural recursive collection empty otherwise consists distinguish root zero more nonempty
//connect direct edge implement each besides child number per vary great known advance might infeasible
//make too much waste space solution simple keep 