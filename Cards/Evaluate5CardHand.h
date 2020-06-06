#ifndef __EVALUATE5CARDHAND_H
#define __EVALUATE5CARDHAND_H

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

#include "CardInfo.h"
#include <memory>
#include <unordered_map>

struct SPlayInfo;
enum class EHand;

namespace NHandUtils
{
    class C5CardInfo : public CCardInfo
    {
    public:
        C5CardInfo();
        C5CardInfo( const THand& cards );
        C5CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4, ECard c5, ESuit s5 );

        virtual bool allHandsComputed() const override { return sCardInfoData.fAllHandsComputed; }
        virtual void setAllHandsComputed( bool computed ) override { sCardInfoData.fAllHandsComputed = computed; }
        virtual size_t getNumCards() const override { return 5; }

        static uint32_t evaluateCardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo );
        static EHand rankToCardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo );

    public:
        void initMaps();
        static SCardInfoData sCardInfoData;
    };
}

#endif