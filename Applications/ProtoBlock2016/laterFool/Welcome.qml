import QtQuick 2.5
import ProRotoQml.Sql 1.0

Item {
    Text {
        id: te
    }


    // get fred jackson
    QmlSqlQuery{
        id: queryDB
        connectionName: "proroto"
        queryString: "SELECT * FROM tfprod_srinfo;"
        onErrorStringChanged: {console.log("ERRR " +errorString);}
        onDone:  {
            te.text += lastQueryOutPut
        }
    }
    QmlSqlDatabase{
        id: sqlDatabase
        connectionName: "proroto"
        databaseDriver:  QmlSqlDatabase.SQLight
        databaseName: "/Users/satoshi/work/tmp/proroto.db"
        Component.onCompleted: addDataBase()
        onConnectionOpened: {
            queryDB.exec()
        }
    }
}
