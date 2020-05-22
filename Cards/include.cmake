# The MIT License (MIT)
#
# Copyright (c) 2020 Scott Aron Bloom
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(qtproject_SRCS
    Card.cpp
    CardInfo.cpp
    Evaluate2CardHand.cpp
    Evaluate3CardHand.cpp
    Evaluate4CardHand.cpp
    Evaluate5CardHand.cpp
    GenerateCardTables.cpp
    2CardHandTables.cpp
    3CardHandTables.cpp
    4CardHandTables.cpp
    5CardHandTables.cpp
    Game.cpp
    Hand.cpp
    HandImpl.cpp
    HandUtils.cpp
    Player.cpp
)

set(qtproject_H
)

set(project_H
    Card.h
    CardInfo.h
    Evaluate2CardHand.h
    Evaluate3CardHand.h
    Evaluate4CardHand.h
    Evaluate5CardHand.h
    Game.h
    Hand.h
    HandImpl.h
    HandUtils.h
    Player.h
    PlayInfo.h
)

set(qtproject_UIS
)

set(qtproject_QRC
)

file(GLOB qtproject_QRC_SOURCES "resources/*")
