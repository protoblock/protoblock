import QtQuick 2.4

ListModel {
    id: posModel
    Component.onCompleted: createModel()
    function createModel(){
        var pos = ["QB","RB","TE","WR","K","DEF"];
        for (var i = 0 ; i <pos.length; i ++){
            posModel.append({"text":  pos[i]})
        }
    }
}
