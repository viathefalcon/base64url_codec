//
//  main.c
//  base64url_codec
//
//  Created by Stephen Higgins on 20/03/2023.
//

// Includes
//

// Standard Library Headers
#include <stdio.h>

// Local Project Headers
#include "base64.h"

// Globals
//

int input = EOF;

// Functions
//

// Determines whether there is more input for the codec
int has(void) {

    if (input == EOF){
        // If we _are_ in fact at the end of the stream
        // then asking again will just give us the same
        // answer
        input = getchar( );
    }
    return (input == EOF) ? 0 : 1;
}

// Returns the next input for the codec
unsigned char get(void) {
    const unsigned char got = (unsigned char) input;
    input = EOF;
    return got;
}

#if defined(BASE64URL_ENCODER)
// Gives the output callback passed to the encoder
void base64url_encode_output(const char c) {
    putchar( c );
}
#elif defined(BASE64URL_DECODER)
// Gives the output callback passed to the decoder
void base64url_decode_output(const unsigned char uc) {
    fwrite( &uc, sizeof( uc ), 1, stdout );
}
#endif

// Gives the entry-point
int main(int argc, const char * argv[]) {

    int result = 1;
#if defined(BASE64URL_ENCODER)
    result = base64url_encode( has, get, base64url_encode_output );
#if defined(DEBUG)
    putchar( '\n' );
#endif
#elif defined(BASE64URL_DECODER)
    result = base64url_decode( has, get, base64url_decode_output );
#endif
    return result;
}
