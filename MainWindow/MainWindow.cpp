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
    fNameWidgets = TPlayerWidgetVector(
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

    fWCWidgets = TWildCardWidgetVector(
        {
             { fImpl->wcLabel1,  fImpl->wcCard1,  fImpl->wcSuit1 }
            ,{ fImpl->wcLabel2,  fImpl->wcCard2,  fImpl->wcSuit2 }
            ,{ fImpl->wcLabel3,  fImpl->wcCard3,  fImpl->wcSuit3 }
            ,{ fImpl->wcLabel4,  fImpl->wcCard4,  fImpl->wcSuit4 }
            ,{ fImpl->wcLabel5,  fImpl->wcCard5,  fImpl->wcSuit5 }
            ,{ fImpl->wcLabel6,  fImpl->wcCard6,  fImpl->wcSuit6 }
            ,{ fImpl->wcLabel7,  fImpl->wcCard7,  fImpl->wcSuit7 }
            ,{ fImpl->wcLabel8,  fImpl->wcCard8,  fImpl->wcSuit8 }
            ,{ fImpl->wcLabel9,  fImpl->wcCard9,  fImpl->wcSuit9 }
            ,{ fImpl->wcLabel10, fImpl->wcCard10, fImpl->wcSuit10 }
            ,{ fImpl->wcLabel11, fImpl->wcCard11, fImpl->wcSuit11 }
            ,{ fImpl->wcLabel12, fImpl->wcCard12, fImpl->wcSuit12 }
        }
    );

    fGame = std::make_shared< CGame >();

    (void)connect( fImpl->deal, &QPushButton::clicked, this, &CMainWindow::slotDeal );
    (void)connect( fImpl->autoDeal, &QPushButton::clicked, this, &CMainWindow::slotAutoDeal );
    (void)connect( fImpl->reanalyzeHand, &QPushButton::clicked, this, &CMainWindow::slotReanalyzeHand );
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

    (void)connect( fImpl->numWildCards, static_cast<void( QSpinBox::* )( int )>( &QSpinBox::valueChanged ), this, &CMainWindow::slotNumWildCardsChanged );
    for ( size_t ii = 0; ii < fWCWidgets.size(); ++ii )
    {
        (void)connect( std::get< 1 >( fWCWidgets[ ii ] ), static_cast<void( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ), this, &CMainWindow::slotWildCardsChanged );
        (void)connect( std::get< 2 >( fWCWidgets[ ii ] ), static_cast<void( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ), this, &CMainWindow::slotWildCardsChanged );
    }

    (void)connect( fImpl->straightsAndFlushesCount, &QCheckBox::clicked, this, &CMainWindow::slotStraightsAndFlushesCountChanged );
    (void)connect( fImpl->lowHandWins, &QCheckBox::clicked, this, &CMainWindow::slotLowHandWinsChanged );
    fImpl->handsPerSecondLabel->setHidden( true );
    fImpl->reanalyzeHand->setEnabled( false );
    loadSettings();
    showGame();
}

CMainWindow::~CMainWindow()
{
    saveSettings();
}

void CMainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue( "NumCards", numCards() );
    fImpl->straightsAndFlushesCount->setChecked( settings.value( "StraightsAndFlushesCount", false ).toBool() );
    fImpl->lowHandWins->setChecked( settings.value( "LowHandWins", false ).toBool() );

    settings.beginWriteArray( "Players" );
    for ( int ii = 0; ii < fImpl->numPlayers->value(); ++ii )
    {
        settings.setArrayIndex( ii );
        settings.setValue( "Name", fNameWidgets[ ii ].second->text() );
    }
    settings.endArray();

    settings.beginWriteArray( "WildCards" );
    for ( int ii = 0; ii < fImpl->numWildCards->value(); ++ii )
    {
        settings.setArrayIndex( ii );
        settings.setValue( "Card", std::get< 1 >( fWCWidgets[ ii ] )->currentText() );
        settings.setValue( "Suit", std::get< 2 >( fWCWidgets[ ii ] )->currentText() );
    }
    settings.endArray();
}

