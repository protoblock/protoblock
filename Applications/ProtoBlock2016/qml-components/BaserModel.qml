import QtQuick 2.0

Item {
    id: baseModel
    property string source
    property string json
    property int status: XMLHttpRequest.UNSENT
    property int xhrStatus
    signal updated()
    property bool running: true


    onSourceChanged: {
        if (source  === "1" || source  === ""){
            console.log("onSourceChanged source " + source)
            running = false
        }
        else {
            var xhr = new XMLHttpRequest;
            running = true
            xhr.onreadystatechange = function() {
                status = xhr.readyState;
                if ( xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200 ){
                        console.log("Success : "  +  xhr.readyState
                                    + "\nstatus int: " + xhr.status
                                    )
                        json = xhr.responseText;
                        running = false
                        updated();

                    }else {
                        console.log("Failed: " +  xhr.readyState
                                       + "\nstatus int: " + xhr.status
                                    + " text " + xhr.responseText
                                       )
                        running = false
                   }
                }
            }
            console.log(" getting " + source)
            xhr.open("GET", source );
            xhr.send();
        }
    }
}
