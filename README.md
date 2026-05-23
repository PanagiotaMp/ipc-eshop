# IPC e-shop simulation in C 

An e-shop simulation implemented in C using Linux Inter-Process Communication (IPC) mechanisms.

The project simulates an electronic shop server handling multiple client orders through anonymous pipes.

## Technologies 
- C
- Linux System Calls
- fork()
- pipe()
- read() / write()
- waitpid()

## Features

- Multi-process architecture using fork()
- Bidirectional communication using pipe()
- Client-Server simulation of order handling
- Inventory management system
- Product availability handling
- Request statistics and sales reporting
- Randomized client order generation

## Running the Project

This project is designed for Linux enviroments.

### Compile 

Open a terminal in the project directory and run: 

```bash
gcc IPC_eshop.c -o eshop

./eshop
```
