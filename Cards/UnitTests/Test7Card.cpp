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
    TEST_F( CHandTester, Find7CardWinner )
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

    TEST_F( CHandTester, Find7CardHandWild )
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
}  // namespace


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}
