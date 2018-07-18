import QtQuick 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
import ProRotoQml.Sql 1.0

Item {

ListView{
    width: parent.width
    height: parent.height
    clip: true
    interactive: true
    model: sLandingModel
    delegate: ListItems.Subtitled{
        elevation: 2
        text: model.FullName + " " + model.Conference + " (" + model.Division+ ")"
        subText: "Head coach: " + model.HeadCoach
                 + " Offensive Coordinator: " +model.OffensiveCoordinator
                 +"("+ model.OffensiveScheme
                 + ") DefensiveCoordinator: " + model.DefensiveCoordinator
                 + " ("+model.DefensiveScheme+")"
        action: Image{
            height: parent.height
            width: height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/"+ model.Key+ ".PNG"
        }
        onClicked: {
            currentTeamInFocus = model.Key
            currentPage = "SeasonLongLevelTwo"
        }
    }
}

QmlSqlQueryModel{
    id: sLandingModel
    connectionName: "protoblock"
    queryString: "SELECT * FROM Teams"
    Component.onCompleted: exec()
}

}
