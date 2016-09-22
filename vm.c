//Compiler Builder 15
//Austin Dragone
//Gabriela Fisher
//Jerasimos Strakosha
//Tarek Medrano
//Jonathan Lacombe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Set to 0 when turning in assignment
#define DEBUG               0

#define MAX_LINE_LENGTH     256
#define MAX_ARG_LENGTH      16

#define MAX_STACK_HEIGHT    2000
#define MAX_CODE_LENGTH     500
#define MAX_LEXI_LEVELS     3

#define ISIZE               3 //length of instructions

char codeOutput[MAX_LINE_LENGTH][MAX_CODE_LENGTH];

int  sp = 0;
int  bp = 1;
int  pc = 0;

int  halt = 0;

struct instruction{
	int op;
	int l;
	int m;
} ir, code[MAX_CODE_LENGTH];
int  numInstructions = 0;

int  stack[ MAX_STACK_HEIGHT ];

void initialize();
void printCode();
void printExecution();
void fetch();
int  execute();

void lit();
int  opr();
void lod();
void sto();
void cal();
void inc();
void jmp();
void jpc();
void sio();

int  readInstruction(struct instruction* ir, char* line, int len);
int  nextToken(char* token, int tokenLen, char* line, int lineLen, int index);
int  base( int level, int b);

//read in pm0 code, print assembler version, and execute 
int main(int argc, char** argv) {
	
    char* argument;
    char line[MAX_LINE_LENGTH];
    
    argc--; argv++;
    argument = *argv;
    
    FILE* file = fopen(argument,"r");

    if (file==NULL) {
        printf("File not found: %s\n",argument);
        exit(0);
    }
    
    int c = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        struct instruction instruct;
        
        int hasIr = readInstruction(&instruct, line, MAX_LINE_LENGTH);
        
        if (!hasIr) break;
        
        code[c] = instruct;
        
        c++;
    }
    
    numInstructions = c;
    
    initialize();

    printCode();
    
    printf("Execution:\n");
    printf("pc bp sp stack\n");
    printf("%d  %d  %d\n",pc,bp,sp);
    
    while (!halt) {
        int index = pc;
        fetch();
        execute();
        printExecution(index);
    }
    
    return 0;
}

void initialize() {
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
}

//Does this have to be formated into columns like in trace files?
void printCode() {
    printf("PM/0 code:\n\n");
    int i;
    for (i=0; i<numInstructions; i++) {
        struct instruction isr = code[i];
        char isO[ISIZE+1] = "NUL";
        char isL[MAX_ARG_LENGTH];
        char isM[MAX_ARG_LENGTH];
        
	if( i < 100 )
	    printf(" ");
	else if( i < 10 )
	    printf(" ");
        sprintf(isL,"%d",isr.l);
        sprintf(isM,"%2d",isr.m);
        
        switch (isr.op) {
        case 1: strncpy(isO,"LIT",ISIZE); strncpy(isL," ",ISIZE); break;
        case 2:
            switch(isr.m) {
            case 0:  strncpy(isO,"RET",ISIZE); break;
            	     strncpy(isL," ",ISIZE); strncpy(isM,"  ",ISIZE); 
            case 1:  strncpy(isO,"NEG",ISIZE); break;
            case 2:  strncpy(isO,"ADD",ISIZE); break;
            case 3:  strncpy(isO,"SUB",ISIZE); break;
            case 4:  strncpy(isO,"MUD",ISIZE); break;
            case 5:  strncpy(isO,"DIV",ISIZE); break;
            case 6:  strncpy(isO,"ODD",ISIZE); break;
            case 7:  strncpy(isO,"MOD",ISIZE); break;
            case 8:  strncpy(isO,"EQL",ISIZE); break;
            case 9:  strncpy(isO,"NEQ",ISIZE); break;
            case 10: strncpy(isO,"LSS",ISIZE); break;
            case 11: strncpy(isO,"LEQ",ISIZE); break;
            case 12: strncpy(isO,"GTR",ISIZE); break;
            case 13: strncpy(isO,"GEQ",ISIZE); break;
            }
            break;
        case 3: strncpy(isO,"LOD",ISIZE); break;
        case 4: strncpy(isO,"STO",ISIZE); break;
        case 5: strncpy(isO,"CAL",ISIZE); break;
        case 6: strncpy(isO,"INC",ISIZE); strncpy(isL," ",ISIZE); break;
        case 7: strncpy(isO,"JMP",ISIZE); strncpy(isL," ",ISIZE); break;
        case 8: strncpy(isO,"JPC",ISIZE); strncpy(isL," ",ISIZE); break;
        case 9:
            switch(isr.m) {
            case 0: strncpy(isO,"OUT",ISIZE); break;
            case 1: strncpy(isO,"INP",ISIZE); break;
            case 2: strncpy(isO,"HLT",ISIZE); break;
            }
            strncpy(isL," ",ISIZE); strncpy(isM,"  ",ISIZE); break;
        default: break;
        }
        
        //sprintf(codeOutput[i],"%d %s %s %s",i,isO,isL,isM);
        sprintf(codeOutput[i],"%2d %s %s %s ",i,isO,isL,isM);
        printf("%s\n",codeOutput[i]);
    }
    printf("\n");
}

