//Katherine Sullivan
//CSCI 335 Heap Assignment

#include "DHeap.h"
#include "HashTable.cpp"
#include <cstdlib> //rand

//constructor
template <class T>
DHeap<T>::DHeap(unsigned d, int capacity):
    D(d), current_size(0), table(T{}, -1, 2*capacity)
{
    if(capacity<=0) capacity=10; //make sure capacity is positive
    array.reserve(capacity);
}

//returns the number of items in the heap
template<class T>
unsigned DHeap<T>::size() const
{
    return array.size();
}

//returns true if heap is empty
template<class T>
bool DHeap<T>::empty() const
{
    return array.empty();
}

//inserts item into heap
template <class T>
void DHeap<T>::insert(const T& item)
{
    if(table.contains(item)) //duplicate, do nothing
    {
        return;
    }
    
    ++current_size;
    
    //add item to the end of array (the newest heap)
    array.push_back(item);
    
    //update table
    table.insert(item, current_size-1);
    //the heap allows duplicate items but the table does not, so hopefully this situation will never come up
        
    //find item's real place in the heap
    percolateUp(array.size()-1);
}

//removes minimum from heap and returns it
template <class T>
T DHeap<T>::delete_min()
{
    if(array.empty())
    {
        throw "Called delete_min() on empty heap";
    }
    
    //get/remove minimum
    T minimum = std::move(array[0]);
    
    //update table
    table.remove(minimum);
    
    //check if any additional work needs to be done
    if(array.size()==1) //min is the last item, just clear array
    {
        clear();
        return minimum;
    }
    
    //place last item in the heap in hole left by minimum
    array[0] = std::move(array.back());
    table.set_value(array[0], 0);
    array.pop_back();
    --current_size;
    
    //the last item is unlikely to belong at index 0
    percolateDown(0);

    return minimum;
}


template <class T>
void DHeap<T>::decreaseKey(const T& item, const int change)
{
    int index=index_of(item); //find item
    if(index<0)
    {
        return;
    }
    else
    {
        array[index]-=change; //decrease key
        percolateUp(index); //update
    }
}

template <class T>
void DHeap<T>::increaseKey(const T& item, const int change)
{
    int index=index_of(item); //find item
    if(index<0)
    {
        return;
    }
    else
    {
        array[index]+=change; //increase key
        percolateDown(index); //update
    }
}

//remove item by percolating it up to the top and calling delete_min()
template <class T>
void DHeap<T>::remove(const T& item)
{
    int index=index_of(item);
    if(index<0) //item not in heap
    {
        return;
    }
    else if(0==index) //item is already min, no need to decreaseKey
    {
        delete_min();
    }
    else
    {
        int decrement = static_cast<int>(array[index]) - static_cast<int>(array[0]); 
        //the decrement needed so that item is equivalent to min
        
        //since items percolate as far up or down as possible (while <=) item becomes the new min
        decreaseKey(item, decrement);
        
        /**
        OR:
        while(array[0] < array[index])
            decreaseKey(array[index], some number, 1 would do but could go higher)
        **/
        
        delete_min();
    }
}

//make heap empty
template <class T>
void DHeap<T>::clear()
{
    table.clear(); //update table
    array.clear();
    current_size=0;
}

//shortcut to call T-=(change) on every item in the heap
//since every item decreases by the same constant amount, no need to re-arrange the heap after calling it on each item
template <class T>
void DHeap<T>::decrease_all(const int change)
{
    for(unsigned i=0; i<array.size(); ++i)
    {
        array[i]-=change;
    }
}

//returns a randomly selected object in the heap that satisfies test
//returns nullptr if no qualifying item
template <class T>
T* DHeap<T>::get_random(Test& test)
{
    if(array.empty())
    {
        return nullptr;
    }
    int searched=0, i=rand()%current_size; //start from a random index
    while(searched<current_size) //check every item
    {
        ++searched;
        if(test(array[i]))
        {
            return &(array[i]);
        }
        
        ++i;
        if(current_size==i) //wrap around
        {
            i=0;
        }
    }
    //no qualified item found
    return nullptr;
}


//PERCOLATING

//repeatedly swap array[hole] with its parent as long as parent > array[hole]
//if item are equivalent, keeps going
template <class T>
void DHeap<T>::percolateUp(int hole) //assume hole is valid index
{
    for(int parent=parent_of(hole); parent>-1 && array[hole] <= array[parent]; hole=parent, parent=parent_of(parent))
    {
        std::swap(array[parent], array[hole]);
        table.set_value(array[parent], parent);
        table.set_value(array[hole], hole);
    }
}

//repeatedly swap array[hole] with its smaller child as long as array[hole] is larger
//if item are equivalent, keeps going
template <class T>
void DHeap<T>::percolateDown(int hole) //assume hole is valid index
{
    for(int child=smallest_child(hole); child>-1 && array[child] <= array[hole]; hole=child, child=smallest_child(child))
    {
        std::swap(array[hole], array[child]);
        table.set_value(array[child], child);
        table.set_value(array[hole], hole);

    }
}

//INDEXING FUNCTIONS
//in each case, returns -1 if not found


template <class T>
int DHeap<T>::parent_of(const int child_index) const
{
    if(child_index<=0)
    {
        return -1;
    }
    else if(child_index%D==0) //rightmost child
    {
        return child_index/D - 1;
    }
    else
    {
        return child_index/D;
    }
}

//given parent returns the index of its smallest child
template <class T>
int DHeap<T>::smallest_child(int parent) const
{
    const unsigned limit=array.size();
    const unsigned first=parent*D + 1;
    
    if(first>limit) //parent has no children
    {
        return -1;
    }
    unsigned smallest=first;
    for(unsigned child_index=first; 
                                //last child is first+D-1
        child_index < limit && child_index < first+D; 
        ++child_index)
        {
            if(array[child_index] < array[smallest])
            {
                smallest=child_index;
            }
        }
    return smallest;
}

//uses table to find the index of item
//returns default value (-1) if item is not in table, and therefore not in the heap
template <class T>
int DHeap<T>::index_of(const T& item) const
{
    return table.get_value(item);
}
