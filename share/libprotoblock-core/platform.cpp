#include "platform.h"
#include "appsettings.h"
#include <QDir>
#include <QStandardPaths>
#include <QSysInfo>
#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>

Platform::Platform(){}

Platform::~Platform(){}
/*!
 * \brief Platform::createDefaultIOSDirs
 * \return
 * returns the true if all default files and folders have been created.
 *
 * If it returns false then something went wrong and the application will not act correctly
 */

bool Platform::createDefaultIOSDirs(){
    int counter = 1;
//    if ( systemInfo.productType () == "ios" ){
        QString sDir = QCoreApplication::applicationDirPath ();
        // log file
        QDir mmmDir(sDir.append ("/logs/"));
        if (!mmmDir.exists () ){
            if (mmmDir.mkpath (sDir) ){
                // write our 1st log file;
                counter = ++counter;
                QFile logFile(sDir.append ("/protoblock.log"));
                if ( !logFile.exists () ){
                    if (logFile.open (QFile::ReadWrite | QFile::Text)){
                        QDateTime dateTime = QDateTime::currentDateTime();
                        QTextStream logOut(&logFile);
                        logOut << dateTime.toString () << "\tInit Log File";
                    }else{
                        counter = 1;
                    }
                }
            }
            else
            {
                counter = 1;
                qDebug() << "could not make the log file directory";
            }
        }

        QString aDir = QCoreApplication::applicationDirPath ();
        QDir mDir(aDir.append ("/storage"));
        if (!mDir.exists()){
            counter = 0;
            if( mDir.mkpath(aDir) ){
                // write this to log ? maybe make the secert3 here ? ;
                ++counter;
            }else{
                //write this to error log;
                counter = 1;
            }
        }



        QString tDir = QCoreApplication::applicationDirPath ();
        //Because this dir is not there that means that we must move the files that were installed on the ios.  IOS is real stupid with install paths.
        QDir mmDir(tDir.append ("/bootstrap/"));
        if( ! mmDir.exists () ){
            if(mmDir.mkpath ( tDir ) )
            {
                QFile bFiles;
                // Add to log file that the bootstrap has been created;
                counter = ++counter;
                // move all the default files over to the correct area;
                QList<QString> bootstrapFiles;
                bootstrapFiles.push_back ("/FantasyNameBal.txt");
                bootstrapFiles.push_back ("/GameData.txt");
                bootstrapFiles.push_back ("/GameResults.txt");
                bootstrapFiles.push_back ("/GenesisTransition-Tr-Transaction.txt");
                bootstrapFiles.push_back ("/Global;State.txt");
                bootstrapFiles.push_back ("/PlayerData.txt");
                bootstrapFiles.push_back ("/WeeklySchedule.txt");

                for (int i = 0 ; i < bootstrapFiles.size (); ++i ){
                    if ( bFiles.copy ( bootstrapFiles.at(i), tDir + bootstrapFiles.at(i)) )
                    {
                        // write this to the log file
//                          logOut << bootstrapFiles.at (i) << " was moved and "
                    }
                    else
                    {
                        // write that there was a error copying the files over the corrct place
                        counter = 1;
                    }
                }

                if (counter != 1 ){
                    // now that we have copyied the files over its time to remove all the old ones.
                    for (int i = 0 ; i < bootstrapFiles.size (); ++i ){
                        bFiles.setFileName (bootstrapFiles.at(i));
                        bFiles.remove();
                    }
                }
            }
            else
            {
                counter = 1;
            }
        }


        if(counter != 1 ){
            return true;
        }else{
            return  false;
        }
        //    }
}

void Platform::createIosDir(const QString &dir)
{
    QDir d(dir);
    if(!d.exists ()){
        d.mkpath (dir);
    }

}


// FIXME why is this not a object or a dereferenced point ?
AppSettings * Platform::settings() {
    return AppSettings::instance();
}




//|| systemInfo.productType () == "osx"
//FIXME this is stupid and should be set in a way different way
std::string Platform::getRootDir() {
    if ( systemInfo.productType () == "ios"
         || systemInfo.productType () == "android" )
    {

       QString pRet = QStandardPaths::standardLocations (QStandardPaths::AppDataLocation).value (0);
       createIosDir (pRet);
       pRet.append ("/");
       QDir::setCurrent(pRet);
       qDebug() << "savePath " << pRet;
       return  pRet.toStdString ();
    }
//    else if (systemInfo.productType () == "osx" || systemInfo.productType () == "macos")
//    {
//        QString pRet = QStandardPaths::standardLocations (QStandardPaths::HomeLocation).first ();
//         pRet.append ("/Library/Application Support/ProtoBlock2016/");
//        createIosDir (pRet);
//         return pRet.toStdString ();
//    }
    else
    {
        QString qstrPath = AppSettings::instance()->
                getSetting(AppSettings::ApplicationStorageDir).toString();
        createIosDir (qstrPath);
        return qstrPath.toStdString();
    }
}
