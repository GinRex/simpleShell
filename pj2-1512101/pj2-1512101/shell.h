#pragma once
#include <stdlib.h>
#include <string.h>
#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <direct.h>
#include <iostream>
#include <io.h>
#include <wchar.h>
#include <fcntl.h>


static int const MAX_LEN = 100;
#define args_buff_size 6;
#define args_sign " \t\n\a\r"

char* read_line(void);
char**split_line(char *line);
int shell_execte(char **args);
int function(int i, char**args);


//list of command for user
char *funct_str[] = {
	"pwd",
	"cd",
	"copy",
	"move",
	"del",
	"mkdir",
	"rd",
	"createfile",
	"viewfile",
	"editfile",
	"exit"
};

int shell_num_funct() {
	return sizeof(funct_str) / sizeof(char*);
}


////main loop of the program, exit when user use 'exit' ////
void shell_loop(void) {
	char *line; //command from user
	char **args; //split command to attributes
	int stt;
	//char *crt = new char[100];
	do {
		char *crd = new char[100];
		crd = _getcwd(NULL, 0);
		printf("%s> ", crd);
		line = read_line(); //read the command from user
		args = split_line(line);
		stt = shell_execte(args);
		free(line);
		free(args);
	} while (stt);
}


////support function ////

void clrNewline(char *str) {
	while (*str != '\0') {
		if (*str == '\n') {
			*str = '\0';
			break;
		}
		str++;
	}
}

char* read_line(void) {
	char*line = new char[MAX_LEN];
	fgets(line, MAX_LEN, stdin);
	clrNewline(line);
	return line;
}

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

//support function for string 
int mStrLen(char *str) {
	int len = 0;
	while (*str != '\0') {
		len++;
		str++;
	}
	return len;
}

void joinString(char *str1, char *str2) {
	int len1 = mStrLen(str1), len2 = mStrLen(str2);
	int len1new = len1 + len2 + 1;
	int i = len1;
	*(str1 + i) = '\\';
	i++;
	for (i; i <= len1new; i++) {
		*(str1 + i) = *str2;
		str2++;
	}
	*(str1 + len1new) = '\0';
}

void revString(char*str) {
	int len = mStrLen(str) - 1;
	char *tempt = new char[MAX_LEN];
	//clone the origin string
	for (int i = 0; i <= len; i++) {
		*(tempt + i) = *(str + i);
	}

	for (int i = 0; i <= len; i++) {
		*(str + i) = *(tempt + len - i);
	}
}
void takeNameAndPath(char *str, char *name, char *path) {
	
	int i = mStrLen(str) - 1, n = 0;
	while (*(str + i) != '\\') {
		*(name + n) = *(str + i);
		i--;
		n++;
	}
	*(name + n) = '\0';
	revString(name);

	if (path != NULL) {
		for (int j = 0; j < i; j++) {
			*(path + j) = *(str + j);
		}
		*(path + i) = '\0';
	}
}

//split command from user to attributes
char**split_line(char *line) {
	int pos = 0, buf_size = args_buff_size;
	char** args = (char**)malloc(buf_size*sizeof(char*));
	char *arg;

	if (!args) {
		fprintf_s(stderr, "error: allcation error!\n");
		exit(EXIT_FAILURE);
	}

	arg = strtok(line, args_sign);
	while (arg != NULL) {
		args[pos] = arg;
		pos++;

		if (pos >= buf_size) {
			buf_size += args_buff_size;
			args = (char**)realloc(arg, buf_size*sizeof(char*));
			if (!args) {
				fprintf_s(stderr, "error: allcation error!\n");
				exit(EXIT_FAILURE);
			}
		}

		arg = strtok(NULL, args_sign);
	}

	args[pos] = NULL;
	return args;
}

//run built-in funtion
int shell_execte(char **args) {
	int i;

	if (args[0] == NULL) {
		return 1;
	}

	for (int i = 0; i < shell_num_funct(); i++) {
		if (strcmp(args[0], funct_str[i]) == 0) {
			int j = function(i, args);
			return 1;
		}
	}
}

//// BUILT-IN FUNCTIONS ////

//show current working directory
int pwd() {
	char *crd = new char[100];
	crd = _getcwd(NULL, 0);
	printf("Current working directory:\n	 %s \n", crd);
	return 1;
}


//point to chosen directory
int mCd(char**args) {
	if (args[1] != NULL) {
		LPCWSTR lpPathName = convertCharArrayToLPCWSTR(args[1]);
		SetCurrentDirectory(lpPathName);
		//printf("success to change directory!\n");
	}
	else printf("fail to change directory!\n");
	return 1;
}

