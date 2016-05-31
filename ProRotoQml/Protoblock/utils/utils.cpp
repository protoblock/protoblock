#include "utils/utils.h"
#include <secp256k1/secp256k1.c>

namespace pb {

std::string pb::to_hex(const char *d, uint32_t s) {
    std::string r;
    const char* to_hex="0123456789abcdef";
    uint8_t* c = (uint8_t*)d;
    for( uint32_t i = 0; i < s; ++i )
        (r += to_hex[(c[i]>>4)]) += to_hex[(c[i] &0x0f)];
    return r;
}

uint8_t pb::from_hex(char c) {
    if( c >= '0' && c <= '9' )
        return c - '0';
    if( c >= 'a' && c <= 'f' )
        return c - 'a' + 10;
    if( c >= 'A' && c <= 'F' )
        return c - 'A' + 10;
    //      FC_THROW_EXCEPTION( exception, "Invalid hex character '${c}'", ("c", fc::string(&c,1) ) );
    return 0;
}

size_t pb::from_hex(const std::string &hex_str, char *out_data, size_t out_data_len) {
    std::string::const_iterator i = hex_str.begin();
    uint8_t* out_pos = (uint8_t*)out_data;
    uint8_t* out_end = out_pos + out_data_len;
    while( i != hex_str.end() && out_end != out_pos ) {
        *out_pos = from_hex( *i ) << 4;
        ++i;
        if( i != hex_str.end() )  {
            *out_pos |= from_hex( *i );
            ++i;
        }
        ++out_pos;
    }
    return out_pos - (uint8_t*)out_data;
}

sha256 operator <<(const sha256 &h1, uint32_t i) {
    sha256 result;
    uint8_t* r = (uint8_t*)result.begin();
    uint8_t* s = (uint8_t*)h1.begin();
    for( uint32_t p = 0; p < sizeof(h1.begin())-1; ++p )
        r[p] = s[p] << i | (s[p+1]>>(8-i));
    r[31] = s[31] << i;
    return result;
}

sha256 operator ^ ( const sha256& h1, const sha256& h2 ) {
  sha256 result;
  result.data[0] = h1.data[0] ^ h2.data[0];
  result.data[1] = h1.data[1] ^ h2.data[1];
  result.data[2] = h1.data[2] ^ h2.data[2];
  result.data[3] = h1.data[3] ^ h2.data[3];
  return result;
}
bool operator >= ( const sha256& h1, const sha256& h2 ) {
  return memcmp( h1.data, h2.data, sizeof(h1.data) ) >= 0;
}
bool operator > ( const sha256& h1, const sha256& h2 ) {
  return memcmp( h1.data, h2.data, sizeof(h1.data) ) > 0;
}
bool operator < ( const sha256& h1, const sha256& h2 ) {
  return memcmp( h1.data, h2.data, sizeof(h1.data) ) < 0;
}
bool operator != ( const sha256& h1, const sha256& h2 ) {
  return memcmp( h1.data, h2.data, sizeof(h1.data) ) != 0;
}
bool operator == ( const sha256& h1, const sha256& h2 ) {
  return memcmp( h1.data, h2.data, sizeof(h1.data) ) == 0;
}

std::string pb::sha256::str() const {
    return to_hex( (char*)begin(), 32 );
}


}
