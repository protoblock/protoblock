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
#include <fb/ProtoData.pb.h>
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

    enum State { CONNECTING, SNAPSHOT, REQUESTED, MINING, FOUND, CONFIRMED };
    //MyNameStatus m_status;
    State m_state;
    MyFantasyName m_namestatus{};
    InData indata{};
public:
    explicit sfGUI(QWidget *parent = 0);
    ~sfGUI();

public slots:
    void fromServer(const OutData &in);// {}

    //void fromServer(const fantasybit::OutData &in);
private slots:


    void on_generate_clicked();

    void on_copy_clicked();

protected:
    virtual void closeEvent(QCloseEvent *)
    {
        emit onClose();
    }

signals:
    void onClose();
    void fromGUI(const InData &);

private:
    Ui::sfGUI *ui;
    void updatestatic();
};

}
#endif // SFGUI_H
