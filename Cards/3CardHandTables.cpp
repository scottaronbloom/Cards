#include "Evaluate3CardHand.h"
#include "PlayInfo.h"
#include "Hand.h"

#include <map>

//#define __USECARDMAPS

namespace NHandUtils
{
    SCardInfoData C3CardInfo::sCardInfoData = {};

    void C3CardInfo::initMaps()
    {
        if ( !sCardInfoData.fTablesInitialized )
        {
            sCardInfoData.fTablesInitialized = true;
#ifdef __USECARDMAPS
            // Flushes/Straights Don't Count
            sCardInfoData.fCardMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ] =
            {
                 { { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 1 } // EHand::eThreeOfAKind
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 2 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 3 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 4 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 5 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 6 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 7 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 8 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 9 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 10 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 11 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 12 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 13 } // EHand::eThreeOfAKind
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 14 } // EHand::ePair
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 15 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 16 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 17 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 18 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 19 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 20 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 21 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 22 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 23 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 24 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 25 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 26 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 27 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 28 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 29 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 30 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 31 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 32 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 33 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 34 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 35 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 36 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 37 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 38 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 39 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 40 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 41 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 42 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 43 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 44 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 45 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 46 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 47 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 48 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 49 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 50 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 51 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 52 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 53 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 54 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 55 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 56 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 57 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 58 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 59 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 60 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 61 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 62 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 63 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 64 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 65 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 66 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 67 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 68 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 69 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 70 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 71 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 72 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 73 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 74 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 75 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 76 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 77 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 78 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 79 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 80 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 81 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 82 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 83 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 84 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 85 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 86 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 87 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 88 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 89 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 90 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 91 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 92 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 93 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 94 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 95 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 96 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 97 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 98 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 99 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 100 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 101 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 102 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 103 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 104 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 105 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 106 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 107 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 108 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 109 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 110 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 111 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 112 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 113 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 114 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 115 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 116 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 117 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 118 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 119 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 120 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 121 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 122 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 123 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 124 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 125 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 126 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 127 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 128 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 129 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 130 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 131 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 132 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 133 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 134 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 135 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 136 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 137 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 138 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 139 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 140 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 141 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 142 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 143 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 144 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 145 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 146 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 147 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 148 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 149 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 150 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 151 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 152 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 153 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 154 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 155 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 156 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 157 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 158 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 159 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 160 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 161 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 162 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 163 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 164 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 165 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 166 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 167 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 168 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 169 } // EHand::ePair
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 170 } // EHand::eHighCard
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 171 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 172 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 173 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 174 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 175 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 176 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 177 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 178 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 179 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 180 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 181 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 182 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 183 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 184 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 185 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 186 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 187 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 188 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 189 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 190 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 191 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 192 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 193 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 194 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 195 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 196 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 197 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 198 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 199 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 200 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 201 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 202 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 203 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 204 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 205 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 206 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 207 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 208 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 209 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 210 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 211 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 212 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 213 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 214 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 215 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 216 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 217 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 218 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 219 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 220 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 221 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 222 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 223 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 224 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 225 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 226 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 227 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 228 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 229 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 230 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 231 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 232 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 233 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 234 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 235 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 236 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 237 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 238 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 239 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 240 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 241 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 242 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 243 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 244 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 245 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 246 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 247 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 248 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 249 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 250 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 251 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 252 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 253 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 254 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 255 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 256 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 257 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 258 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 259 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 260 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 261 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 262 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 263 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 264 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 265 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 266 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 267 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 268 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 269 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 270 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 271 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 272 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 273 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 274 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 275 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 276 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 277 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 278 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 279 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 280 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 281 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 282 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 283 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 284 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 285 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 286 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 287 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 288 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 289 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 290 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 291 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 292 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 293 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 294 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 295 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 296 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 297 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 298 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 299 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 300 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 301 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 302 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 303 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 304 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 305 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 306 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 307 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 308 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 309 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 310 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 311 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 312 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 313 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 314 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 315 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 316 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 317 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 318 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 319 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 320 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 321 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 322 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 323 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 324 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 325 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 326 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 327 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 328 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 329 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 330 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 331 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 332 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 333 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 334 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 335 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 336 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 337 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 338 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 339 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 340 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 341 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 342 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 343 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 344 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 345 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 346 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 347 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 348 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 349 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 350 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 351 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 352 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 353 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 354 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 355 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 356 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 357 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 358 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 359 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 360 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 361 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 362 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 363 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 364 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 365 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 366 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 367 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 368 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 369 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 370 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 371 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 372 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 373 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 374 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 375 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 376 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 377 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 378 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 379 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 380 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 381 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 382 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 383 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 384 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 385 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 386 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 387 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 388 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 389 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 390 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 391 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 392 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 393 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 394 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 395 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 396 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 397 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 398 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 399 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 400 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 401 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 402 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 403 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 404 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 405 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 406 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 407 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 408 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 409 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 410 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 411 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 412 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 413 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 414 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 415 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 416 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 417 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 418 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 419 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 420 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 421 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 422 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 423 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 424 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 425 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 426 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 427 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 428 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 429 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 430 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 431 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 432 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 433 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 434 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 435 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 436 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 437 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 438 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 439 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 440 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 441 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 442 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 443 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 444 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 445 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 446 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 447 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 448 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 449 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 450 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 451 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 452 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 453 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 454 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 455 } // EHand::eHighCard
            };

            // Flushes/Straights Count
            sCardInfoData.fCardMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ] =
            {
                 { { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 1 } // EHand::eStraightFlush
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 2 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 3 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 4 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 5 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 6 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 7 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 8 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 9 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 10 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 11 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 12 } // EHand::eStraightFlush
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 13 } // EHand::eThreeOfAKind
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 14 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 15 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 16 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 17 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 18 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 19 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 20 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 21 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 22 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 23 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 24 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 25 } // EHand::eThreeOfAKind
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 26 } // EHand::eStraight
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 27 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 28 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 29 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 30 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 31 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 32 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 33 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 34 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 35 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 36 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 37 } // EHand::eStraight
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 38 } // EHand::eFlush
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 39 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 40 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 41 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 42 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 43 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 44 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 45 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 46 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 47 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 48 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 49 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 50 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 51 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 52 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 53 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 54 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 55 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 56 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 57 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 58 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 59 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 60 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 61 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 62 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 63 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 64 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 65 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 66 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 67 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 68 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 69 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 70 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 71 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 72 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 73 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 74 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 75 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 76 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 77 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 78 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 79 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 80 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 81 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 82 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 83 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 84 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 85 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 86 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 87 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 88 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 89 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 90 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 91 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 92 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 93 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 94 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 95 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 96 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 97 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 98 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 99 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 100 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 101 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 102 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 103 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 104 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 105 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 106 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 107 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 108 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 109 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 110 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 111 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 112 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 113 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 114 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 115 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 116 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 117 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 118 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 119 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 120 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 121 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 122 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 123 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 124 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 125 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 126 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 127 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 128 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 129 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 130 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 131 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 132 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 133 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 134 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 135 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 136 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 137 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 138 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 139 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 140 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 141 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 142 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 143 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 144 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 145 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 146 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 147 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 148 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 149 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 150 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 151 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 152 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 153 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 154 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 155 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 156 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 157 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 158 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 159 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 160 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 161 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 162 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 163 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 164 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 165 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 166 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 167 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 168 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 169 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 170 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 171 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 172 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 173 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 174 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 175 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 176 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 177 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 178 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 179 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 180 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 181 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 182 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 183 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 184 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 185 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 186 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 187 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 188 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 189 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 190 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 191 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 192 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 193 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 194 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 195 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 196 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 197 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 198 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 199 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 200 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 201 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 202 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 203 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 204 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 205 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 206 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 207 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 208 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 209 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 210 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 211 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 212 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 213 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 214 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 215 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 216 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 217 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 218 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 219 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 220 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 221 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 222 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 223 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 224 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 225 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 226 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 227 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 228 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 229 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 230 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 231 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 232 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 233 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 234 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 235 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 236 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 237 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 238 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 239 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 240 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 241 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 242 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 243 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 244 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 245 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 246 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 247 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 248 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 249 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 250 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 251 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 252 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 253 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 254 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 255 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 256 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 257 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 258 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 259 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 260 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 261 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 262 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 263 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 264 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 265 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 266 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 267 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 268 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 269 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 270 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 271 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 272 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 273 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 274 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 275 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 276 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 277 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 278 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 279 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 280 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 281 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 282 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 283 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 284 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 285 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 286 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 287 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 288 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 289 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 290 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 291 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 292 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 293 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 294 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 295 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 296 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 297 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 298 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 299 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 300 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 301 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 302 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 303 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 304 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 305 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 306 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 307 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 308 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 309 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 310 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 311 } // EHand::eFlush
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 312 } // EHand::ePair
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 313 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 314 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 315 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 316 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 317 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 318 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 319 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 320 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 321 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 322 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 323 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 324 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 325 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 326 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 327 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 328 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 329 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 330 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 331 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 332 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 333 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 334 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 335 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 336 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 337 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 338 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 339 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 340 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 341 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 342 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 343 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 344 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 345 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 346 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 347 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 348 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 349 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 350 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 351 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 352 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 353 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 354 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 355 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 356 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 357 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 358 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 359 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 360 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 361 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 362 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 363 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 364 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 365 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 366 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 367 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 368 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 369 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 370 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 371 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 372 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 373 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 374 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 375 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 376 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 377 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 378 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 379 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 380 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 381 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 382 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 383 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 384 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 385 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 386 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 387 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 388 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 389 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 390 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 391 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 392 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 393 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 394 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 395 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 396 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 397 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 398 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 399 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 400 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 401 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 402 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 403 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 404 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 405 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 406 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 407 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 408 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 409 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 410 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 411 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 412 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 413 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 414 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 415 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 416 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 417 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 418 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 419 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 420 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 421 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 422 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 423 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 424 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 425 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 426 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 427 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 428 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 429 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 430 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 431 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 432 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 433 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 434 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 435 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 436 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 437 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 438 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 439 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 440 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 441 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 442 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 443 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 444 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 445 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 446 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 447 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 448 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 449 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 450 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 451 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 452 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 453 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 454 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 455 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 456 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 457 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 458 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 459 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 460 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 461 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 462 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 463 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 464 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 465 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 466 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 467 } // EHand::ePair
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 468 } // EHand::eHighCard
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 469 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 470 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 471 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 472 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 473 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 474 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 475 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 476 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 477 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 478 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 479 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 480 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 481 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 482 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 483 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 484 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 485 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 486 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 487 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 488 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 489 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 490 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 491 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 492 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 493 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 494 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 495 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 496 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 497 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 498 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 499 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 500 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 501 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 502 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 503 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 504 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 505 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 506 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 507 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 508 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 509 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 510 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 511 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 512 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 513 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 514 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 515 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 516 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 517 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 518 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 519 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 520 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 521 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 522 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 523 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 524 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 525 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 526 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 527 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 528 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 529 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 530 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 531 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 532 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 533 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 534 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 535 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 536 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 537 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 538 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 539 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 540 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 541 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 542 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 543 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 544 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 545 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 546 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 547 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 548 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 549 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 550 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 551 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 552 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 553 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 554 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 555 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 556 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 557 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 558 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 559 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 560 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 561 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 562 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 563 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 564 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 565 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 566 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 567 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 568 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 569 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 570 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 571 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 572 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 573 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 574 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 575 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 576 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 577 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 578 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 579 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 580 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 581 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 582 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 583 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 584 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 585 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 586 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 587 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 588 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 589 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 590 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 591 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 592 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 593 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 594 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 595 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 596 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 597 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 598 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 599 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 600 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 601 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 602 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 603 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 604 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 605 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 606 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 607 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 608 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 609 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 610 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 611 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 612 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 613 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 614 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 615 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 616 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 617 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 618 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 619 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 620 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 621 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 622 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 623 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 624 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 625 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 626 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 627 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 628 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 629 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 630 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 631 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 632 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 633 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 634 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 635 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 636 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 637 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 638 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 639 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 640 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 641 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 642 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 643 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 644 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 645 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 646 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 647 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 648 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 649 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 650 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 651 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 652 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 653 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 654 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 655 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 656 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 657 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 658 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 659 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 660 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 661 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 662 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 663 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 664 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 665 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 666 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 667 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 668 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 669 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 670 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 671 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 672 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 673 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 674 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 675 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 676 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 677 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 678 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 679 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 680 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 681 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 682 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 683 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 684 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 685 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 686 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 687 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 688 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 689 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 690 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 691 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 692 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 693 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 694 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 695 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 696 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 697 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 698 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 699 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 700 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 701 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 702 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 703 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 704 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 705 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 706 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 707 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 708 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 709 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 710 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 711 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 712 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 713 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 714 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 715 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 716 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 717 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 718 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 719 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 720 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 721 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 722 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 723 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 724 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 725 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 726 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 727 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 728 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 729 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 730 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 731 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 732 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 733 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 734 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 735 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 736 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 737 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 738 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 739 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 740 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 741 } // EHand::eHighCard
            };

            // Flushes/Straights Don't Count - LowBall
            sCardInfoData.fCardMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ] =
            {
                 { { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 1 } // EHand::eThreeOfAKind
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 2 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 3 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 4 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 5 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 6 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 7 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 8 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 9 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 10 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 11 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 12 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 13 } // EHand::eThreeOfAKind
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 14 } // EHand::ePair
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 15 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 16 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 17 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 18 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 19 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 20 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 21 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 22 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 23 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 24 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 25 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 26 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 27 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 28 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 29 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 30 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 31 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 32 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 33 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 34 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 35 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 36 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 37 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 38 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 39 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 40 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 41 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 42 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 43 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 44 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 45 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 46 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 47 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 48 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 49 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 50 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 51 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 52 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 53 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 54 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 55 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 56 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 57 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 58 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 59 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 60 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 61 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 62 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 63 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 64 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 65 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 66 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 67 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 68 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 69 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 70 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 71 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 72 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 73 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 74 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 75 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 76 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 77 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 78 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 79 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 80 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 81 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 82 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 83 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 84 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 85 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 86 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 87 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 88 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 89 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 90 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 91 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 92 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 93 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 94 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 95 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 96 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 97 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 98 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 99 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 100 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 101 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 102 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 103 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 104 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 105 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 106 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 107 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 108 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 109 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 110 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 111 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 112 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 113 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 114 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 115 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 116 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 117 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 118 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 119 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 120 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 121 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 122 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 123 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 124 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 125 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 126 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 127 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 128 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 129 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 130 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 131 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 132 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 133 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 134 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 135 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 136 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 137 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 138 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 139 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 140 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 141 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 142 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 143 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 144 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 145 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 146 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 147 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 148 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 149 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 150 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 151 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 152 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 153 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 154 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 155 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 156 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 157 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 158 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 159 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 160 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 161 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 162 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 163 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 164 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 165 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 166 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 167 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 168 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 169 } // EHand::ePair
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 170 } // EHand::eHighCard
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 171 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 172 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 173 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 174 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 175 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 176 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 177 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 178 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 179 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 180 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 181 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 182 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 183 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 184 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 185 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 186 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 187 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 188 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 189 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 190 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 191 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 192 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 193 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 194 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 195 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 196 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 197 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 198 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 199 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 200 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 201 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 202 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 203 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 204 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 205 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 206 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 207 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 208 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 209 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 210 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 211 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 212 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 213 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 214 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 215 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 216 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 217 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 218 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 219 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 220 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 221 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 222 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 223 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 224 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 225 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 226 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 227 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 228 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 229 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 230 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 231 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 232 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 233 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 234 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 235 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 236 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 237 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 238 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 239 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 240 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 241 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 242 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 243 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 244 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 245 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 246 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 247 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 248 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 249 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 250 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 251 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 252 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 253 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 254 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 255 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 256 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 257 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 258 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 259 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 260 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 261 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 262 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 263 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 264 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 265 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 266 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 267 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 268 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 269 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 270 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 271 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 272 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 273 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 274 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 275 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 276 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 277 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 278 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 279 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 280 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 281 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 282 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 283 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 284 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 285 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 286 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 287 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 288 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 289 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 290 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 291 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 292 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 293 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 294 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 295 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 296 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 297 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 298 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 299 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 300 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 301 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 302 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 303 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 304 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 305 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 306 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 307 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 308 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 309 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 310 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 311 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 312 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 313 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 314 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 315 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 316 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 317 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 318 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 319 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 320 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 321 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 322 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 323 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 324 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 325 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 326 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 327 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 328 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 329 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 330 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 331 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 332 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 333 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 334 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 335 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 336 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 337 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 338 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 339 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 340 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 341 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 342 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 343 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 344 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 345 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 346 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 347 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 348 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 349 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 350 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 351 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 352 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 353 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 354 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 355 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 356 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 357 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 358 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 359 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 360 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 361 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 362 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 363 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 364 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 365 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 366 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 367 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 368 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 369 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 370 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 371 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 372 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 373 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 374 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 375 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 376 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 377 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 378 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 379 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 380 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 381 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 382 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 383 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 384 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 385 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 386 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 387 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 388 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 389 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 390 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 391 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 392 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 393 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 394 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 395 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 396 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 397 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 398 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 399 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 400 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 401 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 402 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 403 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 404 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 405 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 406 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 407 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 408 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 409 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 410 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 411 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 412 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 413 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 414 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 415 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 416 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 417 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 418 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 419 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 420 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 421 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 422 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 423 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 424 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 425 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 426 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 427 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 428 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 429 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 430 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 431 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 432 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 433 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 434 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 435 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 436 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 437 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 438 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 439 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 440 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 441 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 442 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 443 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 444 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 445 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 446 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 447 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 448 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 449 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 450 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 451 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 452 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 453 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 454 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 455 } // EHand::eHighCard
            };

            // Flushes/Straights Count - LowBall
            sCardInfoData.fCardMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ] =
            {
                 { { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 1 } // EHand::eStraightFlush
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 2 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 3 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 4 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 5 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 6 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 7 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 8 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 9 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 10 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } }, 11 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 12 } // EHand::eStraightFlush
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } }, 13 } // EHand::eThreeOfAKind
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } }, 14 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } }, 15 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } }, 16 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } }, 17 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } }, 18 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } }, 19 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } }, 20 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } }, 21 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } }, 22 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } }, 23 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } }, 24 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } }, 25 } // EHand::eThreeOfAKind
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 26 } // EHand::eStraight
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 27 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 28 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 29 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 30 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 31 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 32 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 33 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 34 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 35 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 36 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 37 } // EHand::eStraight
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 38 } // EHand::eFlush
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 39 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 40 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 41 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 42 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 43 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 44 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 45 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 46 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 47 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 48 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 49 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 50 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 51 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 52 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 53 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 54 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 55 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 56 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 57 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 58 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 59 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 60 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 61 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 62 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 63 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 64 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 65 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 66 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 67 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 68 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 69 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 70 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 71 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 72 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 73 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 74 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 75 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 76 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 77 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 78 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 79 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 80 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 81 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 82 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 83 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 84 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 85 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 86 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 87 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 88 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 89 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 90 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 91 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 92 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 93 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 94 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 95 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 96 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 97 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 98 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 99 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 100 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } }, 101 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 102 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 103 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 104 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 105 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 106 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 107 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 108 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 109 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 110 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 111 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 112 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 113 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 114 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 115 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 116 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 117 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 118 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 119 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 120 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 121 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 122 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 123 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 124 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 125 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 126 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 127 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 128 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 129 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 130 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 131 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 132 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 133 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 134 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 135 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 136 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 137 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 138 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 139 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 140 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 141 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 142 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 143 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 144 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 145 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 146 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 147 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 148 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 149 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 150 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 151 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 152 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 153 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 154 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } }, 155 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 156 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 157 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 158 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 159 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 160 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 161 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 162 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 163 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 164 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 165 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 166 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 167 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 168 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 169 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 170 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 171 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 172 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 173 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 174 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 175 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 176 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 177 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 178 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 179 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 180 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 181 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 182 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 183 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 184 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 185 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 186 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 187 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 188 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 189 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 190 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 191 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 192 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 193 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 194 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 195 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 196 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 197 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 198 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } }, 199 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 200 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 201 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 202 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 203 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 204 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 205 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 206 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 207 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 208 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 209 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 210 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 211 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 212 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 213 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 214 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 215 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 216 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 217 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 218 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 219 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 220 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 221 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 222 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 223 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 224 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 225 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 226 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 227 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 228 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 229 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 230 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 231 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 232 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 233 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } }, 234 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 235 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 236 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 237 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 238 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 239 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 240 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 241 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 242 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 243 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 244 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 245 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 246 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 247 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 248 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 249 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 250 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 251 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 252 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 253 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 254 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 255 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 256 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 257 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 258 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 259 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 260 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } }, 261 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 262 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 263 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 264 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 265 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 266 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 267 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 268 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 269 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 270 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 271 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 272 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 273 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 274 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 275 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 276 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 277 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 278 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 279 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 280 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } }, 281 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 282 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 283 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 284 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 285 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 286 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 287 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 288 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 289 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 290 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 291 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 292 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 293 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 294 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } }, 295 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 296 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 297 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 298 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 299 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 300 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 301 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 302 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 303 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } }, 304 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 305 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 306 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 307 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 308 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } }, 309 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 310 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } }, 311 } // EHand::eFlush
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 312 } // EHand::ePair
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 313 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 314 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 315 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 316 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 317 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 318 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 319 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 320 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 321 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 322 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 323 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 324 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 325 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 326 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 327 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 328 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 329 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 330 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 331 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 332 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 333 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 334 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 335 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 336 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 337 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 338 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 339 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 340 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 341 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 342 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 343 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 344 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 345 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 346 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 347 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 348 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 349 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 350 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 351 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 352 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 353 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 354 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 355 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 356 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 357 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 358 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 359 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 360 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 361 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 362 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 363 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 364 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 365 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 366 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 367 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 368 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 369 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 370 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 371 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 372 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 373 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 374 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 375 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 376 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 377 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 378 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 379 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 380 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 381 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 382 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 383 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 384 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 385 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 386 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 387 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 388 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 389 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 390 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 391 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 392 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 393 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 394 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 395 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 396 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 397 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 398 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 399 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 400 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 401 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 402 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 403 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 404 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 405 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 406 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 407 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 408 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 409 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 410 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 411 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 412 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 413 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 414 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 415 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 416 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 417 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 418 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 419 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 420 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 421 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 422 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 423 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 424 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 425 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 426 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 427 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 428 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 429 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 430 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } }, 431 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 432 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 433 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 434 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 435 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 436 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 437 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 438 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 439 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 440 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 441 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 442 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } }, 443 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 444 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 445 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 446 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 447 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 448 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 449 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 450 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 451 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 452 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 453 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 454 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } }, 455 }
                ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 456 }
                ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 457 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 458 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 459 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 460 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 461 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 462 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 463 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 464 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 465 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 466 }
                ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 467 } // EHand::ePair
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 468 } // EHand::eHighCard
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 469 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 470 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 471 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 472 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 473 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 474 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 475 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 476 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 477 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 478 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 479 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 480 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 481 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 482 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 483 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 484 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 485 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 486 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 487 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 488 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 489 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 490 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 491 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 492 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 493 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 494 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 495 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 496 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 497 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 498 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 499 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 500 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 501 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 502 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 503 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 504 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 505 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 506 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 507 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 508 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 509 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 510 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 511 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 512 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 513 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 514 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 515 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 516 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 517 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 518 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 519 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 520 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } }, 521 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 522 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 523 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 524 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 525 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 526 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 527 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 528 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 529 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 530 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 531 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 532 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 533 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 534 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 535 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 536 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 537 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 538 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 539 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 540 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 541 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 542 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 543 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 544 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 545 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 546 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 547 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 548 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 549 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 550 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 551 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 552 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 553 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 554 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 555 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 556 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 557 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 558 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 559 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 560 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 561 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 562 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 563 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 564 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } }, 565 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 566 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 567 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 568 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 569 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 570 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 571 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 572 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 573 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 574 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 575 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 576 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 577 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 578 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 579 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 580 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 581 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 582 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 583 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 584 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 585 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 586 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 587 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 588 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 589 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 590 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 591 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 592 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 593 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 594 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 595 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 596 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 597 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 598 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 599 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } }, 600 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 601 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 602 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 603 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 604 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 605 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 606 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 607 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 608 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 609 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 610 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 611 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 612 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 613 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 614 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 615 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 616 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 617 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 618 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 619 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 620 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 621 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 622 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 623 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 624 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 625 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 626 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } }, 627 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 628 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 629 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 630 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 631 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 632 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 633 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 634 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 635 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 636 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 637 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 638 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 639 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 640 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 641 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 642 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 643 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 644 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 645 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 646 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } }, 647 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 648 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 649 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 650 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 651 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 652 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 653 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 654 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 655 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 656 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 657 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 658 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 659 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 660 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } }, 661 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 662 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 663 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 664 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 665 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 666 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 667 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 668 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 669 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } }, 670 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 671 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 672 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 673 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 674 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } }, 675 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 676 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } }, 677 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 678 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 679 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 680 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 681 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 682 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 683 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 684 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 685 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 686 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 687 }
                ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 688 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 689 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 690 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 691 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 692 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 693 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 694 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 695 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 696 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 697 }
                ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 698 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 699 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 700 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 701 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 702 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 703 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 704 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 705 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 706 }
                ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 707 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 708 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 709 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 710 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 711 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 712 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 713 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 714 }
                ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 715 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 716 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 717 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 718 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 719 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 720 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 721 }
                ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 722 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 723 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 724 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 725 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 726 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 727 }
                ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 728 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 729 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 730 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 731 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 732 }
                ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 733 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 734 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 735 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 736 }
                ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 737 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 738 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 739 }
                ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 740 }
                ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } }, 741 } // EHand::eHighCard
            };

