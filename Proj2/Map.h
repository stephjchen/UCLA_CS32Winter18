//
//  Map.h
//  proj2
//

#ifndef Map_h
#define Map_h

#include <cstdlib>
#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
    Map();
    Map (const Map &oldMap); //copy constructor
    Map &operator= (const Map &oldMap); //assignment operator
    ~Map();
    
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    bool empty() const; //checks if map is empty
    int size() const;
    
    
private:
    
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
        Node* prev;
    };
    
    int m_numNodes; //used for size
    
    Node* head;
    Node* tail;
    
    void addToFront(const KeyType& key, const ValueType& value)
    //algorithm is the same even if list is originally empty
    {
        Node* p = new Node;
        p->m_key = key;
        p->m_value = value;
        p->next = nullptr;
        p->prev = nullptr;
        
        if (head == nullptr) //the list is orignially empty
        {
            head = p;
            tail = p; //the newly added node will be the only node in the list
            m_numNodes++;
        }
        
        else //the list is originally not empty
        {
            head->prev = p; //connect the original head to the newly added node
            p->next = head;
            head = p; //update the head node to newly added node
            m_numNodes++;
        }
        
    }
    
    void addToRear(const KeyType& key, const ValueType& value)
    {
        Node* n = new Node;
        n->m_key = key;
        n->m_value = value;
        
        n->next = nullptr;
        n->prev = tail;
        
        tail->next = n;
        tail = n;
        
        m_numNodes++;
    }
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_h */
