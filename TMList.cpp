#include<iostream>
using namespace std;
#define bool int
#define TRUE 1
#define True 1
#define true 1
#define FALSE 0
#define False 0
#define false 0
class Iterator
{
private:
Iterator *iterator;
int releaseIteratorAfterIteration;
public:
Iterator()
{
this->releaseIteratorAfterIteration=1;
this->iterator=NULL;
}
Iterator(Iterator *iterator)
{
this->releaseIteratorAfterIteration=1;
this->iterator=iterator;
}
Iterator(const Iterator &other)
{
this->releaseIteratorAfterIteration=1;
this->iterator=other.iterator;
}
virtual ~Iterator()
{
if(this->releaseIteratorAfterIteration==1) delete this->iterator;
}
void setReleaseIteratorAfterIteration(int releaseIteratorAfterIteration)
{
this->releaseIteratorAfterIteration=releaseIteratorAfterIteration;
}
Iterator & operator=(const Iterator &other)
{
this->iterator=other.iterator;
return *this;
}
virtual int hasMoreElement()
{
if(iterator!=NULL) return this->iterator->hasMoreElement();
return 0;
}
virtual int next()
{
if(iterator!=NULL) return this->iterator->next();
return 0;
}
};

class Iterable
{
public :
virtual Iterator getIterator()=0;
};

class TMList:public Iterable
{
public:
virtual void add(int data,int *success)=0;
virtual void insertAt(int index,int data,int *success)=0;
virtual int removeAt(int index,int *success)=0;
virtual void removeAll()=0;
virtual void clearAll()=0;
virtual int getSize() const=0;
virtual void update(int index,int data,int *success)=0;
virtual int get(int index,int *success) const=0;
};

class TMArrayList : public TMList
{
private:
int **ptr;
int capacity;
int size;
char allocationFlag;
bool addRow()
{
if(this->capacity%100==0)
{
int numberOfPointer=this->capacity/10;
int **tmp=new int*[numberOfPointer+10];
if(tmp==NULL) return false;
for(int e=0;e<numberOfPointer;e++) tmp[e]=this->ptr[e];
delete [ ] this->ptr;
this->ptr=tmp;
}
int i=this->capacity/10;
this->ptr[i]=new int [10];
if(this->ptr[i]==NULL) return false;
this->capacity=this->capacity+10;
return true;
}
public:
class TMArrayListIterator : public Iterator
{
int index;
int size;
int **ptr;
public:
TMArrayListIterator(int **ptr,int size)
{
this->index=0;
this->size=size;
this->ptr=ptr;
}
TMArrayListIterator(const TMArrayListIterator &other)
{
this->index=other.index;
}
TMArrayListIterator& operator=(const TMArrayListIterator &other)
{
this->index=other.index;
return *this;
}
int hasMoreElement()
{
return  index<size;
}
int next()
{
if(index==size) return 0;
int rowIndex=index/10;
int columnIndex=index%10;
int data=this->ptr[rowIndex][columnIndex];
index++;
return data;
}
};
Iterator getIterator()
{
TMArrayListIterator *tmArrayListIterator;
tmArrayListIterator=new TMArrayListIterator(this->ptr,this->size);
Iterator k(tmArrayListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}
TMArrayList();
TMArrayList(int bufferSize);
TMArrayList(const TMArrayList &other);
TMArrayList(const TMList &other);
TMArrayList& operator=(const TMArrayList &other);
TMArrayList& operator=(const TMList &other);
void operator +=(const TMArrayList &other);
void operator +=(const TMList &other);
TMArrayList operator +(const TMArrayList &other);
TMArrayList operator +(const TMList &other);
virtual ~TMArrayList();
void add(int data,int *success);
void insertAt(int index,int data,int *success);
int removeAt(int index,int *success);
void removeAll();
void clearAll();
int getSize() const;
void update(int index,int data,int *success);
int get(int index,int *success) const;
};

TMArrayList::TMArrayList()
{
this->ptr=new int*[10];
this->ptr[0]=new int [10];
this->capacity=10;
this->size=0;
this->allocationFlag=0;
}

TMArrayList::TMArrayList(int bufferSize)
{
this->allocationFlag=0;
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int [10];
this->capacity=10;
this->size=0;
}
else
{
int rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
int numberOfPointer=(rows+(10-(rows%10)));
this->ptr=new int* [numberOfPointer];
for(int e=0;e<rows;e++)
{
this->ptr[e]=new int[10];
}
this->capacity=rows*10;
this->size=0;
}
}

