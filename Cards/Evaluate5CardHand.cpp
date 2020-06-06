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

#include "Evaluate5CardHand.h"
#include "HandUtils.h"
#include "Hand.h"
#include "PlayInfo.h"
#include <fstream>
#include <unordered_map>
#include "SABUtils/utils.h"

namespace NHandUtils
{
    C5CardInfo::C5CardInfo() :
        C5CardInfo( THand( { TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) } ) )
    {
    }

    C5CardInfo::C5CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4, ECard c5, ESuit s5 ) :
        C5CardInfo( THand( { TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ), TCard( c4, s4 ), TCard( c5, s5 )  }) )
    {
    }

    C5CardInfo::C5CardInfo( const THand& cards )
    {
        initMaps();

        fHandOrder =
        {
            //EHand::eFiveOfAKind,
            EHand::eStraightFlush,
            EHand::eFourOfAKind,
            EHand::eFullHouse,
            EHand::eFlush,
            EHand::eStraight,
            EHand::eThreeOfAKind,
            EHand::eTwoPair,
            EHand::ePair,
            EHand::eHighCard
        };

        setOrigCards( cards );

        fIsFiveOfAKind = NHandUtils::isCount( fOrigCards, 5 );
        fIsFourOfAKind = NHandUtils::isCount( fOrigCards, 4 );
        fIsFullHouse = NHandUtils::isCount( fOrigCards, { 3,2 } );
        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fStraightType = NHandUtils::isStraight( fOrigCards );
        fIsThreeOfAKind = NHandUtils::isCount( fOrigCards, 3 );
        fIsTwoPair = NHandUtils::isCount( fOrigCards, { 2, 2 } );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
    }
}
