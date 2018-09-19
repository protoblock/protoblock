TARGET = secp256k1
TEMPLATE = lib
QT -= core gui

CONFIG += \
    static \
    warn_off


DEFINES += FIELD=auto
DEFINES += BIGNUM=auto
DEFINES += SCALAR=auto
DEFINES += ENDOMORPHISM=no
DEFINES += STATICPRECOMPUTATION=yes
DEFINES += ASM=no
DEFINES += BUILD=check
DEFINES += ECDH=no
DEFINES += schnorr=no
DEFINES += RECOVERY=no
DEFINES += ENDOMORPHISM=yes
DEFINES += ECDH=yes
DEFINES += EXPERIMENTAL=no




headers.files += $$HEADERS
headers.CONFIG += no_check_exist

win32|win64{
    CONFIG += static
    headers.path = $$INSTALL_PREFIX/$$TARGET/usr/include
    target.path = $$INSTALL_PREFIX/$$TARGET/usr/lib
}
ios{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
macx{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
linux{
    headers.path = $$INSTALL_PREFIX/usr/local/include/$$TARGET
    target.path = $$INSTALL_PREFIX/usr/local/lib
}

INSTALLS += \
    target \
    headers

HEADERS += \
    contrib/lax_der_parsing.h \
    contrib/lax_der_privatekey_parsing.h \
    include/secp256k1.h \
    include/secp256k1_ecdh.h \
    include/secp256k1_recovery.h \
    include/secp256k1_schnorr.h \
    modules/ecdh/main_impl.h \
    modules/ecdh/tests_impl.h \
    modules/recovery/main_impl.h \
    modules/recovery/tests_impl.h \
    modules/schnorr/main_impl.h \
    modules/schnorr/schnorr.h \
    modules/schnorr/schnorr_impl.h \
    modules/schnorr/tests_impl.h \
    basic-config.h \
    bench.h \
    ecdsa.h \
    ecdsa_impl.h \
    eckey.h \
    eckey_impl.h \
    ecmult.h \
    ecmult_const.h \
    ecmult_const_impl.h \
    ecmult_gen.h \
    ecmult_gen_impl.h \
    ecmult_impl.h \
    field.h \
    field_10x26.h \
    field_10x26_impl.h \
    field_5x52.h \
    field_5x52_asm_impl.h \
    field_5x52_impl.h \
    field_5x52_int128_impl.h \
    field_impl.h \
    group.h \
    group_impl.h \
    hash.h \
    hash_impl.h \
    num.h \
    num_gmp.h \
    num_gmp_impl.h \
    num_impl.h \
    scalar.h \
    scalar_4x64.h \
    scalar_4x64_impl.h \
    scalar_8x32.h \
    scalar_8x32_impl.h \
    scalar_impl.h \
    secp256k1.h \
    secp256k1_ecdh.h \
    secp256k1_recovery.h \
    secp256k1_schnorr.h \
    testrand.h \
    testrand_impl.h \
    util.h

SOURCES += \
    contrib/lax_der_parsing.c \
    contrib/lax_der_privatekey_parsing.c \
    bench_ecdh.c \
    bench_internal.c \
    bench_recover.c \
    bench_schnorr_verify.c \
    bench_sign.c \
    bench_verify.c \
    gen_context.c \
    secp256k1.c
