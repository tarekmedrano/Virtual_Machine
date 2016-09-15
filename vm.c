//Compiler Builder 15
//Austin Dragone
//Gabriela Fisher
//Jerasimos Strakosha
//Tarek Medrano
//Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

int stack[ MAX_STACK_HEIGHT ];

struct {
	int op;
	int l;
	int m;
} instruction;

//read in pm0 code, print assembler version, and execute 
int main() {
	
	//Initial values
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    
    int sp = 0;
    int bp = 1;
    int pc = 0;
    int ir = 0;
	
	
	
	return 0;
}


//Instruction Set Architecture (ISA)
//These are the basic instructions provided on Wocjan's bitbucket

//LIT - Push value M onto stack
//Austin
void lit() {
	
}

//OPR - Perform arithmetic or logical operations defined in detail below
void opr() {
	
}

//LOD - Get value at offset M in frame L levels down and push it
void lod() {
	
}

//STO - Pop stack and insert value at offset M in frame L levels down
void sto() {
	
}

//CAL - Call procedure at M (generates new stack frame
void cal() {
	
}

//INC - Allocate M locals on stack
void inc() {
	
}

//JMP - Jumpt to M
void jmp() {
	
}

//JPC - Pop stack and jump to M if value is equal to 0
void jpc() {
	
}

//SIO - Depending on the modifier it will either
//Pop stack and print out value,
//Read in input from user and push it,
//or Halt the machine
void sio() {
	
}

//Arithmetic/ logic instructions for OPR instruction
void arithmeticInstruction() {
	
}










