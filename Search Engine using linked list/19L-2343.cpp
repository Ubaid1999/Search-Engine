#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include <sstream>

using namespace std;

int Tol_words = 0; //global variable to calculate number of word in Index


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
			cout << *(prn) ;
		}
		cout << endl <<endl ;
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
			save->next->prev = head;
			head->next = save->next;

			delete save;
			size--;
		}
	}

	void DeleteAtEnd()
	{
		node* del_end = tail->prev;

		del_end->prev->next = tail;
		tail->prev = del_end->prev;

		delete del_end;
		size--;
	}

	bool SearchTerm(string term , Iterator &itr)    // a  function for term info that checks if term already exist in index or not 
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
			if (search->data->Search_Doc(docs)== true)
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
			DeleteAtStart();
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

	bool SearchDoc(string id , List<DOC_info>::Iterator &temp)
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

class Search_engine
{
private:

	List<Term_info> index;   // Index as a List 
	int array_size;
	int max_size;

public:
	Search_engine()
	{
		this->array_size = 0;
		this->max_size = 0;			//since we are using List instead of an array  
	}

	void Create_Index(string arr[] , int n)  
	{
		string file;		// to store file name
		ifstream infile;
		string word;       //to store a seperate word 

		for (int j = 0; j < n; j++) 
		{
			file = arr[j];
			infile.open(file);
			while (!infile.eof())  
			{
				getline(infile, word, ' ');					 // opening of doc and storing word by word
			
				List<Term_info>::Iterator temp;
				if (index.SearchTerm(word,temp) == false)		 //word not found 
				{
					Term_info* obj= new Term_info(word);	
					index.insertAtEnd(obj);	
					Tol_words++;
				}
			}

			infile.close();
		}

	}

	void print()
	{
		index.print();
	}

	void Add_Doc_to_Index(string filename)
	{
		ifstream infile;
		string word;
		
		infile.open(filename);
		while (!infile.eof())
		{
			List<Term_info>::Iterator ind;

			getline(infile, word, ' ');
			if (index.SearchTerm(word,ind) == false)		 //word not found 
			{
				Term_info* obj = new Term_info(word);
				DOC_info *objr = new DOC_info(filename, 1);
				obj->l1.insertAtEnd(objr);
				index.insertAtEnd(obj);
			}
			else
			{
				List<DOC_info>::Iterator index = (*ind).l1.begin();
				if ((*ind).SearchDoc(filename,index) == true)  // if Doc file opened already exist in the list 
				{
					(*index).inc_freq();
				}
				else
				{
					DOC_info *objr2 = new DOC_info(filename, 1);
					(*ind).l1.insertAtEnd(objr2);
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
			List<Term_info>::Iterator ind;
			if (index.SearchTerm(terms, ind) == false)
			{
				cout << terms << " does not exist in document \n";
			}
			else
			{

				List<DOC_info>::Iterator itr = (*ind).l1.begin();

				List<DOC_info>::Iterator index;

				while (itr != (*ind).l1.end())
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
						if ((*Max).Doc_id > (*Rank).Doc_id)				//Rule 3 : Alphabetically more bigger term (for strings terminology Doc1 is less than Doc2 so we work opposite)
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
		for (int i = 0; i < 2; i++)
		{
			obj.Add_Doc_to_Index(Files[i]);
		}
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