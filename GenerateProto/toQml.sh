#!/bin/bash 



docker run -it -v $(pwd):/opt/protobuf-qml nsuke/protobuf-qml -I. \
    --qml_out=/Users/satoshi/work/tmp/ProRoto2016/GenerateProto/generated/ \
    /Users/satoshi/work/tmp/ProRoto2016/GenerateProto/ApiData.proto
