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
#include "PlayInfo.h"

#include <random>
#include <unordered_set>

#include <QLocale>
#include <QDebug>

CGame::CGame()
{
    fPlayInfo = std::make_shared< SPlayInfo >();

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
    if ( fPlayers.empty() )
         return;
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
        auto nextDealer = currDealer->nextPlayer();
        if ( !nextDealer.expired() )
        {
            fDealer = nextDealer;
            fDealer.lock()->setDealer( true );
        }
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
        QString( "Number of Games: %1\n" ).arg( locale.toString( static_cast< qulonglong >( fGames.size() ) ) ) +
        "=================================\n";

    retVal += QString( "Games Won by Player:\n" );
    for ( size_t ii = 0; ii < fPlayers.size(); ++ii )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" )
            .arg( fPlayers[ ii ]->name() )
            .arg( locale.toString( static_cast< qulonglong >( fWinsByPlayer[ static_cast<size_t>( ii ) ] ) ) )
            .arg( locale.toString( ( 100.0 * fWinsByPlayer[ static_cast<size_t>( ii ) ] ) / fGames.size(), 'g', 3 ) );
    }
    retVal += "=================================\n";

    retVal += QString( "Games Won by Hand:\n" );
    for ( auto&& ii : EHand() )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" )
            .arg( ::toString( ii, false ) )
            .arg( locale.toString( static_cast< qulonglong >( fWinsByHand[ static_cast< size_t >( ii ) ] ) ) )
            .arg( locale.toString( (100.0* fWinsByHand[ static_cast<size_t>( ii ) ] )/fGames.size(), 'g', 3 ) );
    }
    retVal += "=================================\n";

    retVal += QString( "Hand Count:\n" );
    for ( auto&& ii : EHand() )
    {
        retVal += QString( "\t%1 - %2 (%3%)\n" )
            .arg( ::toString( ii, false ) )
            .arg( locale.toString( static_cast< qulonglong >( fHandCount[ static_cast<size_t>( ii ) ] ) ) )
            .arg( locale.toString( ( 100.0 * fHandCount[ static_cast<size_t>( ii ) ] ) / fGames.size(), 'g', 3 ) );
    }

    return retVal;
}

QString CGame::dumpGame( bool details ) const
{
    QString retVal =
        //dumpDeck( false )
        //+ dumpDeck( true )
        dumpGameDetails( details )
        + dumpPlayers( details );
    return retVal;
}

