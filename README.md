# Network Management using RPC

**Author:** Peeyush Gupta  
**Course:** CpeE - 550  
**Email:** peeyush@iastate.edu

## System Level Design

### 1. Overview

The Network Management using RPC project involves a Remote System Monitoring application, where clients can request statistics from remote servers through a Network Management Application. This distributed system keeps track of various system parameters such as CPU usage, user logins, and more on a host, and then transmits this information to clients across a network. The architecture is designed to support fault-tolerance, with multiple servers processing requests from numerous clients.

### 2. Architecture

The application consists of two main components: the Client and the Server. The Client facilitates users to request various parameters like date, time, CPU usage, memory usage, etc., from a remote machine. The Server, on the other hand, retrieves these parameters using system files and C libraries, and then sends the requested values to the clients using Remote Procedure Calls (RPC).

### 3. Components

#### a. Client

The Client component sends requests to the Server for specific data requested by the user, receives the Server's response, and displays it to users. The following functions are supported by the client:

- **Date and Time**: Sends requests for fetching Date and Time values from the remote machine. This function takes a parameter to specify the desired format of the value (Date, Time, or both).

- **CPU Usage**: Requests CPU usage information from the remote machine and receives it as a double.

- **Memory Usage**: Requests RAM and page data information from the remote machine. This client function calls two server functions: one for RAM data and another for Page Data.

- **Load Usage**: Requests load averages for 1min, 5min, and 15mins. The response is received as a string containing load averages.

- **User List**: Requests the user list in the Linux system. It fetches the list of users, and the response is received as a string.

#### b. Server

The Server receives requests from the Client, calculates the required values, and sends the responses back to the Client. The following functions are implemented by the server:

- **Date and Time**: Fetches requests for date and time and responds with the appropriate values. It uses the `strftime` function to obtain date and time values, and the response is based on the input parameter received (1, 2, or 3) for date, time, or both.

- **CPU Usage**: Retrieves CPU utilization information using the '/proc/stat' file and sends it to the client.

- **Memory Usage**: Provides details about RAM and page sizes, and the total available pages. RAM details are obtained from the `sysinfo` function, while page details are retrieved from the `mallinfo` function present in the 'malloc.h' library for C.

- **Load Usage**: Offers system load information for 1min, 5mins, and 15mins. The `getloadavg()` function provides the response, which is then sent to the client.

### The RPC Structure

The Network Management project employs Remote Procedure Call (RPC) technology, enabling clients to call functions or processes executing on a remote server as if they were local. RPC is commonly used in distributed systems to facilitate remote access to services.

For a remote procedure call to occur, a client sends a request to the server over the network. The server processes the request and sends a response back to the client. To achieve this, both client and server implement "stubs," which are specific software elements.

- **Client-Side Stubs**: Responsible for sending requests to the server and receiving responses. These stubs create request messages containing remote procedure parameters and deliver them to the server over the network. They also handle unpacking the server's response and sending the outcome back to the client application.

- **Server-Side Stubs**: Receive requests from client-side stubs, execute the actual process or function on the server, and transmit the server's response to the client-side stub. The server-side stub calls the appropriate function upon unpacking the received request message.

In summary, client-side and server-side stubs are integral to RPC systems. The client-side stub manages communication between the client application and the server, while the server-side stub handles communication between the server and the client. Through stubs, RPC systems abstract the intricacies of network communication, allowing remote procedure calls to be executed as if they were local.
