#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>

namespace Ui {
class RegWidget;
}

class RegWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RegWidget(QWidget *parent = 0);
    ~RegWidget();

    /*�Ƿ�ע��*/
    bool    isRegistered();

    /*��������:�Ƿ���*/
    bool    isExpiration();

signals:
    void onClose();

private slots:
    void close();
    void registrate();
    void setHardwareidClipboard();
    void getUsernameClipboard();
    void getKeyidClipboard();

private:
    Ui::RegWidget *ui;
    bool    m_bExpiration;

    void initialize();
    bool checkKey( const QString& userKey, const QString& userName);
    void processRegistration();
    QString hardwareId();
    QString keyDate();

};

#endif // REGWIDGET_H
