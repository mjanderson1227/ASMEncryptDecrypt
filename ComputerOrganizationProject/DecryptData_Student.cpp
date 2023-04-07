// DecryptData.cpp
//
// This file uses the input data and key information to decrypt the input data
//

#include "Main.h"

void decryptData_01(char* data, int sized)
{
	__asm
	{
		// Zero the accumulator register
		xor eax, eax;

		// Fetch Global Variable data
		lea ebx, gPasswordHash;
		mov esi, gNumRounds;

		// ah = gPasswordHash[0+round*4] * 256
		mov ah, byte ptr[ebx + esi * 4 + 0];

		// al = gPasswordHash[1 + round * 4]
		mov al, byte ptr[ebx + esi * 4 + 1];

		// eax = index = gPasswordHash[0 + round * 4] * 256 + gPasswordHash[1 + round * 4]

		// Setting up loop values:
		mov edx, sized; // Stop Case
		xor ecx, ecx; // Counter
		mov esi, data; // Data Pointer
		lea edi, gkey // gkey Pointer

	DECRYPT:
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
		jmp DECRYPT;

	END:
		nop;
	}
	return;
} // decryptData_01