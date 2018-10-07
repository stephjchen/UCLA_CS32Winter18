//
//  testCarMap.cpp
//  Homework1
//
//

#include "Map.h"
#include "CarMap.h"

#include <cassert>
#include <iostream>

using namespace std;


void test()
{
    double negGas = -9;
    string cars[5] = {"rv", "rawr", "dblock", "geneblock", "A"};
    
    CarMap carMap1;
    assert(carMap1.fleetSize() == 0);
    assert(carMap1.addCar("vroom"));
    assert(carMap1.fleetSize() == 1);
    
    for (int i=0; i<5; i++)
    {
        carMap1.addCar(cars[i]);
    }
    
    assert(carMap1.gas("vroom")==0);
    
    for (int j=0; j<5; j++)
    {
        assert(carMap1.gas(cars[j])==0);
    }
    assert(carMap1.addGas("vroom", 1000));
    assert(!carMap1.useGas(cars[2],negGas) && !carMap1.addGas("vroom", negGas));
    assert(!carMap1.addCar("vroom"));
    carMap1.print();
    
    CarMap carMap2;
    assert(carMap2.addCar("vroomm"));
    assert(!carMap2.addCar("vroomm"));
    assert(carMap2.addCar("ba booh"));
    assert(carMap2.addCar("be booh"));
    assert(carMap2.fleetSize() == 3);
    assert(!carMap2.useGas("be booh", 9) && carMap2.addGas("be booh", 9));
    carMap2.print();
    
    
    CarMap carMap3 = carMap2; //calls auto-generated copy c'tor
    assert(!carMap3.addCar("vroomm"));
    assert(!carMap3.addCar("ba booh"));
    assert(!carMap3.addCar("be booh"));
    assert(carMap3.fleetSize() == 3);
    assert(carMap3.useGas("be booh", 9) && carMap3.addGas("be booh", 9));

    CarMap carMapArr[3] = {carMap1, carMap2, carMap3};
    for (int i = 0; i < 2; i++)
    {
        carMapArr[i] = carMapArr[i+1]; //calls auto-generated assignment operator
        carMapArr[i].print();
    } //all CarMaps should be the same after the loop
    
    
    assert(carMap3.addCar("ouch") && carMap3.gas("ouch")==0);
    assert(carMap3.useGas("ouch", 0) && carMap3.addGas("be booh", 3.14));
    carMap3.print();
    
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
    
}
