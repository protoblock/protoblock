TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    $$PWD/QmlModelsAndMacros/QmlModelsAndMacros.pro \
    $$PWD/GenerateProto/GenerateProto.pro \
    $$PWD/libsecp256k1/libsecp256k1.pro \
    $$PWD/libprotoblock-core/libprotoblock-core.pro \
    $$PWD/libstatemodel/libstatemodel.pro \
    $$PWD/ProtoblockQml/ProtoblockQml.pro
