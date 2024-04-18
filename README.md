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
   - **Details:** This error occurs if either the `key` or `value` parameter is not provided in the request. Both parameters are required for updating a model parameter.

2. **Invalid Key:**
   - **Code:** 400 Bad Request 
   - **Content:** 
     {
       "error": "Error, invalid key: '[key]'",
       "success": false
     }
   - **Details:** This error is returned if the provided `key` does not exist in the `type_map`, indicating that the `key` is not recognized as a valid parameter name.

3. **Invalid Value for the Specified Key:**
   - **Code:** 400 Bad Request 
   - **Content:** 
     {
       "error": "Error, invalid value: '[value]' for parameter: '[key]' of type: [type]",
       "success": false
     }
   - **Details:** If the `value` provided is not valid according to the type specified in `type_map` for the `key`, this error is returned. The validity is determined by specific type checks (e.g., checking if a value is a valid integer or float, as defined in the `ParamType` structure).

**Example Request:**
GET /set_parameter?key=exampleKey&value=exampleValue

**Detailed Operation:**
- The endpoint first checks if both `key` and `value` parameters are present in the request.
- It then checks if the `key` is valid by looking it up in the `type_map`.
- If the `key` is valid, it evaluates the `value` based on the expected type (integer, float, or string).
- If the `value` passes the type check, the parameter in the model is updated, and changes are saved to the configuration file.
- The model is then reloaded and warmed up to reflect the new parameter settings.

