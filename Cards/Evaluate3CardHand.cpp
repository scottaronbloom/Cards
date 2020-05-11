// The MIT License( MIT )
//
// Copyright( c ) 2020 Scott Aron Bloom
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Evaluate3CardHand.h"
#include "Hand.h"
#include "PlayInfo.h"
#include "HandUtils.h"

#include <map>
#include <set>
#include <optional>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>

namespace NHandUtils
{
    S3CardInfo::S3CardInfo() :
        S3CardInfo( THand( TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) ) )
    {

    }

    S3CardInfo::S3CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3 ) :
        S3CardInfo( THand( TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ) ) )
    {

    }

    S3CardInfo::S3CardInfo( const THand& cards ) :
        fCard1( std::get< 0 >( cards ) ),
        fCard2( std::get< 1 >( cards ) ),
        fCard3( std::get< 2 >( cards ) )
    {
        fIsFlush = NHandUtils::isFlush( fCard1, fCard2 ) && NHandUtils::isFlush( fCard1, fCard3 );
        fIsPair = NHandUtils::isCount( std::vector< TCard >( { fCard1, fCard2, fCard3 } ), 2 );
        fIsThreeOfAKind = NHandUtils::isCount( std::vector< TCard >( { fCard1, fCard2, fCard3 } ), 3 );
        fIsStraight = NHandUtils::isStraight( std::vector< TCard >( { fCard1, fCard2, fCard3 } ) );

        auto sortedCards = std::vector< TCard >( { std::get< 0 >( cards ), std::get< 1 >( cards ),std::get< 2 >( cards ) } );
        std::sort( sortedCards.begin(), sortedCards.end(), []( TCard lhs, TCard rhs ) { return lhs.first > rhs.first; } );

        fCards.push_back( sortedCards[ 0 ].first );
        fKickers.push_back( sortedCards[ 1 ].first );
        fKickers.push_back( sortedCards[ 2 ].first );
        if ( ( *(fCards.begin()) == ECard::eAce ) && ( *(fKickers.begin()) == ECard::eTrey ) )
            std::swap( *(fCards.begin()), *(fKickers.begin()) );

        if ( fIsPair )
        {
            fCards.clear();
            fKickers.clear();
            std::map< ECard, uint8_t > cardHits;
            for ( auto && card : sortedCards )
            {
                cardHits[ card.first ]++;
            }

            for ( auto&& ii : cardHits )
            {
                if ( ii.second > 1 )
                    fCards = { ii.first };
                else
                    fKickers.push_back( ii.first );
            }
        }

        fBitValues.resize( 3 );
        fBitValues[ 0 ] = NHandUtils::computeBitValue( fCard1.first, fCard1.second );
        fBitValues[ 1 ] = NHandUtils::computeBitValue( fCard2.first, fCard2.second );
        fBitValues[ 2 ] = NHandUtils::computeBitValue( fCard3.first, fCard3.second );
    }

    bool S3CardInfo::isWheel() const
    {
        return ( *(fCards.begin()) == ECard::eAce ) && ( *(fKickers.begin()) == ECard::eDeuce ) && ( *(fKickers.rbegin()) == ECard::eTrey );
    }

    // straights and flushes dont count
    bool S3CardInfo::compareJustCards( bool flushStraightCount, const S3CardInfo& rhs ) const
    {
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return *(fCards.begin()) < *(rhs.fCards.begin());
        if ( !fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return true;
        if ( fIsThreeOfAKind && !rhs.fIsThreeOfAKind )
            return false;

        if ( fIsPair && rhs.fIsPair )
        {
            if (*(fCards.begin()) < *(rhs.fCards.begin()))
                return true;
            if ( *( fCards.begin() ) > *( rhs.fCards.begin() ) )
                return false;
            // pair of the same value, now compare the kicker
            return *( fKickers.begin() ) < *( rhs.fKickers.begin() );
        }
        if ( !fIsPair && rhs.fIsPair )
            return true;
        if ( fIsPair && !rhs.fIsPair )
            return false;

        if ( flushStraightCount )
        {
            // worry about ace duece
        }
        auto ii = fCards.begin();
        auto jj = rhs.fCards.begin();
        for( ; ( ii != fCards.end() ) && ( jj != rhs.fCards.end() ); ++jj )
        {
            if ( *ii == *jj )
                continue;
            return *ii < *jj;
        }
        if ( ii != fCards.end() && jj == rhs.fCards.end() )
            return true;
        if ( ii == fCards.end() && jj != rhs.fCards.end() )
            return false;
        // both at end or both not at end
        ii = fKickers.begin();
        jj = rhs.fKickers.begin();
        for ( ; ( ii != fKickers.end() ) && ( jj != rhs.fKickers.end() ); ++jj )
        {
            if ( *ii == *jj )
                continue;
            return *ii < *jj;
        }
        if ( ii != fKickers.end() && jj == rhs.fKickers.end() )
            return true;
        if ( ii == fKickers.end() && jj != rhs.fKickers.end() )
            return false;

        return false;
    }

    bool S3CardInfo::lessThan( bool flushStraightCount, const S3CardInfo& rhs ) const
    {
        if ( !flushStraightCount || ( fIsPair || rhs.fIsPair || fIsThreeOfAKind || rhs.fIsThreeOfAKind ) )
            return compareJustCards( flushStraightCount, rhs );


        // straights are higher than flushes in 2, 3 and 4 card (need to compute for 4 card...)
        if ( fIsStraight && rhs.fIsStraight )
        {
            // cant be a pair.. so check for flush only
            if ( fIsFlush && !rhs.fIsFlush ) // im a straight flush -> he isnt he is bigger
                return false;
            if ( !fIsFlush && rhs.fIsFlush )
                return true;

            // if both a flush (then its a straight flush) or both not a flush, its which highcard is bigger
            return compareJustCards( flushStraightCount, rhs ); // which straight is bigger
        }

        if ( fIsStraight && !rhs.fIsStraight )
            return false;
        if ( !fIsStraight && rhs.fIsStraight )
            return true;

        // neigher side is a straight
        // check for flush
        if ( fIsFlush && rhs.fIsFlush )
            return compareJustCards( flushStraightCount, rhs );
        if ( fIsFlush && !rhs.fIsFlush ) // im a straight flush -> he isnt he is bigger
            return false;
        if ( !fIsFlush && rhs.fIsFlush )
            return true;

        return compareJustCards( flushStraightCount, rhs );
    }

    bool S3CardInfo::equalTo( bool flushStraightCount, const S3CardInfo& rhs ) const
    {
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return *(fCards.begin()) == *(rhs.fCards.begin());
        if ( fIsPair && rhs.fIsPair )
            return ( *( fCards.begin() ) == *( rhs.fCards.begin() ) ) && ( *( fKickers.begin() ) == *( rhs.fKickers.begin() ) );

        if ( flushStraightCount )
        {
            if ( fIsFlush != rhs.fIsFlush )
                return false;
            if ( fIsStraight != rhs.fIsStraight )
                return false;
        }
        return ( *(fCards.begin()) == *(rhs.fCards.begin()) ) && ( *(fKickers.begin()) == *(rhs.fKickers.begin()) ) && ( *(fKickers.rbegin()) == *(rhs.fKickers.rbegin()) );
    }

    uint16_t S3CardInfo::getCardsValue() const
    {
        return NHandUtils::getCardsValue( fBitValues );
    }
    
    uint64_t S3CardInfo::handProduct() const
    {
        return NHandUtils::computeHandProduct( fBitValues );
    }

    std::ostream& operator<<( std::ostream& oss, const S3CardInfo& obj )
    {
        oss << obj.fCard1.first << obj.fCard1.second
            << " "
            << obj.fCard2.first << obj.fCard2.second
            << " "
            << obj.fCard3.first << obj.fCard3.second
            ;
        return oss;
    }

    // No Flushes/Straights
    static std::map< S3CardInfo::THand, uint32_t > s3CardMap =
    {
         { { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 1 } // trips
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 2 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 3 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 4 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 5 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 6 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 7 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 8 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 9 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 10 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 11 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 12 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 13 } // trips

        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 14 } // pairs
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 15 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 16 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 17 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 18 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 19 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 20 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 21 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 22 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 23 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 24 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 25 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 26 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 27 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 28 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 29 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 30 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 31 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 32 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 33 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 34 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 35 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 36 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 37 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 38 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 39 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 40 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 41 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 42 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 43 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 44 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 45 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 46 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 47 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 48 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 49 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 50 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 51 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 52 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 53 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 54 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 55 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 56 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 57 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 58 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 59 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 60 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 61 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 62 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 63 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 64 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 65 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 66 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 67 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 68 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 69 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 70 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 71 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 72 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 73 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 74 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 75 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 76 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 77 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 78 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 79 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 80 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 81 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 82 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 83 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 84 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 85 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 86 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 87 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 88 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 89 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 90 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 91 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 92 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 93 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 94 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 95 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 96 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 97 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 98 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 99 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 100 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 101 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 102 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 103 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 104 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 105 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 106 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 107 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 108 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 109 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 110 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 111 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 112 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 113 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 114 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 115 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 116 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 117 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 118 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 119 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 120 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 121 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 122 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 123 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 124 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 125 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 126 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 127 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 128 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 129 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 130 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 131 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 132 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 133 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 134 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 135 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 136 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 137 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 138 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 139 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 140 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 141 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 142 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 143 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 144 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 145 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 146 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 147 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 148 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 149 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 150 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 151 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 152 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 153 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 155 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 156 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 157 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 158 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 159 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 160 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 161 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 162 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 163 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 164 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 165 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 166 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 168 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 169 } // pairs

        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 170 } // high card
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 171 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 172 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 173 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 174 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 175 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 176 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 177 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 178 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 179 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 180 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 181 } // high card
    };

    // Flushes/Straights
    static std::map< S3CardInfo::THand, uint32_t > s3CardMapStraightsAndFlushesCount =
    {
         { { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 1 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 2 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 3 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 4 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 5 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 6 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 7 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 8 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 9 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 10 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 11 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 12 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 13 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 14 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 15 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 16 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 17 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 18 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 19 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 20 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 21 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 22 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 23 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 24 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 25 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 26 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 27 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 28 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 29 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 30 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 31 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 32 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 33 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 34 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 35 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 36 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 37 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 38 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 39 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 40 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 41 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 42 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 43 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 44 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 45 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 46 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 47 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 48 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 49 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 50 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 51 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 52 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 53 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 54 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 55 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 56 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 57 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 58 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 59 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 60 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 61 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 62 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 63 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 64 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 65 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 66 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 67 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 68 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 69 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 70 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 71 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 72 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 73 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 74 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 75 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 76 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 77 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 78 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 79 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 80 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 81 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 82 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 83 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 84 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 85 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 86 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 87 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 88 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 89 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 90 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 91 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 92 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 93 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 94 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 95 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 96 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 97 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 98 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 99 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 100 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 101 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 102 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 103 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 104 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 105 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 106 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 107 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 108 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 109 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 110 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 111 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 112 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 113 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 114 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 115 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 116 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 117 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 118 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 119 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 120 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 121 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 122 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 123 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 124 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 125 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 126 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 127 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 128 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 129 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 130 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 131 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 132 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 133 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 134 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 135 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 136 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 137 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 138 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 139 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 140 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 141 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 142 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 143 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 144 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 145 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 146 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 147 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 148 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 149 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 150 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 151 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 152 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 153 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 155 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 156 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 157 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 158 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 159 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 160 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 161 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 162 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 163 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 164 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 165 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 166 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 168 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 169 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 170 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 171 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 172 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 173 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 174 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 175 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 176 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 177 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 178 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 179 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 180 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 181 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 182 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 183 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 184 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 185 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 186 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 187 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 188 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 189 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 190 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 191 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 192 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 193 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 194 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 195 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 196 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 197 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 198 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 199 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 200 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 201 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 202 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 203 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 204 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 205 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 206 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 207 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 208 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 209 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 210 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 211 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 212 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 213 }
    };

    static std::vector< uint32_t > sFlushes =
    {
        0, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 203, 0, 203, 179, 0, 0, 0, 0, 202,
        0, 202, 202, 0, 0, 202, 202, 0, 178, 0, 0, 0, 0, 0, 0, 201, 0, 201,
        201, 0, 0, 201, 201, 0, 201, 0, 0, 0, 0, 201, 201, 0, 201, 0, 0, 0,
        177, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 200, 0, 0, 200, 200,
        0, 200, 0, 0, 0, 0, 200, 200, 0, 200, 0, 0, 0, 200, 0, 0, 0, 0, 0,
        0, 0, 0, 200, 200, 0, 200, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 176,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 199, 0, 199,
        199, 0, 0, 199, 199, 0, 199, 0, 0, 0, 0, 199, 199, 0, 199, 0, 0, 0,
        199, 0, 0, 0, 0, 0, 0, 0, 0, 199, 199, 0, 199, 0, 0, 0, 199, 0, 0,
        0, 0, 0, 0, 0, 199, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        199, 199, 0, 199, 0, 0, 0, 199, 0, 0, 0, 0, 0, 0, 0, 199, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 198, 0, 198, 198, 0, 0, 198, 198, 0, 198, 0, 0, 0, 0, 198,
        198, 0, 198, 0, 0, 0, 198, 0, 0, 0, 0, 0, 0, 0, 0, 198, 198, 0, 198,
        0, 0, 0, 198, 0, 0, 0, 0, 0, 0, 0, 198, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 198, 198, 0, 198, 0, 0, 0, 198, 0, 0, 0, 0, 0,
        0, 0, 198, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 198, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 198, 198, 0, 198, 0, 0, 0, 198, 0, 0, 0, 0,
        0, 0, 0, 198, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 198, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 0, 197, 197, 0, 0, 197, 197, 0, 197,
        0, 0, 0, 0, 197, 197, 0, 197, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0,
        197, 197, 0, 197, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 197, 0, 197, 0, 0, 0,
        197, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 197, 0, 197, 0, 0,
        0, 197, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 197, 0, 197, 0, 0, 0,
        197, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 0, 196, 196, 0, 0, 196, 196, 0,
        196, 0, 0, 0, 0, 196, 196, 0, 196, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0,
        0, 0, 196, 196, 0, 196, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 196, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 196, 0, 196, 0,
        0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 196, 0, 196,
        0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 196, 0, 196, 0,
        0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 196, 0, 196, 0, 0, 0, 196, 0, 0,
        0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0,
        195, 195, 0, 0, 195, 195, 0, 195, 0, 0, 0, 0, 195, 195, 0, 195, 0,
        0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 195, 195, 0, 195, 0, 0, 0, 195,
        0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 195, 195, 0, 195, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 195, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 195, 195, 0, 195, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 195,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 195, 195, 0, 195, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 195, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 195,
        0, 195, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 195, 195, 0, 195, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0,
        0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        195, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 181, 0, 194, 194, 0, 0, 194, 194, 0, 194, 0, 0, 0, 0, 194, 194,
        0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 194, 194, 0, 194, 0,
        0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0,
        0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0,
        0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0,
        0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0,
        0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 194, 194, 0, 194, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 194, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170
    };

    static std::vector< uint32_t > sHighCardUnique =
    {
        0, 13, 12, 0, 11, 0, 0, 180, 10, 0, 0, 179, 0, 179, 179, 0, 9, 0,
        0, 178, 0, 178, 178, 0, 0, 178, 178, 0, 178, 0, 0, 0, 8, 0, 0, 177,
        0, 177, 177, 0, 0, 177, 177, 0, 177, 0, 0, 0, 0, 177, 177, 0, 177,
        0, 0, 0, 177, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 176, 0, 176, 176, 0, 0,
        176, 176, 0, 176, 0, 0, 0, 0, 176, 176, 0, 176, 0, 0, 0, 176, 0, 0,
        0, 0, 0, 0, 0, 0, 176, 176, 0, 176, 0, 0, 0, 176, 0, 0, 0, 0, 0, 0,
        0, 176, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 175,
        0, 175, 175, 0, 0, 175, 175, 0, 175, 0, 0, 0, 0, 175, 175, 0, 175,
        0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 0, 175, 175, 0, 175, 0, 0, 0, 175,
        0, 0, 0, 0, 0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 175, 175, 0, 175, 0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 175, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 0, 0, 174, 0, 174, 174, 0, 0, 174, 174, 0, 174, 0, 0, 0,
        0, 174, 174, 0, 174, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 174, 174,
        0, 174, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174, 174, 0, 174, 0, 0, 0, 174, 0, 0,
        0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174, 174, 0, 174, 0, 0, 0, 174, 0, 0,
        0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 173, 0, 173, 173, 0, 0, 173, 173, 0,
        173, 0, 0, 0, 0, 173, 173, 0, 173, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0,
        0, 0, 173, 173, 0, 173, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 173, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 173, 0, 173, 0,
        0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 173, 0, 173,
        0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 173, 0, 173, 0,
        0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 172, 0, 172, 172, 0, 0, 172, 172,
        0, 172, 0, 0, 0, 0, 172, 172, 0, 172, 0, 0, 0, 172, 0, 0, 0, 0, 0,
        0, 0, 0, 172, 172, 0, 172, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 172,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 172, 0, 172,
        0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 172, 0, 172,
        0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 172, 0, 172, 0,
        0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 172, 0, 172, 0, 0, 0, 172, 0, 0,
        0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 171, 0,
        171, 171, 0, 0, 171, 171, 0, 171, 0, 0, 0, 0, 171, 171, 0, 171, 0,
        0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 171, 171, 0, 171, 0, 0, 0, 171,
        0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 171, 171, 0, 171, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 171, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 171, 171, 0, 171, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 171,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 171, 171, 0, 171, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 171, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 171,
        0, 171, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 171, 171, 0, 171, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0,
        0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 181, 0, 170, 170, 0, 0, 170, 170, 0, 170, 0, 0, 0, 0, 170, 170,
        0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 170, 170, 0, 170, 0,
        0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0,
        0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0,
        0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0,
        0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0,
        0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 170, 170, 0, 170, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 170, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170
    };

    static std::vector< uint32_t > sStraightsUnique =
    {
        0, 13, 12, 0, 11, 0, 0, 192, 10, 0, 0, 213, 0, 213, 191, 0, 9, 0,
        0, 212, 0, 212, 212, 0, 0, 212, 212, 0, 190, 0, 0, 0, 8, 0, 0, 211,
        0, 211, 211, 0, 0, 211, 211, 0, 211, 0, 0, 0, 0, 211, 211, 0, 211,
        0, 0, 0, 189, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 210, 0, 210, 210, 0, 0,
        210, 210, 0, 210, 0, 0, 0, 0, 210, 210, 0, 210, 0, 0, 0, 210, 0, 0,
        0, 0, 0, 0, 0, 0, 210, 210, 0, 210, 0, 0, 0, 210, 0, 0, 0, 0, 0, 0,
        0, 188, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 209,
        0, 209, 209, 0, 0, 209, 209, 0, 209, 0, 0, 0, 0, 209, 209, 0, 209,
        0, 0, 0, 209, 0, 0, 0, 0, 0, 0, 0, 0, 209, 209, 0, 209, 0, 0, 0, 209,
        0, 0, 0, 0, 0, 0, 0, 209, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 209, 209, 0, 209, 0, 0, 0, 209, 0, 0, 0, 0, 0, 0, 0, 209, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 187, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 0, 0, 208, 0, 208, 208, 0, 0, 208, 208, 0, 208, 0, 0, 0,
        0, 208, 208, 0, 208, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 208, 208,
        0, 208, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 208, 208, 0, 208, 0, 0, 0, 208, 0, 0,
        0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 208,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 208, 208, 0, 208, 0, 0, 0, 208, 0, 0,
        0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 208,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 207, 0, 207, 207, 0, 0, 207, 207, 0,
        207, 0, 0, 0, 0, 207, 207, 0, 207, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0,
        0, 0, 207, 207, 0, 207, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 207, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 207, 0, 207, 0,
        0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 207, 0, 207,
        0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 207, 0, 207, 0,
        0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 185, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 206, 0, 206, 206, 0, 0, 206, 206,
        0, 206, 0, 0, 0, 0, 206, 206, 0, 206, 0, 0, 0, 206, 0, 0, 0, 0, 0,
        0, 0, 0, 206, 206, 0, 206, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 206,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 206, 0, 206,
        0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 206, 0, 206,
        0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 206, 0, 206, 0,
        0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 206, 0, 206, 0, 0, 0, 206, 0, 0,
        0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 184, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 205, 0,
        205, 205, 0, 0, 205, 205, 0, 205, 0, 0, 0, 0, 205, 205, 0, 205, 0,
        0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 205, 205, 0, 205, 0, 0, 0, 205,
        0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 205, 205, 0, 205, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 205, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 205, 205, 0, 205, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 205,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 205, 205, 0, 205, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 205, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 205,
        0, 205, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 205, 205, 0, 205, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0,
        0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 205, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 193, 0, 204, 204, 0, 0, 204, 204, 0, 204, 0, 0, 0, 0, 204, 204,
        0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 204, 204, 0, 204, 0,
        0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0,
        0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0,
        0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0,
        0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0,
        0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 204, 204, 0, 204, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 204, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 182
    };

    static std::unordered_map< int64_t, int16_t > sProductMap =
    {
         { 12, 3 }
        ,{ 18, 3 }
        ,{ 20, 5 }
        ,{ 28, 9 }
        ,{ 44, 17 }
        ,{ 45, 6 }
        ,{ 50, 5 }
        ,{ 52, 33 }
        ,{ 63, 10 }
        ,{ 68, 65 }
        ,{ 75, 6 }
        ,{ 76, 129 }
        ,{ 92, 257 }
        ,{ 98, 9 }
        ,{ 99, 18 }
        ,{ 116, 513 }
        ,{ 117, 34 }
        ,{ 124, 1025 }
        ,{ 147, 10 }
        ,{ 148, 2049 }
        ,{ 153, 66 }
        ,{ 164, 4097 }
        ,{ 171, 130 }
        ,{ 175, 12 }
        ,{ 207, 258 }
        ,{ 242, 17 }
        ,{ 245, 12 }
        ,{ 261, 514 }
        ,{ 275, 20 }
        ,{ 279, 1026 }
        ,{ 325, 36 }
        ,{ 333, 2050 }
        ,{ 338, 33 }
        ,{ 363, 18 }
        ,{ 369, 4098 }
        ,{ 425, 68 }
        ,{ 475, 132 }
        ,{ 507, 34 }
        ,{ 539, 24 }
        ,{ 575, 260 }
        ,{ 578, 65 }
        ,{ 605, 20 }
        ,{ 637, 40 }
        ,{ 722, 129 }
        ,{ 725, 516 }
        ,{ 775, 1028 }
        ,{ 833, 72 }
        ,{ 845, 36 }
        ,{ 847, 24 }
        ,{ 867, 66 }
        ,{ 925, 2052 }
        ,{ 931, 136 }
        ,{ 1025, 4100 }
        ,{ 1058, 257 }
        ,{ 1083, 130 }
        ,{ 1127, 264 }
        ,{ 1183, 40 }
        ,{ 1421, 520 }
        ,{ 1445, 68 }
        ,{ 1519, 1032 }
        ,{ 1573, 48 }
        ,{ 1587, 258 }
        ,{ 1682, 513 }
        ,{ 1805, 132 }
        ,{ 1813, 2056 }
        ,{ 1859, 48 }
        ,{ 1922, 1025 }
        ,{ 2009, 4104 }
        ,{ 2023, 72 }
        ,{ 2057, 80 }
        ,{ 2299, 144 }
        ,{ 2523, 514 }
        ,{ 2527, 136 }
        ,{ 2645, 260 }
        ,{ 2738, 2049 }
        ,{ 2783, 272 }
        ,{ 2873, 96 }
        ,{ 2883, 1026 }
        ,{ 3179, 80 }
        ,{ 3211, 160 }
        ,{ 3362, 4097 }
        ,{ 3509, 528 }
        ,{ 3703, 264 }
        ,{ 3751, 1040 }
        ,{ 3757, 96 }
        ,{ 3887, 288 }
        ,{ 3971, 144 }
        ,{ 4107, 2050 }
        ,{ 4205, 516 }
        ,{ 4477, 2064 }
        ,{ 4693, 160 }
        ,{ 4805, 1028 }
        ,{ 4901, 544 }
        ,{ 4961, 4112 }
        ,{ 5043, 4098 }
        ,{ 5239, 1056 }
        ,{ 5491, 192 }
        ,{ 5819, 272 }
        ,{ 5887, 520 }
        ,{ 6137, 192 }
        ,{ 6253, 2080 }
        ,{ 6647, 320 }
        ,{ 6727, 1032 }
        ,{ 6845, 2052 }
        ,{ 6877, 288 }
        ,{ 6929, 4128 }
        ,{ 8303, 384 }
        ,{ 8381, 576 }
        ,{ 8405, 4100 }
        ,{ 8959, 1088 }
        ,{ 8993, 320 }
        ,{ 9251, 528 }
        ,{ 9583, 2056 }
        ,{ 10051, 384 }
        ,{ 10469, 640 }
        ,{ 10571, 1040 }
        ,{ 10693, 2112 }
        ,{ 10933, 544 }
        ,{ 11191, 1152 }
        ,{ 11767, 4104 }
        ,{ 11849, 4160 }
        ,{ 12493, 1056 }
        ,{ 13357, 2176 }
        ,{ 14297, 576 }
        ,{ 14801, 4224 }
        ,{ 15059, 2064 }
        ,{ 15341, 768 }
        ,{ 15979, 640 }
        ,{ 16337, 1088 }
        ,{ 16399, 1280 }
        ,{ 17797, 2080 }
        ,{ 18259, 1152 }
        ,{ 18491, 4112 }
        ,{ 19343, 768 }
        ,{ 19573, 2304 }
        ,{ 21689, 4352 }
        ,{ 21853, 4128 }
        ,{ 22103, 1280 }
        ,{ 23273, 2112 }
        ,{ 26011, 2176 }
        ,{ 26071, 1536 }
        ,{ 27869, 1536 }
        ,{ 28577, 4160 }
        ,{ 31117, 2560 }
        ,{ 31487, 2304 }
        ,{ 31939, 4224 }
        ,{ 34481, 4608 }
        ,{ 35557, 3072 }
        ,{ 38663, 4352 }
        ,{ 39401, 5120 }
        ,{ 39701, 2560 }
        ,{ 42439, 3072 }
        ,{ 48749, 4608 }
        ,{ 52111, 5120 }
        ,{ 56129, 6144 }
        ,{ 62197, 6144 }
    };

    static void dumpMap( std::ostream& oss, const std::map< uint64_t, uint16_t > & values, const std::string& varName )
    {
        oss
            << "    " << "static std::unordered_map< int64_t, int16_t > " << varName << " = \n"
            << "    {\n"
            ;
        bool first = true;
        for( auto && ii : values )
        {
            oss << "        ";

            if ( !first )
                oss << ",";
            else
                oss << " ";
            first = false;
            oss << "{ " << ii.first << ", " << ii.second << " }\n";
        }
        oss << "    };\n\n";
    }

    static void dumpTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string & varName )
    {
        oss 
            << "    " << "static std::vector< uint32_t > " << varName << " = \n"
            << "    {\n"
            ;
        size_t numChars = 0;
        for( size_t ii = 0; ii < values.size(); ++ii )
        {
            if ( numChars == 0 )
                oss << "        ";

            auto tmp = std::to_string( values[ ii ] );
            oss << tmp;
            numChars += tmp.size();
            if ( ii != ( values.size() -1 ) )
            {
                oss << ", ";
                numChars += 2;
            }

            if ( numChars >= 66 )
            {
                numChars = 0;
                oss << "\n";
            }
        }
        oss << "\n    };\n\n";
    }

    template< typename T>
    void computeAndDumpMap( std::ostream & oss, T & map, const std::string & varName, const std::string & header )
    {
        int num = 1;
        //std::optional< S3CardInfo > prev;
        for ( auto&& ii : map )
        {
            //if ( prev.has_value() && ( *prev->equalTo( ii.first ) ) )
            //    num++;
            //prev = ii.first;
            ii.second = num++;
        }

        oss
            << "    // " << header << "\n"
            << "    static std::map< S3CardInfo::THand, uint32_t > " << varName << " = \n"
            << "    {\n"
            ;

        bool first = true;
        for ( auto&& ii : map )
        {
            oss << "        ";
            if ( first )
                oss << " ";
            else
                oss << ",";
            first = false;
            auto firstCard = ii.first.fCard1;
            auto secondCard = ii.first.fCard2;
            auto thirdCard = ii.first.fCard3;
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );
            oss << "{ { "
                     << "{ " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }, "
                     << "{ " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " }, "
                     << "{ " << qPrintable( ::asEnum( thirdCard.first ) ) << ", " << qPrintable( ::asEnum( thirdCard.second ) ) << " } "
                     << "}, " << ii.second << " }\n";
        }
        oss << "    };\n\n";
    }

    uint32_t evaluate3CardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        using TCardToInfoMap = std::map< S3CardInfo::THand, S3CardInfo >;
        auto gFlushStraightsCount = []( const S3CardInfo& lhs, const S3CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
        auto gJustCardsCount = []( const S3CardInfo& lhs, const S3CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

        using TCardsCountMap = std::map< S3CardInfo, uint32_t, decltype( gJustCardsCount ) >;
        using TFlushesCountMap = std::map< S3CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

        static TCardToInfoMap allHands;
        static TCardsCountMap justCardsCount( gJustCardsCount );
        static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

        if ( NHandUtils::gComputeAllHands && allHands.empty() )
        {
            std::ofstream ofs( "3CardDump.cpp" );
            std::ostream* oss = &ofs; //&std::cout;

            size_t maxCardsValue = 0;

            auto&& allCards = CCard::allCardsList();
            for ( auto&& c1 : allCards )
            {
                for ( auto&& c2 : allCards )
                {
                    if ( *c1 == *c2 )
                        continue;

                    for ( auto&& c3 : allCards )
                    {
                        if ( *c1 == *c3 )
                            continue;
                        if ( *c1 == *c2 )
                            continue;
                        if ( *c2 == *c3 )
                            continue;

                        auto curr = S3CardInfo::THand( TCard( c1->getCard(), c1->getSuit() ), TCard( c2->getCard(), c2->getSuit() ), TCard( c3->getCard(), c3->getSuit() ) );
                        S3CardInfo cardInfo( curr );
                        allHands[ curr ] = cardInfo;
                        justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                        flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                        maxCardsValue = std::max( static_cast<size_t>( cardInfo.getCardsValue() ), maxCardsValue );
                    }
                }
            }

            computeAndDumpMap( *oss, justCardsCount, "s3CardMap", "No Flushes/Straights" );
            computeAndDumpMap( *oss, flushesAndStraightsCount, "s3CardMapStraightsAndFlushesCount", "Flushes/Straights" );

            std::vector< uint32_t > flushes;
            flushes.resize( maxCardsValue + 1 );

            std::vector< uint32_t > highCardUnique;
            highCardUnique.resize( maxCardsValue + 1 );

            std::vector< uint32_t > straightsUnique;
            straightsUnique.resize( maxCardsValue + 1 );

            std::map< uint64_t, uint16_t > productMap;

            for( auto && ii : allHands )
            {
                auto cardValue = ii.second.getCardsValue();
                if ( ii.second.fIsFlush )
                {
                    auto pos = flushesAndStraightsCount.find( ii.second );
                    if ( pos == flushesAndStraightsCount.end() )
                        Q_ASSERT( pos != flushesAndStraightsCount.end() );

                    flushes[ cardValue ] = ( *pos ).second;
                }
                else if ( !ii.second.fIsPair )
                {
                    // unique cards
                    auto pos = flushesAndStraightsCount.find( ii.second );
                    Q_ASSERT( pos != flushesAndStraightsCount.end() );

                    straightsUnique[ cardValue ] = ( *pos ).second;

                    auto pos2 = justCardsCount.find( ii.second );
                    Q_ASSERT( pos2 != justCardsCount.end() );
                    highCardUnique[ cardValue ] = ( *pos2 ).second;
                }
                else
                {
                    auto productValue = ii.second.handProduct();
                    productMap[ productValue ] = cardValue;
                }
            }
            dumpTable( *oss, flushes, "sFlushes" );
            dumpTable( *oss, highCardUnique, "sHighCardUnique" );
            dumpTable( *oss, straightsUnique, "sStraightsUnique" );
            dumpMap( *oss, productMap, "sProductMap" );
        }
    
        if ( cards.size() != 3 )
            return -1;
        //(void)playInfo;

        auto cardsValue = getCardsValue( cards );
        if ( playInfo && playInfo->fStraightsFlushesCountForSmallHands )
        {
            if ( isFlush( cards ) )
                return sFlushes[ cardsValue ];
        }

        auto straightOrHighCard = playInfo->fStraightsFlushesCountForSmallHands ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];
        if ( straightOrHighCard )
            return straightOrHighCard;

        auto product = computeHandProduct( cards );
        auto pos = sProductMap.find( product );
        if ( pos == sProductMap.end() )
            return -1;
        return (*pos).second;
        //return -1;
    }

    EHand rankTo3CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        EHand hand;
        if ( !playInfo->fStraightsFlushesCountForSmallHands )
        {
            if ( rank > 169U )
                hand = EHand::eHighCard;
            else if ( rank > 13U )
                hand = EHand::ePair;
            else
                hand = EHand::eThreeOfAKind;
        }
        else
        {
            if ( rank > 104U )
                hand = EHand::eHighCard;
            else if ( rank > 39U )
                hand = EHand::eFlush;
            else if ( rank > 26U )
                hand = EHand::eStraight;
            else if ( rank > 13U )
                hand = EHand::eStraightFlush;
            else
                hand = EHand::ePair;
        }
        return hand;
    }

}