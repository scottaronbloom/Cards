#ifndef __EVALUATE2CARDHAND_H
#define __EVALUATE2CARDHAND_H

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

#include "Card.h"
#include <memory>
enum class EHand;
struct SPlayInfo;
namespace std
{
    template <>
    struct hash< std::pair< std::pair< ESuit, ECard >, std::pair< ESuit, ECard > > >
    {
        std::size_t operator()( const std::pair< std::pair< ESuit, ECard >, std::pair< ESuit, ECard > >& k ) const
        {
            auto v1 = std::hash<uint8_t>()( static_cast<uint8_t>( k.first.first ) );
            auto v2 = std::hash<int>()( static_cast<uint8_t>( k.first.second ) );
            auto v3 = std::hash<uint8_t>()( static_cast<uint8_t>( k.second.first ) );
            auto v4 = std::hash<int>()( static_cast<uint8_t>( k.second.second ) );

            auto retVal = ( ( ( ( ( v1 << 1 ) ^ ( v2 << 1 ) ) >> 1 ) ^ ( v3 << 1 ) ) >> 1 ) ^ ( v4 << 1 );
            return retVal;
        }
    };
}

namespace NHandUtils
{
    struct S2CardInfo
    {
        S2CardInfo();
        S2CardInfo( const std::pair< std::pair< ECard, ESuit >, std::pair< ECard, ESuit > >& cards );
        S2CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2 );

        bool compareJustCards( bool flushStraightCount, const S2CardInfo& rhs ) const;
        uint16_t getCardsValue() const;
        uint64_t handProduct() const;
        // 2 card ranking
        // pair A->2
        // straight AK -> A2
        // flush AK -> A2
        // high card -> AK -> A2
        bool lessThan( bool flushStraightCount, const S2CardInfo& rhs ) const;
        bool greaterThan( bool flushStraightCount, const S2CardInfo& rhs ) const
        {
            return !lessThan( flushStraightCount, rhs ) && !equalTo( flushStraightCount, rhs );
        }
        bool equalTo( bool flushStraightCount, const S2CardInfo& rhs ) const;

        bool isAceTwo() const{ return ( fHighCard == ECard::eAce ) && ( fKicker == ECard::eDeuce ); }

        bool fIsFlush{ false };
        bool fIsStraight{ false };
        bool fIsPair{ false };
        ECard fHighCard{ ECard::eUNKNOWN };
        ECard fKicker{ ECard::eUNKNOWN };
        std::pair< ECard, ESuit > fCard1;
        std::pair< ECard, ESuit > fCard2;

        std::vector< TCardBitType > fBitValues;
    };
    std::ostream& operator<<( std::ostream& oss, const S2CardInfo& obj );

    uint32_t evaluate2CardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo );
    EHand rankTo2CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo );
}

#endif