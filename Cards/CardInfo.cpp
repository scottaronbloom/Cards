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
#include "Hand.h"
#include "PlayInfo.h"
#include "HandUtils.h"

#include "Evaluate2CardHand.h"
#include "Evaluate3CardHand.h"
#include "Evaluate4CardHand.h"
#include "Evaluate5CardHand.h"

#include <map>
#include <set>
#include <optional>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>

namespace NHandUtils
{
    CCardInfo::CCardInfo()
    {
    }

    std::shared_ptr< NHandUtils::CCardInfo > CCardInfo::createCardInfo( const THand & hand )
    {
        switch ( hand.size() )
        {
            case 2:
                return std::make_shared< C2CardInfo >( hand[ 0 ].first, hand[ 0 ].second, hand[ 1 ].first, hand[ 1 ].second );
            case 3:
                return std::make_shared< C3CardInfo >( hand[ 0 ].first, hand[ 0 ].second, hand[ 1 ].first, hand[ 1 ].second, hand[ 2 ].first, hand[ 2 ].second );
            case 4:
                return std::make_shared< C4CardInfo >( hand[ 0 ].first, hand[ 0 ].second, hand[ 1 ].first, hand[ 1 ].second, hand[ 2 ].first, hand[ 2 ].second, hand[ 3 ].first, hand[ 3 ].second );
            case 5:
                return std::make_shared< C5CardInfo >( hand[ 0 ].first, hand[ 0 ].second, hand[ 1 ].first, hand[ 1 ].second, hand[ 2 ].first, hand[ 2 ].second, hand[ 3 ].first, hand[ 3 ].second, hand[ 4 ].first, hand[ 4 ].second );
            default:
                return nullptr;
        }
    }

    bool CCardInfo::isStraight() const
    {
        return fStraightType.has_value() && ( fStraightType.value() != EStraightType::eNotStraight );
    }

    bool CCardInfo::isWheel() const
    {
        return fStraightType.has_value() && ( fStraightType.value() == EStraightType::eWheel );
    }

