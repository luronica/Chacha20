# Chacha20

This repository contains a C++ implementation of the ChaCha20 stream cipher, created as part of the Innovative Examination (Internal Assessment) group activity.

The code includes detailed debugging output such as:

Initial ChaCha20 state (16-word block)

Quarter-round step-by-step operations

Double-round tracing (10 double rounds = 20 rounds total)

Keystream generation in hex

Per-byte XOR trace during encryption/decryption

Project Purpose

ChaCha20 is a modern symmetric stream cipher widely used for secure encryption.
This implementation demonstrates how ChaCha20 works internally with full transparency for learning and analysis.

Features

256-bit key support (8 × 32-bit words)

96-bit nonce support (3 × 32-bit words)

Counter-based block generation

Full ChaCha20 block function (20 rounds total)

Step-by-step debug for quarter rounds

Encryption + decryption using XOR (same function)

Prints final ciphertext in hexadecimal format

How it Works (High Level)

ChaCha20 generates a keystream block (64 bytes) from:

Constant (4 words)

Key (8 words)

Counter (1 word)

Nonce (3 words)

Then it performs:
Ciphertext = Plaintext XOR Keystream
Plaintext = Ciphertext XOR Keystream

Encryption and decryption are the same operation.

Technologies Used

Language: C++
Concepts: Bitwise operations, rotation, XOR encryption, state matrix (4×4)

How to Run

Compile (G++ recommended):
g++ chacha20.cpp -o chacha20

Run:
./chacha20

Example Input:
Enter plaintext: Hello World

You will see:

Full internal operations

Keystream and XOR tracing

Final ciphertext

Decrypted output text

Output

The program displays:

Final Ciphertext (hex)

Decrypted text (should match input plaintext)



Team Members (Student Details): 

Varnika Kambley — Third Year B.Tech (IoT), Semester V, Roll No. 13

Daksh Kotian — Third Year B.Tech (IoT), Semester V, IoT 17

Dhruv Kotian — Third Year B.Tech (IoT), Semester V, IoT 18

Zenith Kokkodan — Third Year B.Tech (IoT), Semester V, Roll No. 15

Kaustubh Deshpande — Third Year B.Tech (IoT), Semester V, Roll No. 14

For the Subject of : Cryptography and Network Security (2024-2025)

Thakur College of Engineering and Technology, Mumbai.



Note : This project is designed for educational and demonstration purposes, with extra debugging output enabled for learning how ChaCha20 works step-by-step.

License : Free to use for academic and learning purposes.
