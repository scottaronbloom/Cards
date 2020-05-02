#include <stdio.h>
#include "arrays.h"
#include "poker.h"
#include <fstream>
#include <random>
#include <algorithm>
// Poker hand evaluator
//
// Kevin L. Suffecool
// suffecool@bigfoot.com
//

// perform a binary search on a pre-sorted array
//

uint64_t findit( int64_t key )
{
    computeProductLookupTable();
    auto pos = fProductMap.find( key );
    if ( pos == fProductMap.end() )
    {
        fprintf( stderr, "ERROR:  no match found; key = %d\n", (int)key );
        return -1;
    }
    return (*pos).second;
}

//
//   This routine initializes the deck.  A deck of cards is
//   simply an integer array of length 52 (no jokers).  This
//   array is populated with each card, using the following
//   scheme:
//
//   An integer is made up of four bytes.  The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card
//
void
init_deck( int* deck )
{
    int i, j, n = 0, suit = 0x8000;

    std::ofstream ofs( "DeckDump.cpp", std::ofstream::out | std::ofstream::trunc );

    ofs << "fDeckDump = \n{\n";
    bool first = true;
    for ( i = 0; i < 4; i++, suit >>= 1 )
    {
        for ( j = 0; j < 13; j++, n++ )
        {
            deck[ n ] = primes[ j ] | ( j << 8 ) | suit | ( 1 << ( 16 + j ) );
            ofs << "    ";
            if ( first )
                ofs << " ";
            else
                ofs << ",";
            ofs << "{ { ";
            first = false;
            ofs.width( 16 );
            ofs.fill( ' ' );
            switch( suit )
            {
                case 0x8000: ofs << "ESuit::eClub"; break;
                case 0x4000: ofs << "ESuit::eDiamond"; break;
                case 0x2000: ofs << "ESuit::eHeart"; break;
                case 0x1000: ofs << "ESuit::eSpade"; break;
                default:
                    break;
            }
            ofs << ",";

            ofs.width( 14 );
            ofs.fill( ' ' );
            switch( j )
            {
                case Deuce: ofs << "ECard::eTwo"; break;
                case Trey:  ofs << "ECard::eThree"; break;
                case Four:  ofs << "ECard::eFour"; break;
                case Five:  ofs << "ECard::eFive"; break;
                case Six:   ofs << "ECard::eSix"; break;
                case Seven: ofs << "ECard::eSeven"; break;
                case Eight: ofs << "ECard::eEight"; break;
                case Nine:  ofs << "ECard::eNine"; break;
                case Ten:   ofs << "ECard::eTen"; break;
                case Jack:  ofs << "ECard::eJack"; break;
                case Queen: ofs << "ECard::eQueen"; break;
                case King:  ofs << "ECard::eKing"; break;
                case Ace:   ofs << "ECard::eAce"; break;
                default:
                    break;
            }
            ofs << " }, 0x";
            ofs.width( 8 );
            ofs.fill( '0' );
            ofs << std::hex << deck[ n ] << " }\n";
        }
    }
    ofs << "}\n";
}


//  This routine will search a deck for a specific card
//  (specified by rank/suit), and return the INDEX giving
//  the position of the found card.  If it is not found,
//  then it returns -1
//
int
find_card( int rank, int suit, int* deck )
{
    int i, c;

    for ( i = 0; i < 52; i++ )
    {
        c = deck[ i ];
        if ( ( c & suit ) && ( RANK( c ) == rank ) )
            return i;
    }
    return -1;
}


//
//  This routine takes a deck and randomly mixes up
//  the order of the cards.
//
void shuffle_deck( int* deck )
{
    std::random_device rd;
    std::mt19937_64 gen( rd() );
    std::shuffle( deck, deck + 52, gen );
}


void print_hand( int* hand, int n )
{
    int i, r;
    char suit;
    static const char* rank = "23456789TJQKA";

    for ( i = 0; i < n; i++ )
    {
        r = ( *hand >> 8 ) & 0xF;
        if ( *hand & 0x8000 )
            suit = 'c';
        else if ( *hand & 0x4000 )
            suit = 'd';
        else if ( *hand & 0x2000 )
            suit = 'h';
        else
            suit = 's';

        printf( "%c%c ", rank[ r ], suit );
        hand++;
    }
}


int
hand_rank( short val )
{
    if ( val > 6185 ) return HIGH_CARD;        // 1277 high card
    if ( val > 3325 ) return ONE_PAIR;         // 2860 one pair
    if ( val > 2467 ) return TWO_PAIR;         //  858 two pair
    if ( val > 1609 ) return THREE_OF_A_KIND;  //  858 three-kind
    if ( val > 1599 ) return STRAIGHT;         //   10 straights
    if ( val > 322 )  return FLUSH;            // 1277 flushes
    if ( val > 166 )  return FULL_HOUSE;       //  156 full house
    if ( val > 10 )   return FOUR_OF_A_KIND;   //  156 four-kind
    return STRAIGHT_FLUSH;                   //   10 straight-flushes
}


short
eval_5cards( int c1, int c2, int c3, int c4, int c5 )
{
    uint64_t q;
    short s;

    q = ( c1 | c2 | c3 | c4 | c5 ) >> 16;

    /* check for Flushes and StraightFlushes
    */
    if ( c1 & c2 & c3 & c4 & c5 & 0xF000 )
        return flushes[ q ];

    /* check for Straights and HighCard hands
    */
    s = unique5[ q ];
    if ( s )  
        return s;

    /* let's do it the hard way
    */
    q = ( c1 & 0xFF ) * ( c2 & 0xFF ) * ( c3 & 0xFF ) * ( c4 & 0xFF ) * ( c5 & 0xFF );
    q = findit( q );

    return sValues[ q ];
}


short
eval_5hand( int* hand )
{
    int c1, c2, c3, c4, c5;

    c1 = *hand++;
    c2 = *hand++;
    c3 = *hand++;
    c4 = *hand++;
    c5 = *hand;

    return eval_5cards( c1, c2, c3, c4, c5 );
}


// This is a non-optimized method of determining the
// best five-card hand possible out of seven cards.
// I am working on a faster algorithm.
//
short
eval_7hand( int* hand )
{
    int i, j, q, best = 9999, subhand[ 5 ];

    for ( i = 0; i < 21; i++ )
    {
        for ( j = 0; j < 5; j++ )
            subhand[ j ] = hand[ perm7[ i ][ j ] ];
        q = eval_5hand( subhand );
        if ( q < best )
            best = q;
    }
    return best;
}
