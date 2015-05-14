//Katherine Sullivan
//CSCI 335 Heap Assignment

#include "Thermopylae.h"
#include "DHeap.cpp"
#include <cstdlib> //rand
#include <iostream> //cout
using std::cout;


//static variables
BinaryHeap<Soldier>::Test Thermopylae::persian = [] (const Soldier& s) { return !s.is_spartan(); };
BinaryHeap<Soldier>::Test Thermopylae::spartan = [] (const Soldier& s) { return s.is_spartan(); };

//constructor
Thermopylae::Thermopylae(const unsigned nspartans, const unsigned npersians):
    SPARTANS(nspartans), PERSIANS(npersians), heap(SPARTANS+PERSIANS), ticks(0), winner(UNDECIDED), survivors(0)
{}

//simulate battle once
void Thermopylae::run()
{
    setup();
    if(heap.empty()) //no soldiers
    {
        return;
    }
        /**
        while(neither army is gone)
            get_min
            find random enemy
            if enemy found
                attack enemy
                if (enemy killed)
                    remove enemy from heap (and set of Spartans)
                    if Spartan decreaseKey for all Spartans left
                else if (enemy wounded)
                    increaseKey for enemy
            put soldier back, with reset time
            */

    Soldier* target=nullptr;
    do
    {
        //find next attacker and target
        Soldier&& next=heap.delete_min();
        if(next.is_spartan()) target=heap.get_random(persian);
        else target=heap.get_random(spartan);
        
        if(target) //if the opposing army still exists
        {
            //find time until next soldier's move, advance clock unless negative
            int until_next=next.get_time();
            if(until_next>0)
            {
                ticks+=until_next;
                heap.decrease_all(until_next);
            }
            bool success = next.attack(*target);
            
            if(success)
            {
                if(target->killed())
                {
                    //copy *target before it gets removed and destroyed
                    Soldier dead(*target);
                    
                    //not setting target to nullptr because it's checked against that at the end of the loop,
                    //but don't refer to it again
                    heap.remove(dead);
                    
                    if(dead.is_spartan()) //Spartan dies, inspiring the rest
                    {
                        spartan_army.erase(dead.get_id());
                        for(auto& id : spartan_army)
                        {
                            heap.decreaseKey(Soldier(id), rand()%2+1);
                        } //for all Spartans
                    } //if dead.SPARTAN
                }//if target killed
                
                else //target was non-fatally wounded, must be Spartan
                {
                    heap.increaseKey(*target, rand()%4+1);
                }//else wounded
                
            } //if successful attack

            next.set_next_turn();

        } //if target found
        
        heap.insert(next);
        //put soldier back into the heap
        //even if the battle is over, they're still alive and need to be counted as a survivor
        
    } while(nullptr!=target);

    set_stats();
    print_results();
}

//access post-battle statistics:
unsigned Thermopylae::ticks_elapsed() const 
{
    return ticks;
}
unsigned Thermopylae::get_survivors() const 
{
    return survivors;
}
Thermopylae::victor Thermopylae::get_winner() const 
{
    return winner;
}

//reset all variables for next run
void Thermopylae::setup()
{
    winner=UNDECIDED;
    survivors=0;
    ticks=0;
    spartan_army.clear();
    heap.clear();

    //create soldiers
    
    for(unsigned i=0; i<SPARTANS; ++i)
    {
        Soldier s(i, rand()%49 + 1, true);
        spartan_army.insert(spartan_army.end(), s.get_id());
        heap.insert(s);
    }
    
    for(unsigned j=0; j<PERSIANS; ++j)
    {
        Soldier p(j+SPARTANS, rand()%(1000-51) + 51, false);
        heap.insert(p);
    }
}

//sets post-battle statistics
//assume simulation has been run already
void Thermopylae::set_stats()
{
    //ticks stays as it was left in run()
    
    //count survivors before deleting one
    survivors=heap.size();
    
    //determine winner by checking the allegiance of any survivor
    Soldier&& survivor=heap.delete_min();
    if(spartan(survivor))
    {
        winner=SPARTA;
    }
    else
    {
        winner=PERSIA;
    }
    
    //skip re-inserting survivor because it's a worst-case insert that's already been done once
    //and this simulation already takes a long time
    //they're all about to be destroyed on the next run anyway
}

//assumes set_stats() has been called
void Thermopylae::print_results() const
{
    cout<<"Winner: ";
    if(SPARTA==winner)
    {
        cout<<"Sparta";
    }
    else cout<<"Persia";
    
    cout<<"\tSurvivors: "<<survivors;
    cout<<"\tTicks: "<<ticks<<"\n";
}
