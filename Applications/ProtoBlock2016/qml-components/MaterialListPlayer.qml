import QtQuick 2.3
import Material 1.0
import Material.ListItems 1.0 as ListItem
import ProRotoQml.Theme 1.0
//use sub
ListItem.Subtitled {
    id: playerListItem
    signal clicked()
    elevation: 1
    property string photoUrl
    action: Image {
        id: playerIcon
        // get icon source from fantasydata
        source: photoUrl
        width: ProtoScreen.guToPx(4)
        height:  width
    }
    onClicked:{
        playerListItem.clicked()
    }
}
