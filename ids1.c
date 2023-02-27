#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char ident[MAX_IDS][MAX_ID_LEN] = {0};

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents()
{
	int ile = 0;
	int koment_lin = 0;
	int koment_blo = 0;
	int napis = 0;
	int napis2 = 0;

	int n_keywords = sizeof(keywords)/sizeof(keywords[0]);

	int c = getchar();
	int prev_c = -1;
	while(c != EOF)
	{
		if(koment_blo && prev_c == '*' && c == '/') koment_blo = 0;
		else if(koment_lin && c == '\n') koment_lin = 0;
		else if(napis && c == '\"' && prev_c != '\\') napis = 0;
		else if(napis2 && c == '\'' && prev_c != '\\') napis2 = 0;
		else if(koment_lin == 0 && koment_blo == 0 && napis == 0)
		{
			if(prev_c == '/' && c == '/') koment_lin = 1;
			else if(prev_c == '/' && c == '*')
			{
				koment_blo = 1;
				c = getchar();
			}
			else if(c == '\"') napis = 1;
			else if(c == '\'') napis2 = 1;
			else if((isalpha(c) || c == '_' ) && prev_c != '\\' && prev_c != '#' && prev_c != '<' && prev_c != '.' && isalpha(prev_c)==0)
			{	
				int i = 0;
				while(isalnum(c) || c == '_')
				{
					ident[ile][i] = c;
					i++;
					c = getchar();
				}
				ungetc(c, stdin);
				c = 'a';
				ident[ile][i] = -1;

				int ident_w_key = 0;
				for(int j=0;j<n_keywords;j++)
				{
					int k = 0;
					while(ident[ile][k] != -1 && ident[ile][k] == keywords[j][k]) k++;
					if(ident[ile][k] == -1 && keywords[j][k] == '\0')
					{
						ident_w_key = 1;
						break;
					}
				}

				if(ident_w_key == 0)
				{
					int takie_same = 0;
					for(int j=0;j<ile;j++)
					{
						int k = 0;
						while(ident[j][k] == ident[ile][k] && ident[j][k] != -1 && ident[ile][k] != -1) k++;
						if(ident[j][k] == -1 && ident[ile][k] == -1)
						{
							takie_same = 1;
							break;
						}
					}

					if(takie_same == 0)
					{
						/*printf("?");
						int j = 0;
						while(ident[ile][j] != -1)
						{
							printf("%c", ident[ile][j]);
							j++;
						}
						printf("?\n");*/
						ile++;
					}
				}
			}
		}

		if(c == EOF) break;
		prev_c = c;
		c = getchar();
	}
	
	return ile;
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

