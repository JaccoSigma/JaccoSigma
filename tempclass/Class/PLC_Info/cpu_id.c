#ifdef _LSL_TARGETARCH_X86
#ifdef _MSC_VER
void cpu_id( unsigned long param, unsigned char* ret0 )
{
	_asm
	{
		pushad
		mov		eax, param
		cpuid
		mov		edi, ret0
		mov		[edi], eax
		mov		[edi+4],ebx
		mov		[edi+8],edx
		mov		[edi+12],ecx
		popad
	}
}

#else

extern "C" void cpu_id( unsigned long param, unsigned char* ret0 )
{
	asm("push %eax");
	asm("push %ebx");
	asm("push %ecx");
	asm("push %edx");
	asm("push %edi");
	asm("mov	%0, %%eax" :"=r"(param)	:"r"(param)	:"%eax");
	asm("cpuid");
	asm("mov	%0, %%edi" :"=r"(ret0)	:"r"(ret0)	:"%eax");
	asm("mov	%eax, (%edi)");
	asm("mov	%ebx, 4(%edi)");
	asm("mov	%edx, 8(%edi)");
	asm("mov	%ecx, 12(%edi)");
	asm("pop %edi");
	asm("pop %edx");
	asm("pop %ecx");
	asm("pop %ebx");
	asm("pop %eax");
}

#endif
#endif