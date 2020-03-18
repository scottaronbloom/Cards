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

QString toString( ECard card, bool verbose )
{
    switch ( card )
    {
        case ECard::eAce: return verbose ? "Ace" : "A";
        case ECard::eTwo: return verbose ? "Two" : "2";
        case ECard::eThree: return verbose ? "Three" : "3";
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

uint8_t toPrimeValue( ECard card )
{
    switch ( card )
    {
        case ECard::eTwo: return 2;
        case ECard::eThree: return 3;
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
        case 0b00000000000001: return ECard::eTwo;
        case 0b00000000000010: return ECard::eThree;
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
        case ECard::eTwo:   return 0b00000000000001;
        case ECard::eThree: return 0b00000000000010;
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

uint8_t toRankValue( ECard card )
{
    return static_cast<uint8_t>( card );
}

CCard::CCard( ECard card, ESuit suit) :
    fCard( card ),
    fSuit( suit )
{
    computeBitValue();
}

void CCard::computeBitValue()
{
    uint32_t tmp =
        ( toBitValue( fCard ) << 16 )
        | ( ( ( toBitValue( fSuit ) << 4 ) | toRankValue( fCard ) ) << 8 )
        | toPrimeValue( fCard );

    fBitValue = tmp;
}

CCard::~CCard()
{

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
