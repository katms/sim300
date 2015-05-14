//Katherine Sullivan
//CSCI 335 Heap Assignment

#ifndef THERMOPYLAE_H
#define THERMOPYLAE_H
#include "Soldier.h"
#include "BinaryHeap.h"
#include <set> //set

//simulates the Battle of Thermopylae

class Thermopylae
{
    public:
        Thermopylae(const unsigned nspartans, const unsigned npersians);
        
        //runs simulation once and prints results
        void run();


        //returns time count
        unsigned ticks_elapsed() const;
        
        //returns survivors (set after run())
        unsigned get_survivors() const;
        
        
        enum victor {UNDECIDED, SPARTA, PERSIA};
        
        //returns the winner (UNDECIDED before battle, set after battle)
        victor get_winner() const;
        
    private:
        const unsigned SPARTANS;
        const unsigned PERSIANS;
        BinaryHeap<Soldier> heap;
        
        std::set<unsigned> spartan_army; //set of unique ids of every Spartan in battle
                                         //used to find every Spartan in the heap (for when one dies)
        unsigned ticks;
        
        //tests for finding enemy soldiers
        static BinaryHeap<Soldier>::Test persian;
        static BinaryHeap<Soldier>::Test spartan;
        
        //post-simulation stats
        victor winner;
        unsigned survivors;    
        
        //resets variables before battle
        void setup();
        //sets variables after battle
        void set_stats();

        void print_results() const;

};

#endif // THERMOPYLAE_H
