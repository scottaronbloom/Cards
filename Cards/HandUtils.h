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

#ifndef _HANDUTILS_H
#define _HANDUTILS_H
#include <memory>
#include <bitset>
#include <unordered_set>
#include <vector>

class CHand;
class CCard;
struct SPlayInfo;
enum class ECard;
enum class EHand;
enum class ESuit : uint8_t;

using TCard = std::pair< ECard, ESuit >;

namespace std
{
    template <>
    struct hash< std::pair< std::pair< ESuit, ECard >, std::pair< ESuit, ECard > > >
    {
        std::size_t operator()( const std::pair< std::pair< ESuit, ECard >, std::pair< ESuit, ECard > >& k ) const
        {
            auto v1 = std::hash<uint8_t>()( static_cast<uint8_t>( k.first.first ) );
            auto v2 = std::hash<int>()( static_cast<uint8_t>( k.first.second ) );
            auto v3 = std::hash<uint8_t>()( static_cast<uint8_t>( k.second.first ) );
            auto v4 = std::hash<int>()( static_cast<uint8_t>( k.second.second ) );

            auto retVal = ( ( ( ( ( v1 << 1 ) ^ ( v2 << 1 ) ) >> 1 ) ^ ( v3 << 1 ) ) >> 1 ) ^ ( v4 << 1 );
            return retVal;
        }
    };

    template <>
    struct hash< TCard >
    {
        std::size_t operator()( const TCard& k ) const
        {
            auto h1 = std::hash< uint64_t >{}( static_cast<uint64_t>( k.first ) );
            auto h2 = std::hash< uint64_t >{}( static_cast<uint64_t>( k.second ) );
            return h1 ^ ( h2 << 1 );
        }
    };
}

namespace NHandUtils
{
    using TCardBitType = std::bitset< 29 >;

    std::pair< uint32_t, std::unique_ptr< CHand > > findBest( const std::vector< std::shared_ptr< CCard > >& cards, int numCards, const std::shared_ptr< SPlayInfo > & playInfo );
    std::pair< uint32_t, std::unique_ptr< CHand > > findBest( const std::vector< std::vector< std::shared_ptr< CCard > > >& allHands, const std::shared_ptr< SPlayInfo >& playInfo );
    std::pair< uint32_t, std::unique_ptr< CHand > > evaluateHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo );

    bool isFlush( const std::vector< std::shared_ptr< CCard > >& cards );
    bool isStraight( const std::vector< std::shared_ptr< CCard > >& cards );

    uint64_t computeHandProduct( const std::vector< std::shared_ptr< CCard > >& cards );
    TCardBitType cardsOrValue( const std::vector< std::shared_ptr< CCard > >& cards );
    TCardBitType cardsAndValue( const std::vector< std::shared_ptr< CCard > >& cards );
    uint16_t getCardsValue( const std::vector< std::shared_ptr< CCard > >& cards );

    uint64_t computeHandProduct( const std::vector< TCardBitType > & cards );
    TCardBitType cardsOrValue( const std::vector< TCardBitType > & cards );
    TCardBitType cardsAndValue( const std::vector< TCardBitType > & cards );
    uint16_t getCardsValue( const std::vector< TCardBitType > & cards );

    ECard getMaxCard( const std::vector< std::shared_ptr< CCard > > & cards );
    ECard getMinCard( const std::vector< std::shared_ptr< CCard > >& cards );

    EHand rankToHand( uint32_t rank, size_t numCards, const std::shared_ptr< SPlayInfo > & playInfo );

    TCardBitType computeBitValue( ECard card, ESuit suit );
    inline bool isFlush( const TCard & card1, const TCard& card2 )
    {
        return card1.second == card2.second;
    }
    uint32_t getCardRank( ECard card );
}
#endif 