void CMainWindow::loadSettings()
{
    QSettings settings;

    auto numCards = settings.value( "numCards", 5 ).toInt();
    setNumCards( numCards );

    fImpl->straightsAndFlushesCount->setChecked( settings.value( "StraightsAndFlushesCount", false ).toBool() );
    fImpl->lowHandWins->setChecked( settings.value( "LowHandWins", false ).toBool() );

    fGame->setStraightsAndFlushesCount( fImpl->straightsAndFlushesCount->isChecked() );
    fGame->setLowHandWins( fImpl->lowHandWins->isChecked() );

    int size = settings.beginReadArray( "Players" );
    if ( size == 0 )
    {
        QStringList players = { "Scott", "Craig", "Eric", "Keith" };
        for( int ii = 0; ii < players.size(); ++ii )
        {
            fNameWidgets[ ii ].second->setText( players[ ii ] );
            fGame->setPlayerName( ii, players[ ii ] );
        }
    }
    else
    {
        fImpl->numPlayers->setValue( std::max( 1, std::min( 10, size ) ) );
        for ( int ii = 0; ii < size; ++ii )
        {
            settings.setArrayIndex( ii );
            fNameWidgets[ ii ].second->setText( settings.value( "Name" ).toString() );
            fGame->setPlayerName( ii, settings.value( "Name" ).toString() );
        }
    }

    settings.endArray();
    size = settings.beginReadArray( "WildCards" );
    fImpl->numWildCards->setValue( std::min( 12, size ) );
    for ( int ii = 0; ii < size; ++ii )
    {
        settings.setArrayIndex( ii );
        std::get< 1 >( fWCWidgets[ ii ] )->setCurrentIndex( std::get< 1 >( fWCWidgets[ ii ] )->findText( settings.value( "Card" ).toString() ) );
        std::get< 2 >( fWCWidgets[ ii ] )->setCurrentIndex( std::get< 2 >( fWCWidgets[ ii ] )->findText( settings.value( "Suit" ).toString() ) );
    }
    settings.endArray();
    slotNumPlayersChanged();
    slotNumWildCardsChanged();
}

void CMainWindow::setNumCards( int numCards )
{
    fImpl->twoCard->setChecked( numCards == 2 );
    fImpl->threeCard->setChecked( numCards == 3 );
    fImpl->fourCard->setChecked( numCards == 4 );
    fImpl->fiveCard->setChecked( numCards == 5 );
    fImpl->sevenCard->setChecked( numCards == 7 );
    fGame->setNumCards( numCards );
}

int CMainWindow::numCards() const
{
    if ( fImpl->twoCard->isChecked() )
        return 2;
    else if ( fImpl->threeCard->isChecked() )
        return 3;
    else if ( fImpl->fourCard->isChecked() )
        return 4;
    else if ( fImpl->fiveCard->isChecked() )
        return 5;
    else /*if ( fImpl->sevenCard->isChecked() )*/
        return 7;
}

void CMainWindow::slotDeal()
{
    fGame->nextDealer();
    fGame->shuffleAndDeal();
    showGame();
    showStats();
    fImpl->reanalyzeHand->setEnabled( true );
}

void CMainWindow::slotReanalyzeHand()
{
    fGame->analyzeHand( false );
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
    fImpl->reanalyzeHand->setEnabled( !fAutoDealing );
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

void CMainWindow::slotNumWildCardsChanged()
{
    auto num = fImpl->numWildCards->value();
    for ( int ii = 0; ii < num; ++ii )
    {
        std::get< 0 >( fWCWidgets[ ii ] )->setVisible( true );
        std::get< 1 >( fWCWidgets[ ii ] )->setVisible( true );
        std::get< 2 >( fWCWidgets[ ii ] )->setVisible( true );
    }
    for ( size_t ii = num; ii < fWCWidgets.size(); ++ii )
    {
        std::get< 0 >( fWCWidgets[ ii ] )->setVisible( false );
        std::get< 1 >( fWCWidgets[ ii ] )->setVisible( false );
        std::get< 2 >( fWCWidgets[ ii ] )->setVisible( false );
    }
    slotWildCardsChanged( false );
    showGame();
}

void CMainWindow::slotWildCardsChanged( bool showGame )
{
    auto numWildCards = fImpl->numWildCards->value();
    fGame->clearWildCards();
    for( int ii = 0; ii < numWildCards; ++ii )
    {
        fGame->addWildCards( fGame->getCards( std::get< 1 >( fWCWidgets[ ii ] )->currentText(), std::get< 2 >( fWCWidgets[ ii ] )->currentText(), false ) );
    }
    if ( showGame )
        this->showGame();
}

void CMainWindow::slotStraightsAndFlushesCountChanged()
{
    fGame->setStraightsAndFlushesCount( fImpl->straightsAndFlushesCount->isChecked() );
}

void CMainWindow::slotLowHandWinsChanged()
{
    fGame->setLowHandWins( fImpl->lowHandWins->isChecked() );
}
