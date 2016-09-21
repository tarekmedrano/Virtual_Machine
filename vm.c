//Compiler Builder 15
//Austin Dragone
//Gabriela Fisher
//Jerasimos Strakosha
//Tarek Medrano
//Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_ARG_LENGTH 16

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

int sp = 0;
int bp = 1;
int pc = 0;

struct instruction{
	int op;
	int l;
	int m;
} ir, code[MAX_CODE_LENGTH];
int numInstructions = 0;

void fetch();
void execute();

int stack[ MAX_STACK_HEIGHT ];
struct instruction code[ MAX_CODE_LENGTH ];

void lit();
int opr();
void lod();
void sto();
void cal();
void inc();
void jmp();
void jpc();
void sio();
int base(int level, int b);


void readInstruction(struct instruction* ir, char* line, int len);
int nextToken(char* token, int tokenLen, char* line, int lineLen, int index);

//read in pm0 code, print assembler version, and execute 
int main(int argc, char** argv) {
	
    char* argument;
    char line[MAX_LINE_LENGTH];
    
    argc--; argv++;
	argument = *argv;
    
    FILE* file = fopen(argument,"r");
   
    int c = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        
        struct instruction instruct;
        readInstruction(&instruct, line, MAX_LINE_LENGTH);
        code[c] = instruct;
        
        c++;
    }
    numInstructions = c;
    
    //Initial values
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    
    while (1) {
        fetch();
        execute();
    }
	
    return 0;
}

void fetch() {
    ir = code[pc];
    pc = pc + 1;
}

void execute() {
    //won't happen if last instruction is [9 0 2]
    if (pc>numInstructions) {
        printf("No more instructions to run.\n"); 
        exit(1);
    } else {
        switch (ir.op) {
        //case 1: lit();
        //case 2: opr();
        //case 3: lod();
        //case 4: sto();
        //case 5: cal();
        //case 6: inc();
        //case 7: jmp();
        //case 8: jpc();
        case 9: sio();
        }
    }
}


//Instruction Set Architecture (ISA)
//These are the basic instructions provided on Wocjan's bitbucket

//LIT
//Increment stack pointer by 1 then push instruction modifier onto stack
//This is assuming the stack pointer points to data so we must increment first
//Austin
void lit() {
	sp = sp + 1;
	stack[sp] = ir.m;
}


//Base
//Tarek
//Calculates the base of activation record L levels down
int base(int level, int b){
	while (level > 0 ){
		b = stack[b + 1];
		level --;
	}
	return b;
}


//OPR
//Tarek
//Reads in the instruction modifier to decide what arithmetic/logical operation
//to run
int opr() {
	switch(ir.m){
	case 0:
		//exit if we are at the base level
		if(bp == 1)
			return 0;
		sp = bp -1;
		pc = stack[sp + 4];
		bp = stack[sp + 3];
		break;
	
	
	//NEG
	case 1:
		stack[sp] = -stack[sp];
		break;
		
	//for all the below cases, -1 from the sp		
	//ADD	
	case 2:
		sp -= 1;
		stack[sp] = stack[sp] + stack[sp +1];
		break;
		
	//SUB
	case 3:
		sp -= 1;
		stack[sp] = stack[sp] * stack[sp +1];
		break;
		
	//MUL
	case 4:
		sp -= 1;
		stack[sp] = stack[sp] * stack[sp +1];
		break;
	
	//DIV
	case 5:	
		sp -= 1;
		stack[sp] = stack[sp]/stack[sp +1];
		break;
		
	//ODD
	case 6:
		sp -= 1;
		stack[sp] = stack[sp] %2;
		break;
		
	//MOD
	case 7:
		sp -= 1;
		stack[sp] = stack[sp] % stack[(sp) +1];
		break;
	
	//EQL
	case 8:
		sp -= 1;
		stack[sp] = stack[sp] == stack[sp +1];
		break;
		
	//NEQ
	case 9:
		sp -= 1;
		stack[sp] = stack[sp] != stack[sp +1];
		break;
	
	//LSS
	case 10:
		sp -= 1;
		stack[sp] = stack[sp] < stack[sp +1];
		break;		
	
	//LEQ
	case 11:
		sp -= 1;
		stack[sp] = stack[sp] <= stack[sp +1];
		break;		
	
	//GTR
	case 12:
		sp -= 1;
		stack[sp] = stack[sp] > stack[sp +1];
		break;		
	
	//GEQ
	case 13:	
		sp -= 1;
		stack[sp] = stack[sp] >= stack[sp +1];
		break;
		
	}
	
	return 1;
}

//Tarek
//LOD - Get value at offset M in frame L levels down and push it
void lod() {
	sp = sp + 1;
	stack[sp] = stack[base(ir.l,bp) + ir.m];  
	
	
}

//Tarek
//STO - Pop stack and insert value at offset M in frame L levels down
void sto() {
	stack [base(ir.l, bp) + ir.m] = stack[sp];
	sp -= 1;
}

//Tarek
//CAL - Call procedure at M (generates new stack frame
void cal() {
	stack[sp + 1] = 0;
	stack[sp + 2] = base(ir.l, bp);
	stack[sp + 3] = bp;
	stack[sp + 4] = pc;
	bp = sp + 1;
	pc = ir.m; 
}

//Tarek
//INC - Allocate M locals on stack
void inc() {
	sp = sp + ir.m;
	
}
//Gabriela
//Work in progress
//JMP - Jump to M
void jmp() {
	pc = ir.m;
}
//Gabriela
//Work in progress
//JPC - Pop stack and jump to M if value is equal to 0
void jpc() {
	if(stack[sp] == 0){
		pc = ir.m;
	}
	sp -= 1;
}

//Jonathan
//I'm assuming he wants us to read and print from console?
//SIO - Depending on the modifier it will either
//Pop stack and print out value,
//Read in input from user and push it,
//or Halt the machine
void sio() {
    
    switch (ir.m) {
        
    //Output (OUT)
    case 0:
        printf("%d\n",stack[sp]);
        sp = sp - 1;
        
    //Input (INP)
    case 1:
        sp = sp + 1;
        scanf("%d",&stack[sp]);
    
    //Halt (HLT)
    case 2:
        exit(0);
    
    }
}

void readInstruction(struct instruction* instruct, char* line, int len) {
    char arg0[MAX_ARG_LENGTH];
    char arg1[MAX_ARG_LENGTH];
    char arg2[MAX_ARG_LENGTH];
    
    int index = 0;
    index = nextToken(arg0,MAX_ARG_LENGTH,line,len,index);
    index = nextToken(arg1,MAX_ARG_LENGTH,line,len,index);
    index = nextToken(arg2,MAX_ARG_LENGTH,line,len,index);
    
    instruct->op = atoi(arg0);
    instruct->l = atoi(arg1);
    instruct->m = atoi(arg2); 
}

//Char at line[index] must not be a space
//Only works with tokens separated by a single space
int nextToken(char* token, int tokenLen, char* line, int lineLen, int index) {
    int tokenIsDone = 0;
    int i = 0;
    while (index<lineLen) {
        char c = line[index];

        if (c=='\0') {
            token[i] = '\0';
            return lineLen;
        }
        token[i] = c;
        if (c==' ') {
            if (!tokenIsDone) {
                token[i] = '\0';
                tokenIsDone = 1;
                index++;
                break;
            }
        } else {
            token[i] = c;
            i++;
            if (i==tokenLen)
                tokenIsDone = 1;
        }
        index++;
    }
    if (!tokenIsDone) return -1;
    return index;
}
