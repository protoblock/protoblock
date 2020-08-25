include ($$PWD/../defs.pri)
##############
## Globals
##############
## Startup
#DEFINES += NOUSE_GENESIS_BOOT
#DEFINES += NOCHECK_LOCAL_BOOTSTRAP
DEFINES += USE_LOCAL_GENESIS
#DEFINES += BLK18

## sync
#DEFINES += BLOCK_STEP
#DEFINES += NOSYNC
#DEFINES += STOP_HEIGHT_TEST

## Build
DEFINES += PRODFOOTBALL
DEFINES += BUILD_STABLE

## log
#DEFINES += TRACE
#DEFINES += ALLOW_DEBUG

## Features
DEFINES += TRADE_FEATURE

##Testing - Internal
#DEFINES += FULL_NODE_CLIENT
#DEFINES += USE_PB_GATEWAYS
#DEFINES += CLEAN_BLOCKS
#DEFINES += BLOCK_EXPLORER

## Light Client
#DEFINES += LIGHT_CLIENT_ONLY
#DEFINES += USE_LOCALHOST_SERVER
#DEFINES += USE_LOCALNETWORKHOST_SERVER

## secp256k1
DEFINES += USE_NUM_NONE
DEFINES += USE_FIELD_10X26
DEFINES += USE_FIELD_INV_BUILTIN
DEFINES += USE_SCALAR_8X32
DEFINES += USE_SCALAR_INV_BUILTIN

