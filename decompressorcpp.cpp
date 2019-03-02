#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int decompress(char* filename)
{
	FILE *fp = fopen(filename, "r");				// take filename from cmd
	if (fp == NULL)									// check file opening
	{
		puts("ERROR! Cannot read file!");
		return 1;
	}
	
	struct SYM buf[N*2];							// structure array for letters
	initializeArray(buf);							// fill each structure in array
	int tail;

	char arr[N];									// array for reading from file
	
	//fscanf(fp, "%s", arr);						// read first word
	fread(arr, sizeof(char), 6, fp);
	arr[6] = '\0';
	if (strcmpi(arr, "pivrar") != NULL)				// check signature
	{
		puts("ERROR! That is not a .pivrar file!");
		return 1;
	}

	long long i = 0;

	while (1)										// read 'ch' and 'code'
	{
		fread(&buf[i].freq, sizeof(float), 1, fp);
		if (buf[i].freq == 0.0)
			break;
		fread(&buf[i].ch, sizeof(unsigned char), 1, fp);
		i++;
	}

	fread(&tail, sizeof(int), 1, fp);
	//fscanf(fp, "%d", &tail);						// read tail lenght

	char filename_decomp[32];
	filename_decomp[0] = 'd';						// filename
	int filenameLenght;
	//fscanf(fp, "%s", &(filename_decomp[1]));
	fread(&filenameLenght, sizeof(int), 1, fp);
	fread(&filename_decomp[1], sizeof(char), filenameLenght, fp);
	filename_decomp[1 + filenameLenght] = '\0';

	FILE *fp_101 = fopen("decomp101.101", "w");
	if (fp_101 == NULL)								// check file opening
	{
		puts("ERROR! Cannot create decompressed .101 file!");
		return 1;
	}

	unsigned char letter;							// read main part of compessed file 
	unsigned char string[8];						// and write it to .101 file
	
	while (!feof(fp))	
	{
		fread(&letter, 1, 1, fp);
		unpack(string, letter);
		fwrite(string, sizeof(unsigned char), sizeof(string), fp_101);
	}

	fclose(fp);
	fclose(fp_101);
	FILE *fp_101_2 = fopen("decomp101.101", "r");	// open .101 file again
	
	struct SYM *psym[N * 2];
	for (int i = 0; i < N * 2; i++)
	{
		psym[i] = &buf[i];
	}
	struct SYM *root;
	root = buildTree(psym, N);

	makeCodes(root);

	FILE *fp_decomp = fopen(filename_decomp, "w");
	if (fp_decomp == NULL)							// check decompressed file opening
	{
		puts("ERROR! Cannot create decompressed file!");
		return 1;
	}

	char code_101[N * 2];
	char letter_decomp;
	i = 0;
	while (!feof(fp_101_2))							// read .101 and write to decompressed file
	{
		fread(&code_101[i], sizeof(char), 1, fp_101_2);
		code_101[i + 1] = '\0';
		for (int j = 0; j < N; j++)					// find letter
		{
			if (strcmp(code_101, buf[j].code) == 0 && buf[j].left==nullptr && buf[j].right==nullptr)
			{
				fwrite(&buf[j].ch, sizeof(unsigned char), 1, fp_decomp);
				i = -1;
				break;
			}
			else if (buf[j].freq == 0.0)
			{
				break;
				i++;
			}
		}
		i++;
		
	}
	
	
	code_101[strlen(code_101) - tail] = '\0';		// last symbol
	for (int j = 0; j < N; j++)						// find letter
	{
		if (strcmp(code_101, buf[j].code) == 0 && buf[j].left == nullptr && buf[j].right == nullptr)
		{
			fwrite(&buf[j].ch, sizeof(unsigned char), 1, fp_decomp);
			break;
		}
		else if (buf[j].freq == 0.0)
			break;
	}

	fclose(fp_101_2);
	fclose(fp_decomp);
	return 0;
}


unsigned char* unpack(unsigned char *string, unsigned char letter)
{
	union CODE code;
	code.ch = letter;
	string[0] = code.byte.b1 + '0';
	string[1] = code.byte.b2 + '0';
	string[2] = code.byte.b3 + '0';
	string[3] = code.byte.b4 + '0';
	string[4] = code.byte.b5 + '0';
	string[5] = code.byte.b6 + '0';
	string[6] = code.byte.b7 + '0';
	string[7] = code.byte.b8 + '0';
	return string;
}

char findLetter(struct SYM* buf, char *code_101)
{
	for (int i = 0; i < N; i++)
	{
		if (strcmp(code_101, buf[i].code) == 0)
			return buf[i].ch;
	}
}
