#ifndef __CARDINFO_H
#define __CARDINFO_H

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
#include <map>
#include <iostream>
#include "SABUtils/utils.h"
enum class EHand;
struct SPlayInfo;

namespace NHandUtils
{
    struct SCardInfoData
    {
        SCardInfoData()
        {
            fCardMaps.resize( 4 );
            fUniqueVectors.resize( 4 );
            fProductMaps.resize( 4 );
        }
        enum EWhichItem
        {
            eStraightsAndFlushesDontCountLowBall = 0,
            eStraightsAndFlushesCountLowBall = 1,
            eStraightsAndFlushesDontCount =2,
            eStraightsAndFlushesCount = 3
        };

        EWhichItem getWhichItem( bool straightsAndFlushesCount, bool lowHandWins ) const
        {
            auto whichItem =
                straightsAndFlushesCount
                ? ( lowHandWins ? EWhichItem::eStraightsAndFlushesCountLowBall : EWhichItem::eStraightsAndFlushesCount )
                : ( lowHandWins ? EWhichItem::eStraightsAndFlushesDontCountLowBall : EWhichItem::eStraightsAndFlushesDontCount )
                ;
            return whichItem;
        }
        const std::vector< uint32_t > & getUniqueVector( bool straightsAndFlushesCount, bool lowHandWins ) const;
        const std::unordered_map< int64_t, int16_t > & getProductMap( bool straightsAndFlushesCount, bool lowHandWins ) const;
        const std::map< THand, uint32_t >& getCardMap( bool straightsAndFlushesCount, bool lowHandWins ) const;

        std::vector< std::vector< uint32_t > > fUniqueVectors;
        std::vector< std::map< THand, uint32_t > > fCardMaps;
        std::vector< std::unordered_map< int64_t, int16_t > > fProductMaps;
        std::vector< uint32_t > fFlushes;
        bool fAllHandsComputed{false};
        bool fTablesInitialized{false};
    };

    class CCardInfo
    {
    public:
        CCardInfo();

        static std::shared_ptr< CCardInfo > createCardInfo( const THand & card );

        virtual uint16_t getCardsValue() const;
        virtual uint64_t handProduct() const;

        virtual bool lessThan( bool straightsAndFlushesCount, bool lowHandWins, const CCardInfo& rhs ) const;
        virtual bool greaterThan( bool straightsAndFlushesCount, bool lowHandWins, const CCardInfo& rhs ) const final;
        virtual bool equalTo( bool straightsAndFlushesCount, const CCardInfo& rhs ) const;

        virtual void generateAllCardHands();
        virtual bool allHandsComputed() const=0;
        virtual void setAllHandsComputed( bool computed ) = 0;
        virtual size_t getNumCards() const = 0;

        bool operator<( const CCardInfo & rhs ) const { return lessThan( true, false, rhs ); }
        bool operator>( const CCardInfo& rhs ) const { return greaterThan( true, false, rhs ); }
        bool operator==( const CCardInfo& rhs ) const { return equalTo( true, rhs ); }

        const THand & origCards() const{ return fOrigCards; }

        const std::optional< EStraightType > & straightType() const{ return fStraightType; }

        virtual bool isFiveOfAKind() const final { return fIsFiveOfAKind; }
        virtual bool isStraightFlush() const final { return fStraightType.has_value() && fIsFlush; }
        virtual bool isFourOfAKind() const final { return fIsFourOfAKind; }
        virtual bool isFullHouse() const final { return fIsFullHouse; }
        virtual bool isFlush() const final { return fIsFlush; }
        virtual bool isStraight() const final;
        virtual bool isWheel() const final;
        virtual bool isThreeOfAKind() const final { return fIsThreeOfAKind; }
        virtual bool isTwoPair() const final { return fIsTwoPair; }
        virtual bool isPair() const final{ return fIsPair; }
        virtual bool isHighCard() const final;
        virtual EHand getHandType( bool straightsAndFlushesCount = true, bool lowBall = false ) const final;
        virtual bool allCardsUnique() const final;

        virtual const std::list< EHand >& handOrder() const final{ return fHandOrder; }

    protected:
        THand createTHand( std::vector< std::shared_ptr< CCard > >& sharedCards ) const;
        void generateHeader( std::ostream& oss, size_t size ) const;
        void generateFooter( std::ostream& oss ) const;

        template< typename T>
        void computeAndGenerateMap( std::ostream& oss, T& map, bool straightsAndFlushesCount, bool lowBall ) const;
        template< typename T1, typename T2, typename T3, typename T4 >
        void computeAndGenerateMaps( std::ostream& oss, size_t size, T1& straightsAndFlushesDontCount, T2& straightsAndFlushesCount, T3& justCardsLowCount, T4& straightsAndFlushesLowCount ) const;
        template< typename T >
        void generateITE( std::ostream& oss, size_t size, const T& map, bool straightsAndFlushesCount, bool lowBall ) const;
        template< typename T1, typename T2, typename T3, typename T4 >
        void generateRankFunction( std::ostream& oss, size_t size, const T1& straightsAndFlushesDontCount, const T2& straightsAndFlushesCount, const T3& straightsAndFlushesDontCountLowBall, const T4& straightsAndFlushesLowCount ) const;

        void setOrigCards( const THand& cards ); // returns the cards sorted
        void setupKickers();

        std::optional< EStraightType > fStraightType;
        bool fIsFiveOfAKind{ false };
        bool fIsFourOfAKind{ false };
        bool fIsFullHouse{ false };
        bool fIsThreeOfAKind{ false };
        bool fIsFlush{ false };
        bool fIsTwoPair{ false };
        bool fIsPair{ false };
        std::list< ECard > fCards;
        std::list< ECard > fKickers;

        THand fOrigCards;
        std::vector< TCardBitType > fBitValues;

        std::list< EHand > fHandOrder;
    };

    std::ostream& operator<<( std::ostream& oss, const CCardInfo & obj );
}

#endif

