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
	int roundNumber = 0;
	int index = 0;
	int hop_count = 0;

	__asm
	{

	ROUNDLOOP:

		nop;
		nop;
		nop;
		nop;
		nop;

		// Fetch Global Variable data
		// leading the hash function into the base pointer to help narrow down data for encryption as the loops take place through the data array
		// gNumRounds being used to keep track of incrementation
		lea ebx, gPasswordHash;

		// Get the current round
		mov esi, [roundNumber];

		// Round loop stop case
		cmp esi, gNumRounds;
		jge END;

		// Zero the accumulator register
		xor eax, eax;

		// al = gPasswordHash[1 + round * 4]
		mov al, byte ptr[ebx + esi * 4 + 1];

		// ah = gPasswordHash[0+round*4] * 256
		mov ah, byte ptr[ebx + esi * 4 + 0];

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
		// Like with the decryption, the encyption will also need to know when to stop, where the pointer is when scalling through the data array
		// and where the pointer is to check on the encrpytion itself
		mov edx, datalength; // Stop Case
		xor ecx, ecx; // Counter
		mov esi, data; // Data Pointer
		lea edi, gkey; // gkey Pointer
		mov eax, [index];

	ENCRYPT:
		
		// Check to see if the program has reached the end of the array.
		cmp edx, ecx;
		je EXIT_ENCRYPT;

		// data[x] = data[x] ^ gkey[index], x = ecx
		mov bl, byte ptr[edi + eax];
		mov bh, byte ptr[esi + ecx];
		
		xor bh, bl;

		// index = index + hop_count
		mov eax, [hop_count];

		add eax, [index];
		cmp eax, 65537;
		jl CONTINUE2;
		sub eax, 65537;

	CONTINUE2:

		mov [index], eax;

		// Start Milestone 2: ----------------------------------------------------------------------------------------------------
		
		// Save registers
		push eax;
		push edx;
		push esi;
		push edi;
		push ecx;

		// Part A - Code table Swap
		lea eax, gEncodeTable;
		movzx edx, bh;

			// Store swapped variable in bh
		mov bh, byte ptr[eax + edx];

		// Part D - Invert bits of 0, 2, 4, 7
		xor bh, -107;

		// Part E - rotate 2 bits to the right
		ror bh, 2;

		// Part C - reverse bit order
		// Needs some work

		movzx eax, bh; // zero extend and push the address of both edx + ecx

		// ecx holds the number of times the rotate loop must run
		mov ecx, 8;

	ROTATELOOP:
		rcl al, 1;
		rcr ah, 1;
		loop ROTATELOOP;

		mov bh, ah // the value of ah goes into the new value of the combined addresses of edx and ecx

		// Part B - nibble rotate out	
		xor eax, eax;
		xor edx, edx;

			// Rotate left nibble
		mov al, bh;
		and al, -16;
		mov ah, al;
		and ah, -128;
		shr ah, 3; 
		shl al, 1;
		or al, ah;

			// Rotate right nibble
		mov dl, bh;
		and dl, 15;
		mov dh, dl;
		and dh, 1;
		shl dh, 3;
		shr dl, 1;
		or dl, dh;

			// Join both nibbles
		or al, dl;

		//Using bh as the register for the encrypted Character
		mov bh, al;

		// Load saved registers
		pop ecx;
		pop edi;
		pop esi;
		pop edx;
		pop eax;
		
	// -------------------- Milestone 2 END --------------------

		// Put encrypted character back into the file.
		mov byte ptr [esi + ecx], bh;
		inc ecx;

		// Jump back to the beginning of the loop
		jmp ENCRYPT;

	EXIT_ENCRYPT:

		// numrounds++
		inc [roundNumber];

		// Repeat round loop
		jmp ROUNDLOOP;

	END:
	}
	return;
} // encryptData_01
