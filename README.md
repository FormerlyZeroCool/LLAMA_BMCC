# LLAMA_BMCC
## Endpoints Documentation

This documentation covers the interaction with Ollama models through three specific endpoints: `/query`, `/set_context`, and `/set_parameter`. Each endpoint serves a unique purpose, allowing users to query the model, set its context, or adjust its parameters via HTTP GET requests.

### `/query` Endpoint

Allows users to send a prompt to the model and receive its output.

- **Method**: GET
- **URL Parameter Required**: `prompt`
- **Functionality**: Retrieves the prompt from the request, runs the model with the given prompt, and returns the model's output. An error message is returned if the prompt parameter is missing.
- **Response Format**: JSON
- **Success Response Example**: `{"data":"<model_output>", "error":""}`
- **Error Response Example**: `{"data":"", "error":"Exception, must supply parameter 'prompt'."}`

### `/set_context` Endpoint

Enables setting or updating the context for the model, influencing its behavior.

- **Method**: GET
- **URL Parameter Required**: `ctx` (the new context for the model)
- **Functionality**: Updates the model's context with the provided `ctx` value, saves the updated configuration, and reloads the Ollama model with the new context.
- **Response Format**: JSON
- **Success Response Example**: `{"error":"false", "success":"true"}`
- **Error Response Example**: `{"error":"true", "success":"false", "msg":"must have param ctx for new model context."}`

### `/set_parameter` Endpoint

Used for adjusting specific parameters of the model.

- **Method**: GET
- **URL Parameters Required**:
  - `key`: The name of the parameter to be adjusted.
  - `value`: The new value for the parameter.
- **Functionality**: If both `key` and `value` are provided and the key exists, updates the specified parameter and reloads the model with the new settings. Returns an error if any parameter is missing or the key is invalid.
- **Response Format**: JSON
- **Success Response Example**: `{"error":"false", "success":"true"}`
- **Error Response Example**: `{"error":"true", "success":"false", "msg":"must include parameters key for parameter name, and value for value."}`