#else
            sCardInfoData.fCardMaps = { {}, {}, {}, {} };
#endif

            sCardInfoData.fFlushes =
            {
                0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 311, 0, 310, 10, 0, 0, 0, 0, 309,
                0, 308, 307, 0, 0, 306, 305, 0, 9, 0, 0, 0, 0, 0, 0, 304, 0, 303,
                302, 0, 0, 301, 300, 0, 299, 0, 0, 0, 0, 298, 297, 0, 296, 0, 0, 0,
                8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 295, 0, 294, 293, 0, 0, 292, 291,
                0, 290, 0, 0, 0, 0, 289, 288, 0, 287, 0, 0, 0, 286, 0, 0, 0, 0, 0,
                0, 0, 0, 285, 284, 0, 283, 0, 0, 0, 282, 0, 0, 0, 0, 0, 0, 0, 7, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 281, 0, 280, 279,
                0, 0, 278, 277, 0, 276, 0, 0, 0, 0, 275, 274, 0, 273, 0, 0, 0, 272,
                0, 0, 0, 0, 0, 0, 0, 0, 271, 270, 0, 269, 0, 0, 0, 268, 0, 0, 0, 0,
                0, 0, 0, 267, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 266,
                265, 0, 264, 0, 0, 0, 263, 0, 0, 0, 0, 0, 0, 0, 262, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 261, 0, 260, 259, 0, 0, 258, 257, 0, 256, 0, 0, 0, 0, 255, 254,
                0, 253, 0, 0, 0, 252, 0, 0, 0, 0, 0, 0, 0, 0, 251, 250, 0, 249, 0,
                0, 0, 248, 0, 0, 0, 0, 0, 0, 0, 247, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 246, 245, 0, 244, 0, 0, 0, 243, 0, 0, 0, 0, 0, 0,
                0, 242, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 241, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 240, 239, 0, 238, 0, 0, 0, 237, 0, 0, 0, 0, 0,
                0, 0, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 235, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 234, 0, 233, 232, 0, 0, 231, 230, 0, 229,
                0, 0, 0, 0, 228, 227, 0, 226, 0, 0, 0, 225, 0, 0, 0, 0, 0, 0, 0, 0,
                224, 223, 0, 222, 0, 0, 0, 221, 0, 0, 0, 0, 0, 0, 0, 220, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 219, 218, 0, 217, 0, 0, 0,
                216, 0, 0, 0, 0, 0, 0, 0, 215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 214, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 213, 212, 0, 211, 0, 0,
                0, 210, 0, 0, 0, 0, 0, 0, 0, 209, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 206, 205, 0, 204, 0, 0, 0,
                203, 0, 0, 0, 0, 0, 0, 0, 202, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 201, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 199, 0, 198, 197, 0, 0, 196, 195, 0,
                194, 0, 0, 0, 0, 193, 192, 0, 191, 0, 0, 0, 190, 0, 0, 0, 0, 0, 0,
                0, 0, 189, 188, 0, 187, 0, 0, 0, 186, 0, 0, 0, 0, 0, 0, 0, 185, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 183, 0, 182, 0,
                0, 0, 181, 0, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 179, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 178, 177, 0, 176,
                0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 170, 0, 169, 0,
                0, 0, 168, 0, 0, 0, 0, 0, 0, 0, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 165, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 164, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 163, 162, 0, 161, 0, 0, 0, 160, 0, 0,
                0, 0, 0, 0, 0, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 158,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 155, 0,
                154, 153, 0, 0, 152, 151, 0, 150, 0, 0, 0, 0, 149, 148, 0, 147, 0,
                0, 0, 146, 0, 0, 0, 0, 0, 0, 0, 0, 145, 144, 0, 143, 0, 0, 0, 142,
                0, 0, 0, 0, 0, 0, 0, 141, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 140, 139, 0, 138, 0, 0, 0, 137, 0, 0, 0, 0, 0, 0, 0, 136, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 134, 133, 0, 132, 0, 0, 0, 131, 0, 0, 0, 0, 0, 0, 0, 130,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 127, 126, 0, 125, 0, 0, 0, 124, 0, 0, 0, 0, 0, 0, 0, 123, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 121, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 118,
                0, 117, 0, 0, 0, 116, 0, 0, 0, 0, 0, 0, 0, 115, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 113, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 110, 109, 0, 108, 0, 0, 0, 107, 0, 0, 0, 0, 0, 0,
                0, 106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 12, 0, 101, 100, 0, 0, 99, 98, 0, 97, 0, 0, 0, 0, 96, 95, 0, 94,
                0, 0, 0, 93, 0, 0, 0, 0, 0, 0, 0, 0, 92, 91, 0, 90, 0, 0, 0, 89, 0,
                0, 0, 0, 0, 0, 0, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 87, 86, 0, 85, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                81, 80, 0, 79, 0, 0, 0, 78, 0, 0, 0, 0, 0, 0, 0, 77, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 73, 0,
                72, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 65, 0, 64, 0, 0, 0, 63, 0,
                0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 56, 0,
                55, 0, 0, 0, 54, 0, 0, 0, 0, 0, 0, 0, 53, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 46, 0, 45, 0, 0, 0, 44, 0, 0, 0,
                0, 0, 0, 0, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                1
            };

            sCardInfoData.fUniqueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ] =
            {
                0, 0, 0, 0, 0, 0, 0, 455, 0, 0, 0, 454, 0, 453, 452, 0, 0, 0, 0, 451,
                0, 450, 449, 0, 0, 448, 447, 0, 446, 0, 0, 0, 0, 0, 0, 445, 0, 444,
                443, 0, 0, 442, 441, 0, 440, 0, 0, 0, 0, 439, 438, 0, 437, 0, 0, 0,
                436, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 435, 0, 434, 433, 0, 0, 432, 431,
                0, 430, 0, 0, 0, 0, 429, 428, 0, 427, 0, 0, 0, 426, 0, 0, 0, 0, 0,
                0, 0, 0, 425, 424, 0, 423, 0, 0, 0, 422, 0, 0, 0, 0, 0, 0, 0, 421,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 420, 0, 419,
                418, 0, 0, 417, 416, 0, 415, 0, 0, 0, 0, 414, 413, 0, 412, 0, 0, 0,
                411, 0, 0, 0, 0, 0, 0, 0, 0, 410, 409, 0, 408, 0, 0, 0, 407, 0, 0,
                0, 0, 0, 0, 0, 406, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                405, 404, 0, 403, 0, 0, 0, 402, 0, 0, 0, 0, 0, 0, 0, 401, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 399, 0, 398, 397, 0, 0, 396, 395, 0, 394, 0, 0, 0, 0, 393,
                392, 0, 391, 0, 0, 0, 390, 0, 0, 0, 0, 0, 0, 0, 0, 389, 388, 0, 387,
                0, 0, 0, 386, 0, 0, 0, 0, 0, 0, 0, 385, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 384, 383, 0, 382, 0, 0, 0, 381, 0, 0, 0, 0, 0,
                0, 0, 380, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 379, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 378, 377, 0, 376, 0, 0, 0, 375, 0, 0, 0, 0,
                0, 0, 0, 374, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 373, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 372, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 371, 0, 370, 369, 0, 0, 368, 367, 0, 366,
                0, 0, 0, 0, 365, 364, 0, 363, 0, 0, 0, 362, 0, 0, 0, 0, 0, 0, 0, 0,
                361, 360, 0, 359, 0, 0, 0, 358, 0, 0, 0, 0, 0, 0, 0, 357, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 356, 355, 0, 354, 0, 0, 0,
                353, 0, 0, 0, 0, 0, 0, 0, 352, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 351, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 349, 0, 348, 0, 0,
                0, 347, 0, 0, 0, 0, 0, 0, 0, 346, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 345, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 344, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 343, 342, 0, 341, 0, 0, 0,
                340, 0, 0, 0, 0, 0, 0, 0, 339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 338, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 337, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 336, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 335, 0, 334, 333, 0, 0, 332, 331, 0,
                330, 0, 0, 0, 0, 329, 328, 0, 327, 0, 0, 0, 326, 0, 0, 0, 0, 0, 0,
                0, 0, 325, 324, 0, 323, 0, 0, 0, 322, 0, 0, 0, 0, 0, 0, 0, 321, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 320, 319, 0, 318, 0,
                0, 0, 317, 0, 0, 0, 0, 0, 0, 0, 316, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 315, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 314, 313, 0, 312,
                0, 0, 0, 311, 0, 0, 0, 0, 0, 0, 0, 310, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 309, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 308, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 307, 306, 0, 305, 0,
                0, 0, 304, 0, 0, 0, 0, 0, 0, 0, 303, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 302, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 301, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 299, 298, 0, 297, 0, 0, 0, 296, 0, 0,
                0, 0, 0, 0, 0, 295, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 294,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 293, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 292, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 291, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 290, 0,
                289, 288, 0, 0, 287, 286, 0, 285, 0, 0, 0, 0, 284, 283, 0, 282, 0,
                0, 0, 281, 0, 0, 0, 0, 0, 0, 0, 0, 280, 279, 0, 278, 0, 0, 0, 277,
                0, 0, 0, 0, 0, 0, 0, 276, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 275, 274, 0, 273, 0, 0, 0, 272, 0, 0, 0, 0, 0, 0, 0, 271, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 270, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 269, 268, 0, 267, 0, 0, 0, 266, 0, 0, 0, 0, 0, 0, 0, 265,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 264, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 263, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 262, 261, 0, 260, 0, 0, 0, 259, 0, 0, 0, 0, 0, 0, 0, 258, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 257, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 253,
                0, 252, 0, 0, 0, 251, 0, 0, 0, 0, 0, 0, 0, 250, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 249, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 248, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 247, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 246, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 245, 244, 0, 243, 0, 0, 0, 242, 0, 0, 0, 0, 0, 0,
                0, 241, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 238, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                237, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 236, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 235, 0, 234, 233, 0, 0, 232, 231, 0, 230, 0, 0, 0, 0, 229, 228,
                0, 227, 0, 0, 0, 226, 0, 0, 0, 0, 0, 0, 0, 0, 225, 224, 0, 223, 0,
                0, 0, 222, 0, 0, 0, 0, 0, 0, 0, 221, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 220, 219, 0, 218, 0, 0, 0, 217, 0, 0, 0, 0, 0, 0,
                0, 216, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 215, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 214, 213, 0, 212, 0, 0, 0, 211, 0, 0, 0, 0, 0,
                0, 0, 210, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 209, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 207, 206, 0, 205, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0,
                0, 203, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 202, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 201, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 199, 198, 0, 197, 0, 0, 0, 196, 0, 0, 0, 0, 0, 0, 0, 195, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 194, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 193, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 191, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 190, 189, 0, 188, 0, 0, 0, 187, 0, 0,
                0, 0, 0, 0, 0, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 185,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 183, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 182, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 181, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 180, 179, 0, 178, 0, 0, 0, 177, 0, 0, 0, 0, 0, 0, 0, 176, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 175, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170
            };

            sCardInfoData.fUniqueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ] =
            {
                0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 741, 0, 740, 35, 0, 0, 0, 0, 739,
                0, 738, 737, 0, 0, 736, 735, 0, 34, 0, 0, 0, 0, 0, 0, 734, 0, 733,
                732, 0, 0, 731, 730, 0, 729, 0, 0, 0, 0, 728, 727, 0, 726, 0, 0, 0,
                33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 725, 0, 724, 723, 0, 0, 722, 721,
                0, 720, 0, 0, 0, 0, 719, 718, 0, 717, 0, 0, 0, 716, 0, 0, 0, 0, 0,
                0, 0, 0, 715, 714, 0, 713, 0, 0, 0, 712, 0, 0, 0, 0, 0, 0, 0, 32,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 711, 0, 710,
                709, 0, 0, 708, 707, 0, 706, 0, 0, 0, 0, 705, 704, 0, 703, 0, 0, 0,
                702, 0, 0, 0, 0, 0, 0, 0, 0, 701, 700, 0, 699, 0, 0, 0, 698, 0, 0,
                0, 0, 0, 0, 0, 697, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                696, 695, 0, 694, 0, 0, 0, 693, 0, 0, 0, 0, 0, 0, 0, 692, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 691, 0, 690, 689, 0, 0, 688, 687, 0, 686, 0, 0, 0, 0, 685,
                684, 0, 683, 0, 0, 0, 682, 0, 0, 0, 0, 0, 0, 0, 0, 681, 680, 0, 679,
                0, 0, 0, 678, 0, 0, 0, 0, 0, 0, 0, 677, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 676, 675, 0, 674, 0, 0, 0, 673, 0, 0, 0, 0, 0,
                0, 0, 672, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 671, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 670, 669, 0, 668, 0, 0, 0, 667, 0, 0, 0, 0,
                0, 0, 0, 666, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 665, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 664, 0, 663, 662, 0, 0, 661, 660, 0, 659,
                0, 0, 0, 0, 658, 657, 0, 656, 0, 0, 0, 655, 0, 0, 0, 0, 0, 0, 0, 0,
                654, 653, 0, 652, 0, 0, 0, 651, 0, 0, 0, 0, 0, 0, 0, 650, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 649, 648, 0, 647, 0, 0, 0,
                646, 0, 0, 0, 0, 0, 0, 0, 645, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 644, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 643, 642, 0, 641, 0, 0,
                0, 640, 0, 0, 0, 0, 0, 0, 0, 639, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 638, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 637, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 636, 635, 0, 634, 0, 0, 0,
                633, 0, 0, 0, 0, 0, 0, 0, 632, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 631, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 630, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 629, 0, 628, 627, 0, 0, 626, 625, 0,
                624, 0, 0, 0, 0, 623, 622, 0, 621, 0, 0, 0, 620, 0, 0, 0, 0, 0, 0,
                0, 0, 619, 618, 0, 617, 0, 0, 0, 616, 0, 0, 0, 0, 0, 0, 0, 615, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 614, 613, 0, 612, 0,
                0, 0, 611, 0, 0, 0, 0, 0, 0, 0, 610, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 609, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 608, 607, 0, 606,
                0, 0, 0, 605, 0, 0, 0, 0, 0, 0, 0, 604, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 603, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 602, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 601, 600, 0, 599, 0,
                0, 0, 598, 0, 0, 0, 0, 0, 0, 0, 597, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 596, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 595, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 594, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 593, 592, 0, 591, 0, 0, 0, 590, 0, 0,
                0, 0, 0, 0, 0, 589, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 588,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 587, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 586, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 585, 0,
                584, 583, 0, 0, 582, 581, 0, 580, 0, 0, 0, 0, 579, 578, 0, 577, 0,
                0, 0, 576, 0, 0, 0, 0, 0, 0, 0, 0, 575, 574, 0, 573, 0, 0, 0, 572,
                0, 0, 0, 0, 0, 0, 0, 571, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 570, 569, 0, 568, 0, 0, 0, 567, 0, 0, 0, 0, 0, 0, 0, 566, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 565, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 564, 563, 0, 562, 0, 0, 0, 561, 0, 0, 0, 0, 0, 0, 0, 560,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 559, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 558, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 557, 556, 0, 555, 0, 0, 0, 554, 0, 0, 0, 0, 0, 0, 0, 553, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 552, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 551, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                550, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 549, 548,
                0, 547, 0, 0, 0, 546, 0, 0, 0, 0, 0, 0, 0, 545, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 544, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 543, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 542, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 541, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 540, 539, 0, 538, 0, 0, 0, 537, 0, 0, 0, 0, 0, 0,
                0, 536, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 535, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 534, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 533, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                532, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 37, 0, 531, 530, 0, 0, 529, 528, 0, 527, 0, 0, 0, 0, 526, 525,
                0, 524, 0, 0, 0, 523, 0, 0, 0, 0, 0, 0, 0, 0, 522, 521, 0, 520, 0,
                0, 0, 519, 0, 0, 0, 0, 0, 0, 0, 518, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 517, 516, 0, 515, 0, 0, 0, 514, 0, 0, 0, 0, 0, 0,
                0, 513, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 511, 510, 0, 509, 0, 0, 0, 508, 0, 0, 0, 0, 0,
                0, 0, 507, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 506, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 505, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 504, 503, 0, 502, 0, 0, 0, 501, 0, 0, 0, 0, 0, 0,
                0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 499, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 498, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 497, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 496, 495, 0, 494, 0, 0, 0, 493, 0, 0, 0, 0, 0, 0, 0, 492, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 491, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 490, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 489,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 488, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 487, 486, 0, 485, 0, 0, 0, 484, 0, 0,
                0, 0, 0, 0, 0, 483, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 482,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 481, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 480, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 479, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 478, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 477, 476, 0, 475, 0, 0, 0, 474, 0, 0, 0, 0, 0, 0, 0, 473, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 472, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 471, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                470, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 469, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 468, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26
            };

            sCardInfoData.fUniqueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ] =
            {
                0, 0, 0, 0, 0, 0, 0, 389, 0, 0, 0, 388, 0, 387, 386, 0, 0, 0, 0, 385,
                0, 384, 383, 0, 0, 382, 381, 0, 380, 0, 0, 0, 0, 0, 0, 379, 0, 378,
                377, 0, 0, 376, 375, 0, 374, 0, 0, 0, 0, 373, 372, 0, 371, 0, 0, 0,
                370, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 369, 0, 368, 367, 0, 0, 366, 365,
                0, 364, 0, 0, 0, 0, 363, 362, 0, 361, 0, 0, 0, 360, 0, 0, 0, 0, 0,
                0, 0, 0, 359, 358, 0, 357, 0, 0, 0, 356, 0, 0, 0, 0, 0, 0, 0, 355,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 354, 0, 353,
                352, 0, 0, 351, 350, 0, 349, 0, 0, 0, 0, 348, 347, 0, 346, 0, 0, 0,
                345, 0, 0, 0, 0, 0, 0, 0, 0, 344, 343, 0, 342, 0, 0, 0, 341, 0, 0,
                0, 0, 0, 0, 0, 340, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                339, 338, 0, 337, 0, 0, 0, 336, 0, 0, 0, 0, 0, 0, 0, 335, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 334, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 333, 0, 332, 331, 0, 0, 330, 329, 0, 328, 0, 0, 0, 0, 327,
                326, 0, 325, 0, 0, 0, 324, 0, 0, 0, 0, 0, 0, 0, 0, 323, 322, 0, 321,
                0, 0, 0, 320, 0, 0, 0, 0, 0, 0, 0, 319, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 318, 317, 0, 316, 0, 0, 0, 315, 0, 0, 0, 0, 0,
                0, 0, 314, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 313, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 312, 311, 0, 310, 0, 0, 0, 309, 0, 0, 0, 0,
                0, 0, 0, 308, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 307, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 306, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 305, 0, 304, 303, 0, 0, 302, 301, 0, 300,
                0, 0, 0, 0, 299, 298, 0, 297, 0, 0, 0, 296, 0, 0, 0, 0, 0, 0, 0, 0,
                295, 294, 0, 293, 0, 0, 0, 292, 0, 0, 0, 0, 0, 0, 0, 291, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 290, 289, 0, 288, 0, 0, 0,
                287, 0, 0, 0, 0, 0, 0, 0, 286, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 285, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 284, 283, 0, 282, 0, 0,
                0, 281, 0, 0, 0, 0, 0, 0, 0, 280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 279, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 278, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 277, 276, 0, 275, 0, 0, 0,
                274, 0, 0, 0, 0, 0, 0, 0, 273, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 271, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 270, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 269, 0, 268, 267, 0, 0, 266, 265, 0,
                264, 0, 0, 0, 0, 263, 262, 0, 261, 0, 0, 0, 260, 0, 0, 0, 0, 0, 0,
                0, 0, 259, 258, 0, 257, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 255, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 253, 0, 252, 0,
                0, 0, 251, 0, 0, 0, 0, 0, 0, 0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 249, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 248, 247, 0, 246,
                0, 0, 0, 245, 0, 0, 0, 0, 0, 0, 0, 244, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 243, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 242, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 241, 240, 0, 239, 0,
                0, 0, 238, 0, 0, 0, 0, 0, 0, 0, 237, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 235, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 234, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 233, 232, 0, 231, 0, 0, 0, 230, 0, 0,
                0, 0, 0, 0, 0, 229, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 228,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 227, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 226, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 225, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 0,
                223, 222, 0, 0, 221, 220, 0, 219, 0, 0, 0, 0, 218, 217, 0, 216, 0,
                0, 0, 215, 0, 0, 0, 0, 0, 0, 0, 0, 214, 213, 0, 212, 0, 0, 0, 211,
                0, 0, 0, 0, 0, 0, 0, 210, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 209, 208, 0, 207, 0, 0, 0, 206, 0, 0, 0, 0, 0, 0, 0, 205, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 203, 202, 0, 201, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 199,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 198, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 197, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 196, 195, 0, 194, 0, 0, 0, 193, 0, 0, 0, 0, 0, 0, 0, 192, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 191, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 190, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                189, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 188, 187,
                0, 186, 0, 0, 0, 185, 0, 0, 0, 0, 0, 0, 0, 184, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 182, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 181, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 179, 178, 0, 177, 0, 0, 0, 176, 0, 0, 0, 0, 0, 0,
                0, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 174, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 455, 0, 454, 453, 0, 0, 452, 451, 0, 450, 0, 0, 0, 0, 449, 448,
                0, 447, 0, 0, 0, 446, 0, 0, 0, 0, 0, 0, 0, 0, 445, 444, 0, 443, 0,
                0, 0, 442, 0, 0, 0, 0, 0, 0, 0, 441, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 440, 439, 0, 438, 0, 0, 0, 437, 0, 0, 0, 0, 0, 0,
                0, 436, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 435, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 434, 433, 0, 432, 0, 0, 0, 431, 0, 0, 0, 0, 0,
                0, 0, 430, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 429, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 428, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 427, 426, 0, 425, 0, 0, 0, 424, 0, 0, 0, 0, 0, 0,
                0, 423, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 422, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 421, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 420, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 419, 418, 0, 417, 0, 0, 0, 416, 0, 0, 0, 0, 0, 0, 0, 415, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 414, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 413, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 412,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 411, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 410, 409, 0, 408, 0, 0, 0, 407, 0, 0,
                0, 0, 0, 0, 0, 406, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 405,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 404, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 403, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 402, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 401, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 400, 399, 0, 398, 0, 0, 0, 397, 0, 0, 0, 0, 0, 0, 0, 396, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 395, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 394, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                393, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 392, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 391, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 390
            };

            sCardInfoData.fUniqueVectors[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ] =
            {
                0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 677, 0, 676, 35, 0, 0, 0, 0, 675,
                0, 674, 673, 0, 0, 672, 671, 0, 34, 0, 0, 0, 0, 0, 0, 670, 0, 669,
                668, 0, 0, 667, 666, 0, 665, 0, 0, 0, 0, 664, 663, 0, 662, 0, 0, 0,
                33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 661, 0, 660, 659, 0, 0, 658, 657,
                0, 656, 0, 0, 0, 0, 655, 654, 0, 653, 0, 0, 0, 652, 0, 0, 0, 0, 0,
                0, 0, 0, 651, 650, 0, 649, 0, 0, 0, 648, 0, 0, 0, 0, 0, 0, 0, 32,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 647, 0, 646,
                645, 0, 0, 644, 643, 0, 642, 0, 0, 0, 0, 641, 640, 0, 639, 0, 0, 0,
                638, 0, 0, 0, 0, 0, 0, 0, 0, 637, 636, 0, 635, 0, 0, 0, 634, 0, 0,
                0, 0, 0, 0, 0, 633, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                632, 631, 0, 630, 0, 0, 0, 629, 0, 0, 0, 0, 0, 0, 0, 628, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 627, 0, 626, 625, 0, 0, 624, 623, 0, 622, 0, 0, 0, 0, 621,
                620, 0, 619, 0, 0, 0, 618, 0, 0, 0, 0, 0, 0, 0, 0, 617, 616, 0, 615,
                0, 0, 0, 614, 0, 0, 0, 0, 0, 0, 0, 613, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 612, 611, 0, 610, 0, 0, 0, 609, 0, 0, 0, 0, 0,
                0, 0, 608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 607, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 606, 605, 0, 604, 0, 0, 0, 603, 0, 0, 0, 0,
                0, 0, 0, 602, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 601, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 599, 598, 0, 0, 597, 596, 0, 595,
                0, 0, 0, 0, 594, 593, 0, 592, 0, 0, 0, 591, 0, 0, 0, 0, 0, 0, 0, 0,
                590, 589, 0, 588, 0, 0, 0, 587, 0, 0, 0, 0, 0, 0, 0, 586, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 585, 584, 0, 583, 0, 0, 0,
                582, 0, 0, 0, 0, 0, 0, 0, 581, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 580, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 579, 578, 0, 577, 0, 0,
                0, 576, 0, 0, 0, 0, 0, 0, 0, 575, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 574, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 573, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 572, 571, 0, 570, 0, 0, 0,
                569, 0, 0, 0, 0, 0, 0, 0, 568, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 567, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 566, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 565, 0, 564, 563, 0, 0, 562, 561, 0,
                560, 0, 0, 0, 0, 559, 558, 0, 557, 0, 0, 0, 556, 0, 0, 0, 0, 0, 0,
                0, 0, 555, 554, 0, 553, 0, 0, 0, 552, 0, 0, 0, 0, 0, 0, 0, 551, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 550, 549, 0, 548, 0,
                0, 0, 547, 0, 0, 0, 0, 0, 0, 0, 546, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 545, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 544, 543, 0, 542,
                0, 0, 0, 541, 0, 0, 0, 0, 0, 0, 0, 540, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 539, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 538, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 537, 536, 0, 535, 0,
                0, 0, 534, 0, 0, 0, 0, 0, 0, 0, 533, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 532, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 531, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 530, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 529, 528, 0, 527, 0, 0, 0, 526, 0, 0,
                0, 0, 0, 0, 0, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 524,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 523, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 522, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 521, 0,
                520, 519, 0, 0, 518, 517, 0, 516, 0, 0, 0, 0, 515, 514, 0, 513, 0,
                0, 0, 512, 0, 0, 0, 0, 0, 0, 0, 0, 511, 510, 0, 509, 0, 0, 0, 508,
                0, 0, 0, 0, 0, 0, 0, 507, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 506, 505, 0, 504, 0, 0, 0, 503, 0, 0, 0, 0, 0, 0, 0, 502, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 501, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 500, 499, 0, 498, 0, 0, 0, 497, 0, 0, 0, 0, 0, 0, 0, 496,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 495, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 494, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 493, 492, 0, 491, 0, 0, 0, 490, 0, 0, 0, 0, 0, 0, 0, 489, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 488, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 487, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                486, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 485, 484,
                0, 483, 0, 0, 0, 482, 0, 0, 0, 0, 0, 0, 0, 481, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 480, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 479, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 478, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 477, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 476, 475, 0, 474, 0, 0, 0, 473, 0, 0, 0, 0, 0, 0,
                0, 472, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 471, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 470, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 469, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                468, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 37, 0, 741, 740, 0, 0, 739, 738, 0, 737, 0, 0, 0, 0, 736, 735,
                0, 734, 0, 0, 0, 733, 0, 0, 0, 0, 0, 0, 0, 0, 732, 731, 0, 730, 0,
                0, 0, 729, 0, 0, 0, 0, 0, 0, 0, 728, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 727, 726, 0, 725, 0, 0, 0, 724, 0, 0, 0, 0, 0, 0,
                0, 723, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 722, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 721, 720, 0, 719, 0, 0, 0, 718, 0, 0, 0, 0, 0,
                0, 0, 717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 716, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 714, 713, 0, 712, 0, 0, 0, 711, 0, 0, 0, 0, 0, 0,
                0, 710, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 709, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 708, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 707, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 706, 705, 0, 704, 0, 0, 0, 703, 0, 0, 0, 0, 0, 0, 0, 702, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 701, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 699,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 698, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 697, 696, 0, 695, 0, 0, 0, 694, 0, 0,
                0, 0, 0, 0, 0, 693, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 692,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 691, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 690, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 689, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 688, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 687, 686, 0, 685, 0, 0, 0, 684, 0, 0, 0, 0, 0, 0, 0, 683, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 682, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 681, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 679, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 678, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26
            };

            sCardInfoData.fProductMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCount ] =
            {
                 { 8, 13 }
                ,{ 12, 169 }
                ,{ 18, 157 }
                ,{ 20, 168 }
                ,{ 27, 12 }
                ,{ 28, 167 }
                ,{ 44, 166 }
                ,{ 45, 156 }
                ,{ 50, 145 }
                ,{ 52, 165 }
                ,{ 63, 155 }
                ,{ 68, 164 }
                ,{ 75, 144 }
                ,{ 76, 163 }
                ,{ 92, 162 }
                ,{ 98, 133 }
                ,{ 99, 154 }
                ,{ 116, 161 }
                ,{ 117, 153 }
                ,{ 124, 160 }
                ,{ 125, 11 }
                ,{ 147, 132 }
                ,{ 148, 159 }
                ,{ 153, 152 }
                ,{ 164, 158 }
                ,{ 171, 151 }
                ,{ 175, 143 }
                ,{ 207, 150 }
                ,{ 242, 121 }
                ,{ 245, 131 }
                ,{ 261, 149 }
                ,{ 275, 142 }
                ,{ 279, 148 }
                ,{ 325, 141 }
                ,{ 333, 147 }
                ,{ 338, 109 }
                ,{ 343, 10 }
                ,{ 363, 120 }
                ,{ 369, 146 }
                ,{ 425, 140 }
                ,{ 475, 139 }
                ,{ 507, 108 }
                ,{ 539, 130 }
                ,{ 575, 138 }
                ,{ 578, 97 }
                ,{ 605, 119 }
                ,{ 637, 129 }
                ,{ 722, 85 }
                ,{ 725, 137 }
                ,{ 775, 136 }
                ,{ 833, 128 }
                ,{ 845, 107 }
                ,{ 847, 118 }
                ,{ 867, 96 }
                ,{ 925, 135 }
                ,{ 931, 127 }
                ,{ 1025, 134 }
                ,{ 1058, 73 }
                ,{ 1083, 84 }
                ,{ 1127, 126 }
                ,{ 1183, 106 }
                ,{ 1331, 9 }
                ,{ 1421, 125 }
                ,{ 1445, 95 }
                ,{ 1519, 124 }
                ,{ 1573, 117 }
                ,{ 1587, 72 }
                ,{ 1682, 61 }
                ,{ 1805, 83 }
                ,{ 1813, 123 }
                ,{ 1859, 105 }
                ,{ 1922, 49 }
                ,{ 2009, 122 }
                ,{ 2023, 94 }
                ,{ 2057, 116 }
                ,{ 2197, 8 }
                ,{ 2299, 115 }
                ,{ 2523, 60 }
                ,{ 2527, 82 }
                ,{ 2645, 71 }
                ,{ 2738, 37 }
                ,{ 2783, 114 }
                ,{ 2873, 104 }
                ,{ 2883, 48 }
                ,{ 3179, 93 }
                ,{ 3211, 103 }
                ,{ 3362, 25 }
                ,{ 3509, 113 }
                ,{ 3703, 70 }
                ,{ 3751, 112 }
                ,{ 3757, 92 }
                ,{ 3887, 102 }
                ,{ 3971, 81 }
                ,{ 4107, 36 }
                ,{ 4205, 59 }
                ,{ 4477, 111 }
                ,{ 4693, 80 }
                ,{ 4805, 47 }
                ,{ 4901, 101 }
                ,{ 4913, 7 }
                ,{ 4961, 110 }
                ,{ 5043, 24 }
                ,{ 5239, 100 }
                ,{ 5491, 91 }
                ,{ 5819, 69 }
                ,{ 5887, 58 }
                ,{ 6137, 79 }
                ,{ 6253, 99 }
                ,{ 6647, 90 }
                ,{ 6727, 46 }
                ,{ 6845, 35 }
                ,{ 6859, 6 }
                ,{ 6877, 68 }
                ,{ 6929, 98 }
                ,{ 8303, 78 }
                ,{ 8381, 89 }
                ,{ 8405, 23 }
                ,{ 8959, 88 }
                ,{ 8993, 67 }
                ,{ 9251, 57 }
                ,{ 9583, 34 }
                ,{ 10051, 66 }
                ,{ 10469, 77 }
                ,{ 10571, 45 }
                ,{ 10693, 87 }
                ,{ 10933, 56 }
                ,{ 11191, 76 }
                ,{ 11767, 22 }
                ,{ 11849, 86 }
                ,{ 12167, 5 }
                ,{ 12493, 44 }
                ,{ 13357, 75 }
                ,{ 14297, 55 }
                ,{ 14801, 74 }
                ,{ 15059, 33 }
                ,{ 15341, 65 }
                ,{ 15979, 54 }
                ,{ 16337, 43 }
                ,{ 16399, 64 }
                ,{ 17797, 32 }
                ,{ 18259, 42 }
                ,{ 18491, 21 }
                ,{ 19343, 53 }
                ,{ 19573, 63 }
                ,{ 21689, 62 }
                ,{ 21853, 20 }
                ,{ 22103, 41 }
                ,{ 23273, 31 }
                ,{ 24389, 4 }
                ,{ 26011, 30 }
                ,{ 26071, 52 }
                ,{ 27869, 40 }
                ,{ 28577, 19 }
                ,{ 29791, 3 }
                ,{ 31117, 51 }
                ,{ 31487, 29 }
                ,{ 31939, 18 }
                ,{ 34481, 50 }
                ,{ 35557, 39 }
                ,{ 38663, 17 }
                ,{ 39401, 38 }
                ,{ 39701, 28 }
                ,{ 42439, 27 }
                ,{ 48749, 16 }
                ,{ 50653, 2 }
                ,{ 52111, 15 }
                ,{ 56129, 26 }
                ,{ 62197, 14 }
                ,{ 68921, 1 }
            };

            sCardInfoData.fProductMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCount ] =
            {
                 { 8, 25 }
                ,{ 12, 467 }
                ,{ 18, 455 }
                ,{ 20, 466 }
                ,{ 27, 24 }
                ,{ 28, 465 }
                ,{ 44, 464 }
                ,{ 45, 454 }
                ,{ 50, 443 }
                ,{ 52, 463 }
                ,{ 63, 453 }
                ,{ 68, 462 }
                ,{ 75, 442 }
                ,{ 76, 461 }
                ,{ 92, 460 }
                ,{ 98, 431 }
                ,{ 99, 452 }
                ,{ 116, 459 }
                ,{ 117, 451 }
                ,{ 124, 458 }
                ,{ 125, 23 }
                ,{ 147, 430 }
                ,{ 148, 457 }
                ,{ 153, 450 }
                ,{ 164, 456 }
                ,{ 171, 449 }
                ,{ 175, 441 }
                ,{ 207, 448 }
                ,{ 242, 419 }
                ,{ 245, 429 }
                ,{ 261, 447 }
                ,{ 275, 440 }
                ,{ 279, 446 }
                ,{ 325, 439 }
                ,{ 333, 445 }
                ,{ 338, 407 }
                ,{ 343, 22 }
                ,{ 363, 418 }
                ,{ 369, 444 }
                ,{ 425, 438 }
                ,{ 475, 437 }
                ,{ 507, 406 }
                ,{ 539, 428 }
                ,{ 575, 436 }
                ,{ 578, 395 }
                ,{ 605, 417 }
                ,{ 637, 427 }
                ,{ 722, 383 }
                ,{ 725, 435 }
                ,{ 775, 434 }
                ,{ 833, 426 }
                ,{ 845, 405 }
                ,{ 847, 416 }
                ,{ 867, 394 }
                ,{ 925, 433 }
                ,{ 931, 425 }
                ,{ 1025, 432 }
                ,{ 1058, 371 }
                ,{ 1083, 382 }
                ,{ 1127, 424 }
                ,{ 1183, 404 }
                ,{ 1331, 21 }
                ,{ 1421, 423 }
                ,{ 1445, 393 }
                ,{ 1519, 422 }
                ,{ 1573, 415 }
                ,{ 1587, 370 }
                ,{ 1682, 359 }
                ,{ 1805, 381 }
                ,{ 1813, 421 }
                ,{ 1859, 403 }
                ,{ 1922, 347 }
                ,{ 2009, 420 }
                ,{ 2023, 392 }
                ,{ 2057, 414 }
                ,{ 2197, 20 }
                ,{ 2299, 413 }
                ,{ 2523, 358 }
                ,{ 2527, 380 }
                ,{ 2645, 369 }
                ,{ 2738, 335 }
                ,{ 2783, 412 }
                ,{ 2873, 402 }
                ,{ 2883, 346 }
                ,{ 3179, 391 }
                ,{ 3211, 401 }
                ,{ 3362, 323 }
                ,{ 3509, 411 }
                ,{ 3703, 368 }
                ,{ 3751, 410 }
                ,{ 3757, 390 }
                ,{ 3887, 400 }
                ,{ 3971, 379 }
                ,{ 4107, 334 }
                ,{ 4205, 357 }
                ,{ 4477, 409 }
                ,{ 4693, 378 }
                ,{ 4805, 345 }
                ,{ 4901, 399 }
                ,{ 4913, 19 }
                ,{ 4961, 408 }
                ,{ 5043, 322 }
                ,{ 5239, 398 }
                ,{ 5491, 389 }
                ,{ 5819, 367 }
                ,{ 5887, 356 }
                ,{ 6137, 377 }
                ,{ 6253, 397 }
                ,{ 6647, 388 }
                ,{ 6727, 344 }
                ,{ 6845, 333 }
                ,{ 6859, 18 }
                ,{ 6877, 366 }
                ,{ 6929, 396 }
                ,{ 8303, 376 }
                ,{ 8381, 387 }
                ,{ 8405, 321 }
                ,{ 8959, 386 }
                ,{ 8993, 365 }
                ,{ 9251, 355 }
                ,{ 9583, 332 }
                ,{ 10051, 364 }
                ,{ 10469, 375 }
                ,{ 10571, 343 }
                ,{ 10693, 385 }
                ,{ 10933, 354 }
                ,{ 11191, 374 }
                ,{ 11767, 320 }
                ,{ 11849, 384 }
                ,{ 12167, 17 }
                ,{ 12493, 342 }
                ,{ 13357, 373 }
                ,{ 14297, 353 }
                ,{ 14801, 372 }
                ,{ 15059, 331 }
                ,{ 15341, 363 }
                ,{ 15979, 352 }
                ,{ 16337, 341 }
                ,{ 16399, 362 }
                ,{ 17797, 330 }
                ,{ 18259, 340 }
                ,{ 18491, 319 }
                ,{ 19343, 351 }
                ,{ 19573, 361 }
                ,{ 21689, 360 }
                ,{ 21853, 318 }
                ,{ 22103, 339 }
                ,{ 23273, 329 }
                ,{ 24389, 16 }
                ,{ 26011, 328 }
                ,{ 26071, 350 }
                ,{ 27869, 338 }
                ,{ 28577, 317 }
                ,{ 29791, 15 }
                ,{ 31117, 349 }
                ,{ 31487, 327 }
                ,{ 31939, 316 }
                ,{ 34481, 348 }
                ,{ 35557, 337 }
                ,{ 38663, 315 }
                ,{ 39401, 336 }
                ,{ 39701, 326 }
                ,{ 42439, 325 }
                ,{ 48749, 314 }
                ,{ 50653, 14 }
                ,{ 52111, 313 }
                ,{ 56129, 324 }
                ,{ 62197, 312 }
                ,{ 68921, 13 }
            };

            sCardInfoData.fProductMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesDontCountLowBall ] =
            {
                 { 8, 12 }
                ,{ 12, 156 }
                ,{ 18, 144 }
                ,{ 20, 155 }
                ,{ 27, 11 }
                ,{ 28, 154 }
                ,{ 44, 153 }
                ,{ 45, 143 }
                ,{ 50, 132 }
                ,{ 52, 152 }
                ,{ 63, 142 }
                ,{ 68, 151 }
                ,{ 75, 131 }
                ,{ 76, 150 }
                ,{ 92, 149 }
                ,{ 98, 120 }
                ,{ 99, 141 }
                ,{ 116, 148 }
                ,{ 117, 140 }
                ,{ 124, 147 }
                ,{ 125, 10 }
                ,{ 147, 119 }
                ,{ 148, 146 }
                ,{ 153, 139 }
                ,{ 164, 157 }
                ,{ 171, 138 }
                ,{ 175, 130 }
                ,{ 207, 137 }
                ,{ 242, 108 }
                ,{ 245, 118 }
                ,{ 261, 136 }
                ,{ 275, 129 }
                ,{ 279, 135 }
                ,{ 325, 128 }
                ,{ 333, 134 }
                ,{ 338, 96 }
                ,{ 343, 9 }
                ,{ 363, 107 }
                ,{ 369, 145 }
                ,{ 425, 127 }
                ,{ 475, 126 }
                ,{ 507, 95 }
                ,{ 539, 117 }
                ,{ 575, 125 }
                ,{ 578, 84 }
                ,{ 605, 106 }
                ,{ 637, 116 }
                ,{ 722, 72 }
                ,{ 725, 124 }
                ,{ 775, 123 }
                ,{ 833, 115 }
                ,{ 845, 94 }
                ,{ 847, 105 }
                ,{ 867, 83 }
                ,{ 925, 122 }
                ,{ 931, 114 }
                ,{ 1025, 133 }
                ,{ 1058, 60 }
                ,{ 1083, 71 }
                ,{ 1127, 113 }
                ,{ 1183, 93 }
                ,{ 1331, 8 }
                ,{ 1421, 112 }
                ,{ 1445, 82 }
                ,{ 1519, 111 }
                ,{ 1573, 104 }
                ,{ 1587, 59 }
                ,{ 1682, 48 }
                ,{ 1805, 70 }
                ,{ 1813, 110 }
                ,{ 1859, 92 }
                ,{ 1922, 36 }
                ,{ 2009, 121 }
                ,{ 2023, 81 }
                ,{ 2057, 103 }
                ,{ 2197, 7 }
                ,{ 2299, 102 }
                ,{ 2523, 47 }
                ,{ 2527, 69 }
                ,{ 2645, 58 }
                ,{ 2738, 24 }
                ,{ 2783, 101 }
                ,{ 2873, 91 }
                ,{ 2883, 35 }
                ,{ 3179, 80 }
                ,{ 3211, 90 }
                ,{ 3362, 169 }
                ,{ 3509, 100 }
                ,{ 3703, 57 }
                ,{ 3751, 99 }
                ,{ 3757, 79 }
                ,{ 3887, 89 }
                ,{ 3971, 68 }
                ,{ 4107, 23 }
                ,{ 4205, 46 }
                ,{ 4477, 98 }
                ,{ 4693, 67 }
                ,{ 4805, 34 }
                ,{ 4901, 88 }
                ,{ 4913, 6 }
                ,{ 4961, 109 }
                ,{ 5043, 168 }
                ,{ 5239, 87 }
                ,{ 5491, 78 }
                ,{ 5819, 56 }
                ,{ 5887, 45 }
                ,{ 6137, 66 }
                ,{ 6253, 86 }
                ,{ 6647, 77 }
                ,{ 6727, 33 }
                ,{ 6845, 22 }
                ,{ 6859, 5 }
                ,{ 6877, 55 }
                ,{ 6929, 97 }
                ,{ 8303, 65 }
                ,{ 8381, 76 }
                ,{ 8405, 167 }
                ,{ 8959, 75 }
                ,{ 8993, 54 }
                ,{ 9251, 44 }
                ,{ 9583, 21 }
                ,{ 10051, 53 }
                ,{ 10469, 64 }
                ,{ 10571, 32 }
                ,{ 10693, 74 }
                ,{ 10933, 43 }
                ,{ 11191, 63 }
                ,{ 11767, 166 }
                ,{ 11849, 85 }
                ,{ 12167, 4 }
                ,{ 12493, 31 }
                ,{ 13357, 62 }
                ,{ 14297, 42 }
                ,{ 14801, 73 }
                ,{ 15059, 20 }
                ,{ 15341, 52 }
                ,{ 15979, 41 }
                ,{ 16337, 30 }
                ,{ 16399, 51 }
                ,{ 17797, 19 }
                ,{ 18259, 29 }
                ,{ 18491, 165 }
                ,{ 19343, 40 }
                ,{ 19573, 50 }
                ,{ 21689, 61 }
                ,{ 21853, 164 }
                ,{ 22103, 28 }
                ,{ 23273, 18 }
                ,{ 24389, 3 }
                ,{ 26011, 17 }
                ,{ 26071, 39 }
                ,{ 27869, 27 }
                ,{ 28577, 163 }
                ,{ 29791, 2 }
                ,{ 31117, 38 }
                ,{ 31487, 16 }
                ,{ 31939, 162 }
                ,{ 34481, 49 }
                ,{ 35557, 26 }
                ,{ 38663, 161 }
                ,{ 39401, 37 }
                ,{ 39701, 15 }
                ,{ 42439, 14 }
                ,{ 48749, 160 }
                ,{ 50653, 1 }
                ,{ 52111, 159 }
                ,{ 56129, 25 }
                ,{ 62197, 158 }
                ,{ 68921, 13 }
            };

            sCardInfoData.fProductMaps[ SCardInfoData::EWhichItem::eStraightsAndFlushesCountLowBall ] =
            {
                 { 8, 24 }
                ,{ 12, 454 }
                ,{ 18, 442 }
                ,{ 20, 453 }
                ,{ 27, 23 }
                ,{ 28, 452 }
                ,{ 44, 451 }
                ,{ 45, 441 }
                ,{ 50, 430 }
                ,{ 52, 450 }
                ,{ 63, 440 }
                ,{ 68, 449 }
                ,{ 75, 429 }
                ,{ 76, 448 }
                ,{ 92, 447 }
                ,{ 98, 418 }
                ,{ 99, 439 }
                ,{ 116, 446 }
                ,{ 117, 438 }
                ,{ 124, 445 }
                ,{ 125, 22 }
                ,{ 147, 417 }
                ,{ 148, 444 }
                ,{ 153, 437 }
                ,{ 164, 455 }
                ,{ 171, 436 }
                ,{ 175, 428 }
                ,{ 207, 435 }
                ,{ 242, 406 }
                ,{ 245, 416 }
                ,{ 261, 434 }
                ,{ 275, 427 }
                ,{ 279, 433 }
                ,{ 325, 426 }
                ,{ 333, 432 }
                ,{ 338, 394 }
                ,{ 343, 21 }
                ,{ 363, 405 }
                ,{ 369, 443 }
                ,{ 425, 425 }
                ,{ 475, 424 }
                ,{ 507, 393 }
                ,{ 539, 415 }
                ,{ 575, 423 }
                ,{ 578, 382 }
                ,{ 605, 404 }
                ,{ 637, 414 }
                ,{ 722, 370 }
                ,{ 725, 422 }
                ,{ 775, 421 }
                ,{ 833, 413 }
                ,{ 845, 392 }
                ,{ 847, 403 }
                ,{ 867, 381 }
                ,{ 925, 420 }
                ,{ 931, 412 }
                ,{ 1025, 431 }
                ,{ 1058, 358 }
                ,{ 1083, 369 }
                ,{ 1127, 411 }
                ,{ 1183, 391 }
                ,{ 1331, 20 }
                ,{ 1421, 410 }
                ,{ 1445, 380 }
                ,{ 1519, 409 }
                ,{ 1573, 402 }
                ,{ 1587, 357 }
                ,{ 1682, 346 }
                ,{ 1805, 368 }
                ,{ 1813, 408 }
                ,{ 1859, 390 }
                ,{ 1922, 334 }
                ,{ 2009, 419 }
                ,{ 2023, 379 }
                ,{ 2057, 401 }
                ,{ 2197, 19 }
                ,{ 2299, 400 }
                ,{ 2523, 345 }
                ,{ 2527, 367 }
                ,{ 2645, 356 }
                ,{ 2738, 322 }
                ,{ 2783, 399 }
                ,{ 2873, 389 }
                ,{ 2883, 333 }
                ,{ 3179, 378 }
                ,{ 3211, 388 }
                ,{ 3362, 467 }
                ,{ 3509, 398 }
                ,{ 3703, 355 }
                ,{ 3751, 397 }
                ,{ 3757, 377 }
                ,{ 3887, 387 }
                ,{ 3971, 366 }
                ,{ 4107, 321 }
                ,{ 4205, 344 }
                ,{ 4477, 396 }
                ,{ 4693, 365 }
                ,{ 4805, 332 }
                ,{ 4901, 386 }
                ,{ 4913, 18 }
                ,{ 4961, 407 }
                ,{ 5043, 466 }
                ,{ 5239, 385 }
                ,{ 5491, 376 }
                ,{ 5819, 354 }
                ,{ 5887, 343 }
                ,{ 6137, 364 }
                ,{ 6253, 384 }
                ,{ 6647, 375 }
                ,{ 6727, 331 }
                ,{ 6845, 320 }
                ,{ 6859, 17 }
                ,{ 6877, 353 }
                ,{ 6929, 395 }
                ,{ 8303, 363 }
                ,{ 8381, 374 }
                ,{ 8405, 465 }
                ,{ 8959, 373 }
                ,{ 8993, 352 }
                ,{ 9251, 342 }
                ,{ 9583, 319 }
                ,{ 10051, 351 }
                ,{ 10469, 362 }
                ,{ 10571, 330 }
                ,{ 10693, 372 }
                ,{ 10933, 341 }
                ,{ 11191, 361 }
                ,{ 11767, 464 }
                ,{ 11849, 383 }
                ,{ 12167, 16 }
                ,{ 12493, 329 }
                ,{ 13357, 360 }
                ,{ 14297, 340 }
                ,{ 14801, 371 }
                ,{ 15059, 318 }
                ,{ 15341, 350 }
                ,{ 15979, 339 }
                ,{ 16337, 328 }
                ,{ 16399, 349 }
                ,{ 17797, 317 }
                ,{ 18259, 327 }
                ,{ 18491, 463 }
                ,{ 19343, 338 }
                ,{ 19573, 348 }
                ,{ 21689, 359 }
                ,{ 21853, 462 }
                ,{ 22103, 326 }
                ,{ 23273, 316 }
                ,{ 24389, 15 }
                ,{ 26011, 315 }
                ,{ 26071, 337 }
                ,{ 27869, 325 }
                ,{ 28577, 461 }
                ,{ 29791, 14 }
                ,{ 31117, 336 }
                ,{ 31487, 314 }
                ,{ 31939, 460 }
                ,{ 34481, 347 }
                ,{ 35557, 324 }
                ,{ 38663, 459 }
                ,{ 39401, 335 }
                ,{ 39701, 313 }
                ,{ 42439, 312 }
                ,{ 48749, 458 }
                ,{ 50653, 13 }
                ,{ 52111, 457 }
                ,{ 56129, 323 }
                ,{ 62197, 456 }
                ,{ 68921, 25 }
            };

        }
    }

    uint32_t C3CardInfo::evaluateCardHand( const std::vector< std::shared_ptr< CCard > > & cards, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        initMaps();
        return sCardInfoData.evaluateCardHand( cards, playInfo, 3 );
    }

    EHand C3CardInfo::rankToCardHand( uint32_t rank, const std::shared_ptr< SPlayInfo > & playInfo )
    {
        EHand hand;
        if ( !playInfo->fStraightsAndFlushesCount && !playInfo->fLowHandWins )
        {
            if ( rank >= 170U )
                hand = EHand::eHighCard;
            else if ( rank >= 14U )
                hand = EHand::ePair;
            else /* if ( rank >= 1U ) */
                hand = EHand::eThreeOfAKind;
        }
        else if ( !playInfo->fStraightsAndFlushesCount && playInfo->fLowHandWins )
        {
            if ( rank >= 170U )
                hand = EHand::eHighCard;
            else if ( rank >= 14U )
                hand = EHand::ePair;
            else /* if ( rank >= 1U ) */
                hand = EHand::eThreeOfAKind;
        }
        else if ( playInfo->fStraightsAndFlushesCount && !playInfo->fLowHandWins )
        {
            if ( rank >= 468U )
                hand = EHand::eHighCard;
            else if ( rank >= 312U )
                hand = EHand::ePair;
            else if ( rank >= 38U )
                hand = EHand::eFlush;
            else if ( rank >= 26U )
                hand = EHand::eStraight;
            else if ( rank >= 13U )
                hand = EHand::eThreeOfAKind;
            else /* if ( rank >= 1U ) */
                hand = EHand::eStraightFlush;
        }
        else /* if ( playInfo->fStraightsAndFlushesCount && playInfo->fLowHandWins ) */
        {
            if ( rank >= 468U )
                hand = EHand::eHighCard;
            else if ( rank >= 312U )
                hand = EHand::ePair;
            else if ( rank >= 38U )
                hand = EHand::eFlush;
            else if ( rank >= 26U )
                hand = EHand::eStraight;
            else if ( rank >= 13U )
                hand = EHand::eThreeOfAKind;
            else /* if ( rank >= 1U ) */
                hand = EHand::eStraightFlush;
        }
        return hand;
    }
}