void CGame::createDeck()
{
    fCards = CCard::allCards();
    for( auto && card : fCards )
    {
        fStringCardMap[ card->toString( false, false ) ] = card;
        fStringCardMap[ card->toString( true, false ) ] = card;
        fCardMap[ std::make_pair( card->getCard(), card->getSuit() ) ] = card;
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

QString CGame::dumpGameDetails( bool /*details*/ ) const
{
    QString data =
        "=================================\n"
        "Game\n"
        "=================================\n";
    QStringList cards;
    for( auto && ii : fNumCardsToDeal )
        cards << QString::number( ii );

    data += QString( "Number of Cards: %1\n" ).arg( cards.join( "," ) );
    cards.clear();
    for( auto && ii : fPlayInfo->fWildCards )
    {
        cards << ii->toString( false, false );
    }
    data += QString( "Wild Cards: %1\n" ).arg( cards.join( "," ) );
    data += QString( "Sub 5 Card Poker Straights/Flushes Count: %1\n" ).arg( fPlayInfo->fStraightsFlushesCountForSmallHands ? "Yes" : "No" );
    data += QString( "Lowball/Razz: %1\n" ).arg( fPlayInfo->fLowBall ? "Yes" : "No" );
    return data;
}

QString CGame::dumpPlayers( bool details ) const
{
    QString data = 
        "=================================\n"
        "Players\n"
        ;
    for( auto && ii : fPlayers )
    {
        data += ii->toString( details ) += "\n";
    }
    return data;
}

void CGame::resetGames()
{
    fGames.clear(); 
    fHandCount.clear(); 
    fHandCount.resize( static_cast< size_t >( EHand::eFiveOfAKind ) + 1);
    fWinsByHand.clear();
    fWinsByHand.resize( static_cast<size_t>( EHand::eFiveOfAKind ) + 1 );
    fWinsByPlayer.clear();
    fWinsByPlayer.resize( fPlayers.size() );
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
    for ( auto && player : fPlayers )
    {
        player->clearCards();
        player->setWinner( false );
    }

    if ( fDealer.expired() )
        nextDealer();

    auto currCard = fShuffledCards.begin();

    for( auto currDeal = fNumCardsToDeal.begin(); ( currCard != fShuffledCards.end() ) && ( currDeal != fNumCardsToDeal.end() ); ++currDeal  )
    {
        for( uint8_t ii = 0; ii < *currDeal; ++ii )
        {
            auto currPlayer = fDealer.lock();
            while( currPlayer && ( currCard != fShuffledCards.end() ) )
            {
                currPlayer->addCard( *currCard );
                ++currCard;
                currPlayer = currPlayer->nextPlayer().lock();
                if ( currPlayer == fDealer.lock() )
                    break;
            }
        }
    }

    analyzeHand( true );
}

void CGame::analyzeHand( bool updateStats )
{
    for( auto && ii : fPlayers )
    {
        ii->resetHandAnalysis();
    }

    auto winners = findWinners();
    if ( winners.empty() )
        return;

    if ( !updateStats )
        return;

    for( auto && ii : fPlayers )
        fHandCount[ static_cast< size_t >( ii->hand() ) ]++;

    for( auto && winner : winners )
    {
        fWinsByHand[ static_cast<size_t>( winner->hand() ) ]++;
        fWinsByPlayer[ winner->playerID() ]++;
    }
}

std::list< std::shared_ptr< CPlayer > > CGame::findWinners()
{
    auto currGame = fPlayers;

    //qDebug() << "==============================";
    //qDebug() << "Pre Sort";
    //qDebug() << "==============================";
    //for( auto && ii : currGame )
    //{
    //    qDebug() << "Player: " << ii->name() << ii->getHand()->toString();
    //}

    std::stable_sort( currGame.begin(), currGame.end(),
               [ this ]( const std::shared_ptr< CPlayer >& lhs, const std::shared_ptr< CPlayer >& rhs )
                   {
                       return lhs->operator<( *rhs );
                   } );

    //qDebug() << "==============================";
    //qDebug() << "Post Sort";
    //qDebug() << "==============================";
    //for ( auto&& ii : currGame )
    //{
    //    qDebug() << "Player: " << ii->name() << ii->getHand()->toString();
    //}

    std::list< std::shared_ptr< CPlayer > > winners;
    auto currWinner = currGame.rbegin();
    auto winner = *currWinner;
    if ( winner && winner->hasCards() )
    {
        winners.push_back( winner );
    }
    currWinner++;
    while( ( currWinner != currGame.rend() ) && winner->operator==( **currWinner ) )
    {
        if ( (*currWinner)->hasCards() )
            winners.push_back( *currWinner );
        currWinner++;
    }

    for( auto && curr : winners )
    {
        curr->setWinner( true );
        auto hand = curr->hand();
        fGames.push_back( std::make_pair( hand, curr ) );
    }

    return winners;
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

std::vector< std::shared_ptr< CCard > > CGame::getCards( const QString& cardNames ) const
{
    std::vector< std::shared_ptr< CCard > > retVal;
    for ( int ii = 0; ii < cardNames.length()-1; ++ii )
    {
        if ( ( cardNames[ ii ].isSpace() ) || ( cardNames[ ii ] == '_' ) )
            continue;

        auto cardName = cardNames.mid( ii, 2 );
        auto currCard = getCard( cardName );
        if ( !currCard )
            return {};
        retVal.push_back( currCard );
        ii++; // skip one
    }
    return retVal;
}

std::vector< std::shared_ptr< CCard > > CGame::getCards( const QString& cardNames, const QString & suitNames, bool allowAll ) const
{
    bool allCards = cardNames.toLower() == "all";
    bool allSuits = suitNames.toLower() == "all";
    if ( !allowAll && ( allCards && allSuits ) )
        return {};

    ECard card;
    if ( !allCards && !::fromString( card, cardNames ) )
        return {};
   
    ESuit suit;
    if ( !allSuits && !::fromString( suit, suitNames ) )
        return {};

    std::list< ECard > cards;
    if ( allCards )
    {
        for ( auto&& ii : ECard() )
            cards.push_back( ii );
    }
    else
    {
        cards.push_back( card );
    }

    std::list< ESuit > suits;
    if ( allSuits )
    {
        for ( auto&& ii : ESuit() )
            suits.push_back( ii );
    }
    else
    {
        suits.push_back( suit );
    }
    
    std::vector< std::shared_ptr< CCard > > retVal;
    for ( auto&& currCard : cards )
    {
        for( auto && currSuit : suits )
        {
            retVal.push_back( getCard( currCard, currSuit ) );
        }
    }

    return retVal;
}

size_t CGame::setNumPlayers( size_t numPlayers )
{
    while( numPlayers > fPlayers.size() )
    {
        fPlayers.push_back( std::make_shared< CPlayer >( fPlayInfo ) );
    }
    while( numPlayers < fPlayers.size() )
        fPlayers.pop_back();
    for( size_t ii = 0; ii < fPlayers.size(); ++ii )
        fPlayers[ ii ]->setPlayerID( ii );

    fWinsByPlayer.resize( fPlayers.size() );
    return fPlayers.empty() ? 0 : ( fPlayers.size() - 1 );
}

std::shared_ptr< CPlayer > CGame::addPlayer( const QString& name )
{
    auto pos = setNumPlayers( fPlayers.size() + 1 );
    return setPlayerName( pos, name ).second;
}

std::pair< bool, std::shared_ptr< CPlayer > > CGame::setPlayerName( size_t playerNum, const QString& playerName )
{
    if ( playerNum >= fPlayers.size() )
        return std::make_pair( false, nullptr );

    auto currPlayer = fPlayers[ playerNum ];
    if ( !currPlayer )
    {
        return std::make_pair( false, nullptr );
    }
    bool retVal = fPlayers[ playerNum ]->setName( playerName );

    recomputeNextPrev();

    return std::make_pair( retVal, fPlayers[ playerNum ] );
}

void CGame::recomputeNextPrev()
{
    auto prev = ( fPlayers.size() > 1 ) ? fPlayers[ fPlayers.size() - 1 ] : nullptr;
    for ( size_t ii = 0; ii < fPlayers.size(); ++ii )
    {
        if ( !fPlayers[ ii ] )
            continue;

        fPlayers[ ii ]->setPrevPlayer( prev );
        prev = fPlayers[ ii ];

        if ( ii < ( fPlayers.size() - 1 ) )
            fPlayers[ ii ]->setNextPlayer( fPlayers[ ii + 1 ] );
    }
    if ( fPlayers.size() > 1 )
        fPlayers[ fPlayers.size() - 1 ]->setNextPlayer( fPlayers[ 0 ] );
}

void CGame::removePlayer( size_t playerNum )
{
    if ( playerNum >= fPlayers.size() )
        return;
    auto player = fPlayers[ playerNum ];
    auto prev = player->prevPlayer().lock();
    auto next = player->nextPlayer().lock();
    if ( prev )
        prev->setNextPlayer( next );
    if ( next )
        next->setPrevPlayer( prev );
}

void CGame::setStraightsFlushesCountForSmallHands( bool straightsFlushesCountForSmallHands )
{
    fPlayInfo->fStraightsFlushesCountForSmallHands = straightsFlushesCountForSmallHands;
}

bool CGame::straightsFlushesCountForSmallHands() const
{
    return fPlayInfo->fStraightsFlushesCountForSmallHands;
}

void CGame::setLowHandWins( bool lowBall )
{
    fPlayInfo->fLowBall = lowBall;
}

bool CGame::lowHandWins() const
{
    return fPlayInfo->fLowBall;
}

void CGame::addWildCard( std::shared_ptr< CCard > card )
{
    fPlayInfo->fWildCards.insert( card );
}

void CGame::addWildCards( const std::vector< std::shared_ptr< CCard > > & cards )
{
    fPlayInfo->fWildCards.insert( cards.begin(), cards.end() );
}

void CGame::clearWildCards()
{
    fPlayInfo->fWildCards.clear();
}
