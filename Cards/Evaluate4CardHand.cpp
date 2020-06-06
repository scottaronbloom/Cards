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

#include "Evaluate4CardHand.h"
#include "Hand.h"
#include "PlayInfo.h"
#include "HandUtils.h"

#include <map>
#include <set>
#include <optional>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include "SABUtils/utils.h"

namespace NHandUtils
{
    C4CardInfo::C4CardInfo() :
        C4CardInfo( THand( { TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) } ) )
    {
    }

    C4CardInfo::C4CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4 ) :
        C4CardInfo( THand( { TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ), TCard( c4, s4 )  } ) )
    {
    }

    C4CardInfo::C4CardInfo( const THand& cards )
    {
        initMaps();

        fHandOrder = 
        { 
            EHand::eFourOfAKind, 
            EHand::eStraightFlush, 
            EHand::eThreeOfAKind, 
            EHand::eStraight, 
            EHand::eTwoPair, 
            EHand::eFlush, 
            EHand::ePair, 
            EHand::eHighCard 
        };

        setOrigCards( cards );

        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
        fIsTwoPair = NHandUtils::isCount( fOrigCards, { 2, 2 } );
        fIsThreeOfAKind = NHandUtils::isCount( fOrigCards, 3 );
        fIsFourOfAKind = NHandUtils::isCount( fOrigCards, 4 );
        fStraightType = NHandUtils::isStraight( fOrigCards );
    }
}