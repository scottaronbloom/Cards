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

#include "Hand.h"
#include "HandUtils.h"
#include "HandImpl.h"
#include "Card.h"
#include "SABUtils/utils.h"
#include <unordered_set>
#include <QStringList>

CHand::CHand( const std::shared_ptr< SPlayInfo >& playInfo ) :
    fHandImpl( std::make_unique< CHandImpl >( playInfo ) )
{

}

CHand::CHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo ) :
    CHand( playInfo )
{
    setCards( cards );
}

CHand::~CHand()
{

}

QString toString( EHand hand, bool format )
{
    switch ( hand )
    {
        case EHand::eFiveOfAKind: return format ? "Five of a Kind '%1'" : "Five of a Kind";
        case EHand::eStraightFlush: return format ? "Straight Flush to the '%1'" : "Straight Flush";
        case EHand::eFourOfAKind: return format ? "Four of a Kind '%1' - '%2' kicker" : "Four of a Kind";
        case EHand::eFullHouse: return format ? "Full House '%1' over '%2'" : "Full House";
        case EHand::eFlush: return format ? "Flush to the '%1'" : "Flush";
        case EHand::eStraight: return format ? "Straight to the '%1'" : "Straight";
        case EHand::eThreeOfAKind: return format ? "Three of a Kind '%1' - '%2, %3' kicker" : "Three of a Kind";
        case EHand::eTwoPair: return format ? "Two Pair '%1 and %2' - '%3' kicker" : "Two Pair";
        case EHand::ePair: return format ? "Pair of '%1' - '%2, %3, %4' kicker" : "Pair";
        case EHand::eHighCard: return format ? "High Card '%1' : %2, %3, %4, %5 kickers" : "High Card";
        case EHand::eNoCards: return format ? "No Cards" : "No Cards";
    }
    return QString();
}

std::ostream & operator<<( std::ostream& oss, EHand value )
{
    oss << toString( value, false ).toStdString();
    return oss;
}

bool CHand::operator >( const CHand & rhs ) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank > rhsRank;
}

bool CHand::operator <( const CHand & rhs) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank > rhsRank;  // lower the rank, the better the hand
}

bool CHand::operator ==( const CHand & rhs ) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank == rhsRank;
}

const std::vector< std::shared_ptr< CCard > > & CHand::getCards() const
{
    return fHandImpl->getCards();
}

const std::optional< std::pair< uint32_t, std::unique_ptr< CHand > > > & CHand::bestHand() const
{
    return fHandImpl->bestHand();
}

bool CHand::hasCards() const
{
    return fHandImpl->hasCards();
}

void CHand::resetHandAnalysis()
{
    fHandImpl->resetHandAnalysis();
}

bool CHand::isFlush() const
{
    return fHandImpl->isFlush();
}

bool CHand::isStraight() const
{
    return fHandImpl->isStraight();
}

QString CHand::toString() const
{
    return fHandImpl->toString();
}

EHand CHand::getHand() const
{
    return fHandImpl->getHand();
}

// hand my cards kickers
std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > CHand::determineHand() const
{
    return fHandImpl->determineHand();
}

QString CHand::determineHandName( bool details ) const
{
    return fHandImpl->determineHandName( details );
}

void CHand::addWildCard( std::shared_ptr< CCard > card )
{
    fHandImpl->addWildCard( card );
}

EHand CHand::computeHand() const
{
    return fHandImpl->computeHand();
}

void CHand::addCard( std::shared_ptr< CCard > card )
{
    fHandImpl->addCard( card );
}

void CHand::setCards( const std::vector< std::shared_ptr< CCard > >& cards )
{
    fHandImpl->setCards( cards );
}

void CHand::clearCards()
{
    fHandImpl->clearCards();
}


uint32_t CHand::evaluateHand() const
{
    return fHandImpl->evaluateHand();
}

