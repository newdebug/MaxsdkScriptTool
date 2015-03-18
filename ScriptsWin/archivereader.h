
#ifndef ARCHIVEREADER_H
#define ARCHIVEREADER_H

#include <QObject>

class ArchiveReader
{
public:
    explicit ArchiveReader();

    /* ���ݸ��� filename ,�Ӱ���ȡ�����ļ�,����ȡ���ظ��ļ������� */
    QString readArchiveFile(const QString &title, const QString& archiveFile, const QString& password);
    /*  �����ĴӰ��ж�ȡ�ļ�, ���ٶ���Ӱ��,�Ƿ���Կ����Ȱ����������ݶ�ȡ���ڴ�,�����ļ�ʱ���ֱ�Ӵ��ڴ���ȡ���ļ�����? */
//    QVector<QByteArray> filesData;

    // �г����������ļ�
    QStringList listFiles(const QString& archiveFile, const QString& password);
    
    // ��ѹ��
    QStringList extractArchive(const QString &archive, const QString& password);

    // Ȼ�������´��ϰ�
    bool reCompressFiles(const QStringList &files, const QString &archive, const QString &password);
    bool reCompressFile(const QString &file, const QString &archive, const QString &password);

};

#endif // ARCHIVEREADER_H
