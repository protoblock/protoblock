#ifndef QQMLPROCESS_H
#define QQMLPROCESS_H
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QList>

class QQmlProcess : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString program READ program WRITE setProgram NOTIFY programChanged)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments NOTIFY argumentsChanged)
    Q_PROPERTY(QString workingDirectory READ workingDirectory WRITE setWorkingDirectory NOTIFY workingDirectoryChanged)
    Q_PROPERTY(int exitCode READ exitCode NOTIFY exitCodeChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY error)
    Q_PROPERTY(qint64 pid READ pid NOTIFY pidChanged)
    Q_PROPERTY(QStringList systemEnvironment READ systemEnvironment WRITE setSystemEnvironment NOTIFY systemEnvironmentChanged)

    Q_PROPERTY(QStringList runTypes READ runTypes)
    Q_PROPERTY(QStringList inputChannelModeTypes READ inputChannelModeTypes)
    Q_PROPERTY(QStringList processChannelModeTypes READ processChannelModeTypes)
    Q_PROPERTY(QStringList processChannelTypes READ processChannelTypes)


    Q_PROPERTY(RunType runType READ runType WRITE setRunType NOTIFY runTypeChanged)
    Q_PROPERTY(ExitStatus exitStatus READ exitStatus NOTIFY exitStatusChanged)
    Q_PROPERTY(InputChannelMode inputChannelMode READ inputChannelMode WRITE setInputChannelMode NOTIFY inputChannelModeChanged)
    Q_PROPERTY(ProcessChannel processChannel READ processChannel WRITE setProcessChannel NOTIFY processChannelChanged)
    Q_PROPERTY(ProcessChannelMode processMode READ processMode WRITE setProcessMode NOTIFY processModeChanged)
    Q_PROPERTY(ProcessError processError READ processError NOTIFY processErrorChanged)
    Q_PROPERTY(ProcessState processState READ processState NOTIFY processStateChanged)

    Q_ENUMS(RunType)
    Q_ENUMS(ExitStatus)
    Q_ENUMS(InputChannelMode)
    Q_ENUMS(ProcessChannel)
    Q_ENUMS(ProcessChannelMode)
    Q_ENUMS(ProcessError)
    Q_ENUMS(ProcessState)

public:
    explicit QQmlProcess(QObject *parent = 0);
    ~QQmlProcess();

    enum RunType{
        Attached,
        Detaacted
    };
    void setRunType(const RunType &runType);
    RunType runType()const;
    QStringList runTypes();
    void setrunTypes();


    enum InputChannelMode{
        ManagedInputChannel,
        ForwardedInputChannel
    };
    void setInputChannelMode(const InputChannelMode &inputChannelMode);
    InputChannelMode inputChannelMode()const;
    QStringList inputChannelModeTypes();
    void setInputChannelModeTypes();

    enum ProcessChannel{
        StandardOutput,
        StandardError,
        StanderedAll
    };
    ProcessChannel processChannel()const;
    void setProcessChannel(const ProcessChannel &processChannel);
    QStringList processChannelTypes();
    void setProcessChannelTypes();


    enum ProcessError{
        FailedToStart,
        Crashed,
        Timedout,
        WriteError,
        ReadError,
        UnknownError
    };
    ProcessError processError()const;
    void setErrorString(const QString errorString);
    QString errorString()const;

    enum ProcessState{
        NotRunning,
        Starting,
        Running
    };
    ProcessState processState()const;

    enum ProcessChannelMode{
        SeparateChannels,
        MergedChannels,
        ForwardedChannels,
        ForwardedErrorChannel,
        ForwardedOutputChannel
    };
    ProcessChannelMode processMode() const;
    void setProcessMode(ProcessChannelMode &processMode);
    QStringList processChannelModeTypes();
    void setProcessChannelModeTypes();

    enum ExitStatus{
        NormalExit,
        CrashExit
    };
    ExitStatus exitStatus()const;
    void setExitCode(int exitCode);
    int exitCode();

    // Getters and setters
    QString program() const;
    void setProgram(const QString &program);

    QStringList arguments() const;
    void setArguments(const QStringList &arguments);

    QString workingDirectory()const;
    void setWorkingDirectory(const QString &workingDirectory);

    QStringList systemEnvironment()const;
    void setSystemEnvironment(const QStringList &systemEnvironment);

    Q_INVOKABLE void closeWriteChannel();
    Q_INVOKABLE void start();
    Q_INVOKABLE void kill();
    Q_INVOKABLE void terminate();

    bool waitForFinished(int msecs = 30000);
    qint64 pid();
    QString message()const;


signals:
    // for QML
    void messageChanged();
    void finished();
    void error();
    void running();
    void inputChannelModeChanged();
    void processStateChanged();
    void processChannelChanged();
    void started();
    void processModeChanged();
    void exitCodeChanged();
    void exitStatusChanged();
    void processErrorChanged();
    void programChanged();
    void argumentsChanged();
    void workingDirectoryChanged();
    void systemEnvironmentChanged();
    void pidChanged();
    void runTypeChanged();
    // INTERNAL
    void newMessage(QString);
    void processStarted();


protected slots:
    void processReadyRead();

    void processFinished(int exitCode);
    void setProcessState(const QProcess::ProcessState processState);
    void setProcessError(const QProcess::ProcessError processError);
    void setExitStatus(int exitCode, const QProcess::ExitStatus &exitStatus);
    void processMessage(const QString &msg);

private:
    QProcess *m_process;
    QProcessEnvironment m_processEnvironment;

    QString m_program;
    QStringList m_arguments;
    QString m_workingDirectory;
    int m_exitCode;
    QStringList m_systemEnvironment;
    qint64 m_pid;
    QString m_message;
    QList<QStringList> m_pendingProcesses;


    QStringList m_runTypes;
    QStringList m_processChannelModeTypes;
    QStringList m_inputChannelModeTypes;
    QStringList m_processChannelTypes;


    ProcessChannelMode m_processMode;
    QProcess::ProcessChannelMode mm_processMode;

    RunType m_runType;

    ExitStatus m_exitStatus;
    QProcess::ExitStatus mm_exitStatus;

    InputChannelMode m_inputChannelMode;
    QProcess::InputChannelMode mm_inputChannelMode;

    ProcessChannel m_processChannel;
    QProcess::ProcessChannel mm_processChannel;

    ProcessChannelMode m_processChannelMode;
    QProcess::ProcessChannelMode mm_processChannelMode;

    ProcessError m_processError;
    QProcess::ProcessError mm_processError;
    QString m_errorString;

    ProcessState m_processState;
    QProcess::ProcessState mm_processState;


    QByteArray m_byteArrayArguments;
    QByteArray m_byteArrayCommand;

};
#endif //QQMLPROCESS_H
