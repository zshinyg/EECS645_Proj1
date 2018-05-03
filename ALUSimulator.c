//*****************************************************************************
//
//
//		Authors: 		Blaine Edmondson, Coltyn Gatton, Noah
// Kenn
//		Organization:	KU/EECS/EECS 645
//		Date:			2018-05-02
//		Last edited:	2018-05-03
//		Class:			EECS 645
//		Description:	A simulator of a MIPS ALU
//
//*****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "RegisterFile_01.h"

#include "ALUSimulator.h"

// Set wordsize to be 32
#define WORDSIZE 32

// Create a mask that is 32 bits for use with mult
#define MASK (0xFFFFFFFF);

// Enumerate the function codes for ease of understanding
enum Function_Code {
  NOOP_SLL = 0b000000,
  SRL = 0b000010,
  SRA = 0b000011,
  SLLV = 0b000100,
  SRLV = 0b000110,
  MFHI = 0b010000,
  MFLO = 0b010010,
  MULT = 0b011000,
  MULTU = 0b011001,
  DIV = 0b011010,
  DIVU = 0b011011,
  ADD = 0b100000,
  ADDU = 0b100001,
  SUB = 0b100010,
  SUBU = 0b100011,
  AND = 0b100100,
  OR = 0b100101,
  XOR = 0b100110,
  SLT = 0b101010,
  SLTU = 0b101011
};

// Enumerate the OP codes for ease of understanding
enum Op_Code {
  Non_I_Type = 0b000000,
  ADDI = 0b001000,
  ADDIU = 0b001001,
  SLTI = 0b001010,
  SLTIU = 001011
};

