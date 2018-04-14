   1              		.syntax unified
   2              		.cpu cortex-m3
   3              		.fpu softvfp
   4              		.eabi_attribute 20, 1
   5              		.eabi_attribute 21, 1
   6              		.eabi_attribute 23, 3
   7              		.eabi_attribute 24, 1
   8              		.eabi_attribute 25, 1
   9              		.eabi_attribute 26, 1
  10              		.eabi_attribute 30, 6
  11              		.eabi_attribute 34, 1
  12              		.eabi_attribute 18, 4
  13              		.thumb
  14              		.file	"main.c"
  15              		.text
  16              	.Ltext0:
  17              		.cfi_sections	.debug_frame
  18              		.section	.text.compute_stats,"ax",%progbits
  19              		.align	2
  20              		.global	compute_stats
  21              		.thumb
  22              		.thumb_func
  24              	compute_stats:
  25              	.LFB29:
  26              		.file 1 "../src/main.c"
   1:../src/main.c **** #include "LPC17xx.h"
   2:../src/main.c **** #include "stdio.h"
   3:../src/main.c **** #define NUM 198
   4:../src/main.c **** 
   5:../src/main.c **** // EE2024 Assignment 1 skeleton code
   6:../src/main.c **** // (C) CK Tham, ECE, NUS, 2018
   7:../src/main.c **** 
   8:../src/main.c ****  // Do not modify the following function prototype
   9:../src/main.c **** extern int asm_stats(int* px, int* pvar, int* pmin, int* pmax);
  10:../src/main.c **** 
  11:../src/main.c **** typedef struct data
  12:../src/main.c **** {
  13:../src/main.c ****  	int x[NUM];
  14:../src/main.c ****  	int mean;
  15:../src/main.c ****  	int variance;
  16:../src/main.c **** 	int min;
  17:../src/main.c ****  	int max;
  18:../src/main.c **** } dataset;
  19:../src/main.c **** 
  20:../src/main.c **** // Do not modify the following function prototype
  21:../src/main.c **** void compute_stats(dataset* dptr)
  22:../src/main.c **** {
  27              		.loc 1 22 0
  28              		.cfi_startproc
  29              		@ args = 0, pretend = 0, frame = 8
  30              		@ frame_needed = 1, uses_anonymous_args = 0
  31 0000 80B5     		push	{r7, lr}
  32              	.LCFI0:
  33              		.cfi_def_cfa_offset 8
  34              		.cfi_offset 14, -4
  35              		.cfi_offset 7, -8
  36 0002 82B0     		sub	sp, sp, #8
  37              	.LCFI1:
  38              		.cfi_def_cfa_offset 16
  39 0004 00AF     		add	r7, sp, #0
  40              	.LCFI2:
  41              		.cfi_def_cfa_register 7
  42 0006 7860     		str	r0, [r7, #4]
  23:../src/main.c **** 	//  Write the code to call asm_stats() function
  24:../src/main.c **** 	//  in an appropriate manner to compute the statistical parameters
  25:../src/main.c **** 	  asm_stats(dptr->x, dptr->variance, dptr->min, dptr->max);
  43              		.loc 1 25 0
  44 0008 7868     		ldr	r0, [r7, #4]
  45 000a 7B68     		ldr	r3, [r7, #4]
  46 000c D3F81C33 		ldr	r3, [r3, #796]
  47 0010 1946     		mov	r1, r3
  48 0012 7B68     		ldr	r3, [r7, #4]
  49 0014 D3F82033 		ldr	r3, [r3, #800]
  50 0018 1A46     		mov	r2, r3
  51 001a 7B68     		ldr	r3, [r7, #4]
  52 001c D3F82433 		ldr	r3, [r3, #804]
  53 0020 FFF7FEFF 		bl	asm_stats
  26:../src/main.c **** }
  54              		.loc 1 26 0
  55 0024 07F10807 		add	r7, r7, #8
  56 0028 BD46     		mov	sp, r7
  57 002a 80BD     		pop	{r7, pc}
  58              		.cfi_endproc
  59              	.LFE29:
  61              		.section	.rodata
  62              		.align	2
  63              	.LC0:
  64 0000 785B2564 		.ascii	"x[%d]: %d\012\000"
  64      5D3A2025 
  64      640A00
  65 000b 00       		.align	2
  66              	.LC1:
  67 000c 6D65616E 		.ascii	"mean: %d\012\000"
  67      3A202564 
  67      0A00
  68 0016 0000     		.align	2
  69              	.LC2:
  70 0018 76617269 		.ascii	"variance: %d\012\000"
  70      616E6365 
  70      3A202564 
  70      0A00
  71 0026 0000     		.align	2
  72              	.LC3:
  73 0028 6D696E3A 		.ascii	"min: %d\012\000"
  73      2025640A 
  73      00
  74 0031 000000   		.align	2
  75              	.LC4:
  76 0034 6D61783A 		.ascii	"max: %d\012\000"
  76      2025640A 
  76      00
  77 003d 000000   		.section	.text.main,"ax",%progbits
  78              		.align	2
  79              		.global	main
  80              		.thumb
  81              		.thumb_func
  83              	main:
  84              	.LFB30:
  27:../src/main.c **** 
  28:../src/main.c **** int main(void)
  29:../src/main.c **** {
  85              		.loc 1 29 0
  86              		.cfi_startproc
  87              		@ args = 0, pretend = 0, frame = 816
  88              		@ frame_needed = 1, uses_anonymous_args = 0
  89 0000 80B5     		push	{r7, lr}
  90              	.LCFI3:
  91              		.cfi_def_cfa_offset 8
  92              		.cfi_offset 14, -4
  93              		.cfi_offset 7, -8
  94 0002 ADF54C7D 		sub	sp, sp, #816
  95              	.LCFI4:
  96              		.cfi_def_cfa_offset 824
  97 0006 00AF     		add	r7, sp, #0
  98              	.LCFI5:
  99              		.cfi_def_cfa_register 7
  30:../src/main.c **** 	int i;
  31:../src/main.c **** 	//  Instantiate a dataset object X
  32:../src/main.c **** 	dataset X;
  33:../src/main.c **** 
  34:../src/main.c **** 	//  Initialize the dataset object X
  35:../src/main.c **** 	for (i=0;i<NUM;i++)
 100              		.loc 1 35 0
 101 0008 4FF00003 		mov	r3, #0
 102 000c C7F82C33 		str	r3, [r7, #812]
 103 0010 0FE0     		b	.L3
 104              	.L4:
  36:../src/main.c **** 		X.x[i] = i-100;
 105              		.loc 1 36 0 discriminator 2
 106 0012 D7F82C33 		ldr	r3, [r7, #812]
 107 0016 A3F16401 		sub	r1, r3, #100
 108 001a 07F10403 		add	r3, r7, #4
 109 001e D7F82C23 		ldr	r2, [r7, #812]
 110 0022 43F82210 		str	r1, [r3, r2, lsl #2]
  35:../src/main.c **** 	for (i=0;i<NUM;i++)
 111              		.loc 1 35 0 discriminator 2
 112 0026 D7F82C33 		ldr	r3, [r7, #812]
 113 002a 03F10103 		add	r3, r3, #1
 114 002e C7F82C33 		str	r3, [r7, #812]
 115              	.L3:
  35:../src/main.c **** 	for (i=0;i<NUM;i++)
 116              		.loc 1 35 0 is_stmt 0 discriminator 1
 117 0032 D7F82C33 		ldr	r3, [r7, #812]
 118 0036 C52B     		cmp	r3, #197
 119 0038 EBDD     		ble	.L4
  37:../src/main.c **** 	X.mean = X.variance = X.min = X.max = 0;
 120              		.loc 1 37 0 is_stmt 1
 121 003a 07F10403 		add	r3, r7, #4
 122 003e 4FF00002 		mov	r2, #0
 123 0042 C3F82423 		str	r2, [r3, #804]
 124 0046 07F10403 		add	r3, r7, #4
 125 004a D3F82423 		ldr	r2, [r3, #804]
 126 004e 07F10403 		add	r3, r7, #4
 127 0052 C3F82023 		str	r2, [r3, #800]
 128 0056 07F10403 		add	r3, r7, #4
 129 005a D3F82023 		ldr	r2, [r3, #800]
 130 005e 07F10403 		add	r3, r7, #4
 131 0062 C3F81C23 		str	r2, [r3, #796]
 132 0066 07F10403 		add	r3, r7, #4
 133 006a D3F81C23 		ldr	r2, [r3, #796]
 134 006e 07F10403 		add	r3, r7, #4
 135 0072 C3F81823 		str	r2, [r3, #792]
  38:../src/main.c **** 
  39:../src/main.c **** 	//  Call compute_stats() function in an appropriate manner to process the dataset object X
  40:../src/main.c **** 	compute_stats(&X);
 136              		.loc 1 40 0
 137 0076 07F10403 		add	r3, r7, #4
 138 007a 1846     		mov	r0, r3
 139 007c FFF7FEFF 		bl	compute_stats
  41:../src/main.c **** 
  42:../src/main.c **** 	//  Print out the contents of the dataset object X
  43:../src/main.c **** 	for (i=0;i<NUM;i++)
 140              		.loc 1 43 0
 141 0080 4FF00003 		mov	r3, #0
 142 0084 C7F82C33 		str	r3, [r7, #812]
 143 0088 14E0     		b	.L5
 144              	.L6:
  44:../src/main.c **** 		printf("x[%d]: %d\n", i, X.x[i]);
 145              		.loc 1 44 0 discriminator 2
 146 008a 40F20003 		movw	r3, #:lower16:.LC0
 147 008e C0F20003 		movt	r3, #:upper16:.LC0
 148 0092 07F10402 		add	r2, r7, #4
 149 0096 D7F82C13 		ldr	r1, [r7, #812]
 150 009a 52F82120 		ldr	r2, [r2, r1, lsl #2]
 151 009e 1846     		mov	r0, r3
 152 00a0 D7F82C13 		ldr	r1, [r7, #812]
 153 00a4 FFF7FEFF 		bl	printf
  43:../src/main.c **** 	for (i=0;i<NUM;i++)
 154              		.loc 1 43 0 discriminator 2
 155 00a8 D7F82C33 		ldr	r3, [r7, #812]
 156 00ac 03F10103 		add	r3, r3, #1
 157 00b0 C7F82C33 		str	r3, [r7, #812]
 158              	.L5:
  43:../src/main.c **** 	for (i=0;i<NUM;i++)
 159              		.loc 1 43 0 is_stmt 0 discriminator 1
 160 00b4 D7F82C33 		ldr	r3, [r7, #812]
 161 00b8 C52B     		cmp	r3, #197
 162 00ba E6DD     		ble	.L6
  45:../src/main.c **** 	printf("mean: %d\n", X.mean);
 163              		.loc 1 45 0 is_stmt 1
 164 00bc 40F20003 		movw	r3, #:lower16:.LC1
 165 00c0 C0F20003 		movt	r3, #:upper16:.LC1
 166 00c4 07F10402 		add	r2, r7, #4
 167 00c8 D2F81823 		ldr	r2, [r2, #792]
 168 00cc 1846     		mov	r0, r3
 169 00ce 1146     		mov	r1, r2
 170 00d0 FFF7FEFF 		bl	printf
  46:../src/main.c **** 	printf("variance: %d\n", X.variance);
 171              		.loc 1 46 0
 172 00d4 40F20003 		movw	r3, #:lower16:.LC2
 173 00d8 C0F20003 		movt	r3, #:upper16:.LC2
 174 00dc 07F10402 		add	r2, r7, #4
 175 00e0 D2F81C23 		ldr	r2, [r2, #796]
 176 00e4 1846     		mov	r0, r3
 177 00e6 1146     		mov	r1, r2
 178 00e8 FFF7FEFF 		bl	printf
  47:../src/main.c **** 	printf("min: %d\n", X.min);
 179              		.loc 1 47 0
 180 00ec 40F20003 		movw	r3, #:lower16:.LC3
 181 00f0 C0F20003 		movt	r3, #:upper16:.LC3
 182 00f4 07F10402 		add	r2, r7, #4
 183 00f8 D2F82023 		ldr	r2, [r2, #800]
 184 00fc 1846     		mov	r0, r3
 185 00fe 1146     		mov	r1, r2
 186 0100 FFF7FEFF 		bl	printf
  48:../src/main.c **** 	printf("max: %d\n", X.max);
 187              		.loc 1 48 0
 188 0104 40F20003 		movw	r3, #:lower16:.LC4
 189 0108 C0F20003 		movt	r3, #:upper16:.LC4
 190 010c 07F10402 		add	r2, r7, #4
 191 0110 D2F82423 		ldr	r2, [r2, #804]
 192 0114 1846     		mov	r0, r3
 193 0116 1146     		mov	r1, r2
 194 0118 FFF7FEFF 		bl	printf
 195              	.L7:
  49:../src/main.c **** 
  50:../src/main.c **** 	// Enter an infinite loop, just incrementing a counter.
  51:../src/main.c **** 	// Do not modify the code below. It enables values or variables and registers to be inspected befo
  52:../src/main.c **** 	volatile static int loop = 0;
  53:../src/main.c **** 	while (1) {
  54:../src/main.c **** 		loop++;
 196              		.loc 1 54 0 discriminator 1
 197 011c 40F20003 		movw	r3, #:lower16:loop.4859
 198 0120 C0F20003 		movt	r3, #:upper16:loop.4859
 199 0124 1B68     		ldr	r3, [r3, #0]
 200 0126 03F10102 		add	r2, r3, #1
 201 012a 40F20003 		movw	r3, #:lower16:loop.4859
 202 012e C0F20003 		movt	r3, #:upper16:loop.4859
 203 0132 1A60     		str	r2, [r3, #0]
  55:../src/main.c **** 	}
 204              		.loc 1 55 0 discriminator 1
 205 0134 F2E7     		b	.L7
 206              		.cfi_endproc
 207              	.LFE30:
 209 0136 00BF     		.bss
 210              		.align	2
 211              	loop.4859:
 212 0000 00000000 		.space	4
 213              		.text
 214              	.Letext0:
 215              		.file 2 "C:\\Users\\julia\\Documents\\LPCXpresso_6.1.4_194\\workspace\\Lib_CMSISv1p30_LPC17xx\\inc
DEFINED SYMBOLS
                            *ABS*:00000000 main.c
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:19     .text.compute_stats:00000000 $t
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:24     .text.compute_stats:00000000 compute_stats
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:62     .rodata:00000000 $d
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:63     .rodata:00000000 .LC0
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:66     .rodata:0000000c .LC1
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:69     .rodata:00000018 .LC2
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:72     .rodata:00000028 .LC3
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:75     .rodata:00000034 .LC4
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:78     .text.main:00000000 $t
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:83     .text.main:00000000 main
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:211    .bss:00000000 loop.4859
C:\Users\julia\AppData\Local\Temp\cc2Byyju.s:210    .bss:00000000 $d
                     .debug_frame:00000010 $d

UNDEFINED SYMBOLS
asm_stats
printf
