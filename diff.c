#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<ctype.h>
#include<time.h>
#include<sys/stat.h>
#include"fileList.h"
#include"pairstack.h"
#define max(m,n) (m>n)?m:n

int comFlag = 0;//flag for -i option
int negFlag = 0;//flag for -w option
int comnegFlag = 0;//flag for -iw option
int exiFlag = 0;//flag for -b option

//-b option taking strings neglectinf spaces
int strcmpb(char *s1, char *s2) {
	int i = 0, j = 0 ;
	int count = 0;
	int flag = 0;
	while(i < strlen(s1) && j < strlen(s2)) {
		if(s1[i] == s2[j] || ((s1[i]) == ' ' && s2[j] == '\t') || (s1[i] == '\t' && s2[j] == '\t')) {
			if(s1[i] == ' ' || s1[i] == '\t') {
				while(s1[i] == ' ' || s1[i] == '\t')
					i++;
				while(s2[j] == ' ' || s2[j] == '\t')
					j++;
			}
			else {
				i++;
				j++;
			}
		}
		else 
			return 1;
	}
	if(s1[i] == '\0' && s2[j] == '\0')
		return 0;
	else 
		return 1;
}


// -i option neglect lower and upper case
int strcmpi(char *s1, char *s2) {
	int i;
	if(strlen(s1) != strlen(s2))
		return -1;
	for(i = 0; i < strlen(s1); i++)
		if(toupper(s1[i]) != toupper(s2[i]))
			return s1[i] - s2[i];
	return 0;	
}



//-iw option 
int strcmpiw(char *str1, char *str2) {
	int i = 0, j = 0, s1, s2;
	while(str1[i] != '\0' && str2[j] != '\0') {
		while(str1[i] == ' ' || str1[i] == '\t')
			i++;
		while(str2[j] == ' ' || str2[j] == '\t')
			j++;
		s1 = str1[i];
		s2 = str2[j];
		if((s1 == s2) || (s1 == (s2 - 32)) || (s1 == (s2 + 32))) {
			i++;
			j++;
		}
		else
			return s1 - s2;
	}
	if(str1[i] == '\0' && str2[j] == '\0')
		return 0;
	else
		return str1[i] - str2[j];
}



//-w option
int strcmpw(char *str1, char *str2) {
	int i = 0, j = 0;
	while(str1[i] != '\0' && str2[j] != '\0') {
		while(str1[i] == ' ' || str1[i] == '\t')
			i++;
		while(str2[j] == ' ' || str2[j] == '\t')
			j++;
		if(str1[i] == str2[j]) {
			i++;
			j++;
		}
		else
			return str1[i] - str2[j];
	}
	if(str1[i] == '\0' && str2[j] == '\0')
		return 0;
	else
		return str1[i] - str2[j];
}

//-c option 
void newcprint(fileList f1,fileList f2, pairstack *path,char *file1,char *file2) {
	node *ptr = *path;
	node *ptr1;
	int x = 0, y = 0, j = 0;
	int i = 1,cha = 0, del = 0;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	printf("*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	printf("--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	printf("***************\n");
	printf("***%d,%d****\n", i, flength(&f1));
	while(ptr->next) {//file1
		ptr1 = ptr;
		cha = 0;
		del = 0;
		while(ptr1->next) {
			if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)) {
				//delete
				del++;
			}
			else if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)) {
				//insert
				if(del) {
					for(j = 0; j < del; j++) {
						printf("! %s", fgetline(&f1, ptr->k.x + j));
					}
					cha = 1;
					ptr = ptr1->next;
				}
				break;
			}
			else
				break;
			ptr1 = ptr1->next;
		}
		if(cha == 0) {
			if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y))
				//DELETE
				printf("- %s", fgetline(&f1, ptr->k.x));
			else if((ptr->k.x) == (ptr->next->k.x)){
				//insert
			}		
			
			else {
				printf("  %s", fgetline(&f1, ptr->k.x));
			}
			ptr = ptr->next;
		}
	}		
	
	
	printf("\n---%d,%d----\n",i,flength(&f2));
	ptr = *path;
	while(ptr->next) {//file2
		ptr1 = ptr;
		cha = 0;
		del = 0;
		while(ptr1->next) {
			if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)) {
				//delete
				del++;
			}
			
			else if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)) {
				//insert
				if(del) {
					for(j = 0; j < del; j++) {
						printf("! %s", fgetline(&f1, ptr->k.x + j));
					}
					cha++;
					ptr = ptr1->next;
				}
				break;
			}
			else
				break;
			ptr1 = ptr1->next;
		}
		if(cha == 0) {
			if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y))
				printf("+ %s",fgetline(&f2,ptr->k.y));
				//insert
			else if((ptr->k.y) == (ptr->next->k.y)) {
				//delete
			}
			else 
				printf("  %s",fgetline(&f2, ptr->k.y));
			ptr = ptr->next;
		}
		
	}	

}


	

