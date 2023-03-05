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
    static std::string getPadding( size_t sz )
    {
        return std::string( 4 * sz, ' ' );
    }

    static std::string getWhichItemEnum( bool straightsAndFlushesCount, bool lowBall )
    {
        std::string retVal = "SCardInfoData::EWhichItem::eStraightsAndFlushes";

        if ( straightsAndFlushesCount && !lowBall )
            retVal += "Count";
        else if ( straightsAndFlushesCount && lowBall )
            retVal += "CountLowBall";
        else if ( !straightsAndFlushesCount && !lowBall )
            retVal += "DontCount";
        else /* if ( !straightsAndFlushesCount && lowBall ) */
            retVal += "DontCountLowBall";
        return retVal;
    }

    THand CCardInfo::createTHand( std::vector< std::shared_ptr< CCard > >& sharedCards ) const
    {
        THand retVal;
        retVal.reserve( sharedCards.size() );
        for ( auto&& ii : sharedCards )
        {
            retVal.push_back( TCard( ii->getCard(), ii->getSuit() ) );
        }
        return retVal;
    }

    std::string getClassName( size_t sz ) 
    {
        std::string className = "C" + std::to_string( sz ) + "CardInfo";
        return className;
    }
    void CCardInfo::generateHeader( std::ostream& oss, size_t size ) const
    {
        oss << "#include \"Evaluate" << size << "CardHand.h\"\n"
            << "#include \"PlayInfo.h\"\n"
            << "#include \"Hand.h\"\n"
            << "\n"
            << "#include <map>\n"
            << "\n"
            << "//#define __USECARDMAPS\n"
            << "\n"
            << "namespace NHandUtils\n"
            << "{\n"
            << getPadding( 1 ) << "SCardInfoData " << getClassName( size ) << "::sCardInfoData = {};\n"
            << "\n"
            << getPadding( 1 ) << "void " << getClassName( size ) << "::initMaps()\n"
            << getPadding( 1 ) << "{\n"
            << getPadding( 2 )     << "if ( !sCardInfoData.fTablesInitialized )\n"
            << getPadding( 2 )     << "{\n"
            << getPadding( 3 )         << "sCardInfoData.fTablesInitialized = true;\n"
            ;
    }

    void CCardInfo::generateFooter( std::ostream& oss ) const
    {
        oss << "}\n\n";
    }

    template< typename T>
    void CCardInfo::computeAndGenerateMap( std::ostream& oss, T& map, bool straightsAndFlushesCount, bool lowBall ) const
    {
        sabDebugStream() << "Computing hand values: " << map.size() << "\n";

        int num = 1;
        auto updateOn = std::min( static_cast<uint64_t>( 10000 ), static_cast<uint64_t>( map.size() / 25 ) );
        for ( auto&& ii : map )
        {
            if ( ( num % updateOn ) == 0 )
                sabDebugStream() << "   Computing Hand Value: Hand #" << num << " of " << map.size() << "\n";
            ii.second = num++;
        }
        sabDebugStream() << "Finished Computing hand values: " << map.size() << "\n";

        std::string comment;
        if ( straightsAndFlushesCount )
        {
            if ( lowBall )
            {
                comment = "Flushes/Straights Count - LowBall";
            }
            else
            {
                comment = "Flushes/Straights Count";
            }
        }
        else
        {
            if ( lowBall )
            {
                comment = "Flushes/Straights Don't Count - LowBall";
            }
            else
            {
                comment = "Flushes/Straights Don't Count";
            }
        }

        using NHandUtils::getPadding;
        oss
            << getPadding( 3 ) << "// " << comment << "\n"
            << getPadding( 3 ) << "sCardInfoData.fCardMaps[ " << getWhichItemEnum( straightsAndFlushesCount, lowBall ) << " ] =\n"
            << getPadding( 3 ) << "{\n"
            ;

        sabDebugStream() << "Writing Map: " << map.size() << "\n";
        EHand prevHandType = EHand::eNoCards;
        bool first = true;
        for ( auto ii = map.begin(); ii != map.end(); ++ii )
        {
            if ( ( ( *ii ).second % updateOn ) == 0 )
                sabDebugStream() << "   Computing Hand Value: Hand #" << ( *ii ).second << " of " << map.size() << "\n";

            oss << getPadding( 4 );
            if ( first )
                oss << " ";
            else
                oss << ",";
            first = false;
            auto firstCard = ( *ii ).first->fOrigCards[ 0 ];
            auto secondCard = ( *ii ).first->fOrigCards[ 1 ];
            if ( ( firstCard.first == ECard::eDeuce ) && ( secondCard.first == ECard::eAce ) )
                std::swap( firstCard, secondCard );
            oss << "{ { "
                << "{ " << qPrintable( ::asEnum( firstCard.first ) ) << ", " << qPrintable( ::asEnum( firstCard.second ) ) << " }"
                << ", { " << qPrintable( ::asEnum( secondCard.first ) ) << ", " << qPrintable( ::asEnum( secondCard.second ) ) << " }"
                ;
            for ( size_t jj = 2; jj < ( *ii ).first->fOrigCards.size(); ++jj )
                oss << ", { " << qPrintable( ::asEnum( ( *ii ).first->fOrigCards[ jj ].first ) ) << ", " << qPrintable( ::asEnum( ( *ii ).first->fOrigCards[ jj ].second ) ) << " } ";

            oss << "}, " << ( *ii ).second << " }";

            auto currHandType = ( *ii ).first->getHandType( straightsAndFlushesCount, lowBall );

            auto next = ii;
            ++next;
            bool printHandType = ( currHandType != prevHandType ) || ( next == map.end() ) || ( ( *next ).first->getHandType( straightsAndFlushesCount, lowBall ) != currHandType );
            if ( printHandType )
                oss << " // " << toCPPString( currHandType );
            oss << "\n";
            prevHandType = currHandType;
        }
        sabDebugStream() << "Finished Writing Map: " << map.size() << "\n";
        oss << getPadding( 3 ) << "};\n\n";
        oss.flush();
    }

    template< typename T1, typename T2, typename T3, typename T4 >
    void CCardInfo::computeAndGenerateMaps( std::ostream& oss, size_t /*size*/, T1& straightsAndFlushesDontCount, T2& straightsAndFlushesCount, T3& straightsAndFlushesDontCountLowBall, T4& straightsAndFlushesCountLowBall ) const
    {
        oss
            << "#ifdef __USECARDMAPS\n"
            ;

        computeAndGenerateMap( oss, straightsAndFlushesDontCount, false, false );
        computeAndGenerateMap( oss, straightsAndFlushesCount, true, false );
        computeAndGenerateMap( oss, straightsAndFlushesDontCountLowBall, false, true );
        computeAndGenerateMap( oss, straightsAndFlushesCountLowBall, true, true );

        oss << "#else\n"
            << getPadding( 3 ) << "sCardInfoData.fCardMaps = { {}, {}, {}, {} };\n"
            << "#endif\n"
            << "\n"
            ;
    }

    template< typename T >
    void CCardInfo::generateITE( std::ostream& oss, size_t size, const T& map, bool straightsAndFlushesCount, bool lowBall ) const
    {
        std::map< EHand, uint32_t > handTypeToMinRank;
        for ( auto&& ii : map )
        {
            auto handType = ii.first->getHandType( straightsAndFlushesCount, lowBall );
            auto pos = handTypeToMinRank.find( handType );
            auto minValue = ii.second;

            if ( ( pos == handTypeToMinRank.end() ) || ( minValue < (*pos).second ) )
                handTypeToMinRank[ handType ] = minValue;
        }

        std::map< uint32_t, EHand > minRankToHandType;
        for ( auto&& ii : handTypeToMinRank )
        {
            minRankToHandType[ ii.second ] = ii.first;
        }
        std::string wildCardSuffix;
        if ( size == 5 )
            wildCardSuffix = " + ( playInfo->hasWildCards() ? 13 : 0 )";

        bool first = true;
        for ( auto ii = minRankToHandType.rbegin(); ii != minRankToHandType.rend(); ++ii )
        {
            oss << getPadding( 3 );
            if ( !first )
                oss << "else ";
            first = false;

            bool isOne = ( ( *ii ).first == 1 );
            if ( isOne )
                oss << "/* ";
            oss << "if ( rank >= " << ( *ii ).first << "U" << wildCardSuffix << " )";
            if ( isOne )
                oss << " */";
            oss << "\n"
                << getPadding( 4 ) << "hand = " << toCPPString( ( *ii ).second ) << ";\n";
        }
    }

    template< typename T1, typename T2, typename T3, typename T4 >
    void CCardInfo::generateRankFunction( std::ostream& oss, size_t size, const T1& straightsAndFlushesDontCount, const T2& straightsAndFlushesCount, const T3& straightsAndFlushesDontCountLowBall, const T4& straightsAndFlushesCountLowBall ) const
    {
        oss << getPadding( 1 ) << "EHand C" << size << "CardInfo::rankToCardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << getPadding( 1 ) << "{\n"
            << getPadding( 2 ) << "EHand hand;\n"
            << getPadding( 2 ) << "if ( !playInfo->fStraightsAndFlushesCount && !playInfo->fLowHandWins )\n"
            << getPadding( 2 ) << "{\n";
        generateITE( oss, size, straightsAndFlushesDontCount, false, false );
        oss << getPadding( 2 ) << "}\n"
            << getPadding( 2 ) << "else if ( !playInfo->fStraightsAndFlushesCount && playInfo->fLowHandWins )\n"
            << getPadding( 2 ) << "{\n";
        generateITE( oss, size, straightsAndFlushesDontCountLowBall, false, true );
        oss << getPadding( 2 ) << "}\n"
            << getPadding( 2 ) << "else if ( playInfo->fStraightsAndFlushesCount && !playInfo->fLowHandWins )\n"
            << getPadding( 2 ) << "{\n";
        generateITE( oss, size, straightsAndFlushesCount, true, false );
        oss << getPadding( 2 ) << "}\n"
            << getPadding( 2 ) << "else /* if ( playInfo->fStraightsAndFlushesCount && playInfo->fLowHandWins ) */\n"
            << getPadding( 2 ) << "{\n";
        generateITE( oss, size, straightsAndFlushesCountLowBall, true, true );
        oss << getPadding( 2 ) << "}\n"
            << getPadding( 2 ) << "return hand;\n"
            << getPadding( 1 ) << "}\n"
            ;
        oss.flush();
    }

    static void generateTable( std::ostream& oss, const std::vector< uint32_t >& values, const std::string& varName )
    {
        size_t indent = 3;
        oss
            << getPadding( indent ) << "sCardInfoData."
            << varName << " =\n"
            << getPadding( indent )
            ;

        oss << "{\n";


        size_t numChars = 0;
        for ( size_t ii = 0; ii < values.size(); ++ii )
        {
            if ( numChars == 0 )
                oss << getPadding( indent + 1 );

            auto tmp = std::to_string( values[ ii ] );
            oss << tmp;
            numChars += tmp.size();
            bool lastValue = ( ii == ( values.size() - 1 ) );
            if ( !lastValue )
            {
                oss << ",";
                numChars += 2;
            }

            if ( numChars >= 66 )
            {
                numChars = 0;
                oss << "\n";
            }
            else if ( !lastValue )
                oss << " ";
        }
        oss << "\n" << getPadding( indent ) << "}";
        if ( !varName.empty() )
            oss << ";\n";
        oss << "\n";
        oss.flush();
    }

    static void generateMap( std::ostream& oss, const std::map< uint64_t, uint16_t >& values, const std::string & varName )
    {
        size_t indent = 3;
        oss
            << getPadding( indent ) << "sCardInfoData."
            << varName << " =\n"
            << getPadding( indent )
            ;

        oss << "{\n"
            ;
        bool first = true;
        for ( auto&& ii : values )
        {
            oss << getPadding( indent + 1 ) ;

            if ( !first )
                oss << ",";
            else
                oss << " ";
            first = false;
            oss << "{ " << ii.first << ", " << ii.second << " }\n";
        }
        oss << getPadding( indent ) << "};\n\n";
        oss.flush();
    }

    //static void generateMaps( std::ostream& oss, const std::vector< std::map< uint64_t, uint16_t > > & values, const std::string& className, const std::string & typeName, const std::string& varName )
    /*{
        oss
            << getPadding( 1 ) << className << "::" << typeName << " " << className << "::" << varName << " =\n"
            << getPadding( 1 ) << "{\n"
            ;
        for( size_t ii = 0; ii < values.size(); ++ii )
        {
            oss << getPadding( 2 );
            if ( ii != 0 )
                oss << ",";
            generateMap( oss, values[ ii ] );
        }
        oss << getPadding( 1 ) << "};\n\n";
        oss.flush();
    }*/

    static void generateEvaluateFunction( std::ostream& oss, size_t size )
    {
        std::string wildCardSuffix;
        if ( size == 5 )
            wildCardSuffix = " + ( playInfo->hasWildCards() ? 13 : 0 )";
        oss << getPadding( 1 ) << "uint32_t C" << size << "CardInfo::evaluateCardHand( const std::vector< std::shared_ptr< CCard > > & cards, const std::shared_ptr< SPlayInfo > & playInfo )\n"
            << getPadding( 1 ) << "{\n"
            << getPadding( 2 )     << "initMaps();\n"
            << getPadding( 2 )     << "return sCardInfoData.evaluateCardHand( cards, playInfo, " << size << " );\n"
            << getPadding( 1 ) << "}\n\n";
    }

    template< typename T >
    uint32_t getValue( const T & map, std::shared_ptr< CCardInfo > cardInfo ) 
    {
        auto pos = map.find( cardInfo );
        Q_ASSERT( pos != map.end() );
        auto retVal = (*pos).second;
        return retVal;
    }

    void CCardInfo::generateAllCardHands()
    {
        if ( NHandUtils::gComputeAllHands && !allHandsComputed() )
        {
            setAllHandsComputed( true );

            auto cmpStraightsAndFlushesDontCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( *rhs, false, false ); };
            auto cmpFlushAndStraightsCount = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( *rhs, true, false ); };
            auto cmpStraightsAndFlushesDontCountLowBall = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( *rhs, false, true ); };
            auto cmpFlushAndStraightsCountLowBall = []( const std::shared_ptr< CCardInfo >& lhs, const std::shared_ptr< CCardInfo >& rhs ) { return lhs->greaterThan( *rhs, true, true ); };

            using TStraightsAndFlushesDontCountMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( cmpStraightsAndFlushesDontCount ) >;
            using TStraightsAndFlushesCountMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( cmpFlushAndStraightsCount ) >;
            using TStraightsAndFlushesDontCountLowBallMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( cmpStraightsAndFlushesDontCountLowBall ) >;
            using TStraightsAndFlushesCountLowBallMap = std::map< std::shared_ptr< CCardInfo >, uint32_t, decltype( cmpFlushAndStraightsCountLowBall ) >;

            using TCardToInfoMap = std::map< THand, std::shared_ptr< CCardInfo > >;
            TCardToInfoMap allHands;
            TStraightsAndFlushesDontCountMap straightsAndFlushesDontCount( cmpStraightsAndFlushesDontCount );
            TStraightsAndFlushesCountMap straightsAndFlushesCount( cmpFlushAndStraightsCount );
            TStraightsAndFlushesDontCountLowBallMap straightsAndFlushesDontCountLowBall( cmpStraightsAndFlushesDontCountLowBall );
            TStraightsAndFlushesCountLowBallMap straightsAndFlushesCountLowBall( cmpFlushAndStraightsCountLowBall );

            auto numHands = NUtils::numCombinations( 52, getNumCards() );
            sabDebugStream() << "Generating: " << numHands << "\n";

            size_t maxCardsValue = 0;

            auto&& allCardsVector = CCard::allCards();
            auto updateOn = std::min( static_cast<uint64_t>( 10000 ), numHands / 25 );
            auto allCardCombos = NUtils::allCombinations( allCardsVector, getNumCards(), { true, updateOn } );
            for ( size_t ii = 0; ii < allCardCombos.size(); ++ii )
            {
                if ( ( ii % updateOn ) == 0 )
                    sabDebugStream() << "   Generating: Hand #" << ii << " of " << numHands << "\n";

                auto curr = createTHand( allCardCombos[ ii ] );
                auto cardInfo = createCardInfo( curr );
                allHands[ curr ] = cardInfo;
                straightsAndFlushesDontCount.insert( std::make_pair( cardInfo, -1 ) );
                straightsAndFlushesCount.insert( std::make_pair( cardInfo, -1 ) );
                straightsAndFlushesDontCountLowBall.insert( std::make_pair( cardInfo, -1 ) );
                straightsAndFlushesCountLowBall.insert( std::make_pair( cardInfo, -1 ) );

                maxCardsValue = std::max( static_cast<size_t>( cardInfo->getCardsValue() ), maxCardsValue );
            }

            sabDebugStream() << "Finished Generating: " << numHands << "\n";

            std::string fileName = "E:/DropBox/Documents/sb/github/scottaronbloom/CardGame/Cards/" + std::to_string( getNumCards() ) + "CardHandTables.cpp.new";
            std::ofstream ofs( fileName );
            std::ostream& oss = ofs;

            generateHeader( oss, getNumCards() );
            computeAndGenerateMaps( oss, getNumCards(), straightsAndFlushesDontCount, straightsAndFlushesCount, straightsAndFlushesDontCountLowBall, straightsAndFlushesCountLowBall );

            std::vector< uint32_t > flushes;
            flushes.resize( maxCardsValue + 1 );

            std::vector< std::vector< uint32_t > > uniqueValueVectors;
            uniqueValueVectors.resize( 4 );
            for( auto && ii : uniqueValueVectors )
                ii.resize( maxCardsValue + 1 );

            std::vector< std::map< uint64_t, uint16_t > > productMaps;
            productMaps.resize( 4 );

            for ( auto&& ii : allHands )
            {
                auto cardValue = ii.second->getCardsValue();

                auto straightsAndFlushesDontCountValue = getValue( straightsAndFlushesDontCount, ii.second );
                auto straightsAndFlushesCountValue = getValue( straightsAndFlushesCount, ii.second );
                auto straightsAndFlushesDontCountLowBallValue = getValue( straightsAndFlushesDontCountLowBall, ii.second );
                auto straightsAndFlushesCountLowBallValue = getValue( straightsAndFlushesCountLowBall, ii.second );

                if ( ii.second->isFlush() )
                {
                    flushes[ cardValue ] = straightsAndFlushesCountValue;
                }
                else if ( ii.second->allCardsUnique() )
                {
                    uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ][ cardValue ] = straightsAndFlushesDontCountValue;
                    uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ][ cardValue ] = straightsAndFlushesCountValue;
                    uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ][ cardValue ] = straightsAndFlushesDontCountLowBallValue;
                    uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ][ cardValue ] = straightsAndFlushesCountLowBallValue;
                }
                else
                {
                    auto productValue = ii.second->handProduct();
                    productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ][ productValue ] = straightsAndFlushesDontCountValue;
                    productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ][ productValue ] = straightsAndFlushesCountValue;
                    productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ][ productValue ] = straightsAndFlushesDontCountLowBallValue;
                    productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ][ productValue ] = straightsAndFlushesCountLowBallValue;
                }
            }

            std::string className = getClassName( getNumCards() );
            generateTable( oss, flushes, "fFlushes" );

            generateTable( oss, uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ], "fUniqueVectors[ " + getWhichItemEnum( false, false ) + " ]" );
            generateTable( oss, uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ], "fUniqueVectors[ " + getWhichItemEnum( true, false ) + " ]" );
            generateTable( oss, uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ], "fUniqueVectors[ " + getWhichItemEnum( false, true ) + " ]" );
            generateTable( oss, uniqueValueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ], "fUniqueVectors[ " + getWhichItemEnum( true, true ) + " ]" );

            generateMap( oss, productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ], "fProductMaps[ " + getWhichItemEnum( false, false ) + " ]" );
            generateMap( oss, productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ], "fProductMaps[ " + getWhichItemEnum( true, false ) + " ]" );
            generateMap( oss, productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ], "fProductMaps[ " + getWhichItemEnum( false, true ) + " ]" );
            generateMap( oss, productMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ], "fProductMaps[ " + getWhichItemEnum( true, true ) + " ]" );
            oss << getPadding( 2 ) << "}\n" << getPadding( 1 ) << "}\n\n";

            generateEvaluateFunction( oss, getNumCards() );
            generateRankFunction( oss, getNumCards(), straightsAndFlushesDontCount, straightsAndFlushesCount, straightsAndFlushesDontCountLowBall, straightsAndFlushesCountLowBall );
            generateFooter( oss );
        }
    }
}
