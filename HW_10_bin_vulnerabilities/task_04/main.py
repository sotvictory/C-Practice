#!/usr/bin/env python3

import os


def encrypt(text, key):
    ciphertext = b''
    for i in range(0, len(text) - len(key), len(key)):
        block = b''
        for j in range(0, len(key)):
            block += int.to_bytes(
                key[j] ^ text[i + j],
                length=1,
                byteorder='little',
            )
        ciphertext += block
        key = block
    return ciphertext


def main():
    with open('key.txt', 'rb') as f:
        key = f.read()

    with open('flag.txt', 'rb') as f:
        flag = f.read()

    n = len(flag) % len(key)
    if n != 0:
        flag += os.urandom(len(key) - n)

    with open('ciphertext.bin', 'wb') as f:
        f.write(encrypt(flag, key))


if __name__ == '__main__':
    main()
