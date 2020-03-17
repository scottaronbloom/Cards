/*
 * (c) Copyright 2004 - 2019 Blue Pearl Software Inc.
 * All rights reserved.
 *
 * This source code belongs to Blue Pearl Software Inc.
 * It is considered trade secret and confidential, and is not to be used
 * by parties who have not received written authorization
 * from Blue Pearl Software Inc.
 *
 * Only authorized users are allowed to use, copy and modify
 * this software provided that the above copyright notice
 * remains in all copies of this software.
 *
 *
 * $Author: scott $ - $Revision: 56205 $ - $Date: 2019-11-21 10:23:08 -0800 (Thu, 21 Nov 2019) $
 * $HeadURL: http://bpsvn/svn/trunk/CoreApp/dr/UnitTests/Test.cpp $
 *
 *
*/
#include "dr/drDataValue.h"
#include "Licensing/MockLicensing.h"
#include "dr/drModule.h"
#include "dr/drNet.h"
#include "dr/drSeqElement.h"
#include "dr/drBinary.h"
#include "dr/drMux.h"
#include "dr/drPin.h"
#include "dr/drUnary.h"
#include "dr/drGate.h"
#include "dr/drTristate.h"
#include "dr/drTristateBus.h"
#include "dr/drConcatenate.h"
#include "dr/drShifter.h"
#include "dr/drCmp.h"
#include "dr/drEquality.h"
#include "dr/drMemoryPort.h"
#include "dr/drPort.h"
#include "dr/drDot.h"
#include "dr/drNetTap.h"
#include "dr/drInstance.h"
#include "dr/drObjectIterators.h"
#include "dr/drInstance.h"
#include "dr/drNetIterators.h"
#include "drUtils/drConnectionMgr.h"
#include "dr/drUnary.h"
#include "dr/drGroup.h"
#include "Constraints/MockConstraints.h"
#include "UnitTestLib/UnitTestLib.h"
#include "Shared/ModInfoDB/StubModInfoDB.h"
#include "ClockManager/MockClockInfo.h"
#include "CoreApp/TokenMgr/StubToken.h"


#include <string>
#include <memory>
#include "gtest/gtest.h"

namespace 
{
    TEST(TestDrGroup, basic ) {
        drGroup< drNet * > g1;
        EXPECT_EQ( 0, g1.size() );

        drNetObject net1( "net1", nullptr );
        drNetObject net2( "net2", nullptr );
        g1.insert( &net1 );
        g1.insert( &net2 );

        auto pos = g1.find( &net1 );
        EXPECT_TRUE( pos != g1.end() );
        EXPECT_EQ( std::string( "net1" ), (*pos)->getName() );

        pos = g1.find( "net1" );
        EXPECT_TRUE( pos != g1.end() );
        EXPECT_EQ( std::string( "net1" ), (*pos)->getName() );

        pos = g1.find( "NeT1" );
        ASSERT_TRUE( pos != g1.end() );
        EXPECT_EQ( std::string( "net1" ), (*pos)->getName() );


        drGroup< drNet * > g2;
        drNetObject net3( "net3", nullptr );
        drNetObject net4( "net4", nullptr );

        g2.insert( &net3 );
        g2.insert( &net4 );
        EXPECT_EQ( 2, g1.size() );
        EXPECT_EQ( 2, g2.size() );

        g2 += g1;
        EXPECT_EQ( 4, g2.size() );
        pos = g2.find( &net2 );
        ASSERT_TRUE( pos != g2.end() );
        EXPECT_EQ( std::string( "net2" ), (*pos)->getName() );

        pos = g2.find( "net2" );
        ASSERT_TRUE( pos != g2.end() );
        EXPECT_EQ( std::string( "net2" ), (*pos)->getName() );

        pos = g2.find( "NeT2" );
        ASSERT_TRUE( pos != g2.end() );
        EXPECT_EQ( std::string( "net2" ), (*pos)->getName() );

        pos = g2.find( "NeT6" );
        ASSERT_TRUE( pos == g2.end() );
    }

