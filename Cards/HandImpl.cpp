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
#include "SABUtils/utils.h"

void CHandImpl::reset()
{
    fHand.reset();
    f5CardValue.reset();
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
    fWildCards->clear();
    reset();
}

void CHandImpl::setCards( const std::vector< std::shared_ptr< CCard > >& cards )
{
    fCards = cards;
}

void CHandImpl::addCard( std::shared_ptr< CCard >& card )
{
    Q_ASSERT( card );
    fCards.push_back( card );
}

void CHandImpl::setWildCards( const std::shared_ptr< std::unordered_set< std::shared_ptr< CCard > > >& wildCards )
{
    fWildCards = wildCards;
    reset();
}

void CHandImpl::addWildCard( const std::shared_ptr< CCard >& card )
{
    if ( !fWildCards )
        fWildCards = std::make_shared< std::unordered_set< std::shared_ptr< CCard > > >();
    fWildCards->insert( card );
    reset();
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

QString CHandImpl::maxCardName() const
{
    auto maxCard = getMaxCard();
    return ::toString( maxCard, true );
}

uint16_t CHandImpl::get5CardValue() const
{
    if ( f5CardValue.has_value() )
        return f5CardValue.value();

    auto retVal = NHandUtils::get5CardValue( fCards );
    if ( retVal == -1 )
        return retVal;
    f5CardValue = retVal;
    return retVal;
}

ECard CHandImpl::getMaxCard() const
{
    if ( fMaxCard.has_value() )
        return fMaxCard.value();

    fMaxCard = NHandUtils::getMaxCard( fCards );
    return fMaxCard.value();
}

ECard CHandImpl::getMinCard() const
{
    if ( fMinCard.has_value() )
        return fMinCard.value();

    fMinCard = NHandUtils::getMinCard( fCards );
    return fMinCard.value();
}

TCardBitType CHandImpl::cardsAndValue() const
{
    if ( fAndValue.has_value() )
        return fAndValue.value();

    auto retVal = NHandUtils::cardsAndValue( fCards );
    if ( retVal.size() == 0 )
        return retVal;

    fAndValue = retVal;
    return fAndValue.value();
}

TCardBitType CHandImpl::cardsOrValue() const
{
    if ( fOrValue.has_value() )
        return fOrValue.value();

    auto retVal = NHandUtils::cardsOrValue( fCards );
    if ( retVal.size() == 0 )
        return retVal;
    fOrValue = retVal;
    return fOrValue.value();
}

uint64_t CHandImpl::computeHandProduct() const
{
    if ( fHandProduct.has_value() )
        return fHandProduct.value();

    auto retVal = NHandUtils::computeHandProduct( fCards );
    if ( retVal == -1 )
        return retVal;

    fHandProduct = retVal;
    return fHandProduct.value();
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
    return NHandUtils::rankToHand( rank, fWildCards && !fWildCards->empty() );
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
        fBestHand = NHandUtils::evaluateHand( fCards, fWildCards );
        if ( !fBestHand->second )
        {
            fBestHand.reset();
            return -1;
        }
    }

    return fBestHand.value().first;
}
