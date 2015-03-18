#include "regwidget.h"
#include "ui_regwidget.h"

#include <qt_windows.h>
#include "aspr_api.h"
#include "asprotect.h"

#include <QMessageBox>
#include <QClipboard>
#include <QTextCodec>

#define ModeID     0       // no any mode
#define ModeID1    1       // trail
#define ModeID2    2       // registration
#define ModeID3    3       // forever(undefined)

RegWidget::RegWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegWidget),
    m_bExpiration(false)
{
    ui->setupUi(this);

    initialize();

    QObject::connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->cancelBtn, SIGNAL(clicked()), this, SIGNAL(onClose()));

    connect(ui->regBtn, SIGNAL(clicked()), this, SLOT(registrate()));
    connect(ui->copyHardwareBtn, SIGNAL(clicked()), this, SLOT(setHardwareidClipboard()));
    connect(ui->pasteUsernameBtn, SIGNAL(clicked()), this, SLOT(getUsernameClipboard()));
    connect(ui->pasteUserkeyBtn, SIGNAL(clicked()), this, SLOT(getKeyidClipboard()));
}

RegWidget::~RegWidget()
{
    delete ui;
}

void RegWidget::close()
{
    QWidget::close();
    emit onClose();
}

void RegWidget::setHardwareidClipboard()
{
    QClipboard *board = QApplication::clipboard();
    QString txt = ui->hardwareEdt->text();
    board->setText( txt );
}

void RegWidget::getUsernameClipboard()
{
    QClipboard *board = QApplication::clipboard();
    QString txt = board->text();
    ui->usernameEdt->setText( txt );
}

void RegWidget::getKeyidClipboard()
{
    QClipboard *board = QApplication::clipboard();
    QString txt = board->text();
    ui->userkeyEdt->setText( txt );
}

void RegWidget::initialize()
{
    ui->hardwareEdt->setText( hardwareId() );
    processRegistration();
}

QString RegWidget::hardwareId()
{
    char *pbuf = NULL;
    pbuf = GetHardwareID();
    QString hardwareid = QString::fromLocal8Bit(pbuf);

    return hardwareid;
}

bool RegWidget::isRegistered()
{
    char* userKey  = NULL;
    char* userName = NULL;
    GetRegistrationInformation( ModeID2, &userKey,  &userName );
    if ((userKey != NULL) && (strlen(userName) > 0))
    {
        if ( CheckKey( (char*)userKey, (char*)userName, 0 ) )
        {
            return true;
        }
    }

    return false;
}

bool RegWidget::isExpiration()
{
    return m_bExpiration;
}

void RegWidget::registrate()
{
    QString userName = ui->usernameEdt->text();
    QString regKey = ui->userkeyEdt->text();
    if( userName.isEmpty() )
    {
        QMessageBox::information(this, QString::fromLocal8Bit("ע��"), QString::fromLocal8Bit("�������û�����"), 0);
        return;
    }

    if( regKey.isEmpty() )
    {
        QMessageBox::information(this, QString::fromLocal8Bit("ע��"), QString::fromLocal8Bit("������ע���롣"), 0);
        return;
    }

    if ( checkKey(regKey, userName) )
    {
        ui->usernameEdt->setText( userName );
        ui->userkeyEdt->setText( regKey );

        char* ModeName = NULL;
        MODE_STATUS   mode_status;
        REG_CRYPT_BEGIN1
            GetModeInformation( ModeID2, &ModeName, &mode_status );
            ui->regStatusLb->setText(ModeName);
            ui->regUserLb->setText( userName );
            QString regDate = keyDate();
            ui->regDateLb->setText( regDate );

            ui->regBtn->setHidden(true);

            WORD Year= 0, Month = 0, Day = 0;

            if ( GetKeyExpirationDate( ModeID2, &Day, &Month, &Year ))
            {
                ui->regStatusLb->setText(QString::fromLocal8Bit("��ע���<��ʹ������>"));

                QString regDate = keyDate();
                QString left = QString(QString::fromLocal8Bit("<����ʱ��:%1��%2��%3��>")).arg(Year).arg(Month).arg(Day);
                ui->regDateLb->setText(regDate + left);
                ui->regBtn->setHidden(false);
                ui->regBtn->setEnabled(true);
            }
        REG_CRYPT_END1
    }
    else
    {
        QMessageBox::warning(0, QString::fromLocal8Bit("ע��"), QString::fromLocal8Bit("ע��ʧ��:��Ч��ע���롣"));
    }
}

