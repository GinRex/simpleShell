// pj2-1512101.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	//load config file

	//run command loop
	shell_loop();
	//shutdown/cleanup

    return 0;
}

