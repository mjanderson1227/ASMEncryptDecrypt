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
	int roundNumber = 0;
	int index = 0;
	int hop_count = 0;
	
	__asm
	{
		// Set roundNumber to gNumRounds-1 to start in reverse
		mov ebx, gNumRounds;
		dec ebx;
		mov [roundNumber], ebx;

	ROUNDLOOP:
		// Zero the accumulator register
		xor eax, eax;

		// Round Loop Stop Case
		mov esi, [roundNumber];
		test esi, esi;
		jl END;

		// Fetch Global Variable data
		// lea or load effective address so that our hash function has a point on the base register
		// the hash function will serve as a way to keep track of and narrow down our search 
		lea ebx, gPasswordHash;

		// ah = gPasswordHash[0+round*4] * 256
		mov ah, byte ptr[ebx + esi * 4 + 0];

		// al = gPasswordHash[1 + round * 4]
		mov al, byte ptr[ebx + esi * 4 + 1];

		// eax = index = gPasswordHash[0 + round * 4] * 256 + gPasswordHash[1 + round * 4]
		mov [index], eax;

		// Zero eax again
		xor eax, eax;

		// al = gPasswordHash[3+round*4]
		mov al, byte ptr[ebx + esi * 4 + 3];

		// ah = gPasswordHash[2+round*4]*256
		mov ah, byte ptr[ebx + esi * 4 + 2];

		test eax, eax;
		jne CONTINUE1;
		mov eax, 0xFFFF;

	CONTINUE1:
		mov [hop_count], eax;


		// Setting up loop values:
		// The two pointers are used for scaling through the array of values. One to increment the place in the string and the other to check it.
		mov edx, sized; // Stop Case
		xor ecx, ecx; // Counter
		mov esi, data; // Data Pointer
		lea edi, gkey; // gkey Pointer
		mov eax, [index]; // Grab the index

	DECRYPT:

		// Check to see if the program has reached the end of the array.
		// je being jump if equal or a conditional jump in this case checking if the end of the array has been reached.
		cmp edx, ecx;
		je EXIT_DECRYPT;
		
		
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
		push ecx;
		
		// nibble rotate in	
		xor eax, eax;
		xor edx, edx;

		// Rotate left nibble
		mov dl, bh;
		mov dh, dl;
		and dh, 16;
		shl dh, 3;
		shr dl, 1;
		and dl, -16;
		or dl, dh;

		// Rotate right nibble
		mov al, bh; 
		mov ah, al;
		and ah, 8;
		shr ah, 3; 
		shl al, 1;
		and al, 15
		or al, ah;

		// Join both nibbles
		or al, dl;

		//Using bh as the register for the encrypted Character
		mov bh, al;
		
		// Reverse bit order
		movzx eax, bh; // zero extend and load into 32 bit reg for segmented manipulation  
		mov ecx, 8;

		// Loop will reverse bit order
	ROTATELOOP:
		rcl al, 1;
		rcr ah, 1;
		loop ROTATELOOP;

		// Retrieve reversed characters
		mov bh, ah; 

		// Rotate 2 bits left
		rol bh, 2;
		
		// Invert bits of 0, 2, 4, 7
		xor bh, -107;

		// Index decoding table
		lea eax, gDecodeTable;
		movzx edx, bh;

		mov bh, byte ptr[eax + edx];

		// Load saved registers
		pop ecx;
		pop edi;
		pop esi;
		pop edx;
		pop eax;
		
	// -------------------- milestone 2 ---------------------------- //

		mov bl, byte ptr[edi + eax];
		// Perform XOR AFTER the rest of the operations

		// index = index + hop_count
		mov eax, [hop_count];

		add eax, [index];
		cmp eax, 65537;
		jl CONTINUE2;
		sub eax, 65537;

	CONTINUE2:
		mov [index], eax;

		xor bh, bl;

		mov byte ptr[esi + ecx], bh;
		inc ecx;

		jmp DECRYPT;

	EXIT_DECRYPT:
		// round--
		dec [roundNumber];

		//Repeat round loop
		jmp ROUNDLOOP;

	END:
	}
	return;
} // decryptData_01
