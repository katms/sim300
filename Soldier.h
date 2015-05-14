#ifndef SOLDIER_H
#define SOLDIER_H
#include <functional> //std::hash

class Soldier
{
    public:
        //CONSTRUCTORS
        
        //default constructor
        Soldier();
            
        //complete constructor
        Soldier(unsigned _id, int time, bool spartan);
            
        //constructor that only takes id, for finding/comparing to other Soldiers
        explicit Soldier(unsigned _id);


        //OPERATORS
        
        //compares id
        bool operator==(const Soldier& other) const;
        
        //compares id
        bool operator!=(const Soldier& other) const;
        
        //decreases time
        Soldier& operator-=(const int minus);
        
        //increases time
        Soldier& operator+=(const int plus);

        //compares time_to_attack
        bool operator<(const Soldier& other) const;
        
        //compares time_to_attack
        bool operator<=(const Soldier& other) const;
        
        //for Soldier - Soldier
        operator int() const;
        
        
        //BATTLING
        
        //returns true if the soldier has sustained enough wounds to be considered dead
        bool killed() const;
        
        //returns true if successful attack
        bool attack(Soldier& enemy);
        
        
        //GET & SET
        
        //sets next time based on if Spartan or not
        void set_next_turn();
        
        //returns id
        unsigned get_id() const;
        
        //returns time_to_attack
        int get_time() const;
        
        //returns SPARTAN
        bool is_spartan() const;
        
    private:
        unsigned id; //considered unique
        unsigned wounds=0; //tally of wounds received, mostly applicable for just Spartans
        int time_to_attack; //can be negative
        bool spartan; //non-const because assignment
        
        static const unsigned MAX_WOUNDS = 3;
        static const int INJURY_CHANCE   = 5;
};

//hash functor
namespace std
{
    template<>
    class hash<Soldier>
    {
    public:
        size_t operator() (const Soldier& key)
        {
            return key.get_id();
        }
    };
}


#endif // SOLDIER_H