void printExecution(int index) {
    printf("%s  %2d  %2d  %2d  ",codeOutput[index],pc,bp,sp);
    //printf("%s \t %d  %d  %d  \t",codeOutput[index],pc,bp,sp);
    printf("\t");
    
    //why start at i=1? whats stack[0]?
    //he said stack[0] should always be 0 -Austin
    int i;
    printf("  ");
    for (int i=1; i<sp+1; i++) {
        printf("%d  ",stack[i]);
    }
    printf("\n");
}

void fetch() {
    ir = code[pc];
    pc = pc + 1;
}

int execute() {
    //won't happen if last instruction is [9 0 2]
    if (pc>numInstructions) {
#if DEBUG
        printf("No more instructions to run.\n"); 
#endif
        exit(0);
    } else {
        switch (ir.op) {
        case 1: lit(); return 1;
        case 2: opr(); return 1;
        case 3: lod(); return 1;
        case 4: sto(); return 1;
        case 5: cal(); return 1;
        case 6: inc(); return 1;
        case 7: jmp(); return 1;
        case 8: jpc(); return 1;
        case 9: sio(); return 1;
        default: return 0;
        }
    }
    return 0;
}


//Instruction Set Architecture (ISA)
//These are the basic instructions provided on Wocjan's bitbucket

//LIT
//Increment stack pointer by 1 then push instruction modifier onto stack
//Increment first since sp starts at 0, and stack[0] shouldn't be changed from 0
//Austin
void lit() {
    sp = sp + 1;
    stack[sp] = ir.m;
	
#if DEBUG
    printf("LIT set stack[sp=%d]=%d\n",sp,ir.m);
#endif
}

//OPR
//Tarek
//Reads in the instruction modifier to decide what arithmetic/logical operation
//to run
int opr() {
	switch(ir.m){
	
	case 0:
		//exit if we are at the base level
		if(bp == 1) {
#if DEBUG
            printf("OPR RET at base level");
#endif
			return 0;
		}
		sp = bp - 1;
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
		
#if DEBUG
	default:
        printf("Error finding ir.m in OPR\n");
        break;
#endif
		
	}
	
	return 1;
}

//LOD - Get value at offset M in frame L levels down and push it
//Jerasimos
void lod() {
	sp = sp + 1;
	stack[sp] = stack[base(ir.l, bp) + ir.m];
}

//STO - Pop stack and insert value at offset M in frame L levels down
//Jerasimos
void sto() {
	stack[ base(ir.l, bp) + ir.m ] = stack[sp];
	sp = sp - 1;
}

//CAL - Call procedure at M (generates new stack frame
//Jerasimos
void cal() {
	stack[sp + 1] = 0;
	stack[sp + 2] = base(ir.l, bp);
	stack[sp + 3] = bp;
	stack[sp + 4] = pc;
	bp = sp + 1;
	pc = ir.m;
}

//INC - Allocate M locals on stack
//Jerasimos
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
        break;
        
    //Input (INP)
    case 1:
        sp = sp + 1;
        scanf("%d",&stack[sp]);
        break;
    
    //Halt (HLT)
    case 2:
        halt = 1;
        break;
        
#if DEBUG    
    default:
        printf("Error finding ir.m in SIO\n");
        break;
#endif
    
    }
}

int readInstruction(struct instruction* instruct, char* line, int len) {
    char arg0[MAX_ARG_LENGTH];
    char arg1[MAX_ARG_LENGTH];
    char arg2[MAX_ARG_LENGTH];
    
    int index = 0;
    
    index = nextToken(arg0,MAX_ARG_LENGTH,line,len,index);
    
    //No token is found (empty line)
    if (index==len) return 0;
    
    index = nextToken(arg1,MAX_ARG_LENGTH,line,len,index);
    
    index = nextToken(arg2,MAX_ARG_LENGTH,line,len,index);
    
    instruct->op = atoi(arg0);
    instruct->l = atoi(arg1);
    instruct->m = atoi(arg2);
    
    return 1;
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

//Jerasimos
int base( int level, int b){
	while(level > 0){
		b = stack[b+1];
		level--;
	}
	return b;
}