extern void ALUSimulator(RegisterFile theRegisterFile, uint32_t OpCode,
                         uint32_t Rs, uint32_t Rt, uint32_t Rd,
                         uint32_t ShiftAmt, uint32_t FunctionCode,
                         uint32_t ImmediateValue, uint32_t *Status) {

  // Create two integers that will be used to access the register
  uint32_t RsVal = 0;
  uint32_t RtVal = 0;

  // Access the registers at the given addresses by passing pointers to the
  // above integers
  RegisterFile_Read(theRegisterFile, Rs, &RsVal, Rt, &RtVal);

  // Create an integer to hold the value to be written to the register
  uint32_t RdVal = 0;

  switch (OpCode) {
  case ADDI:
    // Cast RsVal to signed int32_t and ImmediateValue to signed int16_t
    // Add those together and save them to RdVal
    RdVal = ((int32_t)RsVal + (int16_t)ImmediateValue);
    //Write RdVal to register Rt
    RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
    return;
    break;

  case ADDIU:
    // Cast ImmediateValue to signed int16_t
    // Add that to RsVal and save the result to RdVal
    RdVal = (RsVal + (int16_t)ImmediateValue);
    //Write RdVal to register Rt
    RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
    return;
    break;

  case SLTI:
    // Cast RsVal to signed int32_t and ImmediateValue to signed int16_t
    // Use a ternary statement to set the value of RdVal
    RdVal = ((int32_t)RsVal < (int16_t)ImmediateValue ? 1 : 0);
    //Write RdVal to register Rt
    RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
    return;
    break;

  case SLTIU:
    // Cast ImmediateValue to signed int16_t
    // Use a ternary statement to set the value of RdVal
    RdVal = (RsVal < (int16_t)ImmediateValue ? 1 : 0);
    //Write RdVal to register Rt
    RegisterFile_Write(theRegisterFile, true, Rt, RdVal);
    return;
    break;

  case Non_I_Type:
    switch (FunctionCode) {
    case NOOP_SLL:
      // Bit-Shift the value of Rt left by ShiftAmt
      RdVal = RtVal << ShiftAmt;
      break;

    case SRL:
      // Bit-Shift the value of Rt right by ShiftAmt
      RdVal = RtVal >> ShiftAmt;
      break;

    case SRA:
      // Cast RtVal to int32_t
      // Bit-Shift the value of Rt right by ShiftAmt
      RdVal = (int32_t)RtVal >> ShiftAmt;
      break;

    case SLLV:
      // Bit-shift Rt left by RsVal
      RdVal = RtVal << RsVal;
      break;

    case SRLV:
      // Bit-shift Rt right by RsVal
      RdVal = RtVal >> RsVal;
      break;

    case MFHI:
      // Read the value in the high register (and Rt, but we ignore this)
      RegisterFile_Read(theRegisterFile, 0b011111, &RsVal, Rt, &RtVal);
      // Set Rd to the value of Rs
      RdVal = RsVal;
      break;

    case MFLO:
      // Read the value in the low register (and Rt, but we ignore this)
      RegisterFile_Read(theRegisterFile, 0b011110, &RsVal, Rt, &RtVal);
      // Set Rd to the value of Rs
      RdVal = RsVal;
      break;

    case MULT:;
      // Multiply the values together and get the full value
      int64_t fullMult = (int32_t)RsVal * (int32_t)RtVal;
      // Grab the first WORDSIZE digits from the multiplication
      int32_t lowVal = fullMult & MASK;
      // Grab the last WORDSIZE digits from the multiplication
      int32_t highVal = (fullMult >> WORDSIZE) & MASK;
      // Write lowVal and highVal to the low and high registers respectively
      RegisterFile_Write(theRegisterFile, true, 0b011110, lowVal);
      RegisterFile_Write(theRegisterFile, true, 0b011111, highVal);
      return;
      break;

    case MULTU:;
      // Multiply the values together and get the full value
      uint64_t ufullMult = RsVal * RtVal;
      // Grab the first WORDSIZE digits from the multiplication
      uint32_t ulowVal = ufullMult & MASK;
      // Grab the last WORDSIZE digits from the multiplication
      uint32_t uhighVal = (ufullMult >> WORDSIZE) & MASK;
      // Write lowVal and highVal to the low and high registers respectively
      RegisterFile_Write(theRegisterFile, true, 0b011110, ulowVal);
      RegisterFile_Write(theRegisterFile, true, 0b011111, uhighVal);
      return;
      break;

    case DIV:
      // Cast RsVal and RtVal to int32_t
      // Write the remainder of the division of Rs and Rt to the low Register
      RegisterFile_Write(theRegisterFile, true, 0b011110,
                         (int32_t)RsVal % (int32_t)RtVal);
      // Cast RsVal and RtVal to int32_t
      // Write the quotient of the division of Rs and Rt to the high Register
      RegisterFile_Write(theRegisterFile, true, 0b011111,
                         (int32_t)RsVal / (int32_t)RtVal);
      return;
      break;

    case DIVU:
      // Write the remainder of the division of Rs and Rt to the low Register
      RegisterFile_Write(theRegisterFile, true, 0b011110, RsVal % RtVal);
      // Write the quotient of the division of Rs and Rt to the hight Register
      RegisterFile_Write(theRegisterFile, true, 0b011111, RsVal / RtVal);
      return;
      break;

    case ADD:
      // Cast RsVal and RtVal to int32_t
      // Add these values and save to RdVal
      RdVal = (int32_t)RsVal + (int32_t)RtVal;
      break;

    case ADDU:
      // Add RsVal and RtVal and save to RdVal
      RdVal = RsVal + RtVal;
      break;

    case SUB:
      // Cast RsVal and RtVal to int32_t
      // Subtract RtVal from RsVal and save to RdVal
      RdVal = (int32_t)RsVal - (int32_t)RtVal;
      break;

    case SUBU:
      // Subtract RtVal from RsVal and save to RdVal
      RdVal = RsVal - RtVal;
      break;

    case AND:
      // Set RdVal to the bitwise AND of RsVal and RtVal
      RdVal = RsVal & RtVal;
      break;

    case OR:
      // Set RdVal to the bitwise OR of RsVal and RtVal
      RdVal = RsVal | RtVal;
      break;

    case XOR:
      // Set RdVal to the bitwise XOR of RsVal and RtVal
      RdVal = RsVal ^ RtVal;
      break;

    case SLT:
      // Cast RsVal and RtVal to int32_t
      // Use a ternary statement to set the value of RdVal
      RdVal = ((int32_t)RsVal < (int32_t)RtVal ? 1 : 0);
      break;

    case SLTU:
      // Use a ternary statement to set the value of RdVal
      RdVal = (RsVal < RtVal ? 1 : 0);
      break;
    }
    break;
  }
  // Write the value of the RdVal to the register Rd
  RegisterFile_Write(theRegisterFile, true, Rd, RdVal);
}
