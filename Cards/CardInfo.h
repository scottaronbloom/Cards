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
enum class EHand;
struct SPlayInfo;

namespace NHandUtils
{
    class CCardInfo
    {
    public:
        CCardInfo();

        virtual uint16_t getCardsValue() const;
        virtual uint64_t handProduct() const;

        virtual bool lessThan( bool flushStraightCount, const CCardInfo& rhs ) const;
        virtual bool greaterThan( bool flushStraightCount, const CCardInfo& rhs ) const final;
        virtual bool equalTo( bool flushStraightCount, const CCardInfo& rhs ) const;

        const std::vector< TCard > & origCards() const{ return fOrigCards; }

        const std::optional< EStraightType > & straightType() const{ return fStraightType; }

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
        virtual EHand getHandType( bool flushStraightCount = true ) const final;
        virtual bool allCardsUnique() const final;

        template< typename T1, typename T2>
        static void computeAndGenerateMaps( std::ostream& oss, size_t size, T1& justCardsCount, T2& flushesAndStraightsCount );
        template< typename T1, typename T2 >
        static void generateRankFunction( std::ostream& oss, size_t size, const T1& justCardsCount, const T2& flushesAndStraightsCount );

        static void generateMap( std::ostream& oss, const std::map< uint64_t, uint16_t >& values, const std::string& varName );
        static void generateTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string& varName );
        static void generateEvaluateFunction( std::ostream& oss, size_t size );
    protected:
        std::vector< TCard > setOrigCards( const std::vector< TCard >& cards ); // returns the cards sorted
        template< typename T >
        static void generateITE( std::ostream& oss, const T& map, bool flushStraightCount );
        template< typename T>
        static void computeAndGenerateMap( std::ostream& oss, size_t size, T& map, bool flushStraightCount );

        std::optional< EStraightType > fStraightType;
        bool fIsFourOfAKind{ false };
        bool fIsFullHouse{ false };
        bool fIsThreeOfAKind{ false };
        bool fIsFlush{ false };
        bool fIsTwoPair{ false };
        bool fIsPair{ false };
        std::list< ECard > fCards;
        std::list< ECard > fKickers;

        std::vector< TCard > fOrigCards;
        std::vector< TCardBitType > fBitValues;

        std::list< EHand > fHandOrder;
    };


    template< typename T>
    void CCardInfo::computeAndGenerateMap( std::ostream& oss, size_t size, T& map, bool flushStraightCount )
    {
        int num = 1;
        for ( auto&& ii : map )
        {
            ii.second = num++;
        }

        auto varName = flushStraightCount ? "sCardMapStraightsAndFlushesCount" : "sCardMap";
        auto header = flushStraightCount ? "Flushes/Straights" : "No Flushes/Straights";
        oss
            << "// " << header << "\n"
            << "static std::map< C" << size << "CardInfo::THand, uint32_t > " << varName << " = \n"
            << "{\n"
            ;

        EHand prevHandType = EHand::eNoCards;
        bool first = true;
        for ( auto ii = map.begin(); ii != map.end(); ++ii )
        {
            oss << "    ";
            if ( first )
                oss << " ";
            else
                oss << ",";
            first = false;
            auto firstCard = (*ii).first.fOrigCards[ 0 ];
            auto secondCard = (*ii).first.fOrigCards[ 1 ];
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );
            oss << "{ { "
                << "{ " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }"
                << ", { " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " }"
                ;
            for ( size_t jj = 2; jj < (*ii).first.fOrigCards.size(); ++jj )
                oss << ", { " << qPrintable( ::asEnum( (*ii).first.fOrigCards[ jj ].first ) ) << ", " << qPrintable( ::asEnum( (*ii).first.fOrigCards[ jj ].second ) ) << " } ";

            oss << "}, " << (*ii).second << " }";

            auto currHandType = (*ii).first.getHandType( flushStraightCount );
            
            auto next = ii;
            ++next;
            bool printHandType = ( currHandType != prevHandType ) || ( next == map.end() ) || ( (*next).first.getHandType( flushStraightCount ) != currHandType );
            if ( printHandType )
                oss << " // " << toCPPString( currHandType );
            oss << "\n";
            prevHandType = currHandType;
        }
        oss << "};\n\n";
        oss.flush();
    }

    template< typename T1, typename T2 >
    void CCardInfo::computeAndGenerateMaps( std::ostream& oss, size_t size, T1& justCardsCount, T2& flushesAndStraightsCount )
    {
        computeAndGenerateMap( oss, size, justCardsCount, false );
        computeAndGenerateMap( oss, size, flushesAndStraightsCount, true );
    }

    template< typename T >
    void CCardInfo::generateITE( std::ostream& oss, const T& map, bool flushStraightCount )
    {
        std::map< EHand, uint32_t > handTypeToFirstRank;
        for ( auto&& ii : map )
        {
            auto handType = ii.first.getHandType( flushStraightCount );
            auto pos = handTypeToFirstRank.find( handType );
            auto minValue = ii.second;

            if ( pos != handTypeToFirstRank.end() )
            {
                minValue = std::min( minValue, ( *pos ).second );
            }
            handTypeToFirstRank[ handType ] = minValue;
        }

        std::map< uint32_t, EHand > firstRankToHandType;
        for ( auto&& ii : handTypeToFirstRank )
        {
            firstRankToHandType[ ii.second ] = ii.first;
        }
        bool first = true;
        for( auto ii = firstRankToHandType.rbegin(); ii != firstRankToHandType.rend(); ++ii )
        {
            oss << "        ";
            if ( !first )
                oss << "else ";
            first = false;

            bool isOne = ( ( *ii ).first == 1 );
            if ( isOne )
                oss << "/* ";
            oss << "if ( rank >= " << (*ii).first << "U )";
            if ( isOne )
                oss << " */";
            oss << "\n"
                << "            hand = " << toCPPString( (*ii).second ) << ";\n";
        }
    }

    template< typename T1, typename T2 >
    void CCardInfo::generateRankFunction( std::ostream& oss, size_t size, const T1& justCardsCount, const T2& flushesAndStraightsCount )
    {
        oss << "EHand rankTo" << size << "CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << "{\n"
            << "    EHand hand;\n"
            << "    if ( !playInfo->fStraightsFlushesCount )\n"
            << "    {\n";
        generateITE( oss, justCardsCount, false );
        oss << "    }\n"
            << "    else\n"
            << "    {\n";
        generateITE( oss, flushesAndStraightsCount, true );
        oss << "    }\n"
            << "    return hand;\n"
            << "}\n\n"
            ;
        oss.flush();
    }

    std::ostream& operator<<( std::ostream& oss, const CCardInfo & obj );
}

#endif

