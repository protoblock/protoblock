#include "mywindow.h"
#include <QMessageBox>
#include <QProgressBar>
#include <QLabel>

MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    auto pb     = new QProgressBar( this );
    auto state  = new QLabel( this );
    auto dSpeed = new QLabel( this );
    auto uSpeed = new QLabel( this );

    auto layout = new QVBoxLayout;
    layout->addWidget( pb );
    layout->addWidget( state );
    layout->addWidget( dSpeed );
    layout->addWidget( uSpeed );
    setLayout( layout );

    QFile file( "D:\\test.torrent" );
    if ( !file.open( QFile::ReadOnly ) ){
        return ;
    }

    m_downloader = new Downloader( TorrentFileInfo::parse( file.readAll() ), this );
    connect( m_downloader, &Downloader::progressChanged, pb, &QProgressBar::setValue );

    pb->setMaximum( 100 );
    connect( m_downloader, &Downloader::downloadSpeedChanged, [=]( quint64 newSpeed ){
        dSpeed->setNum( (int)newSpeed );
    } );
    connect( m_downloader, &Downloader::uploadSpeedChanged, [=]( quint64 newSpeed ){
        uSpeed->setNum( (int)newSpeed );
    } );
    connect( m_downloader, &Downloader::stateIsChanged, [=]( Downloader::States newState ){
        state->setText( Downloader::getStateName( newState ) );
    } );

    m_downloader->startTorrent();

}

MyWindow::~MyWindow()
{

}

