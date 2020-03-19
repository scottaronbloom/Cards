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

#include "MainWindow.h"
#include "Cards/Game.h"
#include "ui_MainWindow.h"

#include <QTimer>

CMainWindow::CMainWindow( QWidget* parent )
    : QDialog( parent ),
    fImpl( new Ui::CMainWindow )
{
    fImpl->setupUi( this );
    fGame = std::make_shared< CGame >();

    (void)connect( fImpl->deal, &QPushButton::clicked, this, &CMainWindow::slotDeal );
    (void)connect( fImpl->autoDeal, &QPushButton::clicked, this, &CMainWindow::slotAutoDeal );
    //QTimer::singleShot( 0, this, &CMainWindow::slotDeal );
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::slotDeal()
{
    fGame->nextDealer();
    fGame->shuffleAndDeal();
    fImpl->data->setPlainText( fGame->dumpGame() );
    fImpl->stats->setPlainText( fGame->dumpStats() );
}

void CMainWindow::slotAutoDeal()
{
    fAutoDealing = !fAutoDealing;
    fImpl->autoDeal->setText( fAutoDealing ? "Stop Auto Deal" : "Auto Deal" );
    slotRunAutoDeal();
}

void CMainWindow::slotRunAutoDeal()
{
    if ( fAutoDealing )
    {
        fGame->nextDealer();
        fGame->shuffleAndDeal();
        int interval = 100;
#ifndef _DEBUG
        interval = 10000;
#endif
        if ( ( fGame->numGames() % interval ) == 0 )
        {
            fImpl->data->setPlainText( fGame->dumpGame() );
            fImpl->stats->setPlainText( fGame->dumpStats() );
        }
        QTimer::singleShot( 0, this, &CMainWindow::slotRunAutoDeal );
    }
}


void CMainWindow::slotNextDealer()
{
    fGame->nextDealer();
    fImpl->data->setPlainText( fGame->dumpGame() );
}

void CMainWindow::slotPrevDealer()
{
    fGame->prevDealer();
    fImpl->data->setPlainText( fGame->dumpGame() );
}

