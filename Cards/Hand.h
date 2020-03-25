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

#include "SABUtils/EnumUtils.h"

#include <QString>
#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <bitset>

using TCardBitType = std::bitset< 29 >;

class CCard;
enum class ECard;
enum class EHand
{
    eNoCards=-1,
    eHighCard=0,
    ePair=1,
    eTwoPair=2,
    eThreeOfAKind=3,
    eStraight=4,
    eFlush=5,
    eFullHouse=6,
    eFourOfAKind=7,
    eStraightFlush=8
};
DECLARE_ENUM_FUNCS_ARITHMETIC( EHand );
DECLARE_ENUM_ITERATORS( EHand, EHand::eHighCard, EHand::eStraightFlush );
QString toString( EHand hand, bool format );
std::ostream & operator<<( std::ostream& oss, EHand value );

class CHand
{
public:
    CHand();
    CHand( const std::vector< std::shared_ptr< CCard > >& cards );
    virtual ~CHand();
public:
    QString toString( bool details ) const;

    QString determineHandName( bool details ) const;
    std::tuple< EHand, std::vector< ECard >, std::vector< ECard > >  determineHand() const; // hand, mycard, kicker cards

    EHand computeHand() const;

    EHand getHand() const;

    bool isFlush() const;
    bool isStraight() const;

    void addCard( std::shared_ptr< CCard > & card );
    void setCards( const std::vector< std::shared_ptr< CCard > > & cards );
    void clearCards();

    bool operator>( const CHand& rhs ) const;

    bool operator<( const CHand & rhs ) const;
    bool operator==( const CHand& rhs ) const;
    QString handCards() const;
private:
    static uint32_t evaluateHand( const std::vector< std::shared_ptr< CCard > > & cards );
    static uint16_t get5CardValue( const std::vector< std::shared_ptr< CCard > >& cards );
    static bool isFlush( const std::vector< std::shared_ptr< CCard > >& cards );
    static uint64_t computeHandProduct( const std::vector< std::shared_ptr< CCard > >& cards );
    static TCardBitType cardsOrValue( const std::vector< std::shared_ptr< CCard > >& cards );
    static TCardBitType cardsAndValue( const std::vector< std::shared_ptr< CCard > >& cards );

    uint32_t evaluateHand() const;
   
    uint16_t get5CardValue() const;
    QString maxCardName() const;
    ECard getMaxCard() const;
    ECard getMinCard() const;
    TCardBitType cardsAndValue() const;
    TCardBitType cardsOrValue() const;
    uint64_t computeHandProduct() const;

    std::vector< std::shared_ptr< CCard > > fCards;
    mutable std::optional< std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > > fHand;
    mutable std::optional< uint16_t > f5CardValue;
    mutable std::optional< ECard > fMaxCard;
    mutable std::optional< ECard > fMinCard;
    mutable std::optional< TCardBitType > fAndValue;
    mutable std::optional< TCardBitType > fOrValue;
    mutable std::optional< uint64_t > fHandProduct;
    mutable std::optional< uint32_t > fHandRank;
};

#endif // _ALCULATOR_H
