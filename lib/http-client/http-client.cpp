#include "http-client.hpp"
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* authServerAddress = "http://192.168.56.1:8080/api/leadyourway/v1/iot/token";
const char* serverAddress= "http://192.168.56.1:8080/api/leadyourway/v1/bicycles/gps/";

const char* login(const char* email, const char* password) {
  Serial.println("Waiting for the authentication response...");
  HTTPClient client;

  // Create HTTP client
  client.setTimeout(10000);
  client.begin(authServerAddress);
  client.addHeader("Content-Type", "application/json");

  // Prepare authentication request body
  String requestBody = "{\"email\":\"" + String(email) + "\",\"password\":\"" + String(password) + "\"}";

  // Send authentication request
  int httpResponseCode = client.POST(requestBody);
  String token = "";

  if (httpResponseCode == 200) {
    // Parse the JSON response to extract the authorization token
    String response = client.getString();
    const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);

    token = doc["idToken"].as<String>();
    client.end();
    Serial.println("Successful authentication");
    Serial.println(token.c_str());
    return token.c_str();
  } else {
    // Handle authentication error
    Serial.print("Authentication Error. HTTP response code:");
    Serial.println(httpResponseCode);
    client.end();
    Serial.println("An error occurred during authentication");
    return nullptr;
  }
}

// Function to send a PUT request to update data on the server
int sendUpdateRequest(const char* requestBody, const char* authToken, int resourceId) {
  // Create an HTTP client instance
  HTTPClient client;
  client.begin(serverAddress + String(resourceId));

  // Set headers for JSON data and authorization
  client.addHeader("Content-Type", "application/json");
  client.addHeader("Authorization", "Bearer " + String(authToken));

  // Send the PUT request and get the HTTP response code
  int httpResponseCode = client.PUT(requestBody);

  // Print the server response for debugging
  Serial.println(client.getString());
  client.end();

  // Return the HTTP response code
  return httpResponseCode;
}

// Function to check and print the HTTP response code
void checkResponseCode(int httpResponseCode) {
  // Check if the response code indicates success (HTTP 200 OK)
  if (httpResponseCode == 200) {
    Serial.println("Update successful on the server.");
  } else {
    // Print an error message with the HTTP response code
    Serial.print("Request error. HTTP response code:");
    Serial.println(httpResponseCode);
  }
}