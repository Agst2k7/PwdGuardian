# PwdGuardian

**PwdGuardian** is a command-line password manager written in C. It provides encrypted local storage for credentials, using modern cryptographic algorithms to protect sensitive information.

The project was developed to explore low-level programming, file handling, memory management, and cryptography in C.

> **Status:** Work in progress. Some features are still under development.

## Features

### Implemented
- Master password authentication.
- Local encrypted vault creation.
- Secure credential storage.
- Credential listing.
- Individual credential retrieval and visualization.
- Session-based access to the vault.
- Memory cleansing of sensitive data after use.
- Cross-platform code considerations for Linux and Windows.

### In development
- Credential deletion.
- Additional file-management and backup improvements.
- Further security hardening and testing.

## Cryptography

PwdGuardian uses two main cryptographic components:

### Argon2id — Key Derivation Function

Argon2id derives a cryptographic key from the user's master password.

The current configuration is:

| Parameter | Value |
|---|---:|
| Algorithm | Argon2id |
| Time cost | 3 |
| Memory cost | 65536 KiB |
| Parallelism | 1 |
| Salt size | 16 bytes |
| Derived key size | 32 bytes |

A random salt is generated when the vault is created. The derived key is not stored on disk; it is regenerated when the user authenticates.

### AES-256-GCM — Authenticated Encryption

Credentials are encrypted using AES-256-GCM.

| Parameter | Value |
|---|---:|
| Algorithm | AES-256-GCM |
| Key size | 256 bits |
| Nonce size | 12 bytes |
| Authentication tag | 16 bytes |

AES-GCM provides both confidentiality and integrity verification. Each encrypted credential field uses its own randomly generated nonce and authentication tag.

### Vault Verification

When the vault is created, PwdGuardian encrypts a verification value using the derived key.

When the user attempts to log in, the application derives the key again and attempts to decrypt this value. Successful authentication confirms that the entered master password produced the expected key.

## Project Structure

```text
PwdGuardian/
├── data/
├── main.c
├── compile.txt
└── utils/
    ├── CLI/
    │   ├── CLI.c
    │   ├── CLI.h
    │   ├── inputs.c
    │   └── inputs.h
    ├── encrypt/
    │   ├── aes.c
    │   ├── aes.h
    │   ├── kdf.c
    │   └── kdf.h
    ├── file_handler/
    │   ├── fileHandler.c
    │   └── fileHandler.h
    ├── structs/
    │   └── CREDENTIAL.h
    └── vault/
        ├── session.c
        ├── session.h
        ├── vault.c
        └── vault.h
```

### Module responsibilities

| Module | Responsibility |
|---|---|
| `CLI` | Terminal interface and user interaction |
| `encrypt` | AES-256-GCM encryption and Argon2id key derivation |
| `file_handler` | Vault file reading and writing |
| `structs` | Credential data structures |
| `vault` | Vault operations and session management |
| `main.c` | Application flow and menu navigation |

## Requirements

- GCC or another compatible C compiler.
- OpenSSL development libraries.
- Argon2 development libraries.

## Installation and Compilation

### Linux (Ubuntu/Debian)

Install the dependencies:

```bash
sudo apt update
sudo apt install build-essential libssl-dev libargon2-dev
```

Clone the repository:

```bash
git clone https://github.com/Agst2k7/PwdGuardian.git
cd PwdGuardian
```

Compile the project:

```bash
gcc main.c \
    ./utils/encrypt/aes.c \
    ./utils/encrypt/kdf.c \
    ./utils/file_handler/fileHandler.c \
    ./utils/vault/vault.c \
    ./utils/vault/session.c \
    ./utils/CLI/CLI.c \
    ./utils/CLI/inputs.c \
    -o programa -largon2 -lcrypto
```

Run:

```bash
./programa
```

### Windows

The source code includes Windows-specific handling for some terminal operations.

To compile on Windows, you need a C compiler and compatible OpenSSL and Argon2 development libraries.

The linker configuration must include the appropriate library paths and dependencies for your Windows toolchain.

Windows compilation has not yet been documented as a verified build procedure.

## Usage

1. Start the application.
2. Create a vault by defining a master password, if one does not already exist.
3. Select **Enter** to access the vault.
4. Enter your master password.
5. After authentication, use the main menu to:
   - View available credentials.
   - Add a new credential.
   - Log out.

Credentials are stored locally in the encrypted vault file.

## Data Storage

The vault is stored in:

```text
./data/vault.dat
```

The file contains the vault header and encrypted credential records.

Since the contents are encrypted, the vault file can also be copied as a backup without decrypting individual credentials.

However, backups should be kept in a secure location. Anyone who obtains the vault file can attempt offline password-guessing attacks against the master password.

## Security Considerations

- The master password is not stored in plaintext.
- The encryption key is derived using Argon2id.
- Random salts and nonces are used.
- AES-256-GCM provides authenticated encryption.
- Sensitive buffers are cleansed from memory using OpenSSL's `OPENSSL_cleanse()` where implemented.

**Important:** PwdGuardian is an educational and personal project that is still under development. It has not undergone an independent security audit. It should not yet be considered a thoroughly audited password manager for production use.

## License

No license has been specified yet.
