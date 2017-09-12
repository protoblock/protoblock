##############
## Globals
##############

## Write to SQL for dataagent - and testing protoblock

contains(DEFINES, DATAAGENTDEFS){
    DEFINES += SQL
    DEFINES += SQLSTUFF
    DEFINES += WRITESYMBOLS
    DEFINES += TIMEAGENTWRITEFILLS
    DEFINES += DATAAGENTWRITENAMES
    DEFINES += DATAAGENTWRITEPROFIT
}


contains(DEFINES, SQLFORCE){
    DEFINES += SQL
    DEFINES += SQLSTUFF
    DEFINES += WRITESYMBOLS
    DEFINES += TIMEAGENTWRITEFILLS
    DEFINES += DATAAGENTWRITENAMES
    DEFINES += DATAAGENTWRITEPROFIT
    DEFINES += WRITESYMBOLS_FORCE

    DEFINES += TIMEAGENTWRITEFILLS_FORCE
    DEFINES += TIMEAGENTWRITEFILLS_FORCE_NOTICKS
    DEFINES += DATAAGENTWRITENAMES_FORCE
    DEFINES += DATAAGENTWRITENAMES_FORCE_NONAMES
}

#DEFINES += PLAYERLOADERFD

#DEFINES += WRITESYMBOLS_FORCE
#DEFINES += WRITESYMBOLS_FORCE_INITALL
#DEFINES += WRITESYMBOLS_FORCE_SPECIAL
#DEFINES += TIMEAGENTWRITEFILLS_FORCE
#DEFINES += TIMEAGENTWRITEFILLS_FORCE_NOTICKS
#DEFINES += DATAAGENTWRITENAMES_FORCE
#DEFINES += DATAAGENTWRITENAMES_FORCE_NONAMES
#DEFINES += DATAAGENTWRITENAMES_SPECIAL

## Tweet Fills
contains( DEFINES, WSSERVER ) {
DEFINES += TIMEAGENTWRITETWEETS
DEFINES += LIGHT_CLIENT_ONLY
}

## Startup

DEFINES += USE_LOCAL_GENESIS
DEFINES += NO_DOSPECIALRESULTS
DEFINES += NO_REMOVEALL_TRADING
DEFINES += START2017WITH2014


#DEFINES += NOUSE_GENESIS_BOOT
#DEFINES += NOCHECK_LOCAL_BOOTSTRAP
#DEFINES += NOCHECK_LOCAL_BOOTSTRAP_MINUS1

#DEFINES += CHECKPOINTS
#DEFINES += CHECKPOINT_2100

## sync
#DEFINES += BLOCK_STEP
#DEFINES += NOSYNC
#DEFINES += STOP_HEIGHT_TEST

## Build
DEFINES += PRODFOOTBALL
DEFINES += BUILD_STABLE
DEFINES += ALLOW_DEBUG

## log
DEFINES += TRACE
#DEFINES += TRACE4
#DEFINES += TRACEDEBUG

## Features
DEFINES += TRADE_FEATURE
#DEFINES += NO_SQL_LEADERS
DEFINES += JAYHACK
#DEFINES += NO_EXITONLY
#DEFINES += FORCE_LIVE

##Testing - Internal
#DEFINES += WRITE_BOOTSTRAP
#DEFINES += BLOCK_EXPLORER_WRITE_FILLS
#DEFINES += DATAAGENTGUIJay2015PrePreSeasonTestDemo
#DEFINES += Jay2015PrePreSeasonTestDemo
#DEFINES += TESTING_PRE_ROW_TRADE_FEATURE
#DEFINES += CUSTOM_TESTING_PATH
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