TMArrayList::TMArrayList(const TMArrayList &other)
{
this->allocationFlag=0;
int bufferSize=other.size;
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int [10];
this->capacity=10;
this->size=0;
}
else
{
int rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
int numberOfPointer=(rows+(10-(rows%10)));
this->ptr=new int* [numberOfPointer];
for(int e=0;e<rows;e++)
{
this->ptr[e]=new int[10];
}
this->capacity=rows*10;
this->size=0;
}
int succ;
for(int e=0;e<other.size;e++)
{
this->add(other.get(e,&succ),&succ);
}
}

TMArrayList::TMArrayList(const TMList &other)
{
this->ptr=new int*[10];
this->ptr[0]=new int [10];
this->capacity=10;
this->size=0;
this->allocationFlag=0;
int succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
}

TMArrayList::~TMArrayList()
{
if(this->allocationFlag==0)
{
int rows=this->capacity/10;
for(int j=0;j<rows;j++)
{
delete [ ] this->ptr[j];
}
delete [ ] this->ptr;
}
}


TMArrayList& TMArrayList::operator=(const TMArrayList &other)
{
if(other.allocationFlag==0)
{
this->size=0;
int succ;
for(int e=0;e<other.size;e++)
{
this->add(other.get(e,&succ),&succ);
}
}
else
{
int rows=this->capacity/10;
for(int j=0;j<rows;j++)
{
delete [ ] this->ptr[j];
}
delete [ ] this->ptr;
this->ptr=other.ptr;
this->capacity=other.capacity;
this->size=other.size;
}
return *this;
}

