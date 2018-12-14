#ifndef PROTOBLOCK_UTILS_H
#define PROTOBLOCK_UTILS_H

#include <QDebug>
#include <string>

#include <secp256k1/hash_impl.h>
#include <secp256k1/secp256k1.h>

#include <google/protobuf/message.h>

#include <city.hpp>
#include <base58.h>
#include <genericsingleton.h>
#include <openssl/ripemd.h>
#include <sstream>
#include <iomanip>
#include <fbutils.h>

namespace pb {


    inline bool is_big_endian( )
    {
       union
       {
          uint32_t i;
          char c[ 4 ];
       } bint = { 0x01020304 };

       return bint.c[ 0 ] == 1;
    }

    inline uint32_t fix_endian( uint32_t x )
    {
       if( is_big_endian( ) )
          return x;
       else
          return ( ( ( x & 0x000000ff ) << 24 ) | ( ( x & 0x0000ff00 ) << 8 )
           | ( ( x & 0x00ff0000 ) >> 8 ) | ( ( x & 0xff000000 ) >> 24 ) );
    }

    inline uint64_t fix_endian( uint64_t x )
    {
       if( !is_big_endian( ) )
       {
          x = ( x & 0x00000000ffffffff ) << 32 | ( x & 0xffffffff00000000 ) >> 32;
          x = ( x & 0x0000ffff0000ffff ) << 16 | ( x & 0xffff0000ffff0000 ) >> 16;
          x = ( x & 0x00ff00ff00ff00ff ) << 8 | ( x & 0xff00ff00ff00ff00 ) >> 8;
       }

       return x;
    }

    inline std::string toReverseHexFromDecimal(uint32_t  t) {
        std::ostringstream is;
        is << std::hex << std::setw( 8 ) << std::setfill( '0' ) << fix_endian(t);
        return is.str();
    }

    inline std::string toReverseHexFromDecimal(uint64_t t) {
        std::ostringstream is;
        is << std::hex << std::setw( 16 ) << std::setfill( '0' ) << fix_endian(t);
        return is.str();
    }


    uint8_t from_hex( char c );

    std::string to_hex( const unsigned char* d, uint32_t s );

    std::string to_hex( const std::string &in );

    class TheCTX : public GenericSingleton<TheCTX> {
        friend class GenericSingleton<TheCTX>;
        TheCTX(){
            mCTX = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
        }

        secp256k1_context *mCTX;
    public:
        secp256k1_context *CTX() {
            return mCTX;
        }
    };

    size_t from_hex( const std::string& hex_str, char* out_data, size_t out_data_len );

    typedef secp256k1_ecdsa_signature signature;

    struct sha256 {
        sha256() {
            memset(begin(),0,32);
        }

        sha256( const std::string& hex_str ) {
          from_hex( hex_str, (char*)begin(), 32 );
        }

        operator std::string()const;

        std::string str()const;

        std::string reversestr()const;
//        sha256::operator string()const { return  str(); }
        sha256(const sha256 &other) {
            memcpy(begin(),other.begin(),32);
        }
        unsigned char data[32];
        unsigned char * begin() {  return &data[0]; }
        const unsigned char * begin() const {  return &data[0]; }

        friend sha256 operator << ( const sha256& h1, uint32_t i       );
        friend bool   operator == ( const sha256& h1, const sha256& h2 );
        friend bool   operator != ( const sha256& h1, const sha256& h2 );
        friend sha256 operator ^  ( const sha256& h1, const sha256& h2 );
        friend bool   operator >= ( const sha256& h1, const sha256& h2 );
        friend bool   operator >  ( const sha256& h1, const sha256& h2 );
        friend bool   operator <  ( const sha256& h1, const sha256& h2 );

    };



    struct public_key_data {
        public_key_data(const public_key_data &other) {
            memcpy(key_data,other.key_data,33);
        }
        public_key_data(char indata[33]) {
            memcpy(key_data,indata,33);
        }

        public_key_data() {}
        unsigned char key_data[33];
        unsigned char * begin() {  return &key_data[0]; }
        const unsigned char * begin() const {  return &key_data[0]; }


    };

    inline bool operator==( const public_key_data& a, const public_key_data& b ) {
        return memcmp(a.key_data,b.key_data,33) == 0;
    }

