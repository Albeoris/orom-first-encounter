#include <kaitai/kaitaistream.h>

#if defined(__APPLE__)
#include <machine/endian.h>
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)
#define __BYTE_ORDER    BYTE_ORDER
#define __BIG_ENDIAN    BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#elif defined(_MSC_VER) // !__APPLE__
#include <stdlib.h>
#define __LITTLE_ENDIAN     1234
#define __BIG_ENDIAN        4321
#define __BYTE_ORDER        __LITTLE_ENDIAN
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)
#else // !__APPLE__ or !_MSC_VER
#include <endian.h>
#include <byteswap.h>
#endif

#include <iostream>
#include <vector>
#include <stdexcept>

kaitai::kstream::kstream(std::istream* io) {
    m_io = io;
    init();
}

kaitai::kstream::kstream(std::string& data): m_io_str(data) {
    m_io = &m_io_str;
    init();
}

void kaitai::kstream::init() {
    exceptions_enable();
    align_to_byte();
}

void kaitai::kstream::close() {
    //  m_io->close();
}

void kaitai::kstream::exceptions_enable() const {
    m_io->exceptions(
        std::istream::eofbit |
        std::istream::failbit |
        std::istream::badbit
    );
}

// ========================================================================
// Stream positioning
// ========================================================================

bool kaitai::kstream::is_eof() const {
    if (m_bits_left > 0) {
        return false;
    }
    char t;
    m_io->exceptions(
        std::istream::badbit
    );
    m_io->get(t);
    if (m_io->eof()) {
        m_io->clear();
        exceptions_enable();
        return true;
    } else {
        m_io->unget();
        exceptions_enable();
        return false;
    }
}

void kaitai::kstream::seek(uint64_t pos) {
    m_io->seekg(pos);
}

uint64_t kaitai::kstream::pos() {
    return m_io->tellg();
}

uint64_t kaitai::kstream::size() {
    std::iostream::pos_type cur_pos = m_io->tellg();
    m_io->seekg(0, std::ios::end);
    std::iostream::pos_type len = m_io->tellg();
    m_io->seekg(cur_pos);
    return len;
}

// ========================================================================
// Integer numbers
// ========================================================================

// ------------------------------------------------------------------------
// Signed
// ------------------------------------------------------------------------

int8_t kaitai::kstream::read_s1() {
    char t;
    m_io->get(t);
    return t;
}

// ........................................................................
// Big-endian
// ........................................................................

