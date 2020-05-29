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
#include "Cards/Evaluate2CardHand.h"
#include "Cards/Evaluate3CardHand.h"
#include "Cards/Evaluate4CardHand.h"
#include "Cards/Evaluate5CardHand.h"
#include "Cards/Game.h"
#include "Cards/Player.h"
#include "Cards/Hand.h"
#include "Cards/Card.h"
#include "SABUtils/utils.h"

#include "gmock/gmock.h"

#include <string>

namespace NHandTester
{
    TEST( HandUtils, HandUtils )
    {
        auto cards = THand( { TCard( ECard::eDeuce, ESuit::eSpades ), TCard( ECard::eDeuce, ESuit::eHearts ), TCard( ECard::eTrey, ESuit::eSpades ), TCard( ECard::eTrey, ESuit::eHearts ), TCard( ECard::eFour, ESuit::eHearts ) } );
        EXPECT_FALSE( NHandUtils::isCount( cards, 4 ) );
        EXPECT_TRUE( NHandUtils::isCount( cards, { 2, 2 } ) );
        EXPECT_FALSE( NHandUtils::isCount( cards, { 2, 3 } ) );
        EXPECT_FALSE( NHandUtils::isCount( cards, { 3, 2 } ) );

        cards = THand( { TCard( ECard::eDeuce, ESuit::eSpades ), TCard( ECard::eDeuce, ESuit::eHearts ), TCard( ECard::eTrey, ESuit::eSpades ), TCard( ECard::eTrey, ESuit::eHearts ), TCard( ECard::eDeuce, ESuit::eHearts ) } );
        EXPECT_FALSE( NHandUtils::isCount( cards, { 2, 2 } ) );
        EXPECT_TRUE( NHandUtils::isCount( cards, { 2, 3 } ) );
        EXPECT_TRUE( NHandUtils::isCount( cards, { 3, 2 } ) );
    }
    class C2CardHandTester : public CHandTester
    {
    protected:
        C2CardHandTester() {}
        virtual ~C2CardHandTester() {}
    };
    
    TEST_F( C2CardHandTester, AllCardHandsByCardInfo )
    {
        auto allHands = getAllCardInfoHands( 2 );

        EXPECT_EQ( 1326, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 169, std::get< 0 >( analyzedHands ).size() );

        CompareHandOrder( std::get< 1 >( analyzedHands ), NHandUtils::C2CardInfo() );

        EXPECT_EQ( 52, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 78, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 156, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 260, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 780, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 65, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 65, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );

        NHandUtils::gComputeAllHands = true;
        auto cardInfo = std::make_unique< NHandUtils::C2CardInfo >();
        cardInfo->generateAllCardHands();
    }

    TEST_F( C2CardHandTester, Basic )
    {
        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::C2CardInfo h2( ECard::eDeuce, ESuit::eClubs, ECard::eDeuce, ESuit::eDiamonds );
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );
            EXPECT_TRUE( h2.equalTo( false, h1 ) );

