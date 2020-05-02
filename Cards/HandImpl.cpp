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

#include "HandImpl.h"
#include "Card.h"
#include "Hand.h"
#include "PlayInfo.h"
#include "SABUtils/utils.h"

CHandImpl::CHandImpl( const std::shared_ptr< SPlayInfo >& playInfo ) :
    fPlayInfo( playInfo )
{
    if ( !fPlayInfo )
        fPlayInfo = std::make_shared< SPlayInfo >();
}

void CHandImpl::resetHandAnalysis()
{
    fHand.reset();
    fCardsValue.reset();
    fMaxCard.reset();
    fMinCard.reset();
    fAndValue.reset();
    fOrValue.reset();
    fHandProduct.reset();
    fBestHand.reset();
}

void CHandImpl::clearCards()
{
    fCards.clear();
    resetHandAnalysis();
}

void CHandImpl::setCards( const std::vector< std::shared_ptr< CCard > >& cards )
{
    fCards = cards;
}

void CHandImpl::addCard( std::shared_ptr< CCard > card )
{
    Q_ASSERT( card );
    fCards.push_back( card );
}

void CHandImpl::addWildCard( std::shared_ptr< CCard > card )
{
    fPlayInfo->fWildCards.insert( card );
    resetHandAnalysis();
}

QString CHandImpl::toString() const
{
    QString retVal = "Cards: ";
    bool first = true;
    for ( auto&& ii : fCards )
    {
        if ( !first )
            retVal += " ";
        first = false;
        retVal += ii->toString( false, false );
    }

    if ( fBestHand.has_value() )
    {
        retVal += " - Best Hand: " + fBestHand.value().second->toString() + " - " + determineHandName( true );
    }
    return retVal;
}

QString CHandImpl::determineHandName( bool details ) const
{
    if ( details )
    {
        auto hand = determineHand();
        QString retVal = ::toString( std::get< 0 >( hand ), true );
        for ( auto&& ii : std::get< 1 >( hand ) )
            retVal = retVal.arg( ::toString( ii, true ) );
        for ( auto&& ii : std::get< 2 >( hand ) )
            retVal = retVal.arg( ::toString( ii, true ) );
        return retVal;
    }
    else
        return ::toString( computeHand(), false );
}

// hand my cards kickers
std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > CHandImpl::determineHand() const
{
    if ( fHand.has_value() )
        return *fHand;

    auto emptyHand = std::make_tuple( EHand::eNoCards, std::vector< ECard >(), std::vector< ECard >() );
    if ( fCards.empty() )
        return emptyHand;

    auto hand = computeHand();
    if ( !fBestHand.has_value() )
        return emptyHand;

    std::vector< ECard > cards;
    std::vector< ECard > kickers;
    switch( hand )
    {
        case EHand::eFiveOfAKind:
        case EHand::eStraightFlush:
        case EHand::eFlush:
        case EHand::eStraight:
            {
                cards = std::vector< ECard >( { NHandUtils::getMaxCard( fBestHand.value().second->getCards() ) } );
                for( auto && ii : fBestHand.value().second->getCards() )
                {
                    if ( ii->getCard() == cards[ 0 ] )
                        continue;
                    kickers.push_back( ii->getCard() );
                }
            }
            break;

        case EHand::eFourOfAKind:
        case EHand::eFullHouse:
        case EHand::eThreeOfAKind:
        case EHand::eTwoPair:
        case EHand::ePair:
        case EHand::eHighCard:
            {
                std::map< ECard, uint8_t > cardHits;
                for ( auto && card : fBestHand.value().second->getCards() )
                    cardHits[ card->getCard() ]++;

                for ( auto&& ii : cardHits )
                {
                    if ( ( ( hand == EHand::eFullHouse ) && ( ii.second == 3 ) ) || ( ( hand != EHand::eFullHouse ) && ( ( ii.second > 1 ) || ( hand == EHand::eHighCard ) ) ) )
                        cards.push_back( ii.first );
                    else 
                        kickers.push_back( ii.first );
                }
            }
            break;
        default:
            return emptyHand;
    }

    std::sort( kickers.begin(), kickers.end(), []( ECard lhs, ECard rhs ){ return lhs > rhs; } );
    std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
    if ( hand == EHand::eHighCard )
    {
        kickers = cards;
        kickers.erase( kickers.begin(), ++kickers.begin() );
        cards.erase( ++cards.begin(), cards.end() );
    }

    return *( fHand = std::make_tuple( hand, cards, kickers ) );
}

EHand CHandImpl::computeHand() const
{
    if ( fCards.empty() )
        return EHand::eNoCards;
    auto rank = evaluateHand();
    return NHandUtils::rankToHand( rank, fCards.size(), fPlayInfo );
}

EHand CHandImpl::getHand() const
{
    if ( fHand.has_value() )
        return std::get< 0 >( fHand.value() );
    return computeHand();
}

uint32_t CHandImpl::evaluateHand() const
{
    if ( !fBestHand.has_value() )
    {
        fBestHand = NHandUtils::evaluateHand( fCards, fPlayInfo );
        if ( !fBestHand->second )
        {
            fBestHand.reset();
            return -1;
        }
    }

    return fBestHand.value().first;
}

bool CHandImpl::isFlush() const
{
    auto hand = determineHand();
    return ( std::get< 0 >( hand ) == EHand::eFlush ) || ( std::get< 0 >( hand ) == EHand::eStraightFlush );
}

bool CHandImpl::isStraight() const
{
    auto hand = determineHand();
    return std::get< 0 >( hand ) == EHand::eStraight || ( std::get< 0 >( hand ) == EHand::eStraightFlush );
}
