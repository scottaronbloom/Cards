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

#include "Player.h"
#include "Hand.h"
#include "SABUtils/utils.h"

#include <QStringList>

CPlayer::CPlayer( const std::shared_ptr< std::unordered_set< std::shared_ptr< CCard > > > & wildCards, const QString& playerName ) :
    fName( playerName ),
    fHand( std::make_shared< CHand >() )
{
    fHand->setWildCards( wildCards );
}

CPlayer::~CPlayer()
{

}


QString CPlayer::toString( bool details ) const
{
    QString retVal = "=================================\n"
        "Player: " + fName + "\n"
        "=================================\n";

    retVal += QString( "Is Dealer: %1\n" ).arg( fDealer ? "Yes" : "No" );
    retVal += QString( "Is Winner: %1\n" ).arg( fWinner ? "Yes" : "No" );
    if ( details )
    {
        retVal += QString( "Prev Player: %1\n" ).arg( prevPlayer().expired() ? "<NONE>" : prevPlayer().lock()->name() );
        retVal += QString( "Next Player: %1\n" ).arg( nextPlayer().expired() ? "<NONE>" : nextPlayer().lock()->name() );
    }
    retVal += fHand->toString() + "\n";
    return retVal;
}

bool CPlayer::operator>( const CPlayer& rhs ) const
{
    return fHand->operator >( *rhs.fHand );
}

bool CPlayer::operator<( const CPlayer& rhs ) const
{
    return fHand->operator<( *rhs.fHand );
}

bool CPlayer::operator==( const CPlayer& rhs ) const
{
    return fHand->operator==( *rhs.fHand );
}

bool CPlayer::setName( const QString& name )
{
    if ( fName != name ) 
    { 
        fName = name; 
        return true; 
    } 
    return false;
}

EHand CPlayer::hand() const
{
    return fHand->getHand();
}


std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > CPlayer::determineHand() const// hand, mycard, kicker cards
{
    return fHand->determineHand();
}

void CPlayer::addCard( std::shared_ptr< CCard >& card )
{
    fHand->addCard( card );
}

void CPlayer::setCards( const std::vector< std::shared_ptr< CCard > >& cards )
{
    fHand->setCards( cards );
}

void CPlayer::clearCards()
{
    fHand->clearCards();
}
