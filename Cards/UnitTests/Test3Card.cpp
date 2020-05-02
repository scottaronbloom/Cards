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

#include <string>
#include "gmock/gmock.h"

namespace NHandTester
{
    TEST_F(CHandTester, StraightFlushes) 
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for( auto suit : ESuit() )
        {
            for( auto && highCard : ECard() )
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
                    for( auto currCard = ( highCard - 4 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_TRUE( p1->isStraight() );
                auto hand =p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( CHandTester, Flush )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto suit : ESuit() )
        {
            for ( auto && highCard : ECard() )
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

                auto hand = p1->determineHand();
                EXPECT_TRUE( p1->isFlush() );
                EXPECT_EQ( EHand::eFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( CHandTester, Straight )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            if ( highCard <= ECard::eFour )
                continue;

            for( auto && suit : ESuit() )
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

                auto hand = p1->determineHand();
                EXPECT_TRUE( p1->isStraight() );
                EXPECT_EQ( EHand::eStraight, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( CHandTester, ThreeOfAKind )
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

                        for( auto && kickSuit1 : ESuit() )
                        {
                            for( auto && kickSuit2 : ESuit() )
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

    TEST_F( CHandTester, Pair )
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

                    for ( auto && kicker1 : ECard() )
                    {
                        if ( kicker1 == highCard )
                            continue;
                        for ( auto&& kickerSuit1 : ESuit() )
                        {
                            for ( auto&& kicker2 : ECard() )
                            {
                                if ( ( kicker2 == highCard ) || ( kicker2 == kicker1 ) )
                                    continue;

                                for ( auto && kickerSuit2 : ESuit() )
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

    TEST_F( CHandTester, DISABLED_HighCard )
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

                                            if( isStraight( std::vector< ECard >( { card1, card2, card3, card4, card5 } ) ) )
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

    TEST_F( CHandTester, All3CardHands )
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

    //TEST_F( CHandTester, Find3CardWinner )
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

    //TEST_F( CHandTester, Find3CardHandWild )
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
}  // namespace


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}
