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

    bool CCardInfo::isStraight() const
    {
        return fStraightType.has_value() && ( fStraightType.value() != EStraightType::eNotStraight );
    }

    bool CCardInfo::isWheel() const
    {
        return fStraightType.has_value() && ( fStraightType.value() == EStraightType::eWheel );
    }


    bool CCardInfo::lessThan( bool flushStraightCount, const CCardInfo& rhs ) const
    {
        for ( auto&& handType : fHandOrder )
        {
            if ( !flushStraightCount && NHandUtils::isStraightOrFlush( handType ) )
                continue;
            switch ( handType )
            {
                case EHand::eHighCard:
                    return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                case EHand::ePair:
                    {
                        if ( !fIsPair && rhs.fIsPair )
                            return true;
                        if ( fIsPair && !rhs.fIsPair )
                            return false;
                        if ( fIsPair && rhs.fIsPair )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    }
                    break;
                case EHand::eTwoPair:
                    {
                        if ( !fIsTwoPair && rhs.fIsTwoPair )
                            return true;
                        if ( fIsTwoPair && !rhs.fIsTwoPair )
                            return false;
                        if ( fIsTwoPair && rhs.fIsTwoPair )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    }
                    break;
                case EHand::eThreeOfAKind:
                    {
                        if ( !fIsThreeOfAKind && rhs.fIsThreeOfAKind )
                            return true;
                        if ( fIsThreeOfAKind && !rhs.fIsThreeOfAKind )
                            return false;
                        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    }
                    break;
                case EHand::eFullHouse:
                    {
                        if ( !fIsFullHouse && rhs.fIsFullHouse )
                            return true;
                        if ( fIsThreeOfAKind && !rhs.fIsFullHouse )
                            return false;
                        if ( fIsFullHouse && rhs.fIsFullHouse )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    }
                    break;
                case EHand::eFourOfAKind:
                    {
                        if ( !fIsFourOfAKind && rhs.fIsFourOfAKind )
                            return true;
                        if ( fIsFourOfAKind && !rhs.fIsFourOfAKind )
                            return false;
                        if ( fIsFourOfAKind && rhs.fIsFourOfAKind )
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    }
                    break;
                    //case EHand::eFiveOfAKind:
                    //    {
                    //        if ( !fIsF && rhs.fIsFourOfAKind )
                    //            return true;
                    //        if ( fIsFourOfAKind && !rhs.fIsFourOfAKind )
                    //            return false;
                    //        if ( fIsFourOfAKind && rhs.fIsFourOfAKind )
                    //            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
                    //    }
                    //    break;
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
                            return NHandUtils::compareCards( { fCards, fKickers }, { rhs.fCards, rhs.fKickers } );
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

    bool CCardInfo::greaterThan( bool flushStraightCount, const CCardInfo& rhs ) const
    {
        return !lessThan( flushStraightCount, rhs ) && !equalTo( flushStraightCount, rhs );
    }

    bool CCardInfo::equalTo( bool flushStraightCount, const CCardInfo& rhs ) const
    {
        if ( fIsFourOfAKind && rhs.fIsFourOfAKind )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;
        if ( fIsThreeOfAKind && rhs.fIsThreeOfAKind )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;
        if ( fIsPair && rhs.fIsPair )
            return fCards == rhs.fCards && fKickers == rhs.fKickers;

        if ( flushStraightCount )
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

    void CCardInfo::generateMap( std::ostream& oss, const std::map< uint64_t, uint16_t > & values, const std::string& varName )
    {
        oss
            << "" << "static std::unordered_map< int64_t, int16_t > " << varName << " = \n"
            << "{\n"
            ;
        bool first = true;
        for( auto && ii : values )
        {
            oss << "    ";

            if ( !first )
                oss << ",";
            else
                oss << " ";
            first = false;
            oss << "{ " << ii.first << ", " << ii.second << " }\n";
        }
        oss << "};\n\n";
        oss.flush();
    }

    void CCardInfo::generateTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string & varName )
    {
        oss 
            << "" << "static std::vector< uint32_t > " << varName << " = \n"
            << "{\n"
            ;
        size_t numChars = 0;
        for( size_t ii = 0; ii < values.size(); ++ii )
        {
            if ( numChars == 0 )
                oss << "    ";

            auto tmp = std::to_string( values[ ii ] );
            oss << tmp;
            numChars += tmp.size();
            if ( ii != ( values.size() -1 ) )
            {
                oss << ", ";
                numChars += 2;
            }

            if ( numChars >= 66 )
            {
                numChars = 0;
                oss << "\n";
            }
        }
        oss << "\n    };\n\n";
        oss.flush();
    }

    void CCardInfo::generateEvaluateFunction( std::ostream& oss, size_t size )
    {
        oss << "uint32_t evaluate" << size << "CardHand( const std::vector< std::shared_ptr< CCard > > & cards, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << "{\n"
            << "    if ( cards.size() != " << size << " )\n"
            << "        return -1;\n"
            << "\n"
            << "    auto cardsValue = getCardsValue( cards );\n"
            << "    if ( playInfo && playInfo->fStraightsFlushesCount )\n"
            << "    {\n"
            << "        if ( isFlush( cards ) )\n"
            << "            return sFlushes[ cardsValue ];\n"
            << "    }\n"
            << "\n"
            << "    auto straightOrHighCard = playInfo->fStraightsFlushesCount ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];\n"
            << "    if ( straightOrHighCard )\n"
            << "        return straightOrHighCard;\n"
            << "\n"
            << "    auto product = computeHandProduct( cards );\n"
            << "    auto productMap = playInfo->fStraightsFlushesCount ? sStraitsAndFlushesProductMap : sProductMap;\n"
            << "    auto pos = productMap.find( product );\n"
            << "    if ( pos == productMap.end() )\n"
            << "        return -1;\n"
            << "    return ( *pos ).second;\n"
            << "}\n\n";
    }

    bool CCardInfo::isHighCard() const
    {
        return !isStraightFlush() 
            && !isFourOfAKind() 
            && !isFullHouse() 
            && !isFlush() 
            && !isStraight() 
            && !isThreeOfAKind() 
            && !isTwoPair() 
            && !isPair();
    }

    EHand CCardInfo::getHandType( bool flushStraightCount ) const
    {
        if ( flushStraightCount && isStraightFlush() )
            return EHand::eStraightFlush;
        else if ( isFourOfAKind() )
            return EHand::eFourOfAKind;
        else if ( isFullHouse() )
            return EHand::eFullHouse;
        else if ( flushStraightCount && isFlush() )
            return EHand::eFlush;
        else if ( flushStraightCount && isStraight() )
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
        return !fIsFourOfAKind && !fIsFullHouse && !fIsThreeOfAKind && !fIsTwoPair && !fIsPair;
    }

    std::vector< TCard > CCardInfo::setOrigCards( const std::vector< TCard >& cards )
    {
        fOrigCards = cards;
        fBitValues.resize( fOrigCards.size() );
        for ( size_t ii = 0; ii < fOrigCards.size(); ++ii )
            fBitValues[ ii ] = NHandUtils::computeBitValue( fOrigCards[ ii ].first, fOrigCards[ ii ].second );

        auto sortedCards = fOrigCards;
        std::sort( sortedCards.begin(), sortedCards.end(), []( TCard lhs, TCard rhs ) { return lhs.first > rhs.first; } );
        return sortedCards;
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
        return oss;
    }

}