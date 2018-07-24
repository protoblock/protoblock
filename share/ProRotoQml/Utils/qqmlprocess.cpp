#include "qqmlprocess.h"


QQmlProcess::QQmlProcess(QObject *parent) :
    QObject(parent),
    m_process(new QProcess(this)),
    m_runType(Attached),
    m_processChannelMode(MergedChannels),
    m_processState(NotRunning)
{

    connect(this,SIGNAL(newMessage(QString)),this,SLOT(processMessage(QString)));
    connect(m_process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(setProcessError(QProcess::ProcessError)));
    connect(m_process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(setProcessState(QProcess::ProcessState)));
    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(setExitStatus(int,QProcess::ExitStatus)));
    connect(m_process,SIGNAL(readyRead()),this,SLOT(processReadyRead()));
//    connect(m_process,SIGNAL(started()),this,SLOT(processStarted()));
    connect(m_process,SIGNAL(finished(int)),this,SLOT(processFinished(int)));

    setrunTypes();
    setInputChannelModeTypes();
    setProcessChannelTypes();
    setProcessChannelModeTypes();

}

QQmlProcess::~QQmlProcess()
{
    if(m_process->state() == QProcess::Running )
    {
        m_process->close();
        m_process->kill();
        delete m_process;
    }
    delete m_process;
}

void QQmlProcess::setRunType(const QQmlProcess::RunType &runType)
{
    if(m_runType == runType){
        return;
    }else {
        switch (runType) {
        case Attached:
            m_runType = Attached;
            break;
        case Detaacted:
            m_runType = Detaacted;
            break;
        }
        emit runTypeChanged();
    }

}

QQmlProcess::RunType QQmlProcess::runType() const
{
    return m_runType;
}

QStringList QQmlProcess::runTypes()
{
    return m_runTypes;
}

void QQmlProcess::setrunTypes()
{
    m_runTypes.clear();
    QList<QString> li;
    li << "Attached" <<"Detaacted";
    m_runTypes << li;
}

void QQmlProcess::setInputChannelMode(const QQmlProcess::InputChannelMode &inputChannelMode)
{
    if(m_inputChannelMode == inputChannelMode){
        return;
    }else{
        switch (inputChannelMode) {
        case ManagedInputChannel:
            m_inputChannelMode = ManagedInputChannel;
            mm_inputChannelMode = QProcess::ManagedInputChannel;
            break;
        case ForwardedInputChannel:
            m_inputChannelMode = ForwardedInputChannel;
            mm_inputChannelMode = QProcess::ForwardedInputChannel;
            break;
        }
        emit inputChannelModeChanged();
    }
}

QQmlProcess::InputChannelMode QQmlProcess::inputChannelMode() const
{
    return m_inputChannelMode;
}

QStringList QQmlProcess::inputChannelModeTypes()
{
    return m_inputChannelModeTypes;
}

void QQmlProcess::setInputChannelModeTypes()
{
    m_inputChannelModeTypes.clear();
    QList<QString> li;
    li << "ManagedInputChannel" <<  "ForwardedInputChannel";
    m_inputChannelModeTypes << li;
}

QQmlProcess::ProcessChannel QQmlProcess::processChannel() const
{
    return m_processChannel;
}

void QQmlProcess::setProcessChannel(const QQmlProcess::ProcessChannel &processChannel)
{
    if(m_processChannel == processChannel){
        return;
    }else {
        switch (processChannel) {
        case StandardError:
            m_processChannel = StandardError;
            //            mm_processChannel = QProcess::StandardError;
            break;
        case StandardOutput:
            m_processChannel = StandardOutput;
            //            mm_processChannel = QProcess::StandardOutput;
            break;
        case StanderedAll:
            m_processChannel = StanderedAll;
            break;
        }
        emit processChannelChanged();
    }
}

QStringList QQmlProcess::processChannelTypes()
{
    return m_processChannelTypes;
}

void QQmlProcess::setProcessChannelTypes()
{
    m_processChannelModeTypes.clear();
    QList<QString> li;
    li <<  "StandardOutput" << "StandardError" << "StanderedAll";
    m_processChannelTypes << li;
}


QQmlProcess::ProcessState QQmlProcess::processState() const
{
    return m_processState;
}

QQmlProcess::ProcessChannelMode QQmlProcess::processMode() const
{
    return m_processMode;
}