    struct public_key {
        public_key(const secp256k1_pubkey &in ) : key(in) {
        }

        public_key(const public_key_data &in ) : key{} {
//            qDebug() << "public_key(const public_key_data &in ) before ";
            auto ret = secp256k1_ec_pubkey_parse(pb::TheCTX::instance()->CTX(),&key,in.key_data,33);
//            qDebug() << ret << "public_key(const public_key_data &in ) after ";
        }

        public_key_data serialize() const {
            pb::public_key_data retpk;
            size_t pubkeyclen = 33;
            secp256k1_ec_pubkey_serialize(pb::TheCTX::instance()->CTX(), retpk.key_data, &pubkeyclen, &key, SECP256K1_EC_COMPRESSED);
            return retpk;
        }

        bool verify(const sha256 &msg32, const secp256k1_ecdsa_signature *sig) {
            return verify(sig,msg32.data, &key);
        }

        bool verify(const sha256 &msg32, const signature &sig) {
            return verify(&sig, msg32.data, &key);
        }

        static bool verify(const secp256k1_ecdsa_signature *sig,
                           const unsigned char *msg32,
                           const secp256k1_pubkey *pubkey) {
            return secp256k1_ecdsa_verify(pb::TheCTX::instance()->CTX(), sig, msg32, pubkey) == 1;
        }


        secp256k1_pubkey key;
    };

    struct secp256k1_privkey {

        secp256k1_privkey() {}
        secp256k1_privkey(const sha256 &in) : _secret{in} , key_data{_secret.begin()} {}

        secp256k1_privkey(const secp256k1_privkey &pk ) {
          _secret = pk.get_secret();
          key_data = _secret.begin();
       }

        pb::sha256 _secret;
        unsigned char *key_data;

        secp256k1_ecdsa_signature sign(const std::string &in) {
            secp256k1_ecdsa_signature sig;
            if ( 1 != secp256k1_ecdsa_sign(pb::TheCTX::instance()->CTX(), &sig, (const unsigned char*)in.data(), key_data, NULL, NULL) ) {
                qDebug() << " error sig";
            }
            return sig;
        }

        secp256k1_ecdsa_signature sign(const pb::sha256 &in) {
            secp256k1_ecdsa_signature sig;
            if ( 1 != secp256k1_ecdsa_sign(pb::TheCTX::instance()->CTX(),
                                           &sig, in.data, key_data, NULL, NULL) ) {
                qDebug() << " error sig";
            }
            return sig;
        }


         unsigned char * data() const {
             return key_data;
         }

         sha256 get_secret() const {
             return _secret;
         }

         public_key get_public_key() const {
             secp256k1_pubkey pubkey;
             int ret = secp256k1_ec_pubkey_create(pb::TheCTX::instance()->CTX(), &pubkey, key_data);
             return public_key(pubkey);
         }

         static secp256k1_privkey regenerate(const sha256 &secret ) {
             return secp256k1_privkey(secret);
         }

         secp256k1_privkey operator = ( const secp256k1_privkey &pk ) {
            _secret = pk.get_secret();
            key_data = _secret.begin();
            return *this;
         }

    };

    inline void hashc(const unsigned  char *in,size_t size,unsigned char *out32) {
        secp256k1_sha256_t hasher;
        secp256k1_sha256_initialize(&hasher);
        secp256k1_sha256_write(&hasher, in, size);
        secp256k1_sha256_finalize(&hasher, out32);
    }


    inline std::string serialize_der(const secp256k1_ecdsa_signature &in) {
        std::vector <char> buffer(100);
        size_t size = buffer.size ();
        secp256k1_ecdsa_signature_serialize_der(pb::TheCTX::instance ()->CTX (),(unsigned char*)buffer.data (),&size,&in);
        std::string ret;
        ret.assign (buffer.data (),size);
        return ret;
    }

    inline secp256k1_ecdsa_signature  parse_der(const unsigned char *input, size_t inputlen) {
        secp256k1_ecdsa_signature sig;
        auto ret = secp256k1_ecdsa_signature_parse_der(pb::TheCTX::instance()->CTX(),&sig,input,inputlen);
        return sig;
    }

