//
//  sfgui.h
//  cute-fantasy
//
//  Created by Jay Berg on 4/8/14.
//
//
#ifndef SFGUI_H
#define SFGUI_H

#include <QWidget>
#include <ProtoData.pb.h>
#include <QThread>
#include <QString>

namespace Ui {
class sfGUI;
}

namespace fantasybit
{

class sfGUI : public QWidget
{
    Q_OBJECT

    enum State { CONNECTING, LIVE };
    //MyNameStatus m_status;
    //State m_state;
    //MyFantasyName m_namestatus{};
    InData indata{};
    DeltaData snapData{}, deltaData{};
public:
    explicit sfGUI(QWidget *parent = 0);
    ~sfGUI();

public slots:
    void fromServer(const DeltaData &in);// {}

    //void fromServer(const fantasybit::OutData &in);
private slots:


    void on_generate_clicked();

    void on_copy_clicked();
    //void flashing();


protected:
    virtual void closeEvent(QCloseEvent *)
    {
        emit onClose();
    }

signals:
    void onClose();
    void fromGUI(const InData &);
    //void on_flash();

private:
    Ui::sfGUI *ui;
    void updatesnap();
    void updatedelta();

};

}
#endif // SFGUI_H
