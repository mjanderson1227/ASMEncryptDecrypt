// DecryptData.cpp
//
// This file uses the input data and key information to decrypt the input data
//

#include "Main.h"

void decryptData_01(char* data, int sized)
{
	// Setup to create global variables if needed in the future for debugging
	// Two global variables to compare string information when searching for the decryption 
	// globalDebug1 = 0;
	// globalDebug2 = 0;
	
	__asm
	{
		// Zero the accumulator register
		xor eax, eax;

		// Fetch Global Variable data
		// lea or load effective address so that our hash function has a point on the base register
		// the hash function will serve as a way to keep track of and narrow down our search 
		lea ebx, gPasswordHash;
		mov esi, gNumRounds;

		// Set rounds to 0
		dec esi;

		// ah = gPasswordHash[0+round*4] * 256
		mov ah, byte ptr[ebx + esi * 4 + 0];

		// al = gPasswordHash[1 + round * 4]
		mov al, byte ptr[ebx + esi * 4 + 1];

		// eax = index = gPasswordHash[0 + round * 4] * 256 + gPasswordHash[1 + round * 4]

		// Setting up loop values:
		// The two pointers are used for scaling through the array of values. One to increment the place in the string and the other to check it.
		mov edx, sized; // Stop Case
		xor ecx, ecx; // Counter
		mov esi, data; // Data Pointer
		lea edi, gkey // gkey Pointer

	DECRYPT:

		// Check to see if the program has reached the end of the array.
		// je being jump if equal or a conditional jump in this case checking if the end of the array has been reached.
		cmp edx, ecx;
		je END;
		
		
		// data[x] = data[x] ^ gkey[index]
		mov bl, byte ptr[edi + eax];
		mov bh, byte ptr[esi + ecx];
		

		// Increment counter
		
	// -------------------- milestone 2 ---------------------------- //

		// Save registers
		push eax;
		push edx;
		push esi;
		push edi;
		
		// nibble rotate in	
		xor eax, eax;
		xor edx, edx;

		// Rotate left nibble
		mov dl, bh;
		and dl, -16;
		mov dh, dl;
		and dh, 16;
		shl dh, 3; // <-- Possible error.
		shr dl, 1;
		and dl, -16;
		or dl, dh;

		// Rotate right nibble
		mov al, bh;
		and al, 15;
		mov ah, al;
		and ah, 8;
		shr ah, 3; // <-- Possible error.
		shl al, 1;
		and al, 15
		or al, ah;

		// Join both nibbles
		or al, dl;

		//Using bh as the register for the encrypted Character
		mov bh, al;
		
		// Reverse bit order
		movzx eax, bh; // zero extend and load into 32 bit reg for segmented manipulation  
		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		rcl al, 1;
		rcr ah, 1;

		mov bh, ah; // the value of ah goes into the new value of the combined addresses of edx and ecx

		// Rotate 2 bits left
		rol bh, 2;
		
		// Invert bits of 0, 2, 4, 7
		xor bh, -107;

		// Index decoding table
		lea eax, gDecodeTable;
		movzx edx, bh;

		mov bh, byte ptr[eax + edx];

		// Load saved registers
		pop edi;
		pop esi;
		pop edx;
		pop eax;
		
	// -------------------- milestone 2 ---------------------------- //

		mov bl, byte ptr[edi + eax];
		// Perform XOR AFTER the rest of the operations
		xor bh, bl;

		mov byte ptr[esi + ecx], bh;
		inc ecx;

		// Jump back to the beginning of the loop
		jmp DECRYPT;

	END:
		nop;
	}
	return;
} // decryptData_01
