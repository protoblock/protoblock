import QtQuick 2.0
import QtQuick.Controls 1.4

Item {


    TabView{
        id: tView
        width: parent.width
        height: parent.height


        Tab{
            title: "Projections Level one"
            TableView{
                width: parent.width - leaderboard.width
                id: projectionsTable

            }


            TableView{
                id: leaderboard
                width: parent.width - projectionsTable.width


            }

        }
        Tab{
            title: "Trading Level two"

        }




    }






}
