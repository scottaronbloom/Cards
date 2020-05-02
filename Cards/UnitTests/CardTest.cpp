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

#include "CardTest.h"
#include "Cards/Game.h"
#include "Cards/Card.h"

std::ostream& operator<<( std::ostream& os, const QString& data )
{
    return os << data.toStdString();
}

void PrintTo( const QString& data, std::ostream* os )
{
    *os << data.toStdString();
}

NHandTester::CHandTester::CHandTester()
{
    fGame = new CGame;
}

NHandTester::CHandTester::~CHandTester()
{
    delete fGame;
}

bool NHandTester::CHandTester::isStraight( std::vector< ECard > cards ) const
{
    if ( cards.empty() )
        return false;

    std::sort( cards.begin(), cards.end(), []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
    auto ii = cards.begin();
    auto prev = *ii;
    ii++;
    bool highCardAce = prev == ECard::eAce;

    bool isStraight = true;
    for( ; isStraight && ( ii != cards.end() ); ++ii )
    {
        if ( prev != ( (*ii) + 1 ) )
            isStraight = false;
        prev = **ii;
    }
    if ( isStraight )
        return true;

    if ( !highCardAce )
        return false;

    auto jj = cards.rbegin();
    jj++;
    if ( jj == cards.rend() )
        return false; // onecard cant be a straight

    auto expected = ECard::eDeuce;

    while( jj != cards.rend() )
    {
        if ( *jj != expected )
            return false;
        ++jj;
        expected++;
    }
    return true;
}
