# TCP-Terminal-Texter

## About
TTT is a multi-threaded TCP TUI chat server written in C using raw sockets, on top of which a custom protocol will be built. Open Sourcing text based communication so I can talk to my friends without propreitary software usage.

## Installation
### Clone Repo then run the following:

```
make server
make client
```

## Running
### Start server

```
./server_side
```

### Start client

```
./client_side
```

## Features

* Multithreaded server using pthread - **Implemented**
* Mutex usage to prevent race conditions - **Implemented**
* Input cleaning - **Implemented**
* Broadcast messaging system (Global Chat)- **Implemented**
* Custom protocol on top of TCP - **To Implement**
* Thread-safe client list using mutex - **Implemented**
* Session ID generation and management - **To Implement**
* Persistent session storage (client-side + server-side DB) - **To Implement**
* Session restoration on reconnect - **To Implement**
* Authentication system - **To Implement**
* Private messaging / proper chat rooms - **To Implement**
* Rate limiting / Content Moderation - **To Implement**