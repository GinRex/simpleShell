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
int edit_execte(wchar_t **args, char **argv);
int function(int i, char**args);
int edit_function(int i, char**args, wchar_t**argv);


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

const wchar_t *funct_edit[] = {
	L"replace",
	L"insert",
	L"close"
};

int shell_num_funct(char *functs[]) {
	return sizeof(functs) / sizeof(char*);
}


////main loop of the program, exit when user use 'exit' ////
void shell_loop(void) {
	char *line; //command from user
	char **args; //split command to attributes
	int stt;
	//char *crt = new char[100];
	printf("Simple Shell - CMD Clone\n");
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
void wclrNewline(wchar_t *str) {
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

wchar_t* wread_line(void) {
	wchar_t*line = new wchar_t[MAX_LEN];
	_setmode(_fileno(stdin), _O_WTEXT);
	fgetws(line, MAX_LEN, stdin);
	wclrNewline(line);
	_setmode(_fileno(stdin), _O_TEXT);
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

int mWStrLen(wchar_t *str) {
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

void inputString(char *&str) {
	printf("\nEnter string: ");
	str = new char[MAX_LEN];
	fgets(str, MAX_LEN, stdin);
	clrNewline(str);
}

int findWord(wchar_t*str1, wchar_t*str2) {
	int len = mWStrLen(str1) - 1, count = 0, len2 = mWStrLen(str2);
	for (int i = 0; i <= len; i++) {
		if (*(str1 + i) == *str2) {
			int j = i, n = 0, m = j;
			while (*(str2 + n) != '\0') {
				if (*(str1 + j) == *(str2 + n)) {
					n++;
					j++;
					count++;
				}
				else break;
			}
			if (count == len2) {
				return m;
			}
			else {
				count = 0;
			}
		}
	}
	if (count == 0) {
		printf("\ncannot find '%s' in the string!", str2);
		return 1;
	}
}

void delDupSpace(wchar_t *str) {
	int len = mWStrLen(str);
	for (int i = 0; i < len; i++) {
		if (*(str + i) == ' ' && *(str + i + 1) == ' ') {
			for (int j = i + 1; j < len; j++) {
				*(str + j) = *(str + j + 1);
				if (*(str + j + 1) == ' ') {
					i--;
				}
			}
		}
	}
}

wchar_t *insertWord(wchar_t*wRoot, wchar_t*wIns, int pos) {
	int tempt = pos;
	int len = mWStrLen(wIns);
	wchar_t *strC = new wchar_t[100];
	wcsncpy(strC, wRoot, pos);
	strC[pos] = ' ';
	pos++;
	strC[pos] = '\0';
	wcscat(strC, wIns);
	pos += mWStrLen(wIns);
	//strC[pos] = ' ';
	//pos++;
	strC[pos] = '\0';
	wcscat(strC, wRoot + tempt);
	return strC;
}

int deleteWord(wchar_t*wRoot, int pos) {
	int len = 0;
	while (*(wRoot + pos) != ' ') {
		*(wRoot + pos) = ' ';
		pos++;
		len++;
	}
	delDupSpace(wRoot);
	//wRoot = insertWord(wRoot, wNew, pos);
	return 1;
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
//split-line for wchar_t of edit_function
wchar_t**wsplit_line(wchar_t *line) {
	int pos = 0, buf_size = args_buff_size;
	wchar_t** args = (wchar_t**)malloc(buf_size*sizeof(wchar_t*));
	wchar_t *arg;

	if (!args) {
		fprintf_s(stderr, "error: allcation error!\n");
		exit(EXIT_FAILURE);
	}

	arg = wcstok(line, L" \t\n\a\r");
	while (arg != NULL) {
		args[pos] = arg;
		pos++;

		if (pos >= buf_size) {
			buf_size += args_buff_size;
			args = (wchar_t**)realloc(arg, buf_size*sizeof(wchar_t*));
			if (!args) {
				fprintf_s(stderr, "error: allcation error!\n");
				exit(EXIT_FAILURE);
			}
		}

		arg = wcstok(NULL, L" \t\n\a\r");
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
	for (int i = 0; i <= 11; i++) {
		if (strcmp(args[0], funct_str[i]) == 0) {
			int j = function(i, args);
			return j;
		}
	}
	return 1;
}

int edit_execte(wchar_t **args, char **argv) {

	if (args[0] == NULL) {
		return 1;
	}
	
	for (int i = 0; i <= 2; i++) {
		if (wcscmp(args[0], funct_edit[i]) == 0) {
			int j = edit_function(i, argv, args);
			return j;
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

//delete with : 'del filePath'
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

//delete directory with 'rd dirPath'
int mDelDir(char**args) {
	if (args[1] != NULL) {
		WIN32_FIND_DATAA ffd;
		HANDLE hFind;
		DWORD Attributes;
		char str[100];
		std::string data;
		strcpy(str, args[1]);
		strcat(str, "\\*");
		hFind = FindFirstFileA(str, &ffd);
		char convert[100];
		do {
			data = args[1];
			data += "\\";
			data += ffd.cFileName;
			DeleteFile(convertCharArrayToLPCWSTR(data.c_str()));
		} while (FindNextFileA(hFind, &ffd) != 0);
		FindClose(hFind);
		if (RemoveDirectory(convertCharArrayToLPCWSTR(args[1]))) {
			printf("sucess to delete directory!\n");
		}
		else {
			printf("fail to delete directory!\n");
		}
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
		_setmode(_fileno(stdin), _O_TEXT);
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


int mEditFile(char**args) {
	if (args[1] != NULL) {
		printf("FILE: \n");
		mViewFile(args);
		wchar_t *line; //command from user
		wchar_t **argv; //split command to attributes
		int stt;
		char *name = new char[100];
		char *path = new char[100];
		takeNameAndPath(args[1], name, path);
		do {
			char *crd = new char[100];
			printf("%s> ", name);
			line = wread_line(); //read the command from user
			argv = wsplit_line(line);
			stt = edit_execte(argv, args);
			free(line);
			free(argv);
		} while (stt);
		return 1;

	}
	else {
		printf("Fail to read file!\n");
		return 0;
	}
	return 1;
}

int replace(char**args, wchar_t**argv) {
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
		fgetws(text, MAX_LEN, f);
		fclose(f);

		//change DES and inserted word to wchar_t
		int pos = findWord(text, argv[1]);
		deleteWord(text, pos);
		pos--;
		wchar_t *rturn = insertWord(text, argv[2], pos);

		FILE* fr = fopen(name, "wt, ccs=UTF-8");
		printf("success to insert text!\n");

		fwrite(rturn, sizeof(wchar_t), wcslen(rturn), fr);
		fclose(f);

		return 1;
	}
	else {
		printf("Fail to read file!\n");
		return 0;
	}
	return 1;

	return 1;
}

int insertW(char**args, wchar_t**argv) {
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
		fgetws(text, MAX_LEN, f);
		fclose(f);

		//change DES and inserted word to wchar_t
		int pos = findWord(text, argv[1]) + mWStrLen(argv[1]);
		wchar_t *rturn =  insertWord(text, argv[2], pos);
		
		FILE* fr = fopen(name, "wt, ccs=UTF-8");
		printf("success to insert text!\n");

		fwrite(rturn, sizeof(wchar_t), wcslen(rturn), fr);
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
	case 9: mEditFile(args);break;
	case 10: exit(1);break;
	default:return 1;
	}
	return 1;
}

int edit_function(int i, char**args, wchar_t**argv) {
	switch (i) {
	case 0:replace(args, argv);break;
	case 1:insertW(args, argv);break;
	case 2:return 0;
	default:return 1;
	}
	return 1;
}