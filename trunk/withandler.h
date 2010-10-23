#ifndef WITHANDLER_H
#define WITHANDLER_H

#include <QObject>
#include <QProcess>
#include <QTreeWidgetItem>

#define WIT_MINIMUM_VERSION 1883

extern QString rootAskStr;
extern QString rootWrongStr;
extern QString rootFailStr;

enum
{
    witNoJob = 1000,
    witGetVersion,
    witCopy,
    witIlist,
    witDump,
    witEdit,
    witListLLLHDD
};

class WitHandler : public QObject
{
    Q_OBJECT


public:
    WitHandler( QObject *parent = 0, bool root = false );
    ~WitHandler();

    void SetRunAsRoot( bool root = true );
    void ListLLL_HDD( QString path );

    void SetNamesFromWiiTDB( bool wiitdb = true );

    //block the current thread and wait for wit to finish
    bool Wait( int msecs = 30000 );
    void Kill();


    void RunJob( QStringList args, int jobType );

    //static functions are blocking, and will halt the current thread until the process finishes
    //static functions dont respect the "root" settings
    static bool ReadAttributes();
    static bool NameSupportsAttribute( QString name, QString attr );
    static bool OptionSupportsAttribute( QString opt, QString attr );
    static bool ExtensionSupportsAttribute( QString ext, QString attr );
    static bool ReadVersion();
    static bool VersionIsOk();
    static QString GetVersionString();
    static QStringList FileType( QStringList files );
    //static bool OptionSupports( QString option, QString attr );

private:
    QProcess *process;
    int witJob;

    QString stdStr;
    QString errStr;

    QString currJobText;

    bool runAsRoot;
    bool requestedPassword;

    bool namesFromWiiTDB;

    QString GetWitPath();

    QList<QTreeWidgetItem *> StringListToGameList( QStringList list, bool *ok );

signals:
    void KillProcess();
    void SendFatalErr( QString, int );
    void SendStdErr( QString );
    void SendStdOut( QString );
    void SendProgress( int );
    void RequestPassword();
    void SendJobDone( int );//sent when done copying & editing.  not sent ofter every joy ( yet )
    void SendMessageForStatusBar( QString );

    void SendListLLL( QList<QTreeWidgetItem *> games, QString MibUsed );

    void SendGameInfo( QString type, QString id, QString name, int ios, int region, QStringList files, QStringList partitionOffsets, bool fakesigned );

public slots:
    void PasswordIsEntered();

private slots:
    void ReadyReadStdOutSlot();
    void ReadyReadStdErrSlot();
    void ProcessFinishedSlot( int, QProcess::ExitStatus );

};

#endif // WITHANDLER_H