//
//  Map.cpp
//  proj2
//

#include "Map.h"

bool combine(const Map& m1, const Map& m2, Map& result)
{
    KeyType k1, k2;
    ValueType v1, v2;
    Map tempMap; //create an empty map
    
    if (&m1 == &m2)
    {
        tempMap = m1; //calls assignment operator make empty map a deep copy of m1
        result = tempMap; //set result to tempMap, which have all data of m1
        return true;
    }
    
    else
    {
        bool diffval = false;
        
        for (int i =0; i < m1.size(); i++)
        {
            m1.get(i,k1,v1);
            
            if (!m2.contains(k1)) //this will loop through elements of m2
                tempMap.insert(k1,v1); //insert key/value pairs ONLY in m1
            else
            {   m2.get(k1, v2);
                if (v1 == v2)
                    tempMap.insert(k1,v1); //insert key/value pairs in BOTH m1 and m2
                else
                    diffval = true;
            }
        }
        
        for (int i = 0; i < m2.size(); i++)
        {
            m2.get(i,k2,v2);
            if (!m1.contains(k2))
                tempMap.insert(k2,v2); //insert key value pairs ONLY in m2
        }
        
        result = tempMap;
        
        if (diffval == true)
            return false;
        
        return true;
        
    }
    
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType k2;
    ValueType v2;
    
    Map temp;
    
    if (&m1 == &m2)
    {
        result = temp; //set result as an empty map
    }
    
    else {
        
        temp = m1; //calls copy c'tor to create deep copy of m1
        for (int j=0; j < m2.size(); j++)
        {
            m2.get(j,k2, v2);
            
            if (temp.contains(k2))
                temp.erase(k2);
        }
        
        result = temp;
    }
}

bool Map::empty() const //checks if map is empty
{
    if (size() == 0)
        return true;
    return false;
}

int Map::size() const
{
    return m_numNodes;
}


Map::Map()
//creates an empty map
{
    head = nullptr;
    tail = nullptr;
    m_numNodes = 0;
    
}

Map::Map (const Map &oldMap) //copy constructor
{
    Node* temp = oldMap.head;
    
    if (oldMap.empty()) //indicates oldMap is empty
    {
        head = nullptr;
        tail = nullptr;
        m_numNodes = 0;
    }//creates a deep copy of an empty map
    
    //creates a deep copy of the first node if oldMap isn't empty
    else
    {
        m_numNodes = 0;
        
        //construct the first node
        Node* n = new Node;
        
        n->m_key = temp->m_key;
        n->m_value = temp->m_value;
        n->prev = nullptr;
        n->next = nullptr;
        head = n;
        tail = n;
        
        m_numNodes++;
        
        while ((temp = temp->next) != nullptr)
            this->insert(temp->m_key, temp->m_value);
        
    }
    
}

Map& Map::operator= (const Map &oldMap) //assignment operator
{
    if (&oldMap == this)
        return *this;
    
    //destructs the current map first
    Node* p = head;
    
    while (p != nullptr) //traverse and process each node in the map
    {
        Node* n = p->next;
        delete p;
        p = n; //update target node being deleted
    }
    
    head = nullptr;
    tail = nullptr;
    
    
    Node* temp = oldMap.head;
    
    //creates an empty map if the oldMap is empty
    if (oldMap.empty())
    {
        head = nullptr;
        tail = nullptr;
        m_numNodes = 0;
    }
    
    else
    {
        m_numNodes = 0;
        
        //creates a deep copy of the first node if oldMap isn't empty
        Node* n = new Node;
        
        n->m_key = temp->m_key;
        n->m_value = temp->m_value;
        n->prev = nullptr;
        n->next = nullptr;
        head = n;
        tail = n;
        
        m_numNodes ++;
        
        while ((temp = temp->next) != nullptr) //creates subsequent nodes if there are any
            this->insert(temp->m_key, temp->m_value);
        
    }
    
    return *this;
}

Map::~Map()
//same algorithm as that of simply linked list
{
    Node* p = head;
    
    while (p != nullptr) //traverse and process each node in the map
    {
        Node* n = p->next;
        delete p;
        m_numNodes --;
        p = n; //update target node being deleted
    }
    
    head = nullptr;
    tail = nullptr;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (empty())
    {
        addToFront(key, value);
        return true;
    }
    
    else if (!contains(key))
    {
        addToRear(key, value);
        return true;
    }
    
    return false; //if map is not empty, and key is already in the map
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    if (empty()==true) //map is empty, so key is not in the map
        return false;
    
    if (contains(key)==false) //key is not in the map
        return false;
    
    Node* p = head;
    
    while(key != p->m_key && p != nullptr) //traverses through and processes each node
        p = p->next;
    
    if (p == nullptr) //key is not found in the map
        return false;
    
    p->m_value = value;
    
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (contains(key)!=true)
    {
        insert(key,value);
        return true;
    }
    
    update(key,value);
    return true;
}

bool Map::erase(const KeyType& key)
{
    if (empty()==true) //map is empty
        return false;
    
    if (contains(key)==false) //target key is not in map
        return false;
    
    if (key == head->m_key) //target node is the first node in map
    {
        //check for only one node in map
        
        Node* kill = head;
        
        if (head->next == nullptr && head->prev ==nullptr) //special case for only one node in map
        {
            head = nullptr;
            tail = nullptr;
            delete kill;
            m_numNodes --;
        }
        
        else
        {
            kill->next->prev = nullptr;
            head = kill->next;
            delete kill; //deletes the node kill points to
            m_numNodes --;

        }
    }
    
    else //target node is in middle or end of map
    {
        Node* p = head; //used to traverse through nodes in map
        
        while (key != p->m_key && p != nullptr) //traverse through map without dereferencing the nullptr
            p = p->next;
        
        if (p == nullptr) //did not find target node in map
            return false;
        
        //found target node
        if (p != tail) //target node is in middle of the map
        {
            Node* kill = p;
            kill->next->prev = kill->prev;
            kill->prev->next = kill->next;
            delete kill;
            m_numNodes --;
        }
        
        else //target node is the last node
        {
            
            Node* kill = p;
            kill->prev->next = nullptr;
            tail = kill->prev;
            delete kill;
            m_numNodes --;
        }
        
        
    }
    
    return true;
}

bool Map::contains(const KeyType& key) const
//same algorithm as for simply linked list, will process the last node
{
    Node* p = head;
    
    while (p != nullptr)
    {
        if (key == p->m_key)
            return true;
        p = p->next;
    }
    
    return false;
}


bool Map::get(const KeyType& key, ValueType& value) const
{
    Node * p = head;
    
    while (p != nullptr)
    {
        if (key == p->m_key)
        {
            value = p->m_value;
            return true;
        }
        
        p = p->next;
    }
    
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= size())
        return false;
    
    Node * p = head;
    
    for (int j = 1 ; j <= i ; j++)
        p = p->next;
    
    if (p!= nullptr)
    {
        key = p->m_key;
        value = p->m_value;
        return true;
    }
    
    return false;
}

void Map::swap(Map& other)
{
    
    Node* tempH;
    Node* tempT;
    int tempnumNodes = 0;
    
    tempnumNodes = m_numNodes;
    m_numNodes = other.m_numNodes;
    other.m_numNodes = tempnumNodes;
    
    tempH = head;
    head = other.head;
    other.head = tempH;
    
    tempT = tail;
    tail = other.tail;
    other.tail = tempT;
}


