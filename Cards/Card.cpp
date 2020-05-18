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

#include "Card.h"
#include "HandUtils.h"

QString toString( ESuit suit, bool verbose )
{
    switch ( suit )
    {
        case ESuit::eHearts: return verbose ? "Hearts" : "H";
        case ESuit::eSpades: return verbose ? "Spades" : "S";
        case ESuit::eDiamonds: return verbose ? "Diamonds" : "D";
        case ESuit::eClubs: return verbose ? "Clubs" : "C";
        default:
            return "Unknown";
    }
}

QString asEnum( ESuit suit )
{
    switch ( suit )
    {
        case ESuit::eHearts: return "ESuit::eHearts";
        case ESuit::eSpades: return "ESuit::eSpades";
        case ESuit::eDiamonds: return "ESuit::eDiamonds";
        case ESuit::eClubs: return "ESuit::eClubs";
        default:
            return "ESuit::eUNKNOWN";
    }
}



bool fromString( ESuit& suit, const QString& suitName )
{
    suit = ESuit::eUNKNOWN;
    for ( auto&& ii : ESuit() )
    {
        if ( ( suitName.toLower() == toString( ii, false ).toLower() )
             || ( suitName.toLower() == toString( ii, true ).toLower() ) )
        {
            suit = ii;
            return true;
        }
    }
    return false;
} 


QString toString( ECard card, bool verbose )
{
    switch ( card )
    {
        case ECard::eAce: return verbose ? "Ace" : "A";
        case ECard::eDeuce: return verbose ? "Duece" : "2";
        case ECard::eTrey: return verbose ? "Trey" : "3";
        case ECard::eFour: return verbose ? "Four" : "4";
        case ECard::eFive: return verbose ? "Five" : "5";
        case ECard::eSix: return verbose ? "Six" : "6";
        case ECard::eSeven: return verbose ? "Seven" : "7";
        case ECard::eEight: return verbose ? "Eight" : "8";
        case ECard::eNine: return verbose ? "Nine" : "9";
        case ECard::eTen: return verbose ? "Ten" : "T";
        case ECard::eJack: return verbose ? "Jack" : "J";
        case ECard::eQueen: return verbose ? "Queen" : "Q";
        case ECard::eKing: return verbose ? "King" : "K";
        default:
            return "Unknown";
    }
}

QString asEnum( ECard card )
{
    switch ( card )
    {
        case ECard::eAce: return "ECard::eAce";
        case ECard::eDeuce: return "ECard::eDeuce";
        case ECard::eTrey: return "ECard::eTrey";
        case ECard::eFour: return "ECard::eFour";
        case ECard::eFive: return "ECard::eFive";
        case ECard::eSix: return "ECard::eSix";
        case ECard::eSeven: return "ECard::eSeven";
        case ECard::eEight: return "ECard::eEight";
        case ECard::eNine: return "ECard::eNine";
        case ECard::eTen: return "ECard::eTen";
        case ECard::eJack: return "ECard::eJack";
        case ECard::eQueen: return "ECard::eQueen";
        case ECard::eKing: return "ECard::eKing";
        default:
            return "ECard::eUNKNOWN";
    }
}


bool fromString( ECard & card, const QString & cardName )
{
    card = ECard::eUNKNOWN;
    for( auto && ii : ECard() )
    {
        if (   ( cardName.toLower() == toString( ii, false ).toLower() )
            || ( cardName.toLower() == toString( ii, true ).toLower() ) )
        {
            card = ii;
            return true;
        }
    }
    return false;
}


uint8_t toPrimeValue( ECard card )
{
    switch ( card )
    {
        case ECard::eDeuce: return 2;
        case ECard::eTrey: return 3;
        case ECard::eFour: return 5;
        case ECard::eFive: return 7;
        case ECard::eSix: return 11;
        case ECard::eSeven: return 13;
        case ECard::eEight: return 17;
        case ECard::eNine: return 19;
        case ECard::eTen: return 23;
        case ECard::eJack: return 29;
        case ECard::eQueen: return 31;
        case ECard::eKing: return 37;
        case ECard::eAce: return 41;
        default:
            return 0;
    }
}

