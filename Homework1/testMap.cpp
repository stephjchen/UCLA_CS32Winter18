//
//  testMap.cpp
//  Homework1
//
//  Created by Stephanie Chen on 1/20/18.
//  Copyright © 2018 Stephanie Chen. All rights reserved.
//


#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    
    //empty string
    Map gpas;
    gpas.insert("Fred", 2.956);
    assert(!gpas.contains(""));
    gpas.insert("Ethel", 3.538);
    gpas.insert("", 4.000);
    gpas.insert("Lucy", 2.956);
    assert(gpas.contains(""));
    gpas.erase("Fred");
    assert(gpas.size() == 3  &&  gpas.contains("Lucy")  &&  gpas.contains("Ethel")  &&
           gpas.contains(""));
    
    //testing swap
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  &&
           m2.size() == 1  &&  m2.contains("Fred"));
    
    //testing get with 3 parameters 
    Map mm;  // maps strings to doubles
    assert(mm.empty());
    ValueType v1 = -1234.5;
    assert( !mm.get("abc", v1)  &&  v1 == -1234.5); // v1 unchanged by get failure
    mm.insert("xyz", 9876.5);
    assert(mm.size() == 1);
    KeyType k = "hello";
    assert(mm.get(0, k, v1)  &&  k == "xyz"  &&  v1 == 9876.5);
    
    
    /*
     //test for switching KeyType and ValueType (from spec)
    Map m;
    assert(m.insert(10, "diez"));
    assert(m.insert(20, "veinte"));
    assert(m.size() == 2);
    ValueType v = "cuarenta y dos";
    assert(!m.get(30, v)  &&  v == "cuarenta y dos");
    assert(m.get(10, v)  &&  v == "diez");
    v = "cuarenta y dos";
    KeyType x = 30;
    assert(m.get(0, x, v)  &&
           ((x == 10  &&  v == "diez")  ||  (x == 20  &&  v == "veinte")));
    KeyType x2 = 40;
    assert(m.get(1, x2, v)  &&
           ((x2 == 10  &&  v == "diez")  ||  (x2 == 20  &&  v == "veinte"))  &&
           x != x2);
     
     Map m0;
     assert(m0.empty());
     ValueType v0 = DUMMY_VALUE;
     assert( !m0.get(NO_SUCH_KEY, v)  &&  v0 == DUMMY_VALUE); // v unchanged by get failure
     m0.insert(KEY1, VALUE1);
     assert(m0.size() == 1);
     KeyType k0 = DUMMY_KEY;
     assert(m0.get(0, k0, v0)  &&  k0 == KEY1  &&  v0 == VALUE1);
     */
    
    
    KeyType testKey = "";
    ValueType testValue = 0;
    
    Map someMap;
    assert(someMap.empty());
    
    KeyType keys[5] = {"a", "b", "c", "d", "e"};
    for (int i=0; i<4; i++)
    {
        assert(someMap.insert(keys[i], 0)); //insert 4 keys a, b, c, d
    }
    
    assert(someMap.insert("fifth", 100));
    assert(someMap.insertOrUpdate("fifth",50));
    assert(someMap.get("fifth", testValue));
    assert(testValue == 50);
    
    for (int i=0; i<4; i++)
    {
        assert(someMap.erase(keys[i]));
    }
    assert(someMap.size()==1);
    assert(someMap.erase("fifth"));
    assert(someMap.empty()); //someMap is now empty
    
    testKey = "";
    testValue = 0;
    
    for (int i=0; i < mm.size(); i++)
    {
        assert(mm.get(i, testKey, testValue));
        assert(someMap.insertOrUpdate(testKey, testValue));
    } //mm and someMap should become the same
    
    mm.swap(someMap);
    v1 = -1234.5;
    assert( !mm.get("abc", v1)  &&  v1 == -1234.5); // v1 unchanged by get failure
    mm.insert("xyz", 9876.5);
    assert(mm.size() == 1);
    k = "hello";
    assert(mm.get(0, k, v1)  &&  k == "xyz"  &&  v1 == 9876.5);
    
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}

