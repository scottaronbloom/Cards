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
#include "SABUtils/utils.h"
#include "ui_MainWindow.h"

#include <QSettings>
#include <QTimer>


CMainWindow::CMainWindow( QWidget* parent )
    : QDialog( parent ),
    fImpl( new Ui::CMainWindow )
{
    fImpl->setupUi( this );
    using nameWidgetVector = std::vector< std::pair< QLabel*, QLineEdit* > >;
    fNameWidgets = nameWidgetVector( 
                { 
                      { fImpl->playerLabel1, fImpl->playerName1 }
                    , { fImpl->playerLabel2, fImpl->playerName2 }
                    , { fImpl->playerLabel3, fImpl->playerName3 }
                    , { fImpl->playerLabel4, fImpl->playerName4 }
                    , { fImpl->playerLabel5, fImpl->playerName5 }
                    , { fImpl->playerLabel6, fImpl->playerName6 }
                    , { fImpl->playerLabel7, fImpl->playerName7 }
                    , { fImpl->playerLabel8, fImpl->playerName8 }
                    , { fImpl->playerLabel9, fImpl->playerName9 }
                    , { fImpl->playerLabel10, fImpl->playerName10 }
        }
                );
    fGame = std::make_shared< CGame >();

    (void)connect( fImpl->deal, &QPushButton::clicked, this, &CMainWindow::slotDeal );
    (void)connect( fImpl->autoDeal, &QPushButton::clicked, this, &CMainWindow::slotAutoDeal );
    (void)connect( fImpl->numPlayers, static_cast< void( QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CMainWindow::slotNumPlayersChanged );
    for( size_t ii = 0; ii < fNameWidgets.size(); ++ii )
    {
        (void)connect( fNameWidgets[ ii ].second, &QLineEdit::editingFinished, this, 
                       [=]()
                       {
                            if ( fGame->setPlayerName( ii, fNameWidgets[ ii ].second->text() ).first )
                                showGame();
                       } );
    }

    fImpl->handsPerSecondLabel->setHidden( true );
    loadSettings();
    showGame();
}

CMainWindow::~CMainWindow()
{
    saveSettings();
}

void CMainWindow::loadSettings()
{
    QStringList players = { "Scott", "Craig", "Eric", "Keith" };
    
    QSettings settings;
    players = settings.value( "Players", players ).toStringList();
    fImpl->numPlayers->setValue( players.count() );
    for( int ii = 0; ii < players.count(); ++ii )
    {
        fNameWidgets[ ii ].second->setText( players[ ii ] );
        fGame->setPlayerName( ii, players[ ii ] );
    }
    auto numCards = settings.value( "numCards", 5 ).toInt();
    fImpl->fiveCard->setChecked( numCards == 5 );
    fImpl->sevenCard->setChecked( numCards == 7 );
    fGame->setNumCards( numCards );
}

void CMainWindow::saveSettings()
{
    QStringList players;
    for( int ii = 0; ii < fImpl->numPlayers->value(); ++ii )
    {
        players << fNameWidgets[ ii ].second->text();
    }
    QSettings settings;
    settings.setValue( "Players", players );
    settings.setValue( "NumCards", fImpl->fiveCard->isChecked() ? 5 : 7 );
}

void CMainWindow::slotDeal()
{
    fGame->nextDealer();
    fGame->shuffleAndDeal();
    showGame();
    showStats();
}

void CMainWindow::showStats()
{
    if ( fStartTime.has_value() && fGame )
    {
        auto endTime = std::chrono::system_clock::now();
        auto seconds = NUtils::getSeconds( endTime - fStartTime.value(), true );
        auto numGames = 1.0*fGame->numGames();
        auto hps = numGames / seconds;
        auto timeString = tr( "%1 Hands/Second" ).arg( locale().toString( hps ) );
        fImpl->handsPerSecondLabel->setHidden( false );
        fImpl->handsPerSecondLabel->setText( timeString );
    }
    else 
        fImpl->handsPerSecondLabel->setHidden( true );
    return fImpl->stats->setPlainText( fGame->dumpStats() );
}

void CMainWindow::showGame()
{
    return fImpl->data->setPlainText( fGame->dumpGame( !fAutoDealing ) );
}

void CMainWindow::slotAutoDeal()
{
    fAutoDealing = !fAutoDealing;
    fImpl->autoDeal->setText( fAutoDealing ? "Stop Auto Deal" : "Auto Deal" );
    fImpl->deal->setEnabled( !fAutoDealing );
    if ( fAutoDealing )
    {
        this->fStartTime = std::chrono::system_clock::now();
    }
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
            showGame();
            showStats();
        }
        QTimer::singleShot( 0, this, &CMainWindow::slotRunAutoDeal );
    }
}


void CMainWindow::slotNextDealer()
{
    fGame->nextDealer();
    showGame();
}

void CMainWindow::slotPrevDealer()
{
    fGame->prevDealer();
    showGame();
}

void CMainWindow::slotNumPlayersChanged()
{
    fGame->setNumPlayers( fImpl->numPlayers->value() );
    for( int ii = 0; ii < fImpl->numPlayers->value(); ++ii )
    {
        fNameWidgets[ ii ].first->setVisible( true );
        fNameWidgets[ ii ].second->setVisible( true );
        fGame->setPlayerName( ii, fNameWidgets[ ii ].second->text() );
    }
    for ( size_t ii = fImpl->numPlayers->value(); ii < fNameWidgets.size(); ++ii )
    {
        fNameWidgets[ ii ].first->setVisible( false );
        fNameWidgets[ ii ].second->setVisible( false );
        fGame->removePlayer( ii );
    }
    showGame();
}

