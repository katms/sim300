#include "Soldier.h"
#include <cstdlib> //rand
#include <climits> //UINT_MAX

//default constructor
//id defaults to UINT_MAX so I can use 0 for other things
Soldier::Soldier():
    id(UINT_MAX), time_to_attack(0), spartan(false)
    {}

//constructor that sets all variables
Soldier::Soldier(unsigned _id, int time, bool spartan):
    id(_id), time_to_attack(time), spartan(spartan)
    {}
            
//constructor that only takes id, for finding/comparing to other instances
Soldier::Soldier(unsigned _id):
    id(_id), time_to_attack(0), spartan(false)
    {}

bool Soldier::operator==(const Soldier& other) const
{
    return id==other.id;
}

bool Soldier::operator!=(const Soldier& other) const
{
            return !(*this==other);
}

//decrease key
Soldier& Soldier::operator-=(const int minus)
{
    time_to_attack-=minus;
    //if(time_to_attack<0) time_to_attack=0;
    return *this;
}

//increase key
Soldier& Soldier::operator+=(const int plus)
{
    time_to_attack+=plus;
    return *this;
}

bool Soldier::operator<(const Soldier& other) const
{
    return time_to_attack<other.time_to_attack;
}

bool Soldier::operator<=(const Soldier& other) const
{
    return time_to_attack<=other.time_to_attack;
}

Soldier::operator int() const
{
    return time_to_attack;
}

//determines if the soldier is dead
//Persian die after any wounds, Spartans after 3
bool Soldier::killed() const
{
    return (spartan) ? (wounds>=MAX_WOUNDS) : (wounds>0);
}

//handles attacking
//returns true if succeeded (to cut down on checking attacks by Persians)
bool Soldier::attack(Soldier& enemy)
{
    bool success=false;
                    //0 to 4 out of 99
    if(spartan || rand()%100 < INJURY_CHANCE) //attack succeeds
    {
        ++enemy.wounds;
        success=true;
        //handle death or injury in simulator
    }
    return success;
}

//GET & SET VARIABLES

//handles resetting time_to_attack
void Soldier::set_next_turn()
{
    //assume the clock advanced since attacking
    //countdown is treated as zero (even if negative)
    if(spartan) 
    {
        time_to_attack=rand()%6 + 1;
    }
    else
    {
        time_to_attack=rand()%51 + 10;
    }
}

unsigned Soldier::get_id() const
{
    return id;
}
        
int Soldier::get_time() const
{
    return time_to_attack;
}
        
bool Soldier::is_spartan() const
{
    return spartan;
}
