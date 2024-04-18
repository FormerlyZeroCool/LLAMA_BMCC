# Comparing Fetch API Implementations for Web Service Interaction

This document explores two different methods of using the JavaScript Fetch API to interact with a web service hosted on `127.0.0.1:8080`. We'll demonstrate how to use both a direct query string and a JavaScript object with `URLSearchParams` to provide parameters for GET requests. Each method has its benefits, which we will discuss along with providing code examples.

Both methods have their advantages. Direct query strings are simple and more similar to our curl example, while URLSearchParams provides more flexibility and automatic URL encoding. The choice depends on the specific requirements and complexity of the parameters being used.

## Overview

The Fetch API offers a flexible approach to making HTTP requests in JavaScript. It can handle query parameters directly included in the URL string or construct them dynamically using JavaScript objects, providing clear advantages in different scenarios.

## Method 1: Direct Query String

Using a direct query string is straightforward and simple, suitable for scenarios where the parameters are static or dynamically generated in a part of the code that doesn't require manipulation or encoding.

### Example: `/query` Endpoint Using a Direct Query String

```javascript
async function fetchQueryDirect() {
  try {
    const response = await fetch('http://127.0.0.1:8080/query?prompt=Hello%20what%20are%20you');
    const data = await response.json();
    console.log(data);
  } catch (error) {
    console.error('Error:', error);
  }
}
fetchQueryDirect();
```

### Expected output:
```json
{
  "data": "I am a virtual assistant designed to help with your queries.",
  "error": "",
  "success": true
}```

## Method 2: Using URLSearchParams
URLSearchParams provides a more flexible and robust method for handling query parameters. It's particularly useful when dealing with multiple parameters that might require encoding, or when parameters are dynamically generated and manipulated. It also automatically handles URL encoding. In other words you don't have to replace spaces with %20 etc.

### Example: `/query` Endpoint Using URLSearchParams
```javascript
async function fetchQueryURLSearchParams() {
  try {
    const params = new URLSearchParams({ prompt: "Hello what are you" });
    const response = await fetch(`http://127.0.0.1:8080/query?${params.toString()}`);
    const data = await response.json();
    console.log(data);
  } catch (error) {
    console.error('Error:', error);
  }
}
fetchQueryURLSearchParams();
```
```json
### Expected output:
{
  "data": "I am a virtual assistant designed to help with your queries.",
  "error": "",
  "success": true
}
```
## Conclusion
