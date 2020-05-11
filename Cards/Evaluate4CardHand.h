#ifndef __EVALUATE4CARDHAND_H
#define __EVALUATE4CARDHAND_H

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
#include "HandUtils.h"
#include <memory>
enum class EHand;
struct SPlayInfo;

namespace NHandUtils
{
    struct S4CardInfo
    {
        using THand = std::tuple< TCard, TCard, TCard, TCard >;
        S4CardInfo();
        S4CardInfo( const THand& cards );
        S4CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4 );

        bool compareJustCards( bool flushStraightCount, const S4CardInfo& rhs ) const;
        uint16_t getCardsValue() const;
        uint64_t handProduct() const;
        // 2 card ranking
        // pair A->2
        // straight AK -> A2
        // flush AK -> A2
        // high card -> AK -> A2
        bool lessThan( bool flushStraightCount, const S4CardInfo& rhs ) const;
        bool greaterThan( bool flushStraightCount, const S4CardInfo& rhs ) const
        {
            return !lessThan( flushStraightCount, rhs ) && !equalTo( flushStraightCount, rhs );
        }
        bool equalTo( bool flushStraightCount, const S4CardInfo& rhs ) const;

        bool isWheel() const;

        bool fIsFlush{ false };
        bool fIsStraight{ false };
        bool fIsPair{ false };
        std::list< ECard > fCards;
        std::list< ECard > fKickers;
        TCard fCard1;
        TCard fCard2;
        TCard fCard3;
        TCard fCard4;

        std::vector< TCardBitType > fBitValues;
    };
    std::ostream& operator<<( std::ostream& oss, const S4CardInfo& obj );

    uint32_t evaluate4CardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo );
    EHand rankTo4CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo );
}

#endif