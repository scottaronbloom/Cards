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

#include "HandUtils.h"
#include "Hand.h"
#include "Card.h"
#include "Evaluate2CardHand.h"
#include "Evaluate3CardHand.h"
#include "Evaluate4CardHand.h"
#include "Evaluate5CardHand.h"
#include "PlayInfo.h"

#include "SABUtils/utils.h"
#include <iostream>
#include <set>
#include <unordered_map>

namespace NHandUtils
{
    uint32_t evaluateHandInternal( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo > & playInfo );

    std::pair< uint32_t, std::unique_ptr< CHand > > findBest( const std::vector< std::vector< std::shared_ptr< CCard > > >& allHands, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        auto best = std::make_pair( std::numeric_limits< uint32_t >::max(), std::unique_ptr< CHand >() );
        for ( size_t ii = 0; ii < allHands.size(); ++ii )
        {
            auto&& currHand = allHands[ ii ];
            if ( currHand.size() > 5 )
            {
                auto bestFive = findBest( currHand, 5, playInfo );
                if ( bestFive.first < best.first )
                {
                    best.first = bestFive.first;
                    best.second.swap( bestFive.second );
                }
            }
            else
            {
                auto currHandValue = evaluateHandInternal( currHand, playInfo );
                if ( currHandValue < best.first )
                {
                    best.first = currHandValue;
                    best.second = std::make_unique< CHand >( currHand, nullptr );
                }
            }
        }
        return best;
    }

    std::pair< uint32_t, std::unique_ptr< CHand > > findBest( const std::vector< std::shared_ptr< CCard > >& cards, int numCards, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        auto allCombinations = NUtils::allCombinations( cards, numCards );
        return findBest( allCombinations, playInfo );
    }

    static std::unordered_map< ECard, uint32_t > sFiveOfAKindMap =
    {
         { ECard::eAce  ,  0 }
        ,{ ECard::eKing ,  1 }
        ,{ ECard::eQueen,  2 }
        ,{ ECard::eJack ,  3 }
        ,{ ECard::eTen  ,  4 }
        ,{ ECard::eNine ,  5 }
        ,{ ECard::eEight,  6 }
        ,{ ECard::eSeven,  7 }
        ,{ECard::eSix   ,  8 }
        ,{ECard::eFive  ,  9 }
        ,{ECard::eFour  , 10 }
        ,{ECard::eTrey  , 11 }
        ,{ECard::eDeuce , 12 }
    };

    std::pair< uint32_t, std::unique_ptr< CHand > > evaluateHand( const std::vector< std::shared_ptr< CCard > >& inputCards, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        auto&& allCards = CCard::allCardsList();
        std::vector< std::list< std::shared_ptr< CCard > > > hands;

        if ( !playInfo || !playInfo->hasWildCards() )
        {
            for ( auto&& ii : inputCards )
            {
                hands.push_back( { ii } );
            }
        }
        else
        {
            // for 5 of a kind...
            // cards are all wild, and all the others are the same card
            std::map< ECard, uint8_t > cardHits;
            size_t numWild = 0;

            for ( auto&& ii : inputCards )
            {
                if ( playInfo && playInfo->isWildCard( ii ) )
                {
                    hands.push_back( allCards );
                    numWild++;
                }
                else
                {
                    hands.push_back( { ii } );
                    cardHits[ ii->getCard() ]++;
                }
            }
            if ( ( cardHits.size() == 1 ) // all the same card
                 && ( ( cardHits.begin()->second + numWild ) == hands.size() ) )
            {
                // five of a kind
                auto card = cardHits.begin()->first;
                auto rank = sFiveOfAKindMap[ card ];
                std::vector< std::shared_ptr< CCard > > hand;
                std::unordered_set< ESuit > suits = { ESuit::eClubs, ESuit::eDiamonds, ESuit::eHearts, ESuit::eSpades };
                for ( auto&& ii : inputCards )
                {
                    if ( playInfo && playInfo->isWildCard( ii ) )
                        continue;
                    hand.push_back( ii );
                    auto pos = suits.find( ii->getSuit() );
                    if ( pos != suits.end() )
                        suits.erase( pos );
                }
                if ( !suits.empty() )
                {
                    for ( auto&& ii : allCards )
                    {
                        if ( ii->getCard() == card )
                        {
                            if ( suits.find( ii->getSuit() ) == suits.end() )
                                continue;
                            else
                                hand.push_back( ii );
                        }
                    }
                }
                // hand should have 4 cards....
                while ( hand.size() != 5 )
                    hand.push_back( *hand.begin() );
                auto retVal = std::make_pair( rank, std::make_unique< CHand >( hand, playInfo ) );
                return retVal;
            }
        }

        std::function< bool( const std::vector< std::shared_ptr< CCard > >& curr, const std::shared_ptr< CCard >& obj ) > addToResult =
            []( const std::vector< std::shared_ptr< CCard > >& data, const std::shared_ptr< CCard >& obj )
        {
            for ( auto&& ii : data )
            {
                if ( *ii == *obj )
                    return false;
            }
            return true;
            //return std::find( curr.begin(), curr.end(), obj ) != curr.end();
        };

        auto allHands = NUtils::cartiseanProduct( hands, addToResult );
        auto retVal = findBest( allHands, playInfo );

        return retVal;
    }

