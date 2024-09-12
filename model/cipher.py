import base64

def repeat_key(key: str, length: int) -> str:
    repeated_key = key
    while len(repeated_key) < length:
        repeated_key += key
    return repeated_key[:length]

def cipher_algo(message: str, key: str) -> str:
    key = repeat_key(key, len(message))

    encrypted = bytearray()
    for m, k in zip(message, key):
        encrypted_char = ord(m) ^ ord(k)
        encrypted.append(encrypted_char)

    encrypted_message = base64.urlsafe_b64encode(encrypted).decode('utf-8')
    return "C:" + encrypted_message

def decipher_algo(message: str, key: str) -> str:
    encrypted = base64.urlsafe_b64decode(message[2:])
    key = repeat_key(key, len(encrypted))

    decrypted_chars = []
    for e, k in zip(encrypted, key):
        decrypted_char = chr(e ^ ord(k))
        decrypted_chars.append(decrypted_char)

    decrypted_message = ''.join(decrypted_chars)
    return decrypted_message
