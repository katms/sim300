//Katherine Sullivan
//CSCI 335 Heap Assignment

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <vector> //vector
#include <functional> //hash

//uses quadratic probing

template <class Key, class Value>
class HashTable
{
  public:
    //default constructor
    explicit HashTable(int size = 101);
    
    //constructor that sets default values for each entry
    HashTable(const Key& def_key, const Value& def_val, int size = 101);
    
    typedef std::pair<Key, Value> pair;
    //holds default values for Key and Value
    const pair DEFAULTS;
    
    //creates an entry for key and value
    void insert(const Key& key, const Value& value);
    
    //returns a std::pair containing a copy of the exact key in the table and the associated value
    pair get(const Key& key) const;
    
    //returns the value associated with the given key
    const Value& get_value(const Key& key) const;
    
    //sets the value of an existing entry
    void set_value(const Key& key, const Value& value); 
    
    //returns true if there is an entry for key
    bool contains(const Key& key) const;
    
    //clears and resets the table (maintaining capacity)
    void clear();
    
    //removes the entry associated with key
    void remove(const Key& key);
      
  private:
      //used for empty cells and lazy deletion
      enum Entry_Status {EMPTY, VALID, DELETED};
      struct Entry
      {          
          Key key;
          Value value;
          Entry_Status status;
          
          Entry(Key k=Key{}, Value i=Value{}, Entry_Status s=EMPTY):
              key(k), value(i), status(s)
              {}
      };
      
      int current_size; //number of non-empty entries
      std::vector<Entry> array; //entries
      
      std::hash<Key> hash_function;
      std::size_t hash_index(const Key& key) const; //returns the index key hashes to
      
      //returns the index key is stored at (or -1)
      int index_of(const Key& key) const;
      
      //resizes table and hashes all valid entries
      void rehash();
      
      
      //defining these two outside of the class causes compiler errors
      
      //returns true if n is prime
      static bool is_prime(const int n);
      
      //returns the first prime >=n
      static int next_prime(int n); 
};



#endif