    uint32_t evaluateHandInternal( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        if ( cards.size() == 2 )
            return evaluate2CardHand( cards, playInfo );
        else if ( cards.size() == 3 )
            return evaluate3CardHand( cards, playInfo );
        else if ( cards.size() == 4 )
            return evaluate4CardHand( cards, playInfo );
        else if ( cards.size() == 5 )
            return evaluate5CardHand( cards, playInfo );

        // 6 or greater is permuations of 5
        return -1;
    }

    bool isFlush( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        if ( cards.empty() )
            return false;

        auto value = cardsAndValue( cards );
        value &= 0x0F000;
        return value.to_ulong() != 0;
    }

    uint32_t getCardRank( ECard card )
    {
        auto pos = sFiveOfAKindMap.find( card );
        if ( pos == sFiveOfAKindMap.end() )
            return -1;
        return ( *pos ).second;
    }

    bool isCount( const std::vector< TCard >& cards, uint8_t count )
    {
        std::map< ECard, uint8_t > cardHits;
        for ( auto && card : cards )
            cardHits[ card.first ]++;

        int numAtCount = 0;
        for ( auto&& ii : cardHits )
        {
            if ( ii.second == 1 )
                continue;
            if ( ii.second != count ) // looking for a two, got a 3
                return false;
            numAtCount++;
        }
        return numAtCount == 1;
    }

    enum EStraightType
    {
        eAce = 0b01111100000000, // AKQJT
        eKing = 0b00111110000000, // KQJT9
        eQueen = 0b00011111000000, // QJT98
        eJack = 0b00001111100000, // JT987
        eTen = 0b00000111110000, // T9876
        eNine = 0b00000011111000, // 98765
        eEight = 0b00000001111100, // 87654
        eSeven = 0b00000000111110, // 76543
        eSix = 0b00000000011111, // 65432
        eWheel = 0b01000000001111
    };

    bool isStraight( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        static std::unordered_set< uint16_t > sStraights =
        {
            EStraightType::eAce, // AKQJT
            EStraightType::eKing, // KQJT9
            EStraightType::eQueen, // QJT98
            EStraightType::eJack, // JT987
            EStraightType::eTen, // T9876
            EStraightType::eNine, // 98765
            EStraightType::eEight, // 87654
            EStraightType::eSeven, // 76543
            EStraightType::eSix, // 65432
            EStraightType::eWheel  // 5432A
        };
        if ( cards.empty() )
            return false;

        auto value = getCardsValue( cards );
        return sStraights.find( value ) != sStraights.end();
    }


    bool isStraight( const std::vector< TCard >& cards )
    {
        if ( cards.size() < 2 )
            return false;

        auto sortedCards = cards;
        std::sort( sortedCards.begin(), sortedCards.end(), []( TCard lhs, TCard rhs ) { return lhs.first > rhs.first; } );

        if ( ( sortedCards[ 0 ].first == ECard::eAce ) && ( (*sortedCards.rbegin()).first == ECard::eDeuce ) )
        {
            auto prevCard = *sortedCards.rbegin(); // 2
            for ( size_t ii = sortedCards.size() - 2; ii >= 1; --ii )
            {

                if ( ( static_cast<int>( sortedCards[ ii ].first ) - static_cast<int>( prevCard.first ) ) != 1 )
                    return false;
                prevCard = sortedCards[ ii ];
            }
            return true;
        }

        auto prevCard = sortedCards[ 0 ];
        for ( size_t ii = 1; ii < sortedCards.size(); ++ii )
        {
            if ( ( static_cast<int>( prevCard.first ) - static_cast<int>( sortedCards[ ii ].first ) ) != 1 )
                return false;
            prevCard = sortedCards[ ii ];
        }
        return true;
    }

