//
//  base64.c
//  base64url_codec
//
//  Created by Stephen Higgins on 20/03/2023.
//

// Includes
//

// Standard Library Headers
#include <strings.h>

// Declarations
#include "base64.h"

// Macros
//

// Defines the sizes of decoded and
// encoded blocks, respectively
#define DECODED_BLOCK_SIZE  3
#define ENCODED_BLOCK_SIZE  4

// Defines the bitmasks applied to input characters
// to retrieve the bits to make up the indices into
// the Base64 alphabet
#define MASK1   0xfc
#define MASK2   0x03
#define MASK3   0xf0
#define MASK4   0x0f
#define MASK5   0xc0
#define MASK6   0x3f

// Constants
//

// Gives the Base64URL output characters
static const char* base64url_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// Functions
//

int base64url_encode(int(*has)(void), unsigned char(*get)(void), void(*output)(const char)) {

    unsigned char block[DECODED_BLOCK_SIZE];
    do {
        // Read in the next block
        size_t r = 0;
        while (r < DECODED_BLOCK_SIZE){
            if (has( )){
                *(block + (r++)) = get( );
                continue;
            }
            break;
        }
        
        // If we read in nothing, just try again
        if (r == 0){
            continue;
        }
        const unsigned char* ptr = block;

        // 1st output char
        const unsigned char b1 = *(ptr++);
        const char c1 = *(base64url_alphabet + ((b1 & MASK1) >> 2));
        output( c1 );
        
        // 2nd output char
        const unsigned char b2 = (r == 1) ? 0 : *(ptr++);
        const char n2 = (b1 & MASK2) << 4;
        const char n3 = (b2 & MASK3) >> 4;
        const char c2 = *(base64url_alphabet + (n2 | n3));
        output( c2 );
        
        // 3rd + 4th output chars
        const unsigned char b3 = (r == 2) ? 0: *(ptr++);
        if (r > 1){
            const char n4 = (b2 & MASK4) << 2;
            const char n5 = (b3 & MASK5) >> 6;
            const char c3 = *(base64url_alphabet + (n4 | n5));
            output( c3 );
        }
        if (r > 2){
            const char c4 = *(base64url_alphabet + (b3 & MASK6));
            output( c4 );
        }
    } while (has( ));
    return 0;
}

int base64url_decode(int(*has)(void), unsigned char(*get)(void), void(*output)(const char)) {

    // Build the reverse lookup table from the alphabet
    char base64_reverse[128] = { 0 };
    memset( base64_reverse, -1, sizeof( base64_reverse ) );
    const size_t alphabet_len = strlen( base64url_alphabet );
    for (size_t s = 0; s < alphabet_len; s++){
        base64_reverse[base64url_alphabet[s]] = (int) s;
    }

    char block[ENCODED_BLOCK_SIZE];
    do {
        // Read in the next block
        size_t r = 0;
        while (r < ENCODED_BLOCK_SIZE){
            if (has( )){
                *(block + (r++)) = get( );
                continue;
            }
            break;
        }
        
        // If we read in nothing, just try again
        if (r == 0){
            continue;
        }
        const char* encoded = block;

        // Get out the first two encoded characters
        const char n1 = *(base64_reverse + *(encoded++));
        const char n2 = *(base64_reverse + *(encoded++));
        r -= 2;

        // n1 contains the top six bits
        // n2 contains the bottom two bits
        const char c1 = ((n1 << 2) & MASK1) | ((n2 >> 4) & MASK2);
        output( c1 );
        if (!r){
            continue;
        }

        // n2 contains the top four bits
        // n3 contains the next four bits (or padding)
        const char n3 = *(base64_reverse + *(encoded++));
        --r;
        const char c2 = ((n2 & MASK4) << 4) | ((n3 >> 2) & MASK4);
        output( c2 );
        if (!r){
            continue;
        }

        // n3 contains the top two bits (or padding)
        // n4 contains the bottom six bits (or padding)
        const char n4 = *(base64_reverse + *(encoded++));
        const char c3 = ((n3 << 6) & MASK5) | (n4 & MASK6);
        output( c3 );
    } while (has( ));
    return 0;
}
