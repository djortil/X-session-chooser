#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool checkRoot(void){
	//This is a bad implementation, but it'll do for now
	FILE* wai = popen("whoami", "r");
	if(wai==NULL)
		return true;
	char result[5];

	fgets(result, sizeof(char)*5, wai);
	
	

	pclose(wai);
	if(strcmp("root", result)==0){
		return true;
	}
	return false;
}


int listX(char** list, char* home){
	char path[1024];
	char* name = "/.xsessions";
	char* templist = malloc(sizeof(char)*1024);
		
	snprintf(path, sizeof(path), "%s%s", home, name);
	// printf("%s\n", path);

	FILE* sessions = fopen(path, "r");
	if(sessions==NULL){
		printf("No such file.\n");
		goto cleanup;
		
	}

	int i=0;
	int j=0;

	if(templist==NULL)
		goto cleanup;
	
	if((fgets(templist, 1024, sessions))==NULL)
		goto cleanup;
	
	if(templist[0]=='\0'){
		printf("Empty .xsessions file!\n");
		printf("Create an .xsessions file in your home directory and fill it up with your xsessions separated by ;\n");
		
		goto cleanup;
	}
	
	list[j]=templist;
	while(templist[i]!='\0'){
		if(templist[i]==';'){
			j++;
			templist[i]='\0';
			list[j] = templist+i+1;
			
		}
		i++;
	}

	fclose(sessions);

	return j+1;

	
	
	
	cleanup:
	if(sessions!=NULL)
		fclose(sessions);
	
	if(templist!=NULL)
		free(templist);
	
	return -1;
	
}
int main(void){
	//Check root 
	if(checkRoot()){fprintf(stderr, "ERROR: CANNOT START AS ROOT!!!\n"); return 1;}
		if(getenv("DISPLAY")!=NULL){
			printf("Error: Display variable is not set to NULL; X server could already be running?\n");
			return 1;
		}


	char *home = getenv("HOME");
	if(home==NULL){
		return 1;
	}

	char** list = calloc(10, sizeof(char*));
	if(list==NULL){printf("Allocation failure!!!\n"); return 1;}//Check if malloc/calloc fails
	
	int size = listX(list, home);	
	
	if(size==-1)
		goto cleanup;
	if(size>9){size=9;}
	
	
		char path[1024];

	snprintf(path, sizeof(path), "%s/.xinitrc", home); 
		
	remove(path);
	//open xinit
	FILE* xinit=NULL;
	xinit= fopen(path,"w");
	if(xinit==NULL)
		goto cleanup;
	
	
	for(int i=0;i<size-1;i++){
		printf("[%d] ", i+1);
		if(list[i]!=NULL){
			printf("%s\n", list[i]);
		}else{
			printf("\n");
		}
	}

	printf("\n[0] Exit\n");



	

	if((fprintf(xinit, "exec ")) < 0){
		goto cleanup;	
	}
	
	

	int running=1;
	while(running)
	{
		printf("\nWhich X sessions would you like to launch?: ");
	
			int input = fgetc(stdin) - '0';		//Read one character out of the input buffer
			while (getchar() != '\n' && !feof(stdin));		//Empty the rest of the input buffer

		printf("\n%d\n", input);
		

		if (input == 0) {
		    running = 0;
          goto cleanup;
		} else if (input >= 1 && input <= 9 && list[input-1] != NULL && input!=size) {
		    fprintf(xinit, "%s", list[input-1]);
		    running = 0;
		} else {
		    printf("No session under %d!\n", input);
		}	
	}


	fclose(xinit);	
	free(list[0]);
	free(list);

	system("startx");
	return 0;
	
	
	//Error handling
	cleanup:
	
	if(xinit!=NULL)
		fclose(xinit);
	
	if(list!=NULL){
		if(list[0]!=NULL){
			free(list[0]);
		}
		free(list);
	}
		
	return 1;
}
