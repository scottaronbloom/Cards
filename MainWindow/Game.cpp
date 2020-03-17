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

#include "Game.h"
#include "Card.h"
#include "Player.h"

#include <random>
#include <unordered_set>

CGame::CGame()
{
    createDeck();
    createPlayers();
}


void CGame::shuffle()
{
    shuffleDeck();
    dealCards();
    //findWinner();
}


void CGame::nextDealer()
{
    if ( fDealer.expired() )
        fDealer = fPlayers[ 0 ];
    else
    {
        auto currDealer = fDealer.lock();
        currDealer->setDealer( false );
        fDealer = currDealer->nextPlayer();
    }
    fDealer.lock()->setDealer( true );
}

CGame::~CGame()
{

}


QString CGame::dumpGame() const
{
    QString retVal =
        //dumpDeck( false )
        //+ dumpDeck( true )
        dumpPlayers();
    return retVal;
}

void CGame::createDeck()
{
    for( const auto && currSuit : ESuit() )
    {
        for ( const auto&& currCard : ECard() )
        {
            auto card = std::make_shared< CCard >( currCard, currSuit );
            fCards.push_back( card );
        }
    }
}


QString CGame::dumpDeck( bool shuffled ) const
{
    QString data = 
        "=================================\n" +
        QString( shuffled ? "Shuffled " : "" ) + "Cards\n" +
        "=================================\n";
    for ( auto&& ii : ( shuffled ? fShuffledCards : fCards ) )
    {
        data += ii->toString( false ) + "\n";
    }
    return data;
}

QString CGame::dumpPlayers( ) const
{
    QString data = 
        "=================================\n"
        "Players\n"
        "=================================\n";
    for( auto && ii : fPlayers )
    {
        data += ii->toString() += "\n";
    }
    return data;
}

const int kScott{0};
const int kCraig{ 1 };
const int kKeith{ 2 };
const int kEric{ 3 };

void CGame::createPlayers()
{
    fPlayers.push_back( std::make_shared< CPlayer >( "Scott" ) );
    fPlayers.push_back( std::make_shared< CPlayer >( "Craig" ) );
    fPlayers.push_back( std::make_shared< CPlayer >( "Keith" ) );
    fPlayers.push_back( std::make_shared< CPlayer >( "Eric" ) );

    (*fPlayers.begin())->setPrevPlayer( *fPlayers.rbegin() );
    ( *fPlayers.rbegin() )->setNextPlayer( *fPlayers.begin() );

    for( size_t ii = 0; ii < fPlayers.size(); ++ii )
    {
        if ( ii == 0 )
            fPlayers[ ii ]->setPrevPlayer( fPlayers[ fPlayers.size() - 1 ] );
        else
            fPlayers[ ii ]->setPrevPlayer( fPlayers[ ii - 1 ] );

        if ( ii == ( fPlayers.size() - 1 ) )
            fPlayers[ ii ]->setNextPlayer( fPlayers[ 0 ] );
        else
            fPlayers[ ii ]->setNextPlayer( fPlayers[ ii + 1 ] );
    }
    nextDealer();
}

void CGame::shuffleDeck()
{
    std::unordered_set< size_t > usedCards;

    std::random_device rd;
    std::mt19937_64 gen( rd() );
    std::uniform_int_distribution<> dis( 0, 51 );

    fShuffledCards.clear();
    for( size_t ii = 0; ii < fCards.size(); ++ii )
    {
        size_t value = dis( gen );
        while( usedCards.find( value ) != usedCards.end() )
        {
            value = dis( gen );
        }
        fShuffledCards.push_back( fCards[ value ] );
        usedCards.insert( value );
    }
}

void CGame::dealCards()
{
    for ( auto&& player : fPlayers )
        player->clearCards();

    size_t currCard = 0;
    for( size_t ii = 0; ii < 5; ++ii )
    {
        for( auto && player : fPlayers)
        {
            player->addCard( fShuffledCards[ currCard++] );
            if ( currCard >= fShuffledCards.size() )
                break;
        }
        if ( currCard >= fShuffledCards.size() )
            break;
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

    std::sort( fPlayers.begin(), fPlayers.end(),
               []( const std::shared_ptr< CPlayer > & lhs, const std::shared_ptr< CPlayer >& rhs )
               {
                   return !(lhs->operator <( *rhs ));// sort in descending order
               } );
}
