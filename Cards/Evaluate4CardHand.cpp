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

#include "Evaluate4CardHand.h"
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
    S4CardInfo::S4CardInfo() :
        S4CardInfo( THand( TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) ) )
    {
    }

    S4CardInfo::S4CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4 ) :
        S4CardInfo( THand( TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ), TCard( c4, s4 ) ) )
    {
    }

    S4CardInfo::S4CardInfo( const THand& cards ) :
        fCard1( std::get< 0 >( cards ) ),
        fCard2( std::get< 1 >( cards ) ),
        fCard3( std::get< 2 >( cards ) ),
        fCard4( std::get< 3 >( cards ) )
    {
        auto cardsVector = std::vector< TCard >( { fCard1, fCard2, fCard3, fCard4 } );
        fIsFlush = NHandUtils::isFlush( cardsVector );
        fIsPair = NHandUtils::isCount( cardsVector, 2 );
        fIsThreeOfAKind = NHandUtils::isCount( cardsVector, 3 );
        fIsFourOfAKind = NHandUtils::isCount( cardsVector, 4 );
        fStraightType = NHandUtils::isStraight( cardsVector );

        auto sortedCards = cardsVector;
        std::sort( sortedCards.begin(), sortedCards.end(), []( TCard lhs, TCard rhs ) { return lhs.first > rhs.first; } );

        fCards.push_back( sortedCards[ 0 ].first );
        fKickers.push_back( sortedCards[ 1 ].first );
        fKickers.push_back( sortedCards[ 2 ].first );
        fKickers.push_back( sortedCards[ 3 ].first );

        std::map< ECard, uint8_t > cardHits;
        for ( auto&& card : sortedCards )
        {
            cardHits[ card.first ]++;
        }

        if ( fIsPair )
        {
            fCards.clear();
            fKickers.clear();
            for ( auto&& ii : cardHits )
            {
                if ( ii.second > 1 )
                    fCards = { ii.first };
                else
                    fKickers.push_back( ii.first );
            }
        }
        else if ( fIsThreeOfAKind )
        {
        }
        else if ( fIsTwoPair )
        {
        }

        fBitValues.resize( 4 );
        fBitValues[ 0 ] = NHandUtils::computeBitValue( fCard1.first, fCard1.second );
        fBitValues[ 1 ] = NHandUtils::computeBitValue( fCard2.first, fCard2.second );
        fBitValues[ 2 ] = NHandUtils::computeBitValue( fCard3.first, fCard3.second );
        fBitValues[ 3 ] = NHandUtils::computeBitValue( fCard4.first, fCard4.second );
    }

    bool S4CardInfo::isWheel() const
    {
        return fStraightType.has_value() && ( fStraightType.value() == EStraightType::eWheel );
    }

    // straights and flushes dont count
    bool S4CardInfo::compareJustCards( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return *( fCards.begin() ) < *( rhs.fCards.begin() );
        if ( !fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return true;
        if ( fIsThreeOfAKind && !rhs.fIsThreeOfAKind )
            return false;

        if ( fIsPair && rhs.fIsPair )
        {
            if ( *( fCards.begin() ) < *( rhs.fCards.begin() ) )
                return true;
            if ( *( fCards.begin() ) > * ( rhs.fCards.begin() ) )
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
            auto straightCompare = NHandUtils::compareStraight( fStraightType, rhs.fStraightType );
            if ( straightCompare.has_value() )
                return straightCompare.value();
        }

        auto ii = fCards.begin();
        auto jj = rhs.fCards.begin();
        for ( ; ( ii != fCards.end() ) && ( jj != rhs.fCards.end() ); ++ii, ++jj )
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
        for ( ; ( ii != fKickers.end() ) && ( jj != rhs.fKickers.end() ); ++ii, ++jj )
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

    bool S4CardInfo::lessThan( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( !flushStraightCount )
            return compareJustCards( flushStraightCount, rhs );

        if ( isStraightFlush() && !rhs.isStraightFlush() )
            return false;
        if ( !isStraightFlush() && rhs.isStraightFlush() )
            return true;
        if ( isStraightFlush() && rhs.isStraightFlush() )
            return compareJustCards( flushStraightCount, rhs ); // which straight flush is bigger

        if ( fIsThreeOfAKind && !rhs.fIsThreeOfAKind )
            return false;
        if ( !fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return true;
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return compareJustCards( flushStraightCount, rhs ); // which best trips

        auto straightCompare = NHandUtils::compareStraight( fStraightType, rhs.fStraightType );
        if ( straightCompare.has_value() )
            return straightCompare.value();

        if ( fIsFlush && !rhs.fIsFlush )
            return false;
        if ( !fIsFlush && rhs.fIsFlush )
            return true;
        // if ( fIsFlush && rhs.fIsFlush ) // no need to compare
        return compareJustCards( flushStraightCount, rhs );
    }

    bool S4CardInfo::equalTo( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return *( fCards.begin() ) == *( rhs.fCards.begin() );
        if ( fIsPair && rhs.fIsPair )
            return ( *( fCards.begin() ) == *( rhs.fCards.begin() ) ) && ( *( fKickers.begin() ) == *( rhs.fKickers.begin() ) );

        if ( flushStraightCount )
        {
            if ( fIsFlush != rhs.fIsFlush )
                return false;
            if ( fStraightType.has_value() && rhs.fStraightType.has_value() && ( fStraightType.value() != rhs.fStraightType.value() ) )
                return false;
        }
        return ( fCards == rhs.fCards ) && ( fKickers == rhs.fKickers );
    }

    uint16_t S4CardInfo::getCardsValue() const
    {
        return NHandUtils::getCardsValue( fBitValues );
    }

    uint64_t S4CardInfo::handProduct() const
    {
        return NHandUtils::computeHandProduct( fBitValues );
    }

    std::ostream& operator<<( std::ostream& oss, const S4CardInfo& obj )
    {
        oss << obj.fCard1.first << obj.fCard1.second
            << " "
            << obj.fCard2.first << obj.fCard2.second
            << " "
            << obj.fCard3.first << obj.fCard3.second
            ;
        return oss;
    }

    static void dumpMap( std::ostream& oss, const std::map< uint64_t, uint16_t >& values, const std::string& varName )
    {
        oss
            << "    " << "static std::unordered_map< int64_t, int16_t > " << varName << " = \n"
            << "    {\n"
            ;
        bool first = true;
        for ( auto&& ii : values )
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
        oss.flush();
    }

    static void dumpTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string& varName )
    {
        oss
            << "    " << "static std::vector< uint32_t > " << varName << " = \n"
            << "    {\n"
            ;
        size_t numChars = 0;
        for ( size_t ii = 0; ii < values.size(); ++ii )
        {
            if ( numChars == 0 )
                oss << "        ";

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

    template< typename T>
    void computeAndDumpMap( std::ostream& oss, T& map, const std::string& varName, const std::string& header )
    {
        int num = 1;
        //std::optional< S4CardInfo > prev;
        for ( auto&& ii : map )
        {
            //if ( prev.has_value() && ( *prev->equalTo( ii.first ) ) )
            //    num++;
            //prev = ii.first;
            ii.second = num++;
        }

        oss
            << "    // " << header << "\n"
            << "    static std::map< S4CardInfo::THand, uint32_t > " << varName << " = \n"
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
            auto fourthCard = ii.first.fCard4;
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );

            oss << "{ { "
                << "{ " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }, "
                << "{ " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " }, "
                << "{ " << qPrintable( ::asEnum( thirdCard.first ) ) << ", " << qPrintable( ::asEnum( thirdCard.second ) ) << " } "
                << "{ " << qPrintable( ::asEnum( fourthCard.first ) ) << ", " << qPrintable( ::asEnum( fourthCard.second ) ) << " } "
                << "}, " << ii.second << " }\n";
        }
        oss << "    };\n\n";
        oss.flush();
    }

    uint32_t evaluate4CardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        using TCardToInfoMap = std::map< S4CardInfo::THand, S4CardInfo >;
        auto gFlushStraightsCount = []( const S4CardInfo& lhs, const S4CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
        auto gJustCardsCount = []( const S4CardInfo& lhs, const S4CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

        using TCardsCountMap = std::map< S4CardInfo, uint32_t, decltype( gJustCardsCount ) >;
        using TFlushesCountMap = std::map< S4CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

        static TCardToInfoMap allHands;
        static TCardsCountMap justCardsCount( gJustCardsCount );
        static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

        if ( NHandUtils::gComputeAllHands && allHands.empty() )
        {
            auto numHands = 52 * 51 * 50 * 49;
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
                        if ( *c2 == *c3 )
                            continue;

                        for ( auto&& c4 : allCards )
                        {
                            if ( *c1 == *c4 )
                                continue;
                            if ( *c2 == *c4 )
                                continue;
                            if ( *c4 == *c3 )
                                continue;

                            handCount++;
                            if ( ( handCount % ( numHands / 10 ) ) == 0 )
                                std::cout << "   Generating: Hand #" << handCount << " of " << numHands << "\n";

                            auto curr = S4CardInfo::THand( TCard( c1->getCard(), c1->getSuit() ), TCard( c2->getCard(), c2->getSuit() ), TCard( c3->getCard(), c3->getSuit() ), TCard( c4->getCard(), c4->getSuit() ) );
                            S4CardInfo cardInfo( curr );
                            allHands[ curr ] = cardInfo;
                            
                            justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                            flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                            maxCardsValue = std::max( static_cast<size_t>( cardInfo.getCardsValue() ), maxCardsValue );
                        }
                    }
                }
            }

            std::cout << "Finished Generating: " << numHands << "\n";
            std::ofstream ofs( "4CardDump.cpp" );
            std::ostream* oss = &ofs; //&std::cout;

            computeAndDumpMap( *oss, justCardsCount, "sCardMap", "No Flushes/Straights" );
            computeAndDumpMap( *oss, flushesAndStraightsCount, "sCardMapStraightsAndFlushesCount", "Flushes/Straights" );

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

                if ( ii.second.fIsFlush )
                {
                    flushes[ cardValue ] = straightsValue;
                }
                else if ( !ii.second.fIsPair && !ii.second.fIsThreeOfAKind )
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
            dumpTable( *oss, flushes, "sFlushes" );
            dumpTable( *oss, highCardUnique, "sHighCardUnique" );
            dumpTable( *oss, straightsUnique, "sStraightsUnique" );
            dumpMap( *oss, highCardProductMap, "sHighCardProductMap" );
            dumpMap( *oss, straightsProductMap, "sStraitsProductMap" );
        }

        if ( cards.size() != 4 )
            return -1;

        auto cardsValue = getCardsValue( cards );
        //if ( playInfo && playInfo->fStraightsFlushesCountForSmallHands )
        //{
        //    if ( isFlush( cards ) )
        //        return sFlushes[ cardsValue ];
        //}

        //auto straightOrHighCard = playInfo->fStraightsFlushesCountForSmallHands ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];
        //if ( straightOrHighCard )
        //    return straightOrHighCard;

        //auto product = computeHandProduct( cards );
        //auto pos = playInfo->fStraightsFlushesCountForSmallHands ? sStraitsProductMap.find( product ) : sHighCardProductMap.find( product );
        //if ( pos == ( playInfo->fStraightsFlushesCountForSmallHands ? sStraitsProductMap.end() : sHighCardProductMap.end() ) )
        //    return -1;
        //return ( *pos ).second;
        (void)playInfo;
        return -1;
    }

    EHand rankTo4CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        EHand hand;
        if ( !playInfo->fStraightsFlushesCountForSmallHands )
        {
            if ( rank > 169U )
                hand = EHand::eHighCard;
            else if ( rank > 13U )
                hand = EHand::ePair;
            else if ( rank > 13U )
                hand = EHand::eTwoPair;
            else if ( rank > 13U )
                hand = EHand::eThreeOfAKind;
            else 
                hand = EHand::eFourOfAKind;
        }
        else
        {
            if ( rank > 467U )
                hand = EHand::eHighCard;
            else if ( rank > 311U )
                hand = EHand::ePair;
            else if ( rank > 311U )
                hand = EHand::eTwoPair;
            else if ( rank > 25U )
                hand = EHand::eStraight;
            else if ( rank > 37U )
                hand = EHand::eFlush;
            else if ( rank > 12U )
                hand = EHand::eThreeOfAKind;
            else if ( rank > 5U )
                hand = EHand::eFourOfAKind;
            else
                hand = EHand::eStraightFlush;
        }
        return hand;
    }

}