TMArrayList& TMArrayList::operator=(const TMList &other)
{
this->size=0;
int e,succ;
for( e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
return *this;
}


void TMArrayList::operator +=(const TMArrayList &other)
{
int succ;
for(int e=0;e<other.size;e++)
{
this->add(other.get(e,&succ),&succ);
}
}

void TMArrayList::operator +=(const TMList &other)
{
int succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
}

TMArrayList TMArrayList::operator +(const TMArrayList &other)
{
TMArrayList k;
int succ;
for(int e=0;e<this->size;e++) k.add(this->get(e,&succ),&succ);
for(int e=0;e<other.size;e++) k.add(other.get(e,&succ),&succ);
k.allocationFlag=1;
return k;
}

TMArrayList TMArrayList::operator +(const TMList &other)
{
TMArrayList k;
int succ;
for(int e=0;e<this->size;e++) k.add(this->get(e,&succ),&succ);
for(int e=0;e<other.getSize();e++) k.add(other.get(e,&succ),&succ);
return k;
}

void TMArrayList::add(int data,int *success)
{
if(success) *success=false;
if(this->capacity==this->size)
{
if(!addRow()) return;
}
int rowIndex=this->size/10;
int columnIndex=this->size%10;
this->ptr[rowIndex][columnIndex]=data;
this->size++;
if(success) *success=true;
}

void TMArrayList::insertAt(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return ;
if(index==this->size)
{
this->add(index,success);
return ;
}
bool succ;
int k=this->get(this->size-1,&succ);
this->add(k,&succ);
if(succ==false) return ;
int j=this->size-3;
while(j>=index)
{
this->update(j+1,this->get(j,&succ),&succ);
j--;
}
this->update(index,data,&succ);
if(success) *success=true;
}

int TMArrayList::removeAt(int index,int *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
bool succ;
int data=this->get(index,&succ);
int j=index;
int ep=this->size-1;
while(j<=ep)
{
this->update(j,this->get(j+1,&succ),&succ);
j++;
}
this->size--;
if(success) *success=true;
return data;
}

void TMArrayList::removeAll()
{
this->size=0;
}
void TMArrayList::clearAll()
{
this->size=0;
}
int TMArrayList::getSize() const
{
return this->size;
}

void TMArrayList::update(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return ;
int rowIndex=index/10;
int columnIndex=index%10;
this->ptr[rowIndex][columnIndex]=data;
if(success) *success=true;
}

int TMArrayList::get(int index,int *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int rowIndex=index/10;
int columnIndex=index%10;
if(success) *success=true;
return ptr[rowIndex][columnIndex];
}


/*
//TestCase 3 TMArrayList
int main()
{
TMArrayList list1;
bool k;
for(int e=0;e<=100;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
cout<<"Content of list 1"<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMArrayList list2;
for(int e=200;e<=215;e++)
{
list2.add(e,&k);
}
cout<<endl;
cout<<"Size is "<<list2.getSize()<<endl;
cout<<"Content of list 2"<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
list2+=list1;
cout<<"Content of list 2 after += "<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
cout<<"Size is "<<list2.getSize()<<endl;
TMArrayList list3;
list3=list1+list2;
cout<<"Content of list 3"<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;
cout<<"Size is "<<list3.getSize()<<endl;
return 0;
}
*/


/*
//Testcase1 ArrayList
int main()
{
TMArrayList list1;
bool k;
for(int e=0;e<=100;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;

TMArrayList list2(list1);
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
}
*/

/*
//TestCase2 ArrayList
int main()
{
int k;
TMArrayList list1(6000);
for(int x=100;x<=123;x++)
{
list1.add(x,&k);
}
cout<<"Size is : "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<" ";
}
cout<<endl;

list1.update(3,2020,&k);
if(k) cout<<"Data updated at index 3"<<endl;
else cout<<"Unable to update"<<endl;

list1.insertAt(8,1060,&k);
if(k) cout<<"Data insert at index 8"<<endl;
else cout<<"Unable to insert data at index 8"<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<" ";
}
cout<<endl;

int u=list1.removeAt(3,&k);
if(k) cout<<u<<"removed at index 3"<<endl;
else cout<<"Unable to  remove  data at index"<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<" ";
}
cout<<endl;

return 0;
}
*/

//TMForwardList implementation starts

class TMForwardList : public TMList
{
class TMNode 
{
public:
int data;
TMNode *next;
TMNode()
{
this->next=NULL;
}
};
private:
TMNode *start,*end;
int size;
char allocationFlag;
public:
class TMForwardListIterator : public Iterator
{
TMNode *ptr;
public:
TMForwardListIterator()
{
this->ptr=NULL;
}
TMForwardListIterator(TMNode *ptr)
{
this->ptr=ptr;
}
TMForwardListIterator(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
}
TMForwardListIterator& operator=(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
return *this;
}
int hasMoreElement()
{
return this->ptr!=NULL;
}
int next()
{
if(this->ptr==NULL) return 0;
int data=this->ptr->data;
this->ptr=this->ptr->next;
return data;
}
};
public:
Iterator getIterator()
{
TMForwardListIterator *tmForwardListIterator;
tmForwardListIterator=new TMForwardListIterator(this->start);
Iterator k(tmForwardListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}
TMForwardList();
TMForwardList(int bufferSize);
TMForwardList(const TMForwardList &other);
TMForwardList(const TMList &other);
TMForwardList& operator=(const TMForwardList &other);
TMForwardList& operator=(const TMList &other);
void operator +=(const TMForwardList &other);
void operator +=(const TMList &other);
TMForwardList operator +(const TMForwardList &other);
TMForwardList operator +(const TMList &other);
virtual ~TMForwardList();
void add(int data,int *success);
void insertAt(int index,int data,int *success);
int removeAt(int index,int *success);
void removeAll();
void clearAll();
int getSize() const;
void update(int index,int data,int *success);
int get(int index,int *success) const;
};

TMForwardList::TMForwardList()
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMForwardList::TMForwardList(int bufferSize)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMForwardList::TMForwardList(const TMForwardList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}

TMForwardList::TMForwardList(const TMList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}

TMForwardList::~TMForwardList()
{
this->clearAll();
}

TMForwardList& TMForwardList::operator=(const TMForwardList &other)
{
this->clearAll();
if(other.allocationFlag==0)
{
this->size=other.size;
this->start=other.start;
this->end=other.end;
}
else
{
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}

TMForwardList& TMForwardList::operator=(const TMList &other)
{
{
this->clearAll();
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}

void TMForwardList::operator +=(const TMForwardList &other)
{
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}


void TMForwardList::operator +=(const TMList &other)
{
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}

TMForwardList TMForwardList::operator +(const TMForwardList &other)
{
TMForwardList k;
k+=(*this);
k+=other;
return k;
}

TMForwardList TMForwardList::operator +(const TMList &other)
{
TMForwardList k;
k+=(*this);
k+=other;
return k;
}

void TMForwardList::add(int data,int *success)
{
if(success) *success=false;
TMNode *t;
t=new TMNode ;
if(t==NULL) return ;
t->data=data;
if(this->start==NULL)
{
this->start=t;
this->end=t;
}
else
{
this->end->next=t;
this->end=t;
}
this->size++;
if(success) *success=true;
}

void TMForwardList::insertAt(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return ;
if(index==this->size)
{
this->add(data,success);
return ;
}
TMNode *gg;
gg=new TMNode;
if(gg==NULL) return ;
gg->data=data;
TMNode *t,*j;
int x;
t=this->start;
for(x=0;x<index;x++) 
{
j=t;
t=t->next;
}
if(t==this->start)
{
gg->next=this->start;
this->start=gg;
}
else
{
gg->next=t;
j->next=gg;
}
this->size++;
if(success) *success=true;
}

int TMForwardList::removeAt(int index,int *success)
{
int data=0;
if(success) *success=false;
if(index<0 || index>=this->size) return data;
TMNode *t,*j;
int x;
t=this->start;
for(x=0;x<index;x++)
{
j=t;
t=t->next;
}
if(t==this->start && t==this->end) //only one node
{
this->start=this->end=NULL;
}
else if(t==this->start)//many but start
{
this->start=this->start->next;
}
else if(t==this->end)//many but end
{
j->next=NULL;
this->end=j;
}
else
{
j->next=t->next;
}
data=t->data;
delete t;
this->size--;
if(success) *success=true;
return data;
}

void TMForwardList::removeAll()
{
clearAll();
}
void TMForwardList::clearAll()
{
TMNode *t;
while(this->start!=NULL)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->end=NULL;
this->size=0;
}

int TMForwardList::getSize() const
{
return this->size;
}

void TMForwardList::update(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return ;
int x;
TMNode *t;
t=this->start;
for(x=0;x<index;x++) t=t->next;
t->data=data;
if(success) *success=true;
}

int TMForwardList::get(int index,int *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int x;
TMNode *t;
t=this->start;
for(x=0;x<index;x++) t=t->next;
if(success) *success=true;
return t->data;
}



int main()
{
cout<<"ArrayList Implemention"<<endl;
TMArrayList list1;
int k;
list1.add(100,&k);
list1.add(243,&k);
list1.add(2000,&k);
list1.add(20,&k);
list1.add(30,&k);
list1.add(4040,&k);
list1.add(502,&k);
list1.add(70,&k);
Iterator iterator1=list1.getIterator();
Iterator iterator2=list1.getIterator();
Iterator iterator3=list1.getIterator();
cout<<iterator1.next()<<endl;
cout<<iterator2.next()<<endl;
cout<<iterator3.next()<<endl;
cout<<"**********************"<<endl;
cout<<iterator1.next()<<endl;
cout<<iterator2.next()<<endl;
cout<<iterator3.next()<<endl;
cout<<"ForwardList Implemention"<<endl;
TMForwardList list2;
list2.add(100,&k);
list2.add(243,&k);
list2.add(2000,&k);
list2.add(20,&k);
list2.add(30,&k);
list2.add(4040,&k);
list2.add(502,&k);
list2.add(70,&k);
iterator1=list2.getIterator();
iterator2=list2.getIterator();
iterator3=list2.getIterator();
cout<<iterator1.next()<<endl;
cout<<iterator2.next()<<endl;
cout<<iterator3.next()<<endl;
cout<<"**********************"<<endl;
cout<<iterator1.next()<<endl;
cout<<iterator2.next()<<endl;
cout<<iterator3.next()<<endl;
/*
while(iterator.hasMoreElement())
{
cout<<iterator.next()<<endl;
}
*/
return 0;
}




/*
int main()
{
TMForwardList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;

TMArrayList list2(list1);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
list1+=list2;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;

return 0;
}
*/


/*
//TestCase 3
int main()
{
TMForwardList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMForwardList list2;
list2.add(2020,&k);
list2.add(5050,&k);
list2.add(1000,&k);
list1+=list2;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMForwardList list3;
list3=list2;
cout<<"Size is "<<list3.getSize()<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;
list3=list2+list1;
cout<<"Size is "<<list3.getSize()<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;
return 0;
}
*/


/*
//Testcase2 TMForwardList
int main()
{
TMForwardList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMForwardList list2(list1);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
list2.add(2020,&k);
list2.insertAt(5,5050,&k);
list2.removeAt(0,&k);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
return 0;
}
*/


/*
//TestCase 1 TMForwardList
int main()
{
TMForwardList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.update(25,100,&k);
if(k) cout<<" index 25 updated"<<endl;
else cout<<"Unable to update"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.update(2,1000,&k);
if(k) cout<<"index 2 updated"<<endl;
else cout<<"Unable to update"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.insertAt(24,2000,&k);
if(k) cout<<"2000 inserted at index 24"<<endl;
else cout<<"Unable to insert"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.insertAt(0,3000,&k);
if(k) cout<<"3000 inserted at index 0"<<endl;
else cout<<"Unable to insert"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(0,&k);
if(k) cout<<"data at index 0 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(24,&k);
if(k) cout<<"data at index 24 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(2,&k);
if(k) cout<<"data at index 2 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAll();
cout<<"Size is "<<list1.getSize()<<endl;
return 0;
}
*/



//done
//TMToWayList implementation starts
/*
class TMToWayList : public TMList
{
class TMNode 
{
public:
int data;
TMNode *prev;
TMNode *next;
TMNode()
{
this->prev=NULL;
this->next=NULL;
}
};
private:
TMNode *start,*end;
int size;
char allocationFlag;
public:
TMToWayList();
TMToWayList(int bufferSize);
TMToWayList(const TMToWayList &other);
TMToWayList(const TMList &other);
TMToWayList& operator=(const TMToWayList &other);
TMToWayList& operator=(const TMList &other);
void operator +=(const TMToWayList &other);
void operator +=(const TMList &other);
TMToWayList operator +(const TMToWayList &other);
TMToWayList operator +(const TMList &other);
virtual ~TMToWayList();
void add(int data,int *success);
void insertAt(int index,int data,int *success);
int removeAt(int index,int *success);
void removeAll();
void clearAll();
int getSize() const;
void update(int index,int data,int *success);
int get(int index,int *success) const;
};

TMToWayList::TMToWayList()
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMToWayList::TMToWayList(int bufferSize)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMToWayList::TMToWayList(const TMToWayList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
int k;
for(int e=0;e<other.size;e++) this->add(other.get(e,&k),&k);
}

TMToWayList::TMToWayList(const TMList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}

TMToWayList::~TMToWayList()
{
this->clearAll();
}

TMToWayList& TMToWayList::operator=(const TMToWayList &other)
{
this->clearAll();
if(other.allocationFlag==0)
{
this->size=other.size;
this->start=other.start;
this->end=other.end;
}
else
{
int succ;
for(int e=0;e<other.size;e++) this->add(other.get(e,&succ),&succ);
}
return *this;
}

TMToWayList& TMToWayList::operator=(const TMList &other)
{
this->clearAll();
int succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
return *this;
}

void TMToWayList::operator +=(const TMToWayList &other)
{
int k;
for(int e=0;e<other.size;e++) this->add(other.get(e,&k),&k);
}


void TMToWayList::operator +=(const TMList &other)
{
int k;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&k),&k);
}

TMToWayList TMToWayList::operator +(const TMToWayList &other)
{
TMToWayList k;
k+=(*this);
k+=other;
return k;
}

TMToWayList TMToWayList::operator +(const TMList &other)
{
TMToWayList k;
k+=(*this);
k+=other;
return k;
}

void TMToWayList::add(int data,int *success)
{
if(success) *success=false;
TMNode *t;
t=new TMNode ;
if(t==NULL) return ;
t->data=data;
if(this->start==NULL)
{
this->start=t;
this->end=t;
}
else
{
this->end->next=t;
t->prev=this->end;
this->end=t;
}
this->size++;
if(success) *success=true;
}

void TMToWayList::insertAt(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return ;
if(index==this->size)
{
this->add(data,success);
return ;
}
TMNode *gg;
gg=new TMNode;
if(gg==NULL) return ;
gg->data=data;
TMNode *t;
int x;
t=this->start;
for(x=0;x<index;x++) 
{
t=t->next;
}
if(t==this->start)
{
gg->next=this->start;
this->start->prev=gg;
this->start=gg;
}
else
{
gg->next=t;
gg->prev=t->prev;
t->prev->next=gg;
t->prev=gg;
}
this->size++;
if(success) *success=true;
}

int TMToWayList::removeAt(int index,int *success)
{
int data=0;
if(success) *success=false;
if(index<0 || index>=this->size) return data;
TMNode *t;
int x;
t=this->start;
for(x=0;x<index;x++)
{
t=t->next;
}
if(t==this->start && t==this->end) //only one node
{
this->start=this->end=NULL;
}
else if(t==this->start)//many but start
{
this->start=this->start->next;
this->start->prev=NULL;
}
else if(t==this->end)//many but end
{
this->end=this->end->prev;
this->end=NULL;
}
else
{
t->prev->next=t->next;
t->next->prev=t->prev;
}
data=t->data;
delete t;
this->size--;
if(success) *success=true;
return data;
}

void TMToWayList::removeAll()
{
this->clearAll();
}
void TMToWayList::clearAll()
{
TMNode *t;
while(this->start!=NULL)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->end=NULL;
this->size=0;
}

int TMToWayList::getSize() const
{
return this->size;
}

void TMToWayList::update(int index,int data,int *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return ;
int x;
TMNode *t;
t=this->start;
for(x=0;x<index;x++) t=t->next;
t->data=data;
if(success) *success=true;
}

int TMToWayList::get(int index,int *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int x;
TMNode *t;
t=this->start;
for(x=0;x<index;x++) t=t->next;
if(success) *success=true;
return t->data;
}





int main()
{
TMToWayList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;

TMArrayList list2(list1);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
list1+=list2;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;

TMForwardList list3(list2);
cout<<"Size is "<<list3.getSize()<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;
list2+=list3;
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;

return 0;
}





//TestCase 3 TMToWayList 
int main()
{
cout<<"TMToWayList"<<endl;

TMToWayList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
cout<<"Content of list1"<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMToWayList list2;
list2.add(2020,&k);
list2.add(5050,&k);
list2.add(1000,&k);

cout<<"Size is "<<list2.getSize()<<endl;
cout<<"Content of list2"<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;

list1+=list2;
cout<<"Size is "<<list1.getSize()<<endl;
cout<<"Content of list1 after += "<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;

TMToWayList list3;
list3=list2;
cout<<"Size is "<<list3.getSize()<<endl;
cout<<"Content of list3 "<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;

list3=list2+list1;
cout<<"Size is "<<list3.getSize()<<endl;
cout<<"Content of list3 after list2+list1 "<<endl;
for(int e=0;e<list3.getSize();e++)
{
cout<<list3.get(e,&k)<<"  ";
}
cout<<endl;
return 0;
}




//Testcase2 TMToWayList 
int main()
{
TMToWayList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
TMToWayList list2(list1);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
list2.add(2020,&k);
list2.insertAt(5,5050,&k);
list2.removeAt(0,&k);
cout<<"Size is "<<list2.getSize()<<endl;
for(int e=0;e<list2.getSize();e++)
{
cout<<list2.get(e,&k)<<"  ";
}
cout<<endl;
return 0;
}




//TestCase 1 TMToWayList
int main()
{
TMToWayList list1;
bool k;
for(int e=100;e<=123;e++)
{
list1.add(e,&k);
}
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.update(25,100,&k);
if(k) cout<<" index 25 updated"<<endl;
else cout<<"Unable to update"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.update(2,1000,&k);
if(k) cout<<"index 2 updated"<<endl;
else cout<<"Unable to update"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.insertAt(24,2000,&k);
if(k) cout<<"2000 inserted at index 24"<<endl;
else cout<<"Unable to insert"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.insertAt(0,3000,&k);
if(k) cout<<"3000 inserted at index 0"<<endl;
else cout<<"Unable to insert"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(0,&k);
if(k) cout<<"data at index 0 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(24,&k);
if(k) cout<<"data at index 24 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAt(2,&k);
if(k) cout<<"data at index 2 removed"<<endl;
else cout<<"Unable to removed"<<endl;
cout<<"Size is "<<list1.getSize()<<endl;
for(int e=0;e<list1.getSize();e++)
{
cout<<list1.get(e,&k)<<"  ";
}
cout<<endl;
list1.removeAll();
cout<<"Size is "<<list1.getSize()<<endl;
return 0;
}
//done
*/
