#ifndef ARCHIVEREADER_H
#define ARCHIVEREADER_H

#include <QObject>

class ArchiveReader
{
public:
    explicit ArchiveReader();

    bool readArchive(const QString& archiveFile, const QString& password);

    /* ���ݸ��� filename ,�Ӱ���ȡ�����ļ�,����ȡ���ظ��ļ������� */
    QString readArchiveFile(const QString &title, const QString& archiveFile, const QString& password);

    /*  �����ĴӰ��ж�ȡ�ļ�, ���ٶ���Ӱ��,�Ƿ���Կ����Ȱ����������ݶ�ȡ���ڴ�,�����ļ�ʱ���ֱ�Ӵ��ڴ���ȡ���ļ�����? */
//    QVector<QByteArray> filesData;

    // �г����������ļ�
    QStringList listFiles(const QString& archiveFile, const QString& password);
    
};

#endif // ARCHIVEREADER_H
