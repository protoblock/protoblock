import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import ProRotoQml.Theme 1.0
import Material 1.0
import Material.ListItems 1.0  as ListItem

Item {
    property string userInFocus
    property string currentWeek
    property string currentPostion

    property string  lastUrl: "NULL"
    property string lastTitle
    property string lastSubText
    property string currPID

    property int  currentLevel: 0
    onCurrentLevelChanged: console.log("currentlevel " + currentLevel)
//    property string  baseUrl: "https://158.222.102.83:4545/"
    property string  baseUrl: "http://protoblock.com/php/simple.php?url=https://158.222.102.83:4545/"
    property string currentFPlayer

    signal modelFull()
    Component.onCompleted: {
        pageHelper.title = "Leaderboard"
        console.log(" on completeed  leaderboard ")
    }
    onModelFull: {

        switch(currentLevel){
        case 0:
            projectionsView.model = null
            projectionsView.model = projModel
            projectionsView.delegate = levelZeroDel
            break;
        case 1 :
            projectionsView.model = null
            projectionsView.model= projModelLevelOne
            projectionsView.delegate = levelOneDel
            break;
        case 2:
            projectionsView.model = null
            projectionsView.model = projModelLevelTwo
            projectionsView.delegate = levelTwoDel
            break;
        }
    }

    Rectangle{
        id: pickers
        color: "transparent"
        width: parent.width / 1.07
        height : (week.height * 1.05) + 10 + (mainTitle.paintedHeight + mainSubText.paintedHeight) + backButton.height
        anchors.horizontalCenter: parent.horizontalCenter
        Column{
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5
            Label{
                id: mainTitle
                font.family: "Roboto"
                font.pixelSize: ProtoScreen.font(ProtoScreen.LARGE)
                width: parent.width
                wrapMode: Text.WordWrap
                text: "2016 Leaderboard"
            }
            Label{
                id: mainSubText
                font.family: "Roboto"
                font.weight: Font.Light
                font.pixelSize: ProtoScreen.font(ProtoScreen.NORMAL)
                width: parent.width
                wrapMode: Text.WordWrap
                text: "Week: " + week.currentText
            }
//            Row{
//                width: parent.width
//                height: pos.height
//                spacing: posLable.paintedWidth / 2
//                Label{
//                    id: posLable
//                    text: "Postion"
//                    height: pos.height
//                    font{
//                        family: "Roboto"
//                        weight: Font.Light
//                        pixelSize: pos.height / 2
//                    }
//                    verticalAlignment: Text.AlignVCenter
//                }
//                ComboBox{
//                    id:  pos
//                    width: (parent.width / 1.07)  - (posLable.paintedWidth + 5 )
//                    model: postionModel
//                    enabled: !jsonGetter.running
//                    onCurrentTextChanged: {
//                        console.log("onCurrentTextChanged " + "pos" + pos.currentText + " week " + week.currentText)
//                        if ( jsonGetter.running ) return
//                        switch(currentLevel){
//                        case 0 :
//                            jsonGetter.source  = baseUrl + "fantasy/leaders?position="
//                                    + replaceSpace(pos.currentText)
//                                    +"&week=" + replaceSpace(week.currentText)
//                            break;
//                        case 1:
//                            jsonGetter.source =  baseUrl +"fantasy/players/" + replaceSpace( currentFPlayer )
//                                    + "/awards?position="
//                                    +  replaceAllAny(  replaceSpace(pos.currentText) )
//                                    + "&week="
//                                    + replaceAllAny( replaceSpace(week.currentText) )
//                            break;
//                        case 2:
//                            break;
//                        }
//                    }
//                }
//            }
            Row{
                width: parent.width
                height: week.height
                spacing: weekLable.paintedWidth / 2
                Label{
                    id: weekLable
                    text: "Week"
                    height: week.height
//                    width: posLable.paintedWidth
                    verticalAlignment: Text.AlignVCenter
                    font{
                        family: "Roboto"
                        weight: Font.Light
                        pixelSize: week.height / 2
                    }
                }


                ComboBox{
                    id:  week
                    width: (parent.width / 1.07)  - (weekLable.paintedWidth + 5 )
                    model: weekModel
                    enabled: !jsonGetter.running && currentLevel != 2
                    onCurrentTextChanged: {
                        if (jsonGetter.running ) return;
                        switch(currentLevel){
                        case 0 :
                            jsonGetter.source  = baseUrl + "fantasy/leaders?week=" + replaceSpace(week.currentText)
                            mainSubText.text = "Week " + week.currentText
                            break;
//                            if (pos.currentText === "all positions" ){
//                                jsonGetter.source  = baseUrl + "fantasy/leaders?week=" + replaceSpace(currentText)
//                            }else{
//                                jsonGetter.source  = baseUrl + "fantasy/leaders?position="
//                                        +replaceSpace(pos.currentText) +"&week="
//                                        + replaceSpace(week.currentText)
//                            }
//                            break;
                        case 1:
                            jsonGetter.source =  baseUrl +"fantasy/players/" + replaceSpace( currentFPlayer )
//                                    + "/awards?position="
//                                    +  replaceAllAny(  replaceSpace(pos.currentText) )
                                    + "/awards?week="
                                    + replaceAllAny( replaceSpace(week.currentText) )
                            break;
                        case 2:
                            break;
                        }
                    }
                }
            }
            // later
            TextField{
                id: searchBar
                width: parent.width
                height: 0
//                placeholderText: "Search Projections"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button{
                id: backButton
                width: parent.width
                text: "back"
                elevation: 1
                visible: true//(currentLevel > 0 ? true : false)
                enabled: !jsonGetter.running && currentLevel > 0
                height: week.height * 2
                onClicked: {
                    currentLevel = currentLevel -1
                    if(currentLevel === 0 ){
                        mainTitle.text = "2016 Leaderboard"
                        mainSubText.text = (week.currentText == "all weeks" ? "2016" : "Week " + week.currentText )
                        jsonGetter.source =  baseUrl + "fantasy/leaders?week=" + replaceSpace(week.currentText)
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
        addDisplaced:Transition{
            ParallelAnimation{
                NumberAnimation{duration: 600;from: 0 ; to : 1 ;  easing.type: Easing.OutQuint}
                NumberAnimation{duration: 1000; from: 0 ; to: 1 ;easing.type: Easing.OutQuint}
            }
        }
        remove: Transition{
            ParallelAnimation{
                NumberAnimation{duration: 600;from: 1 ; to: 0; easing.type: Easing.OutQuint}
                NumberAnimation{duration: 1000; from: 1 ; to: 0 ;easing.type: Easing.OutQuint}
            }
        }
        anchors{
            horizontalCenter: parent.horizontalCenter
            top:  pickers.bottom
            topMargin:  10
        }
        delegate: levelZeroDel
    }

    // the main model that gets filled
    ListModel{id: projModel}
    ListModel{id: projModelLevelOne}
    ListModel{id: projModelLevelTwo}

    // used to run the xhr request to get the json =
    BaserModel {
        id: jsonGetter
        //        onSourceChanged: console.log(source)
        source:  baseUrl + "fantasy/leaders"
        onUpdated:{
            fillProjectionsModel(json)
            currentPage = "Leaderboard"
        }
    }

    // run against all the levels and fill the models
    function   fillProjectionsModel(d){
        var list = JSON.parse(d)
        switch(currentLevel){
        case 0:
            if(list.contents.length !== undefined ){
                console.log('refilling ' + list.contents.length)
                projModel.clear()
                for (var i in  list.contents ) {
                    projModel.append({ 'name' : list.contents[i].name  ,'score' : list.contents[i].score });
                }
                modelFull()
            }else{
//                console.log("Error " + list.contents.length)
            }
                break;
        case 1:
            if (list.contents.data.length !== undefined){
                projModelLevelOne.clear();
                for (var ii in  list.contents.data ) {
                projModelLevelOne.append({
                                             'playerId' : list.contents.data[ii].PLAYERID , 'firstName' : list.contents.data[ii].FIRSTNAME
                                             ,'lastName' : list.contents.data[ii].LASTNAME , 'team' : list.contents.data[ii].TEAM
                                             ,'pos' : list.contents.data[ii].POS , 'result' : list.contents.data[ii].RESULT
                                             ,'award' : list.contents.data[ii].AWARD , 'projection' : list.contents.data[ii].PROJECTION
                                             ,'fantasyName' : list.contents.data[ii].FANTASYNAME , 'week' : list.contents.data[ii].WEEK
                                         });
            }
            modelFull()
            }else{
                console.log("Error " + list.contents.data.length)
            }
            break;
        case 2:
            projModelLevelTwo.clear()
            for (var iii in  list.contents.data ) {
                projModelLevelTwo.append({
                                             'firstName' : list.contents.data[iii].FIRST , 'lastName' : list.contents.data[iii].LAST
                                             , 'team' : list.contents.data[iii].TEAM , 'pos' : list.contents.data[iii].POS
                                             , 'result' : list.contents.data[iii].RESULT , 'award' : list.contents.data[iii].AWARD
                                             , 'projection' : list.contents.data[iii].PROJECTION , 'fantasyName' : list.contents.data[iii].FANTASYNAME
                                             , 'week' : list.contents.data[iii].WEEK
                                         });
            }
            modelFull()
            break;
        }
    }

    function resetPickers(){
//        pos.currentIndex=0
        week.currentIndex=0
    }

    function replaceAllAny(str){
        var k = str.replace("positions","position")
        var t = k.replace("weeks","week")
        return t.replace("all","any")
    }

    function replaceSpace(str){
        var ret = str.replace(/\s/g, "%20");
        return ret;
    }

    Component{
        id: levelZeroDel
        ListItem.Subtitled {
            backgroundColor: MiddleMan.isMyName(model.name) ?
                                 (model.name !== uname ? Theme.light.textColor : themeroot.theme.accentColor )
                               : (elevation > 0 ? "white" : "transparent")

            textColor: MiddleMan.isMyName(model.name) ?
                       (model.name !== uname ? themeroot.theme.accentColor : Theme.light.textColor)
                     : Theme.light.textColor

            text: "Fantasy Name: " + model.name

            subText: (week.currentText == "all weeks" ? "2016" : "Week " + week.currentText ) + " Balance: " +  model.score

            subColor: MiddleMan.isMyName(model.name) ?
                       (model.name !== uname ? themeroot.theme.accentColor : Theme.light.textColor)
                     : Theme.light.textColor

            elevation: 2
            action: Image {
                source : "qrc:/icons/action_account_circle.png"
                width: ProtoScreen.guToPx(4)
                height:  width
            }
            onClicked:{
                lastUrl = jsonGetter.source
                lastSubText = mainSubText.text
                lastTitle = mainTitle.text

                mainTitle.text = text//"Fantasy Name: "+ model.name
                mainSubText.text = subText//"Balance: " + model.score
                currentFPlayer = model.name
//                resetPickers()
                jsonGetter.source =  baseUrl  + "fantasy/players/"+model.name+"/awards?week=" + replaceAllAny( replaceSpace(week.currentText) )
                currentLevel = 1
            }
        }
    }

    Component{
        id: levelOneDel
        //FIXME add a team icon
        ListItem.Subtitled {

            text: model.firstName +" " + model.lastName +  " ("+ model.pos + ") - Week " + model.week
            subText: "Projection: " + model.projection + " Result: " + model.result
            elevation: 2
            valueText: "Award: " + model.award
            // FIXME set this as a platform.os android
            action: Image {
             source: "qrc:/" + model.team+".PNG"
                width: ProtoScreen.guToPx(4)
                height:  width
            }


            onClicked:{
                lastUrl = jsonGetter.source
                lastSubText = mainSubText.text
                lastTitle = mainTitle.text

                mainTitle.text = "Player Name : "+model.firstName +" " + model.lastName +  " ("+ model.pos + ") " + model.team
                mainSubText.text = "Week " + model.week + " - Fantasy Points: " + model.result
                currPID = model.playerId
                jsonGetter.source = baseUrl + "fantasy/nfl/" + model.playerId + "/week/" + model.week
                currentLevel = 2
            }
        }
    }

    Component{
        id: levelTwoDel
        ListItem.Subtitled {
            backgroundColor: MiddleMan.isMyName(model.fantasyName) ?
                                 (model.fantasyName !== uname ? Theme.light.textColor : themeroot.theme.accentColor )
                               : (elevation > 0 ? "white" : "transparent")

            textColor: MiddleMan.isMyName(model.fantasyName) ?
                       (model.fantasyName !== uname ? themeroot.theme.accentColor : Theme.light.textColor)
                     : Theme.light.textColor

            text: model.fantasyName
            subText: "Projection: " + model.projection + " Result: " + model.result

            subColor: MiddleMan.isMyName(model.fantasyName) ?
                       (model.fantasyName !== uname ? themeroot.theme.accentColor : Theme.light.textColor)
                     : Theme.light.textColor

            valueText: "Award: " + model.award
            valueColor: MiddleMan.isMyName(model.fantasyName) ?
                       (model.fantasyName !== uname ? themeroot.theme.accentColor : Theme.light.textColor)
                     : Theme.light.textColor

            elevation: 2
            action: Image {
                source: "qrc:/icons/action_account_circle.png"
                width: ProtoScreen.guToPx(4)
                height:  width
            }
            onClicked:{
            }
        }
    }
    ProgressCircle{
        anchors.centerIn: parent
        visible:  jsonGetter.running ? true : false
    }
}
