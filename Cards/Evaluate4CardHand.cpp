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
        fIsFlush = NHandUtils::isFlush( fCard1, fCard2 ) && NHandUtils::isFlush( fCard1, fCard3 ) && NHandUtils::isFlush( fCard1, fCard4 );
        fIsPair = ( fCard1.first == fCard2.first );
        fHighCard = std::max( fCard1.first, fCard2.first );
        fKicker1 = std::min( fCard1.first, fCard2.first );
        if ( ( fHighCard == ECard::eAce ) && ( fKicker1 == ECard::eDeuce ) )
            std::swap( fHighCard, fKicker1 );
        if ( fHighCard == ECard::eDeuce )
        {
            fIsStraight = ( fKicker1 == ECard::eAce );
        }
        else
        {
            fIsStraight = ( static_cast<int>( fHighCard ) - static_cast<int>( fKicker1 ) ) == 1;
        }
        fBitValues.resize( 2 );
        fBitValues[ 0 ] = NHandUtils::computeBitValue( fCard1.first, fCard1.second );
        fBitValues[ 1 ] = NHandUtils::computeBitValue( fCard2.first, fCard2.second );
    }

    bool S4CardInfo::compareJustCards( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( fIsPair && rhs.fIsPair )
            return fHighCard < rhs.fHighCard;
        if ( !fIsPair && rhs.fIsPair )
            return true;
        if ( fIsPair && !rhs.fIsPair )
            return false;

        auto high1 = ( !flushStraightCount && ( fHighCard == ECard::eDeuce && fKicker1 == ECard::eAce ) ) ? fKicker1 : fHighCard;
        auto kick1 = ( !flushStraightCount && ( fHighCard == ECard::eDeuce && fKicker1 == ECard::eAce ) ) ? fHighCard : fKicker1;

        auto high2 = ( !flushStraightCount && ( rhs.fHighCard == ECard::eDeuce && rhs.fKicker1 == ECard::eAce ) ) ? rhs.fKicker1 : rhs.fHighCard;
        auto kick2 = ( !flushStraightCount && ( rhs.fHighCard == ECard::eDeuce && rhs.fKicker1 == ECard::eAce ) ) ? rhs.fHighCard : rhs.fKicker1;

        if ( high1 != high2 )
            return ( high1 < high2 );
        if ( kick1 != kick2 )
            return kick1 < kick2;
        return false;
    }

    bool S4CardInfo::lessThan( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( !flushStraightCount || ( fIsPair || rhs.fIsPair ) )
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

    bool S4CardInfo::equalTo( bool flushStraightCount, const S4CardInfo& rhs ) const
    {
        if ( fIsPair && rhs.fIsPair )
            return fKicker1 == rhs.fKicker1;
        if ( flushStraightCount )
        {
            if ( fIsFlush != rhs.fIsFlush )
                return false;
            if ( fIsStraight != rhs.fIsStraight )
                return false;
        }
        return ( fHighCard == rhs.fHighCard ) && ( fKicker1 == rhs.fKicker1 );
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
            ;
        return oss;
    }

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
        //std::optional< S4CardInfo > prev;
        for ( auto&& ii : map )
        {
            //if ( prev.has_value() && ( *prev->equalTo( ii.first ) ) )
            //    num++;
            //prev = ii.first;
            ii.second = num++;
        }

        oss
            << "// " << header << "\n"
            << "static std::map< THand, uint32_t > " << varName << " = \n"
            << "{\n"
            ;

        bool first = true;
        for ( auto&& ii : map )
        {
            oss << "    ";
            if ( first )
                oss << " ";
            else
                oss << ",";
            first = false;
            auto firstCard = ii.first.fCard1;
            auto secondCard = ii.first.fCard2;
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );
            oss << "{ { { " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }, { " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " } }, " << ii.second << " }\n";
        }
        oss << "};\n\n";
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

        if ( allHands.empty() )
        {
            std::ofstream ofs( "CardDump.cpp" );
            std::ostream* oss = &ofs; //&std::cout;

            size_t maxCardsValue = 0;

            auto&& allCards = CCard::allCardsList();
            for ( auto&& c1 : allCards )
            {
                for ( auto&& c2 : allCards )
                {
                    if ( *c2 == *c1 )
                        continue;

                    for ( auto&& c3 : allCards )
                    {
                        if ( *c3 == *c1 )
                            continue;
                        if ( *c3 == *c2 )
                            continue;

                        for ( auto&& c4 : allCards )
                        {
                            if ( *c4 == *c1 )
                                continue;
                            if ( *c4 == *c2 )
                                continue;
                            if ( *c4 == *c3 )
                                continue;

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

            computeAndDumpMap( *oss, justCardsCount, "s4CardMap", "No Flushes/Straights" );
            computeAndDumpMap( *oss, flushesAndStraightsCount, "s4CardMapStraightsAndFlushesCount", "Flushes/Straights" );

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
    
        if ( cards.size() != 2 )
            return -1;

        (void)playInfo;

        //auto cardsValue = getCardsValue( cards );
        //if ( playInfo && playInfo->fStraightsFlushesCountForSmallHands )
        //{
        //    if ( isFlush( cards ) )
        //        return sFlushes[ cardsValue ];
        //}

        //auto straightOrHighCard = playInfo->fStraightsFlushesCountForSmallHands ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];
        //if ( straightOrHighCard )
        //    return straightOrHighCard;

        //auto product = computeHandProduct( cards );
        //auto pos = sProductMap.find( product );
        //if ( pos == sProductMap.end() )
        //    return -1;
        //return (*pos).second;
        return -1;
    }

    EHand rankTo4CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        EHand hand;
        if ( !playInfo->fStraightsFlushesCountForSmallHands )
        {
            if ( rank > 13U )
                hand = EHand::eHighCard;
            else
                hand = EHand::ePair;
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