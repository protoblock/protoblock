import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
//import ProRotoQml.Models 1.0
import Material 1.0
import Material.ListItems 1.0  as ListItem

Item {
    property int  currentLevel: 0

    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"
    property string  lastUrl: "NULL"
    property string lastTitle
    property string lastSubText

    property string currentFPlayer
    signal modelFull()
    Component.onCompleted: pageHelper.title = "2015 Final Trading"
    onModelFull: {
        switch(currentLevel){
        case 0:
            projectionsView.model = null
            projectionsView.model = pnlModel
            projectionsView.delegate = pnlZeroDel
            break;
        case 1 :
            projectionsView.model = null
            projectionsView.model= pnlLevelOne
            projectionsView.delegate = pnlOneDel
            break;
        case 2:
            projectionsView.model = null
            projectionsView.model = pnlLevelTwo
            projectionsView.delegate = pnlTwoDel
            break;
        }
    }

    Rectangle{
        id: pickers
        color: "transparent"
        width: parent.width / 1.07
        height : (10 + backButton.height) + (mainTitle.paintedHeight + mainSubText.paintedHeight)
        anchors.horizontalCenter: parent.horizontalCenter
        Column{
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 9
            Label{
                id: mainTitle
                font.family: "Roboto"
                font.pixelSize: Qt.platform.os === "android" ? 64 :  34
                width: parent.width
                wrapMode: Text.WordWrap
            }
            Label{
                id: mainSubText
                font.family: "Roboto"
                font.weight: Font.Light
                font.pixelSize: Qt.platform.os === "android" ? 32 :  24
                width: parent.width
                wrapMode: Text.WordWrap
            }
            //later
            TextField{
                id: searchBar
                width: parent.width
//                placeholderText: "Search Projections"
                anchors.horizontalCenter: parent.horizontalCenter
                height: 0
            }
            Button{
                id: backButton
                width: parent.width
                text: "back"
                elevation: 1
                visible: currentLevel > 0 ? true : false
                height: currentLevel === 0 ? 0 : rootLoader.height / 10
                onClicked: {
                    currentLevel = currentLevel -1
                    if(currentLevel === 0 ){
//                        console.log("default level")
                        mainTitle.text = ""
                        mainSubText.text = ""
                        jsonGetter.source =  baseUrl + "/trading/leaders"
                    }else{
                        mainTitle.text = lastTitle
                        mainSubText.text = lastSubText
                        jsonGetter.source = lastUrl
                    }
                }
            }
        }
    }

    ListView{
        id: projectionsView
        width: parent.width / 1.07
        height: parent.height - ( pickers.height + 10 )
        clip: true
        spacing: 5
        opacity: jsonGetter.running ?  0: 1
        Behavior on opacity{NumberAnimation{duration: 600; easing.type: Easing.OutQuint}}
        anchors{
            horizontalCenter: parent.horizontalCenter
            top:  pickers.bottom
            topMargin:  10
        }
        delegate: pnlZeroDel
    }

    // the main model that gets filled
    ListModel{id: pnlModel}
    ListModel{id: pnlLevelOne}
    ListModel{id: pnlLevelTwo}

    // used to run the xhr request to get the json =
    BaserModel {
        id: jsonGetter
//        onSourceChanged: console.log(source)
        source:  baseUrl + "/trading/leaders"
        onUpdated:  fillProjectionsModel(json)
    }

    // run against all the levels and fill the models
    function   fillProjectionsModel(d){
        var list = JSON.parse(d)

        switch(currentLevel){
        case 0:
            if(list.contents.length !== undefined ){
//                console.log('refilling ' + list.contents.length)
                pnlModel.clear()
                for (var i in  list.contents ) {
                    pnlModel.append({ 'name' : list.contents[i].name ,'score' : list.contents[i].score });
                }
                modelFull()
            }else{
//                console.log("Error " + list.contents.length)
            }
                break;
        case 1:
            if (list.contents.data.length !== undefined){
                pnlLevelOne.clear();
                for (var ii in  list.contents.data ) {
                pnlLevelOne.append({
                                             'playerId' : list.contents.data[ii].PLAYERID , 'firstName' : list.contents.data[ii].FIRSTNAME
                                             ,'lastName' : list.contents.data[ii].LASTNAME , 'team' : list.contents.data[ii].TEAM
                                             ,'pos' : list.contents.data[ii].POS , 'result' : list.contents.data[ii].RESULT
                                             ,'pnl' : list.contents.data[ii].PNL , 'qty' : list.contents.data[ii].QTY
                                             ,'fantasyName' : list.contents.data[ii].FANTASYNAME , 'week' : list.contents.data[ii].WEEK
                                             ,'price' : list.contents.data[ii].PRICE
                                         });
            }
            modelFull()
            }else{
//                console.log("Error " + list.contents.data.length)
            }
            break;
        case 2:
            for (var iii in  list.contents.data ) {
                pnlLevelTwo.append({
                                             'firstName' : list.contents.data[iii].FIRST , 'lastName' : list.contents.data[iii].LAST
                                             , 'team' : list.contents.data[iii].TEAM , 'pos' : list.contents.data[iii].POS
                                             , 'buyer' : list.contents.data[iii].FANTASYNAMEBUYER
                                             , 'seller' : list.contents.data[iii].FANTASYNAMESELLER
                                             , 'qty' : list.contents.data[iii].QTY , 'price' : list.contents.data[iii].PRICE
                                             , 'week' : list.contents.data[iii].WEEK , 'isABuy' : list.contents.data[iii].ISABUY
                                         });
            }
            modelFull()
            break;
        }
    }


    Component{
        id: pnlZeroDel
        ListItem.Subtitled {
            text: "Fantasy Name: " + model.name
            subText: "Balance: " +  model.score
            elevation: 2
            action: Image {
                source: "qrc:/icons/action_account_circle.png"
                width: 32
                height:  width
            }
            onClicked:{
                lastUrl = jsonGetter.source
                lastSubText = mainSubText.text
                lastTitle = mainTitle.text

                mainTitle.text = "Fantasy Name: "+ model.name
                mainSubText.text ="Balance: " + model.score
                currentFPlayer = model.name

                jsonGetter.source =  baseUrl  + "trading/leaders/"+model.name+"/pnl"
                currentLevel = 1
            }
        }
    }

    Component{
        id: pnlOneDel
        //FIXME add a team icon
        ListItem.Subtitled {
            text: model.firstName +" " + model.lastName +  " ("+ model.pos + ") " + model.team
            subText:  "Trade: " + model.qty + " @ " + model.price +  " Profit: "+ model.pnl
            elevation: 2
            valueText: "Week: " + model.week
            action: Image {
                source: "qrc:/" + model.team+".PNG"
                width:  32
                height:  width
            }
            onClicked:{
                lastUrl = jsonGetter.source
                lastSubText = mainSubText.text
                lastTitle = mainTitle.text

                mainTitle.text = "Players Name : "+model.firstName +" " + model.lastName +  " ("+ model.pos + ") " + model.team
                mainSubText.text ="Week: " + model.week + " Results: " + model.result

                jsonGetter.source = baseUrl + "trading/fills/" + model.playerId + "/week/" + model.week
                currentLevel = 2
            }
        }
    }

    Component{
        id: pnlTwoDel
        ListItem.Subtitled {
            text:{
                if (model.isABuy === "1" ){
                    model.buyer  + " <b>Bought from</b> " +  model.seller
                }else{
                    model.seller + " <b>Sold to</b> " + model.buyer
                }
            }
            subText: model.qty + " @ " + model.price
            elevation: 2
            action: Image {
                source: "qrc:/icons/action_account_circle.png"
                width: 32
                height:  width
            }
            onClicked:{
            }
        }
    }
    ProgressCircle{
        anchors.centerIn: parent
        visible:   jsonGetter.running ? true : false
    }
}