int16_t kaitai::kstream::read_s2be() {
    int16_t t;
    m_io->read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

int32_t kaitai::kstream::read_s4be() {
    int32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

int64_t kaitai::kstream::read_s8be() {
    int64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}

// ........................................................................
// Little-endian
// ........................................................................

int16_t kaitai::kstream::read_s2le() {
    int16_t t;
    m_io->read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

int32_t kaitai::kstream::read_s4le() {
    int32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

int64_t kaitai::kstream::read_s8le() {
    int64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}

// ------------------------------------------------------------------------
// Unsigned
// ------------------------------------------------------------------------

uint8_t kaitai::kstream::read_u1() {
    char t;
    m_io->get(t);
    return t;
}

// ........................................................................
// Big-endian
// ........................................................................

uint16_t kaitai::kstream::read_u2be() {
    uint16_t t;
    m_io->read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

uint32_t kaitai::kstream::read_u4be() {
    uint32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

uint64_t kaitai::kstream::read_u8be() {
    uint64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}

// ........................................................................
// Little-endian
// ........................................................................

uint16_t kaitai::kstream::read_u2le() {
    uint16_t t;
    m_io->read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

uint32_t kaitai::kstream::read_u4le() {
    uint32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

uint64_t kaitai::kstream::read_u8le() {
    uint64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}

// ========================================================================
// Floating point numbers
// ========================================================================

// ........................................................................
// Big-endian
// ........................................................................

float kaitai::kstream::read_f4be() {
    uint32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_32(t);
#endif
    return reinterpret_cast<float&>(t);
}

double kaitai::kstream::read_f8be() {
    uint64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_64(t);
#endif
    return reinterpret_cast<double&>(t);
}

// ........................................................................
// Little-endian
// ........................................................................

float kaitai::kstream::read_f4le() {
    uint32_t t;
    m_io->read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_32(t);
#endif
    return reinterpret_cast<float&>(t);
}

double kaitai::kstream::read_f8le() {
    uint64_t t;
    m_io->read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_64(t);
#endif
    return reinterpret_cast<double&>(t);
}

// ========================================================================
// Unaligned bit values
// ========================================================================

void kaitai::kstream::align_to_byte() {
    m_bits_left = 0;
    m_bits = 0;
}

uint64_t kaitai::kstream::read_bits_int(int n) {
    int bits_needed = n - m_bits_left;
    if (bits_needed > 0) {
        // 1 bit  => 1 byte
        // 8 bits => 1 byte
        // 9 bits => 2 bytes
        int bytes_needed = ((bits_needed - 1) / 8) + 1;
        if (bytes_needed > 8)
            throw std::runtime_error("read_bits_int: more than 8 bytes requested");
        char buf[8];
        m_io->read(buf, bytes_needed);
        for (int i = 0; i < bytes_needed; i++) {
            uint8_t b = buf[i];
            m_bits <<= 8;
            m_bits |= b;
            m_bits_left += 8;
        }
    }

    // raw mask with required number of 1s, starting from lowest bit
    uint64_t mask = get_mask_ones(n);
    // shift mask to align with highest bits available in @bits
    int shift_bits = m_bits_left - n;
    mask <<= shift_bits;
    // derive reading result
    uint64_t res = (m_bits & mask) >> shift_bits;
    // clear top bits that we've just read => AND with 1s
    m_bits_left -= n;
    mask = get_mask_ones(m_bits_left);
    m_bits &= mask;

    return res;
}

uint64_t kaitai::kstream::get_mask_ones(int n) {
    if (n == 64) {
        return 0xFFFFFFFFFFFFFFFF;
    } else {
        return ((uint64_t) 1 << n) - 1;
    }
}

// ========================================================================
// Byte arrays
// ========================================================================

std::string kaitai::kstream::read_bytes(std::streamsize len) {
    std::vector<char> result(len);

    // NOTE: streamsize type is signed, negative values are only *supposed* to not be used.
    // http://en.cppreference.com/w/cpp/io/streamsize
    if (len < 0) {
        throw std::runtime_error("read_bytes: requested a negative amount");
    }

    if (len > 0) {
        m_io->read(&result[0], len);
    }

    return std::string(result.begin(), result.end());
}

std::string kaitai::kstream::read_bytes_full() {
    std::iostream::pos_type p1 = m_io->tellg();
    m_io->seekg(0, std::ios::end);
    std::iostream::pos_type p2 = m_io->tellg();
    size_t len = p2 - p1;

    // Note: this requires a std::string to be backed with a
    // contiguous buffer. Officially, it's a only requirement since
    // C++11 (C++98 and C++03 didn't have this requirement), but all
    // major implementations had contiguous buffers anyway.
    std::string result(len, ' ');
    m_io->seekg(p1);
    m_io->read(&result[0], len);

    return result;
}

std::string kaitai::kstream::read_bytes_term(char term, bool include, bool consume, bool eos_error) {
    std::string result;
    std::getline(*m_io, result, term);
    if (m_io->eof()) {
        // encountered EOF
        if (eos_error) {
            throw std::runtime_error("read_bytes_term: encountered EOF");
        }
    } else {
        // encountered terminator
        if (include)
            result.push_back(term);
        if (!consume)
            m_io->unget();
    }
    return result;
}

std::string kaitai::kstream::ensure_fixed_contents(std::string expected) {
    std::string actual = read_bytes(expected.length());

    if (actual != expected) {
        // NOTE: I think printing it outright is not best idea, it could contain non-ascii charactes like backspace and beeps and whatnot. It would be better to print hexlified version, and also to redirect it to stderr.
        throw std::runtime_error("ensure_fixed_contents: actual data does not match expected data");
    }

    return actual;
}

std::string kaitai::kstream::bytes_strip_right(std::string src, char pad_byte) {
    std::size_t new_len = src.length();

    while (new_len > 0 && src[new_len - 1] == pad_byte)
        new_len--;

    return src.substr(0, new_len);
}

std::string kaitai::kstream::bytes_terminate(std::string src, char term, bool include) {
    std::size_t new_len = 0;
    std::size_t max_len = src.length();

    while (new_len < max_len && src[new_len] != term)
        new_len++;

    if (include && new_len < max_len)
        new_len++;

    return src.substr(0, new_len);
}

// ========================================================================
// Byte array processing
// ========================================================================

std::string kaitai::kstream::process_xor_one(std::string data, uint8_t key) {
    size_t len = data.length();
    std::string result(len, ' ');

    for (size_t i = 0; i < len; i++)
        result[i] = data[i] ^ key;

    return result;
}

std::string kaitai::kstream::process_xor_many(std::string data, std::string key) {
    size_t len = data.length();
    size_t kl = key.length();
    std::string result(len, ' ');

    size_t ki = 0;
    for (size_t i = 0; i < len; i++) {
        result[i] = data[i] ^ key[ki];
        ki++;
        if (ki >= kl)
            ki = 0;
    }

    return result;
}

std::string kaitai::kstream::process_rotate_left(std::string data, int amount) {
    size_t len = data.length();
    std::string result(len, ' ');

    for (size_t i = 0; i < len; i++) {
        uint8_t bits = data[i];
        result[i] = (bits << amount) | (bits >> (8 - amount));
    }

    return result;
}

// ========================================================================
// Misc utility methods
// ========================================================================

int kaitai::kstream::mod(int a, int b) {
    if (b <= 0)
        throw std::invalid_argument("mod: divisor b <= 0");
    int r = a % b;
    if (r < 0)
        r += b;
    return r;
}

#include <stdio.h>
std::string kaitai::kstream::to_string(int val) {
    // if int is 32 bits, "-2147483648" is the longest string representation
    //   => 11 chars + zero => 12 chars
    // if int is 64 bits, "-9223372036854775808" is the longest
    //   => 20 chars + zero => 21 chars
    char buf[25];
    int got_len = snprintf(buf, sizeof(buf), "%d", val);

    // should never happen, but check nonetheless
    if (got_len > sizeof(buf))
        throw std::invalid_argument("to_string: integer is longer than string buffer");

    return std::string(buf);
}

#include <algorithm>
std::string kaitai::kstream::reverse(std::string val) {
    std::reverse(val.begin(), val.end());

    return val;
}

uint8_t kaitai::kstream::byte_array_min(const std::string val) {
    uint8_t min = 0xff; // UINT8_MAX
    std::string::const_iterator end = val.end();
    for (std::string::const_iterator it = val.begin(); it != end; ++it) {
        uint8_t cur = static_cast<uint8_t>(*it);
        if (cur < min) {
            min = cur;
        }
    }
    return min;
}

uint8_t kaitai::kstream::byte_array_max(const std::string val) {
    uint8_t max = 0; // UINT8_MIN
    std::string::const_iterator end = val.end();
    for (std::string::const_iterator it = val.begin(); it != end; ++it) {
        uint8_t cur = static_cast<uint8_t>(*it);
        if (cur > max) {
            max = cur;
        }
    }
    return max;
}

// ========================================================================
// Other internal methods
// ========================================================================

#ifndef KS_STR_DEFAULT_ENCODING
#define KS_STR_DEFAULT_ENCODING "UTF-8"
#endif

std::string kaitai::kstream::bytes_to_str(std::string src, std::string src_enc) {
    if(src_enc.compare("IBM866") == 0) {
        const int8_t diff1251_866_A_YA_a_p = 0xC0 - 0x80; //А..Я,а..п
        const int8_t diff1251_866_r_ya     = 0xF0 - 0xE0; //р..я
        const int8_t diff1251_866_JO       = 0xA8 - 0xF0; //Ё
        const int8_t diff1251_866_jo       = 0xB8 - 0xF1; //ё
        for(size_t i = 0; i < src.size(); ++i) {
            auto c = src[i];
            if(c >= 0) {
                continue; // a tipycal ASCII set for latin symbols keeped untouched
            }
            uint8_t current_char = (static_cast<int16_t>(c) + 0x100) & 0xFF;
            if(current_char < 0xB0) {
                src[i] = c + diff1251_866_A_YA_a_p;
            } else if(current_char >= 0xE0 && current_char < 0xF0){
                src[i] = c + diff1251_866_r_ya;
            } else if(current_char == 0xF0) {
                src[i] = c + diff1251_866_JO;
            } else if(current_char == 0xF1) {
                src[i] = c + diff1251_866_jo;
            } else {
                src[i] = ' '; // ignore special symbols
            }
        }
    }
    return src;
}
