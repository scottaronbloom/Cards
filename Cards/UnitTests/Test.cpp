/*
 * (c) Copyright 2004 - 2019 Blue Pearl Software Inc.
 * All rights reserved.
 *
 * This source code belongs to Blue Pearl Software Inc.
 * It is considered trade secret and confidential, and is not to be used
 * by parties who have not received written authorization
 * from Blue Pearl Software Inc.
 *
 * Only authorized users are allowed to use, copy and modify
 * this software provided that the above copyright notice
 * remains in all copies of this software.
 *
 *
 *
*/
#include "Cards/Game.h"
#include "Cards/Player.h"
#include "Cards/Hand.h"
#include "Cards/Card.h"

#include <string>
#include "gmock/gmock.h"

std::ostream& operator<<( std::ostream& os, const QString & data )
{
    return os << data.toStdString();  
}

void PrintTo( const QString& data, std::ostream * os )
{
    *os << data.toStdString();
}

namespace
{
    class CHandTester : public ::testing::Test
    {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.
        CHandTester()
        {
            fGame = new CGame;
        }
        virtual ~CHandTester()
        {
            delete fGame;
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        virtual void SetUp()
        {
        }
        virtual void TearDown()
        {
        }
        bool isStraight( std::vector< ECard > cards ) const
        {
            std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
            if ( ( cards[ 0 ] == ( cards[ 1 ] + 1 ) )
                 && ( cards[ 1 ] == ( cards[ 2 ] + 1 ) )
                 && ( cards[ 2 ] == ( cards[ 3 ] + 1 ) )
                 && ( cards[ 3 ] == ( cards[ 4 ] + 1 ) ) )
                return true;
            if ( ( cards[ 0 ] == ECard::eAce ) &&
                ( cards[ 1 ] == ECard::eFive ) &&
                 ( cards[ 2 ] == ECard::eFour )&&
                 ( cards[ 3 ] == ECard::eTrey )&&
                 ( cards[ 4 ] == ECard::eTwo ) )
                return true;
            return false;
        }
    public:
        CGame * fGame{nullptr};
    };

