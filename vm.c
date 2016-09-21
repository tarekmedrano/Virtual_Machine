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

struct instruction{
	int op;
	int l;
	int m;
} ;

void lit(struct instruction instruct, int stack[], int* sp);
int opr(struct instruction instruct, int stack[], int* sp, int* bp, int* pc);

//read in pm0 code, print assembler version, and execute 
int main(int argc, char** argv) {
	
    char* argument;
    
    argc--; argv++;
	argument = *argv;
    
    FILE* file = fopen(argument,"r");
    
    //Initial values
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    
    int sp = 0;
    int bp = 1;
    int pc = 0;
    int ir = 0;
	
    //Store the current instruction here to be
    //passed into the functions
    //start with initial values
    struct instruction instruct;
    instruct.op = 0;
    instruct.l = 0;
    instruct.m = 0;	
	
    return 0;
}


//Instruction Set Architecture (ISA)
//These are the basic instructions provided on Wocjan's bitbucket

//LIT
//Increment stack pointer by 1 then push instruction modifier onto stack
//This is assuming the stack pointer points to data so we must increment first
//Austin
void lit( struct instruction instruct, int stack[], int* sp) {
	*sp = *sp + 1;
	stack[*sp] = instruct.m;
}

//OPR
//Tarek
//Reads in the instruction modifier to decide what arithmetic/logical operation
//to run
int opr(struct instruction instruct, int stack[], int* sp, int* bp, int* pc) {
	switch(instruct.m){
	case 0:
		//exit if we are at the base level
		if(*bp == 1)
			return 0;
		*sp = *bp -1;
		*pc = stack[*sp + 4];
		*bp = stack[*sp + 3];
		break;
	
	
	//NEG
	case 1:
		stack[*sp] = -stack[*sp];
		break;
		
	//for all the below cases, -1 from the sp		
	//ADD	
	case 2:
		*sp -= 1;
		stack[*sp] = stack[*sp] + stack[*sp +1];
		break;
		
	//SUB
	case 3:
		*sp -= 1;
		stack[*sp] = stack[*sp] * stack[*sp +1];
		break;
		
	//MUL
	case 4:
		*sp -= 1;
		stack[*sp] = stack[*sp] * stack[*sp +1];
		break;
	
	//DIV
	case 5:	
		*sp -= 1;
		stack[*sp] = stack[*sp]/stack[*sp +1];
		break;
		
	//ODD
	case 6:
		*sp -= 1;
		stack[*sp] = stack[*sp] %2;
		break;
		
	//MOD
	case 7:
		*sp -= 1;
		stack[*sp] = stack[*sp] % stack[(*sp) +1];
		break;
	
	//EQL
	case 8:
		*sp -= 1;
		stack[*sp] = stack[*sp] == stack[*sp +1];
		break;
		
	//NEQ
	case 9:
		*sp -= 1;
		stack[*sp] = stack[*sp] != stack[*sp +1];
		break;
	
	//LSS
	case 10:
		*sp -= 1;
		stack[*sp] = stack[*sp] < stack[*sp +1];
		break;		
	
	//LEQ
	case 11:
		*sp -= 1;
		stack[*sp] = stack[*sp] <= stack[*sp +1];
		break;		
	
	//GTR
	case 12:
		*sp -= 1;
		stack[*sp] = stack[*sp] > stack[*sp +1];
		break;		
	
	//GEQ
	case 13:	
		*sp -= 1;
		stack[*sp] = stack[*sp] >= stack[*sp +1];
		break;
		
	}
	
	return 1;
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
//Gabriela
//Work in progress
//JMP - Jumpt to M
void jmp(int* pc, struct instruction instruct) {
	*pc = instruct.m;
}
//Gabriela
//Work in progress
//JPC - Pop stack and jump to M if value is equal to 0
void jpc(int* pc, int* sp, struct instruction instruct) {
	if(stack[*sp] == 0){
		*pc = instruct.m;
	}
	*sp -= 1;
}

//Jonathan
//I'm assuming he wants us to read and print from console?
//SIO - Depending on the modifier it will either
//Pop stack and print out value,
//Read in input from user and push it,
//or Halt the machine
void sio(struct instruction instruct, int* sp) {
    switch (instruct.m) {
        
    //Output (OUT)
    case 0:
        printf("%d\n",stack[*sp]);
        *sp = *sp - 1;
        
    //Input (INP)
    case 1:
        *sp = *sp + 1;
        scanf("%d",&stack[*sp]);
    
    //Halt (HLT)
    case 2:
        exit(0);
    
    }
}











