#ifndef __CARDTEST_H
#define __CARDTEST_H

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

enum class ECard;
class CGame;

#include <QString>
#include <string>
#include <iostream>
#include <tuple>
#include <list>
#include "gmock/gmock.h"

std::ostream& operator<<( std::ostream& os, const QString & data );

void PrintTo( const QString& data, std::ostream * os );

namespace NHandUtils
{
    class CCardInfo;
}

enum class EHand;
class CHand;
class CCard;
struct SPlayInfo;
namespace NHandTester
{
    class CHandTester : public ::testing::Test
    {
    protected:
        CHandTester();
        virtual ~CHandTester();

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        virtual void SetUp();
        virtual void TearDown();
        bool isStraight( std::vector< ECard > cards ) const;
        bool CompareHandOrder( const std::map< EHand, size_t > & freq, const NHandUtils::CCardInfo & cardInfo ) const;

        std::tuple< std::list< std::shared_ptr< NHandUtils::CCardInfo > >, std::map< EHand, size_t >, std::map< EHand, size_t > > getUniqueHands( std::list< std::shared_ptr< NHandUtils::CCardInfo > > & allHands );
        std::tuple< std::list< std::shared_ptr< CHand > >, std::map< EHand, size_t >, std::map< EHand, size_t > > getUniqueHands( std::list< std::shared_ptr< CHand > >& allHands );


        std::list< std::shared_ptr< NHandUtils::CCardInfo > > getAllCardInfoHands( size_t numCards );
        std::list< std::shared_ptr< CHand > > getAllCHandHands( size_t numCards );
    public:
        std::vector< std::vector< std::shared_ptr< CCard > > > getAllCards( size_t numCards );
        CGame* fGame{ nullptr };
    };
}  

#endif
