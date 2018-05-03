//*****************************************************************************
//--ALUSimulator.c
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 645
//		Date:			2017-04-22 (B70422)
//		Version:		1.0
//		Description:	This is the test standin for a simple ALU simulator
//		Notes:
//
//*****************************************************************************
//

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include <stdio.h>

#include "RegisterFile_01.h"
#include "ALUSimulator.h"

//Enumerate the function codes for ease of understanding
enum Function_Code {NOOP_SLL = 0b000000, SRL = 0b000010, SRA = 0b000011,
 SLLV = 0b000100, SRLV = 0b000110, MFHI = 0b010000, MFLO = 0b010010,
 MULT = 0b011000, MULTU = 0b011001, DIV = 0b011011, DIVU = 0b011011,
 ADD = 0b100000, ADDU = 0b100001, SUB = 0b100010, SUBU = 0b100011,
 AND = 0b100100, OR = 0b100101, XOR = 0b100110, SLT = 0b101010, SLTU = 0b101011};

//Enumerate the OP codes for ease of understanding
enum Op_Code(Non_I_Type = 0, ADDI = 8, ADDIU = 9, STLI = 10, STLIU = 11);

extern void ALUSimulator( RegisterFile theRegisterFile,
				uint32_t OpCode,
				uint32_t Rs, uint32_t Rt, uint32_t Rd,
				uint32_t ShiftAmt,
				uint32_t FunctionCode,
				uint32_t ImmediateValue,
				uint32_t* Status ) {
				
	printf( ">>ALU: Opcode: %02X; Rs: %02X; Rt: %02X; Rd: %02X;\n",
				OpCode,
				Rs,
				Rt,
				Rd );
				
	printf( ">>>>ALU: ShiftAmt: %02X; FunctionCode: %02X; ImmediateValue: %04X;\n",
				ShiftAmt,
				FunctionCode,
				ImmediateValue );
				
}