void QQmlProcess::setProcessMode(QQmlProcess::ProcessChannelMode &processMode)
{
    if(m_processMode == processMode){
        return;
    }else{
        switch (processMode) {
        case SeparateChannels:
            m_processMode = SeparateChannels ;
            mm_processChannelMode = QProcess::SeparateChannels ;
            break;
        case MergedChannels:
            m_processMode = MergedChannels;
            mm_processChannelMode = QProcess::MergedChannels ;
            break;
        case ForwardedChannels:
            m_processMode = ForwardedChannels;
            mm_processChannelMode = QProcess::ForwardedChannels ;
            break;
        case ForwardedErrorChannel:
            m_processMode = ForwardedErrorChannel;
            mm_processChannelMode = QProcess::ForwardedErrorChannel ;
            break;
        case ForwardedOutputChannel:
            m_processMode = ForwardedOutputChannel;
            mm_processChannelMode = QProcess::ForwardedOutputChannel ;
            break;
        }
        emit processModeChanged();
    }

}

QStringList QQmlProcess::processChannelModeTypes()
{
    return m_processChannelModeTypes;
}

void QQmlProcess::setProcessChannelModeTypes()
{
    m_processChannelModeTypes.clear();
    QList<QString> li;
    li <<  "SeparateChannels" << "MergedChannels"<< "ForwardedChannels"<< "ForwardedErrorChannel" <<"ForwardedOutputChannel";
    m_processChannelModeTypes << li;
}

QQmlProcess::ExitStatus QQmlProcess::exitStatus() const
{
    return m_exitStatus;
}

void QQmlProcess::setExitCode(int exitCode)
{
    if(m_exitCode == exitCode)
        return;
    m_exitCode = exitCode;
    emit exitCodeChanged();
}

int QQmlProcess::exitCode()
{
    return m_exitCode;
}

QString QQmlProcess::program() const
{
    return m_program;
}

void QQmlProcess::setProgram(const QString &program)
{
    if(m_program == program)
        return;
    m_program = program;
    emit programChanged();
}

QStringList QQmlProcess::arguments() const
{
    return m_arguments;
}

void QQmlProcess::setArguments(const QStringList &arguments)
{
    if(m_arguments == arguments)
        return;
    m_arguments = arguments;


//    /*m_byteArrayArguments*/;


    emit argumentsChanged();
}

QString QQmlProcess::workingDirectory() const
{
    return m_workingDirectory;
}

void QQmlProcess::setWorkingDirectory(const QString &workingDirectory)
{
    if(m_workingDirectory == workingDirectory)
        return;
    m_workingDirectory = workingDirectory;
    emit workingDirectoryChanged();
}

QStringList QQmlProcess::systemEnvironment() const
{
    return m_systemEnvironment;
}

void QQmlProcess::setSystemEnvironment(const QStringList &systemEnvironment)
{
    if(m_systemEnvironment == systemEnvironment){
        return;
    }else{
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString envHelper;
        for (int i = 0 ; i < systemEnvironment.length() ;i++)
        {
            envHelper.append(systemEnvironment.at(i)).append(",");
        }
//        env.insert(envHelper);
//        m_process.setProcessEnvironment(env);
        emit systemEnvironmentChanged();
    }
}

void QQmlProcess::closeWriteChannel()
{
    m_process->closeWriteChannel();
}

////
/// FIXME take the command and the argument and make into a byte array then take that again and make beck into latin1 here
/// This way there is better ways of concatig commands and also escape chars ect
///
void QQmlProcess::start()
{

    if(m_program.length() == 0){
        setErrorString("You Must add a program to run");
        return;
    }
    else
    {
        if (m_runType == Attached)
        {
            if(m_arguments.length() <= 0 )
            {
                m_process->start(m_program);
                processStarted();
                //m_id = m_process->pid();
            }
            else if (m_arguments.length() > 0)
            {
                m_process->start(m_program,m_arguments);
                processStarted();
                //m_id = m_process->pid();
            }
            else
            {
                setErrorString("Not sure what to do");
            }
        }
        else if (m_runType == Detaacted)
        {
            if(m_arguments.length() <= 0 && m_workingDirectory.length() <= 0)
            {
                m_process->startDetached(m_program);
                processStarted();
                //m_id = m_process->pid();
            }
            else if (m_arguments.length() > 0 && m_workingDirectory.length() <= 0)
            {
                m_process->startDetached(m_program,m_arguments);
                processStarted();
                //m_id = m_process->pid();
            }
            else if(m_arguments.length() > 0 && m_workingDirectory.length() > 0)
            {
                m_process->startDetached(m_program,m_arguments,m_workingDirectory);
                processStarted();
                //m_id = m_process->pid();
            }
            else{
                setErrorString("Not sure what to do");
            }

        }
    }
}