    TEST(TestDrDataValue, IntVal10Behavior) {
        drDataValue dv1;
        dv1.setDecimalValue(10);

        EXPECT_EQ(10, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(10.0, dv1.getRealValue());
        EXPECT_EQ("10", dv1.getValueString( true ));
        EXPECT_EQ( "10", dv1.getValueString( false ) );
        EXPECT_TRUE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_FALSE(dv1.isReal());
        EXPECT_FALSE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(32, dv1.getNumBits()); // Is this still true in 64-bit?
    }
    TEST(TestDrDataValue, NegIntBehavior) {
        drDataValue dv0;
        dv0.setDecimalValue(10);
        drDataValue dv1 = dv0.signedNegation();

        EXPECT_EQ(-10, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(-10.0, dv1.getRealValue());
        EXPECT_EQ("-10", dv1.getValueString( true ));
        EXPECT_EQ( "-10", dv1.getValueString( false ) );
        EXPECT_TRUE(dv1.isInteger());
        EXPECT_TRUE(dv1.isNegative());
        EXPECT_FALSE(dv1.isReal());
        EXPECT_FALSE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(32, dv1.getNumBits()); // Is this still true in 64-bit?
    }
    TEST(TestDrDataValue, RealVal10Behavior) {
        drDataValue dv1;
        dv1.setRealValue(10.0);

        EXPECT_EQ(10, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(10.0, dv1.getRealValue());
        EXPECT_EQ("10", dv1.getValueString( true ));
        EXPECT_EQ( "10", dv1.getValueString( false ) );
        EXPECT_FALSE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_TRUE(dv1.isReal());
        EXPECT_FALSE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(64, dv1.getNumBits());
    }

    TEST(TestDrDataValue, RealVal101Behavior) {
        drDataValue dv1;
        dv1.setRealValue(10.1);

        EXPECT_EQ(10, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(10.1, dv1.getRealValue());
        EXPECT_EQ("10.1", dv1.getValueString( true ));
        EXPECT_EQ( "10.1", dv1.getValueString( false ) );
        EXPECT_FALSE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_TRUE(dv1.isReal());
        EXPECT_FALSE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(64, dv1.getNumBits());
    }

    TEST(TestDrDataValue, StringBehavior) {
        // setStringValue does *NOT* have the properties I expected!  It sets a binary value.
        drDataValue dv1;
        dv1.setStringValue("10");

        EXPECT_EQ(12592, dv1.getDecimalValue()); // = 49*256 + 48 - binary coding of string (48, 49 are dec of '0', '1')
        EXPECT_DOUBLE_EQ(12592.0, dv1.getRealValue());
        EXPECT_EQ( "\"00110001" "00110000\"", dv1.getValueString( true ) );
        EXPECT_EQ("16'b00110001" "00110000", dv1.getValueString( false ));
        EXPECT_FALSE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_FALSE(dv1.isReal());
        EXPECT_TRUE(dv1.is4ValueData());
        EXPECT_TRUE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(16, dv1.getNumBits());
    }
    TEST(TestDrDataValue, BoolStringBehavior) {
        // Sets a (4-valued) binary value based on the string contents (taken from "01XZ")
        // This used to be the same as setStringValue
        drDataValue dv1;
        dv1.setBinaryValue("10");

        EXPECT_EQ(2, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(2.0, dv1.getRealValue());
        EXPECT_EQ("\"10\"", dv1.getValueString( true ));
        EXPECT_EQ( "2'b10", dv1.getValueString( false ) );
        EXPECT_FALSE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_FALSE(dv1.isReal());
        EXPECT_TRUE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(2, dv1.getNumBits());
    }

    TEST(TestDrDataValue, BoolStringBehavior2) {
        // Sets a (4-valued) binary value based on the string contents (taken from "01XZ")
        // This used to be the same as setStringValue
        drDataValue dv1;
        dv1.setBinaryValue("010");

        EXPECT_EQ(2, dv1.getDecimalValue());
        EXPECT_DOUBLE_EQ(2.0, dv1.getRealValue());
        EXPECT_EQ("\"010\"", dv1.getValueString( true ));
        EXPECT_EQ( "3'b010", dv1.getValueString( false ) );
        EXPECT_FALSE(dv1.isInteger());
        EXPECT_FALSE(dv1.isNegative());
        EXPECT_FALSE(dv1.isReal());
        EXPECT_TRUE(dv1.is4ValueData());
        EXPECT_FALSE(dv1.isString());
        EXPECT_TRUE(dv1.hasSetValue());
        EXPECT_FALSE(dv1.hasXZ());
        EXPECT_FALSE(dv1.hasZ());
        EXPECT_EQ(3, dv1.getNumBits());
    }

    TEST( TestDrDataValue, TestLargeIntegerValues ) {

        drDataValue dv1;
        int64_t val = ( 1ULL << 32 );
        dv1.setDecimalValue( val, 64 );
        EXPECT_EQ( val, dv1.getDecimalValue() );

        double val2 = val;
        EXPECT_EQ( val2, dv1.getRealValue() );

        drDataValue dv2;
        dv2.setDecimalValue( 0, 71 );

        std::string value = "00000000000000000000000000000000000000000000000000000000000000000000000";
        EXPECT_EQ( 71, value.length() );
        drDataValue dv3;
        dv3.setBinaryValue( value );

        EXPECT_EQ( dv2.getValueStringForAllWords( true ), dv3.getValueStringForAllWords( true ) );
        EXPECT_EQ( dv2.getValueStringForAllWords( false ), dv3.getValueStringForAllWords( false ) );

        drDataValue dv4;
        val = 15;
        dv4.setDecimalValue( val, 5 );
        EXPECT_EQ( val, dv4.getDecimalValue() );
        auto str = dv4.getValueStringForAllWords( false );
        EXPECT_TRUE( "32'bXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX01111" );

        str = dv4.getValueStringForAllWords( true );
        EXPECT_TRUE( "\"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX01111\"" );

        val = -1;
        dv4.setDecimalValue( val, 5 );
        EXPECT_EQ( val, dv4.getDecimalValue() );
        str = dv4.getValueStringForAllWords( false );
        EXPECT_TRUE( "32'bXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX11111" );

        str = dv4.getValueStringForAllWords( true );
        EXPECT_TRUE( "\"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX11111\"" );

        value = "10000000000000000000000000000000000000000000000000000000000000000000000";
        EXPECT_EQ( 71, value.length() );
        drDataValue dv6;
        dv6.setBinaryValue( value );
        str = dv6.getValueStringForAllWords( false );
        EXPECT_EQ( "96'bXXXXXXXXXXXXXXXXXXXXXXXXX10000000000000000000000000000000000000000000000000000000000000000000000", str );

        str = dv6.getValueStringForAllWords( true );
        EXPECT_EQ( "\"XXXXXXXXXXXXXXXXXXXXXXXXX10000000000000000000000000000000000000000000000000000000000000000000000\"", str );
    }

    class TestDrNet : public drNetObject
    {
    public:
        TestDrNet( const std::string & netName ) :
          drNetObject( netName, Verific::LineFile::EncodeLineFile( "fileName", 1 ) )
        {
        }

        void setObjectID( TObjID id )
        {
            drNetObject::setObjectID( id );
        }
    };

    TEST( TestDrNetUnassigned, TestDrNetUnassigned )
    {
        TestDrNet net( "netname" );
        net.setObjectID( UNASSIGNED );
        EXPECT_EQ( UNASSIGNED, net.getObjectID() );
    }

    TEST(TestDrFactory, Basic)
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );
            if ( ii == EObjectType::eSYNTHBUF )
                EXPECT_EQ( EObjectType::eBUF, obj->getObjectType() ) << obj->getClassName() << " failed to create a correct object type";
            else
                EXPECT_EQ( ii, obj->getObjectType() ) << obj->getClassName() << " failed to create a correct object type";
            delete obj;
        }
    }

    void checkIsSequential( EObjectType ii, drObject * obj )
    {
        switch( ii )
        {
        case EObjectType::eDFFSR:
        case EObjectType::eDFFRS:
        case EObjectType::eDFFR:
        case EObjectType::eDFFS:
        case EObjectType::eDFF:

        case EObjectType::eLATCHSR:
        case EObjectType::eLATCHRS:
        case EObjectType::eLATCHR:
        case EObjectType::eLATCHS:
        case EObjectType::eLATCH: 
            EXPECT_TRUE( obj->isSequentialElement() );
            break;
        default:
            EXPECT_FALSE( obj->isSequentialElement() );
            break;
        }
    }

    TEST( TestDrFactory, IsSequential )
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );

            checkIsSequential( ii, obj );
            if (HasFatalFailure())
                return;
            delete obj;
        }
    }

    void checkPins( drObject * obj )
    {
        for( TObjID ii = 1; ii <= obj->getNumInputs(); ++ii )
        {
            drPin * pin = obj->getInputPinFromOrder( ii );
            ASSERT_TRUE( pin != nullptr );
            EXPECT_TRUE( pin->isReceiver() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Input Pin: " << ii;
        }

        for( TObjID ii = 1; ii <= obj->getNumOutputs(); ++ii )
        {
            drPin * pin = obj->getOutputPinFromOrder( ii );
            ASSERT_TRUE( pin != nullptr );
            EXPECT_TRUE( pin->isDriver() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Output Pin: " << ii;
        }

        for( TObjID ii = 1; ii <= obj->getNumPins(); ++ii )
        {
            drPin * pin = obj->getPinFromOrder( ii );
            ASSERT_TRUE( pin != nullptr );
            if ( ii <= obj->getNumInputs() )
                EXPECT_TRUE( pin->isReceiver() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Input Pin: " << ii;
            else
                EXPECT_TRUE( pin->isDriver() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Output Pin: " << ii;
        }
    }

    TEST( TestDrFactory, PinTypes )
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );

            drObjPinIterator pIter( obj );
            int pinIdx = 1; // Start from 1
            for( drPin* pin = pIter.first(); pin; pin = pIter.next() )
            {
                if ( ii == EObjectType::eTRISTATE_BUS || ii == EObjectType::eDOT || ii == EObjectType::eCONCAT )
                {
                    EXPECT_TRUE( isUnassigned( pin->getPinID( false ) ) );
                    continue;
                }
                if ( isUnassigned( pin->getPinID( false ) ) )
                {
                    EXPECT_TRUE( false ) << obj->getClassName() << " " << pin->getName();
                    continue;
                }

                EXPECT_EQ( pinIdx, pin->getPinID() ) << obj->getClassName() << " " << pin->getName();
                pinIdx++;
            }

            delete obj;
        }
    }

    TEST( TestDrFactory, PintId )
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );

            if (HasFatalFailure())
                return;

            delete obj;
        }
    }

    void checkClone( drObject * obj, drObject * clone )
    {
        EXPECT_EQ( obj->getName(), clone->getName() );
        EXPECT_EQ( obj->getObjectType(), clone->getObjectType() );
        EXPECT_EQ( obj->getNumPins(), clone->getNumPins() );
        for( TObjID ii = 1; ii <= clone->getNumPins(); ++ii )
        {
            drPin * pin = obj->getPinFromOrder( ii );
            drPin * clonePin = clone->getPinFromOrder( ii );
            ASSERT_TRUE( pin != nullptr );
            ASSERT_TRUE( clonePin != nullptr );
            EXPECT_NE( pin, clonePin )<< NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;;
            EXPECT_EQ( pin->getName(), clonePin->getName() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii; ;
            EXPECT_EQ( pin->getPinType(), clonePin->getPinType() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( pin->getStartIndex(), clonePin->getStartIndex() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( pin->getEndIndex(), clonePin->getEndIndex() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( obj, pin->getObject() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( clone, clonePin->getObject() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
        }
    }

    TEST( TestDrFactory, Clone )
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );
            drObject * clone = obj->clone();
            checkClone( obj, clone );
            if (HasFatalFailure())
                return;
            delete clone;
            delete obj;
        }
    }
    void checkCloneSize( drObject * obj, drObject * clone )
    {
        EXPECT_EQ( obj->getNumPins(), clone->getNumPins() );
        for( TObjID ii = 1; ii <= clone->getNumPins(); ++ii )
        {
            drPin * pin = obj->getPinFromOrder( ii );
            drPin * clonePin = clone->getPinFromOrder( ii );
            ASSERT_TRUE( pin != nullptr );
            ASSERT_TRUE( clonePin != nullptr );
            EXPECT_NE( pin, clonePin ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;;
            EXPECT_EQ( pin->getName(), clonePin->getName() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii; ;
            EXPECT_EQ( pin->getPinType(), clonePin->getPinType() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( pin->getStartIndex(), clonePin->getStartIndex() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( pin->getEndIndex(), clonePin->getEndIndex() )<< NUnitTestLib::getCurrentTestName()  << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( obj, pin->getObject() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
            EXPECT_EQ( clone, clonePin->getObject() ) << NUnitTestLib::getCurrentTestName() << "Object: " << obj->getClassName() << " Pin: " << ii;
        }
    }

    TEST( TestDrFactory, CloneSize )
    {
        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            drObject * obj = drObject::factory( ii );
            for( TObjID ii = 1; ii <= obj->getNumPins(); ++ii )
            {
                drPin * pin = obj->getPinFromOrder( ii );
                ASSERT_TRUE( pin != nullptr );
                pin->setStartIndex( 31 );
                pin->setEndIndex( 0 );
                EXPECT_EQ( 32, pin->getNumBits() );
            }
            drObject * clone = obj->clone();

            checkCloneSize( obj, clone );

            delete clone;
            delete obj;
        }
    }

    template< typename T >
    void runTestObj( T & obj1  )
    {
        SCOPED_TRACE("Run Test Object");
        checkPins( &obj1 );
        if (::testing::Test::HasFatalFailure())
            return;
        checkIsSequential( obj1.getObjectType(), &obj1 );
        if (::testing::Test::HasFatalFailure())
            return;
    }

    template< typename T >
    void runTestCopy( T & obj1  )
    {
        SCOPED_TRACE("Run Test Copy");
        T obj2( obj1 );
        checkClone( &obj1, &obj2 );
        if (::testing::Test::HasFatalFailure())
            return;
        checkCloneSize( &obj1, &obj2 );
        if (::testing::Test::HasFatalFailure())
            return;
    }

    template< typename T >
    void runTestAssignment( T & obj1  )
    {
        SCOPED_TRACE("Run Test Assignment");
        T obj3( "tmp", nullptr );
        obj3 = obj1;
        checkClone( &obj1, &obj3 );
        if (::testing::Test::HasFatalFailure())
            return;
        checkCloneSize( &obj1, &obj3 );
        if (::testing::Test::HasFatalFailure())
            return;
    }

    template< typename T >
    void runTest( T & obj1, std::map< EObjectType, size_t > & types )
    {
        types[ obj1.getObjectType() ]++;

        runTestObj( obj1 );
        if (::testing::Test::HasFatalFailure())
            return;
        runTestCopy( obj1 );
        if (::testing::Test::HasFatalFailure())
            return;
        runTestAssignment( obj1 );
        if (::testing::Test::HasFatalFailure())
            return;
    }

    template< typename T >
    void testCopyAndAssign( std::map< EObjectType, size_t > & types )
    {
        T obj1( "i1", nullptr );
        runTest( obj1, types );
    }

    void testDFF( std::map< EObjectType, size_t > & types )
    {
        std::unique_ptr< drDff > dff( dynamic_cast< drDff * >( drObject::factory( EObjectType::eDFF ) ) );
        runTest( *dff, types );
        dff = std::unique_ptr< drDff >( dynamic_cast< drDff * >( drObject::factory( EObjectType::eDFFR ) ) );
        runTest( *dff, types );
        dff = std::unique_ptr< drDff >( dynamic_cast< drDff * >( drObject::factory( EObjectType::eDFFS ) ) );
        runTest( *dff, types );
        dff = std::unique_ptr< drDff >( dynamic_cast< drDff * >( drObject::factory( EObjectType::eDFFRS ) ) );
        runTest( *dff, types );
        dff = std::unique_ptr< drDff >( dynamic_cast< drDff * >( drObject::factory( EObjectType::eDFFSR ) ) );
        runTest( *dff, types );
    }

    void testLatch( std::map< EObjectType, size_t > & types )
    {
        std::unique_ptr< drLatch > latch( dynamic_cast< drLatch * >( drObject::factory( EObjectType::eLATCH ) ) );
        runTest( *latch, types );
        latch = std::unique_ptr< drLatch >( dynamic_cast< drLatch * >( drObject::factory( EObjectType::eLATCHR ) ) );
        runTest( *latch, types );
        latch = std::unique_ptr< drLatch >( dynamic_cast< drLatch * >( drObject::factory( EObjectType::eLATCHS ) ) );
        runTest( *latch, types );
        latch = std::unique_ptr< drLatch >( dynamic_cast< drLatch * >( drObject::factory( EObjectType::eLATCHRS ) ) );
        runTest( *latch, types );
        latch = std::unique_ptr< drLatch >( dynamic_cast< drLatch * >( drObject::factory( EObjectType::eLATCHSR ) ) );
        runTest( *latch, types );
    }

    TEST( TestDrObjects, AssignAndCopy )
    {
        std::map< EObjectType, size_t > types;

        testCopyAndAssign< drBuf >( types );
        testCopyAndAssign< drSynthBuf >( types );
        testCopyAndAssign< drInversion >( types );
        testCopyAndAssign< drAnd >( types );
        testCopyAndAssign< drOr >( types );
        testCopyAndAssign< drNand >( types );
        testCopyAndAssign< drNor >( types );
        testCopyAndAssign< drXor >( types );
        testCopyAndAssign< drXnor >( types );
        testCopyAndAssign< drUnaryAnd >( types );
        testCopyAndAssign< drUnaryOr >( types );
        testCopyAndAssign< drUnaryNand >( types );
        testCopyAndAssign< drUnaryNor >( types );
        testCopyAndAssign< drUnaryXor >( types );
        testCopyAndAssign< drUnaryXnor >( types );
        testCopyAndAssign< drLogicalNegate >( types );
        testCopyAndAssign< drTristate >( types );
        testCopyAndAssign< drTristateBus >( types );
        testCopyAndAssign< drDot >( types );
        testCopyAndAssign< drConcatenate >( types );
        testCopyAndAssign< drNetTap >( types );
        testCopyAndAssign< drMux >( types );
        testCopyAndAssign< drAdder >( types );

        testCopyAndAssign< drSubtractor >( types );
        testCopyAndAssign< drMultiplier >( types );
        testCopyAndAssign< drDivider >( types );
        testCopyAndAssign< drModulus >( types );
        testCopyAndAssign< drLeftShift >( types );
        testCopyAndAssign< drRightShift >( types );
        testCopyAndAssign< drCmp >( types );
        testCopyAndAssign< drEqualOp >( types );

        testCopyAndAssign< drNotEqualOp >( types );
        testCopyAndAssign< drLessThanOp >( types );
        testCopyAndAssign< drLessThanEqualOp >( types );
        testCopyAndAssign< drDff >( types );
        testDFF( types );
        testCopyAndAssign< drLatch >( types );
        testLatch( types );
        testCopyAndAssign< drInstance >( types );
        testCopyAndAssign< drMemoryWritePort >( types );
        testCopyAndAssign< drMemoryReadPort >( types );


        for( EObjectType ii = static_cast< EObjectType >( static_cast< int >( EObjectType::eNONE )+1 ); ii < EObjectType::eLAST_TYPE; ii = static_cast< EObjectType >( static_cast< int >( ii ) + 1 ) )
        {
            std::map< EObjectType, size_t >::iterator pos = types.find( ii );
            size_t correctNum = 1;
            if ( ii == EObjectType::eBUF )
                correctNum = 2;
            else if ( ii == EObjectType::eSYNTHBUF )
                correctNum = 0;
            else if ( ii == EObjectType::eDFF )
                correctNum = 2;
            else if ( ii == EObjectType::eLATCH )
                correctNum = 2;

            if ( correctNum == 0 )
            {
                EXPECT_TRUE( pos == types.end() ) << "drObject: " << static_cast< int >( ii ) << " " << drObject::getObjectTypeName( ii, false ) << " was tested and should not have been";
            }
            else
            {
                EXPECT_TRUE( pos != types.end() ) << "drObject: " << static_cast< int >( ii ) << " " << drObject::getObjectTypeName( ii, false ) << " was not tested";
                EXPECT_EQ( correctNum, ( *pos ).second ) << "drObject: " << static_cast< int >( ii ) << " " << drObject::getObjectTypeName( ii, false ) << " was not tested correctly";
            }
            if ( pos != types.end() )
                types.erase( pos );
        }
    }


    TEST( TestDrObjects, TestNetAttributes )
    {
        drNetObject * net = new drNetObject( "net", nullptr );
        net->addAttribute( "period", "\"10\"" );
        net->addAttribute( "period2", "10" );
        bool found;

        std::string value;
        value = net->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = net->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        drNet * net2 = net->clone();

        value = net2->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = net2->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        delete net;
        delete net2;
    }

    // actually testing drNode
    TEST( TestDrObjects, TestPortAttributes )
    {
        drPort * port = new drPort( "port", nullptr, EPortPinDirection::eINPUT );
        port->addAttribute( "period", "\"10\"" );
        port->addAttribute( "period2", "10" );
        bool found;

        std::string value;
        value = port->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = port->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        drNode * port2 = port->clone();

        value = port2->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = port2->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        delete port;
        delete port2;
    }

    class drTestInstance : public drInstance
    {
    public:

        drTestInstance( const std::string & name ) :
            drInstance( name, nullptr )
        {
        }
        virtual void setBeenDumped( bool beenDumped ) const override{ drInstance::setBeenDumped( beenDumped ); }
        virtual drObject * clone() const override
        {
            return new drTestInstance( *this );
        }

    };

    // actually testing drNode
    TEST( TestDrObjects, TestInstAttributes )
    {
        auto inst = new drTestInstance( "inst" );
        inst->addAttribute( "period", "\"10\"" );
        inst->addAttribute( "period2", "10" );
        EXPECT_FALSE( inst->beenDumped() );

        inst->setBeenDumped( true );
        EXPECT_TRUE( inst->beenDumped() );
        inst->setBeenDumped( false );
        EXPECT_FALSE( inst->beenDumped() );
        inst->setBeenDumped( true );

        EXPECT_FALSE( inst->isSynthesized() );
        inst->setIsSynthesized( true );
        EXPECT_TRUE( inst->isSynthesized() );
        inst->setIsSynthesized( false );
        EXPECT_FALSE( inst->isSynthesized() );
        inst->setIsSynthesized( true );

        bool found;
        std::string value;
        value = inst->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = inst->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        auto inst2 = dynamic_cast< drTestInstance * >( inst->clone() );

        EXPECT_TRUE( inst2->isSynthesized() );
        EXPECT_TRUE( inst2->beenDumped() );
        inst2->setIsSynthesized( false );
        inst2->setBeenDumped( false );
        EXPECT_FALSE( inst2->isSynthesized() );
        EXPECT_FALSE( inst2->beenDumped() );
        EXPECT_TRUE( inst->isSynthesized() );
        EXPECT_TRUE( inst->beenDumped() );


        value = inst2->getAttribute( "period", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        value = inst2->getAttribute( "period2", &found );
        EXPECT_TRUE( found );
        EXPECT_EQ( "10", value );

        delete inst;
        delete inst2;
    }

    TEST( TestDrObjects, TestName )
    {
        drInstance * inst = new drInstance( "inst", nullptr );
        EXPECT_EQ( "inst", inst->getName() );
        int id = inst->getNameID();
        inst->setName( "foobar" );
        EXPECT_EQ( "foobar", inst->getName() );

        drInstance * inst2 = new drInstance( "inst", nullptr );
        EXPECT_EQ( "inst", inst2->getName() );
        EXPECT_EQ( id, inst2->getNameID() );
        
        drObject * inst3 = inst2->clone();
        EXPECT_EQ( "inst", inst2->getName() );
        EXPECT_EQ( id, inst3->getNameID() );

        delete inst;
        delete inst2;
        delete inst3;
    }

    TEST( TestDrObjects, TestPortWithUnassignedNodeID )
    {
        drPort * outPort = new drPort( "out", nullptr, EPortPinDirection::eOUTPUT, 0, 0 );
        outPort->setObjectID( 1 );
        outPort->setNodeID( UNASSIGNED );
        EXPECT_TRUE( isUnassigned( outPort->getNodeID() ) );
        EXPECT_EQ( 1, outPort->getObjectID() );
        EXPECT_EQ( 1, outPort->getObjectID() );

        drPort * inPort = new drPort( "in", nullptr, EPortPinDirection::eINPUT, 0, 0 );
        inPort->setObjectID( 2 );
        inPort->setNodeID( UNASSIGNED );
        EXPECT_TRUE( isUnassigned( inPort->getNodeID() ) );
        EXPECT_EQ( 2, inPort->getObjectID() );
        EXPECT_EQ( 2, inPort->getObjectID() );

        drNetObject * net = new drNetObject( "out", nullptr );
        net->setObjectID( 1 );
        EXPECT_TRUE( !isUnassigned( net->getObjectID() ) );
        EXPECT_EQ( 1, net->getObjectID() );
        EXPECT_EQ( 1, net->getObjectID() );
        
        drConnectionMgr::instance()->connectNodeNet(outPort, net);
        drConnectionMgr::instance()->connectNodeNet(inPort, net);

        drNode * reciever = net->getFirstReceiverNodeIfValid();
        EXPECT_TRUE( reciever == nullptr );

        drNetSinkIterator iter( net );
        drNode* node = iter.first();
        EXPECT_EQ( node, outPort );

        drNode * driver = net->getFirstDriverNodeIfValid();
        EXPECT_TRUE( driver == nullptr );
        drNetSourceIterator iter2( net );
        drNode* node2 = iter2.first();
        EXPECT_EQ( node2, inPort );
    }

    TEST( TestDrObjects, TestDrBuf )
    {
        drBuf::setFinishedCreatingDBBufs( false );
        drBuf * buf1 = drBuf::createBuf( "buf1", nullptr, false );
        EXPECT_FALSE( buf1->isSynthBuf() );
        drBuf::setFinishedCreatingDBBufs( true );
        drBuf * buf2 = drBuf::createBuf( "buf2", nullptr, false );
        EXPECT_TRUE( buf2->isSynthBuf() );
        delete buf1;
        delete buf2;
    }

    TEST( TestDrObjects, TestDrBufChain )
    {
        drBuf::setFinishedCreatingDBBufs( false );
        drDff * driverDff = new drDff( "dff1", nullptr );
        drBuf * buf1 = drBuf::createBuf( "buf1", nullptr, false );
        drBuf * buf2 = drBuf::createBuf( "buf2", nullptr, false );
        drBuf::setFinishedCreatingDBBufs( true );
        drBuf * buf3 = drBuf::createBuf( "buf3", nullptr, false );
        drBuf * buf4 = drBuf::createBuf( "buf4", nullptr, false );
        drDff * receiverDFF = new drDff( "dff2", nullptr );

        drNet * net1 = new drNetObject( "n1", nullptr );
        driverDff->setQNet( net1 );
        buf1->setInput( net1 );

        drNet * net2 = new drNetObject( "n2", nullptr );
        buf1->setOutput( net2 );
        buf2->setInput( net2 );

        drNet * net3 = new drNetObject( "n3", nullptr );
        buf2->setOutput( net3 );
        buf3->setInput( net3 );

        drNet * net4 = new drNetObject( "n4", nullptr );
        buf3->setOutput( net4 );
        buf4->setInput( net4 );

        drNet * net5 = new drNetObject( "n5", nullptr );
        buf4->setOutput( net5 );
        receiverDFF->setDataNet( net5 );

        EXPECT_EQ( receiverDFF, receiverDFF->getNonSynthBufDriver() );
        EXPECT_EQ( buf2, buf4->drObject::getNonSynthBufDriver() );
        EXPECT_EQ( buf2, buf3->drObject::getNonSynthBufDriver() );
        EXPECT_EQ( buf2, buf2->drObject::getNonSynthBufDriver() );
        EXPECT_EQ( buf1, buf1->drObject::getNonSynthBufDriver() );
        EXPECT_EQ( driverDff, driverDff->getNonSynthBufDriver() );

        EXPECT_EQ( receiverDFF, receiverDFF->getNonSynthUnbufferedReceiver() );
        EXPECT_EQ( receiverDFF, buf4->getNonSynthUnbufferedReceiver() );
        EXPECT_EQ( receiverDFF, buf3->getNonSynthUnbufferedReceiver() );
        EXPECT_EQ( buf2, buf2->getNonSynthUnbufferedReceiver() );
        EXPECT_EQ( buf1, buf1->getNonSynthUnbufferedReceiver() );
        EXPECT_EQ( driverDff, driverDff->getNonSynthUnbufferedReceiver() );
    }

    class TestLookup : public drLookup<TestDrNet *>
    {
    public:
        TObjID compress()
        {
            return drLookup::compress();
        }

        size_t operator[] ( TestDrNet * net ) const
        {
            auto pos = fObjToIdxMap.find( net );
            if ( pos == fObjToIdxMap.end() )
                return UNASSIGNED;
            return ( *pos ).second;
        }

        TestDrNet * operator[] ( size_t pos ) const
        {
            return drLookup::operator[]( pos );
        }

        size_t indexOf( TestDrNet * net )const
        {
            for ( size_t ii = 0; ii < fVector.size(); ++ii )
            {
                if ( ( fVector[ ii ] == net ) && ( net->getObjectID() == ii ) )
                {
                    return ii;
                }
            }
            return UNASSIGNED;
        }

        bool remove( TestDrNet * obj ){ return drLookup::remove( obj ); }

    };

#define VECTORSIZE 500
    TEST( TestLookup, TestDrNetCompress )
    {
        TestLookup lookup;
        EXPECT_FALSE( lookup.compress() );
        EXPECT_EQ( 0, lookup.size() );

        TestDrNet * net1 = new TestDrNet( "net1" );
        lookup.add( net1 );
        net1->setObjectID( UNASSIGNED );

        EXPECT_TRUE( lookup.compress() );
        EXPECT_EQ( 0, lookup.size() );

        for ( TObjID ii = 0; ii < VECTORSIZE; ++ii )
        {
            std::ostringstream oss;
            oss << "net" << ii;
            TestDrNet * net = new TestDrNet( oss.str() );
            net->setObjectID( lookup.add( net ) );
        }
        EXPECT_EQ( VECTORSIZE, lookup.size() );
        EXPECT_FALSE( lookup.compress() );
        for ( size_t ii = 0; ii < lookup.size(); ++ii )
        {
            if ( ii % 2 == 0 )
                lookup[ ii ]->setObjectID( UNASSIGNED );
        }
        for ( size_t ii = 0; ii < lookup.size(); ++ii )
        {
            auto net = lookup[ ii ];
            if ( ii % 2 == 0 )
                EXPECT_TRUE( isUnassigned( net->getObjectID() ) );
            else
                EXPECT_EQ( ii, net->getObjectID() );
        }

        EXPECT_EQ( VECTORSIZE, lookup.size() );
        EXPECT_TRUE( lookup.compress() );
        EXPECT_EQ( VECTORSIZE / 2, lookup.size() );
        for ( int ii = 0; ii < VECTORSIZE / 2; ++ii )
        {
            auto net = lookup[ ii ];
            EXPECT_EQ( ii, net->getObjectID() );

            EXPECT_TRUE( lookup[ net ] == ii );
            EXPECT_TRUE( lookup.indexOf( net ) == ii );
        }

    }

    std::string getID( int num )
    {
        static size_t len = 0;
        if ( len == 0 )
        {
            size_t tmp = VECTORSIZE;
            while ( tmp != 0 )
            {
                tmp /= 10;
                len++;
            }
        }

        std::ostringstream oss;
        oss << std::string( len - oss.str().length(), '0' ) << num;
        return oss.str();
    }
}  // namespace


int main( int argc, char **argv ) {
    ::testing::InitGoogleTest( &argc, argv );
    int retVal = RUN_ALL_TESTS();
    return retVal;
}