// diff option using -u
void uprint(fileList f1,fileList f2, pairstack *path,char *file1, char *file2) {
	node *ptr = *path;
	int flag = 0;
	int x = 0, y = 0;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	printf("*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	printf("--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	printf("***************\n");
	printf("@@ -1,%d +1,%d @@\n",flength(&f1), flength(&f2));
	while(ptr->next) {
		flag = 0;
		if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)) {
			printf("- %s", fgetline(&f1, ptr->k.x));
			flag = 1;
		}
		if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)) {
			printf("+ %s", fgetline(&f2, ptr->k.y));
			flag = 2;
		}
		if(flag == 0)
			printf("  %s",fgetline(&f1, ptr->k.x));
		ptr = ptr->next;
	}
	printf("\n");
}
		
//default option
void defprint(fileList f1, fileList f2, pairstack *path) {
	node *ptr = *path;
	node *ptr1;
	int del = 0;
	int cha = 0;
	int i = 0;
	comFlag = 0;
	while(ptr->next) {
		ptr1 = ptr;
		del = 0;
		cha = 0;
		while(ptr1->next) {
			if((ptr1->k.x) < (ptr1->next->k.x) && (ptr1->k.y) == (ptr1->next->k.y)){
				//printf("delete\n");
				del++;
			}
			else if((ptr1->k.x) == (ptr1->next->k.x) && (ptr1->k.y) < (ptr1->next->k.y)){
				//printf("insert\n");
				if(del > 1){
					printf("%d,%dc%d\n", ptr->k.x + 1, ptr->k.y + del - 1, ptr->k.y + 1);
					for(i = 0; i < del; i++)
						printf("< %s", fgetline(&f1, ptr->k.x + i));
					printf("---\n");
					printf("> %s", fgetline(&f2, ptr->k.y));
					cha = 1;
					ptr = ptr1->next;
					//printf("! %s", fgetline(&f2, ptr->k.y));
				}
				else if(del == 1) {
					cha = 1;
					printf("%dc%d\n", ptr->k.x + 1, ptr->k.y + 1);
					printf("< %s", fgetline(&f1, ptr->k.x));
					printf("---\n");
					printf("> %s", fgetline(&f2, ptr->k.y));
					ptr = ptr1->next;
				}
				break;
			}
			else 
				break;
			ptr1 = ptr1->next;
		}
		if(cha == 0) {
			if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
				//printf("+ %s", fgetline(&f2, ptr->k.y));
				printf("%da%d\n", ptr->k.x, ptr->next->k.y);
				printf("> %s", fgetline(&f2, ptr->k.y));
			}
			else if((ptr->k.y) == (ptr->next->k.y) && (ptr->k.x) < (ptr->next->k.x)) {
				if(del > 1) {
					printf("%d,%dd%d\n", ptr->k.x + 1, ptr->k.y, ptr->k.y);
					for(i = 0; i < del; i++)
						printf("< %s", fgetline(&f1, ptr->k.x + i));
					ptr = ptr1;
				}
				else if(del == 1) {
					printf("%dd%d\n", ptr->k.x + 1, ptr->k.y);
					printf("< %s", fgetline(&f1, ptr->k.x));
				}
			}
			else {
			//	printf("  %s", fgetline(&f2, ptr->k.y));
			}
		ptr = ptr->next;
		}
	}
}
	
//for patch
void patch(fileList f1, fileList f2, pairstack *path, char *str, char* file1, char *file2) {
	FILE *patch;
	patch = fopen(str, "w+");
	node *ptr = *path;
	int x = 0, y = 0;
	int flag = 0;
	int temp;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	fprintf(patch, "*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	fprintf(patch, "--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	fprintf(patch, "***************\n");
	while(ptr->next){
		flag = 0;
		if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
			fprintf(patch, "- %s", fgetline(&f1, ptr->k.x));
			flag = 1;
		//	printf("delete\n");
		}	
		if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
			fprintf(patch, "+ %s", fgetline(&f2, ptr->k.y));
			flag = 2;
		}
		if(flag == 0){
			fprintf(patch, "  %s", fgetline(&f1, ptr->k.x));
		}
		ptr = ptr->next;
	}
}

