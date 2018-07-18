#ifndef PEERCONNECTION_H
#define PEERCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <functional>
#include <QSet>
#include <QLinkedList>

#include "macro.h"
#include "peerinfo.h"

class TorrentClient;
class Torrent;
class QTimer;

struct TorrentBlockDescriptor{
    quint32     Index;
    quint32     Begin;
    quint32     Length;

    TorrentBlockDescriptor(){ Index = 0; Begin = 0; Length = 0; }
    TorrentBlockDescriptor(quint32 i, quint32 b, quint32 l){
        Index = i;
        Begin = b;
        Length = l; }

    friend bool operator ==( const TorrentBlockDescriptor &l, const TorrentBlockDescriptor &r ){
        return  l.Index == r.Index &&
                l.Begin == r.Begin &&
                l.Length == r.Length;
    }
};

inline int qHash( const TorrentBlockDescriptor& blockDesc, uint seed ){
    return qHash( blockDesc.Begin, seed ) ^ qHash( blockDesc.Index, seed ) ^ qHash( blockDesc.Length, seed );
}

struct DownloadingTorrentBlock{
    DownloadingTorrentBlock(){Begin = 0; RequestedLength = 0;}
    DownloadingTorrentBlock(quint32 begin, quint32 requestedLen){Begin = begin; RequestedLength = requestedLen;}
    quint32     Begin;
    QByteArray  Block;
    quint32     RequestedLength;

    friend bool operator ==( const DownloadingTorrentBlock &l, const DownloadingTorrentBlock &r ){
        return  l.Begin == r.Begin &&
                l.RequestedLength == r.RequestedLength &&
                l.Block.size() == r.Block.size();
    }
};

class PeerConnection : public QTcpSocket
{
    Q_OBJECT
public:
    PeerConnection(QObject *parent = nullptr);
    void clear();
    ~PeerConnection();

    //// Read only
    PROP_GET( PeerInfo,         PeerInfo )
    PROP_GET( QByteArray,       InfoHash )

    PROP_GET( uint,             LastVisited )
    PROP_GET( uint,             ConnectStart )
    PROP_GET( uint,             ConnectTime )

    //// Read-Write-Notification
    PROP_SIMPLE( bool,          IsPeerChoking )       // Этот пир блокирует меня
    PROP_SIMPLE( bool,          IsPeerInterested )    // Этот пир заинтересован во мне
    PROP_SIMPLE( bool,          IsAmChoking )         // Я блокирую этого пира
    PROP_SIMPLE( bool,          IsAmInterested )      // Я заинтересован в пире
    PROP_SIMPLE( bool,          IsSeed )

    PROP_SIMPLE( int,           NumCompletedPieces )

    PROP_GET( quint32,          HandshakeTimeout )
    PROP_GET( quint32,          KeepAliveTimeout )
    PROP_GET( quint32,          MyKeepAliveTimeout )

    PROP_GET_SET( quint32,      PieceSize )

public:
    void SetHandshakeTimeout( quint32 newTimeout ){SetTimeout( m_HandshakeTimeout, m_timerHandhsake, newTimeout );}
    void SetKeepAliveTimeout( quint32 newTimeout ){SetTimeout( m_KeepAliveTimeout, m_timerKeepAlive, newTimeout );}
    void SetMyKeepAliveTimeout( quint32 newTimeout ){SetTimeout( m_MyKeepAliveTimeout, m_timerMyKeepAlive, newTimeout );}

    quint64 GetDownloadSpeed() const { return CalculateSpeed( m_bytesDownloadedList ); }
    quint64 GetUploadSpeed() const   { return CalculateSpeed( m_bytesUploadedList ); }

public slots:
    void connectToPeer(const PeerInfo &peerInfo , const QByteArray &infoHash, quint32 pieceCount);
    void peerConnectMe( qintptr socketDescriptor );
    void DownloadBlock(quint32 index, quint32 begin, quint32 length);

private:
    QTimer     *m_timerHandhsake;
    QTimer     *m_timerKeepAlive;
    QTimer     *m_timerMyKeepAlive;
    QTimer     *m_timerSpeedTest;
    QTimer     *m_timerRequestedData;

