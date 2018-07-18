import QtQuick 2.0

Item {


   LocalJsonCaller{
        id: jsonGetter
        source: "qrc:/sc.json"
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
