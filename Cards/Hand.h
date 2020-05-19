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
#include <unordered_set>
#include <string>

enum class EHand
{
    eNoCards = -1,
    eHighCard = 0,
    ePair = 1,
    eTwoPair = 2,
    eThreeOfAKind = 3,
    eStraight = 4,
    eFlush = 5,
    eFullHouse = 6,
    eFourOfAKind = 7,
    eStraightFlush = 8,
    eFiveOfAKind = 9
};
DECLARE_ENUM_FUNCS_ARITHMETIC( EHand );
DECLARE_ENUM_ITERATORS( EHand, EHand::eHighCard, EHand::eFiveOfAKind );
QString toString( EHand hand, bool format );
std::string toCPPString( EHand hand );
std::ostream& operator<<( std::ostream& oss, EHand value );

enum class ECard;
class CCard;
class CHandImpl;
struct SPlayInfo;
class CHand
{
public:
    CHand() = delete;
    CHand( const std::shared_ptr< SPlayInfo > & playInfo );
    CHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo > & playInfo );
    virtual ~CHand();
public:
    QString toString() const;

    QString determineHandName( bool details ) const;
    std::tuple< EHand, std::vector< ECard >, std::vector< ECard > >  determineHand() const; // hand, mycard, kicker cards

    void setStraightsAndFlushesCount( bool straightsAndFlushesCount );
    void setLowHandWins( bool lowHandWins );
    void addWildCard( std::shared_ptr< CCard > card ); // needed for unit testing
    EHand computeHand() const;
    EHand getHand() const;

    void addCard( std::shared_ptr< CCard > card );
    void setCards( const std::vector< std::shared_ptr< CCard > > & cards );
    void clearCards();

    bool operator>( const CHand& rhs ) const;
    bool operator<( const CHand& rhs ) const;
    bool operator==( const CHand& rhs ) const;

    const std::vector< std::shared_ptr< CCard > > & getCards() const;

    const std::optional< std::pair< uint32_t, std::unique_ptr< CHand > > >& bestHand() const;
    bool hasCards() const;
    void resetHandAnalysis();

    bool isFlush() const; // used for unittesting
    bool isStraight() const; // used for unittesting
private:
    uint32_t evaluateHand() const;
   
    mutable std::unique_ptr< CHandImpl > fHandImpl;
};
inline std::ostream & operator<<( std::ostream & oss, const CHand & value )
{
    oss << value.toString().toStdString();
    return oss;
}
#endif 

