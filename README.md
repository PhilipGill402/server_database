# C Key-Value Store (TCP Client/Server)

A simple in-memory key-value database written in C, exposed over a TCP socket using a minimal text command protocol. Includes a TCP server that accepts client connections and a TCP client for sending commands.

---

## Features

- **Commands**
  - `set <key> <value>` — store a value at a key
  - `get <key>` — retrieve a value
  - `del <key>` — deletes a key
  - `exists <key>` — checks if a key exists

- **Value Types**
  - Integers (e.g., `42`)
  - Doubles (e.g., `3.14159`)
  - Strings (e.g., `hello`)

---

## Protocol

Commands are sent as a single line of text:

```
<command> <key> [value]\n
```

Examples:

```
set foo 123
get foo
set pi 3.14
set name philip
get name
```

The server responds with a text message, such as the value or an error message.

---

## Project Structure

- `server.c` / `server.h` — TCP server + request handler
- `client.c` — simple TCP client
- `database.c` / `database.h` — in-memory database implementation
- `Makefile` — build targets

---

## Build

### Server

From the repository root:

```bash
make
```
### Client

From the client directory:

```bash
make client
```

Or:
```bash
gcc client.c -o client
```

---

## Run

### 1) Start the server

```bash
./server
```

### 2) Run a client (in another terminal)

```bash
./client
```

Then type commands like:

```txt
set test 123
get test
```

---

## Roadmap / TODO

- [ ] Add `pthread` support (thread-per-client or thread pool)
- [ ] Add database locking (mutex/RW lock)
- [ ] Improve message framing (support multi-word strings / quoting / length-prefixing)
- [ ] Add tests for `set/get/del/exists`, collision handling, and edge cases

---
