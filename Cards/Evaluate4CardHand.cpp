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

#include "Evaluate4CardHand.h"
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
    // No Flushes/Straights
    static std::map< C4CardInfo::THand, uint32_t > sCardMap =
    {
         { { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } , { ECard::eAce, ESuit::eClubs } }, 1 } // EHand::eFourOfAKind
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } , { ECard::eKing, ESuit::eClubs } }, 2 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } , { ECard::eQueen, ESuit::eClubs } }, 3 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } , { ECard::eJack, ESuit::eClubs } }, 4 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } , { ECard::eTen, ESuit::eClubs } }, 5 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } , { ECard::eNine, ESuit::eClubs } }, 6 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } , { ECard::eEight, ESuit::eClubs } }, 7 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } , { ECard::eSeven, ESuit::eClubs } }, 8 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } , { ECard::eSix, ESuit::eClubs } }, 9 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } , { ECard::eFive, ESuit::eClubs } }, 10 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } , { ECard::eFour, ESuit::eClubs } }, 11 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } , { ECard::eTrey, ESuit::eClubs } }, 12 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } , { ECard::eDeuce, ESuit::eClubs } }, 13 } // EHand::eFourOfAKind
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 14 } // EHand::eThreeOfAKind
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 15 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 16 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 17 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 18 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 19 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 20 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 21 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 22 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 23 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 24 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 25 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 26 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 27 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 28 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 29 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 30 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 31 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 32 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 33 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 34 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 35 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 36 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 37 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 38 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 39 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 40 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 41 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 42 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 43 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 44 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 45 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 46 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 47 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 48 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 49 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 50 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 51 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 52 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 53 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 54 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 55 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 56 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 57 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 58 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 59 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 60 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 61 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 62 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 63 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 64 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 65 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 66 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 67 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 68 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 69 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 70 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 71 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 72 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 73 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 74 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 75 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 76 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 77 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 78 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 79 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 80 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 81 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 82 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 83 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 84 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 85 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 86 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 87 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 88 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 89 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 90 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 91 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 92 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 93 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 94 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 95 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 96 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 97 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 98 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 99 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 100 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 101 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 102 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 103 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 104 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 105 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 106 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 107 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 108 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 109 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 110 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 111 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 112 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 113 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 114 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 115 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 116 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 117 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 118 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 119 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 120 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 121 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 122 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 123 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 124 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 125 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 126 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 127 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 128 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 129 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 130 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 131 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 132 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 133 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 134 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 135 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 136 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 137 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 138 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 139 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 140 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 141 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 142 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 143 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 144 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 145 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 146 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 147 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 148 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 149 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 150 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 151 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 152 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 153 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 155 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 156 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 157 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 158 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 159 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 160 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 161 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 162 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 163 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 164 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 165 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 166 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 168 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 169 } // EHand::eThreeOfAKind
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 170 } // EHand::eTwoPair
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 171 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 172 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 173 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 174 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 175 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 176 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 177 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 178 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 179 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 180 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 181 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 182 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 183 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 184 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 185 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 186 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 187 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 188 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 189 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 190 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 191 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 192 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 193 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 194 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 195 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 196 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 197 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 198 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 199 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 200 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 201 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 202 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 203 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 204 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 205 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 206 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 207 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 208 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 209 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 210 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 211 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 212 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 213 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 214 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 215 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 216 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 217 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 218 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 219 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 220 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 221 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 222 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 223 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 224 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 225 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 226 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 227 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 228 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 229 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 230 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 231 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 232 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 233 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 234 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 235 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 236 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 237 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 238 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 239 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 240 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 241 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 242 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 243 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 244 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eFour, ESuit::eHearts } }, 245 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eFour, ESuit::eHearts } }, 246 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eTrey, ESuit::eHearts } }, 247 } // EHand::eTwoPair
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 248 } // EHand::ePair
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 249 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 250 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 251 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 252 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 253 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 254 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 255 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 256 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 257 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 258 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 259 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 260 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 261 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 262 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 263 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 264 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 265 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 266 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 267 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 268 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 269 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 270 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 271 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 272 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 273 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 274 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 275 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 276 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 277 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 278 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 279 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 280 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 281 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 282 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 283 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 284 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 285 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 286 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 287 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 288 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 289 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 290 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 291 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 292 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 293 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 294 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 295 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 296 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 297 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 298 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 299 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 300 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 301 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 302 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 303 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 304 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 305 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 306 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 307 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 308 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 309 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 310 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 311 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 312 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 313 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 314 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 315 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 316 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 317 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 318 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 319 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 320 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 321 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 322 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 323 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 324 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 325 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 326 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 327 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 328 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 329 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 330 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 331 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 332 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 333 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 334 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 335 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 336 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 337 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 338 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 339 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 340 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 341 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 342 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 343 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 344 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 345 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 346 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 347 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 348 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 349 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 350 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 351 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 352 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 353 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 354 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 355 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 356 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 357 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 358 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 359 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 360 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 361 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 362 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 363 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 364 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 365 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 366 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 367 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 368 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 369 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 370 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 371 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 372 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 373 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 374 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 375 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 376 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 377 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 378 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 379 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 380 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 381 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 382 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 383 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 384 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 385 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 386 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 387 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 388 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 389 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 390 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 391 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 392 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 393 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 394 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 395 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 396 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 397 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 398 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 399 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 400 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 401 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 402 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 403 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 404 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 405 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 406 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 407 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 408 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 409 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 410 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 411 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 412 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 413 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 414 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 415 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 416 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 417 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 418 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 419 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 420 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 421 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 422 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 423 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 424 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 425 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 426 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 427 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 428 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 429 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 430 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 431 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 432 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 433 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 434 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 435 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 436 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 437 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 438 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 439 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 440 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 441 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 442 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 443 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 444 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 445 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 446 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 447 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 448 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 449 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 450 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 451 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 452 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 453 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 454 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 455 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 456 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 457 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 458 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 459 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 460 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 461 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 462 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 463 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 464 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 465 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 466 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 467 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 468 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 469 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 470 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 471 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 472 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 473 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 474 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 475 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 476 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 477 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 478 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 479 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 480 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 481 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 482 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 483 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 484 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 485 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 486 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 487 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 488 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 489 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 490 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 491 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 492 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 493 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 494 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 495 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 496 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 497 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 498 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 499 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 500 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 501 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 502 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 503 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 504 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 505 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 506 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 507 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 508 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 509 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 510 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 511 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 512 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 513 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 514 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 515 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 516 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 517 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 518 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 519 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 520 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 521 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 522 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 523 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 524 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 525 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 526 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 527 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 528 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 529 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 530 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 531 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 532 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 533 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 534 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 535 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 536 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 537 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 538 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 539 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 540 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 541 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 542 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 543 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 544 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 545 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 546 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 547 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 548 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 549 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 550 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 551 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 552 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 553 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 554 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 555 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 556 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 557 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 558 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 559 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 560 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 561 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 562 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 563 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 564 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 565 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 566 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 567 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 568 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 569 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 570 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 571 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 572 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 573 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 574 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 575 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 576 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 577 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 578 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 579 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 580 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 581 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 582 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 583 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 584 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 585 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 586 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 587 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 588 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 589 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 590 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 591 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 592 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 593 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 594 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 595 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 596 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 597 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 598 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 599 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 600 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 601 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 602 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 603 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 604 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 605 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 606 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 607 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 608 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 609 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 610 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 611 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 612 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 613 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 614 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 615 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 616 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 617 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 618 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 619 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 620 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 621 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 622 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 623 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 624 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 625 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 626 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 627 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 628 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 629 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 630 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 631 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 632 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 633 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 634 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 635 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 636 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 637 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 638 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 639 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 640 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 641 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 642 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 643 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 644 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 645 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 646 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 647 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 648 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 649 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 650 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 651 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 652 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 653 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 654 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 655 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 656 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 657 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 658 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 659 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 660 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 661 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 662 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 663 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 664 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 665 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 666 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 667 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 668 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 669 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 670 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 671 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 672 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 673 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 674 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 675 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 676 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 677 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 678 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 679 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 680 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 681 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 682 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 683 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 684 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 685 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 686 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 687 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 688 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 689 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 690 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 691 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 692 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 693 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 694 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 695 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 696 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 697 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 698 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 699 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 700 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 701 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 702 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 703 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 704 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 705 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 706 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 707 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 708 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 709 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 710 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 711 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 712 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 713 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 714 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 715 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 716 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 717 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 718 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 719 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 720 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 721 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 722 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 723 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 724 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 725 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 726 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 727 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 728 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 729 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 730 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 731 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 732 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 733 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 734 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 735 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 736 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 737 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 738 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 739 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 740 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 741 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 742 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 743 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 744 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 745 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 746 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 747 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 748 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 749 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 750 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 751 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 752 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 753 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 754 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 755 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 756 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 757 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 758 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 759 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 760 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 761 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 762 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 763 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 764 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 765 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 766 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 767 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 768 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 769 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 770 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 771 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 772 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 773 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 774 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 775 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 776 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 777 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 778 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 779 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 780 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 781 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 782 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 783 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 784 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 785 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 786 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 787 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 788 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 789 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 790 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 791 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 792 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 793 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 794 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 795 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 796 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 797 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 798 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 799 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 800 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 801 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 802 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 803 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 804 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 805 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 806 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 807 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 808 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 809 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 810 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 811 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 812 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 813 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 814 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 815 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 816 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 817 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 818 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 819 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 820 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 821 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 822 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 823 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 824 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 825 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 826 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 827 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 828 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 829 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 830 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 831 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 832 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 833 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 834 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 835 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 836 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 837 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 838 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 839 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 840 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 841 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 842 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 843 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 844 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 845 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 846 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 847 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 848 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 849 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 850 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 851 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 852 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 853 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 854 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 855 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 856 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 857 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 858 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 859 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 860 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 861 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 862 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 863 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 864 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 865 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 866 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 867 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 868 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 869 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 870 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 871 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 872 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 873 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 874 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 875 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 876 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 877 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 878 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 879 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 880 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 881 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 882 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 883 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 884 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 885 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 886 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 887 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 888 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 889 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 890 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 891 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 892 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 893 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 894 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 895 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 896 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 897 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 898 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 899 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 900 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 901 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 902 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 903 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 904 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 905 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 906 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 907 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 908 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 909 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 910 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 911 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 912 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 913 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 914 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 915 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 916 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 917 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 918 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 919 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 920 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 921 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 922 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 923 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 924 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 925 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 926 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 927 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 928 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 929 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 930 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 931 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 932 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 933 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 934 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 935 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 936 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 937 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 938 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 939 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 940 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 941 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 942 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 943 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 944 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 945 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 946 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 947 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 948 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 949 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 950 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 951 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 952 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 953 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 954 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 955 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 956 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 957 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 958 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 959 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 960 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 961 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 962 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 963 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 964 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 965 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 966 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 967 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 968 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 969 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 970 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 971 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 972 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 973 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 974 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 975 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 976 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 977 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 978 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 979 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 980 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 981 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 982 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 983 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 984 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 985 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 986 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 987 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 988 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 989 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 990 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 991 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 992 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 993 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 994 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 995 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 996 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 997 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 998 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 999 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1000 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1001 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1002 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1003 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1004 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1005 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1006 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1007 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1008 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1009 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1010 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1011 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1012 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1013 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1014 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1015 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1016 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1017 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1018 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1019 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1020 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1021 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1022 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1023 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1024 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1025 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1026 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1027 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1028 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1029 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1030 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1031 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1032 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1033 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1034 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1035 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1036 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1037 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1038 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1039 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1040 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1041 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1042 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1043 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1044 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1045 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1046 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1047 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1048 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1049 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1050 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1051 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1052 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1053 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1054 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1055 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1056 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1057 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1058 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1059 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1060 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1061 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1062 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1063 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1064 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1065 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1066 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1067 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1068 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1069 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1070 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1071 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1072 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1073 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1074 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1075 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1076 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1077 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1078 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1079 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1080 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1081 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1082 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1083 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1084 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1085 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1086 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1087 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1088 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1089 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1090 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1091 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1092 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1093 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1094 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1095 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1096 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1097 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1098 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1099 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1100 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1101 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1102 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1103 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1104 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1105 } // EHand::ePair
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1106 } // EHand::eHighCard
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1107 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1108 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1109 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1110 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1111 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1112 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1113 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1114 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1115 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1116 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1117 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1118 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1119 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1120 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1121 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1122 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1123 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1124 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1125 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1126 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1127 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1128 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1129 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1130 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1131 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1132 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1133 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1134 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1135 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1136 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1137 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1138 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1139 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1140 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1141 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1142 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1143 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1144 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1145 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1146 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1147 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1148 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1149 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1150 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1151 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1152 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1153 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1154 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1155 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1156 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1157 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1158 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1159 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1160 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1161 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1162 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1163 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1164 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1165 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1166 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1167 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1168 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1169 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1170 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1171 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1172 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1173 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1174 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1175 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1176 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1177 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1178 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1179 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1180 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1181 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1182 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1183 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1184 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1185 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1186 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1187 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1188 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1189 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1190 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1191 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1192 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1193 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1194 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1195 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1196 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1197 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1198 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1199 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1200 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1201 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1202 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1203 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1204 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1205 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1206 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1207 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1208 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1209 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1210 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1211 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1212 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1213 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1214 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1215 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1216 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1217 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1218 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1219 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1220 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1221 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1222 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1223 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1224 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1225 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1226 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1227 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1228 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1229 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1230 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1231 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1232 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1233 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1234 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1235 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1236 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1237 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1238 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1239 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1240 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1241 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1242 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1243 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1244 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1245 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1246 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1247 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1248 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1249 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1250 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1251 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1252 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1253 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1254 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1255 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1256 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1257 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1258 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1259 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1260 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1261 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1262 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1263 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1264 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1265 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1266 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1267 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1268 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1269 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1270 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1271 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1272 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1273 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1274 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1275 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1276 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1277 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1278 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1279 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1280 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1281 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1282 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1283 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1284 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1285 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1286 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1287 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1288 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1289 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1290 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1291 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1292 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1293 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1294 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1295 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1296 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1297 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1298 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1299 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1300 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1301 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1302 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1303 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1304 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1305 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1306 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1307 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1308 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1309 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1310 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1311 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1312 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1313 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1314 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1315 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1316 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1317 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1318 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1319 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1320 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1321 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1322 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1323 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1324 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 1325 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1326 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1327 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1328 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1329 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1330 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1331 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1332 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1333 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1334 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1335 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1336 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1337 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1338 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1339 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1340 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1341 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1342 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1343 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1344 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1345 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1346 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1347 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1348 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1349 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1350 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1351 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1352 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1353 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1354 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1355 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1356 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1357 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1358 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1359 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1360 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1361 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1362 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1363 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1364 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1365 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1366 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1367 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1368 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1369 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1370 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1371 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1372 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1373 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1374 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1375 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1376 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1377 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1378 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1379 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1380 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1381 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1382 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1383 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1384 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1385 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1386 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1387 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1388 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1389 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1390 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1391 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1392 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1393 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1394 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1395 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1396 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1397 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1398 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1399 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1400 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1401 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1402 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1403 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1404 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1405 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1406 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1407 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1408 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1409 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1410 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1411 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1412 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1413 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1414 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1415 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1416 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1417 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1418 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1419 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1420 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1421 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1422 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1423 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1424 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1425 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1426 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1427 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1428 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1429 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1430 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1431 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1432 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1433 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1434 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1435 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1436 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1437 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1438 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1439 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1440 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1441 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1442 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1443 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1444 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1445 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1446 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1447 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1448 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1449 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1450 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1451 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1452 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1453 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1454 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1455 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1456 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1457 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1458 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1459 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1460 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1461 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1462 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1463 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1464 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1465 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1466 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1467 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1468 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1469 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1470 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1471 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1472 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1473 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1474 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1475 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1476 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1477 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1478 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1479 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1480 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1481 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1482 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1483 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1484 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1485 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1486 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1487 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1488 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1489 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 1490 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1491 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1492 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1493 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1494 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1495 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1496 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1497 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1498 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1499 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1500 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1501 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1502 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1503 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1504 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1505 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1506 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1507 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1508 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1509 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1510 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1511 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1512 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1513 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1514 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1515 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1516 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1517 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1518 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1519 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1520 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1521 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1522 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1523 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1524 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1525 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1526 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1527 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1528 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1529 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1530 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1531 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1532 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1533 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1534 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1535 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1536 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1537 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1538 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1539 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1540 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1541 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1542 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1543 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1544 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1545 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1546 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1547 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1548 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1549 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1550 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1551 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1552 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1553 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1554 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1555 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1556 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1557 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1558 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1559 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1560 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1561 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1562 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1563 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1564 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1565 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1566 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1567 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1568 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1569 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1570 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1571 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1572 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1573 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1574 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1575 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1576 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1577 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1578 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1579 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1580 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1581 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1582 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1583 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1584 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1585 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1586 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1587 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1588 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1589 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1590 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1591 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1592 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1593 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1594 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1595 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1596 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1597 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1598 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1599 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1600 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1601 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1602 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1603 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1604 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1605 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1606 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1607 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1608 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1609 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 1610 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1611 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1612 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1613 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1614 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1615 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1616 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1617 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1618 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1619 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1620 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1621 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1622 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1623 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1624 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1625 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1626 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1627 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1628 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1629 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1630 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1631 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1632 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1633 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1634 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1635 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1636 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1637 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1638 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1639 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1640 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1641 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1642 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1643 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1644 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1645 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1646 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1647 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1648 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1649 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1650 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1651 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1652 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1653 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1654 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1655 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1656 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1657 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1658 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1659 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1660 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1661 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1662 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1663 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1664 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1665 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1666 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1667 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1668 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1669 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1670 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1671 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1672 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1673 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1674 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1675 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1676 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1677 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1678 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1679 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1680 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1681 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1682 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1683 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1684 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1685 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1686 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1687 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1688 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1689 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1690 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1691 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1692 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1693 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 1694 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1695 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1696 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1697 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1698 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1699 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1700 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1701 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1702 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1703 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1704 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1705 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1706 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1707 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1708 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1709 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1710 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1711 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1712 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1713 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1714 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1715 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1716 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1717 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1718 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1719 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1720 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1721 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1722 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1723 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1724 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1725 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1726 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1727 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1728 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1729 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1730 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1731 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1732 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1733 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1734 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1735 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1736 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1737 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1738 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1739 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1740 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1741 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1742 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1743 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1744 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1745 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1746 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1747 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1748 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1749 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 1750 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1751 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1752 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1753 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1754 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1755 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1756 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1757 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1758 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1759 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1760 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1761 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1762 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1763 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1764 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1765 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1766 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1767 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1768 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1769 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1770 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1771 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1772 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1773 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1774 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1775 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1776 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1777 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1778 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1779 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1780 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1781 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1782 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1783 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1784 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 1785 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1786 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1787 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1788 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1789 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1790 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1791 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1792 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1793 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1794 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1795 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1796 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1797 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1798 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1799 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1800 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1801 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1802 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1803 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1804 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 1805 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1806 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1807 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1808 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1809 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1810 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1811 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1812 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1813 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1814 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 1815 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 1816 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 1817 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 1818 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 1819 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eFive, ESuit::eSpades } }, 1820 } // EHand::eHighCard
    };

    // Flushes/Straights
    static std::map< C4CardInfo::THand, uint32_t > sCardMapStraightsAndFlushesCount =
    {
         { { { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts }, { ECard::eAce, ESuit::eDiamonds } , { ECard::eAce, ESuit::eClubs } }, 1 } // EHand::eFourOfAKind
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts }, { ECard::eKing, ESuit::eDiamonds } , { ECard::eKing, ESuit::eClubs } }, 2 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts }, { ECard::eQueen, ESuit::eDiamonds } , { ECard::eQueen, ESuit::eClubs } }, 3 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts }, { ECard::eJack, ESuit::eDiamonds } , { ECard::eJack, ESuit::eClubs } }, 4 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts }, { ECard::eTen, ESuit::eDiamonds } , { ECard::eTen, ESuit::eClubs } }, 5 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts }, { ECard::eNine, ESuit::eDiamonds } , { ECard::eNine, ESuit::eClubs } }, 6 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts }, { ECard::eEight, ESuit::eDiamonds } , { ECard::eEight, ESuit::eClubs } }, 7 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts }, { ECard::eSeven, ESuit::eDiamonds } , { ECard::eSeven, ESuit::eClubs } }, 8 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts }, { ECard::eSix, ESuit::eDiamonds } , { ECard::eSix, ESuit::eClubs } }, 9 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts }, { ECard::eFive, ESuit::eDiamonds } , { ECard::eFive, ESuit::eClubs } }, 10 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts }, { ECard::eFour, ESuit::eDiamonds } , { ECard::eFour, ESuit::eClubs } }, 11 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts }, { ECard::eTrey, ESuit::eDiamonds } , { ECard::eTrey, ESuit::eClubs } }, 12 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts }, { ECard::eDeuce, ESuit::eDiamonds } , { ECard::eDeuce, ESuit::eClubs } }, 13 } // EHand::eFourOfAKind
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 14 } // EHand::eStraightFlush
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 15 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 16 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 17 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 18 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 19 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 20 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 21 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 22 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eFive, ESuit::eSpades } }, 23 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 24 } // EHand::eStraightFlush
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 25 } // EHand::eThreeOfAKind
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 26 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 27 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 28 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 29 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 30 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 31 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 32 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 33 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 34 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 35 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eAce, ESuit::eHearts } , { ECard::eAce, ESuit::eDiamonds } }, 36 }
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 37 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 38 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 39 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 40 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 41 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 42 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 43 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 44 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 45 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 46 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 47 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eKing, ESuit::eDiamonds } }, 48 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 49 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 50 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 51 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 52 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 53 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 54 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 55 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 56 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 57 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 58 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 59 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eQueen, ESuit::eDiamonds } }, 60 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 61 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 62 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 63 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 64 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 65 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 66 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 67 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 68 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 69 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 70 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 71 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eJack, ESuit::eDiamonds } }, 72 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 73 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 74 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 75 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 76 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 77 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 78 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 79 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 80 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 81 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 82 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 83 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eTen, ESuit::eDiamonds } }, 84 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 85 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 86 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 87 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 88 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 89 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 90 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 91 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 92 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 93 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 94 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 95 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eNine, ESuit::eDiamonds } }, 96 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 97 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 98 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 99 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 100 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 101 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 102 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 103 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 104 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 105 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 106 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 107 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eEight, ESuit::eDiamonds } }, 108 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 109 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 110 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 111 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 112 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 113 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 114 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 115 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 116 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 117 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 118 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 119 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eSeven, ESuit::eDiamonds } }, 120 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 121 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 122 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 123 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 124 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 125 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 126 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 127 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 128 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 129 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 130 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 131 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSix, ESuit::eDiamonds } }, 132 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 133 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 134 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 135 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 136 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 137 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 138 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 139 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 140 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 141 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 142 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 143 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eFive, ESuit::eDiamonds } }, 144 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 145 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 146 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 147 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 148 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 149 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 150 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 151 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 152 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 153 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 155 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFour, ESuit::eDiamonds } }, 156 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 157 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 158 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 159 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 160 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 161 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 162 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 163 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 164 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 165 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 166 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTrey, ESuit::eDiamonds } }, 168 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 169 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 170 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 171 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 172 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 173 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 174 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 175 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 176 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 177 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 178 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 179 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eDeuce, ESuit::eDiamonds } }, 180 } // EHand::eThreeOfAKind
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 181 } // EHand::eStraight
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 182 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 183 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 184 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 185 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 186 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 187 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 188 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 189 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 190 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 191 } // EHand::eStraight
        ,{ { { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eKing, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 192 } // EHand::eTwoPair
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 193 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 194 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 195 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 196 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 197 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 198 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 199 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 200 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 201 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 202 }
        ,{ { { ECard::eAce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eAce, ESuit::eHearts } }, 203 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eQueen, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 204 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 205 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 206 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 207 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 208 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 209 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 210 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 211 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 212 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 213 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eKing, ESuit::eHearts } }, 214 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eJack, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 215 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 216 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 217 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 218 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 219 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 220 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 221 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 222 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 223 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eQueen, ESuit::eHearts } }, 224 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTen, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 225 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 226 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 227 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 228 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 229 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 230 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 231 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 232 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eJack, ESuit::eHearts } }, 233 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eNine, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 234 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 235 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 236 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 237 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 238 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 239 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 240 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eTen, ESuit::eHearts } }, 241 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eEight, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 242 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 243 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 244 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 245 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 246 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 247 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eNine, ESuit::eHearts } }, 248 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSeven, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 249 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 250 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 251 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 252 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 253 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eEight, ESuit::eHearts } }, 254 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eSix, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 255 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 256 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 257 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 258 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eSeven, ESuit::eHearts } }, 259 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFive, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 260 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 261 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 262 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eSix, ESuit::eHearts } }, 263 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eFour, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 264 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 265 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eFive, ESuit::eHearts } }, 266 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTrey, ESuit::eHearts } , { ECard::eFour, ESuit::eHearts } }, 267 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eFour, ESuit::eHearts } }, 268 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eDeuce, ESuit::eHearts } , { ECard::eTrey, ESuit::eHearts } }, 269 } // EHand::eTwoPair
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 270 } // EHand::eFlush
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 271 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 272 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 273 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 274 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 275 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 276 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 277 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 278 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 279 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 280 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 281 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 282 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 283 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 284 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 285 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 286 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 287 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 288 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 289 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 290 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 291 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 292 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 293 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 294 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 295 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 296 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 297 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 298 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 299 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 300 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 301 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 302 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 303 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 304 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 305 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 306 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 307 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 308 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 309 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 310 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 311 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 312 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 313 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 314 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 315 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 316 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 317 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 318 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 319 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 320 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 321 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 322 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 323 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 324 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 325 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 326 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 327 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 328 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 329 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 330 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 331 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 332 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 333 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 334 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 335 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 336 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 337 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 338 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 339 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 340 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 341 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 342 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 343 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 344 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 345 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 346 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 347 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 348 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 349 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 350 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 351 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 352 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 353 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 354 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 355 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 356 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 357 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 358 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 359 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 360 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 361 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 362 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 363 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 364 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 365 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 366 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 367 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 368 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 369 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 370 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 371 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 372 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 373 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 374 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 375 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 376 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 377 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 378 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 379 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 380 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 381 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 382 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 383 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 384 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 385 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 386 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 387 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 388 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 389 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 390 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 391 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 392 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 393 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 394 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 395 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 396 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 397 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 398 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 399 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 400 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 401 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 402 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 403 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 404 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 405 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 406 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 407 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 408 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 409 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 410 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 411 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 412 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 413 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 414 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 415 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 416 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 417 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 418 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 419 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 420 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 421 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 422 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 423 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 424 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 425 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 426 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 427 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 428 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 429 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 430 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 431 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 432 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 433 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 434 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 435 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 436 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 437 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 438 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 439 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 440 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 441 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 442 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 443 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 444 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 445 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 446 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 447 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 448 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 449 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 450 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 451 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 452 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 453 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 454 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 455 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 456 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 457 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 458 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 459 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 460 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 461 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 462 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 463 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 464 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 465 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 466 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 467 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 468 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 469 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 470 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 471 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 472 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 473 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 474 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 475 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 476 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 477 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 478 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 479 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 480 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 481 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 482 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 483 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 484 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 485 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 486 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eSpades } }, 487 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 488 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 489 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 490 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 491 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 492 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 493 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 494 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 495 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 496 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 497 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 498 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 499 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 500 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 501 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 502 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 503 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 504 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 505 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 506 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 507 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 508 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 509 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 510 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 511 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 512 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 513 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 514 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 515 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 516 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 517 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 518 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 519 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 520 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 521 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 522 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 523 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 524 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 525 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 526 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 527 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 528 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 529 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 530 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 531 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 532 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 533 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 534 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 535 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 536 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 537 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 538 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 539 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 540 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 541 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 542 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 543 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 544 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 545 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 546 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 547 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 548 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 549 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 550 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 551 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 552 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 553 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 554 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 555 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 556 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 557 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 558 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 559 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 560 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 561 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 562 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 563 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 564 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 565 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 566 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 567 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 568 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 569 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 570 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 571 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 572 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 573 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 574 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 575 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 576 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 577 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 578 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 579 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 580 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 581 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 582 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 583 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 584 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 585 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 586 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 587 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 588 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 589 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 590 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 591 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 592 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 593 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 594 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 595 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 596 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 597 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 598 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 599 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 600 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 601 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 602 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 603 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 604 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 605 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 606 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 607 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 608 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 609 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 610 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 611 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 612 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 613 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 614 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 615 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 616 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 617 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 618 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 619 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 620 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 621 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 622 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 623 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 624 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 625 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 626 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 627 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 628 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 629 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 630 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 631 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 632 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 633 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 634 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 635 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 636 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 637 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 638 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 639 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 640 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 641 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 642 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 643 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 644 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 645 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 646 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 647 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 648 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 649 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 650 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eKing, ESuit::eSpades } }, 651 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 652 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 653 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 654 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 655 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 656 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 657 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 658 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 659 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 660 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 661 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 662 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 663 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 664 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 665 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 666 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 667 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 668 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 669 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 670 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 671 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 672 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 673 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 674 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 675 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 676 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 677 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 678 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 679 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 680 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 681 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 682 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 683 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 684 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 685 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 686 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 687 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 688 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 689 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 690 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 691 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 692 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 693 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 694 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 695 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 696 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 697 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 698 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 699 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 700 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 701 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 702 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 703 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 704 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 705 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 706 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 707 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 708 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 709 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 710 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 711 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 712 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 713 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 714 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 715 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 716 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 717 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 718 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 719 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 720 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 721 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 722 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 723 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 724 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 725 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 726 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 727 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 728 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 729 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 730 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 731 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 732 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 733 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 734 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 735 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 736 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 737 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 738 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 739 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 740 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 741 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 742 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 743 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 744 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 745 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 746 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 747 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 748 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 749 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 750 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 751 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 752 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 753 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 754 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 755 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 756 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 757 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 758 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 759 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 760 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 761 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 762 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 763 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 764 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 765 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 766 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 767 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 768 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 769 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eQueen, ESuit::eSpades } }, 770 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 771 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 772 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 773 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 774 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 775 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 776 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 777 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 778 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 779 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 780 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 781 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 782 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 783 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 784 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 785 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 786 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 787 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 788 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 789 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 790 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 791 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 792 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 793 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 794 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 795 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 796 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 797 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 798 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 799 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 800 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 801 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 802 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 803 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 804 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 805 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 806 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 807 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 808 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 809 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 810 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 811 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 812 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 813 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 814 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 815 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 816 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 817 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 818 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 819 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 820 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 821 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 822 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 823 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 824 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 825 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 826 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 827 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 828 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 829 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 830 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 831 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 832 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 833 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 834 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 835 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 836 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 837 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 838 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 839 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 840 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 841 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 842 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 843 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 844 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 845 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 846 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 847 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 848 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 849 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 850 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 851 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 852 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eJack, ESuit::eSpades } }, 853 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 854 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 855 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 856 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 857 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 858 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 859 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 860 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 861 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 862 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 863 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 864 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 865 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 866 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 867 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 868 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 869 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 870 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 871 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 872 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 873 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 874 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 875 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 876 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 877 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 878 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 879 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 880 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 881 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 882 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 883 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 884 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 885 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 886 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 887 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 888 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 889 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 890 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 891 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 892 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 893 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 894 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 895 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 896 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 897 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 898 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 899 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 900 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 901 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 902 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 903 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 904 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 905 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 906 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 907 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eTen, ESuit::eSpades } }, 908 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 909 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 910 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 911 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 912 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 913 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 914 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 915 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 916 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 917 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 918 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 919 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 920 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 921 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 922 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 923 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 924 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 925 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 926 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 927 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 928 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 929 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 930 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 931 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 932 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 933 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 934 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 935 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 936 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 937 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 938 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 939 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 940 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 941 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eNine, ESuit::eSpades } }, 942 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 943 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 944 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 945 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 946 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 947 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 948 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 949 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 950 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 951 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 952 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 953 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 954 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 955 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 956 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 957 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 958 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 959 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 960 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eEight, ESuit::eSpades } }, 961 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 962 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 963 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 964 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 965 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 966 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 967 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 968 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 969 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSeven, ESuit::eSpades } }, 970 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 971 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 972 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSix, ESuit::eSpades } }, 973 } // EHand::eFlush
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 974 } // EHand::ePair
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 975 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 976 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 977 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 978 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 979 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 980 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 981 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 982 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 983 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 984 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 985 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 986 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 987 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 988 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 989 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 990 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 991 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 992 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 993 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 994 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 995 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 996 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 997 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 998 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 999 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1000 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1001 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1002 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1003 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1004 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1005 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1006 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1007 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1008 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1009 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1010 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1011 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1012 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1013 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1014 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1015 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1016 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1017 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1018 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1019 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1020 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1021 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1022 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1023 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1024 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1025 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1026 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1027 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1028 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1029 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1030 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1031 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1032 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1033 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1034 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1035 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1036 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1037 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1038 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1039 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1040 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1041 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1042 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1043 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1044 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1045 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1046 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1047 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1048 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1049 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1050 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1051 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1052 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1053 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1054 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1055 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1056 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1057 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1058 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1059 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1060 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1061 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1062 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1063 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1064 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1065 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1066 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1067 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1068 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1069 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1070 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1071 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1072 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1073 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1074 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1075 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1076 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1077 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1078 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1079 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1080 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1081 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1082 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1083 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1084 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1085 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1086 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1087 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1088 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1089 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1090 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1091 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1092 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1093 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1094 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1095 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1096 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1097 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1098 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1099 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1100 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1101 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1102 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1103 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1104 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 1105 }
        ,{ { { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1106 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1107 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1108 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1109 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1110 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1111 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1112 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1113 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1114 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1115 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1116 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1117 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1118 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1119 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1120 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1121 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1122 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1123 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1124 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1125 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1126 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1127 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1128 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1129 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1130 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1131 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1132 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1133 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1134 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1135 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1136 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1137 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1138 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1139 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1140 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1141 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1142 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1143 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1144 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1145 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1146 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1147 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1148 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1149 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1150 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1151 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1152 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1153 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1155 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1156 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1157 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1158 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1159 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1160 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1161 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1162 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1163 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1164 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1165 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1166 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1168 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1169 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1170 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 1171 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1172 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1173 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1174 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1175 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1176 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1177 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1178 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1179 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1180 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1181 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1182 }
        ,{ { { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1183 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1184 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1185 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1186 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1187 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1188 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1189 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1190 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1191 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1192 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1193 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1194 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1195 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1196 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1197 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1198 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1199 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1200 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1201 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1202 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1203 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1204 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1205 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1206 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1207 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1208 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1209 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1210 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1211 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1212 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1213 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1214 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1215 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1216 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1217 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1218 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1219 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1220 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1221 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1222 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1223 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1224 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1225 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1226 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1227 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1228 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1229 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1230 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1231 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1232 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1233 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1234 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1235 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1236 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 1237 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1238 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1239 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1240 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1241 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1242 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1243 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1244 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1245 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1246 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1247 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1248 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1249 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1250 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1251 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1252 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1253 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1254 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1255 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1256 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1257 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1258 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1259 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1260 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1261 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1262 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1263 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1264 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1265 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1266 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1267 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1268 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1269 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1270 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1271 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1272 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1273 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1274 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1275 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1276 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1277 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1278 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1279 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1280 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1281 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1282 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1283 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1284 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1285 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1286 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1287 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1288 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1289 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1290 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1291 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1292 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1293 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1294 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1295 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1296 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1297 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1298 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1299 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1300 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1301 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1302 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 1303 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1304 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1305 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1306 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1307 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1308 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1309 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1310 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1311 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1312 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1313 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1314 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1315 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1316 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1317 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1318 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1319 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1320 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1321 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1322 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1323 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1324 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1325 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1326 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1327 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1328 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1329 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1330 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1331 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1332 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1333 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1334 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1335 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1336 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1337 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1338 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1339 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1340 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1341 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1342 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1343 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1344 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1345 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1346 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1347 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1348 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1349 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1350 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1351 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1352 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1353 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1354 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1355 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1356 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1357 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1358 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1359 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1360 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1361 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1362 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1363 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1364 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1365 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1366 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1367 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1368 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 1369 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1370 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1371 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1372 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1373 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1374 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1375 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1376 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1377 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1378 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1379 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1380 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1381 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1382 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1383 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1384 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1385 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1386 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1387 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1388 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1389 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1390 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1391 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1392 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1393 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1394 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1395 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1396 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1397 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1398 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1399 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1400 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1401 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1402 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1403 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1404 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1405 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1406 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1407 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1408 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1409 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1410 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1411 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1412 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1413 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1414 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1415 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1416 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1417 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1418 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1419 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1420 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1421 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1422 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1423 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1424 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1425 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1426 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1427 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1428 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1429 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1430 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1431 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1432 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1433 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1434 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 1435 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1436 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1437 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1438 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1439 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1440 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1441 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1442 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1443 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1444 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1445 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1446 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1447 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1448 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1449 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1450 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1451 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1452 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1453 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1454 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1455 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1456 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1457 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1458 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1459 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1460 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1461 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1462 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1463 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1464 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1465 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1466 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1467 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1468 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1469 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1470 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1471 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1472 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1473 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1474 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1475 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1476 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1477 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1478 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1479 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1480 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1481 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1482 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1483 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1484 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1485 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1486 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1487 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1488 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1489 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1490 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1491 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1492 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1493 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1494 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1495 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1496 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1497 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1498 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1499 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1500 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 1501 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1502 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1503 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1504 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1505 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1506 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1507 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1508 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1509 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1510 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1511 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1512 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1513 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1514 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1515 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1516 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1517 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1518 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1519 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1520 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1521 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1522 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1523 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1524 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1525 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1526 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1527 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1528 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1529 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1530 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1531 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1532 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1533 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1534 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1535 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1536 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1537 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1538 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1539 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1540 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1541 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1542 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1543 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1544 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1545 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1546 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1547 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1548 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1549 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1550 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1551 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1552 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1553 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1554 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1555 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1556 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1557 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1558 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1559 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1560 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1561 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1562 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1563 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1564 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1565 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1566 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 1567 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1568 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1569 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1570 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1571 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1572 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1573 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1574 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1575 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1576 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1577 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1578 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1579 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1580 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1581 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1582 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1583 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1584 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1585 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1586 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1587 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1588 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1589 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1590 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1591 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1592 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1593 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1594 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1595 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1596 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1597 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1598 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1599 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1600 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1601 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1602 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1603 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1604 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1605 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1606 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1607 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1608 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1609 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1610 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1611 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1612 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1613 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1614 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1615 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1616 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1617 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1618 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1619 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1620 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1621 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1622 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1623 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1624 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1625 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1626 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1627 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1628 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1629 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1630 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1631 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1632 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFive, ESuit::eHearts } }, 1633 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1634 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1635 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1636 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1637 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1638 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1639 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1640 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1641 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1642 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1643 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1644 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1645 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1646 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1647 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1648 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1649 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1650 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1651 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1652 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1653 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1654 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1655 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1656 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1657 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1658 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1659 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1660 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1661 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1662 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1663 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1664 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1665 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1666 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1667 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1668 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1669 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1670 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1671 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1672 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1673 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1674 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1675 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1676 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1677 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1678 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1679 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1680 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1681 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1682 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1683 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1684 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1685 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1686 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1687 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1688 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1689 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1690 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1691 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1692 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1693 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1694 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1695 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1696 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1697 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1698 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eFour, ESuit::eHearts } }, 1699 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1700 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1701 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1702 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1703 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1704 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1705 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1706 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1707 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1708 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1709 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1710 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1711 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1712 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1713 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1714 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1715 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1716 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1717 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1718 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1719 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1720 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1721 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1722 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1723 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1724 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1725 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1726 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1727 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1728 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1729 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1730 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1731 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1732 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1733 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1734 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1735 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1736 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1737 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1738 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1739 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1740 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1741 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1742 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1743 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1744 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1745 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1746 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1747 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1748 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1749 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1750 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1751 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1752 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1753 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1754 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1755 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1756 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1757 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1758 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1759 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1760 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1761 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1762 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1763 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1764 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eTrey, ESuit::eHearts } }, 1765 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1766 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1767 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1768 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1769 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1770 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1771 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1772 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1773 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1774 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1775 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eAce, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1776 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1777 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1778 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1779 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1780 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1781 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1782 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1783 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1784 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1785 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1786 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1787 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1788 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1789 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1790 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1791 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1792 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1793 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1794 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1795 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1796 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1797 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1798 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1799 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1800 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1801 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1802 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1803 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1804 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1805 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1806 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1807 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1808 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1809 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1810 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1811 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1812 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1813 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1814 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1815 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1816 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1817 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1818 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1819 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1820 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1821 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1822 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1823 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1824 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1825 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1826 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1827 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1828 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1829 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1830 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eDeuce, ESuit::eHearts } }, 1831 } // EHand::ePair
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1832 } // EHand::eHighCard
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1833 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1834 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1835 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1836 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1837 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1838 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1839 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1840 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1841 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1842 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1843 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1844 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1845 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1846 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1847 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1848 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1849 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1850 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1851 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1852 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1853 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1854 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1855 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1856 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1857 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1858 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1859 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1860 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1861 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1862 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1863 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1864 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1865 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1866 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1867 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1868 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1869 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1870 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1871 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1872 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1873 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1874 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1875 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1876 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1877 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1878 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1879 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1880 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1881 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1882 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1883 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1884 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eKing, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1885 }
        ,{ { { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1886 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1887 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1888 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1889 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1890 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1891 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1892 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1893 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1894 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1895 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1896 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1897 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1898 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1899 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1900 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1901 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1902 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1903 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1904 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1905 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1906 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1907 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1908 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1909 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1910 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1911 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1912 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1913 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1914 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1915 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1916 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1917 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1918 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1919 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1920 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1921 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1922 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1923 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1924 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1925 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1926 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1927 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1928 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1929 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1930 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1931 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1932 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1933 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1934 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1935 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1936 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1937 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1938 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1939 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1940 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1941 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1942 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1943 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1944 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1945 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1946 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1947 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1948 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1949 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1950 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1951 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1952 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1953 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1954 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1955 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1956 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1957 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1958 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1959 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1960 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1961 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1962 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1963 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1964 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1965 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1966 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1967 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1968 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1969 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1970 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1971 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1972 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1973 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1974 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1975 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1976 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1977 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1978 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1979 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1980 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1981 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1982 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1983 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1984 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1985 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1986 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1987 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1988 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1989 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1990 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1991 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1992 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1993 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1994 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1995 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1996 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1997 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1998 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 1999 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2000 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2001 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2002 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2003 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2004 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2005 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2006 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2007 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2008 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2009 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2010 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2011 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2012 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2013 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2014 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2015 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2016 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2017 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2018 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2019 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2020 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2021 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2022 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2023 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2024 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2025 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2026 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2027 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2028 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2029 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2030 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2031 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2032 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2033 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2034 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2035 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2036 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2037 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2038 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2039 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2040 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2041 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2042 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2043 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2044 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2045 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2046 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2047 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2048 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eAce, ESuit::eHearts } }, 2049 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2050 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2051 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2052 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2053 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2054 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2055 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2056 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2057 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2058 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2059 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2060 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2061 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2062 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2063 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2064 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2065 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2066 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2067 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2068 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2069 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2070 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2071 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2072 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2073 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2074 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2075 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2076 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2077 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2078 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2079 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2080 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2081 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2082 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2083 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2084 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2085 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2086 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2087 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2088 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2089 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2090 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2091 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2092 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eQueen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2093 }
        ,{ { { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2094 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2095 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2096 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2097 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2098 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2099 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2100 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2101 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2102 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2103 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2104 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2105 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2106 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2107 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2108 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2109 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2110 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2111 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2112 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2113 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2114 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2115 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2116 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2117 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2118 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2119 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2120 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2121 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2122 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2123 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2124 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2125 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2126 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2127 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2128 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2129 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2130 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2131 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2132 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2133 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2134 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2135 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2136 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2137 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2138 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2139 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2140 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2141 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2142 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2143 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2144 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2145 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2146 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2147 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2148 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2149 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2150 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2151 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2152 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2153 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2154 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2155 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2156 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2157 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2158 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2159 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2160 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2161 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2162 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2163 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2164 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2165 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2166 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2167 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2168 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2169 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2170 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2171 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2172 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2173 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2174 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2175 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2176 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2177 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2178 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2179 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2180 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2181 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2182 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2183 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2184 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2185 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2186 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2187 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2188 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2189 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2190 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2191 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2192 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2193 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2194 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2195 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2196 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2197 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2198 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2199 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2200 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2201 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2202 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2203 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2204 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2205 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2206 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2207 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2208 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2209 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2210 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2211 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2212 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eKing, ESuit::eHearts } }, 2213 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2214 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2215 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2216 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2217 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2218 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2219 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2220 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2221 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2222 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2223 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2224 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2225 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2226 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2227 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2228 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2229 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2230 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2231 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2232 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2233 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2234 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2235 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2236 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2237 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2238 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2239 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2240 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2241 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2242 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2243 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2244 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2245 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2246 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2247 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eJack, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2248 }
        ,{ { { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2249 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2250 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2251 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2252 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2253 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2254 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2255 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2256 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2257 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2258 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2259 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2260 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2261 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2262 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2263 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2264 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2265 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2266 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2267 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2268 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2269 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2270 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2271 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2272 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2273 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2274 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2275 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2276 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2277 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2278 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2279 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2280 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2281 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2282 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2283 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2284 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2285 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2286 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2287 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2288 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2289 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2290 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2291 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2292 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2293 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2294 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2295 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2296 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2297 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2298 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2299 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2300 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2301 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2302 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2303 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2304 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2305 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2306 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2307 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2308 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2309 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2310 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2311 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2312 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2313 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2314 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2315 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2316 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2317 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2318 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2319 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2320 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2321 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2322 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2323 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2324 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2325 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2326 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2327 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2328 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2329 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2330 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2331 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eQueen, ESuit::eHearts } }, 2332 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2333 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2334 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2335 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2336 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2337 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2338 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2339 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2340 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2341 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2342 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2343 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2344 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2345 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2346 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2347 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2348 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2349 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2350 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2351 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2352 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2353 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2354 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2355 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2356 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2357 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2358 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eTen, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2359 }
        ,{ { { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2360 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2361 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2362 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2363 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2364 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2365 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2366 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2367 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2368 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2369 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2370 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2371 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2372 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2373 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2374 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2375 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2376 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2377 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2378 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2379 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2380 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2381 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2382 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2383 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2384 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2385 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2386 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2387 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2388 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2389 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2390 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2391 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2392 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2393 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2394 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2395 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2396 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2397 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2398 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2399 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2400 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2401 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2402 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2403 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2404 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2405 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2406 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2407 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2408 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2409 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2410 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2411 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2412 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2413 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2414 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eJack, ESuit::eHearts } }, 2415 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2416 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2417 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2418 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2419 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2420 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2421 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2422 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2423 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2424 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2425 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2426 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2427 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2428 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2429 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2430 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2431 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2432 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2433 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2434 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eNine, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2435 }
        ,{ { { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2436 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2437 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2438 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2439 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2440 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2441 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2442 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2443 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2444 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2445 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2446 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2447 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2448 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2449 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2450 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2451 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2452 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2453 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2454 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2455 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2456 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2457 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2458 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2459 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2460 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2461 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2462 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2463 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2464 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2465 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2466 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2467 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2468 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2469 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eTen, ESuit::eHearts } }, 2470 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2471 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2472 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2473 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2474 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2475 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2476 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2477 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2478 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2479 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2480 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2481 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2482 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2483 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eEight, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2484 }
        ,{ { { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2485 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2486 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2487 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2488 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2489 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2490 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2491 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2492 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2493 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2494 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2495 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2496 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2497 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2498 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2499 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2500 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2501 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2502 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2503 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eNine, ESuit::eHearts } }, 2504 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2505 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2506 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2507 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2508 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2509 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2510 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2511 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2512 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSeven, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2513 }
        ,{ { { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2514 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2515 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2516 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2517 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2518 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2519 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2520 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2521 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2522 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eEight, ESuit::eHearts } }, 2523 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2524 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2525 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2526 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2527 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eSix, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2528 }
        ,{ { { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2529 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2530 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2531 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSeven, ESuit::eHearts } }, 2532 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 2533 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFive, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 2534 }
        ,{ { { ECard::eDeuce, ESuit::eSpades }, { ECard::eTrey, ESuit::eSpades }, { ECard::eFour, ESuit::eSpades } , { ECard::eSix, ESuit::eHearts } }, 2535 } // EHand::eHighCard
    };

    static std::vector< uint32_t > sFlushes =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0,
        0, 973, 0, 0, 0, 972, 0, 971, 22, 0, 0, 0, 0, 0, 0, 0, 0, 970, 0,
        0, 0, 969, 0, 968, 967, 0, 0, 0, 0, 966, 0, 965, 964, 0, 0, 963, 962,
        0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 961, 0, 0, 0, 960, 0, 959, 958,
        0, 0, 0, 0, 957, 0, 956, 955, 0, 0, 954, 953, 0, 952, 0, 0, 0, 0,
        0, 0, 951, 0, 950, 949, 0, 0, 948, 947, 0, 946, 0, 0, 0, 0, 945, 944,
        0, 943, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 942,
        0, 0, 0, 941, 0, 940, 939, 0, 0, 0, 0, 938, 0, 937, 936, 0, 0, 935,
        934, 0, 933, 0, 0, 0, 0, 0, 0, 932, 0, 931, 930, 0, 0, 929, 928, 0,
        927, 0, 0, 0, 0, 926, 925, 0, 924, 0, 0, 0, 923, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 922, 0, 921, 920, 0, 0, 919, 918, 0, 917, 0, 0, 0, 0,
        916, 915, 0, 914, 0, 0, 0, 913, 0, 0, 0, 0, 0, 0, 0, 0, 912, 911,
        0, 910, 0, 0, 0, 909, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 908, 0, 0, 0, 907, 0,
        906, 905, 0, 0, 0, 0, 904, 0, 903, 902, 0, 0, 901, 900, 0, 899, 0,
        0, 0, 0, 0, 0, 898, 0, 897, 896, 0, 0, 895, 894, 0, 893, 0, 0, 0,
        0, 892, 891, 0, 890, 0, 0, 0, 889, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 888,
        0, 887, 886, 0, 0, 885, 884, 0, 883, 0, 0, 0, 0, 882, 881, 0, 880,
        0, 0, 0, 879, 0, 0, 0, 0, 0, 0, 0, 0, 878, 877, 0, 876, 0, 0, 0, 875,
        0, 0, 0, 0, 0, 0, 0, 874, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 873, 0, 872, 871, 0, 0, 870, 869, 0, 868, 0, 0, 0, 0,
        867, 866, 0, 865, 0, 0, 0, 864, 0, 0, 0, 0, 0, 0, 0, 0, 863, 862,
        0, 861, 0, 0, 0, 860, 0, 0, 0, 0, 0, 0, 0, 859, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 858, 857, 0, 856, 0, 0, 0, 855, 0, 0,
        0, 0, 0, 0, 0, 854, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 853, 0, 0, 0, 852,
        0, 851, 850, 0, 0, 0, 0, 849, 0, 848, 847, 0, 0, 846, 845, 0, 844,
        0, 0, 0, 0, 0, 0, 843, 0, 842, 841, 0, 0, 840, 839, 0, 838, 0, 0,
        0, 0, 837, 836, 0, 835, 0, 0, 0, 834, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        833, 0, 832, 831, 0, 0, 830, 829, 0, 828, 0, 0, 0, 0, 827, 826, 0,
        825, 0, 0, 0, 824, 0, 0, 0, 0, 0, 0, 0, 0, 823, 822, 0, 821, 0, 0,
        0, 820, 0, 0, 0, 0, 0, 0, 0, 819, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 818, 0, 817, 816, 0, 0, 815, 814, 0, 813, 0,
        0, 0, 0, 812, 811, 0, 810, 0, 0, 0, 809, 0, 0, 0, 0, 0, 0, 0, 0, 808,
        807, 0, 806, 0, 0, 0, 805, 0, 0, 0, 0, 0, 0, 0, 804, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 803, 802, 0, 801, 0, 0, 0, 800, 0,
        0, 0, 0, 0, 0, 0, 799, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        798, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 797, 0, 796, 795, 0, 0, 794,
        793, 0, 792, 0, 0, 0, 0, 791, 790, 0, 789, 0, 0, 0, 788, 0, 0, 0,
        0, 0, 0, 0, 0, 787, 786, 0, 785, 0, 0, 0, 784, 0, 0, 0, 0, 0, 0, 0,
        783, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 782, 781, 0,
        780, 0, 0, 0, 779, 0, 0, 0, 0, 0, 0, 0, 778, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 777, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 776, 775,
        0, 774, 0, 0, 0, 773, 0, 0, 0, 0, 0, 0, 0, 772, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 771, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 770, 0, 0, 0, 769, 0, 768, 767, 0, 0, 0, 0, 766, 0, 765, 764,
        0, 0, 763, 762, 0, 761, 0, 0, 0, 0, 0, 0, 760, 0, 759, 758, 0, 0,
        757, 756, 0, 755, 0, 0, 0, 0, 754, 753, 0, 752, 0, 0, 0, 751, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 750, 0, 749, 748, 0, 0, 747, 746, 0, 745,
        0, 0, 0, 0, 744, 743, 0, 742, 0, 0, 0, 741, 0, 0, 0, 0, 0, 0, 0, 0,
        740, 739, 0, 738, 0, 0, 0, 737, 0, 0, 0, 0, 0, 0, 0, 736, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 735, 0, 734, 733, 0,
        0, 732, 731, 0, 730, 0, 0, 0, 0, 729, 728, 0, 727, 0, 0, 0, 726, 0,
        0, 0, 0, 0, 0, 0, 0, 725, 724, 0, 723, 0, 0, 0, 722, 0, 0, 0, 0, 0,
        0, 0, 721, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 720, 719,
        0, 718, 0, 0, 0, 717, 0, 0, 0, 0, 0, 0, 0, 716, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        714, 0, 713, 712, 0, 0, 711, 710, 0, 709, 0, 0, 0, 0, 708, 707, 0,
        706, 0, 0, 0, 705, 0, 0, 0, 0, 0, 0, 0, 0, 704, 703, 0, 702, 0, 0,
        0, 701, 0, 0, 0, 0, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 699, 698, 0, 697, 0, 0, 0, 696, 0, 0, 0, 0, 0, 0, 0,
        695, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 694, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 693, 692, 0, 691, 0, 0, 0, 690, 0, 0, 0, 0, 0, 0,
        0, 689, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 688, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 687, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 686, 0, 685, 684, 0, 0, 683, 682, 0, 681, 0,
        0, 0, 0, 680, 679, 0, 678, 0, 0, 0, 677, 0, 0, 0, 0, 0, 0, 0, 0, 676,
        675, 0, 674, 0, 0, 0, 673, 0, 0, 0, 0, 0, 0, 0, 672, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 671, 670, 0, 669, 0, 0, 0, 668, 0,
        0, 0, 0, 0, 0, 0, 667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        666, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 665, 664, 0, 663, 0, 0, 0, 662, 0,
        0, 0, 0, 0, 0, 0, 661, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        660, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 659, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 658, 657, 0, 656, 0, 0, 0, 655, 0, 0, 0,
        0, 0, 0, 0, 654, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 653,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 652, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 651, 0, 0, 0, 650, 0, 649, 648, 0, 0,
        0, 0, 647, 0, 646, 645, 0, 0, 644, 643, 0, 642, 0, 0, 0, 0, 0, 0,
        641, 0, 640, 639, 0, 0, 638, 637, 0, 636, 0, 0, 0, 0, 635, 634, 0,
        633, 0, 0, 0, 632, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 631, 0, 630, 629,
        0, 0, 628, 627, 0, 626, 0, 0, 0, 0, 625, 624, 0, 623, 0, 0, 0, 622,
        0, 0, 0, 0, 0, 0, 0, 0, 621, 620, 0, 619, 0, 0, 0, 618, 0, 0, 0, 0,
        0, 0, 0, 617, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        616, 0, 615, 614, 0, 0, 613, 612, 0, 611, 0, 0, 0, 0, 610, 609, 0,
        608, 0, 0, 0, 607, 0, 0, 0, 0, 0, 0, 0, 0, 606, 605, 0, 604, 0, 0,
        0, 603, 0, 0, 0, 0, 0, 0, 0, 602, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 601, 600, 0, 599, 0, 0, 0, 598, 0, 0, 0, 0, 0, 0, 0,
        597, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 596, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 595, 0, 594, 593, 0, 0, 592, 591, 0, 590,
        0, 0, 0, 0, 589, 588, 0, 587, 0, 0, 0, 586, 0, 0, 0, 0, 0, 0, 0, 0,
        585, 584, 0, 583, 0, 0, 0, 582, 0, 0, 0, 0, 0, 0, 0, 581, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 580, 579, 0, 578, 0, 0, 0,
        577, 0, 0, 0, 0, 0, 0, 0, 576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 575, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 574, 573, 0, 572, 0, 0,
        0, 571, 0, 0, 0, 0, 0, 0, 0, 570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 569, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 568, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 567, 0, 566, 565, 0,
        0, 564, 563, 0, 562, 0, 0, 0, 0, 561, 560, 0, 559, 0, 0, 0, 558, 0,
        0, 0, 0, 0, 0, 0, 0, 557, 556, 0, 555, 0, 0, 0, 554, 0, 0, 0, 0, 0,
        0, 0, 553, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 552, 551,
        0, 550, 0, 0, 0, 549, 0, 0, 0, 0, 0, 0, 0, 548, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 547, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 546, 545,
        0, 544, 0, 0, 0, 543, 0, 0, 0, 0, 0, 0, 0, 542, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 541, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 540, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 539, 538, 0,
        537, 0, 0, 0, 536, 0, 0, 0, 0, 0, 0, 0, 535, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 534, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 533, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 532, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 531, 0, 530, 529, 0,
        0, 528, 527, 0, 526, 0, 0, 0, 0, 525, 524, 0, 523, 0, 0, 0, 522, 0,
        0, 0, 0, 0, 0, 0, 0, 521, 520, 0, 519, 0, 0, 0, 518, 0, 0, 0, 0, 0,
        0, 0, 517, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 516, 515,
        0, 514, 0, 0, 0, 513, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 511, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 510, 509,
        0, 508, 0, 0, 0, 507, 0, 0, 0, 0, 0, 0, 0, 506, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 505, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 504, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 503, 502, 0,
        501, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 499, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 498, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 497, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 496, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 495, 494, 0, 493, 0, 0, 0,
        492, 0, 0, 0, 0, 0, 0, 0, 491, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 490, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 489, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 488, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 487, 0, 486, 485, 0, 0, 0, 0, 484,
        0, 483, 482, 0, 0, 481, 480, 0, 479, 0, 0, 0, 0, 0, 0, 478, 0, 477,
        476, 0, 0, 475, 474, 0, 473, 0, 0, 0, 0, 472, 471, 0, 470, 0, 0, 0,
        469, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 468, 0, 467, 466, 0, 0, 465, 464,
        0, 463, 0, 0, 0, 0, 462, 461, 0, 460, 0, 0, 0, 459, 0, 0, 0, 0, 0,
        0, 0, 0, 458, 457, 0, 456, 0, 0, 0, 455, 0, 0, 0, 0, 0, 0, 0, 454,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 453, 0, 452,
        451, 0, 0, 450, 449, 0, 448, 0, 0, 0, 0, 447, 446, 0, 445, 0, 0, 0,
        444, 0, 0, 0, 0, 0, 0, 0, 0, 443, 442, 0, 441, 0, 0, 0, 440, 0, 0,
        0, 0, 0, 0, 0, 439, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        438, 437, 0, 436, 0, 0, 0, 435, 0, 0, 0, 0, 0, 0, 0, 434, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 433, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 432, 0, 431, 430, 0, 0, 429, 428, 0, 427, 0, 0, 0, 0, 426,
        425, 0, 424, 0, 0, 0, 423, 0, 0, 0, 0, 0, 0, 0, 0, 422, 421, 0, 420,
        0, 0, 0, 419, 0, 0, 0, 0, 0, 0, 0, 418, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 417, 416, 0, 415, 0, 0, 0, 414, 0, 0, 0, 0, 0,
        0, 0, 413, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 412, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 411, 410, 0, 409, 0, 0, 0, 408, 0, 0, 0, 0,
        0, 0, 0, 407, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 406, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 405, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 404, 0, 403, 402, 0, 0, 401, 400, 0, 399,
        0, 0, 0, 0, 398, 397, 0, 396, 0, 0, 0, 395, 0, 0, 0, 0, 0, 0, 0, 0,
        394, 393, 0, 392, 0, 0, 0, 391, 0, 0, 0, 0, 0, 0, 0, 390, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 389, 388, 0, 387, 0, 0, 0,
        386, 0, 0, 0, 0, 0, 0, 0, 385, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 383, 382, 0, 381, 0, 0,
        0, 380, 0, 0, 0, 0, 0, 0, 0, 379, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 378, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 377, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 376, 375, 0, 374, 0, 0, 0,
        373, 0, 0, 0, 0, 0, 0, 0, 372, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 371, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 370, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 369, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 368, 0, 367, 366, 0, 0, 365, 364, 0,
        363, 0, 0, 0, 0, 362, 361, 0, 360, 0, 0, 0, 359, 0, 0, 0, 0, 0, 0,
        0, 0, 358, 357, 0, 356, 0, 0, 0, 355, 0, 0, 0, 0, 0, 0, 0, 354, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 353, 352, 0, 351, 0,
        0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 349, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 348, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 347, 346, 0, 345,
        0, 0, 0, 344, 0, 0, 0, 0, 0, 0, 0, 343, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 342, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 341, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 340, 339, 0, 338, 0,
        0, 0, 337, 0, 0, 0, 0, 0, 0, 0, 336, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 335, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 334, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 333, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 332, 331, 0, 330, 0, 0, 0, 329, 0, 0,
        0, 0, 0, 0, 0, 328, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 327,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 326, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 325, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 324, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 323, 0,
        322, 321, 0, 0, 320, 319, 0, 318, 0, 0, 0, 0, 317, 316, 0, 315, 0,
        0, 0, 314, 0, 0, 0, 0, 0, 0, 0, 0, 313, 312, 0, 311, 0, 0, 0, 310,
        0, 0, 0, 0, 0, 0, 0, 309, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 308, 307, 0, 306, 0, 0, 0, 305, 0, 0, 0, 0, 0, 0, 0, 304, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 303, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 302, 301, 0, 300, 0, 0, 0, 299, 0, 0, 0, 0, 0, 0, 0, 298,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 297, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 296, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 295, 294, 0, 293, 0, 0, 0, 292, 0, 0, 0, 0, 0, 0, 0, 291, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 290, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 289, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        288, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 287, 286,
        0, 285, 0, 0, 0, 284, 0, 0, 0, 0, 0, 0, 0, 283, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 282, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 281, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 280, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 279, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 0, 278, 277, 0, 276, 0, 0, 0, 275, 0, 0, 0, 0, 0, 0,
        0, 274, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 273, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 271, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        270, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14
    };

    static std::vector< uint32_t > sHighCardUnique =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1820, 0, 0, 0, 0, 0,
        0, 0, 1819, 0, 0, 0, 1818, 0, 1817, 1816, 0, 0, 0, 0, 0, 0, 0, 0,
        1815, 0, 0, 0, 1814, 0, 1813, 1812, 0, 0, 0, 0, 1811, 0, 1810, 1809,
        0, 0, 1808, 1807, 0, 1806, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1805, 0,
        0, 0, 1804, 0, 1803, 1802, 0, 0, 0, 0, 1801, 0, 1800, 1799, 0, 0,
        1798, 1797, 0, 1796, 0, 0, 0, 0, 0, 0, 1795, 0, 1794, 1793, 0, 0,
        1792, 1791, 0, 1790, 0, 0, 0, 0, 1789, 1788, 0, 1787, 0, 0, 0, 1786,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1785, 0, 0, 0, 1784, 0,
        1783, 1782, 0, 0, 0, 0, 1781, 0, 1780, 1779, 0, 0, 1778, 1777, 0,
        1776, 0, 0, 0, 0, 0, 0, 1775, 0, 1774, 1773, 0, 0, 1772, 1771, 0,
        1770, 0, 0, 0, 0, 1769, 1768, 0, 1767, 0, 0, 0, 1766, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1765, 0, 1764, 1763, 0, 0, 1762, 1761, 0, 1760,
        0, 0, 0, 0, 1759, 1758, 0, 1757, 0, 0, 0, 1756, 0, 0, 0, 0, 0, 0,
        0, 0, 1755, 1754, 0, 1753, 0, 0, 0, 1752, 0, 0, 0, 0, 0, 0, 0, 1751,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1750, 0, 0, 0, 1749, 0, 1748, 1747, 0, 0, 0, 0, 1746, 0, 1745, 1744,
        0, 0, 1743, 1742, 0, 1741, 0, 0, 0, 0, 0, 0, 1740, 0, 1739, 1738,
        0, 0, 1737, 1736, 0, 1735, 0, 0, 0, 0, 1734, 1733, 0, 1732, 0, 0,
        0, 1731, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1730, 0, 1729, 1728, 0, 0,
        1727, 1726, 0, 1725, 0, 0, 0, 0, 1724, 1723, 0, 1722, 0, 0, 0, 1721,
        0, 0, 0, 0, 0, 0, 0, 0, 1720, 1719, 0, 1718, 0, 0, 0, 1717, 0, 0,
        0, 0, 0, 0, 0, 1716, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1715, 0, 1714, 1713, 0, 0, 1712, 1711, 0, 1710, 0, 0, 0,
        0, 1709, 1708, 0, 1707, 0, 0, 0, 1706, 0, 0, 0, 0, 0, 0, 0, 0, 1705,
        1704, 0, 1703, 0, 0, 0, 1702, 0, 0, 0, 0, 0, 0, 0, 1701, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1700, 1699, 0, 1698, 0, 0,
        0, 1697, 0, 0, 0, 0, 0, 0, 0, 1696, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1695, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1694, 0, 0, 0, 1693, 0, 1692, 1691, 0, 0, 0, 0, 1690, 0, 1689,
        1688, 0, 0, 1687, 1686, 0, 1685, 0, 0, 0, 0, 0, 0, 1684, 0, 1683,
        1682, 0, 0, 1681, 1680, 0, 1679, 0, 0, 0, 0, 1678, 1677, 0, 1676,
        0, 0, 0, 1675, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1674, 0, 1673, 1672,
        0, 0, 1671, 1670, 0, 1669, 0, 0, 0, 0, 1668, 1667, 0, 1666, 0, 0,
        0, 1665, 0, 0, 0, 0, 0, 0, 0, 0, 1664, 1663, 0, 1662, 0, 0, 0, 1661,
        0, 0, 0, 0, 0, 0, 0, 1660, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1659, 0, 1658, 1657, 0, 0, 1656, 1655, 0, 1654, 0,
        0, 0, 0, 1653, 1652, 0, 1651, 0, 0, 0, 1650, 0, 0, 0, 0, 0, 0, 0,
        0, 1649, 1648, 0, 1647, 0, 0, 0, 1646, 0, 0, 0, 0, 0, 0, 0, 1645,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1644, 1643, 0, 1642,
        0, 0, 0, 1641, 0, 0, 0, 0, 0, 0, 0, 1640, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1639, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1638,
        0, 1637, 1636, 0, 0, 1635, 1634, 0, 1633, 0, 0, 0, 0, 1632, 1631,
        0, 1630, 0, 0, 0, 1629, 0, 0, 0, 0, 0, 0, 0, 0, 1628, 1627, 0, 1626,
        0, 0, 0, 1625, 0, 0, 0, 0, 0, 0, 0, 1624, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1623, 1622, 0, 1621, 0, 0, 0, 1620, 0, 0,
        0, 0, 0, 0, 0, 1619, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1618, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1617, 1616, 0, 1615, 0, 0, 0,
        1614, 0, 0, 0, 0, 0, 0, 0, 1613, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1612, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1611, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1610,
        0, 0, 0, 1609, 0, 1608, 1607, 0, 0, 0, 0, 1606, 0, 1605, 1604, 0,
        0, 1603, 1602, 0, 1601, 0, 0, 0, 0, 0, 0, 1600, 0, 1599, 1598, 0,
        0, 1597, 1596, 0, 1595, 0, 0, 0, 0, 1594, 1593, 0, 1592, 0, 0, 0,
        1591, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1590, 0, 1589, 1588, 0, 0, 1587,
        1586, 0, 1585, 0, 0, 0, 0, 1584, 1583, 0, 1582, 0, 0, 0, 1581, 0,
        0, 0, 0, 0, 0, 0, 0, 1580, 1579, 0, 1578, 0, 0, 0, 1577, 0, 0, 0,
        0, 0, 0, 0, 1576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1575, 0, 1574, 1573, 0, 0, 1572, 1571, 0, 1570, 0, 0, 0, 0,
        1569, 1568, 0, 1567, 0, 0, 0, 1566, 0, 0, 0, 0, 0, 0, 0, 0, 1565,
        1564, 0, 1563, 0, 0, 0, 1562, 0, 0, 0, 0, 0, 0, 0, 1561, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1560, 1559, 0, 1558, 0, 0,
        0, 1557, 0, 0, 0, 0, 0, 0, 0, 1556, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1555, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1554, 0,
        1553, 1552, 0, 0, 1551, 1550, 0, 1549, 0, 0, 0, 0, 1548, 1547, 0,
        1546, 0, 0, 0, 1545, 0, 0, 0, 0, 0, 0, 0, 0, 1544, 1543, 0, 1542,
        0, 0, 0, 1541, 0, 0, 0, 0, 0, 0, 0, 1540, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1539, 1538, 0, 1537, 0, 0, 0, 1536, 0, 0,
        0, 0, 0, 0, 0, 1535, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1534, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1533, 1532, 0, 1531, 0, 0, 0,
        1530, 0, 0, 0, 0, 0, 0, 0, 1529, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1527, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1526, 0, 1525,
        1524, 0, 0, 1523, 1522, 0, 1521, 0, 0, 0, 0, 1520, 1519, 0, 1518,
        0, 0, 0, 1517, 0, 0, 0, 0, 0, 0, 0, 0, 1516, 1515, 0, 1514, 0, 0,
        0, 1513, 0, 0, 0, 0, 0, 0, 0, 1512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1511, 1510, 0, 1509, 0, 0, 0, 1508, 0, 0, 0, 0,
        0, 0, 0, 1507, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1506,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1505, 1504, 0, 1503, 0, 0, 0, 1502,
        0, 0, 0, 0, 0, 0, 0, 1501, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1499, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1498, 1497, 0, 1496, 0, 0,
        0, 1495, 0, 0, 0, 0, 0, 0, 0, 1494, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1493, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1492, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1491, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1490, 0,
        0, 0, 1489, 0, 1488, 1487, 0, 0, 0, 0, 1486, 0, 1485, 1484, 0, 0,
        1483, 1482, 0, 1481, 0, 0, 0, 0, 0, 0, 1480, 0, 1479, 1478, 0, 0,
        1477, 1476, 0, 1475, 0, 0, 0, 0, 1474, 1473, 0, 1472, 0, 0, 0, 1471,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1470, 0, 1469, 1468, 0, 0, 1467, 1466,
        0, 1465, 0, 0, 0, 0, 1464, 1463, 0, 1462, 0, 0, 0, 1461, 0, 0, 0,
        0, 0, 0, 0, 0, 1460, 1459, 0, 1458, 0, 0, 0, 1457, 0, 0, 0, 0, 0,
        0, 0, 1456, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1455, 0, 1454, 1453, 0, 0, 1452, 1451, 0, 1450, 0, 0, 0, 0, 1449,
        1448, 0, 1447, 0, 0, 0, 1446, 0, 0, 0, 0, 0, 0, 0, 0, 1445, 1444,
        0, 1443, 0, 0, 0, 1442, 0, 0, 0, 0, 0, 0, 0, 1441, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1440, 1439, 0, 1438, 0, 0, 0, 1437,
        0, 0, 0, 0, 0, 0, 0, 1436, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1435, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1434, 0, 1433, 1432,
        0, 0, 1431, 1430, 0, 1429, 0, 0, 0, 0, 1428, 1427, 0, 1426, 0, 0,
        0, 1425, 0, 0, 0, 0, 0, 0, 0, 0, 1424, 1423, 0, 1422, 0, 0, 0, 1421,
        0, 0, 0, 0, 0, 0, 0, 1420, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1419, 1418, 0, 1417, 0, 0, 0, 1416, 0, 0, 0, 0, 0, 0, 0,
        1415, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1414, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1413, 1412, 0, 1411, 0, 0, 0, 1410, 0, 0, 0,
        0, 0, 0, 0, 1409, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1408,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1407, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1406, 0, 1405, 1404, 0, 0, 1403,
        1402, 0, 1401, 0, 0, 0, 0, 1400, 1399, 0, 1398, 0, 0, 0, 1397, 0,
        0, 0, 0, 0, 0, 0, 0, 1396, 1395, 0, 1394, 0, 0, 0, 1393, 0, 0, 0,
        0, 0, 0, 0, 1392, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1391, 1390, 0, 1389, 0, 0, 0, 1388, 0, 0, 0, 0, 0, 0, 0, 1387, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1386, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1385, 1384, 0, 1383, 0, 0, 0, 1382, 0, 0, 0, 0, 0, 0,
        0, 1381, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1380, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1379, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1378, 1377, 0, 1376, 0, 0, 0, 1375, 0, 0, 0,
        0, 0, 0, 0, 1374, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1373,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1372, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1371, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1370, 0, 1369, 1368, 0, 0, 1367, 1366, 0,
        1365, 0, 0, 0, 0, 1364, 1363, 0, 1362, 0, 0, 0, 1361, 0, 0, 0, 0,
        0, 0, 0, 0, 1360, 1359, 0, 1358, 0, 0, 0, 1357, 0, 0, 0, 0, 0, 0,
        0, 1356, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1355, 1354,
        0, 1353, 0, 0, 0, 1352, 0, 0, 0, 0, 0, 0, 0, 1351, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1349, 1348, 0, 1347, 0, 0, 0, 1346, 0, 0, 0, 0, 0, 0, 0, 1345, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1344, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1343, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1342, 1341, 0, 1340, 0, 0, 0, 1339, 0, 0, 0, 0, 0, 0, 0,
        1338, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1337, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1336, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1335, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1334, 1333, 0, 1332, 0, 0, 0, 1331, 0, 0, 0, 0, 0, 0, 0,
        1330, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1329, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1328, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1327, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1326, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 1325, 0, 0, 0, 1324, 0, 1323, 1322, 0, 0, 0, 0, 1321, 0, 1320,
        1319, 0, 0, 1318, 1317, 0, 1316, 0, 0, 0, 0, 0, 0, 1315, 0, 1314,
        1313, 0, 0, 1312, 1311, 0, 1310, 0, 0, 0, 0, 1309, 1308, 0, 1307,
        0, 0, 0, 1306, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1305, 0, 1304, 1303,
        0, 0, 1302, 1301, 0, 1300, 0, 0, 0, 0, 1299, 1298, 0, 1297, 0, 0,
        0, 1296, 0, 0, 0, 0, 0, 0, 0, 0, 1295, 1294, 0, 1293, 0, 0, 0, 1292,
        0, 0, 0, 0, 0, 0, 0, 1291, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1290, 0, 1289, 1288, 0, 0, 1287, 1286, 0, 1285, 0,
        0, 0, 0, 1284, 1283, 0, 1282, 0, 0, 0, 1281, 0, 0, 0, 0, 0, 0, 0,
        0, 1280, 1279, 0, 1278, 0, 0, 0, 1277, 0, 0, 0, 0, 0, 0, 0, 1276,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1275, 1274, 0, 1273,
        0, 0, 0, 1272, 0, 0, 0, 0, 0, 0, 0, 1271, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1270, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1269,
        0, 1268, 1267, 0, 0, 1266, 1265, 0, 1264, 0, 0, 0, 0, 1263, 1262,
        0, 1261, 0, 0, 0, 1260, 0, 0, 0, 0, 0, 0, 0, 0, 1259, 1258, 0, 1257,
        0, 0, 0, 1256, 0, 0, 0, 0, 0, 0, 0, 1255, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1254, 1253, 0, 1252, 0, 0, 0, 1251, 0, 0,
        0, 0, 0, 0, 0, 1250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1249, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1248, 1247, 0, 1246, 0, 0, 0,
        1245, 0, 0, 0, 0, 0, 0, 0, 1244, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1243, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1242, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1241, 0, 1240,
        1239, 0, 0, 1238, 1237, 0, 1236, 0, 0, 0, 0, 1235, 1234, 0, 1233,
        0, 0, 0, 1232, 0, 0, 0, 0, 0, 0, 0, 0, 1231, 1230, 0, 1229, 0, 0,
        0, 1228, 0, 0, 0, 0, 0, 0, 0, 1227, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1226, 1225, 0, 1224, 0, 0, 0, 1223, 0, 0, 0, 0,
        0, 0, 0, 1222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1221,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1220, 1219, 0, 1218, 0, 0, 0, 1217,
        0, 0, 0, 0, 0, 0, 0, 1216, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1214, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1213, 1212, 0, 1211, 0, 0,
        0, 1210, 0, 0, 0, 0, 0, 0, 0, 1209, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1207, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1206, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1205, 0, 1204, 1203,
        0, 0, 1202, 1201, 0, 1200, 0, 0, 0, 0, 1199, 1198, 0, 1197, 0, 0,
        0, 1196, 0, 0, 0, 0, 0, 0, 0, 0, 1195, 1194, 0, 1193, 0, 0, 0, 1192,
        0, 0, 0, 0, 0, 0, 0, 1191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1190, 1189, 0, 1188, 0, 0, 0, 1187, 0, 0, 0, 0, 0, 0, 0,
        1186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1185, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1184, 1183, 0, 1182, 0, 0, 0, 1181, 0, 0, 0,
        0, 0, 0, 0, 1180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1179,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1178, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1177, 1176, 0, 1175, 0, 0, 0, 1174, 0,
        0, 0, 0, 0, 0, 0, 1173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1171, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1170, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1169, 1168, 0, 1167, 0, 0, 0, 1166, 0,
        0, 0, 0, 0, 0, 0, 1165, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1163, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1162, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1161, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 1160, 0, 1159, 1158, 0, 0, 1157, 1156, 0, 1155, 0, 0,
        0, 0, 1154, 1153, 0, 1152, 0, 0, 0, 1151, 0, 0, 0, 0, 0, 0, 0, 0,
        1150, 1149, 0, 1148, 0, 0, 0, 1147, 0, 0, 0, 0, 0, 0, 0, 1146, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1145, 1144, 0, 1143,
        0, 0, 0, 1142, 0, 0, 0, 0, 0, 0, 0, 1141, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1139, 1138,
        0, 1137, 0, 0, 0, 1136, 0, 0, 0, 0, 0, 0, 0, 1135, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1134, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1133,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1132,
        1131, 0, 1130, 0, 0, 0, 1129, 0, 0, 0, 0, 0, 0, 0, 1128, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1127, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1125, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1124,
        1123, 0, 1122, 0, 0, 0, 1121, 0, 0, 0, 0, 0, 0, 0, 1120, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1119, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1117, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1116, 0,
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
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1115, 1114, 0, 1113, 0, 0,
        0, 1112, 0, 0, 0, 0, 0, 0, 0, 1111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1109, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1108, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1107, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 1106
    };

    static std::vector< uint32_t > sStraightsUnique =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 190, 0, 0, 0, 0, 0, 0,
        0, 2535, 0, 0, 0, 2534, 0, 2533, 189, 0, 0, 0, 0, 0, 0, 0, 0, 2532,
        0, 0, 0, 2531, 0, 2530, 2529, 0, 0, 0, 0, 2528, 0, 2527, 2526, 0,
        0, 2525, 2524, 0, 188, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2523, 0, 0, 0,
        2522, 0, 2521, 2520, 0, 0, 0, 0, 2519, 0, 2518, 2517, 0, 0, 2516,
        2515, 0, 2514, 0, 0, 0, 0, 0, 0, 2513, 0, 2512, 2511, 0, 0, 2510,
        2509, 0, 2508, 0, 0, 0, 0, 2507, 2506, 0, 2505, 0, 0, 0, 187, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2504, 0, 0, 0, 2503, 0, 2502,
        2501, 0, 0, 0, 0, 2500, 0, 2499, 2498, 0, 0, 2497, 2496, 0, 2495,
        0, 0, 0, 0, 0, 0, 2494, 0, 2493, 2492, 0, 0, 2491, 2490, 0, 2489,
        0, 0, 0, 0, 2488, 2487, 0, 2486, 0, 0, 0, 2485, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2484, 0, 2483, 2482, 0, 0, 2481, 2480, 0, 2479, 0, 0,
        0, 0, 2478, 2477, 0, 2476, 0, 0, 0, 2475, 0, 0, 0, 0, 0, 0, 0, 0,
        2474, 2473, 0, 2472, 0, 0, 0, 2471, 0, 0, 0, 0, 0, 0, 0, 186, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2470,
        0, 0, 0, 2469, 0, 2468, 2467, 0, 0, 0, 0, 2466, 0, 2465, 2464, 0,
        0, 2463, 2462, 0, 2461, 0, 0, 0, 0, 0, 0, 2460, 0, 2459, 2458, 0,
        0, 2457, 2456, 0, 2455, 0, 0, 0, 0, 2454, 2453, 0, 2452, 0, 0, 0,
        2451, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2450, 0, 2449, 2448, 0, 0, 2447,
        2446, 0, 2445, 0, 0, 0, 0, 2444, 2443, 0, 2442, 0, 0, 0, 2441, 0,
        0, 0, 0, 0, 0, 0, 0, 2440, 2439, 0, 2438, 0, 0, 0, 2437, 0, 0, 0,
        0, 0, 0, 0, 2436, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2435, 0, 2434, 2433, 0, 0, 2432, 2431, 0, 2430, 0, 0, 0, 0,
        2429, 2428, 0, 2427, 0, 0, 0, 2426, 0, 0, 0, 0, 0, 0, 0, 0, 2425,
        2424, 0, 2423, 0, 0, 0, 2422, 0, 0, 0, 0, 0, 0, 0, 2421, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2420, 2419, 0, 2418, 0, 0,
        0, 2417, 0, 0, 0, 0, 0, 0, 0, 2416, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 185, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2415, 0, 0, 0, 2414, 0, 2413, 2412, 0, 0, 0, 0, 2411, 0, 2410, 2409,
        0, 0, 2408, 2407, 0, 2406, 0, 0, 0, 0, 0, 0, 2405, 0, 2404, 2403,
        0, 0, 2402, 2401, 0, 2400, 0, 0, 0, 0, 2399, 2398, 0, 2397, 0, 0,
        0, 2396, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2395, 0, 2394, 2393, 0, 0,
        2392, 2391, 0, 2390, 0, 0, 0, 0, 2389, 2388, 0, 2387, 0, 0, 0, 2386,
        0, 0, 0, 0, 0, 0, 0, 0, 2385, 2384, 0, 2383, 0, 0, 0, 2382, 0, 0,
        0, 0, 0, 0, 0, 2381, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2380, 0, 2379, 2378, 0, 0, 2377, 2376, 0, 2375, 0, 0, 0,
        0, 2374, 2373, 0, 2372, 0, 0, 0, 2371, 0, 0, 0, 0, 0, 0, 0, 0, 2370,
        2369, 0, 2368, 0, 0, 0, 2367, 0, 0, 0, 0, 0, 0, 0, 2366, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2365, 2364, 0, 2363, 0, 0,
        0, 2362, 0, 0, 0, 0, 0, 0, 0, 2361, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2360, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2359, 0,
        2358, 2357, 0, 0, 2356, 2355, 0, 2354, 0, 0, 0, 0, 2353, 2352, 0,
        2351, 0, 0, 0, 2350, 0, 0, 0, 0, 0, 0, 0, 0, 2349, 2348, 0, 2347,
        0, 0, 0, 2346, 0, 0, 0, 0, 0, 0, 0, 2345, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2344, 2343, 0, 2342, 0, 0, 0, 2341, 0, 0,
        0, 0, 0, 0, 0, 2340, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2338, 2337, 0, 2336, 0, 0, 0,
        2335, 0, 0, 0, 0, 0, 0, 0, 2334, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2333, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2332,
        0, 0, 0, 2331, 0, 2330, 2329, 0, 0, 0, 0, 2328, 0, 2327, 2326, 0,
        0, 2325, 2324, 0, 2323, 0, 0, 0, 0, 0, 0, 2322, 0, 2321, 2320, 0,
        0, 2319, 2318, 0, 2317, 0, 0, 0, 0, 2316, 2315, 0, 2314, 0, 0, 0,
        2313, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2312, 0, 2311, 2310, 0, 0, 2309,
        2308, 0, 2307, 0, 0, 0, 0, 2306, 2305, 0, 2304, 0, 0, 0, 2303, 0,
        0, 0, 0, 0, 0, 0, 0, 2302, 2301, 0, 2300, 0, 0, 0, 2299, 0, 0, 0,
        0, 0, 0, 0, 2298, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2297, 0, 2296, 2295, 0, 0, 2294, 2293, 0, 2292, 0, 0, 0, 0,
        2291, 2290, 0, 2289, 0, 0, 0, 2288, 0, 0, 0, 0, 0, 0, 0, 0, 2287,
        2286, 0, 2285, 0, 0, 0, 2284, 0, 0, 0, 0, 0, 0, 0, 2283, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2282, 2281, 0, 2280, 0, 0,
        0, 2279, 0, 0, 0, 0, 0, 0, 0, 2278, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2277, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2276, 0,
        2275, 2274, 0, 0, 2273, 2272, 0, 2271, 0, 0, 0, 0, 2270, 2269, 0,
        2268, 0, 0, 0, 2267, 0, 0, 0, 0, 0, 0, 0, 0, 2266, 2265, 0, 2264,
        0, 0, 0, 2263, 0, 0, 0, 0, 0, 0, 0, 2262, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2261, 2260, 0, 2259, 0, 0, 0, 2258, 0, 0,
        0, 0, 0, 0, 0, 2257, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2255, 2254, 0, 2253, 0, 0, 0,
        2252, 0, 0, 0, 0, 0, 0, 0, 2251, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2249, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2248, 0, 2247,
        2246, 0, 0, 2245, 2244, 0, 2243, 0, 0, 0, 0, 2242, 2241, 0, 2240,
        0, 0, 0, 2239, 0, 0, 0, 0, 0, 0, 0, 0, 2238, 2237, 0, 2236, 0, 0,
        0, 2235, 0, 0, 0, 0, 0, 0, 0, 2234, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2233, 2232, 0, 2231, 0, 0, 0, 2230, 0, 0, 0, 0,
        0, 0, 0, 2229, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2228,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2227, 2226, 0, 2225, 0, 0, 0, 2224,
        0, 0, 0, 0, 0, 0, 0, 2223, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2221, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2220, 2219, 0, 2218, 0, 0,
        0, 2217, 0, 0, 0, 0, 0, 0, 0, 2216, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2214, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2213, 0, 0,
        0, 2212, 0, 2211, 2210, 0, 0, 0, 0, 2209, 0, 2208, 2207, 0, 0, 2206,
        2205, 0, 2204, 0, 0, 0, 0, 0, 0, 2203, 0, 2202, 2201, 0, 0, 2200,
        2199, 0, 2198, 0, 0, 0, 0, 2197, 2196, 0, 2195, 0, 0, 0, 2194, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2193, 0, 2192, 2191, 0, 0, 2190, 2189,
        0, 2188, 0, 0, 0, 0, 2187, 2186, 0, 2185, 0, 0, 0, 2184, 0, 0, 0,
        0, 0, 0, 0, 0, 2183, 2182, 0, 2181, 0, 0, 0, 2180, 0, 0, 0, 0, 0,
        0, 0, 2179, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2178, 0, 2177, 2176, 0, 0, 2175, 2174, 0, 2173, 0, 0, 0, 0, 2172,
        2171, 0, 2170, 0, 0, 0, 2169, 0, 0, 0, 0, 0, 0, 0, 0, 2168, 2167,
        0, 2166, 0, 0, 0, 2165, 0, 0, 0, 0, 0, 0, 0, 2164, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2163, 2162, 0, 2161, 0, 0, 0, 2160,
        0, 0, 0, 0, 0, 0, 0, 2159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2158, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2157, 0, 2156, 2155,
        0, 0, 2154, 2153, 0, 2152, 0, 0, 0, 0, 2151, 2150, 0, 2149, 0, 0,
        0, 2148, 0, 0, 0, 0, 0, 0, 0, 0, 2147, 2146, 0, 2145, 0, 0, 0, 2144,
        0, 0, 0, 0, 0, 0, 0, 2143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2142, 2141, 0, 2140, 0, 0, 0, 2139, 0, 0, 0, 0, 0, 0, 0,
        2138, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2137, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2136, 2135, 0, 2134, 0, 0, 0, 2133, 0, 0, 0,
        0, 0, 0, 0, 2132, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2131,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2129, 0, 2128, 2127, 0, 0, 2126,
        2125, 0, 2124, 0, 0, 0, 0, 2123, 2122, 0, 2121, 0, 0, 0, 2120, 0,
        0, 0, 0, 0, 0, 0, 0, 2119, 2118, 0, 2117, 0, 0, 0, 2116, 0, 0, 0,
        0, 0, 0, 0, 2115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2114, 2113, 0, 2112, 0, 0, 0, 2111, 0, 0, 0, 0, 0, 0, 0, 2110, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2109, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2108, 2107, 0, 2106, 0, 0, 0, 2105, 0, 0, 0, 0, 0, 0,
        0, 2104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2103, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2101, 2100, 0, 2099, 0, 0, 0, 2098, 0, 0, 0,
        0, 0, 0, 0, 2097, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2096,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2094, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2093, 0, 2092, 2091, 0, 0, 2090, 2089, 0,
        2088, 0, 0, 0, 0, 2087, 2086, 0, 2085, 0, 0, 0, 2084, 0, 0, 0, 0,
        0, 0, 0, 0, 2083, 2082, 0, 2081, 0, 0, 0, 2080, 0, 0, 0, 0, 0, 0,
        0, 2079, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2078, 2077,
        0, 2076, 0, 0, 0, 2075, 0, 0, 0, 0, 0, 0, 0, 2074, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2073, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2072, 2071, 0, 2070, 0, 0, 0, 2069, 0, 0, 0, 0, 0, 0, 0, 2068, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2067, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2066, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2065, 2064, 0, 2063, 0, 0, 0, 2062, 0, 0, 0, 0, 0, 0, 0,
        2061, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2060, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2059, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2058, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2057, 2056, 0, 2055, 0, 0, 0, 2054, 0, 0, 0, 0, 0, 0, 0,
        2053, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2052, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2051, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2050, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 182, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 191, 0, 0, 0, 2049, 0, 2048, 2047, 0, 0, 0, 0, 2046, 0, 2045, 2044,
        0, 0, 2043, 2042, 0, 2041, 0, 0, 0, 0, 0, 0, 2040, 0, 2039, 2038,
        0, 0, 2037, 2036, 0, 2035, 0, 0, 0, 0, 2034, 2033, 0, 2032, 0, 0,
        0, 2031, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2030, 0, 2029, 2028, 0, 0,
        2027, 2026, 0, 2025, 0, 0, 0, 0, 2024, 2023, 0, 2022, 0, 0, 0, 2021,
        0, 0, 0, 0, 0, 0, 0, 0, 2020, 2019, 0, 2018, 0, 0, 0, 2017, 0, 0,
        0, 0, 0, 0, 0, 2016, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2015, 0, 2014, 2013, 0, 0, 2012, 2011, 0, 2010, 0, 0, 0,
        0, 2009, 2008, 0, 2007, 0, 0, 0, 2006, 0, 0, 0, 0, 0, 0, 0, 0, 2005,
        2004, 0, 2003, 0, 0, 0, 2002, 0, 0, 0, 0, 0, 0, 0, 2001, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 1999, 0, 1998, 0, 0,
        0, 1997, 0, 0, 0, 0, 0, 0, 0, 1996, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1995, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1994, 0,
        1993, 1992, 0, 0, 1991, 1990, 0, 1989, 0, 0, 0, 0, 1988, 1987, 0,
        1986, 0, 0, 0, 1985, 0, 0, 0, 0, 0, 0, 0, 0, 1984, 1983, 0, 1982,
        0, 0, 0, 1981, 0, 0, 0, 0, 0, 0, 0, 1980, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1979, 1978, 0, 1977, 0, 0, 0, 1976, 0, 0,
        0, 0, 0, 0, 0, 1975, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1974, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1973, 1972, 0, 1971, 0, 0, 0,
        1970, 0, 0, 0, 0, 0, 0, 0, 1969, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1968, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1967, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1966, 0, 1965,
        1964, 0, 0, 1963, 1962, 0, 1961, 0, 0, 0, 0, 1960, 1959, 0, 1958,
        0, 0, 0, 1957, 0, 0, 0, 0, 0, 0, 0, 0, 1956, 1955, 0, 1954, 0, 0,
        0, 1953, 0, 0, 0, 0, 0, 0, 0, 1952, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1951, 1950, 0, 1949, 0, 0, 0, 1948, 0, 0, 0, 0,
        0, 0, 0, 1947, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1946,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1945, 1944, 0, 1943, 0, 0, 0, 1942,
        0, 0, 0, 0, 0, 0, 0, 1941, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1940, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1939, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1938, 1937, 0, 1936, 0, 0,
        0, 1935, 0, 0, 0, 0, 0, 0, 0, 1934, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1933, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1932, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1931, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1930, 0, 1929, 1928,
        0, 0, 1927, 1926, 0, 1925, 0, 0, 0, 0, 1924, 1923, 0, 1922, 0, 0,
        0, 1921, 0, 0, 0, 0, 0, 0, 0, 0, 1920, 1919, 0, 1918, 0, 0, 0, 1917,
        0, 0, 0, 0, 0, 0, 0, 1916, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1915, 1914, 0, 1913, 0, 0, 0, 1912, 0, 0, 0, 0, 0, 0, 0,
        1911, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1910, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1909, 1908, 0, 1907, 0, 0, 0, 1906, 0, 0, 0,
        0, 0, 0, 0, 1905, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1904,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1903, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1902, 1901, 0, 1900, 0, 0, 0, 1899, 0,
        0, 0, 0, 0, 0, 0, 1898, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1897, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1896, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1895, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1894, 1893, 0, 1892, 0, 0, 0, 1891, 0,
        0, 0, 0, 0, 0, 0, 1890, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1889, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1888, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1887, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1886, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 1885, 0, 1884, 1883, 0, 0, 1882, 1881, 0, 1880, 0, 0,
        0, 0, 1879, 1878, 0, 1877, 0, 0, 0, 1876, 0, 0, 0, 0, 0, 0, 0, 0,
        1875, 1874, 0, 1873, 0, 0, 0, 1872, 0, 0, 0, 0, 0, 0, 0, 1871, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1870, 1869, 0, 1868,
        0, 0, 0, 1867, 0, 0, 0, 0, 0, 0, 0, 1866, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1865, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1864, 1863,
        0, 1862, 0, 0, 0, 1861, 0, 0, 0, 0, 0, 0, 0, 1860, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1859, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1858,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1857,
        1856, 0, 1855, 0, 0, 0, 1854, 0, 0, 0, 0, 0, 0, 0, 1853, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1852, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1851, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1850, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1849,
        1848, 0, 1847, 0, 0, 0, 1846, 0, 0, 0, 0, 0, 0, 0, 1845, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1844, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1843, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1842, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1841, 0,
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
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1840, 1839, 0, 1838, 0, 0,
        0, 1837, 0, 0, 0, 0, 0, 0, 0, 1836, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1835, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1834, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1833, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1832, 0, 0, 0, 0, 0, 0, 0, 0,
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
        0, 0, 0, 0, 0, 181
    };

    static std::unordered_map< int64_t, int16_t > sProductMap =
    {
         { 16, 13 }
        ,{ 24, 169 }
        ,{ 36, 247 }
        ,{ 40, 168 }
        ,{ 54, 157 }
        ,{ 56, 167 }
        ,{ 60, 1105 }
        ,{ 81, 12 }
        ,{ 84, 1104 }
        ,{ 88, 166 }
        ,{ 90, 1039 }
        ,{ 100, 246 }
        ,{ 104, 165 }
        ,{ 126, 1038 }
        ,{ 132, 1102 }
        ,{ 135, 156 }
        ,{ 136, 164 }
        ,{ 140, 1103 }
        ,{ 150, 973 }
        ,{ 152, 163 }
        ,{ 156, 1099 }
        ,{ 184, 162 }
        ,{ 189, 155 }
        ,{ 196, 244 }
        ,{ 198, 1036 }
        ,{ 204, 1095 }
        ,{ 220, 1101 }
        ,{ 225, 245 }
        ,{ 228, 1090 }
        ,{ 232, 161 }
        ,{ 234, 1033 }
        ,{ 248, 160 }
        ,{ 250, 145 }
        ,{ 260, 1098 }
        ,{ 276, 1084 }
        ,{ 294, 907 }
        ,{ 296, 159 }
        ,{ 297, 154 }
        ,{ 306, 1029 }
        ,{ 308, 1100 }
        ,{ 315, 1037 }
        ,{ 328, 158 }
        ,{ 340, 1094 }
        ,{ 342, 1024 }
        ,{ 348, 1077 }
        ,{ 350, 972 }
        ,{ 351, 153 }
        ,{ 364, 1097 }
        ,{ 372, 1069 }
        ,{ 375, 144 }
        ,{ 380, 1089 }
        ,{ 414, 1018 }
        ,{ 441, 243 }
        ,{ 444, 1060 }
        ,{ 459, 152 }
        ,{ 460, 1083 }
        ,{ 476, 1093 }
        ,{ 484, 241 }
        ,{ 490, 906 }
        ,{ 492, 1050 }
        ,{ 495, 1035 }
        ,{ 513, 151 }
        ,{ 522, 1011 }
        ,{ 525, 971 }
        ,{ 532, 1088 }
        ,{ 550, 970 }
        ,{ 558, 1003 }
        ,{ 572, 1096 }
        ,{ 580, 1076 }
        ,{ 585, 1032 }
        ,{ 620, 1068 }
        ,{ 621, 150 }
        ,{ 625, 11 }
        ,{ 644, 1082 }
        ,{ 650, 967 }
        ,{ 666, 994 }
        ,{ 676, 237 }
        ,{ 686, 133 }
        ,{ 693, 1034 }
        ,{ 726, 841 }
        ,{ 735, 905 }
        ,{ 738, 984 }
        ,{ 740, 1059 }
        ,{ 748, 1092 }
        ,{ 765, 1028 }
        ,{ 783, 149 }
        ,{ 812, 1075 }
        ,{ 819, 1031 }
        ,{ 820, 1049 }
        ,{ 825, 969 }
        ,{ 836, 1087 }
        ,{ 837, 148 }
        ,{ 850, 963 }
        ,{ 855, 1023 }
        ,{ 868, 1067 }
        ,{ 875, 143 }
        ,{ 884, 1091 }
        ,{ 950, 958 }
        ,{ 975, 966 }
        ,{ 988, 1086 }
        ,{ 999, 147 }
        ,{ 1012, 1081 }
        ,{ 1014, 775 }
        ,{ 1029, 132 }
        ,{ 1035, 1017 }
        ,{ 1036, 1058 }
        ,{ 1071, 1027 }
        ,{ 1078, 904 }
        ,{ 1089, 240 }
        ,{ 1107, 146 }
        ,{ 1148, 1048 }
        ,{ 1150, 952 }
        ,{ 1156, 232 }
        ,{ 1196, 1080 }
        ,{ 1197, 1022 }
        ,{ 1210, 840 }
        ,{ 1225, 242 }
        ,{ 1274, 901 }
        ,{ 1275, 962 }
        ,{ 1276, 1074 }
        ,{ 1287, 1030 }
        ,{ 1292, 1085 }
        ,{ 1305, 1010 }
        ,{ 1364, 1066 }
        ,{ 1375, 142 }
        ,{ 1395, 1002 }
        ,{ 1425, 957 }
        ,{ 1444, 226 }
        ,{ 1449, 1016 }
        ,{ 1450, 945 }
        ,{ 1508, 1073 }
        ,{ 1521, 236 }
        ,{ 1550, 937 }
        ,{ 1564, 1079 }
        ,{ 1612, 1065 }
        ,{ 1617, 903 }
        ,{ 1625, 141 }
        ,{ 1628, 1057 }
        ,{ 1665, 993 }
        ,{ 1666, 897 }
        ,{ 1683, 1026 }
        ,{ 1690, 774 }
        ,{ 1694, 838 }
        ,{ 1715, 131 }
        ,{ 1725, 951 }
        ,{ 1734, 709 }
        ,{ 1748, 1078 }
        ,{ 1804, 1047 }
        ,{ 1815, 839 }
        ,{ 1827, 1009 }
        ,{ 1845, 983 }
        ,{ 1850, 928 }
        ,{ 1862, 892 }
        ,{ 1881, 1021 }
        ,{ 1911, 900 }
        ,{ 1924, 1056 }
        ,{ 1925, 968 }
        ,{ 1953, 1001 }
        ,{ 1972, 1072 }
        ,{ 1989, 1025 }
        ,{ 2050, 918 }
        ,{ 2108, 1064 }
        ,{ 2116, 219 }
        ,{ 2125, 140 }
        ,{ 2132, 1046 }
        ,{ 2166, 643 }
        ,{ 2175, 944 }
        ,{ 2204, 1071 }
        ,{ 2223, 1020 }
        ,{ 2254, 886 }
        ,{ 2275, 965 }
        ,{ 2277, 1015 }
        ,{ 2325, 936 }
        ,{ 2331, 992 }
        ,{ 2356, 1063 }
        ,{ 2366, 772 }
        ,{ 2375, 139 }
        ,{ 2401, 10 }
        ,{ 2499, 896 }
        ,{ 2516, 1055 }
        ,{ 2535, 773 }
        ,{ 2541, 837 }
        ,{ 2583, 982 }
        ,{ 2601, 231 }
        ,{ 2662, 121 }
        ,{ 2668, 1070 }
        ,{ 2691, 1014 }
        ,{ 2695, 902 }
        ,{ 2775, 927 }
        ,{ 2788, 1045 }
        ,{ 2793, 891 }
        ,{ 2812, 1054 }
        ,{ 2842, 879 }
        ,{ 2852, 1062 }
        ,{ 2871, 1008 }
        ,{ 2875, 138 }
        ,{ 2890, 708 }
        ,{ 2907, 1019 }
        ,{ 2975, 961 }
        ,{ 3025, 239 }
        ,{ 3038, 871 }
        ,{ 3069, 1000 }
        ,{ 3075, 917 }
        ,{ 3116, 1044 }
        ,{ 3146, 835 }
        ,{ 3174, 577 }
        ,{ 3185, 899 }
        ,{ 3249, 225 }
        ,{ 3325, 956 }
        ,{ 3364, 211 }
        ,{ 3381, 885 }
        ,{ 3393, 1007 }
        ,{ 3404, 1053 }
        ,{ 3519, 1013 }
        ,{ 3549, 771 }
        ,{ 3575, 964 }
        ,{ 3596, 1061 }
        ,{ 3610, 642 }
        ,{ 3625, 137 }
        ,{ 3626, 862 }
        ,{ 3627, 999 }
        ,{ 3663, 991 }
        ,{ 3718, 769 }
        ,{ 3772, 1043 }
        ,{ 3773, 130 }
        ,{ 3844, 202 }
        ,{ 3875, 136 }
        ,{ 3933, 1012 }
        ,{ 3993, 120 }
        ,{ 4018, 852 }
        ,{ 4025, 950 }
        ,{ 4046, 706 }
        ,{ 4059, 981 }
        ,{ 4114, 831 }
        ,{ 4165, 895 }
        ,{ 4225, 235 }
        ,{ 4235, 836 }
        ,{ 4263, 878 }
        ,{ 4292, 1052 }
        ,{ 4329, 990 }
        ,{ 4335, 707 }
        ,{ 4394, 109 }
        ,{ 4437, 1006 }
        ,{ 4459, 129 }
        ,{ 4557, 870 }
        ,{ 4588, 1051 }
        ,{ 4598, 826 }
        ,{ 4625, 135 }
        ,{ 4655, 890 }
        ,{ 4675, 960 }
        ,{ 4719, 834 }
        ,{ 4743, 998 }
        ,{ 4756, 1042 }
        ,{ 4761, 218 }
        ,{ 4797, 980 }
        ,{ 4959, 1005 }
        ,{ 5046, 511 }
        ,{ 5054, 640 }
        ,{ 5075, 943 }
        ,{ 5084, 1041 }
        ,{ 5125, 134 }
        ,{ 5225, 955 }
        ,{ 5290, 576 }
        ,{ 5301, 997 }
        ,{ 5415, 641 }
        ,{ 5425, 935 }
        ,{ 5439, 861 }
        ,{ 5476, 192 }
        ,{ 5525, 959 }
        ,{ 5566, 820 }
        ,{ 5577, 768 }
        ,{ 5635, 884 }
        ,{ 5661, 989 }
        ,{ 5746, 765 }
        ,{ 5766, 445 }
        ,{ 5831, 128 }
        ,{ 5915, 770 }
        ,{ 5929, 238 }
        ,{ 6003, 1004 }
        ,{ 6027, 851 }
        ,{ 6068, 1040 }
        ,{ 6069, 705 }
        ,{ 6171, 830 }
        ,{ 6175, 954 }
        ,{ 6273, 979 }
        ,{ 6325, 949 }
        ,{ 6327, 988 }
        ,{ 6358, 703 }
        ,{ 6417, 996 }
        ,{ 6422, 760 }
        ,{ 6475, 926 }
        ,{ 6517, 127 }
        ,{ 6591, 108 }
        ,{ 6655, 119 }
        ,{ 6724, 181 }
        ,{ 6897, 825 }
        ,{ 7007, 898 }
        ,{ 7011, 978 }
        ,{ 7018, 813 }
        ,{ 7105, 877 }
        ,{ 7175, 916 }
        ,{ 7225, 230 }
        ,{ 7406, 574 }
        ,{ 7475, 948 }
        ,{ 7502, 805 }
        ,{ 7514, 699 }
        ,{ 7569, 210 }
        ,{ 7581, 639 }
        ,{ 7595, 869 }
        ,{ 7659, 987 }
        ,{ 7774, 754 }
        ,{ 7865, 833 }
        ,{ 7889, 126 }
        ,{ 7935, 575 }
        ,{ 7942, 637 }
        ,{ 7975, 942 }
        ,{ 8075, 953 }
        ,{ 8091, 995 }
        ,{ 8214, 379 }
        ,{ 8281, 234 }
        ,{ 8349, 819 }
        ,{ 8410, 510 }
        ,{ 8487, 977 }
        ,{ 8525, 934 }
        ,{ 8619, 764 }
        ,{ 8649, 201 }
        ,{ 8954, 796 }
        ,{ 9025, 224 }
        ,{ 9065, 860 }
        ,{ 9163, 894 }
        ,{ 9295, 767 }
        ,{ 9317, 118 }
        ,{ 9386, 633 }
        ,{ 9425, 941 }
        ,{ 9537, 702 }
        ,{ 9610, 444 }
        ,{ 9633, 759 }
        ,{ 9657, 986 }
        ,{ 9775, 947 }
        ,{ 9802, 747 }
        ,{ 9826, 97 }
        ,{ 9922, 786 }
        ,{ 9947, 125 }
        ,{ 10045, 850 }
        ,{ 10075, 933 }
        ,{ 10086, 313 }
        ,{ 10115, 704 }
        ,{ 10175, 925 }
        ,{ 10241, 889 }
        ,{ 10285, 829 }
        ,{ 10323, 985 }
        ,{ 10478, 739 }
        ,{ 10527, 812 }
        ,{ 10633, 124 }
        ,{ 10701, 976 }
        ,{ 10829, 893 }
        ,{ 10925, 946 }
        ,{ 10982, 694 }
        ,{ 10985, 107 }
        ,{ 11011, 832 }
        ,{ 11109, 573 }
        ,{ 11253, 804 }
        ,{ 11271, 698 }
        ,{ 11275, 915 }
        ,{ 11439, 975 }
        ,{ 11495, 824 }
        ,{ 11638, 571 }
        ,{ 11661, 753 }
        ,{ 11774, 508 }
        ,{ 11913, 636 }
        ,{ 12025, 924 }
        ,{ 12103, 888 }
        ,{ 12274, 628 }
        ,{ 12321, 191 }
        ,{ 12325, 940 }
        ,{ 12397, 883 }
        ,{ 12506, 730 }
        ,{ 12615, 509 }
        ,{ 12635, 638 }
        ,{ 12691, 123 }
        ,{ 13013, 766 }
        ,{ 13175, 932 }
        ,{ 13225, 217 }
        ,{ 13294, 688 }
        ,{ 13325, 914 }
        ,{ 13431, 795 }
        ,{ 13454, 442 }
        ,{ 13653, 974 }
        ,{ 13690, 378 }
        ,{ 13718, 85 }
        ,{ 13754, 567 }
        ,{ 13775, 939 }
        ,{ 13858, 720 }
        ,{ 13915, 818 }
        ,{ 14063, 122 }
        ,{ 14079, 632 }
        ,{ 14161, 229 }
        ,{ 14365, 763 }
        ,{ 14399, 828 }
        ,{ 14415, 443 }
        ,{ 14641, 9 }
        ,{ 14651, 882 }
        ,{ 14703, 746 }
        ,{ 14725, 931 }
        ,{ 14739, 96 }
        ,{ 14883, 785 }
        ,{ 15129, 180 }
        ,{ 15379, 106 }
        ,{ 15631, 876 }
        ,{ 15717, 738 }
        ,{ 15725, 923 }
        ,{ 15827, 887 }
        ,{ 15895, 701 }
        ,{ 16055, 758 }
        ,{ 16093, 823 }
        ,{ 16473, 693 }
        ,{ 16606, 622 }
        ,{ 16675, 938 }
        ,{ 16709, 868 }
        ,{ 16762, 681 }
        ,{ 16810, 312 }
        ,{ 17303, 117 }
        ,{ 17425, 913 }
        ,{ 17457, 570 }
        ,{ 17545, 811 }
        ,{ 17575, 922 }
        ,{ 17661, 507 }
        ,{ 17689, 223 }
        ,{ 17825, 930 }
        ,{ 17918, 673 }
        ,{ 17986, 562 }
        ,{ 18411, 627 }
        ,{ 18473, 875 }
        ,{ 18502, 505 }
        ,{ 18515, 572 }
        ,{ 18755, 803 }
        ,{ 18759, 729 }
        ,{ 18785, 697 }
        ,{ 19159, 881 }
        ,{ 19166, 376 }
        ,{ 19435, 752 }
        ,{ 19475, 912 }
        ,{ 19481, 817 }
        ,{ 19747, 867 }
        ,{ 19855, 635 }
        ,{ 19941, 687 }
        ,{ 19943, 859 }
        ,{ 20102, 556 }
        ,{ 20111, 762 }
        ,{ 20181, 441 }
        ,{ 20449, 233 }
        ,{ 20535, 377 }
        ,{ 20577, 84 }
        ,{ 20631, 566 }
        ,{ 20787, 719 }
        ,{ 20938, 615 }
        ,{ 21025, 209 }
        ,{ 21142, 439 }
        ,{ 21275, 921 }
        ,{ 21386, 664 }
        ,{ 21413, 880 }
        ,{ 21866, 501 }
        ,{ 22099, 849 }
        ,{ 22253, 700 }
        ,{ 22382, 607 }
        ,{ 22385, 794 }
        ,{ 22475, 929 }
        ,{ 22477, 757 }
        ,{ 22627, 116 }
        ,{ 23465, 631 }
        ,{ 23534, 310 }
        ,{ 23569, 858 }
        ,{ 23575, 911 }
        ,{ 23698, 654 }
        ,{ 24025, 200 }
        ,{ 24157, 874 }
        ,{ 24167, 105 }
        ,{ 24334, 73 }
        ,{ 24505, 745 }
        ,{ 24563, 810 }
        ,{ 24565, 95 }
        ,{ 24805, 784 }
        ,{ 24909, 621 }
        ,{ 24986, 435 }
        ,{ 25143, 680 }
        ,{ 25215, 311 }
        ,{ 25289, 115 }
        ,{ 25823, 866 }
        ,{ 25921, 216 }
        ,{ 26117, 848 }
        ,{ 26195, 737 }
        ,{ 26257, 802 }
        ,{ 26299, 696 }
        ,{ 26714, 598 }
        ,{ 26741, 827 }
        ,{ 26825, 920 }
        ,{ 26877, 672 }
        ,{ 26979, 561 }
        ,{ 26999, 873 }
        ,{ 27209, 751 }
        ,{ 27455, 692 }
        ,{ 27753, 504 }
        ,{ 27797, 634 }
        ,{ 28561, 8 }
        ,{ 28594, 496 }
        ,{ 28675, 919 }
        ,{ 28749, 375 }
        ,{ 28861, 865 }
        ,{ 29095, 569 }
        ,{ 29435, 506 }
        ,{ 29602, 588 }
        ,{ 29725, 910 }
        ,{ 29887, 822 }
        ,{ 30118, 373 }
        ,{ 30153, 555 }
        ,{ 30613, 114 }
        ,{ 30682, 549 }
        ,{ 30685, 626 }
        ,{ 30821, 857 }
        ,{ 31265, 728 }
        ,{ 31339, 793 }
        ,{ 31407, 614 }
        ,{ 31603, 761 }
        ,{ 31713, 438 }
        ,{ 31775, 909 }
        ,{ 31958, 490 }
        ,{ 32079, 663 }
        ,{ 32674, 430 }
        ,{ 32683, 872 }
        ,{ 32798, 541 }
        ,{ 32799, 500 }
        ,{ 32851, 630 }
        ,{ 33235, 686 }
        ,{ 33573, 606 }
        ,{ 33635, 440 }
        ,{ 34153, 847 }
        ,{ 34225, 190 }
        ,{ 34295, 83 }
        ,{ 34307, 744 }
        ,{ 34385, 565 }
        ,{ 34391, 94 }
        ,{ 34447, 856 }
        ,{ 34645, 718 }
        ,{ 34727, 783 }
        ,{ 34937, 864 }
        ,{ 34969, 228 }
        ,{ 35301, 309 }
        ,{ 35321, 756 }
        ,{ 35547, 653 }
        ,{ 35594, 369 }
        ,{ 36179, 816 }
        ,{ 36501, 72 }
        ,{ 36518, 424 }
        ,{ 36673, 736 }
        ,{ 36982, 307 }
        ,{ 37349, 104 }
        ,{ 37479, 434 }
        ,{ 37925, 908 }
        ,{ 38171, 846 }
        ,{ 38437, 691 }
        ,{ 38599, 113 }
        ,{ 38686, 483 }
        ,{ 39083, 821 }
        ,{ 39146, 532 }
        ,{ 40071, 597 }
        ,{ 40733, 568 }
        ,{ 41209, 208 }
        ,{ 41261, 112 }
        ,{ 41327, 695 }
        ,{ 41515, 620 }
        ,{ 41699, 855 }
        ,{ 41743, 103 }
        ,{ 41905, 679 }
        ,{ 42025, 179 }
        ,{ 42757, 750 }
        ,{ 42891, 495 }
        ,{ 42959, 625 }
        ,{ 43378, 522 }
        ,{ 43681, 222 }
        ,{ 43706, 303 }
        ,{ 43771, 727 }
        ,{ 44051, 863 }
        ,{ 44206, 417 }
        ,{ 44403, 587 }
        ,{ 44795, 671 }
        ,{ 44965, 560 }
        ,{ 45177, 372 }
        ,{ 45617, 809 }
        ,{ 46023, 548 }
        ,{ 46207, 845 }
        ,{ 46255, 503 }
        ,{ 46529, 685 }
        ,{ 46546, 364 }
        ,{ 47089, 199 }
        ,{ 47311, 815 }
        ,{ 47915, 374 }
        ,{ 47937, 489 }
        ,{ 48013, 82 }
        ,{ 48139, 564 }
        ,{ 48503, 717 }
        ,{ 48763, 801 }
        ,{ 48778, 61 }
        ,{ 48841, 227 }
        ,{ 49011, 429 }
        ,{ 49197, 540 }
        ,{ 49247, 111 }
        ,{ 50255, 554 }
        ,{ 50531, 102 }
        ,{ 51623, 629 }
        ,{ 52022, 358 }
        ,{ 52142, 475 }
        ,{ 52345, 613 }
        ,{ 52577, 854 }
        ,{ 52855, 437 }
        ,{ 52877, 814 }
        ,{ 53391, 368 }
        ,{ 53465, 662 }
        ,{ 53911, 743 }
        ,{ 54043, 93 }
        ,{ 54571, 110 }
        ,{ 54587, 755 }
        ,{ 54665, 499 }
        ,{ 54777, 423 }
        ,{ 55473, 306 }
        ,{ 55738, 409 }
        ,{ 55955, 605 }
        ,{ 56203, 853 }
        ,{ 57154, 298 }
        ,{ 57629, 735 }
        ,{ 58029, 482 }
        ,{ 58121, 619 }
        ,{ 58201, 792 }
        ,{ 58261, 844 }
        ,{ 58667, 678 }
        ,{ 58719, 531 }
        ,{ 58835, 308 }
        ,{ 59245, 652 }
        ,{ 59582, 49 }
        ,{ 59653, 808 }
        ,{ 60401, 690 }
        ,{ 60835, 71 }
        ,{ 61009, 221 }
        ,{ 62234, 466 }
        ,{ 62279, 843 }
        ,{ 62465, 433 }
        ,{ 62713, 670 }
        ,{ 62951, 559 }
        ,{ 62974, 351 }
        ,{ 63713, 101 }
        ,{ 63767, 800 }
        ,{ 63869, 92 }
        ,{ 63878, 292 }
        ,{ 64009, 215 }
        ,{ 64493, 782 }
        ,{ 64757, 502 }
        ,{ 65067, 521 }
        ,{ 65559, 302 }
        ,{ 66079, 749 }
        ,{ 66309, 416 }
        ,{ 66671, 807 }
        ,{ 66785, 596 }
        ,{ 67081, 189 }
        ,{ 67507, 624 }
        ,{ 68107, 100 }
        ,{ 68783, 726 }
        ,{ 68962, 456 }
        ,{ 69819, 363 }
        ,{ 70357, 553 }
        ,{ 71114, 400 }
        ,{ 71269, 799 }
        ,{ 71383, 689 }
        ,{ 71485, 494 }
        ,{ 73117, 684 }
        ,{ 73167, 60 }
        ,{ 73283, 612 }
        ,{ 73853, 748 }
        ,{ 73997, 436 }
        ,{ 74005, 586 }
        ,{ 74333, 842 }
        ,{ 74851, 661 }
        ,{ 75295, 371 }
        ,{ 75449, 81 }
        ,{ 75647, 563 }
        ,{ 76109, 791 }
        ,{ 76219, 716 }
        ,{ 76531, 498 }
        ,{ 76705, 547 }
        ,{ 77326, 285 }
        ,{ 78033, 357 }
        ,{ 78213, 474 }
        ,{ 78337, 604 }
        ,{ 78802, 390 }
        ,{ 79402, 343 }
        ,{ 79781, 623 }
        ,{ 79895, 488 }
        ,{ 80707, 806 }
        ,{ 81289, 99 }
        ,{ 81685, 428 }
        ,{ 81995, 539 }
        ,{ 82369, 178 }
        ,{ 82943, 651 }
        ,{ 83317, 742 }
        ,{ 83521, 7 }
        ,{ 83607, 408 }
        ,{ 84337, 781 }
        ,{ 84878, 334 }
        ,{ 85063, 790 }
        ,{ 85169, 70 }
        ,{ 85731, 297 }
        ,{ 86273, 798 }
        ,{ 86411, 683 }
        ,{ 87451, 432 }
        ,{ 88985, 367 }
        ,{ 89063, 734 }
        ,{ 89167, 80 }
        ,{ 89373, 48 }
        ,{ 89401, 214 }
        ,{ 90077, 98 }
        ,{ 91295, 422 }
        ,{ 91333, 618 }
        ,{ 92191, 677 }
        ,{ 92455, 305 }
        ,{ 93119, 741 }
        ,{ 93347, 91 }
        ,{ 93351, 465 }
        ,{ 93499, 595 }
        ,{ 94259, 780 }
        ,{ 94461, 350 }
        ,{ 95817, 291 }
        ,{ 96715, 481 }
        ,{ 97498, 277 }
        ,{ 97865, 530 }
        ,{ 98549, 669 }
        ,{ 98923, 558 }
        ,{ 99541, 733 }
        ,{ 100079, 493 }
        ,{ 101306, 37 }
        ,{ 101761, 207 }
        ,{ 102971, 789 }
        ,{ 103443, 455 }
        ,{ 103607, 585 }
        ,{ 104222, 268 }
        ,{ 104329, 220 }
        ,{ 105413, 370 }
        ,{ 106301, 725 }
        ,{ 106671, 399 }
        ,{ 107387, 546 }
        ,{ 107939, 617 }
        ,{ 108445, 520 }
        ,{ 108779, 797 }
        ,{ 108953, 676 }
        ,{ 109265, 301 }
        ,{ 110515, 415 }
        ,{ 110561, 552 }
        ,{ 111853, 487 }
        ,{ 112258, 324 }
        ,{ 112723, 740 }
        ,{ 112999, 90 }
        ,{ 114103, 779 }
        ,{ 114359, 427 }
        ,{ 114793, 538 }
        ,{ 115159, 611 }
        ,{ 115989, 284 }
        ,{ 116281, 198 }
        ,{ 116365, 362 }
        ,{ 116467, 668 }
        ,{ 116603, 79 }
        ,{ 116909, 557 }
        ,{ 117623, 660 }
        ,{ 117793, 715 }
        ,{ 118203, 389 }
        ,{ 118807, 724 }
        ,{ 119103, 342 }
        ,{ 120263, 497 }
        ,{ 120497, 732 }
        ,{ 121945, 59 }
        ,{ 123101, 603 }
        ,{ 124394, 258 }
        ,{ 124579, 366 }
        ,{ 126293, 682 }
        ,{ 127317, 333 }
        ,{ 127813, 421 }
        ,{ 129437, 304 }
        ,{ 129833, 788 }
        ,{ 130055, 356 }
        ,{ 130321, 6 }
        ,{ 130339, 650 }
        ,{ 130355, 473 }
        ,{ 130663, 551 }
        ,{ 131651, 714 }
        ,{ 133837, 69 }
        ,{ 135401, 480 }
        ,{ 136097, 610 }
        ,{ 137011, 529 }
        ,{ 137423, 431 }
        ,{ 137842, 25 }
        ,{ 138787, 787 }
        ,{ 139009, 659 }
        ,{ 139345, 407 }
        ,{ 141151, 616 }
        ,{ 142129, 206 }
        ,{ 142477, 89 }
        ,{ 142885, 296 }
        ,{ 143819, 723 }
        ,{ 143869, 778 }
        ,{ 145483, 602 }
        ,{ 146247, 276 }
        ,{ 146927, 594 }
        ,{ 148955, 47 }
        ,{ 151823, 519 }
        ,{ 151931, 731 }
        ,{ 151959, 36 }
        ,{ 152303, 88 }
        ,{ 152881, 213 }
        ,{ 152971, 300 }
        ,{ 153791, 777 }
        ,{ 154037, 649 }
        ,{ 154721, 414 }
        ,{ 155585, 464 }
        ,{ 156333, 267 }
        ,{ 157267, 492 }
        ,{ 157435, 349 }
        ,{ 157757, 78 }
        ,{ 158171, 68 }
        ,{ 159239, 675 }
        ,{ 159367, 713 }
        ,{ 159695, 290 }
        ,{ 162409, 197 }
        ,{ 162811, 584 }
        ,{ 162911, 361 }
        ,{ 165649, 188 }
        ,{ 168387, 323 }
        ,{ 168751, 545 }
        ,{ 170221, 667 }
        ,{ 170723, 58 }
        ,{ 170867, 550 }
        ,{ 172405, 454 }
        ,{ 173641, 593 }
        ,{ 175769, 486 }
        ,{ 177785, 398 }
        ,{ 177973, 609 }
        ,{ 179707, 426 }
        ,{ 180389, 537 }
        ,{ 181337, 722 }
        ,{ 181781, 87 }
        ,{ 182077, 355 }
        ,{ 182497, 472 }
        ,{ 183557, 776 }
        ,{ 185861, 491 }
        ,{ 186591, 257 }
        ,{ 190247, 601 }
        ,{ 190969, 212 }
        ,{ 192413, 583 }
        ,{ 192763, 674 }
        ,{ 193315, 283 }
        ,{ 193843, 721 }
        ,{ 195083, 406 }
        ,{ 195767, 365 }
        ,{ 197005, 388 }
        ,{ 198505, 341 }
        ,{ 198911, 77 }
        ,{ 199433, 544 }
        ,{ 200039, 295 }
        ,{ 200849, 420 }
        ,{ 200941, 712 }
        ,{ 201433, 86 }
        ,{ 203167, 658 }
        ,{ 203401, 177 }
        ,{ 206057, 666 }
        ,{ 206763, 24 }
        ,{ 206839, 67 }
        ,{ 207727, 485 }
        ,{ 208537, 46 }
        ,{ 212195, 332 }
        ,{ 212381, 425 }
        ,{ 212629, 76 }
        ,{ 212773, 479 }
        ,{ 213187, 536 }
        ,{ 214799, 711 }
        ,{ 215303, 528 }
        ,{ 217819, 463 }
        ,{ 220409, 348 }
        ,{ 223573, 289 }
        ,{ 225131, 648 }
        ,{ 227069, 592 }
        ,{ 231173, 66 }
        ,{ 231361, 187 }
        ,{ 237367, 419 }
        ,{ 238579, 518 }
        ,{ 240383, 299 }
        ,{ 240787, 608 }
        ,{ 241367, 453 }
        ,{ 243049, 205 }
        ,{ 243133, 413 }
        ,{ 243745, 275 }
        ,{ 245939, 657 }
        ,{ 248899, 397 }
        ,{ 251459, 478 }
        ,{ 251617, 582 }
        ,{ 253265, 35 }
        ,{ 253783, 75 }
        ,{ 254449, 527 }
        ,{ 256003, 360 }
        ,{ 256373, 710 }
        ,{ 257393, 600 }
        ,{ 259811, 665 }
        ,{ 260555, 266 }
        ,{ 260797, 543 }
        ,{ 268279, 57 }
        ,{ 270641, 282 }
        ,{ 271643, 484 }
        ,{ 272527, 647 }
        ,{ 275807, 387 }
        ,{ 277729, 196 }
        ,{ 277907, 340 }
        ,{ 278783, 535 }
        ,{ 279841, 5 }
        ,{ 280645, 322 }
        ,{ 281219, 74 }
        ,{ 281957, 517 }
        ,{ 284089, 176 }
        ,{ 286121, 354 }
        ,{ 286781, 471 }
        ,{ 287339, 412 }
        ,{ 291479, 542 }
        ,{ 297073, 331 }
        ,{ 302549, 359 }
        ,{ 303601, 204 }
        ,{ 306559, 405 }
        ,{ 307211, 591 }
        ,{ 310097, 656 }
        ,{ 310403, 418 }
        ,{ 310985, 256 }
        ,{ 311581, 534 }
        ,{ 314347, 294 }
        ,{ 317057, 56 }
        ,{ 324539, 599 }
        ,{ 327701, 45 }
        ,{ 328831, 477 }
        ,{ 331483, 655 }
        ,{ 332741, 526 }
        ,{ 338143, 353 }
        ,{ 338923, 470 }
        ,{ 340423, 581 }
        ,{ 341243, 274 }
        ,{ 342287, 462 }
        ,{ 343621, 646 }
        ,{ 344605, 23 }
        ,{ 346357, 347 }
        ,{ 346921, 195 }
        ,{ 351329, 288 }
        ,{ 352843, 65 }
        ,{ 354571, 34 }
        ,{ 362297, 404 }
        ,{ 364777, 265 }
        ,{ 367319, 645 }
        ,{ 367517, 476 }
        ,{ 368713, 516 }
        ,{ 371501, 293 }
        ,{ 371887, 525 }
        ,{ 375751, 411 }
        ,{ 377177, 64 }
        ,{ 379291, 452 }
        ,{ 387283, 44 }
        ,{ 387353, 590 }
        ,{ 391127, 396 }
        ,{ 392903, 321 }
        ,{ 395641, 186 }
        ,{ 404521, 461 }
        ,{ 409331, 346 }
        ,{ 412091, 515 }
        ,{ 414067, 589 }
        ,{ 414613, 55 }
        ,{ 415207, 287 }
        ,{ 419957, 410 }
        ,{ 425293, 281 }
        ,{ 429229, 580 }
        ,{ 433411, 386 }
        ,{ 435379, 255 }
        ,{ 436711, 339 }
        ,{ 438413, 644 }
        ,{ 442187, 352 }
        ,{ 443207, 469 }
        ,{ 444889, 203 }
        ,{ 448253, 451 }
        ,{ 450179, 63 }
        ,{ 458831, 579 }
        ,{ 462241, 395 }
        ,{ 463391, 54 }
        ,{ 466829, 330 }
        ,{ 473773, 403 }
        ,{ 475571, 533 }
        ,{ 482447, 22 }
        ,{ 485809, 175 }
        ,{ 494209, 185 }
        ,{ 495349, 468 }
        ,{ 498847, 62 }
        ,{ 502619, 280 }
        ,{ 506447, 43 }
        ,{ 508369, 194 }
        ,{ 512213, 385 }
        ,{ 516113, 338 }
        ,{ 528989, 460 }
        ,{ 529511, 402 }
        ,{ 535279, 345 }
        ,{ 536239, 273 }
        ,{ 542963, 286 }
        ,{ 547637, 578 }
        ,{ 551707, 329 }
        ,{ 557183, 33 }
        ,{ 560947, 53 }
        ,{ 566029, 42 }
        ,{ 567617, 524 }
        ,{ 573221, 264 }
        ,{ 586177, 450 }
        ,{ 591223, 459 }
        ,{ 598253, 344 }
        ,{ 599633, 467 }
        ,{ 604469, 394 }
        ,{ 606763, 523 }
        ,{ 606841, 174 }
        ,{ 617419, 320 }
        ,{ 628981, 514 }
        ,{ 633737, 272 }
        ,{ 640987, 401 }
        ,{ 655139, 449 }
        ,{ 657271, 279 }
        ,{ 658489, 32 }
        ,{ 669817, 384 }
        ,{ 672359, 513 }
        ,{ 674917, 337 }
        ,{ 675583, 393 }
        ,{ 677443, 263 }
        ,{ 684167, 254 }
        ,{ 685193, 41 }
        ,{ 707281, 4 }
        ,{ 715691, 458 }
        ,{ 721463, 328 }
        ,{ 724201, 184 }
        ,{ 729677, 319 }
        ,{ 734597, 278 }
        ,{ 748619, 383 }
        ,{ 754319, 336 }
        ,{ 756059, 52 }
        ,{ 758131, 21 }
        ,{ 793063, 448 }
        ,{ 802493, 512 }
        ,{ 806341, 327 }
        ,{ 808201, 193 }
        ,{ 808561, 253 }
        ,{ 817811, 392 }
        ,{ 828733, 271 }
        ,{ 861101, 31 }
        ,{ 863939, 40 }
        ,{ 885887, 262 }
        ,{ 889249, 173 }
        ,{ 895973, 20 }
        ,{ 902393, 51 }
        ,{ 906223, 382 }
        ,{ 913123, 335 }
        ,{ 923521, 3 }
        ,{ 926231, 270 }
        ,{ 954193, 318 }
        ,{ 962407, 30 }
        ,{ 964627, 457 }
        ,{ 976097, 326 }
        ,{ 990109, 261 }
        ,{ 999949, 50 }
        ,{ 1031153, 391 }
        ,{ 1057349, 252 }
        ,{ 1066451, 317 }
        ,{ 1068911, 447 }
        ,{ 1102267, 39 }
        ,{ 1121227, 269 }
        ,{ 1142629, 381 }
        ,{ 1151329, 183 }
        ,{ 1165019, 29 }
        ,{ 1171657, 19 }
        ,{ 1181743, 251 }
        ,{ 1198553, 260 }
        ,{ 1221431, 38 }
        ,{ 1230731, 325 }
        ,{ 1275797, 446 }
        ,{ 1290967, 316 }
        ,{ 1309499, 18 }
        ,{ 1315609, 182 }
        ,{ 1413721, 172 }
        ,{ 1430531, 250 }
        ,{ 1457837, 380 }
        ,{ 1468937, 28 }
        ,{ 1511219, 259 }
        ,{ 1570243, 27 }
        ,{ 1585183, 17 }
        ,{ 1615441, 171 }
        ,{ 1627741, 315 }
        ,{ 1739999, 314 }
        ,{ 1803713, 249 }
        ,{ 1874161, 2 }
        ,{ 1928107, 248 }
        ,{ 1998709, 16 }
        ,{ 2076773, 26 }
        ,{ 2136551, 15 }
        ,{ 2301289, 170 }
        ,{ 2550077, 14 }
        ,{ 2825761, 1 }
    };

    static std::unordered_map< int64_t, int16_t > sStraitsAndFlushesProductMap =
    {
         { 16, 13 }
        ,{ 24, 180 }
        ,{ 36, 269 }
        ,{ 40, 179 }
        ,{ 54, 168 }
        ,{ 56, 178 }
        ,{ 60, 1831 }
        ,{ 81, 12 }
        ,{ 84, 1830 }
        ,{ 88, 177 }
        ,{ 90, 1765 }
        ,{ 100, 268 }
        ,{ 104, 176 }
        ,{ 126, 1764 }
        ,{ 132, 1828 }
        ,{ 135, 167 }
        ,{ 136, 175 }
        ,{ 140, 1829 }
        ,{ 150, 1699 }
        ,{ 152, 174 }
        ,{ 156, 1825 }
        ,{ 184, 173 }
        ,{ 189, 166 }
        ,{ 196, 266 }
        ,{ 198, 1762 }
        ,{ 204, 1821 }
        ,{ 220, 1827 }
        ,{ 225, 267 }
        ,{ 228, 1816 }
        ,{ 232, 172 }
        ,{ 234, 1759 }
        ,{ 248, 171 }
        ,{ 250, 156 }
        ,{ 260, 1824 }
        ,{ 276, 1810 }
        ,{ 294, 1633 }
        ,{ 296, 170 }
        ,{ 297, 165 }
        ,{ 306, 1755 }
        ,{ 308, 1826 }
        ,{ 315, 1763 }
        ,{ 328, 169 }
        ,{ 340, 1820 }
        ,{ 342, 1750 }
        ,{ 348, 1803 }
        ,{ 350, 1698 }
        ,{ 351, 164 }
        ,{ 364, 1823 }
        ,{ 372, 1795 }
        ,{ 375, 155 }
        ,{ 380, 1815 }
        ,{ 414, 1744 }
        ,{ 441, 265 }
        ,{ 444, 1786 }
        ,{ 459, 163 }
        ,{ 460, 1809 }
        ,{ 476, 1819 }
        ,{ 484, 263 }
        ,{ 490, 1632 }
        ,{ 492, 1776 }
        ,{ 495, 1761 }
        ,{ 513, 162 }
        ,{ 522, 1737 }
        ,{ 525, 1697 }
        ,{ 532, 1814 }
        ,{ 550, 1696 }
        ,{ 558, 1729 }
        ,{ 572, 1822 }
        ,{ 580, 1802 }
        ,{ 585, 1758 }
        ,{ 620, 1794 }
        ,{ 621, 161 }
        ,{ 625, 11 }
        ,{ 644, 1808 }
        ,{ 650, 1693 }
        ,{ 666, 1720 }
        ,{ 676, 259 }
        ,{ 686, 144 }
        ,{ 693, 1760 }
        ,{ 726, 1567 }
        ,{ 735, 1631 }
        ,{ 738, 1710 }
        ,{ 740, 1785 }
        ,{ 748, 1818 }
        ,{ 765, 1754 }
        ,{ 783, 160 }
        ,{ 812, 1801 }
        ,{ 819, 1757 }
        ,{ 820, 1775 }
        ,{ 825, 1695 }
        ,{ 836, 1813 }
        ,{ 837, 159 }
        ,{ 850, 1689 }
        ,{ 855, 1749 }
        ,{ 868, 1793 }
        ,{ 875, 154 }
        ,{ 884, 1817 }
        ,{ 950, 1684 }
        ,{ 975, 1692 }
        ,{ 988, 1812 }
        ,{ 999, 158 }
        ,{ 1012, 1807 }
        ,{ 1014, 1501 }
        ,{ 1029, 143 }
        ,{ 1035, 1743 }
        ,{ 1036, 1784 }
        ,{ 1071, 1753 }
        ,{ 1078, 1630 }
        ,{ 1089, 262 }
        ,{ 1107, 157 }
        ,{ 1148, 1774 }
        ,{ 1150, 1678 }
        ,{ 1156, 254 }
        ,{ 1196, 1806 }
        ,{ 1197, 1748 }
        ,{ 1210, 1566 }
        ,{ 1225, 264 }
        ,{ 1274, 1627 }
        ,{ 1275, 1688 }
        ,{ 1276, 1800 }
        ,{ 1287, 1756 }
        ,{ 1292, 1811 }
        ,{ 1305, 1736 }
        ,{ 1364, 1792 }
        ,{ 1375, 153 }
        ,{ 1395, 1728 }
        ,{ 1425, 1683 }
        ,{ 1444, 248 }
        ,{ 1449, 1742 }
        ,{ 1450, 1671 }
        ,{ 1508, 1799 }
        ,{ 1521, 258 }
        ,{ 1550, 1663 }
        ,{ 1564, 1805 }
        ,{ 1612, 1791 }
        ,{ 1617, 1629 }
        ,{ 1625, 152 }
        ,{ 1628, 1783 }
        ,{ 1665, 1719 }
        ,{ 1666, 1623 }
        ,{ 1683, 1752 }
        ,{ 1690, 1500 }
        ,{ 1694, 1564 }
        ,{ 1715, 142 }
        ,{ 1725, 1677 }
        ,{ 1734, 1435 }
        ,{ 1748, 1804 }
        ,{ 1804, 1773 }
        ,{ 1815, 1565 }
        ,{ 1827, 1735 }
        ,{ 1845, 1709 }
        ,{ 1850, 1654 }
        ,{ 1862, 1618 }
        ,{ 1881, 1747 }
        ,{ 1911, 1626 }
        ,{ 1924, 1782 }
        ,{ 1925, 1694 }
        ,{ 1953, 1727 }
        ,{ 1972, 1798 }
        ,{ 1989, 1751 }
        ,{ 2050, 1644 }
        ,{ 2108, 1790 }
        ,{ 2116, 241 }
        ,{ 2125, 151 }
        ,{ 2132, 1772 }
        ,{ 2166, 1369 }
        ,{ 2175, 1670 }
        ,{ 2204, 1797 }
        ,{ 2223, 1746 }
        ,{ 2254, 1612 }
        ,{ 2275, 1691 }
        ,{ 2277, 1741 }
        ,{ 2325, 1662 }
        ,{ 2331, 1718 }
        ,{ 2356, 1789 }
        ,{ 2366, 1498 }
        ,{ 2375, 150 }
        ,{ 2401, 10 }
        ,{ 2499, 1622 }
        ,{ 2516, 1781 }
        ,{ 2535, 1499 }
        ,{ 2541, 1563 }
        ,{ 2583, 1708 }
        ,{ 2601, 253 }
        ,{ 2662, 132 }
        ,{ 2668, 1796 }
        ,{ 2691, 1740 }
        ,{ 2695, 1628 }
        ,{ 2775, 1653 }
        ,{ 2788, 1771 }
        ,{ 2793, 1617 }
        ,{ 2812, 1780 }
        ,{ 2842, 1605 }
        ,{ 2852, 1788 }
        ,{ 2871, 1734 }
        ,{ 2875, 149 }
        ,{ 2890, 1434 }
        ,{ 2907, 1745 }
        ,{ 2975, 1687 }
        ,{ 3025, 261 }
        ,{ 3038, 1597 }
        ,{ 3069, 1726 }
        ,{ 3075, 1643 }
        ,{ 3116, 1770 }
        ,{ 3146, 1561 }
        ,{ 3174, 1303 }
        ,{ 3185, 1625 }
        ,{ 3249, 247 }
        ,{ 3325, 1682 }
        ,{ 3364, 233 }
        ,{ 3381, 1611 }
        ,{ 3393, 1733 }
        ,{ 3404, 1779 }
        ,{ 3519, 1739 }
        ,{ 3549, 1497 }
        ,{ 3575, 1690 }
        ,{ 3596, 1787 }
        ,{ 3610, 1368 }
        ,{ 3625, 148 }
        ,{ 3626, 1588 }
        ,{ 3627, 1725 }
        ,{ 3663, 1717 }
        ,{ 3718, 1495 }
        ,{ 3772, 1769 }
        ,{ 3773, 141 }
        ,{ 3844, 224 }
        ,{ 3875, 147 }
        ,{ 3933, 1738 }
        ,{ 3993, 131 }
        ,{ 4018, 1578 }
        ,{ 4025, 1676 }
        ,{ 4046, 1432 }
        ,{ 4059, 1707 }
        ,{ 4114, 1557 }
        ,{ 4165, 1621 }
        ,{ 4225, 257 }
        ,{ 4235, 1562 }
        ,{ 4263, 1604 }
        ,{ 4292, 1778 }
        ,{ 4329, 1716 }
        ,{ 4335, 1433 }
        ,{ 4394, 120 }
        ,{ 4437, 1732 }
        ,{ 4459, 140 }
        ,{ 4557, 1596 }
        ,{ 4588, 1777 }
        ,{ 4598, 1552 }
        ,{ 4625, 146 }
        ,{ 4655, 1616 }
        ,{ 4675, 1686 }
        ,{ 4719, 1560 }
        ,{ 4743, 1724 }
        ,{ 4756, 1768 }
        ,{ 4761, 240 }
        ,{ 4797, 1706 }
        ,{ 4959, 1731 }
        ,{ 5046, 1237 }
        ,{ 5054, 1366 }
        ,{ 5075, 1669 }
        ,{ 5084, 1767 }
        ,{ 5125, 145 }
        ,{ 5225, 1681 }
        ,{ 5290, 1302 }
        ,{ 5301, 1723 }
        ,{ 5415, 1367 }
        ,{ 5425, 1661 }
        ,{ 5439, 1587 }
        ,{ 5476, 214 }
        ,{ 5525, 1685 }
        ,{ 5566, 1546 }
        ,{ 5577, 1494 }
        ,{ 5635, 1610 }
        ,{ 5661, 1715 }
        ,{ 5746, 1491 }
        ,{ 5766, 1171 }
        ,{ 5831, 139 }
        ,{ 5915, 1496 }
        ,{ 5929, 260 }
        ,{ 6003, 1730 }
        ,{ 6027, 1577 }
        ,{ 6068, 1766 }
        ,{ 6069, 1431 }
        ,{ 6171, 1556 }
        ,{ 6175, 1680 }
        ,{ 6273, 1705 }
        ,{ 6325, 1675 }
        ,{ 6327, 1714 }
        ,{ 6358, 1429 }
        ,{ 6417, 1722 }
        ,{ 6422, 1486 }
        ,{ 6475, 1652 }
        ,{ 6517, 138 }
        ,{ 6591, 119 }
        ,{ 6655, 130 }
        ,{ 6724, 203 }
        ,{ 6897, 1551 }
        ,{ 7007, 1624 }
        ,{ 7011, 1704 }
        ,{ 7018, 1539 }
        ,{ 7105, 1603 }
        ,{ 7175, 1642 }
        ,{ 7225, 252 }
        ,{ 7406, 1300 }
        ,{ 7475, 1674 }
        ,{ 7502, 1531 }
        ,{ 7514, 1425 }
        ,{ 7569, 232 }
        ,{ 7581, 1365 }
        ,{ 7595, 1595 }
        ,{ 7659, 1713 }
        ,{ 7774, 1480 }
        ,{ 7865, 1559 }
        ,{ 7889, 137 }
        ,{ 7935, 1301 }
        ,{ 7942, 1363 }
        ,{ 7975, 1668 }
        ,{ 8075, 1679 }
        ,{ 8091, 1721 }
        ,{ 8214, 1105 }
        ,{ 8281, 256 }
        ,{ 8349, 1545 }
        ,{ 8410, 1236 }
        ,{ 8487, 1703 }
        ,{ 8525, 1660 }
        ,{ 8619, 1490 }
        ,{ 8649, 223 }
        ,{ 8954, 1522 }
        ,{ 9025, 246 }
        ,{ 9065, 1586 }
        ,{ 9163, 1620 }
        ,{ 9295, 1493 }
        ,{ 9317, 129 }
        ,{ 9386, 1359 }
        ,{ 9425, 1667 }
        ,{ 9537, 1428 }
        ,{ 9610, 1170 }
        ,{ 9633, 1485 }
        ,{ 9657, 1712 }
        ,{ 9775, 1673 }
        ,{ 9802, 1473 }
        ,{ 9826, 108 }
        ,{ 9922, 1512 }
        ,{ 9947, 136 }
        ,{ 10045, 1576 }
        ,{ 10075, 1659 }
        ,{ 10086, 1039 }
        ,{ 10115, 1430 }
        ,{ 10175, 1651 }
        ,{ 10241, 1615 }
        ,{ 10285, 1555 }
        ,{ 10323, 1711 }
        ,{ 10478, 1465 }
        ,{ 10527, 1538 }
        ,{ 10633, 135 }
        ,{ 10701, 1702 }
        ,{ 10829, 1619 }
        ,{ 10925, 1672 }
        ,{ 10982, 1420 }
        ,{ 10985, 118 }
        ,{ 11011, 1558 }
        ,{ 11109, 1299 }
        ,{ 11253, 1530 }
        ,{ 11271, 1424 }
        ,{ 11275, 1641 }
        ,{ 11439, 1701 }
        ,{ 11495, 1550 }
        ,{ 11638, 1297 }
        ,{ 11661, 1479 }
        ,{ 11774, 1234 }
        ,{ 11913, 1362 }
        ,{ 12025, 1650 }
        ,{ 12103, 1614 }
        ,{ 12274, 1354 }
        ,{ 12321, 213 }
        ,{ 12325, 1666 }
        ,{ 12397, 1609 }
        ,{ 12506, 1456 }
        ,{ 12615, 1235 }
        ,{ 12635, 1364 }
        ,{ 12691, 134 }
        ,{ 13013, 1492 }
        ,{ 13175, 1658 }
        ,{ 13225, 239 }
        ,{ 13294, 1414 }
        ,{ 13325, 1640 }
        ,{ 13431, 1521 }
        ,{ 13454, 1168 }
        ,{ 13653, 1700 }
        ,{ 13690, 1104 }
        ,{ 13718, 96 }
        ,{ 13754, 1293 }
        ,{ 13775, 1665 }
        ,{ 13858, 1446 }
        ,{ 13915, 1544 }
        ,{ 14063, 133 }
        ,{ 14079, 1358 }
        ,{ 14161, 251 }
        ,{ 14365, 1489 }
        ,{ 14399, 1554 }
        ,{ 14415, 1169 }
        ,{ 14641, 9 }
        ,{ 14651, 1608 }
        ,{ 14703, 1472 }
        ,{ 14725, 1657 }
        ,{ 14739, 107 }
        ,{ 14883, 1511 }
        ,{ 15129, 202 }
        ,{ 15379, 117 }
        ,{ 15631, 1602 }
        ,{ 15717, 1464 }
        ,{ 15725, 1649 }
        ,{ 15827, 1613 }
        ,{ 15895, 1427 }
        ,{ 16055, 1484 }
        ,{ 16093, 1549 }
        ,{ 16473, 1419 }
        ,{ 16606, 1348 }
        ,{ 16675, 1664 }
        ,{ 16709, 1594 }
        ,{ 16762, 1407 }
        ,{ 16810, 1038 }
        ,{ 17303, 128 }
        ,{ 17425, 1639 }
        ,{ 17457, 1296 }
        ,{ 17545, 1537 }
        ,{ 17575, 1648 }
        ,{ 17661, 1233 }
        ,{ 17689, 245 }
        ,{ 17825, 1656 }
        ,{ 17918, 1399 }
        ,{ 17986, 1288 }
        ,{ 18411, 1353 }
        ,{ 18473, 1601 }
        ,{ 18502, 1231 }
        ,{ 18515, 1298 }
        ,{ 18755, 1529 }
        ,{ 18759, 1455 }
        ,{ 18785, 1423 }
        ,{ 19159, 1607 }
        ,{ 19166, 1102 }
        ,{ 19435, 1478 }
        ,{ 19475, 1638 }
        ,{ 19481, 1543 }
        ,{ 19747, 1593 }
        ,{ 19855, 1361 }
        ,{ 19941, 1413 }
        ,{ 19943, 1585 }
        ,{ 20102, 1282 }
        ,{ 20111, 1488 }
        ,{ 20181, 1167 }
        ,{ 20449, 255 }
        ,{ 20535, 1103 }
        ,{ 20577, 95 }
        ,{ 20631, 1292 }
        ,{ 20787, 1445 }
        ,{ 20938, 1341 }
        ,{ 21025, 231 }
        ,{ 21142, 1165 }
        ,{ 21275, 1647 }
        ,{ 21386, 1390 }
        ,{ 21413, 1606 }
        ,{ 21866, 1227 }
        ,{ 22099, 1575 }
        ,{ 22253, 1426 }
        ,{ 22382, 1333 }
        ,{ 22385, 1520 }
        ,{ 22475, 1655 }
        ,{ 22477, 1483 }
        ,{ 22627, 127 }
        ,{ 23465, 1357 }
        ,{ 23534, 1036 }
        ,{ 23569, 1584 }
        ,{ 23575, 1637 }
        ,{ 23698, 1380 }
        ,{ 24025, 222 }
        ,{ 24157, 1600 }
        ,{ 24167, 116 }
        ,{ 24334, 84 }
        ,{ 24505, 1471 }
        ,{ 24563, 1536 }
        ,{ 24565, 106 }
        ,{ 24805, 1510 }
        ,{ 24909, 1347 }
        ,{ 24986, 1161 }
        ,{ 25143, 1406 }
        ,{ 25215, 1037 }
        ,{ 25289, 126 }
        ,{ 25823, 1592 }
        ,{ 25921, 238 }
        ,{ 26117, 1574 }
        ,{ 26195, 1463 }
        ,{ 26257, 1528 }
        ,{ 26299, 1422 }
        ,{ 26714, 1324 }
        ,{ 26741, 1553 }
        ,{ 26825, 1646 }
        ,{ 26877, 1398 }
        ,{ 26979, 1287 }
        ,{ 26999, 1599 }
        ,{ 27209, 1477 }
        ,{ 27455, 1418 }
        ,{ 27753, 1230 }
        ,{ 27797, 1360 }
        ,{ 28561, 8 }
        ,{ 28594, 1222 }
        ,{ 28675, 1645 }
        ,{ 28749, 1101 }
        ,{ 28861, 1591 }
        ,{ 29095, 1295 }
        ,{ 29435, 1232 }
        ,{ 29602, 1314 }
        ,{ 29725, 1636 }
        ,{ 29887, 1548 }
        ,{ 30118, 1099 }
        ,{ 30153, 1281 }
        ,{ 30613, 125 }
        ,{ 30682, 1275 }
        ,{ 30685, 1352 }
        ,{ 30821, 1583 }
        ,{ 31265, 1454 }
        ,{ 31339, 1519 }
        ,{ 31407, 1340 }
        ,{ 31603, 1487 }
        ,{ 31713, 1164 }
        ,{ 31775, 1635 }
        ,{ 31958, 1216 }
        ,{ 32079, 1389 }
        ,{ 32674, 1156 }
        ,{ 32683, 1598 }
        ,{ 32798, 1267 }
        ,{ 32799, 1226 }
        ,{ 32851, 1356 }
        ,{ 33235, 1412 }
        ,{ 33573, 1332 }
        ,{ 33635, 1166 }
        ,{ 34153, 1573 }
        ,{ 34225, 212 }
        ,{ 34295, 94 }
        ,{ 34307, 1470 }
        ,{ 34385, 1291 }
        ,{ 34391, 105 }
        ,{ 34447, 1582 }
        ,{ 34645, 1444 }
        ,{ 34727, 1509 }
        ,{ 34937, 1590 }
        ,{ 34969, 250 }
        ,{ 35301, 1035 }
        ,{ 35321, 1482 }
        ,{ 35547, 1379 }
        ,{ 35594, 1095 }
        ,{ 36179, 1542 }
        ,{ 36501, 83 }
        ,{ 36518, 1150 }
        ,{ 36673, 1462 }
        ,{ 36982, 1033 }
        ,{ 37349, 115 }
        ,{ 37479, 1160 }
        ,{ 37925, 1634 }
        ,{ 38171, 1572 }
        ,{ 38437, 1417 }
        ,{ 38599, 124 }
        ,{ 38686, 1209 }
        ,{ 39083, 1547 }
        ,{ 39146, 1258 }
        ,{ 40071, 1323 }
        ,{ 40733, 1294 }
        ,{ 41209, 230 }
        ,{ 41261, 123 }
        ,{ 41327, 1421 }
        ,{ 41515, 1346 }
        ,{ 41699, 1581 }
        ,{ 41743, 114 }
        ,{ 41905, 1405 }
        ,{ 42025, 201 }
        ,{ 42757, 1476 }
        ,{ 42891, 1221 }
        ,{ 42959, 1351 }
        ,{ 43378, 1248 }
        ,{ 43681, 244 }
        ,{ 43706, 1029 }
        ,{ 43771, 1453 }
        ,{ 44051, 1589 }
        ,{ 44206, 1143 }
        ,{ 44403, 1313 }
        ,{ 44795, 1397 }
        ,{ 44965, 1286 }
        ,{ 45177, 1098 }
        ,{ 45617, 1535 }
        ,{ 46023, 1274 }
        ,{ 46207, 1571 }
        ,{ 46255, 1229 }
        ,{ 46529, 1411 }
        ,{ 46546, 1090 }
        ,{ 47089, 221 }
        ,{ 47311, 1541 }
        ,{ 47915, 1100 }
        ,{ 47937, 1215 }
        ,{ 48013, 93 }
        ,{ 48139, 1290 }
        ,{ 48503, 1443 }
        ,{ 48763, 1527 }
        ,{ 48778, 72 }
        ,{ 48841, 249 }
        ,{ 49011, 1155 }
        ,{ 49197, 1266 }
        ,{ 49247, 122 }
        ,{ 50255, 1280 }
        ,{ 50531, 113 }
        ,{ 51623, 1355 }
        ,{ 52022, 1084 }
        ,{ 52142, 1201 }
        ,{ 52345, 1339 }
        ,{ 52577, 1580 }
        ,{ 52855, 1163 }
        ,{ 52877, 1540 }
        ,{ 53391, 1094 }
        ,{ 53465, 1388 }
        ,{ 53911, 1469 }
        ,{ 54043, 104 }
        ,{ 54571, 121 }
        ,{ 54587, 1481 }
        ,{ 54665, 1225 }
        ,{ 54777, 1149 }
        ,{ 55473, 1032 }
        ,{ 55738, 1135 }
        ,{ 55955, 1331 }
        ,{ 56203, 1579 }
        ,{ 57154, 1024 }
        ,{ 57629, 1461 }
        ,{ 58029, 1208 }
        ,{ 58121, 1345 }
        ,{ 58201, 1518 }
        ,{ 58261, 1570 }
        ,{ 58667, 1404 }
        ,{ 58719, 1257 }
        ,{ 58835, 1034 }
        ,{ 59245, 1378 }
        ,{ 59582, 60 }
        ,{ 59653, 1534 }
        ,{ 60401, 1416 }
        ,{ 60835, 82 }
        ,{ 61009, 243 }
        ,{ 62234, 1192 }
        ,{ 62279, 1569 }
        ,{ 62465, 1159 }
        ,{ 62713, 1396 }
        ,{ 62951, 1285 }
        ,{ 62974, 1077 }
        ,{ 63713, 112 }
        ,{ 63767, 1526 }
        ,{ 63869, 103 }
        ,{ 63878, 1018 }
        ,{ 64009, 237 }
        ,{ 64493, 1508 }
        ,{ 64757, 1228 }
        ,{ 65067, 1247 }
        ,{ 65559, 1028 }
        ,{ 66079, 1475 }
        ,{ 66309, 1142 }
        ,{ 66671, 1533 }
        ,{ 66785, 1322 }
        ,{ 67081, 211 }
        ,{ 67507, 1350 }
        ,{ 68107, 111 }
        ,{ 68783, 1452 }
        ,{ 68962, 1182 }
        ,{ 69819, 1089 }
        ,{ 70357, 1279 }
        ,{ 71114, 1126 }
        ,{ 71269, 1525 }
        ,{ 71383, 1415 }
        ,{ 71485, 1220 }
        ,{ 73117, 1410 }
        ,{ 73167, 71 }
        ,{ 73283, 1338 }
        ,{ 73853, 1474 }
        ,{ 73997, 1162 }
        ,{ 74005, 1312 }
        ,{ 74333, 1568 }
        ,{ 74851, 1387 }
        ,{ 75295, 1097 }
        ,{ 75449, 92 }
        ,{ 75647, 1289 }
        ,{ 76109, 1517 }
        ,{ 76219, 1442 }
        ,{ 76531, 1224 }
        ,{ 76705, 1273 }
        ,{ 77326, 1011 }
        ,{ 78033, 1083 }
        ,{ 78213, 1200 }
        ,{ 78337, 1330 }
        ,{ 78802, 1116 }
        ,{ 79402, 1069 }
        ,{ 79781, 1349 }
        ,{ 79895, 1214 }
        ,{ 80707, 1532 }
        ,{ 81289, 110 }
        ,{ 81685, 1154 }
        ,{ 81995, 1265 }
        ,{ 82369, 200 }
        ,{ 82943, 1377 }
        ,{ 83317, 1468 }
        ,{ 83521, 7 }
        ,{ 83607, 1134 }
        ,{ 84337, 1507 }
        ,{ 84878, 1060 }
        ,{ 85063, 1516 }
        ,{ 85169, 81 }
        ,{ 85731, 1023 }
        ,{ 86273, 1524 }
        ,{ 86411, 1409 }
        ,{ 87451, 1158 }
        ,{ 88985, 1093 }
        ,{ 89063, 1460 }
        ,{ 89167, 91 }
        ,{ 89373, 59 }
        ,{ 89401, 236 }
        ,{ 90077, 109 }
        ,{ 91295, 1148 }
        ,{ 91333, 1344 }
        ,{ 92191, 1403 }
        ,{ 92455, 1031 }
        ,{ 93119, 1467 }
        ,{ 93347, 102 }
        ,{ 93351, 1191 }
        ,{ 93499, 1321 }
        ,{ 94259, 1506 }
        ,{ 94461, 1076 }
        ,{ 95817, 1017 }
        ,{ 96715, 1207 }
        ,{ 97498, 1003 }
        ,{ 97865, 1256 }
        ,{ 98549, 1395 }
        ,{ 98923, 1284 }
        ,{ 99541, 1459 }
        ,{ 100079, 1219 }
        ,{ 101306, 48 }
        ,{ 101761, 229 }
        ,{ 102971, 1515 }
        ,{ 103443, 1181 }
        ,{ 103607, 1311 }
        ,{ 104222, 994 }
        ,{ 104329, 242 }
        ,{ 105413, 1096 }
        ,{ 106301, 1451 }
        ,{ 106671, 1125 }
        ,{ 107387, 1272 }
        ,{ 107939, 1343 }
        ,{ 108445, 1246 }
        ,{ 108779, 1523 }
        ,{ 108953, 1402 }
        ,{ 109265, 1027 }
        ,{ 110515, 1141 }
        ,{ 110561, 1278 }
        ,{ 111853, 1213 }
        ,{ 112258, 1050 }
        ,{ 112723, 1466 }
        ,{ 112999, 101 }
        ,{ 114103, 1505 }
        ,{ 114359, 1153 }
        ,{ 114793, 1264 }
        ,{ 115159, 1337 }
        ,{ 115989, 1010 }
        ,{ 116281, 220 }
        ,{ 116365, 1088 }
        ,{ 116467, 1394 }
        ,{ 116603, 90 }
        ,{ 116909, 1283 }
        ,{ 117623, 1386 }
        ,{ 117793, 1441 }
        ,{ 118203, 1115 }
        ,{ 118807, 1450 }
        ,{ 119103, 1068 }
        ,{ 120263, 1223 }
        ,{ 120497, 1458 }
        ,{ 121945, 70 }
        ,{ 123101, 1329 }
        ,{ 124394, 984 }
        ,{ 124579, 1092 }
        ,{ 126293, 1408 }
        ,{ 127317, 1059 }
        ,{ 127813, 1147 }
        ,{ 129437, 1030 }
        ,{ 129833, 1514 }
        ,{ 130055, 1082 }
        ,{ 130321, 6 }
        ,{ 130339, 1376 }
        ,{ 130355, 1199 }
        ,{ 130663, 1277 }
        ,{ 131651, 1440 }
        ,{ 133837, 80 }
        ,{ 135401, 1206 }
        ,{ 136097, 1336 }
        ,{ 137011, 1255 }
        ,{ 137423, 1157 }
        ,{ 137842, 36 }
        ,{ 138787, 1513 }
        ,{ 139009, 1385 }
        ,{ 139345, 1133 }
        ,{ 141151, 1342 }
        ,{ 142129, 228 }
        ,{ 142477, 100 }
        ,{ 142885, 1022 }
        ,{ 143819, 1449 }
        ,{ 143869, 1504 }
        ,{ 145483, 1328 }
        ,{ 146247, 1002 }
        ,{ 146927, 1320 }
        ,{ 148955, 58 }
        ,{ 151823, 1245 }
        ,{ 151931, 1457 }
        ,{ 151959, 47 }
        ,{ 152303, 99 }
        ,{ 152881, 235 }
        ,{ 152971, 1026 }
        ,{ 153791, 1503 }
        ,{ 154037, 1375 }
        ,{ 154721, 1140 }
        ,{ 155585, 1190 }
        ,{ 156333, 993 }
        ,{ 157267, 1218 }
        ,{ 157435, 1075 }
        ,{ 157757, 89 }
        ,{ 158171, 79 }
        ,{ 159239, 1401 }
        ,{ 159367, 1439 }
        ,{ 159695, 1016 }
        ,{ 162409, 219 }
        ,{ 162811, 1310 }
        ,{ 162911, 1087 }
        ,{ 165649, 210 }
        ,{ 168387, 1049 }
        ,{ 168751, 1271 }
        ,{ 170221, 1393 }
        ,{ 170723, 69 }
        ,{ 170867, 1276 }
        ,{ 172405, 1180 }
        ,{ 173641, 1319 }
        ,{ 175769, 1212 }
        ,{ 177785, 1124 }
        ,{ 177973, 1335 }
        ,{ 179707, 1152 }
        ,{ 180389, 1263 }
        ,{ 181337, 1448 }
        ,{ 181781, 98 }
        ,{ 182077, 1081 }
        ,{ 182497, 1198 }
        ,{ 183557, 1502 }
        ,{ 185861, 1217 }
        ,{ 186591, 983 }
        ,{ 190247, 1327 }
        ,{ 190969, 234 }
        ,{ 192413, 1309 }
        ,{ 192763, 1400 }
        ,{ 193315, 1009 }
        ,{ 193843, 1447 }
        ,{ 195083, 1132 }
        ,{ 195767, 1091 }
        ,{ 197005, 1114 }
        ,{ 198505, 1067 }
        ,{ 198911, 88 }
        ,{ 199433, 1270 }
        ,{ 200039, 1021 }
        ,{ 200849, 1146 }
        ,{ 200941, 1438 }
        ,{ 201433, 97 }
        ,{ 203167, 1384 }
        ,{ 203401, 199 }
        ,{ 206057, 1392 }
        ,{ 206763, 35 }
        ,{ 206839, 78 }
        ,{ 207727, 1211 }
        ,{ 208537, 57 }
        ,{ 212195, 1058 }
        ,{ 212381, 1151 }
        ,{ 212629, 87 }
        ,{ 212773, 1205 }
        ,{ 213187, 1262 }
        ,{ 214799, 1437 }
        ,{ 215303, 1254 }
        ,{ 217819, 1189 }
        ,{ 220409, 1074 }
        ,{ 223573, 1015 }
        ,{ 225131, 1374 }
        ,{ 227069, 1318 }
        ,{ 231173, 77 }
        ,{ 231361, 209 }
        ,{ 237367, 1145 }
        ,{ 238579, 1244 }
        ,{ 240383, 1025 }
        ,{ 240787, 1334 }
        ,{ 241367, 1179 }
        ,{ 243049, 227 }
        ,{ 243133, 1139 }
        ,{ 243745, 1001 }
        ,{ 245939, 1383 }
        ,{ 248899, 1123 }
        ,{ 251459, 1204 }
        ,{ 251617, 1308 }
        ,{ 253265, 46 }
        ,{ 253783, 86 }
        ,{ 254449, 1253 }
        ,{ 256003, 1086 }
        ,{ 256373, 1436 }
        ,{ 257393, 1326 }
        ,{ 259811, 1391 }
        ,{ 260555, 992 }
        ,{ 260797, 1269 }
        ,{ 268279, 68 }
        ,{ 270641, 1008 }
        ,{ 271643, 1210 }
        ,{ 272527, 1373 }
        ,{ 275807, 1113 }
        ,{ 277729, 218 }
        ,{ 277907, 1066 }
        ,{ 278783, 1261 }
        ,{ 279841, 5 }
        ,{ 280645, 1048 }
        ,{ 281219, 85 }
        ,{ 281957, 1243 }
        ,{ 284089, 198 }
        ,{ 286121, 1080 }
        ,{ 286781, 1197 }
        ,{ 287339, 1138 }
        ,{ 291479, 1268 }
        ,{ 297073, 1057 }
        ,{ 302549, 1085 }
        ,{ 303601, 226 }
        ,{ 306559, 1131 }
        ,{ 307211, 1317 }
        ,{ 310097, 1382 }
        ,{ 310403, 1144 }
        ,{ 310985, 982 }
        ,{ 311581, 1260 }
        ,{ 314347, 1020 }
        ,{ 317057, 67 }
        ,{ 324539, 1325 }
        ,{ 327701, 56 }
        ,{ 328831, 1203 }
        ,{ 331483, 1381 }
        ,{ 332741, 1252 }
        ,{ 338143, 1079 }
        ,{ 338923, 1196 }
        ,{ 340423, 1307 }
        ,{ 341243, 1000 }
        ,{ 342287, 1188 }
        ,{ 343621, 1372 }
        ,{ 344605, 34 }
        ,{ 346357, 1073 }
        ,{ 346921, 217 }
        ,{ 351329, 1014 }
        ,{ 352843, 76 }
        ,{ 354571, 45 }
        ,{ 362297, 1130 }
        ,{ 364777, 991 }
        ,{ 367319, 1371 }
        ,{ 367517, 1202 }
        ,{ 368713, 1242 }
        ,{ 371501, 1019 }
        ,{ 371887, 1251 }
        ,{ 375751, 1137 }
        ,{ 377177, 75 }
        ,{ 379291, 1178 }
        ,{ 387283, 55 }
        ,{ 387353, 1316 }
        ,{ 391127, 1122 }
        ,{ 392903, 1047 }
        ,{ 395641, 208 }
        ,{ 404521, 1187 }
        ,{ 409331, 1072 }
        ,{ 412091, 1241 }
        ,{ 414067, 1315 }
        ,{ 414613, 66 }
        ,{ 415207, 1013 }
        ,{ 419957, 1136 }
        ,{ 425293, 1007 }
        ,{ 429229, 1306 }
        ,{ 433411, 1112 }
        ,{ 435379, 981 }
        ,{ 436711, 1065 }
        ,{ 438413, 1370 }
        ,{ 442187, 1078 }
        ,{ 443207, 1195 }
        ,{ 444889, 225 }
        ,{ 448253, 1177 }
        ,{ 450179, 74 }
        ,{ 458831, 1305 }
        ,{ 462241, 1121 }
        ,{ 463391, 65 }
        ,{ 466829, 1056 }
        ,{ 473773, 1129 }
        ,{ 475571, 1259 }
        ,{ 482447, 33 }
        ,{ 485809, 197 }
        ,{ 494209, 207 }
        ,{ 495349, 1194 }
        ,{ 498847, 73 }
        ,{ 502619, 1006 }
        ,{ 506447, 54 }
        ,{ 508369, 216 }
        ,{ 512213, 1111 }
        ,{ 516113, 1064 }
        ,{ 528989, 1186 }
        ,{ 529511, 1128 }
        ,{ 535279, 1071 }
        ,{ 536239, 999 }
        ,{ 542963, 1012 }
        ,{ 547637, 1304 }
        ,{ 551707, 1055 }
        ,{ 557183, 44 }
        ,{ 560947, 64 }
        ,{ 566029, 53 }
        ,{ 567617, 1250 }
        ,{ 573221, 990 }
        ,{ 586177, 1176 }
        ,{ 591223, 1185 }
        ,{ 598253, 1070 }
        ,{ 599633, 1193 }
        ,{ 604469, 1120 }
        ,{ 606763, 1249 }
        ,{ 606841, 196 }
        ,{ 617419, 1046 }
        ,{ 628981, 1240 }
        ,{ 633737, 998 }
        ,{ 640987, 1127 }
        ,{ 655139, 1175 }
        ,{ 657271, 1005 }
        ,{ 658489, 43 }
        ,{ 669817, 1110 }
        ,{ 672359, 1239 }
        ,{ 674917, 1063 }
        ,{ 675583, 1119 }
        ,{ 677443, 989 }
        ,{ 684167, 980 }
        ,{ 685193, 52 }
        ,{ 707281, 4 }
        ,{ 715691, 1184 }
        ,{ 721463, 1054 }
        ,{ 724201, 206 }
        ,{ 729677, 1045 }
        ,{ 734597, 1004 }
        ,{ 748619, 1109 }
        ,{ 754319, 1062 }
        ,{ 756059, 63 }
        ,{ 758131, 32 }
        ,{ 793063, 1174 }
        ,{ 802493, 1238 }
        ,{ 806341, 1053 }
        ,{ 808201, 215 }
        ,{ 808561, 979 }
        ,{ 817811, 1118 }
        ,{ 828733, 997 }
        ,{ 861101, 42 }
        ,{ 863939, 51 }
        ,{ 885887, 988 }
        ,{ 889249, 195 }
        ,{ 895973, 31 }
        ,{ 902393, 62 }
        ,{ 906223, 1108 }
        ,{ 913123, 1061 }
        ,{ 923521, 3 }
        ,{ 926231, 996 }
        ,{ 954193, 1044 }
        ,{ 962407, 41 }
        ,{ 964627, 1183 }
        ,{ 976097, 1052 }
        ,{ 990109, 987 }
        ,{ 999949, 61 }
        ,{ 1031153, 1117 }
        ,{ 1057349, 978 }
        ,{ 1066451, 1043 }
        ,{ 1068911, 1173 }
        ,{ 1102267, 50 }
        ,{ 1121227, 995 }
        ,{ 1142629, 1107 }
        ,{ 1151329, 205 }
        ,{ 1165019, 40 }
        ,{ 1171657, 30 }
        ,{ 1181743, 977 }
        ,{ 1198553, 986 }
        ,{ 1221431, 49 }
        ,{ 1230731, 1051 }
        ,{ 1275797, 1172 }
        ,{ 1290967, 1042 }
        ,{ 1309499, 29 }
        ,{ 1315609, 204 }
        ,{ 1413721, 194 }
        ,{ 1430531, 976 }
        ,{ 1457837, 1106 }
        ,{ 1468937, 39 }
        ,{ 1511219, 985 }
        ,{ 1570243, 38 }
        ,{ 1585183, 28 }
        ,{ 1615441, 193 }
        ,{ 1627741, 1041 }
        ,{ 1739999, 1040 }
        ,{ 1803713, 975 }
        ,{ 1874161, 2 }
        ,{ 1928107, 974 }
        ,{ 1998709, 27 }
        ,{ 2076773, 37 }
        ,{ 2136551, 26 }
        ,{ 2301289, 192 }
        ,{ 2550077, 25 }
        ,{ 2825761, 1 }
    };

    uint32_t evaluate4CardHand( const std::vector< std::shared_ptr< CCard > >& cards, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        if ( cards.size() != 4 )
            return -1;

        auto cardsValue = getCardsValue( cards );
        if ( playInfo && playInfo->fStraightsFlushesCount )
        {
            if ( isFlush( cards ) )
                return sFlushes[ cardsValue ];
        }

        auto straightOrHighCard = playInfo->fStraightsFlushesCount ? sStraightsUnique[ cardsValue ] : sHighCardUnique[ cardsValue ];
        if ( straightOrHighCard )
            return straightOrHighCard;

        auto product = computeHandProduct( cards );
        auto productMap = playInfo->fStraightsFlushesCount ? sStraitsAndFlushesProductMap : sProductMap;
        auto pos = productMap.find( product );
        if ( pos == productMap.end() )
            return -1;
        return ( *pos ).second;
    }

    EHand rankTo4CardHand( uint32_t rank, const std::shared_ptr< SPlayInfo >& playInfo )
    {
        EHand hand;
        if ( !playInfo->fStraightsFlushesCount )
        {
            if ( rank >= 1106U )
                hand = EHand::eHighCard;
            else if ( rank >= 248U )
                hand = EHand::ePair;
            else if ( rank >= 170U )
                hand = EHand::eTwoPair;
            else if ( rank >= 14U )
                hand = EHand::eThreeOfAKind;
            else /* if ( rank >= 1U ) */
                hand = EHand::eFourOfAKind;
        }
        else
        {
            if ( rank >= 1832U )
                hand = EHand::eHighCard;
            else if ( rank >= 974U )
                hand = EHand::ePair;
            else if ( rank >= 270U )
                hand = EHand::eFlush;
            else if ( rank >= 192U )
                hand = EHand::eTwoPair;
            else if ( rank >= 181U )
                hand = EHand::eStraight;
            else if ( rank >= 25U )
                hand = EHand::eThreeOfAKind;
            else if ( rank >= 14U )
                hand = EHand::eStraightFlush;
            else /* if ( rank >= 1U ) */
                hand = EHand::eFourOfAKind;
        }
        return hand;
    }

    C4CardInfo::C4CardInfo() :
        C4CardInfo( THand( TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ), TCard( ECard::eUNKNOWN, ESuit::eUNKNOWN ) ) )
    {
    }

    C4CardInfo::C4CardInfo( ECard c1, ESuit s1, ECard c2, ESuit s2, ECard c3, ESuit s3, ECard c4, ESuit s4 ) :
        C4CardInfo( THand( TCard( c1, s1 ), TCard( c2, s2 ), TCard( c3, s3 ), TCard( c4, s4 ) ) )
    {
    }

    C4CardInfo::C4CardInfo( const THand& cards )
    {
        fHandOrder = 
        { 
            EHand::eFourOfAKind, 
            EHand::eStraightFlush, 
            EHand::eThreeOfAKind, 
            EHand::eStraight, 
            EHand::eTwoPair, 
            EHand::eFlush, 
            EHand::ePair, 
            EHand::eHighCard 
        };

        setOrigCards( { std::get< 0 >( cards ), std::get< 1 >( cards ), std::get< 2 >( cards ), std::get< 3 >( cards ) } );

        fIsFlush = NHandUtils::isFlush( fOrigCards );
        fIsPair = NHandUtils::isCount( fOrigCards, 2 );
        fIsTwoPair = NHandUtils::isCount( fOrigCards, { 2, 2 } );
        fIsThreeOfAKind = NHandUtils::isCount( fOrigCards, 3 );
        fIsFourOfAKind = NHandUtils::isCount( fOrigCards, 4 );
        fStraightType = NHandUtils::isStraight( fOrigCards );
    }

    void generateAll4CardHands()
    {
        static bool sAllHandsComputed{ false };
        if ( NHandUtils::gComputeAllHands && !sAllHandsComputed )
        {
            sAllHandsComputed = true;

            using TCardToInfoMap = std::map< C4CardInfo::THand, C4CardInfo >;
            auto gFlushStraightsCount = []( const C4CardInfo& lhs, const C4CardInfo& rhs ) { return lhs.greaterThan( true, rhs ); };
            auto gJustCardsCount = []( const C4CardInfo& lhs, const C4CardInfo& rhs ) { return lhs.greaterThan( false, rhs ); };

            using TCardsCountMap = std::map< C4CardInfo, uint32_t, decltype( gJustCardsCount ) >;
            using TFlushesCountMap = std::map< C4CardInfo, uint32_t, decltype( gFlushStraightsCount ) >;

            static TCardToInfoMap allHands;
            static TCardsCountMap justCardsCount( gJustCardsCount );
            static TFlushesCountMap flushesAndStraightsCount( gFlushStraightsCount );

            auto numHands = 52 * 51 * 50 * 49;
            std::cout << "Generating: " << numHands << "\n";

            size_t maxCardsValue = 0;

            auto&& allCards = CCard::allCardsList();
            uint64_t handCount = 0;

            for ( auto&& c1 : allCards )
            {
                for ( auto&& c2 : allCards )
                {
                    if ( *c1 == *c2 )
                        continue;

                    for ( auto&& c3 : allCards )
                    {
                        if ( *c1 == *c3 )
                            continue;
                        if ( *c2 == *c3 )
                            continue;

                        for ( auto&& c4 : allCards )
                        {
                            if ( *c1 == *c4 )
                                continue;
                            if ( *c2 == *c4 )
                                continue;
                            if ( *c3 == *c4 )
                                continue;

                            handCount++;
                            if ( ( handCount % ( numHands / 10 ) ) == 0 )
                                std::cout << "   Generating: Hand #" << handCount << " of " << numHands << "\n";

                            auto curr = C4CardInfo::THand( TCard( c1->getCard(), c1->getSuit() ), TCard( c2->getCard(), c2->getSuit() ), TCard( c3->getCard(), c3->getSuit() ), TCard( c4->getCard(), c4->getSuit() ) );
                            C4CardInfo cardInfo( curr );
                            allHands[ curr ] = cardInfo;

                            justCardsCount.insert( std::make_pair( cardInfo, -1 ) );
                            flushesAndStraightsCount.insert( std::make_pair( cardInfo, -1 ) );

                            maxCardsValue = std::max( static_cast<size_t>( cardInfo.getCardsValue() ), maxCardsValue );
                        }
                    }
                }
            }

            std::cout << "Finished Generating: " << numHands << "\n";
            std::ofstream ofs( "E:/DropBox/Documents/sb/github/scottaronbloom/CardGame/Cards/4CardDump.cpp" );
            std::ostream* oss = &ofs; //&std::cout;

            CCardInfo::computeAndGenerateMaps( *oss, 4, justCardsCount, flushesAndStraightsCount );

            std::vector< uint32_t > flushes;
            flushes.resize( maxCardsValue + 1 );

            std::vector< uint32_t > highCardUnique;
            highCardUnique.resize( maxCardsValue + 1 );

            std::vector< uint32_t > straightsUnique;
            straightsUnique.resize( maxCardsValue + 1 );

            std::map< uint64_t, uint16_t > highCardProductMap;
            std::map< uint64_t, uint16_t > straightsProductMap;

            for ( auto&& ii : allHands )
            {
                auto cardValue = ii.second.getCardsValue();

                auto pos = flushesAndStraightsCount.find( ii.second );
                Q_ASSERT( pos != flushesAndStraightsCount.end() );

                auto straightsValue = ( *pos ).second;

                auto pos2 = justCardsCount.find( ii.second );
                Q_ASSERT( pos2 != justCardsCount.end() );
                auto highCardValue = ( *pos2 ).second;

                if ( ii.second.isFlush() )
                {
                    flushes[ cardValue ] = straightsValue;
                }
                else if ( ii.second.allCardsUnique() )
                {
                    straightsUnique[ cardValue ] = straightsValue;
                    highCardUnique[ cardValue ] = highCardValue;
                }
                else
                {
                    auto productValue = ii.second.handProduct();
                    straightsProductMap[ productValue ] = straightsValue;
                    highCardProductMap[ productValue ] = highCardValue;
                }
            }
            CCardInfo::generateTable( *oss, flushes, "sFlushes" );
            CCardInfo::generateTable( *oss, highCardUnique, "sHighCardUnique" );
            CCardInfo::generateTable( *oss, straightsUnique, "sStraightsUnique" );
            CCardInfo::generateMap( *oss, highCardProductMap, "sProductMap" );
            CCardInfo::generateMap( *oss, straightsProductMap, "sStraitsAndFlushesProductMap" );

            CCardInfo::generateEvaluateFunction( *oss, 4 );
            CCardInfo::generateRankFunction( *oss, 4, justCardsCount, flushesAndStraightsCount );
        }
    }
}