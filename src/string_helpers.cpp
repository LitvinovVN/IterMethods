#ifndef _string_helpers
#define _string_helpers

#include <iostream> 
#include <iterator> 
#include <sstream> 
#include <string> 
#include <vector>

// Function to split a string into tokens based on a 
// delimiter 
std::vector<std::string> splitString(std::string& input, char delimiter) 
{  
    // Creating an input string stream from the input string 
    std::istringstream stream(input); 
  
    // Vector to store the tokens 
    std::vector<std::string> tokens; 
  
    // Temporary string to store each token 
    std::string token; 
  
    // Read tokens from the string stream separated by the 
    // delimiter 
    while (getline(stream, token, delimiter))
    { 
        // Add the token to the vector of tokens 
        tokens.push_back(token); 
    } 
  
    // Return the vector of tokens 
    return tokens; 
}

#endif