            EXPECT_FALSE( h1.lessThan( false, h2 ) );
            EXPECT_FALSE( h1.greaterThan( false, h2 ) );
            EXPECT_TRUE( h1.equalTo( false, h2 ) );

            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_TRUE( h2.equalTo( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_TRUE( h1.equalTo( true, h2 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades );
            NHandUtils::C2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::C2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::C2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eQueen, ESuit::eSpades );
            NHandUtils::C2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eDeuce, ESuit::eSpades );
            NHandUtils::C2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eAce, ESuit::eSpades );
            NHandUtils::C2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eSpades );
            EXPECT_TRUE( h1.lessThan( true, h2 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair
            NHandUtils::C2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eHearts ); // straight
            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair
            NHandUtils::C2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eSpades ); // straight flush
            EXPECT_TRUE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );

            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );   // straight flush
            NHandUtils::C2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts ); // straight
            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h2.lessThan( true, h1 ) );

            auto gFlushStraightsCount = []( const NHandUtils::C2CardInfo& lhs, const NHandUtils::C2CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            using TFlushesCountMap = std::map< NHandUtils::C2CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;
            static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            flushesAndStraightsCount.insert( std::make_pair( h1, -1 ) );
            auto pos = flushesAndStraightsCount.find( h2 );
            EXPECT_TRUE( pos == flushesAndStraightsCount.end() );
        }
    }

    TEST_F( C2CardHandTester, OneOfEachHand )
    {
        auto h1 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );
        EXPECT_TRUE( h1.isStraightFlush() );
        EXPECT_FALSE( h1.isFourOfAKind() );
        EXPECT_FALSE( h1.isFullHouse() );
        EXPECT_TRUE( h1.isFlush() );
        EXPECT_TRUE( h1.isStraight() );
        EXPECT_FALSE( h1.isWheel() );
        EXPECT_FALSE( h1.isThreeOfAKind() );
        EXPECT_FALSE( h1.isTwoPair() );
        EXPECT_FALSE( h1.isPair() );
        EXPECT_FALSE( h1.isHighCard() );

        auto h2 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
        EXPECT_FALSE( h2.isStraightFlush() );
        EXPECT_FALSE( h2.isFourOfAKind() );
        EXPECT_FALSE( h2.isFullHouse() );
        EXPECT_FALSE( h2.isFlush() );
        EXPECT_FALSE( h2.isStraight() );
        EXPECT_FALSE( h2.isWheel() );
        EXPECT_FALSE( h2.isThreeOfAKind() );
        EXPECT_FALSE( h2.isTwoPair() );
        EXPECT_TRUE( h2.isPair() );
        EXPECT_FALSE( h2.isHighCard() );

        auto h3 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts );
        EXPECT_FALSE( h3.isStraightFlush() );
        EXPECT_FALSE( h3.isFourOfAKind() );
        EXPECT_FALSE( h3.isFullHouse() );
        EXPECT_FALSE( h3.isFlush() );
        EXPECT_TRUE( h3.isStraight() );
        EXPECT_FALSE( h3.isWheel() );
        EXPECT_FALSE( h3.isThreeOfAKind() );
        EXPECT_FALSE( h3.isTwoPair() );
        EXPECT_FALSE( h3.isPair() );
        EXPECT_FALSE( h3.isHighCard() );

        auto h4 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eAce, ESuit::eHearts );
        EXPECT_FALSE( h4.isStraightFlush() );
        EXPECT_FALSE( h4.isFourOfAKind() );
        EXPECT_FALSE( h4.isFullHouse() );
        EXPECT_FALSE( h4.isFlush() );
        EXPECT_TRUE( h4.isStraight() );
        EXPECT_TRUE( h4.isWheel() );
        EXPECT_FALSE( h4.isThreeOfAKind() );
        EXPECT_FALSE( h4.isTwoPair() );
        EXPECT_FALSE( h4.isPair() );
        EXPECT_FALSE( h4.isHighCard() );

        auto h5 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eFive, ESuit::eSpades );
        EXPECT_FALSE( h5.isStraightFlush() );
        EXPECT_FALSE( h5.isFourOfAKind() );
        EXPECT_FALSE( h5.isFullHouse() );
        EXPECT_TRUE( h5.isFlush() );
        EXPECT_FALSE( h5.isStraight() );
        EXPECT_FALSE( h5.isWheel() );
        EXPECT_FALSE( h5.isThreeOfAKind() );
        EXPECT_FALSE( h5.isTwoPair() );
        EXPECT_FALSE( h5.isPair() );
        EXPECT_FALSE( h5.isHighCard() );

        auto h6 = NHandUtils::C2CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eFive, ESuit::eHearts );
        EXPECT_FALSE( h6.isStraightFlush() );
        EXPECT_FALSE( h6.isFourOfAKind() );
        EXPECT_FALSE( h6.isFullHouse() );
        EXPECT_FALSE( h6.isFlush() );
        EXPECT_FALSE( h6.isStraight() );
        EXPECT_FALSE( h6.isWheel() );
        EXPECT_FALSE( h6.isThreeOfAKind() );
        EXPECT_FALSE( h6.isTwoPair() );
        EXPECT_FALSE( h6.isPair() );
        EXPECT_TRUE( h6.isHighCard() );

        EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        EXPECT_TRUE( h1.greaterThan( true, h3 ) );
        EXPECT_TRUE( h1.greaterThan( true, h4 ) );
        EXPECT_TRUE( h1.greaterThan( true, h5 ) );
        EXPECT_TRUE( h1.greaterThan( true, h6 ) );

        EXPECT_TRUE( h2.lessThan( true, h1 ) );
        EXPECT_TRUE( h2.greaterThan( true, h3 ) );
        EXPECT_TRUE( h2.greaterThan( true, h4 ) );
        EXPECT_TRUE( h2.greaterThan( true, h5 ) );
        EXPECT_TRUE( h2.greaterThan( true, h6 ) );

        EXPECT_TRUE( h3.lessThan( true, h1 ) );
        EXPECT_TRUE( h3.lessThan( true, h2 ) );
        EXPECT_TRUE( h3.greaterThan( true, h4 ) );
        EXPECT_TRUE( h3.greaterThan( true, h5 ) );
        EXPECT_TRUE( h3.greaterThan( true, h6 ) );

        EXPECT_TRUE( h4.lessThan( true, h1 ) );
        EXPECT_TRUE( h4.lessThan( true, h2 ) );
        EXPECT_TRUE( h4.lessThan( true, h3 ) );
        EXPECT_TRUE( h4.greaterThan( true, h5 ) );
        EXPECT_TRUE( h4.greaterThan( true, h6 ) );

        EXPECT_TRUE( h5.lessThan( true, h1 ) );
        EXPECT_TRUE( h5.lessThan( true, h2 ) );
        EXPECT_TRUE( h5.lessThan( true, h3 ) );
        EXPECT_TRUE( h5.lessThan( true, h4 ) );
        EXPECT_TRUE( h5.greaterThan( true, h6 ) );

        EXPECT_TRUE( h6.lessThan( true, h1 ) );
        EXPECT_TRUE( h6.lessThan( true, h2 ) );
        EXPECT_TRUE( h6.lessThan( true, h3 ) );
        EXPECT_TRUE( h6.lessThan( true, h4 ) );
        EXPECT_TRUE( h6.lessThan( true, h5 ) );
    }

    TEST_F( C2CardHandTester, StraightFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                p1->clearCards();

                if ( highCard == ECard::eDeuce )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for ( auto currCard = ( highCard - 1 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() ) << *p1->getHand();
                EXPECT_TRUE( p1->isStraight() ) << *p1->getHand();
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                if ( highCard == ECard::eDeuce )
                {
                    EXPECT_EQ( ECard::eDeuce, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( ECard::eAce, std::get< 2 >( hand ).front() );
                }
                else
                {
                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( highCard - 1, std::get< 2 >( hand ).front() );
                }
            }
        }
    }

    TEST_F( C2CardHandTester, StraightFlushes_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                p1->clearCards();

                if ( highCard == ECard::eDeuce )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for ( auto currCard = ( highCard - 1 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_FALSE( p1->isFlush() );
                EXPECT_FALSE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                if ( highCard == ECard::eDeuce )
                {
                    EXPECT_EQ( ECard::eAce, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( ECard::eDeuce, std::get< 2 >( hand ).front() );
                }
                else
                {
                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( highCard - 1, std::get< 2 >( hand ).front() );
                }
            }
        }
    }

    TEST_F( C2CardHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eDeuce ) // for a flush, the high card can only be a 3 or above, otherwise its a straight flush
                    continue;

                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, suit ) );
                if ( highCard == ECard::eTrey )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
                else
                    p1->addCard( fGame->getCard( highCard - 2, suit ) );

                auto hand = p1->determineHand();
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C2CardHandTester, Flush_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eDeuce ) // for a flush, the high card can only be a 3 or above, otherwise its a straight flush
                    continue;

                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, suit ) );
                if ( highCard == ECard::eTrey )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
                else
                    p1->addCard( fGame->getCard( highCard - 2, suit ) );

                auto hand = p1->determineHand();
                EXPECT_FALSE( p1->isFlush() );
                EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C2CardHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& suit : ESuit() )
            {
                p1->clearCards();
                if ( suit == ESuit::eClubs )
                    continue;

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                if ( highCard == ECard::eDeuce )
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                else
                    p1->addCard( fGame->getCard( highCard - 1, suit ) );

                EXPECT_TRUE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C2CardHandTester, Straight_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        for ( auto&& highCard : ECard() )
        {
            //if ( highCard <= ECard::eFour )
            //    continue;

            for ( auto&& suit : ESuit() )
            {
                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                if ( highCard == ECard::eDeuce )
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                else
                    p1->addCard( fGame->getCard( highCard - 1, suit ) );

                EXPECT_FALSE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                if ( highCard != ECard::eDeuce )
                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                else
                    EXPECT_EQ( ECard::eAce, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C2CardHandTester, Pair )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    p1->clearCards();

                    p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                    p1->addCard( fGame->getCard( highCard, highSuit2 ) );

                    auto hand = p1->determineHand();
                    EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) )
                        << "Cards: "
                        << highCard << highSuit1 << " "
                        << highCard << highSuit2 << " "
                        ;

                    ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( 0, std::get< 2 >( hand ).size() );
                }
            }
        }
    }

    TEST_F( C2CardHandTester, Pair_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    p1->clearCards();

                    p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                    p1->addCard( fGame->getCard( highCard, highSuit2 ) );

                    auto hand = p1->determineHand();
                    EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
                    ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( 0, std::get< 2 >( hand ).size() );
                }
            }
        }
    }

    TEST_F( C2CardHandTester, HighCard )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        for ( auto&& card1 : ECard() )
        {
            for ( auto&& suit1 : ESuit() )
            {
                for ( auto&& card2 : ECard() )
                {
                    if ( card1 == card2 )
                        continue;
                    for ( auto&& suit2 : ESuit() )
                    {
                        if ( suit1 == suit2 )
                            continue;

                        if ( isStraight( std::vector< ECard >( { card1, card2 } ) ) )
                            continue;

                        p1->clearCards();

                        p1->addCard( fGame->getCard( card1, suit1 ) );
                        p1->addCard( fGame->getCard( card2, suit2 ) );

                        auto hand = p1->determineHand();
                        EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) )
                            << "Cards: "
                            << card1 << suit1 << " "
                            << card2 << suit2 << " "
                            ;

                        auto cards = std::vector< ECard >( { card1, card2 } );
                        std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                        auto kickers = cards;
                        kickers.erase( kickers.begin(), ++kickers.begin() );
                        cards.erase( ++cards.begin(), cards.end() );

                        EXPECT_EQ( cards, std::get< 1 >( hand ) )
                            << "Cards: "
                            << card1 << suit1 << " "
                            << card2 << suit2 << " "
                            ;

                        EXPECT_EQ( kickers, std::get< 2 >( hand ) )
                            << "Cards: "
                            << card1 << suit1 << " "
                            << card2 << suit2 << " "
                            ;
                    }
                }
            }
        }
    }

    TEST_F( C2CardHandTester, AllCardHands )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        auto allHands = getAllCHandHands( 2 );

        EXPECT_EQ( 1326, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 169, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 52, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 78, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 156, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 260, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 780, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 65, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 65, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    TEST_F( C2CardHandTester, AllCardHands_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        auto allHands = getAllCHandHands( 2 );

        EXPECT_EQ( 1326, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 91, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 78, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1248, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 78, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    class C3CardHandTester : public CHandTester
    {
    protected:
        C3CardHandTester() {}
        virtual ~C3CardHandTester() {}
    };

    TEST_F( C3CardHandTester, AllCardHandsByCardInfo )
    {
        auto allHands = getAllCardInfoHands( 3 );

        EXPECT_EQ( 22100, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 741, std::get< 0 >( analyzedHands ).size() );

        CompareHandOrder( std::get< 1 >( analyzedHands ), NHandUtils::C3CardInfo() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 48, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 52, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 720, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 1096, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 16440, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 12, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 274, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 12, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 274, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );

        NHandUtils::gComputeAllHands = true;
        auto cardInfo = std::make_unique< NHandUtils::C3CardInfo >();
        cardInfo->generateAllCardHands();
    }

    TEST_F( C3CardHandTester, Basic )
    {
        {
            using TCardToInfoMap = std::map< THand, NHandUtils::C3CardInfo >;
            auto gFlushStraightsCount = []( const NHandUtils::C3CardInfo& lhs, const NHandUtils::C3CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            auto gFlushesAndStraightsDontCount = []( const NHandUtils::C3CardInfo& lhs, const NHandUtils::C3CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

            using TCardsCountMap = std::map< NHandUtils::C3CardInfo, uint32_t, decltype( gFlushesAndStraightsDontCount ) >;
            using TFlushesCountMap = std::map< NHandUtils::C3CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

            TCardsCountMap justCardsCount( gFlushesAndStraightsDontCount );
            TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            NHandUtils::C3CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eKing, ESuit::eHearts, ECard::eJack, ESuit::eSpades ); // pair of 2s 3 kicker
            NHandUtils::C3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eAce, ESuit::eSpades ); // pair or 3s 2 kicker
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );

            auto inserted = justCardsCount.insert( std::make_pair( h1, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = justCardsCount.insert( std::make_pair( h2, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = flushesAndStraightsCount.insert( std::make_pair( h1, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = flushesAndStraightsCount.insert( std::make_pair( h2, -1 ) );
            EXPECT_TRUE( inserted.second );

            justCardsCount.clear();
            flushesAndStraightsCount.clear();
            NHandUtils::C3CardInfo h3( ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eSpades ); 
            NHandUtils::C3CardInfo h4( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eSpades ); 
            inserted = justCardsCount.insert( std::make_pair( h3, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = justCardsCount.insert( std::make_pair( h4, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = flushesAndStraightsCount.insert( std::make_pair( h3, -1 ) );
            EXPECT_TRUE( inserted.second );

            inserted = flushesAndStraightsCount.insert( std::make_pair( h4, -1 ) );
            EXPECT_TRUE( inserted.second );
        }

        {
            NHandUtils::C3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } } );  /// ace high 3 2
            NHandUtils::C3CardInfo h2( { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } } ); // K high
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }

        {
            NHandUtils::C3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } } ); // a23 straight
            NHandUtils::C3CardInfo h2( { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } } ); // kqj straight
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::C3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } } ); 
            NHandUtils::C3CardInfo h2( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eDiamonds } } ); 
            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_TRUE( h2.equalTo( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_TRUE( h1.equalTo( true, h2 ) );
        }

        {
            auto p1 = fGame->addPlayer( "Scott" );
            fGame->setStraightsAndFlushesCount( true );

            p1->addCard( fGame->getCard( ECard::eTrey, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eAce, ESuit::eSpades ) );

            EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
            EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
            auto hand = p1->determineHand();
            EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
        }

        {
            NHandUtils::C3CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair of 2s 3 kicker
            NHandUtils::C3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eTrey, ESuit::eHearts ); // pair or 3s 2 kicker
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_TRUE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }
        {
            NHandUtils::C3CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair of 2s 3 kicker
            NHandUtils::C3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );  // pair of 2s 4 kicker
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_TRUE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }
        {
            NHandUtils::C3CardInfo h1( ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eSpades, ECard::eAce, ESuit::eSpades ); // pair of 2s 3 kicker
            EXPECT_TRUE( h1.isFlush() );
            EXPECT_TRUE( h1.straightType().has_value() );
            EXPECT_EQ( NHandUtils::EStraightType::eWheel, h1.straightType().value() );
        }
        {
            NHandUtils::C2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::C2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::C2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eQueen, ESuit::eSpades );
            NHandUtils::C2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
    }
    
    TEST_F( C3CardHandTester, OneOfEachHand )
    {
        auto h1 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades );
        EXPECT_TRUE( h1.isStraightFlush() );
        EXPECT_FALSE( h1.isFourOfAKind() );
        EXPECT_FALSE( h1.isFullHouse() );
        EXPECT_TRUE( h1.isFlush() );
        EXPECT_TRUE( h1.isStraight() );
        EXPECT_FALSE( h1.isWheel() );
        EXPECT_FALSE( h1.isThreeOfAKind() );
        EXPECT_FALSE( h1.isTwoPair() );
        EXPECT_FALSE( h1.isPair() );
        EXPECT_FALSE( h1.isHighCard() );

        auto h2 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eDeuce, ESuit::eClubs );
        EXPECT_FALSE( h2.isStraightFlush() );
        EXPECT_FALSE( h2.isFourOfAKind() );
        EXPECT_FALSE( h2.isFullHouse() );
        EXPECT_FALSE( h2.isFlush() );
        EXPECT_FALSE( h2.isStraight() );
        EXPECT_FALSE( h2.isWheel() );
        EXPECT_TRUE( h2.isThreeOfAKind() );
        EXPECT_FALSE( h2.isTwoPair() );
        EXPECT_FALSE( h2.isPair() );
        EXPECT_FALSE( h2.isHighCard() );

        auto h3 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eHearts );
        EXPECT_FALSE( h3.isStraightFlush() );
        EXPECT_FALSE( h3.isFourOfAKind() );
        EXPECT_FALSE( h3.isFullHouse() );
        EXPECT_FALSE( h3.isFlush() );
        EXPECT_TRUE( h3.isStraight() );
        EXPECT_FALSE( h3.isWheel() );
        EXPECT_FALSE( h3.isThreeOfAKind() );
        EXPECT_FALSE( h3.isTwoPair() );
        EXPECT_FALSE( h3.isPair() );
        EXPECT_FALSE( h3.isHighCard() );

        auto h4 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eAce, ESuit::eHearts );
        EXPECT_FALSE( h4.isStraightFlush() );
        EXPECT_FALSE( h4.isFourOfAKind() );
        EXPECT_FALSE( h4.isFullHouse() );
        EXPECT_FALSE( h4.isFlush() );
        EXPECT_TRUE( h4.isStraight() );
        EXPECT_TRUE( h4.isWheel() );
        EXPECT_FALSE( h4.isThreeOfAKind() );
        EXPECT_FALSE( h4.isTwoPair() );
        EXPECT_FALSE( h4.isPair() );
        EXPECT_FALSE( h4.isHighCard() );

        auto h5 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFive, ESuit::eSpades );
        EXPECT_FALSE( h5.isStraightFlush() );
        EXPECT_FALSE( h5.isFourOfAKind() );
        EXPECT_FALSE( h5.isFullHouse() );
        EXPECT_TRUE( h5.isFlush() );
        EXPECT_FALSE( h5.isStraight() );
        EXPECT_FALSE( h5.isWheel() );
        EXPECT_FALSE( h5.isThreeOfAKind() );
        EXPECT_FALSE( h5.isTwoPair() );
        EXPECT_FALSE( h5.isPair() );
        EXPECT_FALSE( h5.isHighCard() );

        auto h6 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eTrey, ESuit::eClubs );
        EXPECT_FALSE( h6.isStraightFlush() );
        EXPECT_FALSE( h6.isFourOfAKind() );
        EXPECT_FALSE( h6.isFullHouse() );
        EXPECT_FALSE( h6.isFlush() );
        EXPECT_FALSE( h6.isStraight() );
        EXPECT_FALSE( h6.isWheel() );
        EXPECT_FALSE( h6.isThreeOfAKind() );
        EXPECT_FALSE( h6.isTwoPair() );
        EXPECT_TRUE( h6.isPair() );
        EXPECT_FALSE( h6.isHighCard() );

        auto h7 = NHandUtils::C3CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eFive, ESuit::eHearts, ECard::eTrey, ESuit::eClubs );
        EXPECT_FALSE( h7.isStraightFlush() );
        EXPECT_FALSE( h7.isFourOfAKind() );
        EXPECT_FALSE( h7.isFullHouse() );
        EXPECT_FALSE( h7.isFlush() );
        EXPECT_FALSE( h7.isStraight() );
        EXPECT_FALSE( h7.isWheel() );
        EXPECT_FALSE( h7.isThreeOfAKind() );
        EXPECT_FALSE( h7.isTwoPair() );
        EXPECT_FALSE( h7.isPair() );
        EXPECT_TRUE( h7.isHighCard() );

        EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        EXPECT_TRUE( h1.greaterThan( true, h3 ) );
        EXPECT_TRUE( h1.greaterThan( true, h4 ) );
        EXPECT_TRUE( h1.greaterThan( true, h5 ) );
        EXPECT_TRUE( h1.greaterThan( true, h6 ) );
        EXPECT_TRUE( h1.greaterThan( true, h7 ) );

        EXPECT_TRUE( h2.lessThan( true, h1 ) );
        EXPECT_TRUE( h2.greaterThan( true, h3 ) );
        EXPECT_TRUE( h2.greaterThan( true, h4 ) );
        EXPECT_TRUE( h2.greaterThan( true, h5 ) );
        EXPECT_TRUE( h2.greaterThan( true, h6 ) );
        EXPECT_TRUE( h2.greaterThan( true, h7 ) );

        EXPECT_TRUE( h3.lessThan( true, h1 ) );
        EXPECT_TRUE( h3.lessThan( true, h2 ) );
        EXPECT_TRUE( h3.greaterThan( true, h4 ) );
        EXPECT_TRUE( h3.greaterThan( true, h5 ) );
        EXPECT_TRUE( h3.greaterThan( true, h6 ) );
        EXPECT_TRUE( h3.greaterThan( true, h7 ) );

        EXPECT_TRUE( h4.lessThan( true, h1 ) );
        EXPECT_TRUE( h4.lessThan( true, h2 ) );
        EXPECT_TRUE( h4.lessThan( true, h3 ) );
        EXPECT_TRUE( h4.greaterThan( true, h5 ) );
        EXPECT_TRUE( h4.greaterThan( true, h6 ) );
        EXPECT_TRUE( h4.greaterThan( true, h7 ) );

        EXPECT_TRUE( h5.lessThan( true, h1 ) );
        EXPECT_TRUE( h5.lessThan( true, h2 ) );
        EXPECT_TRUE( h5.lessThan( true, h3 ) );
        EXPECT_TRUE( h5.lessThan( true, h4 ) );
        EXPECT_TRUE( h5.greaterThan( true, h6 ) );
        EXPECT_TRUE( h5.greaterThan( true, h7 ) );

        EXPECT_TRUE( h6.lessThan( true, h1 ) );
        EXPECT_TRUE( h6.lessThan( true, h2 ) );
        EXPECT_TRUE( h6.lessThan( true, h3 ) );
        EXPECT_TRUE( h6.lessThan( true, h4 ) );
        EXPECT_TRUE( h6.lessThan( true, h5 ) );
        EXPECT_TRUE( h6.greaterThan( true, h7 ) );

        EXPECT_TRUE( h7.lessThan( true, h1 ) );
        EXPECT_TRUE( h7.lessThan( true, h2 ) );
        EXPECT_TRUE( h7.lessThan( true, h3 ) );
        EXPECT_TRUE( h7.lessThan( true, h4 ) );
        EXPECT_TRUE( h7.lessThan( true, h5 ) );
        EXPECT_TRUE( h7.lessThan( true, h6 ) );
    }

    TEST_F( C3CardHandTester, StraightFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard < ECard::eTrey )
                    continue;

                p1->clearCards();

                if ( highCard == ECard::eTrey )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eDeuce, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for ( auto currCard = ( highCard - 2 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
                EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 2, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C3CardHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );
        for ( auto suit : ESuit() )
        {
            for ( auto && highCard : ECard() )
            {
                if ( highCard <= ECard::eTrey ) // for a flush, the high card can only be a 4 or above, otherwise its a striaght flush
                    continue;

                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, suit ) );
                p1->addCard( fGame->getCard( highCard - 2, suit ) );
                if ( highCard == ECard::eFour )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
                else
                    p1->addCard( fGame->getCard( highCard - 3, suit ) );

                auto hand = p1->determineHand();
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 2, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C3CardHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto&& highCard : ECard() )
        {
            if ( highCard <= ECard::eDeuce )
                continue;

            for( auto && suit : ESuit() )
            {
                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                p1->addCard( fGame->getCard( highCard - 1, ESuit::eDiamonds ) );
                if ( highCard == ECard::eTrey )
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                else
                    p1->addCard( fGame->getCard( highCard - 2, suit ) );

                auto hand = p1->determineHand();
                EXPECT_TRUE( p1->isStraight() );
                EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 2, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C3CardHandTester, ThreeOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& suit : ESuit() )
            {
                p1->clearCards();
                for( auto && skipSuit : ESuit() )
                {
                    if ( suit == skipSuit )
                        continue;

                    p1->addCard( fGame->getCard( highCard, skipSuit ) );
                }
                // now have 3 of a kind

                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eThreeOfAKind, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 0, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C3CardHandTester, Pair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for ( auto && kicker : ECard() )
                    {
                        if ( kicker == highCard )
                            continue;
                        for ( auto&& kickerSuit : ESuit() )
                        {
                            p1->clearCards();

                            p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                            p1->addCard( fGame->getCard( highCard, highSuit2 ) );
                            p1->addCard( fGame->getCard( kicker, kickerSuit ) );

                            auto hand = p1->determineHand();
                            EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
                            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                            auto kickers = std::vector< ECard >( { kicker } );
                            std::sort( kickers.begin(), kickers.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                            EXPECT_EQ( kickers, std::get< 2 >( hand ) );
                        }
                    }
                }
            }
        }
    }

    TEST_F( C3CardHandTester, HighCard )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& card1 : ECard() )
        {
            for ( auto&& suit1 : ESuit() )
            {
                for ( auto&& card2 : ECard() )
                {
                    if ( card1 == card2 )
                        continue;
                    for ( auto&& suit2 : ESuit() )
                    {
                        for ( auto&& card3 : ECard() )
                        {
                            if (    
                                    ( card1 == card2 ) 
                                 || ( card1 == card3 ) 
                                 || ( card2 == card3 )
                                 )
                                continue;

                            for ( auto&& suit3 : ESuit() )
                            {
                                if ( suit3 == suit1 || suit3 == suit2 )
                                    continue;

                                if ( isStraight( std::vector< ECard >( { card1, card2, card3 } ) ) )
                                    continue;

                                p1->clearCards();

                                p1->addCard( fGame->getCard( card1, suit1 ) );
                                p1->addCard( fGame->getCard( card2, suit2 ) );
                                p1->addCard( fGame->getCard( card3, suit3 ) );

                                auto hand = p1->determineHand();
                                EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) )
                                    << "Cards: "
                                    << card1 << suit1 << " "
                                    << card2 << suit2 << " "
                                    << card3 << suit3 << " "
                                    ;

                                auto cards = std::vector< ECard >( { card1, card2, card3 } );
                                std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                                auto kickers = cards;
                                kickers.erase( kickers.begin(), ++kickers.begin() );
                                cards.erase( ++cards.begin(), cards.end() );

                                EXPECT_EQ( cards, std::get< 1 >( hand ) )
                                    << "Cards: "
                                    << card1 << suit1 << " "
                                    << card2 << suit2 << " "
                                    << card3 << suit3 << " "
                                    ;

                                EXPECT_EQ( kickers, std::get< 2 >( hand ) )
                                    << "Cards: "
                                    << card1 << suit1 << " "
                                    << card2 << suit2 << " "
                                    << card3 << suit3 << " "
                                    ;
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C3CardHandTester, AllCardHands )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        auto allHands = getAllCHandHands( 3 );

        EXPECT_EQ( 22100, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 741, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 48, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 52, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 720, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 1096, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 16440, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 12, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 274, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 12, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 274, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    TEST_F( C3CardHandTester, AllCardHands_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        auto allHands = getAllCHandHands( 3 );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 22100, allHands.size() );

        EXPECT_EQ( 455, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 52, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 18304, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 286, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    class C4CardHandTester : public CHandTester
    {
    protected:
        C4CardHandTester() {}
        virtual ~C4CardHandTester() {}
    };

    TEST_F( C4CardHandTester, Basic )
    {
        {
            NHandUtils::C4CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eDeuce, ESuit::eClubs, ECard::eDeuce, ESuit::eDiamonds );
            NHandUtils::C4CardInfo h2( ECard::eSeven, ESuit::eSpades, ECard::eSeven, ESuit::eHearts, ECard::eSeven, ESuit::eClubs, ECard::eSeven, ESuit::eDiamonds );
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_TRUE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );

            EXPECT_TRUE( h1.lessThan( false, h2 ) );
            EXPECT_FALSE( h1.greaterThan( false, h2 ) );
            EXPECT_FALSE( h1.equalTo( false, h2 ) );

            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );

            EXPECT_TRUE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );
        }
        {
            NHandUtils::C4CardInfo h1( ECard::eSeven, ESuit::eSpades, ECard::eSeven, ESuit::eHearts, ECard::eSeven, ESuit::eDiamonds, ECard::eSeven, ESuit::eClubs );
            NHandUtils::C4CardInfo h2( ECard::eSeven, ESuit::eDiamonds, ECard::eSeven, ESuit::eClubs, ECard::eSix, ESuit::eHearts, ECard::eSix, ESuit::eClubs );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );

            EXPECT_FALSE( h1.lessThan( false, h2 ) );
            EXPECT_TRUE( h1.greaterThan( false, h2 ) );
            EXPECT_FALSE( h1.equalTo( false, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );
        }
    }

    TEST_F( C4CardHandTester, AllCardHandsByCardInfo )
    {
        auto allHands = getAllCardInfoHands( 4 );

        EXPECT_EQ( 270725, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 2535, std::get< 0 >( analyzedHands ).size() );

        CompareHandOrder( std::get< 1 >( analyzedHands ), NHandUtils::C4CardInfo() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 13, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 44, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 2496, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 2772, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 2808, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 2816, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 82368, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 177408, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 11, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 11, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 78, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 704, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 704, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::ePair ] );

        NHandUtils::gComputeAllHands = true;
        auto cardInfo = std::make_unique< NHandUtils::C4CardInfo >();
        cardInfo->generateAllCardHands();
    }

    TEST_F( C4CardHandTester, OneOfEachHand )
    {
        auto h1 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eDeuce, ESuit::eClubs, ECard::eDeuce, ESuit::eDiamonds );
        EXPECT_FALSE( h1.isStraightFlush() );
        EXPECT_TRUE( h1.isFourOfAKind() );
        EXPECT_FALSE( h1.isFullHouse() );
        EXPECT_FALSE( h1.isFlush() );
        EXPECT_FALSE( h1.isStraight() );
        EXPECT_FALSE( h1.isWheel() );
        EXPECT_FALSE( h1.isThreeOfAKind() );
        EXPECT_FALSE( h1.isTwoPair() );
        EXPECT_FALSE( h1.isPair() );
        EXPECT_FALSE( h1.isHighCard() );

        auto h2 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eSpades ); 
        EXPECT_TRUE( h2.isStraightFlush() );
        EXPECT_FALSE( h2.isFourOfAKind() );
        EXPECT_FALSE( h2.isFullHouse() );
        EXPECT_TRUE( h2.isFlush() );
        EXPECT_TRUE( h2.isStraight() );
        EXPECT_FALSE( h2.isWheel() );
        EXPECT_FALSE( h2.isThreeOfAKind() );
        EXPECT_FALSE( h2.isTwoPair() );
        EXPECT_FALSE( h2.isPair() );
        EXPECT_FALSE( h2.isHighCard() );

        auto h3 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eDeuce, ESuit::eClubs, ECard::eTrey, ESuit::eDiamonds );
        EXPECT_FALSE( h3.isStraightFlush() );
        EXPECT_FALSE( h3.isFourOfAKind() );
        EXPECT_FALSE( h3.isFullHouse() );
        EXPECT_FALSE( h3.isFlush() );
        EXPECT_FALSE( h3.isStraight() );
        EXPECT_FALSE( h3.isWheel() );
        EXPECT_TRUE( h3.isThreeOfAKind() );
        EXPECT_FALSE( h3.isTwoPair() );
        EXPECT_FALSE( h3.isPair() );
        EXPECT_FALSE( h3.isHighCard() );

        auto h4 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eHearts );
        EXPECT_FALSE( h4.isStraightFlush() );
        EXPECT_FALSE( h4.isFourOfAKind() );
        EXPECT_FALSE( h4.isFullHouse() );
        EXPECT_FALSE( h4.isFlush() );
        EXPECT_TRUE( h4.isStraight() );
        EXPECT_FALSE( h4.isWheel() );
        EXPECT_FALSE( h4.isThreeOfAKind() );
        EXPECT_FALSE( h4.isTwoPair() );
        EXPECT_FALSE( h4.isPair() );
        EXPECT_FALSE( h4.isHighCard() );

        auto h5 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eAce, ESuit::eHearts );
        EXPECT_FALSE( h5.isStraightFlush() );
        EXPECT_FALSE( h5.isFourOfAKind() );
        EXPECT_FALSE( h5.isFullHouse() );
        EXPECT_FALSE( h5.isFlush() );
        EXPECT_TRUE( h5.isStraight() );
        EXPECT_TRUE( h5.isWheel() );
        EXPECT_FALSE( h5.isThreeOfAKind() );
        EXPECT_FALSE( h5.isTwoPair() );
        EXPECT_FALSE( h5.isPair() );
        EXPECT_FALSE( h5.isHighCard() );

        auto h6 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eTrey, ESuit::eClubs, ECard::eTrey, ESuit::eDiamonds );
        EXPECT_FALSE( h6.isStraightFlush() );
        EXPECT_FALSE( h6.isFourOfAKind() );
        EXPECT_FALSE( h6.isFullHouse() );
        EXPECT_FALSE( h6.isFlush() );
        EXPECT_FALSE( h6.isStraight() );
        EXPECT_FALSE( h6.isWheel() );
        EXPECT_FALSE( h6.isThreeOfAKind() );
        EXPECT_TRUE( h6.isTwoPair() );
        EXPECT_FALSE( h6.isPair() );
        EXPECT_FALSE( h6.isHighCard() );

        auto h7 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eSix, ESuit::eSpades );
        EXPECT_FALSE( h7.isStraightFlush() );
        EXPECT_FALSE( h7.isFourOfAKind() );
        EXPECT_FALSE( h7.isFullHouse() );
        EXPECT_TRUE( h7.isFlush() );
        EXPECT_FALSE( h7.isStraight() );
        EXPECT_FALSE( h7.isWheel() );
        EXPECT_FALSE( h7.isThreeOfAKind() );
        EXPECT_FALSE( h7.isTwoPair() );
        EXPECT_FALSE( h7.isPair() );
        EXPECT_FALSE( h7.isHighCard() );

        auto h8 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts, ECard::eTrey, ESuit::eClubs, ECard::eAce, ESuit::eDiamonds );
        EXPECT_FALSE( h8.isStraightFlush() );
        EXPECT_FALSE( h8.isFourOfAKind() );
        EXPECT_FALSE( h8.isFullHouse() );
        EXPECT_FALSE( h8.isFlush() );
        EXPECT_FALSE( h8.isStraight() );
        EXPECT_FALSE( h8.isWheel() );
        EXPECT_FALSE( h8.isThreeOfAKind() );
        EXPECT_FALSE( h8.isTwoPair() );
        EXPECT_TRUE( h8.isPair() );
        EXPECT_FALSE( h8.isHighCard() );

        auto h9 = NHandUtils::C4CardInfo( ECard::eDeuce, ESuit::eSpades, ECard::eFive, ESuit::eHearts, ECard::eTrey, ESuit::eClubs, ECard::eAce, ESuit::eDiamonds );
        EXPECT_FALSE( h9.isStraightFlush() );
        EXPECT_FALSE( h9.isFourOfAKind() );
        EXPECT_FALSE( h9.isFullHouse() );
        EXPECT_FALSE( h9.isFlush() );
        EXPECT_FALSE( h9.isStraight() );
        EXPECT_FALSE( h9.isWheel() );
        EXPECT_FALSE( h9.isThreeOfAKind() );
        EXPECT_FALSE( h9.isTwoPair() );
        EXPECT_FALSE( h9.isPair() );
        EXPECT_TRUE( h9.isHighCard() );

        EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        EXPECT_TRUE( h1.greaterThan( true, h3 ) );
        EXPECT_TRUE( h1.greaterThan( true, h4 ) );
        EXPECT_TRUE( h1.greaterThan( true, h5 ) );
        EXPECT_TRUE( h1.greaterThan( true, h6 ) );
        EXPECT_TRUE( h1.greaterThan( true, h7 ) );
        EXPECT_TRUE( h1.greaterThan( true, h8 ) );
        EXPECT_TRUE( h1.greaterThan( true, h9 ) );

        EXPECT_TRUE( h2.lessThan( true, h1 ) );
        EXPECT_TRUE( h2.greaterThan( true, h3 ) );
        EXPECT_TRUE( h2.greaterThan( true, h4 ) );
        EXPECT_TRUE( h2.greaterThan( true, h5 ) );
        EXPECT_TRUE( h2.greaterThan( true, h6 ) );
        EXPECT_TRUE( h2.greaterThan( true, h7 ) );
        EXPECT_TRUE( h2.greaterThan( true, h8 ) );
        EXPECT_TRUE( h2.greaterThan( true, h9 ) );

        EXPECT_TRUE( h3.lessThan( true, h1 ) );
        EXPECT_TRUE( h3.lessThan( true, h2 ) );
        EXPECT_TRUE( h3.greaterThan( true, h4 ) );
        EXPECT_TRUE( h3.greaterThan( true, h5 ) );
        EXPECT_TRUE( h3.greaterThan( true, h6 ) );
        EXPECT_TRUE( h3.greaterThan( true, h7 ) );
        EXPECT_TRUE( h3.greaterThan( true, h8 ) );
        EXPECT_TRUE( h3.greaterThan( true, h9 ) );

        EXPECT_TRUE( h4.lessThan( true, h1 ) );
        EXPECT_TRUE( h4.lessThan( true, h2 ) );
        EXPECT_TRUE( h4.lessThan( true, h3 ) );
        EXPECT_TRUE( h4.greaterThan( true, h5 ) );
        EXPECT_TRUE( h4.greaterThan( true, h6 ) );
        EXPECT_TRUE( h4.greaterThan( true, h7 ) );
        EXPECT_TRUE( h4.greaterThan( true, h8 ) );
        EXPECT_TRUE( h4.greaterThan( true, h9 ) );

        EXPECT_TRUE( h5.lessThan( true, h1 ) );
        EXPECT_TRUE( h5.lessThan( true, h2 ) );
        EXPECT_TRUE( h5.lessThan( true, h3 ) );
        EXPECT_TRUE( h5.lessThan( true, h4 ) );
        EXPECT_TRUE( h5.greaterThan( true, h6 ) );
        EXPECT_TRUE( h5.greaterThan( true, h7 ) );
        EXPECT_TRUE( h5.greaterThan( true, h8 ) );
        EXPECT_TRUE( h5.greaterThan( true, h9 ) );

        EXPECT_TRUE( h6.lessThan( true, h1 ) );
        EXPECT_TRUE( h6.lessThan( true, h2 ) );
        EXPECT_TRUE( h6.lessThan( true, h3 ) );
        EXPECT_TRUE( h6.lessThan( true, h4 ) );
        EXPECT_TRUE( h6.lessThan( true, h5 ) );
        EXPECT_TRUE( h6.greaterThan( true, h7 ) );
        EXPECT_TRUE( h6.greaterThan( true, h8 ) );
        EXPECT_TRUE( h6.greaterThan( true, h9 ) );

        EXPECT_TRUE( h7.lessThan( true, h1 ) );
        EXPECT_TRUE( h7.lessThan( true, h2 ) );
        EXPECT_TRUE( h7.lessThan( true, h3 ) );
        EXPECT_TRUE( h7.lessThan( true, h4 ) );
        EXPECT_TRUE( h7.lessThan( true, h5 ) );
        EXPECT_TRUE( h7.lessThan( true, h6 ) );
        EXPECT_TRUE( h7.greaterThan( true, h8 ) );
        EXPECT_TRUE( h7.greaterThan( true, h9 ) );

        EXPECT_TRUE( h8.lessThan( true, h1 ) );
        EXPECT_TRUE( h8.lessThan( true, h2 ) );
        EXPECT_TRUE( h8.lessThan( true, h3 ) );
        EXPECT_TRUE( h8.lessThan( true, h4 ) );
        EXPECT_TRUE( h8.lessThan( true, h5 ) );
        EXPECT_TRUE( h8.lessThan( true, h6 ) );
        EXPECT_TRUE( h8.lessThan( true, h7 ) );
        EXPECT_TRUE( h8.greaterThan( true, h9 ) );

        EXPECT_TRUE( h9.lessThan( true, h1 ) );
        EXPECT_TRUE( h9.lessThan( true, h2 ) );
        EXPECT_TRUE( h9.lessThan( true, h3 ) );
        EXPECT_TRUE( h9.lessThan( true, h4 ) );
        EXPECT_TRUE( h9.lessThan( true, h5 ) );
        EXPECT_TRUE( h9.lessThan( true, h6 ) );
        EXPECT_TRUE( h9.lessThan( true, h7 ) );
        EXPECT_TRUE( h9.lessThan( true, h8 ) );
    }

    TEST_F( C4CardHandTester, Basic2 )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        p1->addCard( fGame->getCard( ECard::eTrey, ESuit::eSpades ) );
        p1->addCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );
        p1->addCard( fGame->getCard( ECard::eAce, ESuit::eSpades ) );
        p1->addCard( fGame->getCard( ECard::eFour, ESuit::eSpades ) );

        EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
        EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
        auto hand = p1->determineHand();
        EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
    }

    TEST_F( C4CardHandTester, StraightFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eFour )
                    continue;

                p1->clearCards();

                if ( highCard == ECard::eFour )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eTrey, suit ) );
                    p1->addCard( fGame->getCard( ECard::eDeuce, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for ( auto currCard = ( highCard - 3 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
                EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 3, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C4CardHandTester, FourOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            p1->clearCards();

            p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
            p1->addCard( fGame->getCard( highCard, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( highCard, ESuit::eHearts ) );
            p1->addCard( fGame->getCard( highCard, ESuit::eDiamonds ) );

            auto hand = p1->determineHand();
            EXPECT_EQ( EHand::eFourOfAKind, std::get< 0 >( hand ) ) << "4 of Kind: " << highCard;
            ASSERT_EQ( 1, std::get< 1 >( hand ).size() ) << "4 of Kind: " << highCard;
            ASSERT_EQ( 0, std::get< 2 >( hand ).size() ) << "4 of Kind: " << highCard;
            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() ) << "4 of Kind: " << highCard;
        }
    }

    TEST_F( C4CardHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eFour ) // for a flush, the high card can only be a 5 or above, otherwise its a striaght flush
                    continue;

                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, suit ) );
                p1->addCard( fGame->getCard( highCard - 2, suit ) );
                p1->addCard( fGame->getCard( highCard - 3, suit ) );
                if ( highCard == ECard::eFive )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
                else
                    p1->addCard( fGame->getCard( highCard - 4, suit ) );

                EXPECT_TRUE( p1->isFlush() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 3, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C4CardHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto&& highCard : ECard() )
        {
            if ( highCard <= ECard::eTrey )
                continue;

            for ( auto&& suit : ESuit() )
            {
                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                p1->addCard( fGame->getCard( highCard - 1, ESuit::eDiamonds ) );
                p1->addCard( fGame->getCard( highCard - 2, ESuit::eHearts ) );
                if ( highCard == ECard::eFour )
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                else
                    p1->addCard( fGame->getCard( highCard - 3, suit ) );

                EXPECT_TRUE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 3, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C4CardHandTester, ThreeOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& suit : ESuit() )
            {
                for ( auto&& kickCard1 : ECard() )
                {
                    if ( kickCard1 == highCard )
                        continue;
                    for ( auto&& kickSuit1 : ESuit() )
                    {
                        p1->clearCards();
                        for ( auto&& skipSuit : ESuit() )
                        {
                            if ( suit == skipSuit )
                                continue;

                            p1->addCard( fGame->getCard( highCard, skipSuit ) );
                        }
                        // now have 3 of a kind

                        p1->addCard( fGame->getCard( kickCard1, kickSuit1 ) );

                        auto hand = p1->determineHand();
                        EXPECT_EQ( EHand::eThreeOfAKind, std::get< 0 >( hand ) );
                        ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                        ASSERT_EQ( 1, std::get< 2 >( hand ).size() );
                        EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                        EXPECT_EQ( kickCard1, std::get< 2 >( hand ).front() );
                    }
                }
            }
        }
    }

    TEST_F( C4CardHandTester, TwoPair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for ( auto&& lowCard : ECard() )
                    {
                        if ( lowCard == highCard )
                            continue;

                        for ( auto&& lowSuit1 : ESuit() )
                        {
                            for ( auto&& lowSuit2 : ESuit() )
                            {
                                if ( lowSuit1 == lowSuit2 )
                                    continue;

                                p1->clearCards();

                                p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                                p1->addCard( fGame->getCard( highCard, highSuit2 ) );
                                p1->addCard( fGame->getCard( lowCard, lowSuit1 ) );
                                p1->addCard( fGame->getCard( lowCard, lowSuit2 ) );

                                auto hand = p1->determineHand();
                                EXPECT_EQ( EHand::eTwoPair, std::get< 0 >( hand ) );
                                ASSERT_EQ( 2, std::get< 1 >( hand ).size() );
                                ASSERT_EQ( 0, std::get< 2 >( hand ).size() );

                                if ( highCard > lowCard )
                                {
                                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                                    EXPECT_EQ( lowCard, std::get< 1 >( hand ).back() );
                                }
                                else
                                {
                                    EXPECT_EQ( lowCard, std::get< 1 >( hand ).front() );
                                    EXPECT_EQ( highCard, std::get< 1 >( hand ).back() );
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C4CardHandTester, Pair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for ( auto&& kicker1 : ECard() )
                    {
                        if ( kicker1 == highCard )
                            continue;
                        for ( auto&& kickerSuit1 : ESuit() )
                        {
                            for ( auto&& kicker2 : ECard() )
                            {
                                if ( ( kicker2 == highCard ) || ( kicker2 == kicker1 ) )
                                    continue;

                                for ( auto&& kickerSuit2 : ESuit() )
                                {
                                    p1->clearCards();

                                    p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                                    p1->addCard( fGame->getCard( highCard, highSuit2 ) );
                                    p1->addCard( fGame->getCard( kicker1, kickerSuit1 ) );
                                    p1->addCard( fGame->getCard( kicker2, kickerSuit2 ) );

                                    auto hand = p1->determineHand();
                                    EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
                                    ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                                    auto kickers = std::vector< ECard >( { kicker1, kicker2 } );
                                    std::sort( kickers.begin(), kickers.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                                    EXPECT_EQ( kickers, std::get< 2 >( hand ) );
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C4CardHandTester, HighCard )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& card1 : ECard() )
        {
            for ( auto&& suit1 : ESuit() )
            {
                for ( auto&& card2 : ECard() )
                {
                    if ( card1 == card2 )
                        continue;
                    for ( auto&& suit2 : ESuit() )
                    {
                        for ( auto&& card3 : ECard() )
                        {
                            if (
                                ( card1 == card2 )
                                || ( card1 == card3 )
                                || ( card2 == card3 )
                                )
                                continue;

                            for ( auto&& suit3 : ESuit() )
                            {
                                for ( auto&& card4 : ECard() )
                                {
                                    if (
                                        ( card1 == card2 )
                                        || ( card1 == card3 )
                                        || ( card1 == card4 )
                                        || ( card2 == card3 )
                                        || ( card2 == card4 )
                                        || ( card3 == card4 )
                                        )
                                        continue;
                                    for ( auto&& suit4 : ESuit() )
                                    {
                                        if ( isStraight( std::vector< ECard >( { card1, card2, card3, card4 } ) ) )
                                            continue;

                                        if ( suit1 == suit4 )
                                            continue; // prevent flush

                                        p1->clearCards();

                                        p1->addCard( fGame->getCard( card1, suit1 ) );
                                        p1->addCard( fGame->getCard( card2, suit2 ) );
                                        p1->addCard( fGame->getCard( card3, suit3 ) );
                                        p1->addCard( fGame->getCard( card4, suit4 ) );

                                        auto hand = p1->determineHand();
                                        EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) )
                                            << "Cards: "
                                            << card1 << suit1 << " "
                                            << card2 << suit2 << " "
                                            << card3 << suit3 << " "
                                            << card4 << suit4 << " "
                                            ;

                                        auto cards = std::vector< ECard >( { card1, card2, card3, card4 } );
                                        std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                                        auto kickers = cards;
                                        kickers.erase( kickers.begin(), ++kickers.begin() );
                                        cards.erase( ++cards.begin(), cards.end() );

                                        EXPECT_EQ( cards, std::get< 1 >( hand ) )
                                            << "Cards: "
                                            << card1 << suit1 << " "
                                            << card2 << suit2 << " "
                                            << card3 << suit3 << " "
                                            << card4 << suit4 << " "
                                            ;

                                        EXPECT_EQ( kickers, std::get< 2 >( hand ) )
                                            << "Cards: "
                                            << card1 << suit1 << " "
                                            << card2 << suit2 << " "
                                            << card3 << suit3 << " "
                                            << card4 << suit4 << " "
                                            ;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C4CardHandTester, AllCardHands )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        auto allHands = getAllCHandHands( 4 );

        EXPECT_EQ( 270725, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 2535, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 13, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 44, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 2496, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 2772, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 2808, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 2816, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 82368, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 177408, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 11, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 11, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 78, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 704, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 704, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
    }

    TEST_F( C4CardHandTester, AllCardHands_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        auto allHands = getAllCHandHands( 4 );

        EXPECT_EQ( 270725, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 1820, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 13, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 2496, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 2808, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 82368, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 183040, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 13, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 0, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 78, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 715, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }


    class C5CardHandTester : public CHandTester
    {
    protected:
        C5CardHandTester() {}
        virtual ~C5CardHandTester() {}
    };

    TEST_F( C5CardHandTester, Basic )
    {
        {
            NHandUtils::C5CardInfo h1( ECard::eTrey, ESuit::eHearts, ECard::eKing, ESuit::eDiamonds, ECard::eAce, ESuit::eSpades, ECard::eAce, ESuit::eHearts, ECard::eAce, ESuit::eDiamonds );
            NHandUtils::C5CardInfo h2( ECard::eFour, ESuit::eClubs, ECard::eSeven, ESuit::eSpades, ECard::eEight, ESuit::eSpades, ECard::eEight, ESuit::eHearts, ECard::eEight, ESuit::eDiamonds );

            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );

            EXPECT_FALSE( h1.lessThan( false, h2 ) );
            EXPECT_FALSE( h1.equalTo( false, h2 ) );
            EXPECT_TRUE( h1.greaterThan( false, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        }

        {
            NHandUtils::C5CardInfo h1( ECard::eTrey, ESuit::eHearts, ECard::eKing, ESuit::eDiamonds, ECard::eAce, ESuit::eSpades, ECard::eAce, ESuit::eHearts, ECard::eAce, ESuit::eDiamonds );
            NHandUtils::C5CardInfo h2( ECard::eFour, ESuit::eClubs, ECard::eSeven, ESuit::eSpades, ECard::eEight, ESuit::eSpades, ECard::eEight, ESuit::eHearts, ECard::eEight, ESuit::eDiamonds );

            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );

            EXPECT_FALSE( h1.lessThan( false, h2 ) );
            EXPECT_FALSE( h1.equalTo( false, h2 ) );
            EXPECT_TRUE( h1.greaterThan( false, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        }
        {
            NHandUtils::C5CardInfo h1( ECard::eTrey, ESuit::eHearts, ECard::eTrey, ESuit::eDiamonds, ECard::eTrey, ESuit::eClubs, ECard::eFour, ESuit::eSpades, ECard::eFour, ESuit::eHearts );
            NHandUtils::C5CardInfo h2( ECard::eDeuce, ESuit::eHearts, ECard::eDeuce, ESuit::eDiamonds, ECard::eDeuce, ESuit::eClubs, ECard::eFive, ESuit::eSpades, ECard::eFive, ESuit::eHearts );

            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );

            EXPECT_FALSE( h1.lessThan( false, h2 ) );
            EXPECT_FALSE( h1.equalTo( false, h2 ) );
            EXPECT_TRUE( h1.greaterThan( false, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
        }
    }
    
    TEST_F( C5CardHandTester, AllCardHandsByCardInfo )
    {
        auto allHands = getAllCardInfoHands( 5 );

        EXPECT_EQ( 2598960, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 7462, std::get< 0 >( analyzedHands ).size() );

        CompareHandOrder( std::get< 1 >( analyzedHands ), NHandUtils::C5CardInfo() );

        EXPECT_EQ( 40, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 624, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 5108, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10200, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 54912, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 123552, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 1098240, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1302540, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 2860, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );

        NHandUtils::gComputeAllHands = true;
        auto cardInfo = std::make_unique< NHandUtils::C5CardInfo >();
        cardInfo->generateAllCardHands();
    }

    TEST_F( C5CardHandTester, FourOfAKind_CardInfo )
    {
        std::list< std::shared_ptr< NHandUtils::CCardInfo > > allHands;

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& kicker : ECard() )
            {
                if ( kicker == highCard )
                    continue;

                for ( auto suit : ESuit() )
                {
                    auto cardInfo = std::make_shared< NHandUtils::C5CardInfo >( highCard, ESuit::eClubs, highCard, ESuit::eDiamonds, highCard, ESuit::eHearts, highCard, ESuit::eSpades, kicker, suit );
                    allHands.push_back( cardInfo );
                }
            }
        }

        for ( auto&& highCard : ECard() )
        {
            for ( auto highSuit : ESuit() )
            {
                for ( auto&& kicker : ECard() )
                {
                    if ( kicker == highCard )
                        continue;

                    for ( auto ksuit1 : ESuit() )
                    {
                        for ( auto ksuit2 : ESuit() )
                        {
                            if ( ksuit1 == ksuit2 )
                                continue;

                            THand cards;
                            // for highsuit we add ALL but the highSuit of the high card
                            for ( auto currSuit : ESuit() )
                            {
                                if ( highSuit == currSuit )
                                    continue;

                                cards.push_back( TCard( highCard, currSuit ) );
                            }
                            // now have 3 of a kind

                            // add the pair
                            cards.push_back( TCard( kicker, ksuit1 ) );
                            cards.push_back( TCard( kicker, ksuit2 ) );
                            auto cardInfo = std::make_shared< NHandUtils::C5CardInfo >( cards );
                            allHands.push_back( cardInfo );
                        }
                    }
                }
            }
        }

        auto analyzedHands = getUniqueHands( allHands );
    }

    TEST_F( C5CardHandTester, CheckBitValues )
    {
        static std::map< std::pair< ESuit, ECard >, int > map =
        {
             { {     ESuit::eClubs,   ECard::eDeuce }, 0x00018002 }
            ,{ {     ESuit::eClubs, ECard::eTrey }, 0x00028103 }
            ,{ {     ESuit::eClubs,  ECard::eFour }, 0x00048205 }
            ,{ {     ESuit::eClubs,  ECard::eFive }, 0x00088307 }
            ,{ {     ESuit::eClubs,   ECard::eSix }, 0x0010840b }
            ,{ {     ESuit::eClubs, ECard::eSeven }, 0x0020850d }
            ,{ {     ESuit::eClubs, ECard::eEight }, 0x00408611 }
            ,{ {     ESuit::eClubs,  ECard::eNine }, 0x00808713 }
            ,{ {     ESuit::eClubs,   ECard::eTen }, 0x01008817 }
            ,{ {     ESuit::eClubs,  ECard::eJack }, 0x0200891d }
            ,{ {     ESuit::eClubs, ECard::eQueen }, 0x04008a1f }
            ,{ {     ESuit::eClubs,  ECard::eKing }, 0x08008b25 }
            ,{ {     ESuit::eClubs,   ECard::eAce }, 0x10008c29 }
            ,{ {  ESuit::eDiamonds,   ECard::eDeuce }, 0x00014002 }
            ,{ {  ESuit::eDiamonds, ECard::eTrey }, 0x00024103 }
            ,{ {  ESuit::eDiamonds,  ECard::eFour }, 0x00044205 }
            ,{ {  ESuit::eDiamonds,  ECard::eFive }, 0x00084307 }
            ,{ {  ESuit::eDiamonds,   ECard::eSix }, 0x0010440b }
            ,{ {  ESuit::eDiamonds, ECard::eSeven }, 0x0020450d }
            ,{ {  ESuit::eDiamonds, ECard::eEight }, 0x00404611 }
            ,{ {  ESuit::eDiamonds,  ECard::eNine }, 0x00804713 }
            ,{ {  ESuit::eDiamonds,   ECard::eTen }, 0x01004817 }
            ,{ {  ESuit::eDiamonds,  ECard::eJack }, 0x0200491d }
            ,{ {  ESuit::eDiamonds, ECard::eQueen }, 0x04004a1f }
            ,{ {  ESuit::eDiamonds,  ECard::eKing }, 0x08004b25 }
            ,{ {  ESuit::eDiamonds,   ECard::eAce }, 0x10004c29 }
            ,{ {    ESuit::eHearts,   ECard::eDeuce }, 0x00012002 }
            ,{ {    ESuit::eHearts, ECard::eTrey }, 0x00022103 }
            ,{ {    ESuit::eHearts,  ECard::eFour }, 0x00042205 }
            ,{ {    ESuit::eHearts,  ECard::eFive }, 0x00082307 }
            ,{ {    ESuit::eHearts,   ECard::eSix }, 0x0010240b }
            ,{ {    ESuit::eHearts, ECard::eSeven }, 0x0020250d }
            ,{ {    ESuit::eHearts, ECard::eEight }, 0x00402611 }
            ,{ {    ESuit::eHearts,  ECard::eNine }, 0x00802713 }
            ,{ {    ESuit::eHearts,   ECard::eTen }, 0x01002817 }
            ,{ {    ESuit::eHearts,  ECard::eJack }, 0x0200291d }
            ,{ {    ESuit::eHearts, ECard::eQueen }, 0x04002a1f }
            ,{ {    ESuit::eHearts,  ECard::eKing }, 0x08002b25 }
            ,{ {    ESuit::eHearts,   ECard::eAce }, 0x10002c29 }
            ,{ {    ESuit::eSpades,   ECard::eDeuce }, 0x00011002 }
            ,{ {    ESuit::eSpades, ECard::eTrey }, 0x00021103 }
            ,{ {    ESuit::eSpades,  ECard::eFour }, 0x00041205 }
            ,{ {    ESuit::eSpades,  ECard::eFive }, 0x00081307 }
            ,{ {    ESuit::eSpades,   ECard::eSix }, 0x0010140b }
            ,{ {    ESuit::eSpades, ECard::eSeven }, 0x0020150d }
            ,{ {    ESuit::eSpades, ECard::eEight }, 0x00401611 }
            ,{ {    ESuit::eSpades,  ECard::eNine }, 0x00801713 }
            ,{ {    ESuit::eSpades,   ECard::eTen }, 0x01001817 }
            ,{ {    ESuit::eSpades,  ECard::eJack }, 0x0200191d }
            ,{ {    ESuit::eSpades, ECard::eQueen }, 0x04001a1f }
            ,{ {    ESuit::eSpades,  ECard::eKing }, 0x08001b25 }
            ,{ {    ESuit::eSpades,   ECard::eAce }, 0x10001c29 }
        };

        auto allCards = getAllCardsVector();
        for ( auto&& ii : allCards )
        {
            auto card = ii->getCard();
            auto suit = ii->getSuit();
            auto bitValue = ii->bitValue();

            auto pos = map.find( std::make_pair( suit, card ) );
            ASSERT_TRUE( pos != map.end() );
            EXPECT_EQ( ( *pos ).second, bitValue.to_ulong() );
        }
    }

    TEST_F( C5CardHandTester, StraightFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eFour )
                    continue;

                p1->clearCards();

                if ( highCard == ECard::eFive )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eFour, suit ) );
                    p1->addCard( fGame->getCard( ECard::eTrey, suit ) );
                    p1->addCard( fGame->getCard( ECard::eDeuce, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for ( auto currCard = ( highCard - 4 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_TRUE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C5CardHandTester, FourOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& kicker : ECard() )
            {
                if ( kicker == highCard )
                    continue;

                for ( auto suit : ESuit() )
                {
                    p1->clearCards();

                    p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                    p1->addCard( fGame->getCard( highCard, ESuit::eSpades ) );
                    p1->addCard( fGame->getCard( highCard, ESuit::eHearts ) );
                    p1->addCard( fGame->getCard( highCard, ESuit::eDiamonds ) );

                    p1->addCard( fGame->getCard( kicker, suit ) );

                    auto hand = p1->determineHand();
                    EXPECT_EQ( EHand::eFourOfAKind, std::get< 0 >( hand ) ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
                    ASSERT_EQ( 1, std::get< 1 >( hand ).size() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
                    ASSERT_EQ( 1, std::get< 2 >( hand ).size() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
                    EXPECT_EQ( kicker, std::get< 2 >( hand ).front() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
                }
            }
        }
    }

    TEST_F( C5CardHandTester, FullHouse )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto highSuit : ESuit() )
            {
                for ( auto&& kicker : ECard() )
                {
                    if ( kicker == highCard )
                        continue;

                    for ( auto ksuit1 : ESuit() )
                    {
                        for ( auto ksuit2 : ESuit() )
                        {
                            if ( ksuit1 == ksuit2 )
                                continue;

                            p1->clearCards();

                            // for highsuit we add ALL but the highSuit of the high card
                            for ( auto currSuit : ESuit() )
                            {
                                if ( highSuit == currSuit )
                                    continue;

                                p1->addCard( fGame->getCard( highCard, currSuit ) );
                            }
                            // now have 3 of a kind

                            // add the pair
                            p1->addCard( fGame->getCard( kicker, ksuit1 ) );
                            p1->addCard( fGame->getCard( kicker, ksuit2 ) );


                            auto hand = p1->determineHand();
                            EXPECT_EQ( EHand::eFullHouse, std::get< 0 >( hand ) ) << "Full House: " << highCard << " Kicker: " << kicker << " Skipped High Suit: " << highSuit << " Kicker Suits: " << ksuit1 << " " << ksuit2;
                            ASSERT_EQ( 1, std::get< 1 >( hand ).size() ) << "Full House: " << highCard << " Kicker: " << kicker << " Skipped High Suit: " << highSuit << " Kicker Suits: " << ksuit1 << " " << ksuit2;
                            ASSERT_EQ( 1, std::get< 2 >( hand ).size() ) << "Full House: " << highCard << " Kicker: " << kicker << " Skipped High Suit: " << highSuit << " Kicker Suits: " << ksuit1 << " " << ksuit2;
                            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() ) << "Full House: " << highCard << " Kicker: " << kicker << " Skipped High Suit: " << highSuit << " Kicker Suits: " << ksuit1 << " " << ksuit2;
                            EXPECT_EQ( kicker, std::get< 2 >( hand ).front() ) << "Full House: " << highCard << " Kicker: " << kicker << " Skipped High Suit: " << highSuit << " Kicker Suits: " << ksuit1 << " " << ksuit2;
                        }
                    }
                }
            }
        }
    }

    TEST_F( C5CardHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto suit : ESuit() )
        {
            for ( auto&& highCard : ECard() )
            {
                if ( highCard <= ECard::eFive ) // for a flush, the high card can only be a 6 or above, otherwise its a striaght flush
                    continue;

                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, suit ) );
                p1->addCard( fGame->getCard( highCard - 2, suit ) );
                p1->addCard( fGame->getCard( highCard - 3, suit ) );
                p1->addCard( fGame->getCard( highCard - 4, suit ) );
                if ( highCard == ECard::eSix )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
                else
                    p1->addCard( fGame->getCard( highCard - 5, suit ) );

                EXPECT_TRUE( p1->isFlush() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C5CardHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        for ( auto&& highCard : ECard() )
        {
            if ( highCard <= ECard::eFour )
                continue;

            for ( auto&& suit : ESuit() )
            {
                p1->clearCards();

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                p1->addCard( fGame->getCard( highCard - 1, ESuit::eDiamonds ) );
                p1->addCard( fGame->getCard( highCard - 2, ESuit::eHearts ) );
                p1->addCard( fGame->getCard( highCard - 3, ESuit::eSpades ) );
                if ( highCard == ECard::eFive )
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                else
                    p1->addCard( fGame->getCard( highCard - 4, suit ) );

                EXPECT_TRUE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( C5CardHandTester, ThreeOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& suit : ESuit() )
            {
                for ( auto&& kickCard1 : ECard() )
                {
                    if ( kickCard1 == highCard )
                        continue;
                    for ( auto&& kickCard2 : ECard() )
                    {
                        if ( kickCard1 == kickCard2 )
                            continue;

                        if ( kickCard2 == highCard )
                            continue;

                        for ( auto&& kickSuit1 : ESuit() )
                        {
                            for ( auto&& kickSuit2 : ESuit() )
                            {
                                p1->clearCards();
                                for ( auto&& skipSuit : ESuit() )
                                {
                                    if ( suit == skipSuit )
                                        continue;

                                    p1->addCard( fGame->getCard( highCard, skipSuit ) );
                                }
                                // now have 3 of a kind

                                p1->addCard( fGame->getCard( kickCard1, kickSuit1 ) );
                                p1->addCard( fGame->getCard( kickCard2, kickSuit2 ) );

                                auto hand = p1->determineHand();
                                EXPECT_EQ( EHand::eThreeOfAKind, std::get< 0 >( hand ) );
                                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                                ASSERT_EQ( 2, std::get< 2 >( hand ).size() );
                                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                                if ( kickCard1 > kickCard2 )
                                {
                                    EXPECT_EQ( kickCard1, std::get< 2 >( hand ).front() );
                                    EXPECT_EQ( kickCard2, std::get< 2 >( hand ).back() );
                                }
                                else
                                {
                                    EXPECT_EQ( kickCard2, std::get< 2 >( hand ).front() );
                                    EXPECT_EQ( kickCard1, std::get< 2 >( hand ).back() );
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C5CardHandTester, TwoPair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for ( auto&& lowCard : ECard() )
                    {
                        if ( lowCard == highCard )
                            continue;

                        for ( auto&& lowSuit1 : ESuit() )
                        {
                            for ( auto&& lowSuit2 : ESuit() )
                            {
                                if ( lowSuit1 == lowSuit2 )
                                    continue;

                                for ( auto&& kicker : ECard() )
                                {
                                    if ( ( kicker == highCard ) || ( kicker == lowCard ) )
                                        continue;
                                    for ( auto&& kickSuit1 : ESuit() )
                                    {
                                        p1->clearCards();

                                        p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                                        p1->addCard( fGame->getCard( highCard, highSuit2 ) );
                                        p1->addCard( fGame->getCard( lowCard, lowSuit1 ) );
                                        p1->addCard( fGame->getCard( lowCard, lowSuit2 ) );
                                        p1->addCard( fGame->getCard( kicker, kickSuit1 ) );

                                        auto hand = p1->determineHand();
                                        EXPECT_EQ( EHand::eTwoPair, std::get< 0 >( hand ) );
                                        ASSERT_EQ( 2, std::get< 1 >( hand ).size() );
                                        ASSERT_EQ( 1, std::get< 2 >( hand ).size() );

                                        if ( highCard > lowCard )
                                        {
                                            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                                            EXPECT_EQ( lowCard, std::get< 1 >( hand ).back() );
                                        }
                                        else
                                        {
                                            EXPECT_EQ( lowCard, std::get< 1 >( hand ).front() );
                                            EXPECT_EQ( highCard, std::get< 1 >( hand ).back() );
                                        }
                                        EXPECT_EQ( kicker, std::get< 2 >( hand ).front() );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C5CardHandTester, DISABLED_Pair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        std::vector< THand > hands;
        for ( auto&& highCard : ECard() )
        {
            for ( auto&& highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for ( auto&& kicker1 : ECard() )
                    {
                        if ( kicker1 == highCard )
                            continue;
                        for ( auto&& kickerSuit1 : ESuit() )
                        {
                            for ( auto&& kicker2 : ECard() )
                            {
                                if ( ( kicker2 == highCard ) || ( kicker2 == kicker1 ) )
                                    continue;

                                for ( auto&& kickerSuit2 : ESuit() )
                                {
                                    for ( auto&& kicker3 : ECard() )
                                    {
                                        if ( ( kicker3 == highCard ) || ( kicker3 == kicker1 ) || ( kicker3 == kicker2 ) )
                                            continue;
                                        for ( auto&& kickerSuit3 : ESuit() )
                                        {
                                            hands.push_back( THand( { { highCard, highSuit1 }, { highCard, highSuit2 }, { kicker1, kickerSuit1 }, { kicker2, kickerSuit2 }, { kicker3, kickerSuit3 } } ) );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        for( auto && handData : hands )
        {
            p1->clearCards();

            p1->addCard( fGame->getCard( handData[ 0 ] ) );
            p1->addCard( fGame->getCard( handData[ 1 ] ) );
            p1->addCard( fGame->getCard( handData[ 2 ] ) );
            p1->addCard( fGame->getCard( handData[ 3 ] ) );
            p1->addCard( fGame->getCard( handData[ 4 ] ) );

            auto hand = p1->determineHand();
            EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

            EXPECT_EQ( handData[ 0 ].first, std::get< 1 >( hand ).front() );
            auto kickers = std::vector< ECard >( { handData[ 2 ].first, handData[ 3 ].first, handData[ 4 ].first } );
            std::sort( kickers.begin(), kickers.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
            EXPECT_EQ( kickers, std::get< 2 >( hand ) );
        }
    }

    TEST_F( C5CardHandTester, DISABLED_HighCard )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& card1 : ECard() )
        {
            for ( auto&& suit1 : ESuit() )
            {
                for ( auto&& card2 : ECard() )
                {
                    if ( card1 == card2 )
                        continue;
                    for ( auto&& suit2 : ESuit() )
                    {
                        for ( auto&& card3 : ECard() )
                        {
                            if (
                                ( card1 == card2 )
                                || ( card1 == card3 )
                                || ( card2 == card3 )
                                )
                                continue;

                            for ( auto&& suit3 : ESuit() )
                            {
                                for ( auto&& card4 : ECard() )
                                {
                                    if (
                                        ( card1 == card2 )
                                        || ( card1 == card3 )
                                        || ( card1 == card4 )
                                        || ( card2 == card3 )
                                        || ( card2 == card4 )
                                        || ( card3 == card4 )
                                        )
                                        continue;
                                    for ( auto&& suit4 : ESuit() )
                                    {
                                        for ( auto&& card5 : ECard() )
                                        {
                                            if (
                                                ( card1 == card2 )
                                                || ( card1 == card3 )
                                                || ( card1 == card4 )
                                                || ( card1 == card5 )
                                                || ( card2 == card3 )
                                                || ( card2 == card4 )
                                                || ( card2 == card5 )
                                                || ( card3 == card4 )
                                                || ( card3 == card5 )
                                                || ( card4 == card5 )
                                                )
                                                continue;

                                            if ( isStraight( std::vector< ECard >( { card1, card2, card3, card4, card5 } ) ) )
                                                continue;

                                            for ( auto&& suit5 : ESuit() )
                                            {
                                                if ( suit1 == suit5 )
                                                    continue; // prevent flush

                                                p1->clearCards();

                                                p1->addCard( fGame->getCard( card1, suit1 ) );
                                                p1->addCard( fGame->getCard( card2, suit2 ) );
                                                p1->addCard( fGame->getCard( card3, suit3 ) );
                                                p1->addCard( fGame->getCard( card4, suit4 ) );
                                                p1->addCard( fGame->getCard( card5, suit5 ) );

                                                auto hand = p1->determineHand();
                                                EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) )
                                                    << "Cards: "
                                                    << card1 << suit1 << " "
                                                    << card2 << suit2 << " "
                                                    << card3 << suit3 << " "
                                                    << card4 << suit4 << " "
                                                    << card5 << suit5 << " "
                                                    ;

                                                auto cards = std::vector< ECard >( { card1, card2, card3, card4, card5 } );
                                                std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
                                                auto kickers = cards;
                                                kickers.erase( kickers.begin(), ++kickers.begin() );
                                                cards.erase( ++cards.begin(), cards.end() );

                                                EXPECT_EQ( cards, std::get< 1 >( hand ) )
                                                    << "Cards: "
                                                    << card1 << suit1 << " "
                                                    << card2 << suit2 << " "
                                                    << card3 << suit3 << " "
                                                    << card4 << suit4 << " "
                                                    << card5 << suit5 << " "
                                                    ;

                                                EXPECT_EQ( kickers, std::get< 2 >( hand ) )
                                                    << "Cards: "
                                                    << card1 << suit1 << " "
                                                    << card2 << suit2 << " "
                                                    << card3 << suit3 << " "
                                                    << card4 << suit4 << " "
                                                    << card5 << suit5 << " "
                                                    ;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F( C5CardHandTester, Find5CardWinner )
    {
        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC" ) ); // ace high, QJ74
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // 9 high, 8632
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "9H 8S 6H 3C 2S" ) ); // 9 high, 8632
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 7C 6S 5C" ) ); // K high, Q765

        fGame->setLowHandWins( true );
        fGame->setStraightsAndFlushesCount( false );
        auto winners = fGame->findWinners();
        EXPECT_EQ( 1, winners.size() );
        EXPECT_EQ( "Scott", winners.front()->name() );
    }

    TEST_F( C5CardHandTester, Find5CardWinnerLowBall1 )
    {
        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "AD KD QS 7C 6S" ) ); // KQ76A 4919
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // 98632 6135
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "9H 8S 6H 3C 2S" ) ); //  98632 6135
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 7C 6S 5C" ) ); // KQ765 5484

        fGame->setLowHandWins( true );
        fGame->setStraightsAndFlushesCount( false );

        auto winners = fGame->findWinners();
        EXPECT_EQ( 2, winners.size() );
        EXPECT_EQ( "Craig", winners.front()->name() );
        EXPECT_EQ( "Eric", winners.back()->name() );
    }

    TEST_F( C5CardHandTester, Find5CardWinnerLowBall2 )
    {
        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "2D AS 3D 4H 5C" ) ); // Wheel.. best 5 card low
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // 9 high, 8632
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "9H 8S 6H 3C 2S" ) ); // 9 high, 8632
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS jC TS 9C" ) ); // K high striaght.. worst 5 card low

        fGame->setLowHandWins( true );
        fGame->setStraightsAndFlushesCount( false );

        auto winners = fGame->findWinners();
        EXPECT_EQ( 1, winners.size() );
        EXPECT_EQ( "Scott", winners.front()->name() );
    }

    TEST_F( C5CardHandTester, Find5CardWinnerWild )
    {
        for ( auto&& suit : ESuit() )
        {
            fGame->addWildCard( fGame->getCard( ECard::eDeuce, suit ) ); // all twos wild
        }

        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC" ) ); // ace high, QJ74
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // Pair of 9s (9C 2H), 863
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "9H 8S 6H 3C 2S" ) ); // Pair of 9s (9C 2S), 863
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 7C 6S 5C" ) ); // K high, Q765

        auto winners = fGame->findWinners();
        EXPECT_EQ( 2, winners.size() );
        EXPECT_EQ( "Craig", winners.back()->name() );
        auto winningHandName = winners.back()->getHand()->determineHandName( true );
        EXPECT_EQ( "Pair of 'Nine' - 'Eight, Six, Trey' kicker", winners.back()->getHand()->determineHandName( true ) );

        winningHandName = winners.front()->getHand()->determineHandName( true );
        EXPECT_EQ( "Eric", winners.front()->name() );
        EXPECT_EQ( "Pair of 'Nine' - 'Eight, Six, Trey' kicker", winners.front()->getHand()->determineHandName( true ) );
    }

    TEST_F( C5CardHandTester, DetermineHandWild )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2C 2H" ), nullptr ); // Ace H flush
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFlush, handValue );
        EXPECT_EQ( ECard::eAce, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2H 2H" ), nullptr ); // Invalid hand
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eNoCards, handValue );

        hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2C 2D" ), nullptr ); // Pair of kings
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) ); // 2 clubs and hearts wild
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::ePair, handValue );
        EXPECT_EQ( ECard::eKing, *card.begin() );
        EXPECT_EQ( ECard::eSeven, *kickers.begin() );
        EXPECT_EQ( ECard::eDeuce, *kickers.rbegin() );
    }

    TEST_F( C5CardHandTester, DetermineHand5OfAKind )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "2S 2D 4H 2C 2H" ), nullptr ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( "Five of a Kind 'Four'", hand->bestHand().value().second->determineHandName( true ) );

        hand = std::make_shared< CHand >( fGame->getCards( "2S 4C 4H 2C 2H" ), nullptr ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "2S 4C 4H 4D 2H" ), nullptr ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "4S 4C 4H 4D 2H" ), nullptr ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
    }

    TEST_F( C5CardHandTester, AllCardHands )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( true );

        auto allHands = getAllCHandHands( 5 );

        EXPECT_EQ( 2598960, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 7462, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 40, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 624, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 5108, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10200, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 54912, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 123552, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 1098240, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1302540, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 2860, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    TEST_F( C5CardHandTester, AllCardHands_NoStraightsFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsAndFlushesCount( false );

        auto allHands = getAllCHandHands( 5 );

        EXPECT_EQ( 2598960, allHands.size() );

        auto analyzedHands = getUniqueHands( allHands );

        EXPECT_EQ( 7462, std::get< 0 >( analyzedHands ).size() );

        EXPECT_EQ( 40, std::get< 1 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 624, std::get< 1 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 3744, std::get< 1 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 5108, std::get< 1 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10200, std::get< 1 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 54912, std::get< 1 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 123552, std::get< 1 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 1098240, std::get< 1 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1302540, std::get< 1 >( analyzedHands )[ EHand::eHighCard ] );

        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraightFlush ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 156, std::get< 2 >( analyzedHands )[ EHand::eFullHouse ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eFlush ] );
        EXPECT_EQ( 10, std::get< 2 >( analyzedHands )[ EHand::eStraight ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 858, std::get< 2 >( analyzedHands )[ EHand::eTwoPair ] );
        EXPECT_EQ( 2860, std::get< 2 >( analyzedHands )[ EHand::ePair ] );
        EXPECT_EQ( 1277, std::get< 2 >( analyzedHands )[ EHand::eHighCard ] );
    }

    class C7CardHandTester : public CHandTester
    {
    protected:
        C7CardHandTester() {}
        virtual ~C7CardHandTester() {}
    };
    TEST_F( C7CardHandTester, Find7CardWinner )
    {
        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC 3C 2C" ) ); // ace high, QJ74
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "JC TC 8D 5D 4H 3H 2H" ) ); // J high, T854
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "JH TS 8H 5C 4S 3D 2D" ) ); // J high, T854
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 8C 7S 5C 4C 3S 2S" ) ); // K high, Q875

        auto winners = fGame->findWinners();
        EXPECT_EQ( 1, winners.size() );
        EXPECT_EQ( "Scott", winners.front()->name() );
        ASSERT_TRUE( winners.front()->getHand()->bestHand().has_value() );
        EXPECT_EQ( "Cards: 7D AS 4D QH JC", winners.front()->getHand()->bestHand().value().second->toString() );
        EXPECT_EQ( "High Card 'Ace' : Queen, Jack, Seven, Four kickers", winners.front()->getHand()->bestHand().value().second->determineHandName( true ) );
    }

    TEST_F( C7CardHandTester, Find7CardHandWild )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "3C 4D 7H KH 4H 2C 2H" ), nullptr ); // Ace H flush
        //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eTwoPair, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( ECard::eDeuce, *card.rbegin() );
        EXPECT_EQ( ECard::eKing, *kickers.begin() );

        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFourOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( ECard::eKing, *kickers.begin() );
    }
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}
