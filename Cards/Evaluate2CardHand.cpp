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

#include "Evaluate2CardHand.h"
#include "Hand.h"
#include "PlayInfo.h"
#include "HandUtils.h"
#include "SABUtils/utils.h"

#include <map>
#include <set>
#include <optional>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>

namespace NHandUtils
{
    bool C2CardInfo::sAllHandsComputed{ false };

    C2CardInfo::C2CardInfo() :
        C2CardInfo( THand( { TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) } ) )
    {

    }

    C2CardInfo::C2CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2 ) :
        C2CardInfo( THand( { TCard( c1, s1 ), TCard( c2, s2 ) } ) )
    {

    }

    C2CardInfo::C2CardInfo( const THand& cards )
    {
        fHandOrder = { EHand::eStraightFlush, EHand::ePair, EHand::eStraight, EHand::eFlush, EHand::eHighCard };

        assert( cards.size() == 2 );
        setOrigCards( cards );
           
        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
        fStraightType = NHandUtils::isStraight( fOrigCards );
    }
}
