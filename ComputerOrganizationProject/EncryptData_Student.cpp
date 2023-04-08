// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

void encryptData_01(char* data, int datalength)
{
	
	// Just like with the decryption files, these will serve as a test further on 
	// Setup to create global variables if needed in the future for debugging
	// Two global variables to compare string information when searching for the decryption 
	// globalDebug1 = 0;
	// globalDebug2 = 0;
	
	__asm
	{
		// Zero the accumulator register
		xor eax, eax;

		// Fetch Global Variable data
		// leading the hash function into the base pointer to help narrow down data for encryption as the loops take place through the data array
		// gNumRounds being used to keep track of incrementation
		lea ebx, gPasswordHash;
		mov esi, gNumRounds;

		// ah = gPasswordHash[0+round*4] * 256
		mov ah, byte ptr[ebx + esi * 4 + 0];

		// al = gPasswordHash[1 + round * 4]
		mov al, byte ptr[ebx + esi * 4 + 1];

		// eax = index = gPasswordHash[0 + round * 4] * 256 + gPasswordHash[1 + round * 4]

		// Setting up loop values:
		// Like with the decryption, the encyption will also need to know when to stop, where the pointer is when scalling through the data array
		// and where the pointer is to check on the encrpytion itself
		mov edx, datalength; // Stop Case
		xor ecx, ecx; // Counter
		mov esi, data; // Data Pointer
		lea edi, gkey // gkey Pointer

	ENCRYPT:
		// Check to see if the program has reached the end of the array.
		cmp edx, ecx;
		je END;
		
		// data[x] = data[x] ^ gkey[index]
		mov bl, byte ptr[edi + eax];
		mov bh, byte ptr[esi + ecx];
		xor bh, bl;
		mov byte ptr[esi + ecx], bh;

		// Increment counter
		add ecx, 1;

		// Jump back to the beginning of the loop
		jmp ENCRYPT;

	END:
		nop;
	}
	return;
} // encryptData_01
