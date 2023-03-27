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

// Gives the output callback passed to the codec
void output(const char c) {
    putchar( c );
}

// Gives the entry-point
int main(int argc, const char * argv[]) {

#if defined(BASE64URL_ENCODER)
    const int result = base64url_encode( has, get, output );
#if defined(DEBUG)
    putchar( '\n' );
#endif
#else
    const int result = base64url_decode( has, get, output );
#endif
    return result;
}
