import hmac
import hashlib
import os
import sys

def ask_alg():
    filename = input()
    filepath = os.path.join("./bin", filename + ".bin")
    if not os.path.exists(filepath):
        print("File does not exist.")
        sys.exit(1)
    return filename

def gen_hmac_sha256(key: bytes, msg: bytes) -> str:
    return hmac.new(key, msg, hashlib.sha256).hexdigest()

def gen_hmac_from_file(key:bytes, filename: str) -> bytes:
    with open(filename, 'rb') as f:
        return gen_hmac_sha256(key, f.read())

def gen_hmac_hexfile(key:bytes, filename: str) -> str:
    hmac_val = gen_hmac_from_file(key, filename)
    filepath = os.getcwd() + f'/integrity_test/hmac_{filename[6:-4]}.txt'
    with open(filepath, 'w') as f:
        f.write(hmac_val)
    return hmac_val

if __name__ == '__main__':
    alg = ask_alg()

    key = [0] * 32
    key = bytes(key)

    ret = gen_hmac_hexfile(key, f'./bin/{alg}.bin')
    print(alg.split('/')[-1])


