var weeks = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16']

var currentHomeTeam
var currentAwayTeam

function resetPickers(){
    pos.currentIndex=0
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

// run against all the levels and fill the models
function   fillProjectionsModel(d){
    var list = JSON.parse(d)
    switch(currentLevel){
    case 0:
        if(list.contents.length !== undefined ){
            console.log('refilling ' + list.contents.length)
            projModel.clear()
            for (var i in  list.contents ) {
                projModel.append({ 'name' : list.contents[i].name ,'score' : list.contents[i].score });
            }
            modelFull()
        }else{
            console.log("Error " + list.contents.length)
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
