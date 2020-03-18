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

#ifndef _PLAYER_H
#define _PLAYER_H

#include <QString>
#include <memory>
#include <vector>

class CHand;
class CCard;
enum class EHand;
class CPlayer 
{
public:
    CPlayer( const QString & playerName );
    virtual ~CPlayer();

public:
    void setNextPlayer( std::shared_ptr< CPlayer > nextPlayer ){ fNextPlayer = nextPlayer; }
    std::weak_ptr< CPlayer > nextPlayer() const{ return fNextPlayer; }

    void setPrevPlayer( std::shared_ptr< CPlayer > prevPlayer ) { fPrevPlayer = prevPlayer; }
    std::weak_ptr< CPlayer > prevPlayer() const { return fPrevPlayer; }

    void setDealer( bool isDealer ){ fDealer = isDealer; }
    bool isDealer() const { return fDealer; }

    void setWinner( bool isWinner ) { fWinner = isWinner; }
    bool isWinner() const { return fWinner; }

    QString toString() const;
    bool operator<( const CPlayer & rhs ) const;

    QString name() const{ return fName; }

    void setPlayerID( size_t id ) { fPlayerID = id; }
    size_t playerID() const { return fPlayerID; }

    EHand hand() const;
public:
    void addCard( std::shared_ptr< CCard > & card );
    void clearCards();
private:
    QString fName;
    size_t fPlayerID{ 0 };
    std::shared_ptr< CHand > fHand;
    std::weak_ptr< CPlayer > fNextPlayer;
    std::weak_ptr< CPlayer > fPrevPlayer;
    bool fDealer{ false };
    bool fWinner{ false };
};

#endif // _ALCULATOR_H
