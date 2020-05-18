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
    bool C3CardInfo::sAllHandsComputed{ false };

    C3CardInfo::C3CardInfo() :
        C3CardInfo( THand( TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) ) )
    {

    }

    C3CardInfo::C3CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3 ) :
        C3CardInfo( THand( TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ) ) )
    {

    }

    C3CardInfo::C3CardInfo( const THand& cards )
    {
        fHandOrder = 
            { 
                EHand::eStraightFlush, 
                EHand::eThreeOfAKind, 
                EHand::eStraight, 
                EHand::eFlush, 
                EHand::ePair, 
                EHand::eHighCard 
            };

        setOrigCards( { std::get< 0 >( cards ), std::get< 1 >( cards ), std::get< 2 >( cards ) } );

        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
        fIsThreeOfAKind = NHandUtils::isCount( fOrigCards, 3 );
        fStraightType = NHandUtils::isStraight( fOrigCards );
    }

    void C3CardInfo::generateAllCardHands()
    {
        if ( NHandUtils::gComputeAllHands && !sAllHandsComputed )
        {
            sAllHandsComputed = true;

            using TCardToInfoMap = std::map< THand, C3CardInfo >;
            auto gFlushStraightsCount = []( const C3CardInfo& lhs, const C3CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            auto gJustCardsCount = []( const C3CardInfo& lhs, const C3CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

            using TCardsCountMap = std::map< C3CardInfo, uint32_t, decltype( gJustCardsCount ) >;
            using TFlushesCountMap = std::map< C3CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

            static TCardToInfoMap allHands;
            static TCardsCountMap justCardsCount( gJustCardsCount );
            static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            auto numHands = 52 * 51 * 50;
            std::cout << "Generating: " << numHands << "\n";

            size_t maxCardsValue = 0;

            auto&& allCards = CCard::allCardsList();
            uint64_t handCount = 0;

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

                        handCount++;
                        if ( ( handCount % ( numHands / 10 ) ) == 0 )
                            std::cout << "   Generating: Hand #" << handCount << " of " << numHands << "\n";

                        auto curr = THand( TCard( c1->getCard(), c1->getSuit() ), TCard( c2->getCard(), c2->getSuit() ), TCard( c3->getCard(), c3->getSuit() ) );
                        C3CardInfo cardInfo( curr );
                        allHands[ curr ] = cardInfo;

                        justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                        flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                        maxCardsValue = std::max( static_cast<size_t>( cardInfo.getCardsValue() ), maxCardsValue );
                    }
                }
            }

            std::cout << "Finished Generating: " << numHands << "\n";

            std::ofstream ofs( "E:/DropBox/Documents/sb/github/scottaronbloom/CardGame/Cards/3CardHandTables.cpp" );
            std::ostream & oss = ofs; //&std::cout;

            generateHeader( oss, 3 );
            computeAndGenerateMaps( oss, 3, justCardsCount, flushesAndStraightsCount );

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
            generateTable( oss, flushes, "C3CardInfo::sFlushes" );
            generateTable( oss, highCardUnique, "C3CardInfo::sHighCardUnique" );
            generateTable( oss, straightsUnique, "C3CardInfo::sStraightsUnique" );
            generateMap( oss, highCardProductMap, "C3CardInfo::sProductMap" );
            generateMap( oss, straightsProductMap, "C3CardInfo::sStraitsAndFlushesProductMap" );

            generateEvaluateFunction( oss, 3 );
            generateRankFunction( oss, 3, justCardsCount, flushesAndStraightsCount );
            generateFooter( oss );
        }
    }
}