    inline secp256k1_ecdsa_signature signature_normalize(const secp256k1_ecdsa_signature &insig) {
        secp256k1_ecdsa_signature sig;

        if ( secp256k1_ecdsa_signature_normalize(pb::TheCTX::instance()->CTX(), &sig, &insig ) == 1 )
             return sig;
        else
            return insig;
    }


    inline pb::sha256 hashit(const std::string &in) {
        pb::sha256 ret;
        const unsigned char* uc = (const unsigned char*)in.c_str();
//        hashc(uc, in.size(), (unsigned char*)&ret.front());
        hashc(uc, in.size(), ret.data);
        return ret;
    }


    // Fuck we need to rebuild protobuf as _public
    inline pb::sha256 hashit(const GOOGLE_NAMESPACE::protobuf::Message  &in) {
        return hashit(in.SerializeAsString());
    }

    inline pb::sha256 hashfromhex(const std::string &in) {
        int size = (in.size()/2) + 1;
        std::vector<char> buffer(size);
        size = from_hex(in,&buffer[0],buffer.size());
        pb::sha256 ret;
        hashc((const unsigned char*)&buffer[0], size, ret.data);
        return ret;
    }

    inline pb::sha256 hashit(const pb::sha256 &in) {
        pb::sha256 ret;
        hashc(in.data,sizeof(in.data),ret.data);
        return ret;
    }

    static std::string toBtcAddress(const public_key_data &in ) {
        pb::sha256 ret;
        hashc(in.key_data, 33, ret.data);
        qDebug() << ret.str ().data ();
        unsigned char hash2data[25];
        unsigned char *hash2 = hash2data;//new unsigned char[25];

        RIPEMD160(ret.data, sizeof(ret.data), (unsigned char*)&hash2[1]);
        hash2[0] = fantasybit::BTC_NETWORK;
        hashc(hash2, 21, ret.data);
        pb::sha256  ret2x;
        hashc(ret.data,sizeof(ret.data),ret2x.data);
        memcpy((unsigned char*)&hash2[21],ret2x.begin (),4);
        std::string   temp =  to_hex(hash2,25);

//       temp;
//        temp.assign((char *)hash2,25);

        qDebug() << " temp string " << temp.data ();
        return EncodeBase58(hash2,hash2+25);
    }

//    static std::string toRedeemScript(const public_key_data &in ) {
//        pb::sha256 ret;
//        hashc(in.key_data, 33, ret.data);
//        qDebug() << ret.str ().data ();
//        unsigned char hash2data[20];
//        unsigned char *hash2 = hash2data;//new unsigned char[25];

//        RIPEMD160(ret.data, sizeof(ret.data), (unsigned char*)&hash2[0]);
//        std::string   temp =  to_hex(hash2,20);
//        return temp;

//    }

    static std::string fromBtcAddress(const std::string &in ) {
        std::vector<unsigned char> out;
        DecodeBase58 (in,out);
        return to_hex(((unsigned char*)out.data())+1,out.size ()-5);
    }

    static std::string to_base58( const public_key_data &in ) {
        return EncodeBase58( in.key_data, in.key_data+33);
    }

    static std::string to_base58( const char* d, size_t s ) {
        return EncodeBase58( (const unsigned char*)d, (const unsigned char*)d+s ).c_str();
    }

    static std::vector<char> from_base58( const std::string& base58_str ) {
       std::vector<unsigned char> out;
       if( !DecodeBase58( base58_str.c_str(), out ) ) {
            qDebug() << "Unable to decode base58 string ${base58_str}" << base58_str.data();
       }
       return std::vector<char>((const char*)out.data(), ((const char*)out.data())+out.size() );
    }

    /**
     *  @return the number of bytes decoded
     */
    static size_t from_base58( const std::string& base58_str, char* out_data, size_t out_data_len ) {
      //slog( "%s", base58_str.c_str() );
      std::vector<unsigned char> out;
      if( !DecodeBase58( base58_str.c_str(), out ) ) {
          qDebug() << "Unable to decode base58 string ${base58_str}" << base58_str.data();
      }

      memcpy( out_data, out.data(), out.size() );
      return out.size();
    }
}

namespace std {
//        template<typename T> struct hash;

    template<>
    struct hash<pb::public_key_data>
    {
       size_t operator()( const pb::public_key_data& e )const {
           return  pb::city_hash64((char *)e.begin(), 33);
       }
    };
}

#endif // PROTOBLOCK_UTILS_H
