#pragma once
#include <stdlib.h>
#include <string.h>
#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <direct.h>
#include <iostream>


static int const MAX_LEN = 100;
#define args_buff_size 6;
#define args_sign " \t\n\a\r"

char* read_line(void);
char**split_line(char *line);
int shell_execte(char **args);
int function(int i, char**args);

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

void shell_loop(void) {
	char *line;
	char **args;
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

int pwd() {
	char *crd = new char[100];
	crd = _getcwd(NULL, 0);
	printf("Current working directory:\n	 %s \n", crd);
	return 1;
}

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

int mCd(char**args) {
	if (args[1] != NULL) {
		LPCWSTR lpPathName = convertCharArrayToLPCWSTR(args[1]);
		SetCurrentDirectory(lpPathName);
		//printf("success to change directory!\n");
	}
	else printf("fail to change directory!\n");
	return 1;
}

int mCopy(char**args) {
	if (args[1] != NULL && args[2] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		LPCWSTR newFileName = convertCharArrayToLPCWSTR(args[2]);
		CopyFile(lpPathFile, newFileName, FALSE);
		printf("sucess to copy!\n");
	}
	else {
		printf("Fail to copy!\n");
		return 0;
	}
	return 1;
}

int mMove(char**args) {
	if (args[1] != NULL && args[2] != NULL) {
		LPCWSTR lpPathFile = convertCharArrayToLPCWSTR(args[1]);
		LPCWSTR newFileName = convertCharArrayToLPCWSTR(args[2]);
		MoveFile(lpPathFile, newFileName);
		//CopyFile(lpPathFile, newFileName, FALSE);
		printf("sucess to move file!\n");
	}
	else {
		printf("Fail to move file!\n");
		return 0;
	}
	return 1;
}

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


int function(int i, char**args) {
	switch (i) {
	case 0: pwd();break;
	case 1: mCd(args);break;
	case 2: mCopy(args);break;
	case 3: mMove(args);break;
	case 4: mDel(args);break;
	default:return 1;
	}

	return 1;
}