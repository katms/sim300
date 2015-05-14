//Katherine Sullivan
//CSCI 335 Heap Assignment

#include "Thermopylae.h"
#include <vector> //vector
#include <cmath> //sqrt
#include <cstdlib> //atoi
#include <ctime> //time
#include <iostream> //cout, endl
#include <cctype> //isdigit

using std::vector;
using std::cout;
using std::endl;


bool all_digits(const std::string& word);

//returns the average of numbers
//T could be unsigned or double
template <class T> 
double average(const vector<T>& numbers);

//returns the standard deviation of numbers, takes a precomputed mean to avoid finding it twice since main already needs the average
double standard_deviation(const vector<unsigned>& numbers, const double mean);


int main(int argc, char* argv[])
{
    if(argc!=4 || !(all_digits(argv[1]) && all_digits(argv[2]) && all_digits(argv[3])))
    {
        std::cerr<<"Usage: sim300 <runs> <nSpartans> <nPersians>"<<endl;
        return 1;
    }
    
    srand(static_cast<unsigned>(time(0)));
    
    
    //assume every argument is a number
    const int N_RUNS=atoi(argv[1]);
    const int N_SPARTANS=atoi(argv[2]);
    const int N_PERSIANS=atoi(argv[3]);
    
    
    Thermopylae sim(N_SPARTANS, N_PERSIANS);
    
    //setup vectors
    vector<unsigned> spartan_victors;
    spartan_victors.reserve(N_RUNS/2+1);
    
    vector<unsigned> persian_victors;
    persian_victors.reserve(N_RUNS/2+1);
    
    vector<unsigned> time;
    time.reserve(N_RUNS);
    
    //run simulation(s)
    for(int i=1; i<=N_RUNS; ++i)
    {
        cout<<"Run "<<i<<":\t";
        sim.run(); //prints results
        
        time.push_back(sim.ticks_elapsed());
        
        if(Thermopylae::SPARTA==sim.get_winner())
        {
            spartan_victors.push_back(sim.get_survivors());
        }
        else if(Thermopylae::PERSIA==sim.get_winner())
        {
            persian_victors.push_back(sim.get_survivors());
        }
    }

    //final statistics
    
    cout<<"\n\nSpartan victories: "<<spartan_victors.size()<<endl;
    
    double avg_spartans = average(spartan_victors);
    cout<<"Average number of survivors: "<<avg_spartans
        <<"\tStandard deviation: "<<standard_deviation(spartan_victors, avg_spartans)<<endl;
        
    cout<<"\n";
    
    cout<<"Persian victories: "<<persian_victors.size()<<endl;
    
    double avg_persians = average(persian_victors);
    cout<<"Average number of survivors: "<<avg_persians
    <<"\tStandard deviation: "<<standard_deviation(persian_victors, avg_persians)<<endl;
    
    double avg_time = average(time);
    cout<<"\n\nAverage time to complete battle: "<<avg_time<<"\tStandard deviation: "<<standard_deviation(time, avg_time)<<endl;
    
    cout<<"\a";
    
    return 0;
}

double standard_deviation(const vector<unsigned>& numbers, const double mean)
{
    if(numbers.empty())
    {
        return 0.0;
    }
    vector<double> squared_differences;
    squared_differences.reserve(numbers.size());

    for(auto &n: numbers)
    {
        double difference=n-mean;
        squared_differences.push_back(difference*difference);
    }
    
    double avg_differences = average<double>(squared_differences);
    return sqrt(avg_differences);
}

template <class T>
double average(const vector<T>& numbers)
{
    if(numbers.empty())
    {
        return 0.0;
    }
    T total=0;
    for(auto& n: numbers)
    {
        total+=n;
    }
    //even if T is non-floating point the quotient will always be a double
    double count=static_cast<double>(numbers.size());
    
    return total/count;
}

bool all_digits(const std::string& word)
{
    for(unsigned i=0; i<word.size(); ++i)
    {
        if(!isdigit(word[i]))
        {
            return false;
        }
    }
        return true;
}
