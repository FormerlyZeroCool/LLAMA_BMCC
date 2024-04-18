# LLAMA_BMCC

## `/query` Endpoint

**Method:** GET  
**Description:** Processes a query by running a given prompt through the model, and returns the model's output.  
**URL Parameters:**
- `prompt` (string): The input prompt to be processed by the model.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
  {
    "data": "Processed model output",
    "error": "",
    "success": true
  }

**Error Response:**
- **Code:** 400 Bad Request 
- **Content:** 
  {
    "data": "",
    "error": "Exception, must supply parameter 'prompt'.",
    "success": false
  }

**Example Request:**
GET /query?prompt=Hello%20World

**Example Interaction:**
- **Request:** GET /query?prompt=Hello%20what%20are%20you
- **Response:**
  {
    "data": "I am a virtual assistant designed to help with your queries.",
    "error": "",
    "success": true
  }

## `/set_context` Endpoint

**Method:** GET  
**Description:** Updates the model's context and saves the new configuration to a file.  
**URL Parameters:**
- `ctx` (string): The new context string for the model.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
  {
    "error": "",
    "success": true
  }

**Error Response:**
- **Code:** 400 Bad Request 
- **Content:** 
  {
    "error": "must have param ctx for new model context.",
    "success": false
  }

**Example Request:**
GET /set_context?ctx=new%20context

## `/set_parameter` Endpoint

**Method:** GET  
**Description:** Updates a specific model parameter and validates the new value based on predefined parameter types. Updates the model configuration and reloads the model as necessary.  
**URL Parameters:**
- `key` (string): The name of the parameter to update.
- `value` (string): The new value for the parameter.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
  {
    "error": "",
    "success": true
  }

**Error Responses:**
1. **Missing Parameter Key or Value:**
   - **Code:** 400 Bad Request 
   - **Content:** 
     {
       "error": "must include parameters 'key' for parameter name, and 'value' for value.",
       "success": false
     }
2. **Invalid Key:**
   - **Code:** 400 Bad Request 
   - **Content:** 
     {
       "error": "Error, invalid key: '[key]'",
       "success": false
     }
3. **Invalid Value for the Specified Key:**
   - **Code:** 400 Bad Request 
   - **Content:** 
     {
       "error": "Error, invalid value: '[value]' for parameter: '[key]' of type: [type]",
       "success": false
     }

**Example Request:**
GET /set_parameter?key=exampleKey&value=exampleValue

**Example Parameter Update:**
- **Good Input:**
  - **Request:** GET /set_parameter?key=num_ctx&value=10
  - **Response:**
    {
      "error": "",
      "success": true
    }
- **Bad Input:**
  - **Request:** GET /set_parameter?key=num_ctx&value=ten
  - **Response:**
    {
      "error": "Error, invalid value: 'ten' for parameter: 'num_ctx' of type: Integer",
      "success": false
    }

