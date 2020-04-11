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

#ifndef _GAME_H
#define _GAME_H

#include "SABUtils/QtUtils.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <QString>

class CCard;
class CPlayer;
enum class EHand;
enum class ECard;
enum class ESuit : uint8_t;

using TCardDeal = std::pair< std::vector< uint8_t >, std::vector< uint8_t > >;
namespace std
{
    template <>
    struct hash< std::pair< ECard, ESuit > >
    {
        std::size_t operator()( const std::pair< ECard, ESuit > & k ) const
        {
            auto h1 = std::hash< uint64_t >{}( static_cast< uint64_t >( k.first ) );
            auto h2 = std::hash< uint64_t >{}( static_cast<uint64_t>( k.second ) );
            return h1 ^ (h2<<1);
        }
    };
}

class CGame 
{
public:
    CGame();
    virtual ~CGame();
public:
    std::weak_ptr< CPlayer > currDealer() const{ return fDealer; }

    QString dumpGame( bool details ) const;
    void shuffleAndDeal();
    void nextDealer();
    void prevDealer();
    void autoSetDealer();
    std::list< std::shared_ptr< CPlayer > > findWinners(); // possible ties

    void resetGames();
    size_t numGames() const{ return fGames.size(); }
    QString dumpStats() const;
    std::shared_ptr< CCard > getCard( const QString & cardName ) const;
    std::shared_ptr< CCard > getCard( ECard card, ESuit suit ) const;
    std::vector< std::shared_ptr< CCard > > getCards( const QString & cardNames ) const;

    void setNumCards( uint8_t numCards ){ fNumCardsToDeal = TCardDeal( { numCards }, {} ); }
    void setCardDeal( const TCardDeal & cards ){ fNumCardsToDeal = cards; }
    TCardDeal cardDeal() const{ return fNumCardsToDeal; }

    size_t setNumPlayers( size_t numPlayers );
    std::shared_ptr< CPlayer > addPlayer( const QString& name );
    std::pair< bool, std::shared_ptr< CPlayer > > setPlayerName( size_t playerNum, const QString & playerName );

    void removePlayer( size_t playerNum );

    void addWildCard( std::shared_ptr< CCard > card );
    void clearWildCards();
private:
    void recomputeNextPrev();
    void createDeck();

    void shuffleDeck();
    void dealCards();

    QString dumpDeck( bool shuffled ) const;
    QString dumpPlayers( bool details ) const;

    std::vector< std::shared_ptr< CPlayer > > fPlayers;
    std::weak_ptr< CPlayer > fDealer;
    std::vector< std::shared_ptr< CCard > > fCards; // original and sorted
    std::vector< std::shared_ptr< CCard > > fShuffledCards;
    std::unordered_map< QString, std::shared_ptr< CCard > > fStringCardMap;
    std::unordered_map< std::pair< ECard, ESuit >, std::shared_ptr< CCard > > fCardMap;
    std::shared_ptr< std::unordered_set< std::shared_ptr< CCard > > > fWildCards;

    std::vector< std::pair< EHand, std::weak_ptr< CPlayer > > > fGames;
    std::vector< uint64_t > fWinsByHand;
    std::vector< uint64_t > fHandCount;
    std::vector< uint64_t > fWinsByPlayer;

    TCardDeal fNumCardsToDeal{ { 5 }, {} }; // first vector is player deals (first) then last is community, default is 5 card 
};

#endif 
