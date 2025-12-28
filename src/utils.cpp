#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdint>
#include "utils.h"
// Uncomment to enable test harness #define TEST_SHA256 

namespace gearforge {
namespace utils {

// SHA-256 helper functions
namespace {
inline uint32_t ROTR(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
inline uint32_t SHR(uint32_t x, uint32_t n) { return x >> n; }
inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
inline uint32_t Sigma0(uint32_t x) { return ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22); }
inline uint32_t Sigma1(uint32_t x) { return ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25); }
inline uint32_t sigma0(uint32_t x) { return ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3); }
inline uint32_t sigma1(uint32_t x) { return ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10); }
} // unnamed namespace

Sha256Hash sha256(const std::string& input) {
    const uint32_t K[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    Sha256Hash hash;
    std::vector<uint8_t> bytes(input.begin(), input.end());
    uint64_t bit_len = bytes.size() * 8;

    // Padding
    bytes.push_back(0x80);
    while ((bytes.size() % 64) != 56) bytes.push_back(0);
    for (int i = 7; i >= 0; --i) bytes.push_back((bit_len >> (i*8)) & 0xFF);

    // Initialize hash state
    hash.state = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                  0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};

    for (size_t i = 0; i < bytes.size(); i += 64) {
        uint32_t W[64];
        for (int t = 0; t < 16; ++t) {
            W[t] = (bytes[i + t*4] << 24) | (bytes[i + t*4 + 1] << 16) |
                   (bytes[i + t*4 + 2] << 8) | (bytes[i + t*4 + 3]);
        }
        for (int t = 16; t < 64; ++t) {
            W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
        }

        uint32_t a = hash.state[0], b = hash.state[1], c = hash.state[2], d = hash.state[3];
        uint32_t e = hash.state[4], f = hash.state[5], g = hash.state[6], h = hash.state[7];

        for (int t = 0; t < 64; ++t) {
            uint32_t T1 = h + Sigma1(e) + Ch(e,f,g) + K[t] + W[t];
            uint32_t T2 = Sigma0(a) + Maj(a,b,c);
            h = g; g = f; f = e; e = d + T1;
            d = c; c = b; b = a; a = T1 + T2;
        }

        hash.state[0] += a; hash.state[1] += b; hash.state[2] += c; hash.state[3] += d;
        hash.state[4] += e; hash.state[5] += f; hash.state[6] += g; hash.state[7] += h;
    }

    return hash;
}

// Helper: convert state to hex string
std::string to_hex_string(const Sha256Hash& h) {
    std::ostringstream oss;
    for (auto word : h.state) {
        oss << std::hex << std::setw(8) << std::setfill('0') << word;
    }
    return oss.str();
}

}  // namespace utils
}  // namespace gearforge

// ---------------------------
// Test harness
#ifdef TEST_SHA256
int main() {
    std::string input;
    std::cout << "Enter string to hash: ";
    std::getline(std::cin, input);

    auto hash = gearforge::utils::sha256(input);
    std::cout << "SHA-256: " << gearforge::utils::to_hex_string(hash) << std::endl;
    return 0;
}
#endif
