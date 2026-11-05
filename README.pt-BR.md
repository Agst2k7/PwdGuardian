🇧🇷 Português | [🇺🇸 English](README.md)
# PwdGuardian

**PwdGuardian** é um gerenciador de senhas desenvolvido em C, com interface de linha de comando. Ele permite armazenar credenciais localmente de forma criptografada, utilizando algoritmos criptográficos modernos para proteger informações sensíveis.

O projeto foi desenvolvido com o objetivo de explorar programação de baixo nível, manipulação de arquivos, gerenciamento de memória e criptografia em C.

> **Status:** Em desenvolvimento. Algumas funcionalidades ainda estão sendo implementadas.

## Funcionalidades

### Implementadas
- Autenticação por senha mestra.
- Criação de um cofre local criptografado.
- Armazenamento seguro de credenciais.
- Listagem de credenciais.
- Consulta individual de credenciais.
- Gerenciamento de sessões de acesso ao cofre.
- Limpeza de dados sensíveis da memória após o uso.
- Considerações de compatibilidade entre Linux e Windows.

### Em desenvolvimento
- Remoção de credenciais.
- Melhorias no gerenciamento de arquivos e backups.
- Aprimoramentos e testes adicionais de segurança.

## Criptografia

O PwdGuardian utiliza dois componentes criptográficos principais:

### Argon2id — Função de Derivação de Chaves

O Argon2id é utilizado para derivar uma chave criptográfica a partir da senha mestra do usuário.

A configuração atual é:

| Parâmetro | Valor |
|---|---:|
| Algoritmo | Argon2id |
| Custo de tempo | 3 |
| Custo de memória | 65536 KiB |
| Paralelismo | 1 |
| Tamanho do salt | 16 bytes |
| Tamanho da chave derivada | 32 bytes |

Um salt aleatório é gerado durante a criação do cofre. A chave derivada não é armazenada no disco: ela é gerada novamente sempre que o usuário realiza a autenticação.

### AES-256-GCM — Criptografia Autenticada

As credenciais são criptografadas utilizando AES-256-GCM.

| Parâmetro | Valor |
|---|---:|
| Algoritmo | AES-256-GCM |
| Tamanho da chave | 256 bits |
| Tamanho do nonce | 12 bytes |
| Tamanho da tag de autenticação | 16 bytes |

O AES-GCM fornece confidencialidade e verificação de integridade. Cada campo criptografado de uma credencial utiliza seu próprio nonce aleatório e sua respectiva tag de autenticação.

### Verificação do Cofre

Durante a criação do cofre, o PwdGuardian criptografa um valor de verificação utilizando a chave derivada.

Quando o usuário tenta realizar o login, a aplicação deriva novamente a chave e tenta descriptografar esse valor. A descriptografia bem-sucedida confirma que a senha informada produziu a chave esperada.

## Estrutura do Projeto

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

### Responsabilidade dos módulos

| Módulo | Responsabilidade |
|---|---|
| `CLI` | Interface de terminal e interação com o usuário |
| `encrypt` | Criptografia AES-256-GCM e derivação de chaves com Argon2id |
| `file_handler` | Leitura e escrita de arquivos do cofre |
| `structs` | Definição das estruturas de dados das credenciais |
| `vault` | Operações do cofre e gerenciamento de sessões |
| `main.c` | Fluxo principal da aplicação e navegação entre menus |

## Requisitos

- GCC ou outro compilador C compatível.
- Bibliotecas de desenvolvimento do OpenSSL.
- Bibliotecas de desenvolvimento do Argon2.

## Instalação e Compilação

### Linux (Ubuntu/Debian)

Instale as dependências:

```bash
sudo apt update
sudo apt install build-essential libssl-dev libargon2-dev
```

Clone o repositório:

```bash
git clone https://github.com/Agst2k7/PwdGuardian.git
cd PwdGuardian
```

Compile o projeto:

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

Execute:

```bash
./programa
```

### Windows

O código-fonte possui implementações específicas para Windows em algumas operações de terminal.

Para compilar no Windows, é necessário um compilador C e bibliotecas de desenvolvimento compatíveis do OpenSSL e Argon2.

A configuração do linker deve incluir os caminhos e as dependências apropriadas para a ferramenta de compilação utilizada.

O procedimento de compilação no Windows ainda não foi documentado como um processo validado.

## Utilização

1. Inicie a aplicação.
2. Crie um cofre definindo uma senha mestra, caso ainda não exista um.
3. Selecione **Enter** para acessar o cofre.
4. Informe sua senha mestra.
5. Após a autenticação, utilize o menu principal para:
   - Visualizar as credenciais disponíveis.
   - Adicionar uma nova credencial.
   - Encerrar a sessão.

As credenciais são armazenadas localmente no arquivo criptografado do cofre.

## Armazenamento de Dados

O cofre é armazenado em:

```text
./data/vault.dat
```

O arquivo contém o cabeçalho do cofre e os registros de credenciais criptografados.

Como seu conteúdo está criptografado, o arquivo pode ser copiado diretamente para a criação de um backup, sem a necessidade de descriptografar as credenciais individualmente.

Entretanto, os backups devem ser armazenados em um local seguro. Qualquer pessoa que obtenha o arquivo do cofre pode tentar realizar ataques de tentativa de senhas offline contra a senha mestra.

## Considerações de Segurança

- A senha mestra não é armazenada em texto puro.
- A chave de criptografia é derivada utilizando Argon2id.
- Salts e nonces aleatórios são utilizados.
- AES-256-GCM fornece criptografia autenticada.
- Buffers que armazenam dados sensíveis são limpos da memória utilizando `OPENSSL_cleanse()` nos pontos em que essa funcionalidade foi implementada.

**Importante:** O PwdGuardian é um projeto educacional e pessoal que ainda está em desenvolvimento. Ele não passou por uma auditoria independente de segurança e ainda não deve ser considerado um gerenciador de senhas completamente auditado para uso em produção.

## Licença

Nenhuma licença foi especificada até o momento.