ECard fromBitValue( uint16_t value )
{
    switch ( value )
    {
        case 0b00000000000001: return ECard::eDeuce;
        case 0b00000000000010: return ECard::eTrey;
        case 0b00000000000100: return ECard::eFour;
        case 0b00000000001000: return ECard::eFive;
        case 0b00000000010000: return ECard::eSix;
        case 0b00000000100000: return ECard::eSeven;
        case 0b00000001000000: return ECard::eEight;
        case 0b00000010000000: return ECard::eNine;
        case 0b00000100000000: return ECard::eTen;
        case 0b00001000000000: return ECard::eJack;
        case 0b00010000000000: return ECard::eQueen;
        case 0b00100000000000: return ECard::eKing;
        case 0b01000000000000: return ECard::eAce;
        default:
            return ECard::eUNKNOWN;
    }
}

uint16_t toBitValue( ECard card )
{
    switch ( card )
    {
        case ECard::eDeuce: return 0b00000000000001;
        case ECard::eTrey:  return 0b00000000000010;
        case ECard::eFour:  return 0b00000000000100;
        case ECard::eFive:  return 0b00000000001000;
        case ECard::eSix:   return 0b00000000010000;
        case ECard::eSeven: return 0b00000000100000;
        case ECard::eEight: return 0b00000001000000;
        case ECard::eNine:  return 0b00000010000000;
        case ECard::eTen:   return 0b00000100000000;
        case ECard::eJack:  return 0b00001000000000;
        case ECard::eQueen: return 0b00010000000000;
        case ECard::eKing:  return 0b00100000000000;
        case ECard::eAce:   return 0b01000000000000;
        default:
            return 0b00000000000000;
    }
}

uint8_t toBitValue( ESuit suit )
{
    return static_cast< uint8_t >( suit );
}

std::ostream & operator<<( std::ostream& oss, ESuit suit )
{
    oss << toString( suit, false ).toStdString();
    return oss;
}

std::ostream & operator<<( std::ostream& oss, ECard suit )
{
    oss << toString( suit, false ).toStdString();
    return oss;
}

std::ostream& operator<<( std::ostream& oss, const CCard& card )
{
    oss << qPrintable( card.toString( false, false ) );
    return oss;
}

std::list< std::shared_ptr< CCard > > getAllCardsList()
{
    std::list< std::shared_ptr< CCard > > retVal;
    for ( auto&& card : ECard() )
    {
        for ( auto&& suit : ESuit() )
        {
            retVal.push_back( std::make_shared< CCard >( card, suit ) );
        }
    }
    return retVal;
}

std::vector< std::shared_ptr< CCard > > getAllCardsVector()
{
    auto tmp = getAllCardsList();
    auto retVal = std::vector< std::shared_ptr< CCard > >( tmp.begin(), tmp.end() );
    return retVal;
}

uint8_t toRankValue( ECard card )
{
    return static_cast<uint8_t>( card );
}

CCard::CCard( ECard card, ESuit suit) :
    fCard( card ),
    fSuit( suit )
{
    fBitValue = NHandUtils::computeBitValue( fCard, fSuit );
}

CCard::~CCard()
{

}

std::vector< std::shared_ptr< CCard > > CCard::allCards()
{
    static std::vector< std::shared_ptr< CCard > > retVal;
    if ( retVal.empty() )
    {
        for ( const auto&& currSuit : ESuit() )
        {
            for ( const auto&& currCard : ECard() )
            {
                auto card = std::make_shared< CCard >( currCard, currSuit );
                retVal.push_back( card );
            }
        }
    }
    return retVal;
}

std::list< std::shared_ptr< CCard > > CCard::allCardsList()
{
    static std::list< std::shared_ptr< CCard > > retVal;
    if ( retVal.empty() )
    {
        auto allCards = CCard::allCards();
        retVal = { allCards.begin(), allCards.end() };
    }
    return retVal;
}

QString CCard::toString( bool verbose, bool includeBitValue ) const
{
    auto retVal = ::toString( fCard, verbose ) + ::toString( fSuit, verbose );
    if ( includeBitValue )
        retVal += + " - " + QString::fromStdString( fBitValue.to_string() );
    return retVal;
}

bool CCard::operator<( const CCard& rhs ) const
{
    if ( fSuit == rhs.fSuit )
        return fCard < rhs.fCard;
    return fSuit < rhs.fSuit;
}

bool CCard::operator>( const CCard& rhs ) const
{
    if ( fSuit == rhs.fSuit )
        return fCard > rhs.fCard;
    return fSuit > rhs.fSuit;
}

bool CCard::operator==( const CCard& rhs ) const
{
    return ( fSuit == rhs.fSuit ) && ( fCard == rhs.fCard );
}