    void        initTimer(QTimer *&timer, quint32 timeout, std::function< void(void) > timeoutHandler);
    void        clearTimer( QTimer *timer, quint32 timeout );
    void        SetTimeout(quint32 &timeoutVar, QTimer *timerVar, quint32 newTimeout);

    TorrentBlockDescriptor m_lastRequestData;
    void        timerRequestedDataHandler();

    //// Длинна списокв, хранящих кол-во скач/отд байт несколько обработок назад. Чем больше, тем сглаженнее скорость.
    uint m_bytesSpeedSize;
    //// Хранят количество скачанных/отданных байт несколько итераций назад
    QLinkedList< quint64 >  m_bytesDownloadedList;
    QLinkedList< quint64 >  m_bytesUploadedList;
    //// Накапливают количество скач/отд байт
    quint64                 m_bytesDownloaded;
    quint64                 m_bytesUploaded;
    void timerSpeedTestHandler();
    void timerSpeedTestHandler(quint64 &bytes, QLinkedList< quint64 > &list);
    quint64 CalculateSpeed(const QLinkedList<quint64> &list ) const;

    QSet< TorrentBlockDescriptor >              m_peerRequestedBlocks;
    QSet< TorrentBlockDescriptor >              m_amRequestedBlocks;

    QMultiHash< quint32, DownloadingTorrentBlock > m_downloadedBlocks;     //< index, begin + block >
    QBitArray                                   m_peerPieces;

signals:
    void IsPeerChokingChanged( bool newVal );
    void IsPeerInterestedChanged( bool newVal );
    void IsAmChokingChanged( bool newVal );
    void IsAmInterestedChanged( bool newVal );
    void IsSeedChanged( bool newVal );

    void PeerPiecesChanged( const QBitArray &newVal );
    void NumCompletedPiecesChanged( int newVal );
    void PeerRequest( quint32 index, quint32 begin, quint32 length );
    void BlockDownloaded( quint32 index, quint32 begin, const QByteArray &block );

    void handshakeIsDone();
    void handshakeFailed();
    void invalidPeer();

    void incomingHandhsakeData( const QByteArray &infoHash, const QByteArray &peerId );

//private
public slots:
    // handshake: <pstrlen><pstr><reserved><info_hash><peer_id>
    void prepareHandshakeMsg();
    void sendKeepAlive();
    void sendChoke();
    void sendUnchoke();
    void sendInterested();
    void sendNotInterested();
    void sendHave( quint32 pieceIndex );
    void sendRequest( quint32 index, quint32 begin, quint32 length );

    void responseHandler();

    bool handshakeHandler();
    void bitFildHandler();
    void RequestHandler();
    void PieceHandler();
    void CancelHandler();

private:
    void processingIncomingBlock(const TorrentBlockDescriptor &desc, const QByteArray &block);

private:
    QByteArray      m_outgoingBuffer;
    QByteArray      m_incomingBuffer;
    bool            m_handshakeHandled;         // Рукопожатие было обработано мной
    bool            m_handshakeSended;          // Рукопожатие было послано мной

private:
    enum PacketType {
        PacketKeepAlive = -1,
        PacketChoke = 0,
        PacketUnchoke = 1,
        PacketInterested = 2,
        PacketNotInterested = 3,
        PacketHave = 4,
        PacketBitField = 5,
        PacketRequest = 6,
        PacketPiece = 7,
        PacketCancel = 8,
        PacketPort = 9,
        PacketInvalid
    };

    PacketType  incomingPacetType();
    quint32     incomingLen();
};

#endif // PEERCONNECTION_H
