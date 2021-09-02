import QtQuick 2.12

Item {
    id: baseModel
    property string source
    property string html
    property int status: XMLHttpRequest.UNSENT
    property int xhrStatus
    signal updated()
    property bool running: false
    onSourceChanged: {
        console.log("source")
        if (source  === "1" || source  === ""){
            running = false
        }
        else
        {
        var xhr = new XMLHttpRequest;
          running = true
            xhr.open("GET", source );
            xhr.onreadystatechange = function() {
                status = xhr.readyState;
                if (xhr.readyState === 4  && xhr.status === 200){
                    html = xhr.responseText;
                    running = false
                    updated();
                }else {
                }
            }
            xhr.send();
        }
    }
}
