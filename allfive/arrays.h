#ifndef __ARRAYS_H
#define __ARRAYS_H

#include <unordered_map>
#include <vector>
/*
** this is a table lookup for all "flush" hands (e.g.  both
** flushes and straight-flushes.  entries containing a zero
** mean that combination is not possible with a five-card
** flush hand.
*/
extern short flushes[];

/*
** this is a table lookup for all non-flush hands consisting
** of five unique ranks (i.e.  either Straights or High Card
** hands).  it's similar to the above "flushes" array.
*/
extern short unique5[];
extern std::vector< int > products;
extern std::unordered_map< int64_t, size_t > fProductMap;
extern void computeProductLookupTable();
extern std::vector< short > sValues;

/*
** each of the thirteen card ranks has its own prime number
**
** deuce = 2
** trey  = 3
** four  = 5
** five  = 7
** ...
** king  = 37
** ace   = 41
*/
extern int primes[];
extern int perm7[21][5];


extern const char* value_str[];
#endif

