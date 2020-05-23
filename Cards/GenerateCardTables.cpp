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

#include "CardInfo.h"
#include "Hand.h"
#include "PlayInfo.h"
#include "HandUtils.h"

#include "Evaluate2CardHand.h"
#include "Evaluate3CardHand.h"
#include "Evaluate4CardHand.h"
#include "Evaluate5CardHand.h"

#include <map>
#include <set>
#include <optional>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>

namespace NHandUtils
{
    THand CCardInfo::createTHand( std::vector< std::shared_ptr< CCard > >& sharedCards ) const
    {
        THand retVal;
        retVal.reserve( sharedCards.size() );
        for ( auto&& ii : sharedCards )
        {
            retVal.push_back( TCard( ii->getCard(), ii->getSuit() ) );
        }
        return retVal;
    }

    void CCardInfo::generateHeader( std::ostream& oss, size_t size ) const
    {
        oss << "#include \"Evaluate" << size << "CardHand.h\"\n"
            << "#include \"PlayInfo.h\"\n"
            << "#include \"Hand.h\"\n"
            << "\n"
            << "#include <map>\n"
            << "\n"
            << "namespace NHandUtils\n"
            << "{\n"
            ;
    }

    void CCardInfo::generateFooter( std::ostream& oss ) const
    {
        oss << "}\n\n";
    }

    template< typename T>
    void CCardInfo::computeAndGenerateMap( std::ostream& oss, size_t size, T& map, bool flushStraightCount ) const
    {
        sabDebugStream() << "Computing hand values: " << map.size() << "\n";

        int num = 1;
        auto updateOn = std::min( static_cast<uint64_t>( 10000 ), map.size() / 25 );
        for ( auto&& ii : map )
        {
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "   Computing Hand Value: Hand #" << num << " of " << map.size() << "\n";
            ii.second = num++;
        }
        sabDebugStream() << "Finished Computing hand values: " << map.size() << "\n";

        std::string varName = flushStraightCount ? "sCardMapStraightsAndFlushesCount" : "sCardMap";
        varName = "C" + std::to_string( size ) + "CardInfo::" + varName;

        auto header = flushStraightCount ? "Flushes/Straights" : "No Flushes/Straights";
        oss
            << "// " << header << "\n"
            << "std::map< THand, uint32_t > " << varName << " = \n"
            << "{\n"
            ;

        sabDebugStream() << "Writing Map: " << map.size() << "\n";
        EHand prevHandType = EHand::eNoCards;
        bool first = true;
        for ( auto ii = map.begin(); ii != map.end(); ++ii )
        {
            if ( ( ( *ii ).second % updateOn ) == 0 )
                sabDebugStream() << "   Computing Hand Value: Hand #" << ( *ii ).second << " of " << map.size() << "\n";

            oss << "    ";
            if ( first )
                oss << " ";
            else
                oss << ",";
            first = false;
            auto firstCard = ( *ii ).first->fOrigCards[ 0 ];
            auto secondCard = ( *ii ).first->fOrigCards[ 1 ];
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );
            oss << "{ { "
                << "{ " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }"
                << ", { " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " }"
                ;
            for ( size_t jj = 2; jj < ( *ii ).first->fOrigCards.size(); ++jj )
                oss << ", { " << qPrintable( ::asEnum( ( *ii ).first->fOrigCards[ jj ].first ) ) << ", " << qPrintable( ::asEnum( ( *ii ).first->fOrigCards[ jj ].second ) ) << " } ";

            oss << "}, " << ( *ii ).second << " }";

            auto currHandType = ( *ii ).first->getHandType( flushStraightCount );

