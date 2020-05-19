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

#include "Evaluate5CardHand.h"
#include "HandUtils.h"
#include "Hand.h"
#include "PlayInfo.h"
#include <fstream>
#include <unordered_map>
#include "SABUtils/utils.h"

namespace NHandUtils
{
    C5CardInfo::C5CardInfo() :
        C5CardInfo( THand( TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) ) )
    {
    }

    C5CardInfo::C5CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4, ECard c5, ESuit s5 ) :
        C5CardInfo( THand( TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ), TCard( c4, s4 ), TCard( c5, s5 ) ) )
    {
    }

    C5CardInfo::C5CardInfo( const std::vector< TCard >& cards ) :
        C5CardInfo( std::make_tuple( cards[ 0 ], cards[ 1 ], cards[ 2 ], cards[ 3 ], cards[ 4 ] ) )
    {
    }

    C5CardInfo::C5CardInfo( const THand& cards )
    {
        fHandOrder =
        {
            //EHand::eFiveOfAKind,
            EHand::eStraightFlush,
            EHand::eFourOfAKind,
            EHand::eFullHouse,
            EHand::eFlush,
            EHand::eStraight,
            EHand::eThreeOfAKind,
            EHand::eTwoPair,
            EHand::ePair,
            EHand::eHighCard
        };

        setOrigCards( { std::get< 0 >( cards ), std::get< 1 >( cards ), std::get< 2 >( cards ), std::get< 3 >( cards ), std::get< 4 >( cards ) } );

        fIsFiveOfAKind = NHandUtils::isCount( fOrigCards, 5 );
        fIsFourOfAKind = NHandUtils::isCount( fOrigCards, 4 );
        fIsFullHouse = NHandUtils::isCount( fOrigCards, { 3,2 } );
        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fStraightType = NHandUtils::isStraight( fOrigCards );
        fIsThreeOfAKind = NHandUtils::isCount( fOrigCards, 3 );
        fIsTwoPair = NHandUtils::isCount( fOrigCards, { 2, 2 } );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
    }

    void C5CardInfo::generateAllCardHands()
    {
        static bool sAllHandsComputed{ false };
        if ( NHandUtils::gComputeAllHands && !sAllHandsComputed )
        {
            sAllHandsComputed = true;

            using TCardToInfoMap = std::map< C5CardInfo::THand, C5CardInfo >;
            auto gFlushStraightsCount = []( const C5CardInfo& lhs, const C5CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            auto gJustCardsCount = []( const C5CardInfo& lhs, const C5CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

            using TCardsCountMap = std::map< C5CardInfo, uint32_t, decltype( gJustCardsCount ) >;
            using TFlushesCountMap = std::map< C5CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

            TCardToInfoMap allHands;
            TCardsCountMap justCardsCount( gJustCardsCount );
            TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            auto numHands = NUtils::numCombinations( 52, 5 );
            std::cout << "Generating: " << numHands << "\n";

            size_t maxCardsValue = 0;

            auto&& allCardsVector = CCard::allCards();
            auto updateOn = std::min( static_cast<uint64_t>( 10000 ), numHands / 25 );
            auto allCardCombos = NUtils::allCombinations( allCardsVector, 5, { true, updateOn } );
            for ( size_t ii = 0; ii < allCardCombos.size(); ++ii )
            {
                if ( ( ii % updateOn ) == 0 )
                    std::cout << "   Generating: Hand #" << ii << " of " << numHands << "\n";

                auto curr = THand( TCard( allCardCombos[ ii ][ 0 ]->getCard(), allCardCombos[ ii ][ 0 ]->getSuit() ), TCard( allCardCombos[ ii ][ 1 ]->getCard(), allCardCombos[ ii ][ 1 ]->getSuit() ), TCard( allCardCombos[ ii ][ 2 ]->getCard(), allCardCombos[ ii ][ 2 ]->getSuit() ), TCard( allCardCombos[ ii ][ 3 ]->getCard(), allCardCombos[ ii ][ 3 ]->getSuit() ), TCard( allCardCombos[ ii ][ 4 ]->getCard(), allCardCombos[ ii ][ 4 ]->getSuit() ) );
                C5CardInfo cardInfo( curr );
                allHands[ curr ] = cardInfo;

                justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                maxCardsValue = std::max( static_cast<size_t>( cardInfo.getCardsValue() ), maxCardsValue );
            }
            std::cout << "Finished Generating: " << numHands << "\n";
            std::ofstream ofs( "E:/DropBox/Documents/sb/github/scottaronbloom/CardGame/Cards/5CardHandTables.cpp" );
            std::ostream & oss = ofs; //&std::cout;

            CCardInfo::generateHeader( oss, 5 );
            CCardInfo::computeAndGenerateMaps( oss, 5, justCardsCount, flushesAndStraightsCount );

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
                auto cardValue = ii.second.getCardsValue();

                auto pos = flushesAndStraightsCount.find( ii.second );
                Q_ASSERT( pos != flushesAndStraightsCount.end() );

                auto straightsValue = ( *pos ).second;

                auto pos2 = justCardsCount.find( ii.second );
                Q_ASSERT( pos2 != justCardsCount.end() );
                auto highCardValue = ( *pos2 ).second;

                if ( ii.second.isFlush() )
                {
                    flushes[ cardValue ] = straightsValue;
                }
                else if ( ii.second.allCardsUnique() )
                {
                    straightsUnique[ cardValue ] = straightsValue;
                    highCardUnique[ cardValue ] = highCardValue;
                }
                else
                {
                    auto productValue = ii.second.handProduct();
                    straightsProductMap[ productValue ] = straightsValue;
                    highCardProductMap[ productValue ] = highCardValue;
                }
            }
            CCardInfo::generateTable( oss, flushes, "C5CardInfo::sFlushes" );
            CCardInfo::generateTable( oss, highCardUnique, "C5CardInfo::sHighCardUnique" );
            CCardInfo::generateTable( oss, straightsUnique, "C5CardInfo::sStraightsUnique" );
            CCardInfo::generateMap( oss, highCardProductMap, "C5CardInfo::sProductMap" );
            CCardInfo::generateMap( oss, straightsProductMap, "C5CardInfo::sStraitsAndFlushesProductMap" );

            CCardInfo::generateEvaluateFunction( oss, 5 );
            CCardInfo::generateRankFunction( oss, 5, justCardsCount, flushesAndStraightsCount );
            CCardInfo::generateFooter( oss );
        }
    }
}
