# Web Service Interaction Using `curl`

This document provides examples of how to interact with the web service running on `127.0.0.1:8080` using `curl` commands. Each endpoint is detailed with the appropriate `curl` command and the expected output.

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

