#include "poker.h"
#include "arrays.h"

#include <stdio.h>
#include <set>

/*************************************************/
/*                                               */
/* This code tests my evaluator, by looping over */
/* all 2,598,960 possible five card hands, cal-  */
/* culating each hand's distinct value, and dis- */
/* playing the frequency count of each hand type */
/*                                               */
/* Kevin L. Suffecool, 2001                      */
/* suffecool@bigfoot.com                         */
/*                                               */
/*************************************************/

int main( int /*argc*/, char** /*argv*/ )
{
    int deck[ 52 ] = {0};
    init_deck( deck );

    int freq[ 10 ] = {0};
    int subFreq[ 10 ] = {0};
    //int deck[ 52 ], hand[ 5 ], freq[ 10 ];
    //int a, b, c, d, e, i, j;

    //// zero out the frequency array
    //for ( i = 0; i < 10; i++ )
    //    freq[ i ] = 0;

    auto c1 = deck[ find_card( Deuce, DIAMOND, deck ) ];
    auto c2 = deck[ find_card( Deuce, HEART, deck ) ];
    auto c3 = deck[ find_card( Deuce, CLUB, deck ) ];
    auto c4 = deck[ find_card( Trey, SPADE, deck ) ];
    auto c5 = deck[ find_card( Trey, HEART, deck ) ];
    auto i = eval_5cards( c1, c2, c3 , c4, c5 );
    auto j = hand_rank( i );
    std::set< int > seen;
    // loop over every possible five-card hand
    for ( int c1 = 0; c1 < 48; c1++ )
    {
        int hand[ 5 ] = { 0 };
        hand[ 0 ] = deck[ c1 ];
        for ( int c2 = c1 + 1; c2 < 49; c2++ )
        {
            hand[ 1 ] = deck[ c2 ];
            for ( int c3 = c2 + 1; c3 < 50; c3++ )
            {
                hand[ 2 ] = deck[ c3 ];
                for ( int c4 = c3 + 1; c4 < 51; c4++ )
                {
                    hand[ 3 ] = deck[ c4 ];
                    for ( int c5 = c4 + 1; c5 < 52; c5++ )
                    {
                        hand[ 4 ] = deck[ c5 ];

                        auto i = eval_5hand( hand );
                        auto j = hand_rank( i );

                        freq[ j ]++;
                        if ( seen.find( i ) != seen.end() )
                        {
                            continue;
                        }
                        seen.insert( i );
                        subFreq[ j ]++;
                    }
                }
            }
        }
    }

    printf( "All Hands:\n" );
    for ( int i = 1; i <= 9; i++ )
        printf( "%15s: %8d\n", value_str[ i ], freq[ i ] );
    printf( "Unique Hands:\n" );
    for ( int i = 1; i <= 9; i++ )
        printf( "%15s: %8d\n", value_str[ i ], subFreq[ i ] );
}