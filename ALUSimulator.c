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

#define WORDSIZE 32

//Enumerate the function codes for ease of understanding
enum Function_Code {NOOP_SLL = 0b000000, SRL = 0b000010, SRA = 0b000011,
 SLLV = 0b000100, SRLV = 0b000110, MFHI = 0b010000, MFLO = 0b010010,
 MULT = 0b011000, MULTU = 0b011001, DIV = 0b011010, DIVU = 0b011011,
 ADD = 0b100000, ADDU = 0b100001, SUB = 0b100010, SUBU = 0b100011,
 AND = 0b100100, OR = 0b100101, XOR = 0b100110, SLT = 0b101010, SLTU = 0b101011};

//Enumerate the OP codes for ease of understanding
enum Op_Code{Non_I_Type = 0b000000, ADDI = 0b001000, ADDIU = 0b001001, SLTI = 0b001010, SLTIU = 001011};

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

	//Create two pointers that will be used to access the register
	uint32_t RsVal = 0;
	uint32_t RtVal = 0;

	//Create an it to hold the value to be written to the register
	uint32_t RdVal = 0;
	//Access the register at the given addresses
	RegisterFile_Read(theRegisterFile, Rs, &RsVal, Rt, &RtVal);
				
	switch(OpCode)
	{
		case ADDI:
		//Fall through to ADDIU since both cases perform the same
		case ADDIU:
		RdVal = (RsVal + ImmediateValue);
		RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
		break;
		case SLTI:
		case SLTIU:
		RdVal = (RsVal < ImmediateValue ? 1 : 0);
		RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
		break;
		case Non_I_Type:
		switch(FunctionCode)
		{
			case NOOP_SLL:
			RdVal = RsVal << ShiftAmt;
			break;
			case SRL:
			RdVal = RsVal >> ShiftAmt;
			break;
			case SRA:
			RdVal = RsVal >> ShiftAmt;
			break;
			case SLLV:
			RdVal = RtVal << RsVal;
			break;
			case SRLV:
			RdVal = RtVal >> RsVal;
			break;
			case MFHI:
			RegisterFile_Read(theRegisterFile, 0b011111, &RsVal, Rt, &RtVal);
			RdVal = RsVal;
			break;
			case MFLO:
			RegisterFile_Read(theRegisterFile, 0b011110, &RsVal, Rt, &RtVal);
			RdVal = RsVal;
			break;
			case MULT:
			//Fall through to MULTU since both cases perform the same
			case MULTU:
			//Multiply the values together and get the full value
			uint64_t fullMult = RsVal * RtVal;
			//Create a mask that is WORDSIZE number of digits
			uint32_t mask = (1 << WORDSIZE) - 1;
			//Grab the first WORDSIZE digits from the multiplication
			uint32_t lowVal = fullMult & mask;
			//Grab the last WORDSIZE digits from the multiplication
			uint32_t highVal = (fullMult >> WORDSIZE) & mask;
			//Write lowVal and highVal to the low and high registers respectively
			RegisterFile_Write(theRegisterFile, true, 0b011110, lowVal);
			RegisterFile_Write(theRegisterFile, true, 0b011111, highVal);
			return;
			break;
			case DIV:
			//Fall through to DIVU since both cases perform the same
			case DIVU:
			RegisterFile_Write(theRegisterFile, true, 0b011110, RsVal % RtVal);
			RegisterFile_Write(theRegisterFile, true, 0b011111, RsVal / RtVal);
			return;
			break;
			case ADD:
			//Fall through to ADDU since both cases perform the same
			case ADDU:
			RdVal = RsVal + RtVal;
			break;
			case SUB:
			//Fall through to SUBU since both cases perform the same
			case SUBU:
			RdVal = RsVal - RtVal;
			break;
			case AND:
			RdVal = RsVal & RtVal;
			break;
			case OR:
			RdVal = RsVal | RtVal;
			break;
			case XOR:
			RdVal = RsVal ^ RtVal;
			break;
			case SLT:
			case SLTU:
			RdVal = (RsVal < RtVal ? 1 : 0);
			break;
		}
		break;
	}
	RegisterFile_Write(theRegisterFile, true, Rd, RdVal);
}
