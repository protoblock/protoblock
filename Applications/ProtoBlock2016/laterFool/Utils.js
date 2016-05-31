function getTeamIcon(name){
    var n = name.toString()
    var t = n.toLowerCase();
    var tt = "qrc:/" + t + ".png"
    return tt
}
var weeks = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16']
// later
var awardData = []
var awardLable =  []
var awardProj = []

var awardProjedction , awardAward;

var dounutLineData = [{ value: awardProjedction, color: "#F7464A" }, { value: awardAward, color: theme.primaryColor }]

var ChartLineData = {
    labels: ["January","February","March","April","May","June","July"],
    datasets: [
        {
            fillColor: theme.primaryColor ,
            strokeColor: "transparent",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#ffffff",
            data: awardData
        },
        {
            fillColor: "green" ,
            strokeColor: "transparent",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#ffffff",
            data: awardProj
        }
    ]
}

var ChartDoughnutData =
        [{ value: awardAward, color: Theme.primaryColor }, { value: awardProjedction, color: "#E2EAE9" } ]


function sumitup(mod, del){
    var sum = 0;
    for( var i = 0; i < mod.count; i++ ){
        sum += parseInt( model.get(i).del , 10 );
    }
    var avg = sum / mod.count;;
    console.log( "The sum : " + sum + " The average: " + avg );
}
