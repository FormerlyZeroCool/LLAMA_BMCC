# Web Service Interaction Using `curl`

This document provides examples of how to interact with the web service running on `127.0.0.1:8080` using `curl` commands. Each endpoint is detailed with the appropriate `curl` command and the expected output.
## First what is curl?
### Curl Overview

`curl` (Client URL) is a versatile command-line tool used to transfer data to or from a server using a variety of protocols such as HTTP, HTTPS, FTP, FTPS, SCP, SFTP, LDAP, and more. It is widely utilized for testing, developing, and interacting with web APIs or for automating file transfers over network connections.

### Key Features

- **Multiple Protocols**: Supports a wide range of protocols, making it applicable for various network communication tasks.
- **HTTP Methods**: Facilitates performing requests like GET, POST, PUT, DELETE, and other HTTP methods directly from the command line or within scripts.
- **Platform Support**: Available on multiple platforms including Windows, macOS, and Linux.
- **Advanced Features**: Offers capabilities such as user authentication, header manipulation, form submissions, and cookie handling.

### Common Uses

- **API Testing**: Frequently used in development environments to test API endpoints before their integration into applications.
- **Scripting and Automation**: Useful for scripting interactions with web services in both development and production environments.

`curl` is recognized for its robustness and flexibility, making it an essential tool for developers and system administrators for both simple and complex network interactions.

## `/query` Endpoint

**Endpoint Description:**
Processes a query by running a given prompt through the model, and returns the model's output.

**curl Command:**
curl -X GET "http://127.0.0.1:8080/query?prompt=Hello%20what%20are%20you"

**Expected Output:**
{
  "data": "I am a virtual assistant designed to help with your queries.",
  "error": "",
  "success": true
}

## `/set_context` Endpoint

**Endpoint Description:**
Updates the model's context and saves the new configuration to a file.

**curl Command:**
curl -X GET "http://127.0.0.1:8080/set_context?ctx=New%20context%20setup"

**Expected Output:**
{
  "error": "",
  "success": true
}

## `/set_parameter` Endpoint

**Endpoint Description:**
Updates a specific model parameter and validates the new value based on predefined parameter types. Updates the model configuration and reloads the model as necessary.

### Good Input Example

**curl Command:**
curl -X GET "http://127.0.0.1:8080/set_parameter?key=num_ctx&value=10"

**Expected Output:**
{
  "error": "",
  "success": true
}

### Bad Input Example

**curl Command:**
curl -X GET "http://127.0.0.1:8080/set_parameter?key=num_ctx&value=ten"

**Expected Output:**
{
  "error": "Error, invalid value: 'ten' for parameter: 'num_ctx' of type: Integer",
  "success": false
}

## What's next?

Move on to the FETCH_README.md to see the next series of instructions to follow, or try to send more requests to the web service, and have some fun if you have extra time!
