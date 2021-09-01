# Protoblock 
This repository contains the open source protoblock blockchain, fantasybit token and proof-of-skill consensus code. 

[Webpage](https://fantasybit.com)
[Proof-of-Skill - Medium](https://medium.com/@jaybny/on-proof-of-skill-6af149f45ce8)

QML/C++ desktop client for Protoblock blockchain full node + Fantasybit wallet 

![Desktop](docs/trading.PNG)

## Dependencies

Qt 5.10.0 with at least the following modules is required:
 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
 * [qtgraphicaleffects](http://code.qt.io/cgit/qt/qtgraphicaleffects.git)
 * [qtsvg](http://code.qt.io/cgit/qt/qtsvg.git)
 * [qtmultimedia](http://code.qt.io/cgit/qt/qtmultimedia.git)
 * [qtdoc](http://code.qt.io/cgit/qt/qtdoc.git)
 * [qtwebsockets](http://code.qt.io/cgit/qt/qtwebsockets.git)
 * [qtxmlpatterns](http://code.qt.io/cgit/qt/qtxmlpatterns.git/)

libs
 * protobuf 2.5
 * leveldb 1.18 
 * openssl 

### Building on MacOS/Windows
1. Install Qt5, using their official installer.
1. get and build libs - or get binaries from [winodws](https://github.com/protoblock/protoblock/tree/win64-libs/libs) or [osx](https://github.com/protoblock/protoblock/tree/osx-libs/libs) branch. 
1. update libcommuni submodule 
1. open project plugins project [ProRotoQml.pro](share/ProRotoQml/ProRotoQml.pro)
1. setup kits 
1. add make install 
1. build ProRotoQml plugins 
1. open apps project [apps.pro](apps/apps.pro)
1. compile protoblockd
1. compile protoblock-qt
1. run protoblock-qt 

Design Docs
-----------
Design decisions and discussions documented here: [docs](docs/README.md)

## Getting involved

Telgram is the preferred means of getting in touch with the developers.
* [t.me/protoblock](https://t.me/protoblock)
* [Email us - contact@protoblock.com][dev-email]
* [home page][web-home]
* [download][web-download]
* [twitter][twitter-main]
* [twitter-ticker][twitter-feed]
* [reddit][reddit]
* [get coins][coins]
* [skillSale](https://github.com/SatoshiFantasy/--skillSale/releases)

<<>> THE PROTOBLOCK DEVS <<>>

```
88""Yb 88""Yb  dP"Yb  888888  dP"Yb  88""Yb 88      dP"Yb   dP""b8 88  dP
88__dP 88__dP dP   Yb   88   dP   Yb 88__dP 88     dP   Yb dP   `" 88odP
88"""  88"Yb  Yb   dP   88   Yb   dP 88""Yb 88  .o Yb   dP Yb      88"Yb
88     88  Yb  YbodP    88    YbodP  88oodP 88ood8  YbodP   YboodP 88  Yb
```

[web-home]: https://fantasybit.com
[web-download]: https://fantasybit.com/downloads
[twitter-main]: https://twitter.com/protoblock
[twitter-feed]: https://twitter.com/prototicker
[reddit]: https://www.reddit.com/r/Protoblock/
[coins]: http://fantasybit.com/coins/
[dev-email]: mailto:contact@protoblock.com
