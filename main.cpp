#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SYM // ������������� ������� 
{ 
unsigned char ch; // ASCII-��� 
float freq; // ������� ������������� 
char code[256]; // ������ ��� ������ ���� 
struct SYM *left; // ����� ������� � ������ 
struct SYM *right; // ������ ������� � ������ 
};

struct SYM* buildTree(struct SYM *psym[], int N) 
{	// ������ ��������� ���� 
	struct SYM *temp=(struct SYM*)malloc(sizeof(struct SYM)); 
	// � ���� ������� ������������ ����� ������ 
	// ���������� � �������������� ��������� ������� psym
	temp->freq=psym[N-2]->freq+psym[N-1]->freq; // ��������� ��������� ���� � ����� ���������� ������ 
	temp->left=psym[N-1]; 
	temp->right=psym[N-2]; 
	temp->code[0]=0; 
	if(N==2) // �� ������������ �������� ������� � �������� 1.0 
		return temp; 
	else
	{
		for (int i = 0; i < N; i++)
		{
			if (psym[i]->freq <= temp->freq)
			{
				for (int j = N-1; j > i; j--)
					psym[j] = psym[j-1];
				psym[i] = temp;
				break;
				
			}
			
		}
		return buildTree(psym, N - 1);
	}

}

void makeCodes(struct SYM *root) 
{ 
	if (root->left) 
	{ 
		strcpy(root->left->code, root->code); 
		strcat(root->left->code, "0"); 
		makeCodes(root->left); 
	} 
	if (root->right) 
	{ 
		strcpy(root->right->code, root->code); 
		strcat(root->right->code, "1"); 
		makeCodes(root->right); 
	} 
}

int main()
{
	struct SYM syms[256];

	
	FILE *fp = fopen("file.txt", "r");
	if (fp == 0)
	{
		puts("INPUT FILE ERROR!");
		return 1;
	}
	while (!feof(fp))
	{
		
	}

	fclose(fp);
	
	/*
	struct SYM syms[]=			// zaglushka
	{
		'x', 0.4, "", 0, 0,
		'y', 0.25, "", 0, 0,
		'z', 0.25, "", 0, 0,
		'.', 0.1, "", 0, 0
	};
	*/
	
	
	struct SYM *psym[8];
	for (int i = 0; i < 8; i++)
	{
		psym[i] = &syms[i];
	}
	struct SYM *root;
	root=buildTree(psym, 4);

	printf("%f\n", root->freq);
	makeCodes(root);

	return 0;
}