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

#ifndef _HAND_H
#define _HAND_H

#include "Card.h"

#include <QString>
#include <memory>
#include <vector>
#include <map>

class CCard;
enum class ECard;
enum class EHand
{
    eStraightFlush,
    eFourOfAKind,
    eFullHouse,
    eFlush,
    eStraight,
    eThreeOfAKind,
    eTwoPair,
    ePair,
    eHighCard
};
DECLARE_ENUM_FUNCS_ARITHMETIC( EHand );
DECLARE_ENUM_ITERATORS( EHand, EHand::eStraightFlush, EHand::eHighCard );
QString toString( EHand hand, bool format );


class CHand
{
public:
    CHand();
    virtual ~CHand();
public:
    QString toString() const;

    QString determineHandName() const;
    std::tuple< EHand, std::vector< ECard >, std::vector< ECard > >  determineHand() const; // hand, mycard, kicker cards

    bool isFlush() const;
    bool isStraight() const;

    void addCard( std::shared_ptr< CCard > & card );
    void clearCards();

    bool operator<( const CHand & rhs ) const;
private:

    uint16_t get5CardValue() const;
    QString maxCardName() const;
    ECard getMaxCard() const;
    TCardBitType cardsAndValue() const;
    TCardBitType cardsOrValue() const;

    std::vector< std::shared_ptr< CCard > > fCards;
};

#endif // _ALCULATOR_H
