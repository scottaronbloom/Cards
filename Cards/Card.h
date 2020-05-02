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

#ifndef _CARDS_H
#define _CARDS_H
#include "SABUtils/EnumUtils.h"
#include <bitset>
#include <vector>
#include <list>
#include <memory>
#include <QString>
class CCard;

enum class ESuit : uint8_t
{
    eUNKNOWN=0,
    eSpades   =0b00001,
    eHearts   =0b00010,
    eDiamonds =0b00100,
    eClubs    =0b01000
};
DECLARE_ENUM_FUNCS_ARITHMETIC_ONEHOT( ESuit );
DECLARE_ENUM_ITERATORS( ESuit, ESuit::eSpades, ESuit::eClubs );
QString toString( ESuit suit, bool verbose );
QString asEnum( ESuit suit );
bool fromString( ESuit& suit, const QString& suitName );
uint8_t toBitValue( ESuit suit );
std::ostream & operator<<( std::ostream & oss, ESuit suit );

enum class ECard
{
    eUNKNOWN=99,
    eDeuce=0,
    eTrey=1,
    eFour=2,
    eFive=3,
    eSix=4,
    eSeven=5,
    eEight=6,
    eNine=7,
    eTen=8,
    eJack=9,
    eQueen=10,
    eKing=11,
    eAce=12
};
DECLARE_ENUM_FUNCS_ARITHMETIC( ECard );
DECLARE_ENUM_ITERATORS( ECard, ECard::eDeuce, ECard::eAce );
QString toString( ECard card, bool verbose );
QString asEnum( ECard suit );
bool fromString( ECard & card, const QString& cardName );
uint8_t toPrimeValue( ECard card );
uint16_t toBitValue( ECard card );
ECard fromBitValue( uint16_t value );
uint8_t toRankValue( ECard card );
std::ostream & operator<<( std::ostream& oss, ECard card );

std::vector< std::shared_ptr< CCard > > getAllCards();
using TCardBitType = std::bitset< 29 >;
class CCard
{
public:
    CCard( ECard card, ESuit suit );
    virtual ~CCard();

    static std::vector< std::shared_ptr< CCard > > allCards();
    static std::list< std::shared_ptr< CCard > > allCardsList();
    QString toString( bool verbose, bool includeBitValue ) const;
    TCardBitType bitValue() const{ return fBitValue; }

    ESuit getSuit() const{ return fSuit; }
    ECard getCard() const { return fCard; }
public:
    bool operator<( const CCard & rhs ) const;
    bool operator>( const CCard& rhs ) const;
    bool operator==( const CCard& rhs ) const;

private:
    ECard fCard{ ECard::eUNKNOWN };
    ESuit fSuit{ ESuit::eUNKNOWN };

    TCardBitType fBitValue;
};

#endif 