//copy with : 'copy filePath subFoldername' while in destination directory and want to copy to sub folder
int mCopy(char**args) {
	if (args[1] != NULL && args[2] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		//LPCWSTR newFileSub = convertCharArrayToLPCWSTR(args[2]);

		//move to sub-folder arcording to the args[2]
		char *crd = new char[100];
		char *tempt = new char[100];
		char *name = new char[100];
		crd = _getcwd(NULL, 0);
		tempt = _getcwd(NULL, 0); //save the current dir to use later
		takeNameAndPath(args[1], name, NULL);

		joinString(crd, args[2]);
		LPCWSTR lpPathName = convertCharArrayToLPCWSTR(crd);
		LPCWSTR fileName = convertCharArrayToLPCWSTR(name);
		SetCurrentDirectory(lpPathName);

		CopyFile(lpPathFile, fileName, FALSE);
		printf("sucess to copy!\n");

		//set dir to before copy
		LPCWSTR dirname = convertCharArrayToLPCWSTR(tempt);
		SetCurrentDirectory(dirname);
	}
	else {
		printf("Fail to copy!\n");
		return 0;
	}
	return 1;
}

//move with : 'move filePath subFoldername' while in destination directory and want to move to sub folder
int mMove(char**args) {
	if (args[1] != NULL && args[2] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		//LPCWSTR newFileSub = convertCharArrayToLPCWSTR(args[2]);

		//move to sub-folder arcording to the args[2]
		char *crd = new char[100];
		char *tempt = new char[100];
		char *name = new char[100];
		crd = _getcwd(NULL, 0);
		tempt = _getcwd(NULL, 0); //save the current dir to use later
		takeNameAndPath(args[1], name, NULL);

		joinString(crd, args[2]);
		LPCWSTR lpPathName = convertCharArrayToLPCWSTR(crd);
		LPCWSTR fileName = convertCharArrayToLPCWSTR(name);
		SetCurrentDirectory(lpPathName);

		MoveFile(lpPathFile, fileName);
		printf("sucess to move file!\n");

		//set dir to before move
		LPCWSTR dirname = convertCharArrayToLPCWSTR(tempt);
		SetCurrentDirectory(dirname);
	}
	else {
		printf("Fail to move file!\n");
		return 0;
	}
	return 1;
}

//delete with : 'move filePath'
int mDel(char**args) {
	if (args[1] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		DeleteFile(lpPathFile);
		printf("sucess to delete file!\n");
	}
	else {
		printf("Fail to delete file!\n");
		return 0;
	}
	return 1;
}

//create new directory with 'mkdir dirPath'
int mDir(char**args) {
	if (args[1] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		CreateDirectory(lpPathFile, NULL);
		printf("sucess to make directory!\n");
	}
	else {
		printf("Fail to make directory!\n");
		return 0;
	}
	return 1;
}

//delete directory with 'del dirPath'
int mDelDir(char**args) {
	if (args[1] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		RemoveDirectory(lpPathFile);
		printf("sucess to delete directory!\n");
	}
	else {
		printf("Fail to delete directory!\n");
		return 0;
	}
	return 1;
}

//create new text file by 'createfile filepath\filename'
int mCreateFile(char**args) {
	if (args[1] != NULL) {
		FILE* f = fopen(args[1], "wt, ccs=UTF-8");
		printf("success to create file!\n");
		printf("Type something to file: ");
		wchar_t* text = new wchar_t[MAX_LEN];
		int i = 0;
		_setmode(_fileno(stdin), _O_WTEXT);
		fgetws(text, MAX_LEN, stdin);
		
		/*while (*(text + i) != '\0') {
			i++;
		}*/
		fwrite(text, sizeof(wchar_t), wcslen(text) - 1, f);
		fclose(f);
		return 1;
		
	}
	else {
		printf("Fail to create file!\n");
		return 0;
	}
	return 1;
}

//view text file by 'viewfile filepath\filename'
int mViewFile(char**args) {
	if (args[1] != NULL) {
		//move to sub-folder arcording to the args[2]
		char *tempt = new char[100];
		char *name = new char[100];
		char *path = new char[100];

		//tempt = _getcwd(NULL, 0); //save the current dir to use later
		takeNameAndPath(args[1], name, path);

		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(path);

		SetCurrentDirectory(lpPathFile);

		//set dir to before copy
		LPCWSTR dirname = convertCharArrayToLPCWSTR(tempt);
		SetCurrentDirectory(dirname);

		FILE* f = fopen(name, "rt, ccs=UTF-8");
		wchar_t* text = new wchar_t[MAX_LEN];
		int i = 0;
		if (f == NULL) {
			return 1;
		}
		printf("\n");
		_setmode(_fileno(stdout), _O_WTEXT);
		while (fgetws(text, MAX_LEN, f) != NULL) {
			wprintf(L"%s", text);
		}
		_setmode(_fileno(stdout), _O_TEXT);
		printf("\n");
		
		fclose(f);

		return 1;

	}
	else {
		printf("Fail to read file!\n");
		return 0;
	}
	return 1;
}

int function(int i, char**args) {
	switch (i) {
	case 0: pwd();break;
	case 1: mCd(args);break;
	case 2: mCopy(args);break;
	case 3: mMove(args);break;
	case 4: mDel(args);break;
	case 5: mDir(args);break;
	case 6: mDelDir(args);break;
	case 7: mCreateFile(args);break;
	case 8: mViewFile(args);break;
	default:return 1;
	}

	return 1;
}