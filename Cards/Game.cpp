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

#include <QLocale>
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
    QLocale locale;
    QString retVal = 
        QString( "Number of Games: %1\n" ).arg( locale.toString( fGames.size() ) ) +
        "=================================\n";

    retVal += QString( "Games Won by Player:\n" );
    for ( size_t ii = 0; ii < fPlayers.size(); ++ii )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" ).arg( fPlayers[ ii ]->name() ).arg( locale.toString( fWinsByPlayer[ static_cast<size_t>( ii ) ] ) ).arg( locale.toString( ( 100.0 * fWinsByPlayer[ static_cast<size_t>( ii ) ] ) / fGames.size(), 'g', 3 ) );
    }
    retVal += "=================================\n";

    retVal += QString( "Games Won by Hand:\n" );
    for ( auto&& ii : EHand() )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" ).arg( ::toString( ii, false ) ).arg( locale.toString( fWinsByHand[ static_cast< size_t >( ii ) ] ) ).arg( locale.toString( (100.0* fWinsByHand[ static_cast<size_t>( ii ) ] )/fGames.size(), 'g', 3 ) );
    }
    retVal += "=================================\n";

    retVal += QString( "Hand Count:\n" );
    for ( auto&& ii : EHand() )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" ).arg( ::toString( ii, false ) ).arg( locale.toString( fHandCount[ static_cast<size_t>( ii ) ] ) ).arg( locale.toString( ( 100.0 * fHandCount[ static_cast<size_t>( ii ) ] ) / fGames.size(), 'g', 3 ) );
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
            fStringCardMap[ card->toString( false, false ) ] = card;
            fStringCardMap[ card->toString( true, false ) ] = card;
            fCardMap[ std::make_pair( currCard, currSuit ) ] = card;
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
        data += ii->toString( false, true ) + "\n";
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

void CGame::resetGames()
{
    fGames.clear(); 
    fHandCount.clear(); 
    fHandCount.resize( static_cast< size_t >( EHand::eHighCard ) + 1);
    fWinsByHand.clear();
    fWinsByHand.resize( static_cast<size_t>( EHand::eHighCard ) + 1 );
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

    std::random_device rd;
    std::mt19937_64 gen( rd() );
    fShuffledCards = fCards;
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
    std::shuffle( fShuffledCards.begin(), fShuffledCards.end(), gen );
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
                   return !lhs->operator <( *rhs );// sort in descending order
               } );
    (*winner)->setWinner( true );
    auto hand = (*winner)->hand();
    fGames.push_back( std::make_pair( hand, *winner ) );

    for( auto && ii : fPlayers )
        fHandCount[ static_cast< size_t >( ii->hand() ) ]++;
    fWinsByHand[ static_cast<size_t>( hand ) ]++;
    fWinsByPlayer[ (*winner)->playerID() ]++;
}

std::shared_ptr< CCard > CGame::getCard( const QString & cardName ) const
{
    auto pos = fStringCardMap.find( cardName );
    if ( pos == fStringCardMap.end() )
        return nullptr;
    return (*pos).second;
}

std::shared_ptr< CCard > CGame::getCard( ECard card, ESuit suit ) const
{
    auto pos = fCardMap.find( std::make_pair( card, suit ) );
    if ( pos == fCardMap.end() )
        return nullptr;
    return ( *pos ).second;
}
