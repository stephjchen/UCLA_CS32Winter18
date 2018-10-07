// MyHash.h
//
// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

#ifndef _MY_HASH
#define _MY_HASH

//#include <iostream>
//#include <cassert>

using namespace std;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:

    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
    };

    void insert(const KeyType& key, const ValueType& value);
    bool checkSize();
    unsigned int calculateHash(const KeyType& key) const;

    double m_maxload; //maximum load specified
    int m_size; //number of buckets in hashtable
    int m_numitems; //number of items in hashtable
    Node ** hashptr; //to dynamically allocate array of pointers

};

//Private Member Functions Implementations

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
    //check if resizing is needed before inserting
    checkSize();

    //dynamically allocate a new node
    Node * add = new Node;
    add -> m_key = key;
    add -> m_value = value;
    add -> next = nullptr;

    //calculate bucket number
    unsigned int h = calculateHash(key);

    if (hashptr[h] == nullptr)
    {
        hashptr[h] = add;
        m_numitems++;
    }
    
    //insert new node as first element pointed to in the hash table
    else
    {
        Node * p = hashptr[h];
        add->next = p;
        hashptr[h] = add;
        m_numitems++;
    }
}

template<typename KeyType, typename ValueType>
bool MyHash<KeyType, ValueType>::checkSize()
{
    if ( ( (double)(m_numitems+1)/(double) m_size ) > m_maxload )
    {
        int oldSize = m_size;
        
        m_size *= 2; //double in size first
        
        Node ** oldHash = hashptr;
        hashptr = new Node* [m_size]; //dynamically allocate new hashtable

        for (int i = 0; i < m_size; i++)
            hashptr[i] = nullptr;

        m_numitems = 0;

        for (int i = 0; i < oldSize; i++)
        {
            if (oldHash[i] != nullptr)
            {
                Node * hold = nullptr;
                Node * from = oldHash[i];

                while(from != nullptr)
                {
                    insert(from->m_key, from->m_value); //insert into the new hashtable using data from old hash table
                    hold = from;
                    from = from->next;
                    delete hold; //delete each node
                }
            }
        }
        
        delete [] oldHash; //delete the old dynamically hash table
        return true;
    }
    
    else
        return false;
}


template<typename KeyType, typename ValueType>
unsigned int MyHash<KeyType,ValueType>::calculateHash(const KeyType& key) const
{
    unsigned int hash(const KeyType& key);
    unsigned int h = hash(key);

    return h % m_size;
}

//Public Member Functions Implementations

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    if (maxLoadFactor <= 0)
        m_maxload = 0.5;
    else if (maxLoadFactor > 2)
        m_maxload = 2.0;
    else
        m_maxload = maxLoadFactor;

    m_size = 100;
    m_numitems = 0;

    hashptr = new Node* [m_size]; //Dynamically allocate new hashtable

    for (int i = 0; i < m_size; i++)
        hashptr[i] = nullptr;

}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{

    if (hashptr != nullptr)
    {
        for (int i=0; i < m_size; i++)
        {
            if (hashptr[i]!=nullptr)
            {
                Node* hold = nullptr;
                Node* cur = hashptr[i];

                while (cur != nullptr)
                {
                    hold = cur;
                    cur = cur->next;
                    delete hold;
                    m_numitems--;
                }
            }
        }

        delete [] hashptr;
        hashptr = nullptr;
    }

}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    for (int i=0; i < m_size; i++)
    {
        if (hashptr[i] != nullptr)
        {
            Node* hold = nullptr;
            Node* cur = hashptr[i];
            
            while (cur != nullptr)
            {
                hold = cur;
                cur = cur->next;
                delete hold;
            }
        }
    }
    
    delete [] hashptr;
    
    m_size = 100;
    m_numitems = 0;
    
    hashptr = new Node* [m_size];
    
    for (int i = 0; i < m_size; i++)
        hashptr[i] = nullptr;
}

template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType,ValueType>::find(const KeyType& key) const
{
    unsigned int bucket = calculateHash(key);
    
    Node* temp = hashptr[bucket];

    while (temp != nullptr)
    {

        if (temp -> m_key == key)
        {
            ValueType* value = &(temp->m_value);
                return value;
        }
        
        temp = temp->next;
    }

    return nullptr;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* vp = find(key);

    if (vp != nullptr) {
        *vp = value;
    }
    else {
        insert(key,value);
    }
}


template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_numitems;
}


template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return (double)m_numitems/(double)m_size;
}
#endif