    bool CCardInfo::lessThan( const CCardInfo& rhs, bool straightsAndFlushesCount, bool lowHandWins ) const
    {
        for ( auto&& handType : fHandOrder )
        {
            if ( !straightsAndFlushesCount && NHandUtils::isStraightOrFlush( handType ) )
                continue;
            switch ( handType )
            {
                case EHand::eHighCard:
                    {
                        bool isHighCard = this->isHighCard( straightsAndFlushesCount );
                        bool rhsIsHighCard = rhs.isHighCard( straightsAndFlushesCount );
                        if ( !isHighCard && rhsIsHighCard )
                            return false; // im better than a highcard
                        if ( isHighCard && !rhsIsHighCard )
                            return true;
                        if ( isHighCard && rhsIsHighCard )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                case EHand::ePair:
                    {
                        if ( !fIsPair && rhs.fIsPair )
                            return true;
                        if ( fIsPair && !rhs.fIsPair )
                            return false;
                        if ( fIsPair && rhs.fIsPair )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                    break;
                case EHand::eTwoPair:
                    {
                        if ( !fIsTwoPair && rhs.fIsTwoPair )
                            return true;
                        if ( fIsTwoPair && !rhs.fIsTwoPair )
                            return false;
                        if ( fIsTwoPair && rhs.fIsTwoPair )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                    break;
                case EHand::eThreeOfAKind:
                    {
                        if ( !fIsThreeOfAKind && rhs.fIsThreeOfAKind )
                            return true;
                        if ( fIsThreeOfAKind && !rhs.fIsThreeOfAKind )
                            return false;
                        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                    break;
                case EHand::eFullHouse:
                    {
                        if ( !fIsFullHouse && rhs.fIsFullHouse )
                            return true;
                        if ( fIsFullHouse && !rhs.fIsFullHouse )
                            return false;
                        if ( fIsFullHouse && rhs.fIsFullHouse )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                    break;
                case EHand::eFourOfAKind:
                    {
                        if ( !fIsFourOfAKind && rhs.fIsFourOfAKind )
                            return true;
                        if ( fIsFourOfAKind && !rhs.fIsFourOfAKind )
                            return false;
                        if ( fIsFourOfAKind && rhs.fIsFourOfAKind )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                    }
                    break;
                    case EHand::eFiveOfAKind:
                        {
                            if ( !fIsFiveOfAKind && rhs.fIsFiveOfAKind )
                                return true;
                            if ( fIsFiveOfAKind && !rhs.fIsFiveOfAKind )
                                return false;
                            if ( fIsFiveOfAKind && rhs.fIsFiveOfAKind )
                                return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, lowHandWins );
                        }
                        break;
                case EHand::eStraight:
                    {
                        auto straightCompare = NHandUtils::compareStraight( fStraightType, rhs.fStraightType );
                        if ( straightCompare.has_value() )
                            return straightCompare.value();
                    }
                    break;
                case EHand::eFlush:
                    {
                        if ( fIsFlush && !rhs.fIsFlush )
                            return false;
                        if ( !fIsFlush && rhs.fIsFlush )
                            return true;
                        if ( fIsFlush && rhs.fIsFlush )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers }, false );
                    }
                    break;
                case EHand::eStraightFlush:
                    {
                        if ( isStraightFlush() && !rhs.isStraightFlush() )
                            return false;
                        if ( !isStraightFlush() && rhs.isStraightFlush() )
                            return true;
                        if ( isStraightFlush() && rhs.isStraightFlush() )
                        {
                            auto straightCompare = NHandUtils::compareStraight( fStraightType, rhs.fStraightType );
                            if ( straightCompare.has_value() )
                                return straightCompare.value();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        return false;
    }

    bool CCardInfo::greaterThan( const CCardInfo& rhs, bool straightsAndFlushesCount, bool lowHandWins ) const
    {
        return !lessThan( rhs, straightsAndFlushesCount, lowHandWins ) && !equalTo( rhs, straightsAndFlushesCount );
    }

    bool CCardInfo::equalTo( const CCardInfo& rhs, bool straightsAndFlushesCount ) const
    {
        if ( fIsFiveOfAKind && rhs.fIsFiveOfAKind )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;
        if ( fIsFourOfAKind && rhs.fIsFourOfAKind )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;
        if ( fIsPair && rhs.fIsPair )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;

        if ( straightsAndFlushesCount )
        {
            if ( fIsFlush != rhs.fIsFlush )
                return false;

            auto straightsEqual = NHandUtils::straightsEqual( fStraightType, rhs.fStraightType );
            if ( straightsEqual.has_value() )
                return straightsEqual.value();
        }
        return ( fCards == rhs.fCards ) && ( fKickers == rhs.fKickers );
    }

    uint16_t CCardInfo::getCardsValue() const
    {
        return NHandUtils::getCardsValue( fBitValues );
    }
    
    uint64_t CCardInfo::handProduct() const
    {
        return NHandUtils::computeHandProduct( fBitValues );
    }

    bool CCardInfo::isHighCard( bool straightsAndFlushesCount ) const
    {
        if ( straightsAndFlushesCount && ( isStraightFlush() || isFlush() || isStraight() ) )
            return false;
        return !isFourOfAKind()
            && !isFullHouse() 
            && !isThreeOfAKind() 
            && !isTwoPair() 
            && !isPair();
    }

    EHand CCardInfo::getHandType( bool straightsAndFlushesCount, bool /*lowBall*/ ) const
    {
        if ( straightsAndFlushesCount && isStraightFlush() )
            return EHand::eStraightFlush;
        else if ( isFourOfAKind() )
            return EHand::eFourOfAKind;
        else if ( isFullHouse() )
            return EHand::eFullHouse;
        else if ( straightsAndFlushesCount && isFlush() )
            return EHand::eFlush;
        else if ( straightsAndFlushesCount && isStraight() )
            return EHand::eStraight;
        else if ( isThreeOfAKind() )
            return EHand::eThreeOfAKind;
        else if ( isTwoPair() )
            return EHand::eTwoPair;
        else if ( isPair() )
            return EHand::ePair;
        else
            return EHand::eHighCard;
    }

    bool CCardInfo::allCardsUnique() const
    {
        return !fIsFiveOfAKind && !fIsFourOfAKind && !fIsFullHouse && !fIsThreeOfAKind && !fIsTwoPair && !fIsPair;
    }

    void CCardInfo::setOrigCards( const std::vector< TCard >& cards )
    {
        fOrigCards = cards;
        fBitValues.resize( fOrigCards.size() );
        for ( size_t ii = 0; ii < fOrigCards.size(); ++ii )
            fBitValues[ ii ] = NHandUtils::computeBitValue( fOrigCards[ ii ].first, fOrigCards[ ii ].second );

        setupKickers();
    }

    void CCardInfo::setupKickers()
    {
        if ( fOrigCards.empty() )
            return;

        auto sortedCards = fOrigCards;
        std::sort( sortedCards.begin(), sortedCards.end(), []( TCard lhs, TCard rhs ) { return lhs.first > rhs.first; } );

        std::map< ECard, uint8_t > cardHits;
        for ( auto&& card : sortedCards )
        {
            cardHits[ card.first ]++;
        }
        std::map< uint8_t, std::list< ECard > > numHitsToCard;
        for ( auto ii : cardHits )
        {
            numHitsToCard[ ii.second ].push_back( ii.first );
        }

        for ( auto&& ii : numHitsToCard )
        {
            ii.second.sort( []( ECard lhs, ECard rhs ) { return lhs > rhs; } );
        }

        for ( auto ii = numHitsToCard.rbegin(); ii != numHitsToCard.rend(); ++ii )
        {
            if ( ( *ii ).first > 1 )
            {
                for ( auto&& jj : ( *ii ).second )
                    fCards.push_back( jj );
            }
            else
            {
                for ( auto&& jj : ( *ii ).second )
                    fKickers.push_back( jj );
            }
        }
        if ( fCards.empty() )
        {
            fCards.push_back( *fKickers.begin() );
            fKickers.erase( fKickers.begin() );
        }
    }

    std::ostream& operator<<( std::ostream& oss, const CCardInfo& obj )
    {
        bool first = true;
        for ( auto&& ii : obj.origCards() )
        {
            if ( !first )
                oss << " ";
            first = false;
            oss << ii.first << ii.second;
        }

        oss << " - Hand Value: " << NHandUtils::getCardsValue( obj.origCards() );
        return oss;
    }

    const std::vector< uint32_t > & SCardInfoData::getUniqueVector( bool straightsAndFlushesCount, bool lowHandWins ) const
    {
        auto whichItem = getWhichItem( straightsAndFlushesCount, lowHandWins );
        return fUniqueVectors[ whichItem ];
    }

    const std::unordered_map< int64_t, int16_t > & SCardInfoData::getProductMap( bool straightsAndFlushesCount, bool lowHandWins ) const
    {
        auto whichItem = getWhichItem( straightsAndFlushesCount, lowHandWins );
        return fProductMaps[ whichItem ];
    }

    const std::map< THand, uint32_t > & SCardInfoData::getCardMap( bool straightsAndFlushesCount, bool lowHandWins ) const
    {
        auto whichItem = getWhichItem( straightsAndFlushesCount, lowHandWins );
        return fCardMaps[ whichItem ];
    }

    uint32_t SCardInfoData::evaluateCardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo, size_t expectedSize )
    {
        if ( cards.size() != expectedSize )
            return -1;

        auto cardsValue = NHandUtils::getCardsValue( cards );
        if ( playInfo && playInfo->fStraightsAndFlushesCount )
        {
            if ( NHandUtils::isFlush( cards ) )
                return fFlushes[ cardsValue ] + ( playInfo->hasWildCards() ? 13 : 0 );
        }

        auto && straightOrHighCardVector = getUniqueVector( playInfo->fStraightsAndFlushesCount, playInfo->fLowHandWins );
        auto straightOrHighCard = straightOrHighCardVector[ cardsValue ];
        if ( straightOrHighCard )
            return straightOrHighCard + ( playInfo->hasWildCards() ? 13 : 0 );

        auto product = computeHandProduct( cards );
        auto && productMap = getProductMap( playInfo->fStraightsAndFlushesCount, playInfo->fLowHandWins );
        auto pos = productMap.find( product );
        if ( pos == productMap.end() )
            return -1;
        return ( *pos ).second + ( playInfo->hasWildCards() ? 13 : 0 );
    }

}