    TEST_F( CHandTester, CheckBitValues )
    {
        static std::map< std::pair< ESuit, ECard >, int > map =
        {
             { {     ESuit::eClubs,   ECard::eTwo }, 0x00018002 }
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
            ,{ {  ESuit::eDiamonds,   ECard::eTwo }, 0x00014002 }
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
            ,{ {    ESuit::eHearts,   ECard::eTwo }, 0x00012002 }
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
            ,{ {    ESuit::eSpades,   ECard::eTwo }, 0x00011002 }
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
        for( auto && ii : allCards )
        {
            auto card = ii->getCard();
            auto suit = ii->getSuit();
            auto bitValue = ii->bitValue();

            auto pos = map.find( std::make_pair( suit, card ) );
            ASSERT_TRUE( pos != map.end() );
            EXPECT_EQ( (*pos).second, bitValue.to_ulong() );
        }
    }

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
                    p1->addCard( fGame->getCard( ECard::eTwo, suit ) );
                    p1->addCard( fGame->getCard( ECard::eAce, suit ) );
                }
                else
                {
                    for( auto currCard = ( highCard - 4 ); currCard <= highCard; ++currCard )
                    {
                        p1->addCard( fGame->getCard( currCard, suit ) );
                    }
                }
                auto hand =p1->determineHand();
                EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
                ASSERT_EQ( 1, std::get< 1 >( hand ).size() );
                ASSERT_EQ( 4, std::get< 2 >( hand ).size() );
                EXPECT_EQ( highCard, std::get< 1 >( hand ).front() );
            }
        }
    }

    TEST_F( CHandTester, FourOfAKind )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for( auto && kicker : ECard() )
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

    TEST_F( CHandTester, FullHouse )
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

    TEST_F( CHandTester, TwoPair )
    {
        auto p1 = fGame->addPlayer( "Scott" );

        for ( auto&& highCard : ECard() )
        {
            for ( auto && highSuit1 : ESuit() )
            {
                for ( auto&& highSuit2 : ESuit() )
                {
                    if ( highSuit1 == highSuit2 )
                        continue;

                    for( auto && lowCard : ECard() )
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
                                    for ( auto && kickSuit1 : ESuit() )
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

    TEST_F( CHandTester, FindWinner )
    {
        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC" ) ); // ace high, QJ74
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // 9 high, 8632
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards(  "9H 8S 6H 3C 2S" ) ); // 9 high, 8632
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 7C 6S 5C" ) ); // K high, Q765

        auto winners = fGame->findWinners();
        EXPECT_EQ( 1, winners.size() );
        EXPECT_EQ( "Scott", winners.front()->name() );
    }

    TEST_F( CHandTester, FindWinnerWild )
    {
        for( auto && suit : ESuit() )
        {
            fGame->addWildCard( fGame->getCard( ECard::eTwo, suit ) ); // all twos wild
        }

        fGame->addPlayer( "Scott" )->setCards( fGame->getCards( "7D AS 4D QH JC" ) ); // ace high, QJ74
        fGame->addPlayer( "Craig" )->setCards( fGame->getCards( "9C 8C 6D 3D 2H" ) ); // Pair of 9s (9C 2H), 863
        fGame->addPlayer( "Eric" )->setCards( fGame->getCards( "9H 8S 6H 3C 2S" ) ); // Pair of 9s (9C 2S), 863
        fGame->addPlayer( "Keith" )->setCards( fGame->getCards( "KD QS 7C 6S 5C" ) ); // K high, Q765

        auto winners = fGame->findWinners();
        EXPECT_EQ( 2, winners.size() );
        EXPECT_EQ( "Craig", winners.back()->name() );
        EXPECT_EQ( "Pair of 'Nine' - 'Eight, Six, Three' kicker", winners.back()->getHand()->determineHandName( true ) );

        EXPECT_EQ( "Eric", winners.front()->name() );
        EXPECT_EQ( "Pair of 'Nine' - 'Eight, Six, Three' kicker", winners.front()->getHand()->determineHandName( true ) );
    }

    TEST_F( CHandTester, DetermineHandWild )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2C 2H" ) ); // Ace H flush
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFlush, handValue );
        EXPECT_EQ( ECard::eAce, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2H 2H" ) ); // Invalid hand
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eNoCards, handValue );

        hand = std::make_shared< CHand >( fGame->getCards( "7H KH 4H 2C 2D" ) ); // Pair of kings
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) ); // 2 clubs and hearts wild
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) ); 
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::ePair, handValue );
        EXPECT_EQ( ECard::eKing, *card.begin() );
        EXPECT_EQ( ECard::eSeven, *kickers.begin() );
        EXPECT_EQ( ECard::eTwo, *kickers.rbegin() );
    }

    TEST_F( CHandTester, DetermineHand5OfAKind )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "2S 2D 4H 2C 2H" ) ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eSpades ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( "Five of a Kind 'Four'", hand->bestHand().value().second->determineHandName( true ) );

        hand = std::make_shared< CHand >( fGame->getCards( "2S 4C 4H 2C 2H" ) ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "2S 4C 4H 4D 2H" ) ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );

        hand = std::make_shared< CHand >( fGame->getCards( "4S 4C 4H 4D 2H" ) ); // 5 of a kind 4s
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eDiamonds ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eSpades ) );

        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFiveOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
    }

    TEST_F( CHandTester, FindWinner7Card )
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

    TEST_F( CHandTester, Determine7CHandWild )
    {
        auto hand = std::make_shared< CHand >( fGame->getCards( "3C 4D 7H KH 4H 2C 2H" ) ); // Ace H flush
        //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        //hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );

        EHand handValue;
        std::vector< ECard > card;
        std::vector< ECard > kickers;
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eTwoPair, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( ECard::eTwo, *card.rbegin() );
        EXPECT_EQ( ECard::eKing, *kickers.begin() );

        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eClubs ) );
        hand->addWildCard( fGame->getCard( ECard::eTwo, ESuit::eHearts ) );
        std::tie( handValue, card, kickers ) = hand->determineHand();
        EXPECT_EQ( EHand::eFourOfAKind, handValue );
        EXPECT_EQ( ECard::eFour, *card.begin() );
        EXPECT_EQ( ECard::eKing, *kickers.begin() );
    }

    TEST_F( CHandTester, AllHands )
    {
        std::vector< std::shared_ptr< CHand > > allHands;
        size_t numHands = 0;
        auto allCards = getAllCards();
        for ( size_t c1 = 0; c1 < allCards.size(); ++c1 )
        {
            for ( size_t c2 = c1+1; c2 < allCards.size(); ++c2 )
            {
                for ( size_t c3 = c2 + 1; c3 < allCards.size(); ++c3 )
                {
                    for ( size_t c4 = c3 + 1; c4 < allCards.size(); ++c4 )
                    {
                        for ( size_t c5 = c4 + 1; c5 < allCards.size(); ++c5 )
                        {
                            auto hand = std::make_shared< CHand >( std::vector< std::shared_ptr< CCard > >( { allCards[ c1 ], allCards[ c2 ], allCards[ c3 ], allCards[ c4 ], allCards[ c5 ] } ) );
                            allHands.push_back( hand );
                            numHands++;
                        }
                    }
                }
            }
        }

        std::sort( allHands.begin(), allHands.end(), []( const std::shared_ptr< CHand > &lhs, const std::shared_ptr< CHand > & rhs ) 
                   { return lhs->operator>( *rhs ); } 
        );

        EXPECT_EQ( 2598960, numHands );
        EXPECT_EQ( 2598960, allHands.size() );

        std::vector< std::shared_ptr< CHand > > uniqueHands;
        std::shared_ptr< CHand > prevHand;
        for( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            if ( prevHand && ( prevHand->operator==( *allHands[ ii ] ) ) )
                continue;
            prevHand = allHands[ ii ];
            uniqueHands.push_back( prevHand );
        }
        EXPECT_EQ( 7462, uniqueHands.size() );

        std::map< EHand, size_t > freq;
        for( auto && ii : allHands )
        {
            auto hand = ii->determineHand();
            freq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ(      40, freq[ EHand::eStraightFlush ] );
        EXPECT_EQ(     624, freq[ EHand::eFourOfAKind   ] );
        EXPECT_EQ(    3744, freq[ EHand::eFullHouse     ] );
        EXPECT_EQ(    5108, freq[ EHand::eFlush         ] );
        EXPECT_EQ(   10200, freq[ EHand::eStraight      ] );
        EXPECT_EQ(   54912, freq[ EHand::eThreeOfAKind  ] );
        EXPECT_EQ(  123552, freq[ EHand::eTwoPair       ] );
        EXPECT_EQ( 1098240, freq[ EHand::ePair          ] );
        EXPECT_EQ( 1302540, freq[ EHand::eHighCard      ] );

        std::map< EHand, size_t > subFreq;
        for ( auto&& ii : uniqueHands )
        {
            auto hand = ii->determineHand();
            subFreq[ std::get< 0 >( hand ) ]++;
        }
        EXPECT_EQ(   10, subFreq[ EHand::eStraightFlush ] );
        EXPECT_EQ(  156, subFreq[ EHand::eFourOfAKind   ] );
        EXPECT_EQ(  156, subFreq[ EHand::eFullHouse     ] );
        EXPECT_EQ( 1277, subFreq[ EHand::eFlush         ] );
        EXPECT_EQ(   10, subFreq[ EHand::eStraight      ] );
        EXPECT_EQ(  858, subFreq[ EHand::eThreeOfAKind  ] );
        EXPECT_EQ(  858, subFreq[ EHand::eTwoPair       ] );
        EXPECT_EQ( 2860, subFreq[ EHand::ePair          ] );
        EXPECT_EQ( 1277, subFreq[ EHand::eHighCard      ] );
    }
}  // namespace


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}
