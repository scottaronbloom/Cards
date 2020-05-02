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

#ifndef _HANDIMPL_H
#define _HANDIMPL_H

#include <QString>
#include <vector>

#include "HandUtils.h"

#include <memory>
#include <optional>

enum class ECard;
enum class EHand;
class CCard;
struct SPlayInfo;

class CHandImpl
{
public:
    CHandImpl( const std::shared_ptr< SPlayInfo >& playInfo );
    void resetHandAnalysis();

    void clearCards();

    void setCards( const std::vector< std::shared_ptr< CCard > >& cards );
    void addCard( std::shared_ptr< CCard > card );

    bool hasCards() const{ return !fCards.empty(); }

    void addWildCard( std::shared_ptr< CCard > card );

    QString toString() const;
    QString determineHandName( bool details ) const;
    std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > determineHand() const;
    EHand computeHand() const;
    EHand getHand() const;
    const std::vector< std::shared_ptr< CCard > > & getCards() const{ return fCards; }
    const std::optional< std::pair< uint32_t, std::unique_ptr< CHand > > >& bestHand() const{ return fBestHand; }

    uint32_t evaluateHand() const;

    bool isStraight() const;
    bool isFlush() const;
private:
    std::vector< std::shared_ptr< CCard > > fCards;
    std::shared_ptr< SPlayInfo > fPlayInfo;

    // these values get cached for speed, but are called via const functions, hence the mutable nature
    mutable std::optional< std::tuple< EHand, std::vector< ECard >, std::vector< ECard > > > fHand;
    mutable std::optional< uint16_t > fCardsValue;
    mutable std::optional< ECard > fMaxCard;
    mutable std::optional< ECard > fMinCard;
    mutable std::optional< NHandUtils::TCardBitType > fAndValue;
    mutable std::optional< NHandUtils::TCardBitType > fOrValue;
    mutable std::optional< uint64_t > fHandProduct;
    mutable std::optional< std::pair< uint32_t, std::unique_ptr< CHand > > > fBestHand; // tanks into account wildcard
};

#endif 

