var merkel_Is_Drilable

function getTypeFromText(strings) {

    var messageValue
    switch (strings){
    case "fnbalmetaroot":
        messageValue = "MerkleTree"
        break;
    case "teamstatemid":
        messageValue = "MerkleTree"
        break;
    case "projstateid":
        messageValue = "MerkleTree"
        break;
    case "leaderboardstateid":
        messageValue = "LeaderBoardMeta"
    break;

    case "txmetaid":
        messageValue = "TxMeta"
        break;
    case 'trmetaid':
        messageValue = "TrMeta";
        break;
    case 'globalstateid':
        messageValue = "GlobalStateMeta";
    break;
    case 'pbstateid':
        messageValue = "pbstate";
    break;
    case "schedulestateid":
        messageValue = "MerkleTree";
   break;
    case "teamid":
        messageValue = "MerkleTree";
        break;
    case 'datametaroot':
        messageValue = "MerkleTree"
        break;
    case 'marketstateid':
        messageValue = "MerkleTree"
    break;
    case "marketticmetaid":
            messageValue = 'MarketTicMeta'
        break;
    case "playermetaidroot":
        messageValue = "MerkleTree";
        break;
//    case "limitbookmetaid":
//        messageValue = "MerkleTree"
//        break;
    case "askordermetaroot":
        messageValue = "MerkleTree"
        break;
    case "bidordermetaroot":
        messageValue = "MerkleTree"
    break;
    case "ingameprojmetaroot":
        messageValue = "MerkleTree"
        break;
    case "gameresultmetaroot":
        messageValue = "MerkleTree"
        break;
    case "opengamestatusroot":
        messageValue = "MerkleTree"
        break;
    case "homeresultmeta":
        messageValue = "TeamResultMeta"
        break;
    case "playerresultmetaroot":
    messageValue = "MerkleTree"
        break;
    case "resultdatametaid":
    messageValue = "DataMeta";
    break;
    case "awayresultmeta":
    messageValue = "TeamResultMeta"
    break;
    case "orderstateid":
        messageValue = "MerkleTree"
    break;
    case 'awardmetaroot' :
        messageValue = "MerkleTree"
    break;
    case "pnlmetaroot":
        messageValue = "MerkleTree"
    break;
    case "awardmetaplayerroot" :
        messageValue = "MerkleTree"
        break;
    case "posstateid" :
        messageValue = "MerkleTree"
        break;
    case "orderfillmetaid" :
        messageValue = "MerkleTree"
        break;
    case "limitbookmetaid" :
        messageValue = "limitbook"
        break;
    case "fantasynamestateid" :
        messageValue = "MerkleTree"
        break;
    case "projmetaroot":
       messageValue = "MerkleTree";
        break;
    case "ordersmetaroot" :
        messageValue = "MerkleTree"
        break;
    case "playergamestatsid" :
        messageValue = "PlayerGameStats"
        break;
    default:
        messageValue = strings
    break;
    }
    return messageValue
}
///////////////////////////
//
//
//
//
//
//

function getMerkleFromText(string){
    var merkelHelper;
    switch(string){
    case "schedulestateid":
        merkelHelper = "WeekGameStatusMeta"
        break;
    case "gameresultmetaroot":
        merkelHelper = "GameResultsMeta"
        break;
    case "ingameprojmetaroot":
        merkelHelper =  "InGameProjMeta"
        break;
    case "opengamestatusroot" :
        merkelHelper =  "GameStatusMeta"
        break;
    case "playerresultmetaroot" :
        merkelHelper = "PlayerResultMeta"
        break;
    case "awardmetaplayerroot" :
        merkelHelper = "AwardMeta"
        break;
    case "pnlmetaplayerroot" :
        merkelHelper = ""
        break;
    case "fnbalmetaroot" :
        merkelHelper = "FantasyNameBalMeta"
        break;
    case "awardmetaroot" :
        merkelHelper = "AwardMeta"
        break;
    case "pnlmetaroot" :
        merkelHelper = "PnlMeta"
        break;
    case "posstateid" :
        merkelHelper = "PosMeta"
        break;
    case "orderstateid" :
        merkelHelper = "OrderMeta"
        break;
    case "orderfillmetaid" :
        merkelHelper = "OrderFillMeta"
        break;
    case "marketstateid" :
        merkelHelper = "PlayerMarketState"
        break;
    case "bidordermetaroot" :
        merkelHelper = "OrderMeta"
        break;
    case "askordermetaroot":
        merkelHelper  = "OrderMeta"
        break;
    case  "datametaroot":
        merkelHelper = "DataMeta"
        break;
    case "teamstatemid" :
        merkelHelper = "TeamMeta"
        break;
    case "playermetaidroot" :
        merkelHelper = "PlayerMeta"
        break;
    case  "fantasynamestateid":
        merkelHelper = "FantasyNameState"
        break;
    case "projmetaroot" :
        merkelHelper = "ProjMeta"
        break;
    case "ordersmetaroot" :
       merkelHelper = "OrderMeta"
        break;
    default:
        merkelHelper = string
        break;
    }
        console.log(merkelHelper)
        return merkelHelper;

}



// this function will drill everything
function bruteDrill(hash){
    for (var i = 0 ; i < stye.count;  i++){

    }
}

function getAllL(arLeaves, keyNames){
    isLeaves = true;

    for (var i = 0 ; i <= arLeaves.length ; i ++){
            if (i === 0) {
                drillModel.clear()
            }
            if(i === arLeaves.length ){
                isLeaves = false
                console.log("isLeaves is changeing back ")
            }

        var k = arLeaves[i]

        var tt = k.replace(/"/g, "");
        LdbHashReaderTool.getDebugStr( tt , keyNames )
        tArea.append("Leave Id: "+ tt)
        tArea.append('Leave Number: ' + i)
    }
}
