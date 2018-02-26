#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* See `man 3 getopt()` for how to use the getopt function. */
#include <getopt.h>

/*
This function is similar to a wrapper function of 
stcmp() with the necessary type casts to get the 
qsort function running further, instructs it to
compare strings in our desired way
@params: const void * line1 -> the 1st line to be compared
@params: const void * line2 -> the 2nd line to be compared
return: +1 if line1>line2 and -1 vice-versa and 0 if equal 
*/
int strcmpwrp(const void * line1, const void * line2){
	/* 
	   as our lines are 2 dimesional pointers
	   1 dimension carrying the lines together 
	   another dimension carries the line itself
	   as an array of characters
	*/
	const char * Aline = *(const char **)line1;
	const char * Bline = *(const char **)line2;
	//returns the casted comparison using strcmp()
	return strcmp(Aline, Bline);


}

/*
This is a helper function to differetiate numbers from
other forms of input.
@params: char c -> character to be tested
return: true if given input is an int false if not an int
*/

int check_digit (char c) {
	//RANGE of integers
	if ((c>='0') && (c<='9')){
		return 1;
	}     
	return 0;
}

/*
This a helper function to print the sorted lines
to the standard output
@params: char * lines[] -> the lines to be printed
@params: int counter -> has the count of total no.
	 of lines in the input provided
return: void
*/
void printArray(char * lines[],int counter){
	for(int i =0; i<counter; i++){
                printf("%s\n",lines[i]);
	}
}
/*
This function takes input from the file and sorts this input
and then prints these sorted lines using the prinArray helper
function
@params: FILE *fp -> first file pointer for reading input
@params: FILE *fpr -> second file pointer for counting the
	 no.of lines present
@params: int nflag -> shows the presence of -n in the input
	 arg
@params: int nvalue -> arg of -n
@params: int reverse -> shows the presence of -r in the input
	 arg
return: void
*/
void sortFile(FILE *fp,FILE *fpr,int nflag,int nvalue, int reverse){
	
	int temp = reverse;
	//contains the number of lines
	int count=0;
	// is the buffer for using in the fgets function
	char buffer [1024];
	// temp variable for holding each line that we get
	//from fgets
	char * element;
		
	// iterating variable
	int i =0;
	// finds the total number of lines in the given file
	while(fgets(buffer,1024, fpr)!=NULL){
		++count;
	}
	//pointer char array for holding lines
	char **line = (char **)malloc(sizeof(char*)*count);
	//file input
	while(fgets(buffer,1024, fp)!= NULL ){

		element = strtok(buffer, "\n");
		line[i] = (char *)malloc(strlen(element) + 1);
		if(line[i]==NULL){
			fprintf(stderr, "NULL returned from the library function malloc\n");
			exit(1);		
		}		
		strcpy(line[i], element);
		++i;
			
	}
	// temp iterating var for reversing the sorted list (-r)
	int h=count;
	--h;
	//temp computer holder
	char* compute;
	//sorts the lines
	qsort(line,count,sizeof(char*),strcmpwrp);
	//if (-r)
	if(temp==1){
		//reverses the sorted list
		for(int i=0, h=count-1; i<count && i<count/2; i++,h--){
			compute = line[i];
			line[i] = line[h];
			line[h] = compute;
		}

	}
	//if(-n flag)
	if(nflag==1)
	count = nvalue;
	// prints the output to stdout
	//printArray(line,count);
	for(int i =0; i<count ; i++){

		printf("%s\n", line[i]);

	}	
	for(int i=0; i<count; i++){
		free(line[i]);
	}
	free(line);
	
		
}
/*
This function takes input from the stdin and sorts this input
and then prints these sorted lines into stdout
@params: int nflag -> shows the presence of -n flag
@params: int nvalue -> the arg for -n flag
@params: int temp -> for -r
return: void
*/
void sortStdin(int nflag,int nvalue, int temp){
	
	// temp holder for each line
	char * element;
	// 2 dimensional for holding lines
	char ** lines = realloc(NULL,sizeof(char *)*1);
	//counts the number of lines given through stdin
	int size=0;
	// buffer for fgets function
	char buf[1024];
	//iterating var inside the while loop
	int pos=0;
	// read every line from stdin
	while((fgets(buf, 1024, stdin))!=NULL) {
		
		
		element = strtok(buf, "\n");	
		lines[pos] = (char *)malloc(strlen(element) + 1);
		if(lines[pos]==NULL){
			fprintf(stderr, "NULL returned from the library function malloc\n");	
			exit(1);	
		}
		if(element!=NULL)
		strcpy(*(lines+pos), element);
		//when the we need to pump in more space (dynamic allocation)				
		if(pos == size) { 
			size = size + 1;
			lines = realloc(lines, sizeof(char*)*size);
			if(lines==NULL){
				fprintf(stderr, "NULL returned from the library function realloc\n");	
				exit(1);			
			}
		}
		//lines[pos] = (char *)malloc(strlen(element) + 1);
		++size;
		pos++;
	}
	//sorts the lines
	qsort(lines,pos,sizeof(char*),strcmpwrp);
	//temp holder var for each line while reversing the list(-r)	
	char * temp1;
	// iterating var
	int h = pos;
	--h;
	//if(-r flag)
	if(temp==1){
		//revereses the already sorted lines (-r)
                for(int i=0, h=pos-1; i<pos && i<pos/2; i++,--h){
                        temp1 = lines[i];
                        lines[i] = lines[h];
                        lines[h] = temp1;
			
                }

        }
	//if(-n)
	if(nflag==1)
	pos = nvalue;
	//prints to the stdout
	for(int i =0; i<pos ; i++){

		printf("%s\n", lines[i]);

	}
	for(int j=0; j<pos; j++){
		free(lines[j]);
	}
	free(lines);

}
/*
Checks the command line args and does error checking
initializes the params for sortFile and sortStdin functions
exit(1) if there was some error
exit(0) if excecuted successfully
*/
 