//lcs on files
void lcs(fileList f1, fileList f2, pairstack *path){
	int m = flength(&f1), n = flength(&f2);
	int l[m+1][n+1];
	int i, j;
	char *str1, *str2;
	pair p;
	int count = 0;
	for(i = 0; i <= m; i++) {
		if(i > 0)
			str1 = fgetline(&f1, i - 1);
		for(j = 0;j <= n; j++){
			if(j > 0)
				str2 = fgetline(&f2, j - 1);
			if(i == 0 || j == 0)
				l[i][j] = 0;
			else if(!(strcmp(str1, str2)) || (!strcmpi(str1, str2) && comFlag) || (!strcmpw(str1, str2) && negFlag)
					|| (!strcmpiw(str1, str2) && comnegFlag) || (!strcmpb(str1, str2) && exiFlag))
				l[i][j] = l[i-1][j-1] + 1;
			else 
				l[i][j] = max(l[i-1][j], l[i][j-1]);

		} 
	}
	int index = l[m][n];

	char lcs[index + 1][2048];//lcs array stores the LCS of two files
	i = m; j = n;
	str1 = fgetline(&f1, i - 1);
	str2 = fgetline(&f2, j - 1);
	p.x = i;
	p.y = j;
	push(path, p);
	while(i > 0 && j > 0) {
		if(!strcmp(str1, str2) || (!strcmpi(str1, str2) && comFlag) || (!strcmpw(str1, str2) && negFlag) 
				|| (!strcmpiw(str1, str2) && comnegFlag) || (!strcmpb(str1, str2) && exiFlag)){
			i--;
			j--;
			strcpy(lcs[index - 1], str1);
			index--;
			count++;
			p.x = i;
			p.y = j;
			push(path, p);
			str1 = fgetline(&f1, i - 1);
			str2 = fgetline(&f2, j - 1);
		}
		else if(l[i - 1][j] > l[i][j - 1]){
			i--;
			p.x = i;
			p.y = j;
			push(path, p);
			str1 = fgetline(&f1, i - 1);
		}
		else {
			j--;
			p.x = i;
			p.y = j;
			push(path, p);
			str2 = fgetline(&f2, j - 1);
		}
	}
	while(i == 0 && j > 0){
		j--;
		p.x = i;
		p.y = j;
		push(path, p);
	}
	while(i > 0 && j == 0){
		i--;
		p.x = i;
		p.y = j;
		push(path, p);

	}
	//traverse(trace);
	
}			
		
			
char *get_line(int fd) {
	int i = 0;
	static char string1[2048];
	while(read(fd, &string1[i], 1)){
		if(string1[i] == '\n')
			break;
		i++;
	}
	i++;
	string1[i] = '\0';
	return string1;	
}


int main(int argc, char *argv[]) {
	FILE *fp1, *fp2;
	int flag = 0,  pflag = 0;
	
	if(argc <= 3){
		fp1 = fopen(argv[1], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[1]);
			return 0;
		}
		fp2 = fopen(argv[2], "r");
		if(!fp2){
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		flag = 0;
	}
	else if(argc == 4){
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		fp2 = fopen(argv[3], "r");
		if(!fp2) {
			printf("%s failed to open\n", argv[3]);
			return 0;
		}
	}
	else if(argc == 6) {
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		fp2 = fopen(argv[3], "r");
		if(!fp2) {
			printf("%s failed to open\n", argv[3]);
			return 0;
		}
		pflag = 1;
	}
	if(!strcmp(argv[1], "-c")){
		flag = 1;
	}
	else if(!strcmp(argv[1], "-u") && pflag == 0) {
		flag = 2;
	} 
	else if(!(strcmp(argv[1], "-i"))) {
		flag = 3;
		comFlag = 1;
	}
	else if(!(strcmp(argv[1], "-w"))) {
		flag = 4;
		negFlag = 1;
	}	
	else if(!(strcmp(argv[1], "-iw"))) {
		flag = 5;
		comnegFlag = 1;
	}
	else if(!(strcmp(argv[1], "-b"))) {
		flag = 6;
		exiFlag = 1;
	}
	else if(!strcmp(argv[1], "-u") && pflag == 1) {
		flag = 7;
	}
        else if(flag == 0) {
		
	}	
	else {
		printf("diff: invalid option -- \n");
		return 0;
	}
	pairstack path;//store the track of graph
	init(&path);
	char str1[2048];
	fileList f1, f2;
	finit(&f1);
	finit(&f2);
	int temp;	
	while(fgets(str1, 2048, fp1) != NULL)
		fappend(&f1, str1);
	while(fgets(str1, 2048, fp2) != NULL)
		fappend(&f2, str1);
	lcs(f1, f2, &path);
	switch(flag){
		case 1:
			newcprint(f1, f2, &path, argv[2], argv[3]);
			break;
		case 2:
			uprint(f1, f2, &path, argv[2], argv[3]);
			break;
		case 3:
			defprint(f1, f2, &path);
			break;
		case 4:
			defprint(f1, f2, &path);
			break;
		case 5:
			defprint(f1, f2, &path);
			break;
		case 6:
			defprint(f1, f2, &path);
			break;
		case 7:
			patch(f1, f2, &path, argv[5], argv[2], argv[3]);
			break;
		default:
			defprint(f1, f2, &path);
			if(strstr(argv[1], ".txt") && strstr(argv[2], ".txt") || strstr(argv[1], ".c") && strstr(argv[2], ".c"))
				flag = 1;
			break;
	}
	return 0;
}
