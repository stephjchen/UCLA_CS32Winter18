//
//  testNewMap.cpp
//  New Map
//
//  Created by Stephanie Chen on 1/21/18.
//  Copyright © 2018 Stephanie Chen. All rights reserved.
//

#include "newMap.h"

#include <string>
#include <iostream>
#include <cassert>

using namespace std;

void test()
{
    
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = { "apple", "bear", "cat", "dog", "ethel", "frank" };
    ValueType v  = 4.0;
    
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));
    
    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));
    
    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));
    
    
    /*
     //spec test case 2 (change Keytype to int and Valuetype to std::string)
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
    
    ValueType testValue = 0;
    KeyType testKey = "";
    
    Map m(250); //empty map m
    Map m1(1); //map m1 of max_hold 1
    
    assert(m1.insert("steph", 20));
    assert(!m1.insert("doggo",3));
    m1.swap(m); //swaps m & m1
    assert(m1.size()==0);
    assert(m.get("steph", testValue) && testValue==20);
    assert(m.size()==1);
    assert(!m.insert("hello", 1.5));
    m.erase("steph");
    assert(m.insert("hello", 1.5));
    assert(m.get(0, testKey, testValue));
    assert(testKey == "hello" && testValue==1.5);
    
    m1 = m; //calls assignment operator
    assert(m1.contains("hello"));
    Map m2 = m;
    Map empM(0); //empM can hold maximum of 0 items
    assert (m2.contains("hello") && !empM.insert("lil", -789));
    testKey = "";
    testValue = 0;
    assert(!empM.get(testKey, testValue));
    assert(!empM.get(0, testKey, testValue));
    assert(testKey == "" && testValue == 0);
}

int main()

{
    test();
    cout << "Passed all tests" << endl;
    
}