int main(int argc, char** argv){	
	/* checking input */
	char* temp;
	int opt,nvalue;
	int rflag=0;
	int nflag=0;
	
	while((opt = getopt(argc, argv, "rn:")) != -1) {
		// for the possible cases -n -r 
		switch(opt) { 
		case 'r':
			rflag=1;
			break;
			
		case 'n':
			nflag=1;
			temp = optarg;
			// checks if -n flag get valid input
			for(int i=0; i<strlen(temp); i++){
				if(! check_digit(temp[i])){
                                 fprintf(stderr, "Usage: Invalid argument of %s\n", argv[1]);
                       		 exit(1);
				}
			}
			nvalue = atoi(temp);
			break;
		default:
			fprintf(stderr, "Usage: There is an error %s\n", argv[1]); 
			exit(1);
		}
	}
	
		
	//ensures only one file input is given	
	if(argv[optind+1]!=NULL && optind!=1 && argv[optind]!=NULL){
		fprintf(stderr, "more than one file name input %s\n",argv[optind+1]);
		exit(1);
	}
	//this is a special case to be handled
	if(nflag==0 && rflag==0 && argv[optind+1]!=NULL){
		 fprintf(stderr, "more than one file name input %s\n",argv[optind+1]);
                exit(1);

	}
	// check if a file arg is present or not
	if(argv[optind] != NULL) { 

		FILE *fp1, *fp2;
		fp1 = fopen(argv[optind], "r");
		fp2 = fopen(argv[optind], "r");
		// checks if the file can be found or can be opened
		if(fp1==NULL){
			fprintf(stderr, "Error in opening file\n");
			exit(1);
		}
		// checks if the file can be found or can be opened
		if(fp2==NULL){
			fprintf(stderr, "Error in opening file\n");
			exit(1);		
		}
		// take input from file and sort
		sortFile(fp1,fp2,nflag,nvalue,rflag);
		fclose(fp1);
		fclose(fp2);
	}
	else {
		//take input from stdin and sort
		sortStdin(nflag,nvalue,rflag);
	}
	/* If successful, exit with a zero status. */
	exit(0);
	return 0;
	
	

	
}


