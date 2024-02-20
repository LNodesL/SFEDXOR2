# SFEDXOR2
SFEDXOR v2.0.0 - Single File Encryption &amp; Decryption: XOR - Easily encrypt or decrypt files in CLI with key strings, or key input files. Supports random encryption key generating &amp; key output files (save new key to file, for recovery later).

## Usage

### Encrypt Mode

To encrypt a file:

```bash
./SFEDXOR -i <inputfile> -o <outputfile> -m encrypt [-k <keyfile>] [-ko <keyoutputfile>] [-ks <keystring>]
```

- `<inputfile>`: Path to the input file to be encrypted.
- `<outputfile>`: Path to save the encrypted output file.
- `-m encrypt`: Mode for encryption.
- `-k <keyfile>`: Optional. Path to a file containing the encryption key. If not provided, a key will be auto-generated.
- `-ko <keyoutputfile>`: Optional. Path to save the encryption key to a file.
- `-ks <keystring>`: Optional. Inline key string to use for encryption.

Examples:

Encrypt with auto-generated key:

```bash
./SFEDXOR -i input.txt -o encrypted.txt -m encrypt
```

Encrypt with specified key file and save key to another file:

```bash
./SFEDXOR -i input.txt -o encrypted.txt -m encrypt -k key.txt -ko key_saved.txt
```

Encrypt with inline key string:

```bash
./SFEDXOR -i input.txt -o encrypted.txt -m encrypt -ks mysecretpassword
```

### Decrypt Mode

To decrypt a file:

```bash
./SFEDXOR -i <inputfile> -o <outputfile> -m decrypt [-k <keyfile>] [-ks <keystring>]
```

- `<inputfile>`: Path to the encrypted input file.
- `<outputfile>`: Path to save the decrypted output file.
- `-m decrypt`: Mode for decryption.
- `-k <keyfile>`: Optional. Path to a file containing the decryption key.
- `-ks <keystring>`: Optional. Inline key string to use for decryption.

Examples:

Decrypt with specified key file:

```bash
./SFEDXOR -i encrypted.txt -o decrypted.txt -m decrypt -k key.txt
```

Decrypt with inline key string:

```bash
./SFEDXOR -i encrypted.txt -o decrypted.txt -m decrypt -ks mysecretpassword
```

**Note**

The program uses XOR encryption, which is a basic form of encryption and may not be suitable for all security needs. Ensure to keep your keys secure and never share them with unauthorized parties.
