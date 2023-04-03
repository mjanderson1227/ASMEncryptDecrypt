// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"
#include <iostream>

void encryptData_01(char* data, int datalength)
{
	__asm
	{
		//Set up stack frame
		push ebp;
		mov ebp, esp;
		sub esp, 12; // Change later to correspond to the other milestones
		push ebx;
		push esi;
		push edi;

		// Starting_index[round] = gPasswordHash[0+round*4] * 256 + gPasswordHash[1+round*4];
		mov eax, 256;
		mov edx, dword ptr[gNumRounds];
		movzx ebx, byte ptr[gPasswordHash + edx * 4];

		// Store gPasswordHash[0+round*4] * 256 in eax
		mul ebx;

		// Store gPasswordHash[1+round*4] in ebx
		movzx ebx, byte ptr[esi + edx * 4 + 1];

		// Add together and store in Starting_index local variable
		add eax, ebx;
		mov dword ptr[ebp - 4], eax;

		// index = Starting_index[round]
		mov dword ptr[ebp - 8], eax;

		// Begin Loop through the file
		mov ecx, 0;
		mov edx, dword ptr[ebp + 12];

	ENCRYPT:
		// If the value of ecx is equal to the datalength then stop the loop.
		cmp ecx, edx;
		je END;

		mov ebx, dword ptr[ebp - 8];
		mov al, byte ptr[gkey + ebx];
		mov bl, byte ptr[ebp + ecx + 8];
		xor al, bl;
		mov byte ptr[ebp + ecx + 8], al;


		add ecx, 1;
		jmp ENCRYPT;

	END:
		// Tear Down Stack Frame
		add esp, 12;
		pop edi;
		pop esi;
		pop ebx;
		mov esp, ebp;
		pop ebp;
		ret;
	}

	return;
} // encryptData_01