//
//  base64.h
//  base64url_codec
//
//  Created by Stephen Higgins on 20/03/2023.
//

#ifndef base64_h
#define base64_h

// Functions
//

// Encodes the given input with Base64 URL encoding, invoking the callback for each output character
int base64url_encode(int(*has)(void), unsigned char(*get)(void), void(*output)(const char));

// Decodes the given input from Base64 URL encoding, invoking the callback for each output byte
int base64url_decode(int(*has)(void), unsigned char(*get)(void), void(*output)(const char));

#endif /* base64_h */