void QQmlProcess::kill()
{
    m_process->kill();
}

void QQmlProcess::terminate()
{
    m_process->terminate();
}

qint64 QQmlProcess::pid()
{
    return m_pid;
}

QString QQmlProcess::message() const
{
    return m_message;
}

void QQmlProcess::processReadyRead()
{
    QString standeredErr = QString::fromLatin1(m_process->readAllStandardError());
    QString standeredOut = QString::fromLatin1(m_process->readAllStandardOutput());
    QString standeredAll = QString::fromLatin1(m_process->readAll());
    if(m_processChannel == StandardError)
    {
        if (!standeredErr.isEmpty()) {
            newMessage(standeredErr);
        }
    }
    else if(m_processChannel == StandardOutput)
    {
        if (!standeredOut.isEmpty()) {
            newMessage(standeredOut);
        }
    }
    else if (m_processChannel == StanderedAll)
    {
        if(!standeredAll.isEmpty()){
            newMessage(standeredAll);
        }
    }
}

void QQmlProcess::processFinished(int exitCode)
{
//    qDebug() << exitCode;

    if (exitCode != 0) {
        setErrorString(QString(QLatin1String(m_process->readAllStandardError())));
        m_pendingProcesses.clear();
        return;
    }

    QString errorMsg = QString::fromLatin1(m_process->readAllStandardError());

    if (errorMsg.trimmed().length()>0)
        setErrorString(errorMsg);

    QString msg = QString::fromLatin1(m_process->readAllStandardOutput());
    if (msg.trimmed().length()>0)
        newMessage(msg);
}



void QQmlProcess::setExitStatus(int exitCode, const QProcess::ExitStatus &exitStatus)
{
    setExitCode(exitCode);
    if (mm_exitStatus == exitStatus){
        return;
    }else{
        switch (exitStatus) {
        case QProcess::NormalExit:
            m_exitCode = NormalExit;
            break;
        case QProcess::CrashExit:
            m_exitStatus = CrashExit;
        }
        emit exitStatusChanged();
    }
}

void QQmlProcess::processMessage(const QString &msg)
{
    if(m_message == msg)
        return;
    m_message = msg;
    emit messageChanged();
}

void QQmlProcess::setProcessState(const QProcess::ProcessState processState)
{
    if(mm_processState == processState){
        return;
    }else{
        switch (processState) {
        case QProcess::NotRunning:
            m_processState = NotRunning;
            break;
        case QProcess::Starting:
            m_processState = Starting;
            emit started();
            break;
        case QProcess::Running:
            m_processState = Running;
            emit running();
            break;
        }
        emit processStateChanged();
    }
}

QQmlProcess::ProcessError QQmlProcess::processError() const
{
    return m_processError;
}

void QQmlProcess::setProcessError(const QProcess::ProcessError processError)
{
    if(mm_processError == processError){
        return;
    }else{
        switch (processError) {
        case QProcess::FailedToStart:
            m_processError = FailedToStart;
            setErrorString("FailedToStart Error");
            break;
        case QProcess::Crashed:
            m_processError = Crashed;
            setErrorString("Crased Error");
            break;
        case QProcess::Timedout:
            m_processError = Timedout;
            setErrorString("TimeOut Error");
            break;
        case QProcess::WriteError:
            m_processError = WriteError;
            setErrorString("WriteError");
            break;
        case QProcess::ReadError:
            m_processError = ReadError;
            setErrorString("ReadError");
        case QProcess::UnknownError:
            m_processError = UnknownError;
            setErrorString("Unknown Error");
            break;
        }
        emit processErrorChanged();
    }
}
void QQmlProcess::setErrorString(const QString errorString)
{
    if(m_errorString == errorString)
        return;
    m_errorString = errorString;
    emit error();
}

QString QQmlProcess::errorString() const
{
    return m_errorString;
}