bool RegWidget::checkKey(const QString &userKey, const QString &userName)
{
    Q_ASSERT( userName.isEmpty() == false );
    Q_ASSERT( userKey.isEmpty() == false );

    QByteArray data1 = userKey.toLatin1();
    char* userkey = data1.data();
    QByteArray data2 = userName.toLatin1();
    char* username = data2.data();
    if ( CheckKeyAndDecrypt(userkey, username, true) )
        return true;

    return false;
}

void RegWidget::processRegistration()
{
    char* userkey  = NULL;
    char* username = NULL;
    char* ModeName = NULL;
    MODE_STATUS   mode_status;

    GetRegistrationInformation( ModeID2, &userkey,  &username );

    // �������ע��,������ע����ʹ������
    if ((userkey != NULL) && (strlen(userkey) > 0))
    {
        QString key = QString::fromLocal8Bit( userkey );
        QString name = QString::fromLocal8Bit( username );

        REG_CRYPT_BEGIN1
            GetModeInformation( ModeID2, &ModeName, &mode_status );
            ui->usernameEdt->setText( name );
            ui->userkeyEdt->setText( key );

            ui->regStatusLb->setText(ModeName);
            ui->regUserLb->setText( username );
            QString regDate = keyDate();
            ui->regDateLb->setText( regDate );

            ui->regBtn->setHidden(true);

            WORD Year= 0, Month = 0, Day = 0;

            if ( GetKeyExpirationDate( ModeID2, &Day, &Month, &Year ))
            {
                ui->regStatusLb->setText(QString::fromLocal8Bit("��ע���<��ʹ������>"));

                QString regDate = keyDate();
                QString left = QString(QString::fromLocal8Bit("<����ʱ��:%1��%2��%3��>")).arg(Year).arg(Month).arg(Day);
                ui->regDateLb->setText(regDate + left);
                ui->regBtn->setHidden(false);
                ui->regBtn->setEnabled(true);
            }
        REG_CRYPT_END1
    }
    else
    {
        DWORD	TrialDaysTotal	= 0,
                TrialDaysLeft	= 0;
        if (GetTrialDays( ModeID1, &TrialDaysTotal, &TrialDaysLeft ))
        {
            if( TrialDaysLeft == 0 )
            {
                ui->regStatusLb->setText(QString::fromLocal8Bit("δע�����ð渱��"));
                ui->regDateLb->setText(QString::fromLocal8Bit("���ð渱���ѵ���,����Ҫ����ʹ����ע��"));
                ui->regBtn->setHidden(false);
                ui->regBtn->setEnabled(true);
                m_bExpiration = true;
            }
            else
            {
                ui->regStatusLb->setText(QString::fromLocal8Bit("δע�����ð渱��"));
                QString str = QString(QString::fromLocal8Bit("δע�����ð渱��:ʣ�� %1 ������,���������� %2"))
                        .arg(TrialDaysLeft).arg(TrialDaysTotal);
                ui->regDateLb->setText(str);
                ui->regBtn->setHidden(false);
                ui->regBtn->setEnabled(true);
                m_bExpiration = false;
            }
        }
    }
}

QString RegWidget::keyDate()
{
    WORD day, month, year;
    GetKeyDate(ModeID2, &day, &month, &year);
    QString date = QString(QString::fromLocal8Bit("%1��%2��%3��")).arg(year).arg(month).arg(day);

    return date;
}
