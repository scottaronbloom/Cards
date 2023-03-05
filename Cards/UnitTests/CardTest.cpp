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

#include "CardTest.h"
#include "Cards/Game.h"
#include "Cards/Card.h"
#include "Cards/CardInfo.h"
#include "Cards/Hand.h"
#include "Cards/CardInfo.h"
#include "SABUtils/utils.h"

std::ostream& operator<<( std::ostream& os, const QString& data )
{
    return os << data.toStdString();
}

void PrintTo( const QString& data, std::ostream* os )
{
    *os << data.toStdString();
}

namespace NHandTester
{
    CHandTester::CHandTester()
    {
        fGame = new CGame;
    }

    CHandTester::~CHandTester()
    {
        delete fGame;
    }

    void CHandTester::SetUp()
    {
        NHandUtils::gComputeAllHands = false;
    }

    void CHandTester::TearDown()
    {

    }

    bool CHandTester::isStraight( std::vector< ECard > cards ) const
    {
        if ( cards.empty() )
            return false;

        std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
        auto ii = cards.begin();
        auto prev = *ii;
        ii++;
        bool highCardAce = prev == ECard::eAce;

        bool isStraight = true;
        for ( ; isStraight && ( ii != cards.end() ); ++ii )
        {
            if ( prev != ( ( *ii ) + 1 ) )
                isStraight = false;
            prev = **ii;
        }
        if ( isStraight )
            return true;

        if ( !highCardAce )
            return false;

        cards.erase( cards.begin() );

        auto jj = cards.rbegin();
        if ( jj == cards.rend() )
            return false; // onecard cant be a straight

        auto expected = ECard::eDeuce;

        while ( jj != cards.rend() )
        {
            if ( *jj != expected )
                return false;
            ++jj;
            expected++;
        }
        return true;
    }

    bool CHandTester::CompareHandOrder( const std::map< EHand, size_t >& freq, const NHandUtils::CCardInfo& cardInfo ) const
    {
        std::map< size_t, EHand > leastToMost;
        for ( auto&& ii : freq )
        {
            leastToMost[ ii.second ] = ii.first;
        }
        auto handOrder = cardInfo.handOrder();
        auto ii = leastToMost.begin();
        auto jj = handOrder.begin();
        bool retVal = true;
        for ( ; ii != leastToMost.end() && jj != handOrder.end(); ++ii, ++jj )
        {
            retVal = retVal && ( ( *ii ).second == *jj );
            EXPECT_EQ( ( *ii ).second, *jj );
        }
        return retVal;
    }

    std::tuple< std::list< std::shared_ptr< NHandUtils::CCardInfo > >, std::map< EHand, size_t >, std::map< EHand, size_t > > CHandTester::getUniqueHands( std::list< std::shared_ptr< NHandUtils::CCardInfo > >& allHands )
    {
        allHands.sort( []( const std::shared_ptr< NHandUtils::CCardInfo >& lhs, const std::shared_ptr< NHandUtils::CCardInfo >& rhs ) { return lhs->greaterThan( true, false, *rhs ); } );

        auto comp = []( const std::shared_ptr< NHandUtils::CCardInfo >& lhs, const std::shared_ptr< NHandUtils::CCardInfo >& rhs ) { return lhs->lessThan( true, false, *rhs ); };
        auto sortedMap = std::set< std::shared_ptr< NHandUtils::CCardInfo >, decltype(comp) >( comp );

        auto updateOn = std::min( static_cast<uint64_t>( 10000 ), static_cast< uint64_t >( allHands.size() / 25 ) );

        std::list< std::shared_ptr< NHandUtils::CCardInfo > > uniqueHands;
        std::optional< std::shared_ptr< NHandUtils::CCardInfo > > prevHand;
        for ( auto ii = allHands.begin(); ii != allHands.end(); ++ii )
        {
            bool inserted = sortedMap.insert( *ii ).second;
            if ( prevHand.has_value() && ( prevHand.value()->equalTo( true, **ii ) ) )
            {
                EXPECT_TRUE( !inserted );
                continue;
            }
            EXPECT_TRUE( inserted );
            prevHand = *ii;
            uniqueHands.push_back( *ii );
        }
        
        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->getHandType();
            freq[ hand ]++;
        }

        std::map< EHand, size_t > uniqueFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->getHandType();
            uniqueFreq[ hand ]++;
        }

        return std::make_tuple( uniqueHands, freq, uniqueFreq );
    }
    
    std::tuple< std::list< std::shared_ptr< CHand > >, std::map< EHand, size_t >, std::map< EHand, size_t > > CHandTester::getUniqueHands( std::list< std::shared_ptr< CHand > >& allHands )
    {
        allHands.sort( []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs ) { return lhs->operator>( *rhs ); } );

        auto updateOn = std::min( static_cast<size_t>( 10000 ), allHands.size() / 25 );

        std::list< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        int num = 0;
        for ( auto ii = allHands.begin(); ii != allHands.end(); ++ii )
        {
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "Analyzing Unique Hands: " << num << "\n";
            num++;
            
            if ( prevHand && ( prevHand->operator ==( **ii ) ) )
                continue;
            prevHand = *ii;
            uniqueHands.push_back( *ii );
        }

        num = 0;
        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "Analyzing Unique Hands: " << num << "\n";
            num++;

            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }

        num = 0;
        std::map< EHand, size_t > uniqueFreq;
        for ( auto&& ii : uniqueHands )
        {
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "Analyzing Unique Hands: " << num << "\n";
            num++;
                
            auto hand = ii->determineHand();
            uniqueFreq[ std::get< 0 >( hand ) ]++;
        }

        return std::make_tuple( uniqueHands, freq, uniqueFreq );
    }


    std::vector< std::vector< std::shared_ptr< CCard > > > CHandTester::getAllCards( size_t numCards )
    {
        auto num = NUtils::numCombinations( 52, numCards );
        auto updateOn = std::min( static_cast<uint64_t>( 10000 ), num / 25 );
        auto allCards = NUtils::allCombinations( getAllCardsVector(), numCards, { true, updateOn } );
        return allCards;
    }

    std::list< std::shared_ptr< NHandUtils::CCardInfo > > CHandTester::getAllCardInfoHands( size_t numCards )
    {
        auto allCards = getAllCards( numCards );

        int num = 0;
        auto updateOn = std::min( static_cast<size_t>( 10000 ), allCards.size() / 25 );

        std::list< std::shared_ptr< NHandUtils::CCardInfo > > retVal;
        for( auto && ii : allCards )
        {
            THand hand;
            for( auto && jj : ii )
            {
                hand.push_back( TCard( jj->getCard(), jj->getSuit() ) );
            }
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "Constructing Card Info: " << num << "\n";
            num++;
            retVal.push_back( NHandUtils::CCardInfo::createCardInfo( hand ) );
        }
        return retVal;
    }

    std::list< std::shared_ptr< CHand > > CHandTester::getAllCHandHands( size_t numCards )
    {
        auto allCards = getAllCards( numCards );

        std::list< std::shared_ptr< CHand > > retVal;
        for ( auto&& ii : allCards )
        {
            retVal.push_back( std::make_shared< CHand >( ii, fGame->playInfo() ) );
        }
        return retVal;
    }

}