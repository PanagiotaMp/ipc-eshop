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


## How It Works

1. The parent process acts as the e-shop server and initializes
   a product catalog containing product information, prices,
   and available stock.

2. The program creates multiple child processes using `fork()`.
   Each child process represents a client placing orders.

3. Two anonymous pipes are created for communication:
   - One pipe for client-to-server communication
   - One pipe for server-to-client communication

4. Each client:
   - Randomly selects a product
   - Sends the product number to the server through a pipe
   - Waits for the server response

5. The server:
   - Receives the client request
   - Checks product availability
   - Updates inventory and statistics
   - Sends the result back to the client

6. If the product is available:
   - The client receives confirmation
   - The product price is sent to the client
   - The server updates total sales and profit

7. If the product is unavailable:
   - The client receives a failure message

8. After all client requests are completed, the server prints:
   - Total requests
   - Successful and unsuccessful orders
   - Sold products
   - Product statistics
   - Total profit

## Running the Project

This project is designed for Linux enviroments.

### Compile 

Open a terminal in the project directory and run: 

```bash
gcc IPC_eshop.c -o eshop

./eshop
```
