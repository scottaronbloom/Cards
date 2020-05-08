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
#include "Cards/Player.h"
#include "Cards/Hand.h"
#include "Cards/Card.h"
#include "Cards/Evaluate2CardHand.h"
#include <string>
#include "gmock/gmock.h"

namespace NHandTester
{
    TEST_F( CHandTester, TestCompare )
    {
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades );
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts);
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts  );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eQueen, ESuit::eSpades);
            NHandUtils::S2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }

        {
            NHandUtils::S2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eDeuce, ESuit::eSpades );
            NHandUtils::S2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }

        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eAce, ESuit::eSpades );
            NHandUtils::S2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eSpades );
            EXPECT_TRUE( h1.lessThan( true, h2 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );   // straight flush
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts ); // straight
            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h2.lessThan( true, h1 ) );

            auto gFlushStraightsCount = []( const NHandUtils::S2CardInfo & lhs, const NHandUtils::S2CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            using TFlushesCountMap = std::map< NHandUtils::S2CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;
            static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            flushesAndStraightsCount.insert( std::make_pair( h1, -1 ) );
            auto pos = flushesAndStraightsCount.find( h2 );
            EXPECT_TRUE( pos == flushesAndStraightsCount.end() );
        }
    }

    TEST_F(CHandTester, StraightFlushes) 
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

        for( auto suit : ESuit() )
        {
            for( auto && highCard : ECard() )
            {
                p1->clearCards();

                if ( highCard == ECard::eDeuce )
                {
                    p1->addCard( fGame->getCard( highCard, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for( auto currCard = ( highCard - 1 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_TRUE( p1->isStraight() );
                auto hand = p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
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

    TEST_F( CHandTester, StraightFlushesDisabled )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

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

    TEST_F( CHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

        for ( auto suit : ESuit() )
        {
            for ( auto && highCard : ECard() )
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

    TEST_F( CHandTester, FlushDisabled )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

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

    TEST_F( CHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

        for ( auto&& highCard : ECard() )
        {
            for( auto && suit : ESuit() )
            {
                p1->clearCards();
                if ( suit == ESuit::eClubs )
                    continue;

                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
                if ( highCard == ECard::eDeuce )
                {
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                    highCard = ECard::eAce;
                }
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

    TEST_F( CHandTester, StraightDisabled )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

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

    TEST_F( CHandTester, Pair )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

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
                        << highCard<< highSuit1 << " "
                        << highCard << highSuit2 << " "
                        ;

                    ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                    EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                    EXPECT_EQ( 0, std::get< 2 >( hand ).size() );
                }
            }
        }
    }

    TEST_F( CHandTester, PairDisabled )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

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

    TEST_F( CHandTester, HighCard )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

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

                        if( isStraight( std::vector< ECard >( { card1, card2 } ) ) )
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

    TEST_F( CHandTester, AllCardHands )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

        std::vector< std::shared_ptr< CHand > > allHands;
        size_t numHands = 0;
        auto allCards = getAllCards();
        for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
        {
            for ( size_t c2 = c1 + 1; c2 < allCards.size(); ++c2 )
            {
                auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ] } ), fGame->playInfo() );
                allHands.push_back( hand );
                numHands++;
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
                   { return lhs->operator>( *rhs ); }
        );

        EXPECT_EQ( 1326, numHands );
        EXPECT_EQ( 1326, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 169, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 52, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 260, freq[ EHand::eFlush ] );
        EXPECT_EQ( 156, freq[ EHand::eStraight ] );
        EXPECT_EQ( 78, freq[ EHand::ePair ] );
        EXPECT_EQ( 780, freq[ EHand::eHighCard ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 13, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 65, subFreq[ EHand::eFlush ] );
        EXPECT_EQ( 13, subFreq[ EHand::eStraight ] );
        EXPECT_EQ( 13, subFreq[ EHand::ePair ] );
        EXPECT_EQ( 65, subFreq[ EHand::eHighCard ] );
    }

    TEST_F( CHandTester, AllCardHandsDisabled )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( false );

        std::vector< std::shared_ptr< CHand > > allHands;
        size_t numHands = 0;
        auto allCards = getAllCards();
        for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
        {
            for ( size_t c2 = c1 + 1; c2 < allCards.size(); ++c2 )
            {
                auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ] } ), nullptr );
                allHands.push_back( hand );
                numHands++;
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
                   { return lhs->operator>( *rhs ); }
        );

        EXPECT_EQ( 1326, numHands );
        EXPECT_EQ( 1326, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 91, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 0, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, freq[ EHand::eFlush ] );
        EXPECT_EQ( 0, freq[ EHand::eStraight ] );
        EXPECT_EQ( 78, freq[ EHand::ePair ] );
        EXPECT_EQ( 1248, freq[ EHand::eHighCard ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 0, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFlush ] );
        EXPECT_EQ( 0, subFreq[ EHand::eStraight ] );
        EXPECT_EQ( 13, subFreq[ EHand::ePair ] );
        EXPECT_EQ( 78, subFreq[ EHand::eHighCard ] );
    }
}  // namespace


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}
