//
//  CarMap.cpp
//  Homework1
//
//  Created by Stephanie Chen on 1/20/18.
//  Copyright © 2018 Stephanie Chen. All rights reserved.
//

#include "CarMap.h"
#include "Map.h" //shouldnt depend on carmap.h to include map.h

#include <cstdlib> //uses std
#include <string> //uses string
#include <iostream> //uses cout in print()

using namespace std;

CarMap::CarMap()       // Create an empty car map.
//auto-generated C++ constructor should call Map's default constructor which will generate an empty map
{}

bool CarMap::addCar(std::string license)
// If a car with the given license plate is not currently in the map,
// and there is room in the map, add an entry for that car recording
// that it has 0 gallons of gas in it, and return true.  Otherwise,
// make no change to the map and return false.
{
    if (m_map.insert(license, 0))
        return true;
    
    return false;
}

double CarMap::gas(std::string license) const
// If a car with the given license plate is in the map, return the
// number of gallons of gas in its tank; otherwise, return -1.
{
    double numGallons = 0;
    
    if (m_map.get(license, numGallons))
        return numGallons;
    
    return -1;
}

bool CarMap::addGas(std::string license, double gallons)
// If no car with the given license plate is in the map or if
// gallons is negative, make no change to the map and return
// false.  Otherwise, increase the number of gallons of gas in the
// indicated car by the gallons parameter and return true.
{
    if (!m_map.contains(license) || gallons < 0)
        return false;
    
    double numGallons = 0;
    numGallons = gas(license) + gallons;
    m_map.update(license, numGallons);
    return true;
}

bool CarMap::useGas(std::string license, double gallons)
// If no car with the given license plate is in the map or if
// gallons is negative or if gallons > gas(), make no change to the
// map and return false.  Otherwise, decrease the number of gallons
// of gas in the indicated car by the gallons parameter and return
// true.
{
    if (!m_map.contains(license) || gallons < 0 || gallons > gas(license))
        return false;
    
    double numGallons = 0;
    numGallons = gas(license) - gallons;
    m_map.update(license, numGallons);
    return true;

}

int CarMap::fleetSize() const  // Return the number of cars in the CarMap.
{
    return m_map.size();
}

void CarMap::print() const
// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of gallons in that car's tank.  Write
// no other text.
{
    string licensePlate = "";
    double numGallons = 0;
    
    for (int i = 0; i < m_map.size(); i++)
    {
        m_map.get(i, licensePlate, numGallons);
        cout << licensePlate << " " << numGallons << endl;
    }
}
