#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <cstring>
using namespace std;


// Rotate left function
uint32_t rotate_left(uint32_t value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}

// Print 16-word state
void print_state(const uint32_t state[16], const string &msg) {
    cout << msg << ":\n";
    for (int i = 0; i < 16; i++) {
        cout << hex << setw(8) << setfill('0') << state[i] << " ";
        if ((i + 1) % 4 == 0) cout << "\n";
    }
    cout << "\n";
}

// Quarter round with detailed debug
void quarter_round(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, const string &name = "") {
    cout << "\nQuarter Round " << name << " start: ";
    cout << hex << setw(8) << setfill('0') << a << " "
         << setw(8) << b << " "
         << setw(8) << c << " "
         << setw(8) << d << "\n";

    a += b;       cout << "a += b: " << setw(8) << a << "\n";
    d ^= a;       cout << "d ^= a: " << setw(8) << d << "\n";
    d = rotate_left(d, 16); cout << "d <<< 16: " << setw(8) << d << "\n";

    c += d;       cout << "c += d: " << setw(8) << c << "\n";
    b ^= c;       cout << "b ^= c: " << setw(8) << b << "\n";
    b = rotate_left(b, 12); cout << "b <<< 12: " << setw(8) << b << "\n";

    a += b;       cout << "a += b: " << setw(8) << a << "\n";
    d ^= a;       cout << "d ^= a: " << setw(8) << d << "\n";
    d = rotate_left(d, 8);  cout << "d <<< 8: " << setw(8) << d << "\n";

    c += d;       cout << "c += d: " << setw(8) << c << "\n";
    b ^= c;       cout << "b ^= c: " << setw(8) << b << "\n";
    b = rotate_left(b, 7);  cout << "b <<< 7: " << setw(8) << b << "\n";

    cout << "Quarter Round " << name << " end: ";
    cout << setw(8) << a << " "
         << setw(8) << b << " "
         << setw(8) << c << " "
         << setw(8) << d << "\n";
}

// ChaCha20 block function
void chacha20_block(uint32_t output[16], const uint32_t input[16]) {
    memcpy(output, input, sizeof(uint32_t) * 16);
    print_state(output, "Initial block state");

    for (int i = 0; i < 10; ++i) { // 10 double rounds
        cout << "\n=== Double Round " << i+1 << " ===\n";

        // Column rounds
        quarter_round(output[0], output[4], output[8], output[12], "0-4-8-12");
        quarter_round(output[1], output[5], output[9], output[13], "1-5-9-13");
        quarter_round(output[2], output[6], output[10], output[14], "2-6-10-14");
        quarter_round(output[3], output[7], output[11], output[15], "3-7-11-15");

        // Diagonal rounds
        quarter_round(output[0], output[5], output[10], output[15], "0-5-10-15");
        quarter_round(output[1], output[6], output[11], output[12], "1-6-11-12");
        quarter_round(output[2], output[7], output[8], output[13], "2-7-8-13");
        quarter_round(output[3], output[4], output[9], output[14], "3-4-9-14");
    }

    for (int i = 0; i < 16; ++i)
        output[i] += input[i];

    print_state(output, "Final keystream block");
}

// XOR function with per-byte trace
void chacha20_xor(vector<uint8_t> &out, const vector<uint8_t> &in,
                  const vector<uint32_t> &key, const vector<uint32_t> &nonce, uint32_t counter) {
    uint32_t state[16], keystream[16];
    uint8_t block[64];

    size_t len = in.size();
    out.resize(len);

    // Initialize state
    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;
    for (int i = 0; i < 8; i++) state[4 + i] = key[i];
    state[12] = counter;
    state[13] = nonce[0];
    state[14] = nonce[1];
    state[15] = nonce[2];

    cout << "\nStarting encryption/decryption...\n";

    for (size_t offset = 0; offset < len; offset += 64) {
        cout << "\nProcessing block starting at byte " << offset << "\n";

        chacha20_block(keystream, state);
        memcpy(block, keystream, 64);

        // Print keystream
        cout << "Keystream block (hex): ";
        for (int i = 0; i < 64; i++)
            cout << hex << setw(2) << setfill('0') << (int)block[i] << " ";
        cout << "\n";

        size_t block_size = min(len - offset, size_t(64));
        for (size_t i = 0; i < block_size; i++) {
            out[offset + i] = in[offset + i] ^ block[i];

            // Per-byte trace
            cout << "Byte " << offset+i << ": "
                 << "Plaintext=0x" << setw(2) << (int)in[offset+i] << " "
                 << "Keystream=0x" << setw(2) << (int)block[i] << " "
                 << "Output=0x" << setw(2) << (int)out[offset+i] << "\n";
        }

        state[12]++;
    }
}

int main() {
    // 256-bit key
    vector<uint32_t> key = {0x03020100,0x07060504,0x0b0a0908,0x0f0e0d0c,
                            0x13121110,0x17161514,0x1b1a1918,0x1f1e1d1c};

    // 96-bit nonce
    vector<uint32_t> nonce = {0x00000009,0x4a000000,0x00000000};

    uint32_t counter = 1;

    string plaintext;
    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    vector<uint8_t> input(plaintext.begin(), plaintext.end());
    vector<uint8_t> ciphertext, decrypted;

    // Encrypt
    chacha20_xor(ciphertext, input, key, nonce, counter);

    // Decrypt
    chacha20_xor(decrypted, ciphertext, key, nonce, counter);

    // Display final results
    cout << "\nFinal Ciphertext (hex): ";
    for (auto c : ciphertext)
        cout << hex << setw(2) << setfill('0') << (int)c << " ";
    cout << "\nDecrypted text: " << string(decrypted.begin(), decrypted.end()) << endl;

    return 0;
}
