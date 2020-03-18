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
 * $Author: scott $ - $Revision: 55850 $ - $Date: 2019-11-02 02:25:36 -0700 (Sat, 02 Nov 2019) $
 * $HeadURL: http://bpsvn/svn/trunk/CoreApp/dr/UnitTests/TestAndGatingTree.cpp $
 *
 *
*/
#include "Cards/Game.h"
#include "Cards/Player.h"
#include "Cards/Hand.h"

#include <string>
#include "gmock/gmock.h"

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
    public:
        CGame * fGame{nullptr};
    };

    TEST_F(CHandTester, Flushes) {
        auto p1 = fGame->addPlayer( "Scott" );

        for( )
        p1->addCard( fGame->getCard( "AH" ) );
        p1->addCard( fGame->getCard( "KH" ) );
        p1->addCard( fGame->getCard( "QH" ) );
        p1->addCard( fGame->getCard( "JH" ) );
        p1->addCard( fGame->getCard( "TH" ) );

        auto hand =p1->determineHand();
        EXPECT_EQ( EHand::eStraightFlush, std::get< 0 >( hand ) );
    }

}  // namespace


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    return retVal;
}



//fPlayers[ kScott ]->clearCards();
//fPlayers[ kScott ]->addCard( fCards[ 8 ] );
//fPlayers[ kScott ]->addCard( fCards[ 9 ] );
//fPlayers[ kScott ]->addCard( fCards[ 10 ] );
//fPlayers[ kScott ]->addCard( fCards[ 11 ] );
//fPlayers[ kScott ]->addCard( fCards[ 12 ] ); // straight flush to the ace -- first

//fPlayers[ kCraig ]->clearCards();
//fPlayers[ kCraig ]->addCard( fCards[ 8 ] );
//fPlayers[ kCraig ]->addCard( fCards[ 22 ] );
//fPlayers[ kCraig ]->addCard( fCards[ 36 ] );
//fPlayers[ kCraig ]->addCard( fCards[ 50 ] );
//fPlayers[ kCraig ]->addCard( fCards[ 12 ] ); // straight to the ace - 3rd

//fPlayers[ kKeith ]->clearCards();
//fPlayers[ kKeith ]->addCard( fCards[ 0 ] );
//fPlayers[ kKeith ]->addCard( fCards[ 14 ] );
//fPlayers[ kKeith ]->addCard( fCards[ 28 ] );
//fPlayers[ kKeith ]->addCard( fCards[ 42 ] );
//fPlayers[ kKeith ]->addCard( fCards[ 4 ] ); // straight to the 6 // last

//fPlayers[ kEric ]->clearCards();
//fPlayers[ kEric ]->addCard( fCards[ 0 ] );
//fPlayers[ kEric ]->addCard( fCards[ 1 ] );
//fPlayers[ kEric ]->addCard( fCards[ 2 ] );
//fPlayers[ kEric ]->addCard( fCards[ 3 ] );
//fPlayers[ kEric ]->addCard( fCards[ 12 ] ); // wheel flush  - second

//bool isLess = fPlayers[ kScott ]->operator <( *fPlayers[ kCraig ] ); // false
//isLess = fPlayers[ kScott ]->operator <( *fPlayers[ kKeith ] ); // false
//isLess = fPlayers[ kScott ]->operator <( *fPlayers[ kEric ] ); // false

//isLess = fPlayers[ kCraig ]->operator <( *fPlayers[ kScott ] ); // true
//isLess = fPlayers[ kCraig ]->operator <( *fPlayers[ kKeith ] ); // false
//isLess = fPlayers[ kCraig ]->operator <( *fPlayers[ kEric ] );  // true

//isLess = fPlayers[ kKeith ]->operator <( *fPlayers[ kScott ] ); // true
//isLess = fPlayers[ kKeith ]->operator <( *fPlayers[ kCraig ] ); // true
//isLess = fPlayers[ kKeith ]->operator <( *fPlayers[ kEric ] ); // true

//isLess = fPlayers[ kEric ]->operator <( *fPlayers[ kScott ] ); // true
//isLess = fPlayers[ kEric ]->operator <( *fPlayers[ kCraig ] ); // false
//isLess = fPlayers[ kEric ]->operator <( *fPlayers[ kKeith ] ); // false
