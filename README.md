# LLAMA_BMCC

A webservice front end to Meta's ollama tool to get large language models running on your machine
The service provides a means to query a language model, change the starting context, and parameters.
For more information on ollama check out their github repo, and documentation. 

repo:
https://github.com/ollama/ollama/tree/main

docs:
https://github.com/ollama/ollama/blob/main/docs/modelfile.md

## `/query` Endpoint

**Method:** GET  
**Description:** Processes a query by running a given prompt through the model, and returns the model's output.  
**URL Parameters:**
- `prompt` (string): The input prompt to be processed by the model.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
```javascript
  {
    "data": "Processed model output",
    "error": "",
    "success": true
  }
  ```

**Error Response:**
- **Code:** 200 OK 
- **Content:** 
```javascript
  {
    "data": "",
    "error": "Exception, must supply parameter 'prompt'.",
    "success": false
  }
  ```

**Example Request:**
```GET /query?prompt=Hello%20World```

**Example Interaction:**
- **Request:** GET /query?prompt=Hello%20what%20are%20you
- **Response:**
```javascript
  {
    "data": "I am a virtual assistant designed to help with your queries.",
    "error": "",
    "success": true
  }
  ```

## `/set_context` Endpoint

**Method:** GET  
**Description:** Updates the model's context and saves the new configuration to a file.  
**URL Parameters:**
- `ctx` (string): The new context string for the model.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
```javascript
  {
    "error": "",
    "success": true
  }
  ```

**Error Response:**
- **Code:** 200 OK 
- **Content:** 
```javascript
  {
    "error": "must have param ctx for new model context.",
    "success": false
  }
  ```

**Example Request:**
`GET /set_context?ctx=new%20context`

## `/set_parameter` Endpoint

**Method:** GET  
**Description:** Updates a specific model parameter and validates the new value based on predefined parameter types. Updates the model configuration and reloads the model as necessary.  
**URL Parameters:**
- `key` (string): The name of the parameter to update.
- `value` (string): The new value for the parameter.

**Success Response:**
- **Code:** 200 OK 
- **Content:** 
```javascript
  {
    "error": "",
    "success": true
  }
  ```

**Error Responses:**
1. **Missing Parameter Key or Value:**
   - **Code:** 200 OK 
   - **Content:** 
   ```javascript
     {
       "error": "must include parameters 'key' for parameter name, and 'value' for value.",
       "success": false
     }
     ```
2. **Invalid Key:**
   - **Code:** 200 OK 
   - **Content:** 
   ```javascript
     {
       "error": "Error, invalid key: '[key]'",
       "success": false
     }
     ```
3. **Invalid Value for the Specified Key:**
   - **Code:** 200 OK 
   - **Content:** 
   ```javascript
     {
       "error": "Error, invalid value: '[value]' for parameter: '[key]' of type: [type]",
       "success": false
     }
     ```

**Example Request:**
GET /set_parameter?key=exampleKey&value=exampleValue

**Example Parameter Update:**
- **Good Input:**
  - **Request:** GET /set_parameter?key=num_ctx&value=10
  - **Response:**
  ```javascript
    {
      "error": "",
      "success": true
    }
    ```
- **Bad Input:**
  - **Request:** GET /set_parameter?key=num_ctx&value=ten
  - **Response:**
  ```javascript
    {
      "error": "Error, invalid value: 'ten' for parameter: 'num_ctx' of type: Integer",
      "success": false
    }
    ```

### Endpoint Documentation: `/set_base_model`

This endpoint is responsible for setting the base model to be used by the system. It accepts a single query parameter, `model`, which specifies the name of the base model to be set. The endpoint then loads the corresponding model configuration, updates the system's configuration accordingly, and returns information about the selected base model.

#### Method: GET

#### URL Parameters:
- `model` (string): The name of the base model to be set.

#### Success Response:
- **Code:** 200 OK
- **Content:**
  ```json
  {
    "info": "Model: phi, Parameters: 2.7B, Size: 1.7GB",
    "error": "",
    "success": true
  }
  ```

#### Error Responses:
1. **Missing Parameter Model:**
   - **Code:** 200 OK
   - **Content:**
     ```json
     {
       "error": "must include parameter model",
       "success": false
     }
     ```
   - **Details:** This error occurs when the `model` parameter is not provided in the request. The parameter is mandatory for setting the base model.

2. **Invalid Model Name:**
   - **Code:** 200 OK
   - **Content:**
     ```json
     {
       "error": "Error, invalid model name: '[model_name]'",
       "success": false
     }
     ```
   - **Details:** This error indicates that the provided `model` parameter does not match any valid base model names. It's essential to specify a valid model name to set the base model correctly.

#### Examples:
- **Valid Input:**
  - **Request:**
    ```
    GET /set_base_model?model=phi
    ```
  - **Expected Output:**
    ```json
    {
      "info": "Model: phi, Parameters: 2.7B, Size: 1.7GB",
      "error": "",
      "success": true
    }
    ```

- **Invalid Input 1 (Missing Model Parameter):**
  - **Request:**
    ```
    GET /set_base_model
    ```
  - **Expected Output:**
    ```json
    {
      "error": "must include parameter model",
      "success": false
    }
    ```

- **Invalid Input 2 (Invalid Model Name):**
  - **Request:**
    ```
    GET /set_base_model?model=invalid_model
    ```
  - **Expected Output:**
    ```json
    {
      "error": "Error, invalid model name: 'invalid_model'",
      "success": false
    }
    ```

## `/base_model_options` Endpoint

**Endpoint Description:**
Retrieves a list of available base model options along with their parameters and sizes.

**Example Output (Partial):**
```json
[
  {"model_name":"codellama", "params":"7B", "size":"3.8GB"},
  {"model_name":"dolphin-phi", "params":"2.7B", "size":"1.6GB"},
  {"model_name":"gemma:2b", "params":"2B", "size":"4.8GB"},
  ...
]
```

## `/model_parameter_options` Endpoint

**Endpoint Description:**
Retrieves a list of available model parameter options along with their types and descriptions.

**Example Output (Partial):**
```json
[
  {"param_name":"mirostat", "type":"Integer", "desc":"Enable Mirostat sampling for controlling perplexity."},
  {"param_name":"mirostat_eta", "type":"Float", "desc":"Influences how quickly the algorithm responds to feedback from the generated text."},
  {"param_name":"mirostat_tau", "type":"Float", "desc":"Controls the balance between coherence and diversity of the output."},
  ...
]
```


## What's next?
Check out the INSTALL_README.md to install the necessary software to run this project on windows, and feel free to refer back to this at any time for how to interact with this service when it is running.
