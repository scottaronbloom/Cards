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
#include "Card.h"
#include "SABUtils/utils.h"
#include <unordered_set>
#include <QStringList>

CHand::CHand()
{

}

CHand::CHand( const std::vector< std::shared_ptr< CCard > >& cards )
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

bool CHand::operator>( const CHand& rhs ) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank > rhsRank;
}

bool CHand::operator<( const CHand& rhs ) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank > rhsRank;  // lower the rank, the better the hand
}

bool CHand::operator==( const CHand& rhs ) const
{
    auto rank = evaluateHand();
    auto rhsRank = rhs.evaluateHand();
    return rank == rhsRank;
}

QString CHand::handCards() const
{
    QString retVal;
    bool first = true;
    for ( auto&& ii : fCards )
    {
        if ( !first )
            retVal += " ";
        retVal += ii->toString( false, false );
    }
    return retVal;
}

QString CHand::toString( bool details ) const
{
    QString retVal;
    if ( details )
    {
        retVal += "Cards:\n";
        for ( auto&& ii : fCards )
        {
            retVal += ii->toString( false, details ) + "\n";
        }
    }
    retVal += "HAND: " + determineHandName( details );
    return retVal;
}

QString CHand::determineHandName( bool details ) const
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

enum EStraightType
{
    eAce   = 0b01111100000000, // AKQJT
    eKing  = 0b00111110000000, // KQJT9
    eQueen = 0b00011111000000, // QJT98
    eJack  = 0b00001111100000, // JT987
    eTen   = 0b00000111110000, // T9876
    eNine  = 0b00000011111000, // 98765
    eEight = 0b00000001111100, // 87654
    eSeven = 0b00000000111110, // 76543
    eSix   = 0b00000000011111, // 65432
    eWheel = 0b01000000001111
};


ECard CHand::getMaxCard() const
{
    if ( fHandData.fMaxCard.has_value() )
        return fHandData.fMaxCard.value();

    auto value = cardsOrValue() >> 16;
    if ( isStraight() && ( value == EStraightType::eWheel ) )
        fHandData.fMaxCard = ECard::eFive;
    else
    {
        auto pos = NUtils::findLargestIndexInBitSet( value );
        if ( pos.has_value() )
            fHandData.fMaxCard = static_cast<ECard>( pos.value() );
        else
            return ECard::eUNKNOWN;
    }
    return fHandData.fMaxCard.value();
}

ECard CHand::getMinCard() const
{
    if ( fHandData.fMinCard.has_value() )
        return fHandData.fMinCard.value();

    auto value = cardsOrValue() >> 16;
    if ( isStraight() && ( value == EStraightType::eWheel ) )
        fHandData.fMinCard = ECard::eFive;
    else
    {
        auto pos = NUtils::findSmallestIndexInBitSet( value );
        if ( pos.has_value() )
            fHandData.fMinCard = static_cast<ECard>( pos.value() );
        else
            return ECard::eUNKNOWN;
    }
    return fHandData.fMinCard.value();
}

uint16_t CHand::get5CardValue() const
{
    if ( fHandData.f5CardValue.has_value() )
        return fHandData.f5CardValue.value();

    auto retVal = get5CardValue( fCards );
    if ( retVal == -1 )
        return retVal;
    fHandData.f5CardValue = retVal;
    return retVal;
}

uint16_t CHand::get5CardValue( const std::vector< std::shared_ptr< CCard > > & cards )
{
    if ( cards.empty() )
        return -1;
    return static_cast<uint16_t>( ( cardsOrValue( cards ) >> 16 ).to_ulong() );
}

QString CHand::maxCardName() const
{
    auto maxCard = getMaxCard();
    return ::toString( maxCard, true );
}

bool CHand::isFlush( const std::vector< std::shared_ptr< CCard > > & cards )
{
    if ( cards.empty() )
        return false;

    auto value = cardsAndValue( cards );
    value &= 0x0F000;
    return value.to_ulong() != 0;
}

bool CHand::isFlush() const
{
    return isFlush( fCards );
}

bool CHand::isStraight() const
{
    static std::unordered_set< uint16_t > sStraights =
    {
        EStraightType::eAce, // AKQJT
        EStraightType::eKing, // KQJT9
        EStraightType::eQueen, // QJT98
        EStraightType::eJack, // JT987
        EStraightType::eTen, // T9876
        EStraightType::eNine, // 98765
        EStraightType::eEight, // 87654
        EStraightType::eSeven, // 76543
        EStraightType::eSix, // 65432
        EStraightType::eWheel  // 5432A
    };
    if ( fCards.empty() )
        return false;

    auto value = get5CardValue();
    return sStraights.find( value ) != sStraights.end();
}

