#ifndef __EVALUATE2CARDHAND_H
#define __EVALUATE2CARDHAND_H

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
enum class EHand;
struct SPlayInfo;

namespace NHandUtils
{
    class C2CardInfo : public CCardInfo
    {
    public:
        C2CardInfo();
        C2CardInfo( const THand & cards );
        C2CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2 );

        virtual bool allHandsComputed() const override { return sAllHandsComputed; }
        virtual void setAllHandsComputed( bool computed ) override{ sAllHandsComputed = computed; }
        virtual size_t getNumCards() const override{ return 2; }

        static uint32_t evaluateCardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo );
        static EHand rankToCardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo );

    private:
        struct SCardMaps
        {
            std::map< THand, uint32_t > fFlushesAndStraightsDontCountLowBall;
            std::map< THand, uint32_t > fFlushesAndStraightsCountLowBall;
            std::map< THand, uint32_t > fFlushesAndStraightsDontCount;
            std::map< THand, uint32_t > fFlushesAndStraightsCount;
        };

        static SCardMaps sCardMaps;

        struct SUniqueVectors
        {
            std::vector< uint32_t > fStraitsAndFlushesCount;
            std::vector< uint32_t > fStraitsAndFlushesDontCount;
        };
        static SUniqueVectors sUniqueVectors;

        struct SProductMaps
        {
            std::unordered_map< int64_t, int16_t > fStraitsAndFlushesCount;
            std::unordered_map< int64_t, int16_t > fStraitsAndFlushesDontCount;
        };
        static SProductMaps sProductMaps;

        static std::vector< uint32_t > sFlushes;
        static bool sAllHandsComputed;
    };
}

#endif