            auto next = ii;
            ++next;
            bool printHandType = ( currHandType != prevHandType ) || ( next == map.end() ) || ( ( *next ).first->getHandType( flushStraightCount ) != currHandType );
            if ( printHandType )
                oss << " // " << toCPPString( currHandType );
            oss << "\n";
            prevHandType = currHandType;
        }
        sabDebugStream() << "Finished Writing Map: " << map.size() << "\n";
        oss << "};\n\n";
        oss.flush();
    }

    template< typename T1, typename T2 >
    void CCardInfo::computeAndGenerateMaps( std::ostream& oss, size_t size, T1& justCardsCount, T2& flushesAndStraightsCount ) const
    {
        computeAndGenerateMap( oss, size, justCardsCount, false );
        computeAndGenerateMap( oss, size, flushesAndStraightsCount, true );
    }

    template< typename T >
    void CCardInfo::generateITE( std::ostream& oss, size_t size, const T& map, bool flushStraightCount ) const
    {
        std::map< EHand, uint32_t > handTypeToFirstRank;
        for ( auto&& ii : map )
        {
            auto handType = ii.first->getHandType( flushStraightCount );
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
        std::string wildCardSuffix;
        if ( size == 5 )
            wildCardSuffix = " + ( playInfo->hasWildCards() ? 13 : 0 )";

        bool first = true;
        for ( auto ii = firstRankToHandType.rbegin(); ii != firstRankToHandType.rend(); ++ii )
        {
            oss << "        ";
            if ( !first )
                oss << "else ";
            first = false;

            bool isOne = ( ( *ii ).first == 1 );
            if ( isOne )
                oss << "/* ";
            oss << "if ( rank >= " << ( *ii ).first << "U" << wildCardSuffix << " )";
            if ( isOne )
                oss << " */";
            oss << "\n"
                << "            hand = " << toCPPString( ( *ii ).second ) << ";\n";
        }
    }

    template< typename T1, typename T2 >
    void CCardInfo::generateRankFunction( std::ostream& oss, size_t size, const T1& justCardsCount, const T2& flushesAndStraightsCount ) const
    {
        oss << "EHand C" << size << "CardInfo::rankToCardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << "{\n"
            << "    EHand hand;\n"
            << "    if ( !playInfo->fStraightsAndFlushesCount )\n"
            << "    {\n";
        generateITE( oss, size, justCardsCount, false );
        oss << "    }\n"
            << "    else\n"
            << "    {\n";
        generateITE( oss, size, flushesAndStraightsCount, true );
        oss << "    }\n"
            << "    return hand;\n"
            << "}\n\n"
            ;
        oss.flush();
    }

    static void generateTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string& varName )
    {
        oss
            << "" << "std::vector< uint32_t > " << varName << " = \n"
            << "{\n"
            ;
        size_t numChars = 0;
        for ( size_t ii = 0; ii < values.size(); ++ii )
        {
            if ( numChars == 0 )
                oss << "    ";

            auto tmp = std::to_string( values[ ii ] );
            oss << tmp;
            numChars += tmp.size();
            if ( ii != ( values.size() - 1 ) )
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
        oss.flush();
    }

    static void generateMap( std::ostream& oss, const std::map< uint64_t, uint16_t >& values, const std::string& varName )
    {
        oss
            << "" << "std::unordered_map< int64_t, int16_t > " << varName << " = \n"
            << "{\n"
            ;
        bool first = true;
        for ( auto&& ii : values )
        {
            oss << "    ";

            if ( !first )
                oss << ",";
            else
                oss << " ";
            first = false;
            oss << "{ " << ii.first << ", " << ii.second << " }\n";
        }
        oss << "};\n\n";
        oss.flush();
    }

    static void generateEvaluateFunction( std::ostream& oss, size_t size )
    {
        std::string wildCardSuffix;
        if ( size == 5 )
            wildCardSuffix = " + ( playInfo->hasWildCards() ? 13 : 0 )";
        oss << "uint32_t C" << size << "CardInfo::evaluateCardHand( const std::vector< std::shared_ptr< CCard > > & cards, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << "{\n"
            << "    if ( cards.size() != " << size << " )\n"
            << "        return -1;\n"
            << "\n"
            << "    auto cardsValue = NHandUtils::getCardsValue( cards );\n"
            << "    if ( playInfo && playInfo->fStraightsAndFlushesCount )\n"
            << "    {\n"
            << "        if ( NHandUtils::isFlush( cards ) )\n"
            << "            return sFlushes[ cardsValue ]" << wildCardSuffix << ";\n"
            << "    }\n"
            << "\n"
            << "    auto straightOrHighCard = playInfo->fStraightsAndFlushesCount ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];\n"
            << "    if ( straightOrHighCard )\n"
            << "        return straightOrHighCard" << wildCardSuffix << ";\n"
            << "\n"
            << "    auto product = computeHandProduct( cards );\n"
            << "    auto productMap = playInfo->fStraightsAndFlushesCount ? sStraitsAndFlushesProductMap : sProductMap;\n"
            << "    auto pos = productMap.find( product );\n"
            << "    if ( pos == productMap.end() )\n"
            << "        return -1;\n"
            << "    return ( *pos ).second" << wildCardSuffix << ";\n"
            << "}\n\n";
    }

    void CCardInfo::generateAllCardHands()
    {
        if ( NHandUtils::gComputeAllHands && !allHandsComputed() )
        {
            setAllHandsComputed( true );

            using TCardToInfoMap = std::map< THand, std::shared_ptr< CCardInfo > >;
            auto gFlushStraightsHighCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( true, *rhs ); };
            auto gJustCardsHighCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( false, *rhs ); };
            auto gFlushStraightsLowCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( true, *rhs ); };
            auto gJustCardsLowCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( false, *rhs ); };

            using TCardsCountMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( gJustCardsCount ) >;
            using TFlushesCountMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( gFlushStraightsCount ) >;

            TCardToInfoMap allHands;
            TCardsCountMap justCardsCount( gJustCardsCount );
            TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            auto numHands = NUtils::numCombinations( 52, getNumCards() );
            sabDebugStream() << "Generating: " << numHands << "\n";

            size_t maxCardsValue = 0;

            auto&& allCardsVector = CCard::allCards();
            auto updateOn = std::min( static_cast<uint64_t>( 10000 ), numHands / 25 );
            auto allCardCombos = NUtils::allCombinations( allCardsVector, getNumCards(), { true, updateOn } );
            for ( size_t ii = 0; ii < allCardCombos.size(); ++ii )
            {
                if ( ( ii % updateOn ) == 0 )
                    sabDebugStream() << "   Generating: Hand #" << ii << " of " << numHands << "\n";

                auto curr = createTHand( allCardCombos[ ii ] );
                auto cardInfo = createCardInfo( curr );
                allHands[ curr ] = cardInfo;
                justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                maxCardsValue = std::max( static_cast<size_t>( cardInfo->getCardsValue() ), maxCardsValue );
            }

            sabDebugStream() << "Finished Generating: " << numHands << "\n";

            std::string fileName = "E:/DropBox/Documents/sb/github/scottaronbloom/CardGame/Cards/" + std::to_string( getNumCards() ) + "CardHandTables.cpp";
            std::ofstream ofs( fileName );
            std::ostream& oss = ofs;

            generateHeader( oss, getNumCards() );
            computeAndGenerateMaps( oss, getNumCards(), justCardsCount, flushesAndStraightsCount );

            std::vector< uint32_t > flushes;
            flushes.resize( maxCardsValue + 1 );

            std::vector< uint32_t > highCardUnique;
            highCardUnique.resize( maxCardsValue + 1 );

            std::vector< uint32_t > straightsUnique;
            straightsUnique.resize( maxCardsValue + 1 );

            std::map< uint64_t, uint16_t > highCardProductMap;
            std::map< uint64_t, uint16_t > straightsProductMap;

            for ( auto&& ii : allHands )
            {
                auto cardValue = ii.second->getCardsValue();

                auto pos = flushesAndStraightsCount.find( ii.second );
                Q_ASSERT( pos != flushesAndStraightsCount.end() );
                auto straightsValue = ( *pos ).second;

                auto pos2 = justCardsCount.find( ii.second );
                Q_ASSERT( pos2 != justCardsCount.end() );
                auto highCardValue = ( *pos2 ).second;

                if ( ii.second->isFlush() )
                {
                    flushes[ cardValue ] = straightsValue;
                }
                else if ( ii.second->allCardsUnique() )
                {
                    straightsUnique[ cardValue ] = straightsValue;
                    highCardUnique[ cardValue ] = highCardValue;
                }
                else
                {
                    auto productValue = ii.second->handProduct();
                    straightsProductMap[ productValue ] = straightsValue;
                    highCardProductMap[ productValue ] = highCardValue;
                }
            }

            std::string className = "C" + std::to_string( getNumCards() ) + "CardInfo::";
            generateTable( oss, flushes, className + "sFlushes" );
            generateTable( oss, highCardUnique, className + "sHighCardUnique" );
            generateTable( oss, straightsUnique, className + "sStraightsUnique" );
            generateMap( oss, highCardProductMap, className + "sProductMap" );
            generateMap( oss, straightsProductMap, className + "sStraitsAndFlushesProductMap" );

            generateEvaluateFunction( oss, getNumCards() );
            generateRankFunction( oss, getNumCards(), justCardsCount, flushesAndStraightsCount );
            generateFooter( oss );
        }
    }
}