EHand CHand::getHand() const
{
    if ( fHandData.fHand.has_value() )
        return std::get< 0 >( fHandData.fHand.value() );
    return computeHand();
}
// hand my cards kickers
std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > CHand::determineHand() const
{
    if ( fHandData.fHand.has_value() )
        return *fHandData.fHand;

    auto emptyHand = std::make_tuple( EHand::eNoCards, std::vector< ECard >(), std::vector< ECard >() );
    if ( fCards.empty() )
        return emptyHand;

    auto hand = computeHand();

    std::vector< ECard > cards;
    std::vector< ECard > kickers;
    switch( hand )
    {
        case EHand::eStraightFlush:
        case EHand::eFlush:
        case EHand::eStraight:
            cards = std::vector< ECard >( { getMaxCard() } );
            break;

        case EHand::eFourOfAKind:
        case EHand::eFullHouse:
        case EHand::eThreeOfAKind:
        case EHand::eTwoPair:
        case EHand::ePair:
        case EHand::eHighCard:
            {
                std::map< ECard, uint8_t > cardHits;
                for ( auto&& card : fCards )
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

    return *( fHandData.fHand = std::make_tuple( hand, cards, kickers ) );
}

EHand CHand::computeHand() const
{
    if ( fCards.empty() )
        return EHand::eNoCards;
    auto rank = evaluateHand();
    EHand hand;
    if ( rank > 6185 )
        hand = EHand::eHighCard;
    else if ( rank > 3325 )
        hand = EHand::ePair;
    else if ( rank > 2467 )
        hand = EHand::eTwoPair;
    else if ( rank > 1609 )
        hand = EHand::eThreeOfAKind;
    else if ( rank > 1599 )
        hand = EHand::eStraight;
    else if ( rank > 322 )
        hand = EHand::eFlush;
    else if ( rank > 166 )
        hand = EHand::eFullHouse;
    else if ( rank > 10 )
        hand = EHand::eFourOfAKind;
    else
        hand = EHand::eStraightFlush;
    return hand;
}

void CHand::addCard( std::shared_ptr< CCard >& card )
{
    Q_ASSERT( card );
    fCards.push_back( card );
}

void CHand::setCards( const std::vector< std::shared_ptr< CCard > >& cards )
{
    fCards = cards;
}

void CHand::clearCards()
{
    fCards.clear();
    fHandData.reset();
}

uint64_t CHand::computeHandProduct( const std::vector < std::shared_ptr< CCard > > & cards )
{
    if ( cards.empty() )
        return -1;

    uint64_t retVal = 1;
    TCardBitType value( std::numeric_limits< int64_t >::max() );
    for ( auto&& ii : cards )
        retVal *= ( ii->bitValue().to_ulong() & 0x00FF );

    return retVal;
}

uint64_t CHand::computeHandProduct() const
{
    if ( fHandData.fHandProduct.has_value() )
        return fHandData.fHandProduct.value();

    auto retVal = computeHandProduct( fCards );
    if ( retVal == -1 )
        return retVal;

    fHandData.fHandProduct = retVal;
    return fHandData.fHandProduct.value();
}

TCardBitType CHand::cardsAndValue( const std::vector< std::shared_ptr< CCard > >& cards )
{
    if ( cards.empty() )
        return TCardBitType();

    TCardBitType value( std::numeric_limits< int64_t >::max() );
    for ( auto&& ii : cards )
        value &= ii->bitValue();

    return value;
}

TCardBitType CHand::cardsAndValue() const
{
    if ( fHandData.fAndValue.has_value() )
        return fHandData.fAndValue.value();

    auto retVal = cardsAndValue( fCards );
    if ( retVal.size() == 0 )
        return retVal;

    fHandData.fAndValue = retVal;
    return fHandData.fAndValue.value();
}

TCardBitType CHand::cardsOrValue( const std::vector< std::shared_ptr< CCard > >& cards )
{
    if ( cards.empty() )
        return TCardBitType();


    TCardBitType value;
    for ( auto&& ii : cards )
        value |= ii->bitValue();
    return value;
}

TCardBitType CHand::cardsOrValue() const
{
    if ( fHandData.fOrValue.has_value() )
        return fHandData.fOrValue.value();

    auto retVal = cardsOrValue( fCards );
    if ( retVal.size() == 0 )
        return retVal;
    fHandData.fOrValue = retVal;
    return fHandData.fOrValue.value();
}
