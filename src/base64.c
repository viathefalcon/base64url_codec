//
//  base64.c
//  base64url_codec
//
//  Created by Stephen Higgins on 20/03/2023.
//

// Includes
//

// Standard Library Headers
#include <string.h>

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

// Gives the Base64URL output characters, and the reverse lookup table
static const char* base64url_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
static const char base64url_reverse[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, 52, 53, 54,
                                         55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3,
                                         4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                                         23, 24, 25, -1, -1, -1, -1, 63, -1, 26, 27, 28, 29, 30, 31, 32, 33,
                                         34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
                                         51, -1, -1, -1, -1, -1};

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

int base64url_decode(int(*has)(void), unsigned char(*get)(void), void(*output)(const unsigned char)) {

    char block[ENCODED_BLOCK_SIZE];
    do {
        // Read in the next block
        int r = 0;
        while (r < ENCODED_BLOCK_SIZE){
            if (has( )){
                const char c = *(base64url_reverse + get( ));
                if (c >= 0){
                    *(block + (r++)) = c;
                }
                continue;
            }
            break;
        }
        
        // If we read in nothing, just try again
        if (r == 0){
            continue;
        }
        const char* next = block;
        const int n1 = *(next++);
        const int n2 = *(next++);
        r -= 2;

        // n1 contains the top six bits
        // n2 contains the bottom two bits
        const unsigned char uc1 = (unsigned char)((n1 << 2) & MASK1) | ((n2 >> 4) & MASK2);
        output( uc1 );
        if (r < 1){
            continue;
        }

        // n2 contains the top four bits
        // n3 contains the next four bits (or padding)
        const int n3 = *(next++);
        const unsigned char uc2 = (unsigned char)((n2 & MASK4) << 4) | ((n3 >> 2) & MASK4);
        output( uc2 );
        if (--r < 1){
            continue;
        }

        // n3 contains the top two bits (or padding)
        // n4 contains the bottom six bits (or padding)
        const int n4 = *(next++);
        const unsigned char uc3 = (unsigned char)((n3 << 6) & MASK5) | (n4 & MASK6);
        output( uc3 );
    } while (has( ));
    return 0;
}
