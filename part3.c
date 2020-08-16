
#include <stdio.h>
#include <string.h>
void printState(int hexArray[]);
int conditionCode(int currentRegister);

int main(int argc, char *argv[]){
	int index, temp;
	//initialised the state
	//hex for N=0x4E Z=0x5A P=0x50
	int hexArray[] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5A};
	int instructionArray[65536];

    FILE *file = fopen(argv[1], "rb");
    unsigned char bytes[2];
    fread(bytes, 1, 2, file);//read the first line of the document
    hexArray[8] = (bytes[0] << 8) + bytes[1];//load the starting address of the program
    index = hexArray[8];
 //    printf("Initial state\n");
	// printState(hexArray);

    //reading whole document and store all into an array
    //starting reading the second line of the document
    while(fread(bytes, 2, 1, file)){
    	temp = (bytes[0] << 8) + bytes[1];//complete instruction
    	instructionArray[index] = temp;
    	index++;
    }
    fclose(file);
    //start going through the instructions
    for(int i=hexArray[8]; i < 65536; i++){//start at the .orig
    	if(instructionArray[i] == 0xf025){//if reach the halt
    		hexArray[8] = i+1;//pc
    		hexArray[9] = 0xf025;
    		break;
    	}
    	hexArray[8] = i + 1;//pc
    	hexArray[9] = instructionArray[i];//IR
    	int theOpcode = instructionArray[i] & 0xF000;//find the opcode
	    switch(theOpcode){
	    	case 0x2000:{//LD
	    		int dr = (instructionArray[i] & 0xE00) >> 9 ;//shift right
	    		int offset = instructionArray[i] & 0x1FF;//offset
	    		if(offset >= 256){offset = -(512 - offset);}
	    		hexArray[dr] = instructionArray[hexArray[8] + offset];//load value
	    		hexArray[10] = conditionCode(hexArray[dr]);//set the CC
	   //  		printf("after executing instruction\t0x%04x\n", hexArray[9]);
				// printState(hexArray);
	    		break;
	    	}
	    	case 0xE000:{//LEA
	    		int dr = (instructionArray[i] & 0xE00) >> 9 ;//shift right
	    		int offset = instructionArray[i] & 0x1FF;//offset
	    		if(offset >= 256){offset = -(512 - offset);}
	    		hexArray[dr] = hexArray[8] + offset;//load value
	    		hexArray[10] = conditionCode(hexArray[dr]);//set the CC
	    		printf("after executing instruction\t0x%04x\n", hexArray[9]);
				printState(hexArray);
	    		break;
	    	}
	    }
    }
    return 0;
}

//print the state
void printState(int hexArray[]){
	for(int i = 0; i < 11; i++){
		if(i == 8){
			printf("PC\t0x%04x\n", hexArray[i]);
		}else if(i == 9){
			printf("IR\t0x%04x\n", hexArray[i]);
		}else if(i == 10){
			printf("CC\t%c\n", hexArray[i]);
		}else{
			printf("R%d\t0x%04x\n", i, hexArray[i]);
		}
	}
	printf("==================\n");
}

//change the CC according to the currentRegister value
int conditionCode(int currentRegister){
	if(currentRegister > 0x8000){
		return 0x4E;//N
	}else if(currentRegister == 0){
		return 0x5A;//Z
	}else{
		return 0x50;//P
	}//hex for N=0x4E Z=0x5A P=0x50
}
