//Katherine Sullivan
//CSCI 335 Heap Assignment

#include "HashTable.h"

//constructor
template <class Key, class Value>
HashTable<Key, Value>::HashTable(int size):
    HashTable(Key{}, Value{}, size)
{}

//constructor with default values
template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Key& def_key, const Value& def_val, int size):
    DEFAULTS(def_key, def_val), current_size(0), array(next_prime(size))
{
    clear();
}

//inserts Key, Value pair into table
//ignores duplicates of valid items
//overwrites and validates deleted items
template<class Key, class Value>
void HashTable<Key,Value>::insert(const Key& key, const Value& value)
{
    if(static_cast<unsigned>(current_size)>array.size()/2)
    {
        rehash();
    }
    
    unsigned index=index_of(key);
    
    if(EMPTY==array[index].status) //if entry is not empty, not adding a new entry
    {
        ++current_size;
    }
    else if(VALID==array[index].status) //duplicate item, ignore
    {
        return;
    }
    //add new entry (or overwrite deleted entry with the same key)
    array[index].key=key;
    array[index].value=value;
    array[index].status=VALID;
}

//returns pair<Key, Value> for key
template<class Key, class Value>
auto HashTable<Key, Value>::get(const Key& key) const -> pair
{
    unsigned index=index_of(key);
    if(VALID==array[index].status)
    {
        return pair(array[index].key, array[index].value);
    }
    else return DEFAULTS;
}

//returns value for key
template<class Key, class Value>
const Value& HashTable<Key, Value>::get_value(const Key& key) const
{
    unsigned index=index_of(key);
    if(array[index].status!=VALID)
    {
        return DEFAULTS.second;
    }
    else return array[index].value;
}

//sets value for key
template<class Key, class Value>
void HashTable<Key, Value>::set_value(const Key& key, const Value& value)
{
    unsigned index=index_of(key);
    if(VALID==array[index].status) //only sets value if key is valid
    {
        array[index].value=value;
    }
}

//returns true if key is in table
template<class Key, class Value>
bool HashTable<Key, Value>::contains(const Key& key) const
{
    return VALID==array[index_of(key)].status;
}

//resets all non-empty entries
template <class K, class V>
void HashTable<K,V>::clear()
{
    for(auto& e : array)
    {
        e.key=DEFAULTS.first;
        e.value=DEFAULTS.second;
        e.status=EMPTY;
    }
    current_size=0;
}

//removes key (lazy deletion)
template<class Key, class Value>
void HashTable<Key, Value>::remove(const Key& key)
{
    unsigned index=index_of(key);
    if(VALID==array[index].status) //if EMPTY, item not found, ignore
    {
        array[index].status=DELETED;
    }
}


template<class Key, class Value>
std::size_t HashTable<Key, Value>::hash_index(const Key& key) const
{
    static std::hash<Key> hf;
    return hf(key) % array.size();
}

//quadratic probing
template <class Key, class Value>
int HashTable<Key, Value>::index_of(const Key& key) const
{
    auto start=hash_index(key);
    unsigned index=start, offset=0;
    while(array[index].key!=key && array[index].status!=EMPTY)
    {
        ++offset;
        index=(start + offset*offset) % array.size();
    }
    return index;
}

//resizes the table and only puts back valid entries
template<class Key, class Value>
void HashTable<Key, Value>::rehash()
{
    std::vector<Entry> entries = array;
    array.clear();
    array.resize(next_prime(entries.size()*2));
    current_size=0;
    
    for(auto& e : entries)
    {
        if(VALID==e.status)
        {
            insert(e.key, e.value);
        }
    }
}

template<class Key, class Value>
bool HashTable<Key, Value>::is_prime(const int n)
{
    if(n<=1) return false;
    for(int i=2; i*i<=n; ++i)
    {
        if(0==n%i) return false;
    }
    return true;
}
 
template<class Key, class Value>
int HashTable<Key, Value>::next_prime(int n)
{
    //make sure n isn't 0,1, or a negative number before checking only odd numbers (if it is, strictly speaking the nest prime is 2)
    if(n<=2) return 2;
    else 
    {
       if(0==n%2) ++n; //if n is 2 then it is the next prime so 3 is next
        while(!is_prime(n))
        {
            n+=2; //check only odd numbers
        }
        return n;
    }
}
