# Comparing Fetch API Implementations for Web Service Interaction

Both methods have their advantages. Direct query strings are simple and more similar to our curl example, while URLSearchParams provides more flexibility and automatic URL encoding.

This document explores two different methods of using the JavaScript Fetch API to interact with a web service hosted on `127.0.0.1:8080`. We'll demonstrate how to use both a direct query string and a JavaScript object with `URLSearchParams` to provide parameters for GET requests. Each method has its benefits, which we will discuss along with providing code examples.


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

```javascript
{
  "data": "I am a virtual assistant designed to help with your queries.",
  "error": "",
  "success": true
}
```

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

### Expected output:

```javascript
{
  "data": "I am a virtual assistant designed to help with your queries.",
  "error": "",
  "success": true
}
```
## Conclusion

Direct query strings are simple and more similar to our curl example, while URLSearchParams provides more flexibility and automatic URL encoding.
In general you should prefer to use the URLSearchParams, but the goal of showing you this comparison is to see a closer analogue to our previous curl example, and how it translates to Javascript using the Fetch API.  Which is something you can build integrations with this, and other web services into your front ends.

## What's next?

Now you should try to write some javascript code using the fetch api to change the context of the model, and then try to prompt the model qith a new query to compare the changes
