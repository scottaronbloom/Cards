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
#include "Hand.h"
#include <random>
#include <unordered_set>

CGame::CGame()
{
    createDeck();
    resetGames();
}


void CGame::shuffleAndDeal()
{
    shuffleDeck();
    dealCards();
}

CGame::~CGame()
{

}

void CGame::autoSetDealer()
{
    std::random_device rd;
    std::mt19937_64 gen( rd() );
    std::uniform_int_distribution< size_t > dis( 0, fPlayers.size() - 1 );
    auto newDealer = dis( gen );

    fDealer = fPlayers[ newDealer ];
    fDealer.lock()->setDealer( true );
}

void CGame::nextDealer()
{
    if ( fDealer.expired() )
        autoSetDealer();
    else
    {
        auto currDealer = fDealer.lock();
        currDealer->setDealer( false );
        fDealer = currDealer->nextPlayer();
        fDealer.lock()->setDealer( true );
    }
}

void CGame::prevDealer()
{
    if ( fDealer.expired() )
        autoSetDealer();
    else
    {
        auto currDealer = fDealer.lock();
        currDealer->setDealer( false );
        fDealer = currDealer->prevPlayer();
        fDealer.lock()->setDealer( true );
    }
}

QString CGame::dumpStats() const
{
    QString retVal = 
        QString( "Number of Games: %1\n" ).arg( fGames.size() ) +
        "=================================\n";
    retVal += QString( "Number of Games: %1\n" ).arg( fGames.size() );
    "=================================\n";
    retVal += QString( "Games Won by Hand:\n" );
    for ( auto&& ii : EHand() )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" ).arg( ::toString( ii, false ) ).arg( fWinsByHand[ static_cast< size_t >( ii ) ] ).arg( (100.0* fWinsByHand[ static_cast<size_t>( ii ) ] )/fGames.size(), 0, 'g', 3, QChar( '0' ) );
    }
    retVal += "=================================\n";
    retVal += QString( "Games Won by Player:\n" );
    for ( size_t ii = 0; ii < fPlayers.size(); ++ii )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" ).arg( fPlayers[ ii ]->name() ).arg( fWinsByPlayer[ static_cast<size_t>( ii ) ] ).arg( ( 100.0 * fWinsByPlayer[ static_cast<size_t>( ii ) ] ) / fGames.size(), 0, 'g', 3, QChar( '0' ) );
    }
    return retVal;
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
    addPlayer( "Scott" )->setPlayerID( fPlayers.size() - 1 );
    addPlayer( "Craig" )->setPlayerID( fPlayers.size() - 1 );
    addPlayer( "Keith" )->setPlayerID( fPlayers.size() - 1 );
    addPlayer( "Eric" )->setPlayerID( fPlayers.size() - 1 );
}


void CGame::resetGames()
{
    fGames.clear(); 
    fWinsByHand.clear(); 
    fWinsByHand.resize( static_cast< size_t >( EHand::eHighCard ) + 1);
    fWinsByPlayer.clear();
    fWinsByPlayer.resize( fPlayers.size() );
}

std::shared_ptr< CPlayer > CGame::addPlayer( const QString & name )
{
    auto newPlayer = std::make_shared< CPlayer >( name );
    auto lastPlayer = fPlayers.size() ? fPlayers.back() : nullptr;
    fPlayers.push_back( newPlayer );
    if ( lastPlayer )
        lastPlayer->setNextPlayer( newPlayer );

    newPlayer->setNextPlayer( fPlayers[ 0 ] );
    if ( fPlayers.size() > 1 )
    {
        fPlayers[ 0 ]->setPrevPlayer( newPlayer );
        newPlayer->setPrevPlayer( lastPlayer );
    }
    fWinsByPlayer.resize( fPlayers.size() );
    return newPlayer;
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
    {
        player->clearCards();
        player->setWinner( false );
    }

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

    auto winner = std::max_element( fPlayers.begin(), fPlayers.end(), 
               []( const std::shared_ptr< CPlayer > & lhs, const std::shared_ptr< CPlayer >& rhs )
               {
                   return lhs->operator <( *rhs );// sort in descending order
               } );
    (*winner)->setWinner( true );
    auto hand = (*winner)->hand();
    fGames.push_back( std::make_pair( hand, *winner ) );

    fWinsByHand[ static_cast< size_t >( hand ) ]++;
    fWinsByPlayer[ (*winner)->playerID() ]++;

}