    uint64_t computeHandProduct( const std::vector < std::shared_ptr< CCard > >& cards )
    {
        if ( cards.empty() )
            return -1;

        uint64_t retVal = 1;
        TCardBitType value( std::numeric_limits< int64_t >::max() );
        for ( auto&& ii : cards )
            retVal *= ( ii->bitValue().to_ulong() & 0x00FF );

        return retVal;
    }

    uint64_t computeHandProduct( const std::vector < TCardBitType >& cards )
    {
        if ( cards.empty() )
            return -1;

        uint64_t retVal = 1;
        TCardBitType value( std::numeric_limits< int64_t >::max() );
        for ( auto&& ii : cards )
        {
            auto currPrime = ( ii.to_ulong() & 0x00FF );
            retVal *= currPrime;
        }

        return retVal;
    }

    TCardBitType cardsOrValue( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        if ( cards.empty() )
            return TCardBitType();

        TCardBitType value;
        for ( auto&& ii : cards )
            value |= ii->bitValue();
        return value;
    }

    TCardBitType cardsOrValue( const std::vector< TCardBitType >& cards )
    {
        if ( cards.empty() )
            return TCardBitType();

        TCardBitType value;
        for ( auto&& ii : cards )
            value |= ii;
        return value;
    }

    TCardBitType cardsAndValue( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        if ( cards.empty() )
            return TCardBitType();
    
        TCardBitType value( std::numeric_limits< int64_t >::max() );
        for ( auto&& ii : cards )
            value &= ii->bitValue();
    
        return value;
    }
    
    TCardBitType cardsAndValue( const std::vector< TCardBitType >& cards )
    {
        if ( cards.empty() )
            return TCardBitType();

        TCardBitType value( std::numeric_limits< int64_t >::max() );
        for ( auto&& ii : cards )
            value &= ii;

        return value;
    }

    uint16_t getCardsValue( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        if ( cards.empty() )
            return -1;
        return static_cast<uint16_t>( ( cardsOrValue( cards ) >> 16 ).to_ulong() );
    }

    uint16_t getCardsValue( const std::vector< TCardBitType > & cards )
    {
        if ( cards.empty() )
            return -1;
        return static_cast<uint16_t>( ( cardsOrValue( cards ) >> 16 ).to_ulong() );
    }

    ECard getMaxCard( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        auto value = cardsOrValue( cards ) >> 16;
        if ( isStraight( cards ) && ( value == EStraightType::eWheel ) )
            return ECard::eFive;
        else
        {
            auto pos = NUtils::findLargestIndexInBitSet( value );
            if ( pos.has_value() )
                return static_cast<ECard>( pos.value() );
            else
                return ECard::eUNKNOWN;
        }
    }

    ECard getMinCard( const std::vector< std::shared_ptr< CCard > >& cards )
    {
        auto value = cardsOrValue( cards ) >> 16;
        if ( isStraight( cards ) && ( value == EStraightType::eWheel ) )
            return ECard::eFive;
        else
        {
            auto pos = NUtils::findSmallestIndexInBitSet( value );
            if ( pos.has_value() )
                return static_cast<ECard>( pos.value() );
            else
                return ECard::eUNKNOWN;
        }
    }

    EHand rankToHand( uint32_t rank, size_t numCards, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        if ( numCards == 2 )
            return rankTo2CardHand( rank, playInfo );
        else if ( numCards == 3 )
            return rankTo3CardHand( rank, playInfo );
        else if ( numCards == 4 )
            return rankTo4CardHand( rank, playInfo );
        else if ( numCards >= 5 )
            return rankTo5CardHand( rank, playInfo );
        else
            return EHand::eNoCards;
    }

    TCardBitType computeBitValue( ECard card, ESuit suit )
    {
        uint32_t tmp =
            ( toBitValue( card ) << 16 )
            | ( ( ( toBitValue( suit ) << 4 ) | toRankValue( card ) ) << 8 )
            | toPrimeValue( card );

        return tmp;
    }

    bool gComputeAllHands{true};
}

std::ostream& operator<<( std::ostream& oss, const std::vector< std::shared_ptr< CCard > >& cards )
{
    oss << "Cards: ";
    bool first = true;
    for ( auto&& ii : cards )
    {
        if ( !first )
            oss << " ";
        first = false;
        oss << *ii;
    }
    oss << "\n";
    return oss;
}
