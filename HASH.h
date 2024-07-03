#ifndef Hash_h
#define Hash_h

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>

using namespace std;

template <class HashedObj, class value>
class HashTable
{
  public:
    
    explicit HashTable( const HashedObj & notFound, int size = 101 );
    HashTable( const HashTable & rhs )
           : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
             array( rhs.array ), currentSize( rhs.currentSize ) { }

    const HashedObj & find( const HashedObj & x ) const;
    value getvalue(const HashedObj & x );
    void update(const HashedObj & x, const value & updated);

    void makeEmpty( );
    void insert( const HashedObj & x, const value & y);
    void remove( const HashedObj & x );
    const HashTable & operator=( const HashTable & rhs );
    int output(float & load_ratio);
    
    enum EntryType { ACTIVE, EMPTY, DELETED };
  
  private:
    
    struct HashEntry
    {
         HashedObj element;
         value details;
         EntryType info;
          
         HashEntry( const value & theDetail = value(), const HashedObj & e = "",
                    EntryType i = EMPTY )
                  : details(theDetail), element( e ), info( i )  { }
        
        void makeEmpty1() {
                element = ""; // Reset the element
                details = value();     // Reset the details
                info = EMPTY;          // Mark as empty
            }
    };
            
    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    bool isActive( int currentPos ) const;
    int findPos( const HashedObj & x ) const;
    void rehash( );
    bool isPrime( int n );
    int nextPrime( int n );
    void makeEmpty1();
   // int hash( const string & key, int tableSize ) const;
    int hash( const string & key, int tableSize ) const;
 };
  

/**
 * Construct the hash table.
 */
template <class HashedObj, class value>
HashTable<HashedObj, value>::HashTable( const HashedObj & notFound,
                                      int size )
          : ITEM_NOT_FOUND( notFound ), array( nextPrime( size ) )
{
       makeEmpty( );
}
/**
 * Method that performs quadratic probing resolution.
 * Return the position where the search for x terminates.
 */
// Hash function for strings

template <class HashedObj, class value>
int HashTable<HashedObj, value>::hash(const string& key, int table_size) const {
    
    static const size_t multiplier = 263; // A prime number
    static const size_t prime = 1000000007; // A large prime number
    
    size_t hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * multiplier + c) % prime;
    }
    return hashValue % table_size;
}


template <class HashedObj, class value>
int HashTable<HashedObj, value>::findPos( const HashedObj & x ) const
{
       int collisionNum = 0;
       int currentPos = hash( x, array.size( ) );

       while ( array[ currentPos ].info != EMPTY &&
           array[ currentPos ].element != x )
       {
            currentPos += pow(++collisionNum, 2) ;  //add the difference
            if ( currentPos >= array.size( ) )              // perform the mod
                 currentPos -= array.size( );                // if necessary
       }
       return currentPos;
}

template <class HashedObj, class value>
int HashTable<HashedObj, value>::output(float & load_ratio){
    load_ratio = (float) currentSize / array.size();
    return currentSize;
}

template <class HashedObj, class value>
bool check_document(value& temp, string file_name, int & idx){
    for (int i = 0; i < temp.details.size(); i++){
        if (temp.details[i].documentName == file_name){
            idx = i;
            temp.details->documents[index].count += 1; // Increment count
            return true;
        }
    }
    return false;
}

template <class HashedObj, class value>
value HashTable<HashedObj, value>::getvalue(const HashedObj & x )  {
    
    int currentPos = findPos( x );
    value copy;
    copy.documents = array[currentPos].details.documents;
    copy.word_name = array[currentPos].details.word_name;
    return copy;
}
 

template <class HashedObj, class value>
void HashTable<HashedObj, value>::update(const HashedObj & x, const value & updated){
    
    int currentPos = findPos( x );
    array[ currentPos ].info = EMPTY;       
    array[ currentPos ] = HashEntry( updated, x, ACTIVE );
}
/**
  * Return true if currentPos exists and is active.
  */
template <class HashedObj, class value>
 bool HashTable<HashedObj, value>::isActive( int currentPos ) const
 {
       return array[ currentPos ].info == ACTIVE;
 }
/**
 * Remove item x from the hash table.
 *  x has to be in the table
 */
template <class HashedObj, class value>
void HashTable<HashedObj, value>::remove( const HashedObj & x )
{
      int currentPos = findPos( x );
      if ( isActive( currentPos ) )
          array[ currentPos ].info = DELETED;
}
/**
 * Find item x in the hash table.
 * Return the matching item, or ITEM_NOT_FOUND, if not found.
 */
template <class HashedObj, class value>
const HashedObj & HashTable<HashedObj, value>::find( const HashedObj & x ) const
{
     int currentPos = findPos( x );
     if (isActive( currentPos ))
          return array[ currentPos ].element;

     return   ITEM_NOT_FOUND;
}
/**
  * Insert item x into the hash table. If the item is
  * already present, then do nothing.
  */
template <class HashedObj, class value>
 void HashTable<HashedObj, value>::insert( const HashedObj & x, const value & y)
 {
      // Insert x as active
      int currentPos = findPos( x );
     if ( isActive( currentPos ) )
         return;
         
     array[ currentPos ] = HashEntry( y, x, ACTIVE );

      // enlarge the hash table if necessary
     if (++currentSize >= 0.68 * array.size())
         rehash();

 }

/**
 * Expand the hash table.
 */
template <class HashedObj, class value>
void HashTable<HashedObj, value>::rehash( )
{
    vector<HashEntry> oldArray = array;
    cout << "rehashed..." << endl;
    cout << "previous table size:" << oldArray.size() << ", new table size: " << nextPrime(2 * oldArray.size( )) << ",current unique word count " << currentSize << "," << endl;
    float currentload = (float) currentSize / nextPrime(2 * oldArray.size( ));
    cout << "current load factor: " << currentload << endl;

    // Create new double-sized, empty table
    array.resize( nextPrime( 2 * oldArray.size( ) ) );
    for ( int j = 0; j < array.size( ); j++ )
         array[ j ].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for ( int i = 0; i < oldArray.size( ); i++ )
        if ( oldArray[ i ].info == ACTIVE )
             insert( oldArray[ i ].element, oldArray[ i ].details );
}
/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
template <class HashedObj, class value>
bool HashTable<HashedObj, value>::isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
         if ( n % i == 0 )
             return false;

    return true;
}
/**
    * Internal method to return a prime number
   * at least as large as n.  Assumes n > 0.
    */
template <class HashedObj, class value>
int HashTable<HashedObj, value>::nextPrime( int n )
{
     if ( n % 2 == 0 )
         n++;

     for ( ; ! isPrime( n ); n += 2 )
          ;

     return n;
}

// Make the hash table logically empty.
template <class HashedObj, class value>
void HashTable<HashedObj, value>::makeEmpty( ){
    for( int i = 0; i < array.size( ); i++ )
        array[ i ].makeEmpty1( );
// destroy the lists but not the vector!
}
    
#endif /* Hash_h */


