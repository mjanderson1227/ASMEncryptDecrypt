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

		// Set rounds to 0
		dec esi;

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
		
		// data[x] = data[x] ^ gkey[index], x = esi+ecx
		mov bl, byte ptr[edi + eax];
		mov bh, byte ptr[esi + ecx];
		
		xor bh, bl;

		// Increment counter
		
	// -------------------- milestone 2 ---------------------------- //
		// Save registers
		push eax;
		push edx;
		push esi;
		push edi;

		// Part A - Code table Swap
		lea eax, gEncodeTable;
		movsx edx, bh;

		// Store swapped variable in bh
		mov bh, byte ptr[eax + edx];

		// nibble rotate out	
		xor eax, eax;
		xor edx, edx;

		// Rotate left nibble
		mov al, bh;
		and al, -16;
		mov ah, al;
		and ah, -128;
		rol ah, 4; // <-- Possible error.
		sal al, 1;
		or al, ah;

		// Rotate right nibble
		mov dl, bh;
		and dl, 15;
		mov dh, bl;
		and dh, 1;
		ror dh, 4; // <-- Possible error.
		sar dl, 1;
		or dl, dh;

		// Join both nibbles
		or al, dl;

		//Using bh as the register for the encrypted Character
		mov bh, al;
		
		// -------------------- milestone 2 ---------------------------- //

		// Part C - reverse bit order
		// Needs some work
		-------------------------------------------------------------------------------------------------------
		/*
		movzx eax, [edx + ecx]; // zero extend and push the address of both edx + ecx
		// brute force method
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		rcl al, 1
		rcr ah, 1
			
		mov [edx + ecx], ah // the value of ah goes into the new value of the combined addresses of edx and ecx
		*/
		
		-----------------------------------------------------------------------------------------------------------
		
		// Part C - reverse bit order but using a loop instead
		/*
		mov eax, 8 // initialize the loop counter to 8
		
		bit_reverse_loop:
			rcl al, 1 // rorate left with carry
			rcr ah, 1 // rotate left with carry
			loop bit_reverse_loop // to decrement the counter and jump to the start of the loop if not zero
			
		bit_reverse_done:
			cmp ecx, 0 // comparing the loop counter with zero
			jne continue_after_loop // if not zero, jump to continue_after_loop
		
		continue_after_loop:
			continue // keep going into part D
		
		-----------------------------------------------------------------------------------------------------------	
		
		// moving with the sign extension 
		movsx eax, 4]
		
		
		/* old attempted part C function, not a brute force approach
		xor eax, eax; // zero out the eax registers
		mov al, bh; // set bh to the last bit at al
		mov ah, bh; 
		and ah, -16;
		and al, 15;
		rol ah, 4;
		ror al, 4;
		or al, ah;
		mov bh, al;
		*/

		
		// Part D - Invert bits of 0, 2, 4, 7
		xor bh, 109;
		
		// Part E - rotate 2 bits to the right
		ror bh, 2;

		// Load saved registers
		pop edi;
		pop esi;
		pop edx;
		pop eax;
		
	// -------------------- milestone 2 ---------------------------- //
		
		inc ecx;
		mov byte ptr[esi + ecx], bh;

		// Jump back to the beginning of the loop
		jmp ENCRYPT;

	END:
		nop;
	}
	return;
} // encryptData_01
