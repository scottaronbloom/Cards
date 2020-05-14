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
#include "Cards/Game.h"
#include "Cards/Player.h"
#include "Cards/Hand.h"
#include "Cards/Card.h"

#include <string>
#include "gmock/gmock.h"

namespace NHandTester
{
    class C2CardHandTester : public CHandTester
    {
    protected:
        C2CardHandTester() {}
        virtual ~C2CardHandTester() {}
    };
    TEST_F( C2CardHandTester, Basic )
    {
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eClubs, ECard::eDeuce, ESuit::eDiamonds );
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
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades );
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eQueen, ESuit::eSpades );
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
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair
            NHandUtils::S2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eHearts ); // straight
            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );

            EXPECT_TRUE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair
            NHandUtils::S2CardInfo h2( ECard::eQueen, ESuit::eSpades, ECard::eKing, ESuit::eSpades ); // straight flush
            EXPECT_TRUE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_FALSE( h1.equalTo( true, h2 ) );

            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades );   // straight flush
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts ); // straight
            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_TRUE( h2.lessThan( true, h1 ) );

            auto gFlushStraightsCount = []( const NHandUtils::S2CardInfo& lhs, const NHandUtils::S2CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            using TFlushesCountMap = std::map< NHandUtils::S2CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;
            static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            flushesAndStraightsCount.insert( std::make_pair( h1, -1 ) );
            auto pos = flushesAndStraightsCount.find( h2 );
            EXPECT_TRUE( pos == flushesAndStraightsCount.end() );
        }
    }

    TEST_F( C2CardHandTester, StraightFlushes )
    {
        //NHandUtils::gComputeAllHands = true;

        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

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

    TEST_F( C2CardHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

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

    TEST_F( C2CardHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

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

    TEST_F( C2CardHandTester, Pair )
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

    TEST_F( C2CardHandTester, HighCard )
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

    TEST_F( C2CardHandTester, AllCardHands_NoStraightsFlushes )
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

    class C3CardHandTester : public CHandTester
    {
    protected:
        C3CardHandTester() {}
        virtual ~C3CardHandTester() {}
    };

    TEST_F( C3CardHandTester, Basic )
    {
        {
            using TCardToInfoMap = std::map< NHandUtils::S3CardInfo::THand, NHandUtils::S3CardInfo >;
            auto gFlushStraightsCount = []( const NHandUtils::S3CardInfo& lhs, const NHandUtils::S3CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            auto gJustCardsCount = []( const NHandUtils::S3CardInfo& lhs, const NHandUtils::S3CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

            using TCardsCountMap = std::map< NHandUtils::S3CardInfo, uint32_t, decltype( gJustCardsCount ) >;
            using TFlushesCountMap = std::map< NHandUtils::S3CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

            TCardsCountMap justCardsCount( gJustCardsCount );
            TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            NHandUtils::S3CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eKing, ESuit::eHearts, ECard::eJack, ESuit::eSpades ); // pair of 2s 3 kicker
            NHandUtils::S3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eAce, ESuit::eSpades ); // pair or 3s 2 kicker
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
            NHandUtils::S3CardInfo h3( ECard::eTrey, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eSpades ); 
            NHandUtils::S3CardInfo h4( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eFive, ESuit::eSpades ); 
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
            NHandUtils::S3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } } );  /// ace high 3 2
            NHandUtils::S3CardInfo h2( { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } } ); // K high
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
            EXPECT_FALSE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }

        {
            NHandUtils::S3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } } ); // a23 straight
            NHandUtils::S3CardInfo h2( { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } } ); // kqj straight
            EXPECT_TRUE( h2.greaterThan( true, h1 ) );
            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.equalTo( true, h1 ) );
        }

        {
            NHandUtils::S3CardInfo h1( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } } ); 
            NHandUtils::S3CardInfo h2( { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eDiamonds } } ); 
            EXPECT_FALSE( h2.lessThan( true, h1 ) );
            EXPECT_FALSE( h2.greaterThan( true, h1 ) );
            EXPECT_TRUE( h2.equalTo( true, h1 ) );

            EXPECT_FALSE( h1.lessThan( true, h2 ) );
            EXPECT_FALSE( h1.greaterThan( true, h2 ) );
            EXPECT_TRUE( h1.equalTo( true, h2 ) );
        }

        {
            //NHandUtils::gComputeAllHands = true;
            auto p1 = fGame->addPlayer( "Scott" );
            fGame->setStraightsFlushesCountForSmallHands( true );

            p1->addCard( fGame->getCard( ECard::eTrey, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eAce, ESuit::eSpades ) );

            EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
            EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
            auto hand = p1->determineHand();
            EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
        }

        {
            NHandUtils::S3CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair of 2s 3 kicker
            NHandUtils::S3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eTrey, ESuit::eHearts ); // pair or 3s 2 kicker
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_TRUE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }
        {
            NHandUtils::S3CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts ); // pair of 2s 3 kicker
            NHandUtils::S3CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eFour, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );  // pair of 2s 4 kicker
            EXPECT_FALSE( h2.lessThan( false, h1 ) );
            EXPECT_TRUE( h2.greaterThan( false, h1 ) );
            EXPECT_FALSE( h2.equalTo( false, h1 ) );
        }
        {
            NHandUtils::S3CardInfo h1( ECard::eTrey, ESuit::eSpades, ECard::eDeuce, ESuit::eSpades, ECard::eAce, ESuit::eSpades ); // pair of 2s 3 kicker
            EXPECT_TRUE( h1.fIsFlush );
            EXPECT_TRUE( h1.fStraightType.has_value() );
            EXPECT_EQ( NHandUtils::EStraightType::eWheel, h1.fStraightType.value() );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eDeuce, ESuit::eSpades, ECard::eDeuce, ESuit::eHearts );
            NHandUtils::S2CardInfo h2( ECard::eDeuce, ESuit::eSpades, ECard::eTrey, ESuit::eHearts );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
        {
            NHandUtils::S2CardInfo h1( ECard::eAce, ESuit::eSpades, ECard::eQueen, ESuit::eSpades );
            NHandUtils::S2CardInfo h2( ECard::eTrey, ESuit::eHearts, ECard::eDeuce, ESuit::eSpades );
            EXPECT_TRUE( h2.lessThan( false, h1 ) );
        }
    }
    TEST_F( C3CardHandTester, StraightFlushes )
    {
        auto p1 = fGame->addPlayer( "Scott" );
        fGame->setStraightsFlushesCountForSmallHands( true );

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
        fGame->setStraightsFlushesCountForSmallHands( true );
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
        fGame->setStraightsFlushesCountForSmallHands( true );

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
        fGame->setStraightsFlushesCountForSmallHands( true );

        std::vector< std::shared_ptr< CHand > > allHands;
        size_t numHands = 0;
        auto allCards = getAllCards();
        for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
        {
            for ( size_t c2 = c1 + 1; c2 < allCards.size(); ++c2 )
            {
                for ( size_t c3 = c2 + 1; c3 < allCards.size(); ++c3 )
                {
                    auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ], allCards[ c3 ] } ), fGame->playInfo() );
                    allHands.push_back( hand );
                    numHands++;
                }
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
                   { return lhs->operator>( *rhs ); }
        );

        EXPECT_EQ( 22100, numHands );
        EXPECT_EQ( 22100, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 741, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 48, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, freq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, freq[ EHand::eFullHouse ] );
        EXPECT_EQ( 1096, freq[ EHand::eFlush ] );
        EXPECT_EQ( 720, freq[ EHand::eStraight ] );
        EXPECT_EQ( 52, freq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, freq[ EHand::eTwoPair ] );
        EXPECT_EQ( 3744, freq[ EHand::ePair ] );
        EXPECT_EQ( 16440, freq[ EHand::eHighCard ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 12, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFullHouse ] );
        EXPECT_EQ( 274, subFreq[ EHand::eFlush ] );
        EXPECT_EQ( 12, subFreq[ EHand::eStraight ] );
        EXPECT_EQ( 13, subFreq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, subFreq[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, subFreq[ EHand::ePair ] );
        EXPECT_EQ( 274, subFreq[ EHand::eHighCard ] );
    }

    TEST_F( C3CardHandTester, AllCardHands_NoStraightsFlushes )
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
                for ( size_t c3 = c2 + 1; c3 < allCards.size(); ++c3 )
                {
                    auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ], allCards[ c3 ] } ), fGame->playInfo() );
                    allHands.push_back( hand );
                    numHands++;
                }
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
                   { return lhs->operator>( *rhs ); }
        );

        EXPECT_EQ( 22100, numHands );
        EXPECT_EQ( 22100, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 455, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 0, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, freq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, freq[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, freq[ EHand::eFlush ] );
        EXPECT_EQ( 0, freq[ EHand::eStraight ] );
        EXPECT_EQ( 52, freq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, freq[ EHand::eTwoPair ] );
        EXPECT_EQ( 3744, freq[ EHand::ePair ] );
        EXPECT_EQ( 18304, freq[ EHand::eHighCard ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 0, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFullHouse ] );
        EXPECT_EQ( 0, subFreq[ EHand::eFlush ] );
        EXPECT_EQ( 0, subFreq[ EHand::eStraight ] );
        EXPECT_EQ( 13, subFreq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 0, subFreq[ EHand::eTwoPair ] );
        EXPECT_EQ( 156, subFreq[ EHand::ePair ] );
        EXPECT_EQ( 286, subFreq[ EHand::eHighCard ] );
    }

    class C4CardHandTester : public CHandTester
    {
    protected:
        C4CardHandTester() {}
        virtual ~C4CardHandTester() {}
    };

    TEST_F( C4CardHandTester, DISABLED_Basic )
    {
        NHandUtils::gComputeAllHands = true;
        {
            //NHandUtils::gComputeAllHands = true;
            auto p1 = fGame->addPlayer( "Scott" );
            fGame->setStraightsFlushesCountForSmallHands( true );

            p1->addCard( fGame->getCard( ECard::eTrey, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eDeuce, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eAce, ESuit::eSpades ) );
            p1->addCard( fGame->getCard( ECard::eFour, ESuit::eSpades ) );

            EXPECT_TRUE( p1->isFlush() ) << "Cards: " << p1->getHand()->getCards();
            EXPECT_TRUE( p1->isStraight() ) << "Cards: " << p1->getHand()->getCards();
            auto hand = p1->determineHand();
            EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
        }
    }

    //TEST_F( C4CardHandTester, StraightFlushes )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto suit : ESuit() )
    //    {
    //        for ( auto&& highCard : ECard() )
    //        {
    //            if ( highCard <= ECard::eFour )
    //                continue;

    //            p1->clearCards();

    //            if ( highCard == ECard::eFive )
    //            {
    //                p1->addCard( fGame->getCard( highCard, suit ) );
    //                p1->addCard( fGame->getCard( ECard::eFour, suit ) );
    //                p1->addCard( fGame->getCard( ECard::eTrey, suit ) );
    //                p1->addCard( fGame->getCard( ECard::eDeuce, suit ) );
    //                p1->addCard( fGame->getCard( ECard::eAce, suit ) );
    //            }
    //            else
    //            {
    //                for ( auto currCard = ( highCard - 4 ); currCard <= highCard; ++currCard )
    //                {
    //                    p1->addCard( fGame->getCard( currCard, suit ) );
    //                }
    //            }
    //            EXPECT_TRUE( p1->isFlush() );
    //            EXPECT_TRUE( p1->isStraight() );
    //            auto hand = p1->determineHand();
    //            EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
    //            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
    //            ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
    //            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, FourOfAKind )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& highCard : ECard() )
    //    {
    //        for ( auto&& kicker : ECard() )
    //        {
    //            if ( kicker == highCard )
    //                continue;

    //            for ( auto suit : ESuit() )
    //            {
    //                p1->clearCards();

    //                p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
    //                p1->addCard( fGame->getCard( highCard, ESuit::eSpades ) );
    //                p1->addCard( fGame->getCard( highCard, ESuit::eHearts ) );
    //                p1->addCard( fGame->getCard( highCard, ESuit::eDiamonds ) );

    //                p1->addCard( fGame->getCard( kicker, suit ) );

    //                auto hand = p1->determineHand();
    //                EXPECT_EQ( EHand::eFourOfAKind, std::get< 0 >( hand ) ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
    //                ASSERT_EQ( 1, std::get< 1 >( hand ).size() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
    //                ASSERT_EQ( 1, std::get< 2 >( hand ).size() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
    //                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
    //                EXPECT_EQ( kicker, std::get< 2 >( hand ).front() ) << "4 of Kind: " << highCard << " Kicker: " << kicker << " Suit: " << suit;
    //            }
    //        }
    //    }
    //}
    //TEST_F( C4CardHandTester, Flush )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto suit : ESuit() )
    //    {
    //        for ( auto&& highCard : ECard() )
    //        {
    //            if ( highCard <= ECard::eFive ) // for a flush, the high card can only be a 6 or above, otherwise its a striaght flush
    //                continue;

    //            p1->clearCards();

    //            p1->addCard( fGame->getCard( highCard, suit ) );
    //            p1->addCard( fGame->getCard( highCard - 2, suit ) );
    //            p1->addCard( fGame->getCard( highCard - 3, suit ) );
    //            p1->addCard( fGame->getCard( highCard - 4, suit ) );
    //            if ( highCard == ECard::eSix )
    //            {
    //                p1->addCard( fGame->getCard( ECard::eAce, suit ) );
    //                highCard = ECard::eAce;
    //            }
    //            else
    //                p1->addCard( fGame->getCard( highCard - 5, suit ) );

    //            EXPECT_TRUE( p1->isFlush() );
    //            auto hand = p1->determineHand();
    //            EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
    //            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
    //            ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
    //            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, Straight )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& highCard : ECard() )
    //    {
    //        if ( highCard <= ECard::eFour )
    //            continue;

    //        for ( auto&& suit : ESuit() )
    //        {
    //            p1->clearCards();

    //            p1->addCard( fGame->getCard( highCard, ESuit::eClubs ) );
    //            p1->addCard( fGame->getCard( highCard - 1, ESuit::eDiamonds ) );
    //            p1->addCard( fGame->getCard( highCard - 2, ESuit::eHearts ) );
    //            p1->addCard( fGame->getCard( highCard - 3, ESuit::eSpades ) );
    //            if ( highCard == ECard::eFive )
    //                p1->addCard( fGame->getCard( ECard::eAce, suit ) );
    //            else
    //                p1->addCard( fGame->getCard( highCard - 4, suit ) );

    //            EXPECT_TRUE( p1->isStraight() );
    //            auto hand = p1->determineHand();
    //            EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
    //            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
    //            ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
    //            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, ThreeOfAKind )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& highCard : ECard() )
    //    {
    //        for ( auto&& suit : ESuit() )
    //        {
    //            for ( auto&& kickCard1 : ECard() )
    //            {
    //                if ( kickCard1 == highCard )
    //                    continue;
    //                for ( auto&& kickCard2 : ECard() )
    //                {
    //                    if ( kickCard1 == kickCard2 )
    //                        continue;

    //                    if ( kickCard2 == highCard )
    //                        continue;

    //                    for ( auto&& kickSuit1 : ESuit() )
    //                    {
    //                        for ( auto&& kickSuit2 : ESuit() )
    //                        {
    //                            p1->clearCards();
    //                            for ( auto&& skipSuit : ESuit() )
    //                            {
    //                                if ( suit == skipSuit )
    //                                    continue;

    //                                p1->addCard( fGame->getCard( highCard, skipSuit ) );
    //                            }
    //                            // now have 3 of a kind

    //                            p1->addCard( fGame->getCard( kickCard1, kickSuit1 ) );
    //                            p1->addCard( fGame->getCard( kickCard2, kickSuit2 ) );

    //                            auto hand = p1->determineHand();
    //                            EXPECT_EQ( EHand::eThreeOfAKind, std::get< 0 >( hand ) );
    //                            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
    //                            ASSERT_EQ( 2, std::get< 2 >( hand ).size() );
    //                            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //                            if ( kickCard1 > kickCard2 )
    //                            {
    //                                EXPECT_EQ( kickCard1, std::get< 2 >( hand ).front() );
    //                                EXPECT_EQ( kickCard2, std::get< 2 >( hand ).back() );
    //                            }
    //                            else
    //                            {
    //                                EXPECT_EQ( kickCard2, std::get< 2 >( hand ).front() );
    //                                EXPECT_EQ( kickCard1, std::get< 2 >( hand ).back() );
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, TwoPair )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& highCard : ECard() )
    //    {
    //        for ( auto&& highSuit1 : ESuit() )
    //        {
    //            for ( auto&& highSuit2 : ESuit() )
    //            {
    //                if ( highSuit1 == highSuit2 )
    //                    continue;

    //                for ( auto&& lowCard : ECard() )
    //                {
    //                    if ( lowCard == highCard )
    //                        continue;

    //                    for ( auto&& lowSuit1 : ESuit() )
    //                    {
    //                        for ( auto&& lowSuit2 : ESuit() )
    //                        {
    //                            if ( lowSuit1 == lowSuit2 )
    //                                continue;

    //                            for ( auto&& kicker : ECard() )
    //                            {
    //                                if ( ( kicker == highCard ) || ( kicker == lowCard ) )
    //                                    continue;
    //                                for ( auto&& kickSuit1 : ESuit() )
    //                                {
    //                                    p1->clearCards();

    //                                    p1->addCard( fGame->getCard( highCard, highSuit1 ) );
    //                                    p1->addCard( fGame->getCard( highCard, highSuit2 ) );
    //                                    p1->addCard( fGame->getCard( lowCard, lowSuit1 ) );
    //                                    p1->addCard( fGame->getCard( lowCard, lowSuit2 ) );
    //                                    p1->addCard( fGame->getCard( kicker, kickSuit1 ) );

    //                                    auto hand = p1->determineHand();
    //                                    EXPECT_EQ( EHand::eTwoPair, std::get< 0 >( hand ) );
    //                                    ASSERT_EQ( 2, std::get< 1 >( hand ).size() );
    //                                    ASSERT_EQ( 1, std::get< 2 >( hand ).size() );

    //                                    if ( highCard > lowCard )
    //                                    {
    //                                        EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //                                        EXPECT_EQ( lowCard, std::get< 1 >( hand ).back() );
    //                                    }
    //                                    else
    //                                    {
    //                                        EXPECT_EQ( lowCard, std::get< 1 >( hand ).front() );
    //                                        EXPECT_EQ( highCard, std::get< 1 >( hand ).back() );
    //                                    }
    //                                    EXPECT_EQ( kicker, std::get< 2 >( hand ).front() );
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, Pair )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& highCard : ECard() )
    //    {
    //        for ( auto&& highSuit1 : ESuit() )
    //        {
    //            for ( auto&& highSuit2 : ESuit() )
    //            {
    //                if ( highSuit1 == highSuit2 )
    //                    continue;

    //                for ( auto&& kicker1 : ECard() )
    //                {
    //                    if ( kicker1 == highCard )
    //                        continue;
    //                    for ( auto&& kickerSuit1 : ESuit() )
    //                    {
    //                        for ( auto&& kicker2 : ECard() )
    //                        {
    //                            if ( ( kicker2 == highCard ) || ( kicker2 == kicker1 ) )
    //                                continue;

    //                            for ( auto&& kickerSuit2 : ESuit() )
    //                            {
    //                                for ( auto&& kicker3 : ECard() )
    //                                {
    //                                    if ( ( kicker3 == highCard ) || ( kicker3 == kicker1 ) || ( kicker3 == kicker2 ) )
    //                                        continue;
    //                                    for ( auto&& kickerSuit3 : ESuit() )
    //                                    {
    //                                        p1->clearCards();

    //                                        p1->addCard( fGame->getCard( highCard, highSuit1 ) );
    //                                        p1->addCard( fGame->getCard( highCard, highSuit2 ) );
    //                                        p1->addCard( fGame->getCard( kicker1, kickerSuit1 ) );
    //                                        p1->addCard( fGame->getCard( kicker2, kickerSuit2 ) );
    //                                        p1->addCard( fGame->getCard( kicker3, kickerSuit3 ) );

    //                                        auto hand = p1->determineHand();
    //                                        EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
    //                                        ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

    //                                        EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
    //                                        auto kickers = std::vector< ECard >( { kicker1, kicker2, kicker3 } );
    //                                        std::sort( kickers.begin(), kickers.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
    //                                        EXPECT_EQ( kickers, std::get< 2 >( hand ) );
    //                                    }
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, DISABLED_HighCard )
    //{
    //    auto p1 = fGame->addPlayer( "Scott" );

    //    for ( auto&& card1 : ECard() )
    //    {
    //        for ( auto&& suit1 : ESuit() )
    //        {
    //            for ( auto&& card2 : ECard() )
    //            {
    //                if ( card1 == card2 )
    //                    continue;
    //                for ( auto&& suit2 : ESuit() )
    //                {
    //                    for ( auto&& card3 : ECard() )
    //                    {
    //                        if (
    //                            ( card1 == card2 )
    //                            || ( card1 == card3 )
    //                            || ( card2 == card3 )
    //                            )
    //                            continue;

    //                        for ( auto&& suit3 : ESuit() )
    //                        {
    //                            for ( auto&& card4 : ECard() )
    //                            {
    //                                if (
    //                                    ( card1 == card2 )
    //                                    || ( card1 == card3 )
    //                                    || ( card1 == card4 )
    //                                    || ( card2 == card3 )
    //                                    || ( card2 == card4 )
    //                                    || ( card3 == card4 )
    //                                    )
    //                                    continue;
    //                                for ( auto&& suit4 : ESuit() )
    //                                {
    //                                    for ( auto&& card5 : ECard() )
    //                                    {
    //                                        if (
    //                                            ( card1 == card2 )
    //                                            || ( card1 == card3 )
    //                                            || ( card1 == card4 )
    //                                            || ( card1 == card5 )
    //                                            || ( card2 == card3 )
    //                                            || ( card2 == card4 )
    //                                            || ( card2 == card5 )
    //                                            || ( card3 == card4 )
    //                                            || ( card3 == card5 )
    //                                            || ( card4 == card5 )
    //                                            )
    //                                            continue;

    //                                        if ( isStraight( std::vector< ECard >( { card1, card2, card3, card4, card5 } ) ) )
    //                                            continue;

    //                                        for ( auto&& suit5 : ESuit() )
    //                                        {
    //                                            if ( suit1 == suit5 )
    //                                                continue; // prevent flush

    //                                            p1->clearCards();

    //                                            p1->addCard( fGame->getCard( card1, suit1 ) );
    //                                            p1->addCard( fGame->getCard( card2, suit2 ) );
    //                                            p1->addCard( fGame->getCard( card3, suit3 ) );
    //                                            p1->addCard( fGame->getCard( card4, suit4 ) );
    //                                            p1->addCard( fGame->getCard( card5, suit5 ) );

    //                                            auto hand = p1->determineHand();
    //                                            EXPECT_EQ( EHand::eHighCard, std::get< 0 >( hand ) )
    //                                                << "Cards: "
    //                                                << card1 << suit1 << " "
    //                                                << card2 << suit2 << " "
    //                                                << card3 << suit3 << " "
    //                                                << card4 << suit4 << " "
    //                                                << card5 << suit5 << " "
    //                                                ;

    //                                            auto cards = std::vector< ECard >( { card1, card2, card3, card4, card5 } );
    //                                            std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
    //                                            auto kickers = cards;
    //                                            kickers.erase( kickers.begin(), ++kickers.begin() );
    //                                            cards.erase( ++cards.begin(), cards.end() );

    //                                            EXPECT_EQ( cards, std::get< 1 >( hand ) )
    //                                                << "Cards: "
    //                                                << card1 << suit1 << " "
    //                                                << card2 << suit2 << " "
    //                                                << card3 << suit3 << " "
    //                                                << card4 << suit4 << " "
    //                                                << card5 << suit5 << " "
    //                                                ;

    //                                            EXPECT_EQ( kickers, std::get< 2 >( hand ) )
    //                                                << "Cards: "
    //                                                << card1 << suit1 << " "
    //                                                << card2 << suit2 << " "
    //                                                << card3 << suit3 << " "
    //                                                << card4 << suit4 << " "
    //                                                << card5 << suit5 << " "
    //                                                ;
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    //TEST_F( C4CardHandTester, All3CardHands )
    //{
    //    std::vector< std::shared_ptr< CHand > > allHands;
    //    size_t numHands = 0;
    //    auto allCards = getAllCards();
    //    for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
    //    {
    //        for ( size_t c2 = c1 + 1; c2 < allCards.size(); ++c2 )
    //        {
    //            for ( size_t c3 = c2 + 1; c3 < allCards.size(); ++c3 )
    //            {
    //                for ( size_t c4 = c3 + 1; c4 < allCards.size(); ++c4 )
    //                {
    //                    for ( size_t c5 = c4 + 1; c5 < allCards.size(); ++c5 )
    //                    {
    //                        auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ], allCards[ c3 ], allCards[ c4 ], allCards[ c5 ] } ), nullptr );
    //                        allHands.push_back( hand );
    //                        numHands++;
    //                    }
    //                }
    //            }
    //        }
    //    }

    //    std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
    //               { return lhs->operator>( *rhs ); }
    //    );

    //    EXPECT_EQ( 2598960, numHands );
    //    EXPECT_EQ( 2598960, allHands.size() );

    //    std::vector< std::shared_ptr< CHand > > uniqueHands;
    //    std::shared_ptr< CHand > prevHand;
    //    for ( size_t ii = 0; ii < allHands.size(); ++ii )
    //    {
    //        if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
    //            continue;
    //        prevHand = allHands[ ii ];
    //        uniqueHands.push_back( prevHand );
    //    }
    //    EXPECT_EQ( 7462, uniqueHands.size() );

    //    std::map< EHand, size_t > freq;
    //    for ( auto&& ii : allHands )
    //    {
    //        auto hand = ii->determineHand();
    //        freq[ std::get< 0 >( hand ) ]++;
    //    }
    //    EXPECT_EQ( 40, freq[ EHand::eStraightFlush ] );
    //    EXPECT_EQ( 624, freq[ EHand::eFourOfAKind ] );
    //    EXPECT_EQ( 3744, freq[ EHand::eFullHouse ] );
    //    EXPECT_EQ( 5108, freq[ EHand::eFlush ] );
    //    EXPECT_EQ( 10200, freq[ EHand::eStraight ] );
    //    EXPECT_EQ( 54912, freq[ EHand::eThreeOfAKind ] );
    //    EXPECT_EQ( 123552, freq[ EHand::eTwoPair ] );
    //    EXPECT_EQ( 1098240, freq[ EHand::ePair ] );
    //    EXPECT_EQ( 1302540, freq[ EHand::eHighCard ] );

    //    std::map< EHand, size_t > subFreq;
    //    for ( auto&& ii : uniqueHands )
    //    {
    //        auto hand = ii->determineHand();
    //        subFreq[ std::get< 0 >( hand ) ]++;
    //    }
    //    EXPECT_EQ( 10, subFreq[ EHand::eStraightFlush ] );
    //    EXPECT_EQ( 156, subFreq[ EHand::eFourOfAKind ] );
    //    EXPECT_EQ( 156, subFreq[ EHand::eFullHouse ] );
    //    EXPECT_EQ( 1277, subFreq[ EHand::eFlush ] );
    //    EXPECT_EQ( 10, subFreq[ EHand::eStraight ] );
    //    EXPECT_EQ( 858, subFreq[ EHand::eThreeOfAKind ] );
    //    EXPECT_EQ( 858, subFreq[ EHand::eTwoPair ] );
    //    EXPECT_EQ( 2860, subFreq[ EHand::ePair ] );
    //    EXPECT_EQ( 1277, subFreq[ EHand::eHighCard ] );
    //}

    //TEST_F( C4CardHandTester, Find3CardWinner )
    //{
    //    fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC 3C 2C" ) ); // ace high, QJ74
    //    fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "JC TC 8D 5D 4H 3H 2H" ) ); // J high, T854
    //    fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "JH TS 8H 5C 4S 3D 2D" ) ); // J high, T854
    //    fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 8C 7S 5C 4C 3S 2S" ) ); // K high, Q875

    //    auto winners = fGame->findWinners();
    //    EXPECT_EQ( 1, winners.size() );
    //    EXPECT_EQ( "Scott", winners.front()->name() );
    //    ASSERT_TRUE( winners.front()->getHand()->bestHand().has_value() );
    //    EXPECT_EQ( "Cards: 7D AS 4D QH JC", winners.front()->getHand()->bestHand().value().second->toString() );
    //    EXPECT_EQ( "High Card 'Ace' : Queen, Jack, Seven, Four kickers", winners.front()->getHand()->bestHand().value().second->determineHandName( true ) );
    //}

    //TEST_F( C4CardHandTester, Find3CardHandWild )
    //{
    //    auto hand = std::make_shared< CHand >( fGame->getCards( "3C 4D 7H KH 4H 2C 2H" ), nullptr ); // Ace H flush
    //    //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
    //    //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );

    //    EHand handValue;
    //    std::vector< ECard > card;
    //    std::vector< ECard > kickers;
    //    std::tie( handValue, card, kickers ) = hand->determineHand();
    //    EXPECT_EQ( EHand::eTwoPair, handValue );
    //    EXPECT_EQ( ECard::eFour, *card.begin() );
    //    EXPECT_EQ( ECard::eDeuce, *card.rbegin() );
    //    EXPECT_EQ( ECard::eKing, *kickers.begin() );

    //    hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eClubs ) );
    //    hand->addWildCard( fGame->getCard( ECard::eDeuce, ESuit::eHearts ) );
    //    std::tie( handValue, card, kickers ) = hand->determineHand();
    //    EXPECT_EQ( EHand::eFourOfAKind, handValue );
    //    EXPECT_EQ( ECard::eFour, *card.begin() );
    //    EXPECT_EQ( ECard::eKing, *kickers.begin() );
    //}

    class C5CardHandTester : public CHandTester
    {
    protected:
        C5CardHandTester() {}
        virtual ~C5CardHandTester() {}
    };
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

        auto allCards = getAllCards();
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

    TEST_F( C5CardHandTester, Pair )
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
                                    for ( auto&& kicker3 : ECard() )
                                    {
                                        if ( ( kicker3 == highCard ) || ( kicker3 == kicker1 ) || ( kicker3 == kicker2 ) )
                                            continue;
                                        for ( auto&& kickerSuit3 : ESuit() )
                                        {
                                            p1->clearCards();

                                            p1->addCard( fGame->getCard( highCard, highSuit1 ) );
                                            p1->addCard( fGame->getCard( highCard, highSuit2 ) );
                                            p1->addCard( fGame->getCard( kicker1, kickerSuit1 ) );
                                            p1->addCard( fGame->getCard( kicker2, kickerSuit2 ) );
                                            p1->addCard( fGame->getCard( kicker3, kickerSuit3 ) );

                                            auto hand = p1->determineHand();
                                            EXPECT_EQ( EHand::ePair, std::get< 0 >( hand ) );
                                            ASSERT_EQ( 1, std::get< 1 >( hand ).size() );

                                            EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
                                            auto kickers = std::vector< ECard >( { kicker1, kicker2, kicker3 } );
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

    TEST_F( C5CardHandTester, All5CardHands )
    {
        std::vector< std::shared_ptr< CHand > > allHands;
        size_t numHands = 0;
        auto allCards = getAllCards();
        for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
        {
            for ( size_t c2 = c1 + 1; c2 < allCards.size(); ++c2 )
            {
                for ( size_t c3 = c2 + 1; c3 < allCards.size(); ++c3 )
                {
                    for ( size_t c4 = c3 + 1; c4 < allCards.size(); ++c4 )
                    {
                        for ( size_t c5 = c4 + 1; c5 < allCards.size(); ++c5 )
                        {
                            auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ], allCards[ c3 ], allCards[ c4 ], allCards[ c5 ] } ), nullptr );
                            allHands.push_back( hand );
                            numHands++;
                        }
                    }
                }
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand >& lhs, const std::shared_ptr< CHand >& rhs )
                   { return lhs->operator>( *rhs ); }
        );

        EXPECT_EQ( 2598960, numHands );
        EXPECT_EQ( 2598960, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 7462, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for ( auto&& ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 40, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 624, freq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 3744, freq[ EHand::eFullHouse ] );
        EXPECT_EQ( 5108, freq[ EHand::eFlush ] );
        EXPECT_EQ( 10200, freq[ EHand::eStraight ] );
        EXPECT_EQ( 54912, freq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 123552, freq[ EHand::eTwoPair ] );
        EXPECT_EQ( 1098240, freq[ EHand::ePair ] );
        EXPECT_EQ( 1302540, freq[ EHand::eHighCard ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ( 10, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ( 156, subFreq[ EHand::eFourOfAKind ] );
        EXPECT_EQ( 156, subFreq[ EHand::eFullHouse ] );
        EXPECT_EQ( 1277, subFreq[ EHand::eFlush ] );
        EXPECT_EQ( 10, subFreq[ EHand::eStraight ] );
        EXPECT_EQ( 858, subFreq[ EHand::eThreeOfAKind ] );
        EXPECT_EQ( 858, subFreq[ EHand::eTwoPair ] );
        EXPECT_EQ( 2860, subFreq[ EHand::ePair ] );
        EXPECT_EQ( 1277, subFreq[ EHand::eHighCard ] );
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
