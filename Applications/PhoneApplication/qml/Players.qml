import QtQuick 2.0
import Material 1.0
import Material.ListItems 1.0 as ListItems
Item {


    ListView{
        id: projectionsView
        width: parent.width / 1.07
        height: parent.height
        clip: true
        spacing: 5
        model: sportsRadarModel //schModel
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: jsonGetter.running ?  0: 1
        Behavior on opacity{NumberAnimation{duration: 600; easing.type: Easing.OutQuint}}
        anchors{
            horizontalCenter: parent.horizontalCenter
        }
        delegate: pnlZeroDel
    }



    Component{
        id: pnlZeroDel
        ListItems.Subtitled {
            text: model.week + " " +  model.homeTeam + " vs " + model.awayTeam
            subText: stadiumDetails + " "+  model.city + " " + model.state
            elevation: 2
            height: rootLoader.height / 6
            // FIXME set this as a platform.os android
            itemLabel.font.pixelSize: 48
            itemSubLabel.font.pixelSize: 32
            action: Image {
                source:{
                    var t = model.homeTeam.replace("JAX","JAC")
                    t.replace("LA","STL")
                    var at = model.awayTeam.replace("JAX","JAC")
                    at.replace("LA","STL")
                    "qrc:/teams/"+t+".PNG"
                }
                width: Qt.platform.os === "android" ? 64 : 32
                height:  width
            }
            onClicked:{
                console.log(model.awayTeam)
            }
        }
    }



    SportsRadarApiCaller{
        id: sportsRadarGetter
        source: "http://api.sportradar.us/nfl-t1/2016/REG/schedule.json?api_key="+ apiKey
        onUpdated: fillSportsRadarWeekModel(json)
    }
    ListModel{id:sportsRadarModel}
    function fillSportsRadarWeekModel(d){
//        var jsonS = JSON.stringify(d)
        var li = JSON.parse(d.games);
        console.log(li)
        for (var i in li) {
                console.log(li[i].season.weeks.number)
//                console.log(li, li[i])
//                traverse(li[i] );
            }
//console.log( li[1].weeks.number     )
//        for(var i =0 ; li.length ; i++){
//            console.log( li[i].season     )// .number)
//            sportsRadarModel.append({
//                                        "week": list[i].weeks.number
//                                        ,"awayTeam" : list[i].week.games.away
//                                        ,"homeTeam" : list[i].week.games.home
//                                        ,"stadiumDetails" : list[i].week.games.venue.name
//                                        ,"city" : list[i].week.games.venue.city
//                                        ,"state" : list[i].week.games.venue.state
//                                        , "playingSurface" : list[i].week.games.venue.surface
//                                    })
//        }
    }


    FantasyDataApiCaller{
        id: jsonGetter
//        source: "https://api.fantasydata.net/nfl/v2/JSON/Schedules/2016"
        onUpdated: fillWeekModel(json)
    }
    ListModel{id: schModel}
    function fillWeekModel(d){
        var list = JSON.parse(d);
        for(var i in d){
            schModel.append({
                            "homeTeam": list[i].HomeTeam
                            ,"awayTeam" : list[i].AwayTeam
                            , "week" : list[i].Week
                            ,"date": list[i].Date
                            ,"stadiumDetails": list[i].StadiumDetails.Name
                            ,"city" : list[i].StadiumDetails.City
                            ,"state" : list[i].StadiumDetails.State
                            , "playingSurface" : list[i].StadiumDetails.PlayingSurface
                            ,"capacity" : list[i].StadiumDetails.Capacity
                            , "forecastTempLow":   list[i].StadiumDetails.ForecastTempLow
                            ,"forecastTempHigh":   list[i].StadiumDetails.ForecastTempHigh
                            ,"forecastDescription" : list[i].StadiumDetails.ForecastDescription
                            ,"forecastWindChill" : list[i].StadiumDetails.ForecastWindChill
                            ,"forecastWindSpeed" :  list[i].StadiumDetails.ForecastWindSpeed
                            , "overUnder" : list[i].StadiumDetails.OverUnder
                            , "pointSpread" : list[i].StadiumDetails.PointSpread
                            ,"channel" : list[i].Channel
                            })
        }
    }
}
