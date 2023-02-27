#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

/*int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}*/

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc)
{
	*nl = 0;
	*nw = 0;
	*nc = 0;

	int c = getchar();
	int prev_c = ' ';
	while(c != EOF)
	{
		*nc = *nc + 1;

		if((c == ' ' || c == NEWLINE || c == '\t') && (prev_c != ' ' && prev_c != NEWLINE && prev_c != '\t')) *nw = *nw + 1;

		if(c == NEWLINE) *nl = *nl + 1;

		prev_c = c;
		c = getchar();
	}
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt)
{
	int count[MAX_CHARS] = {0};
	char chars[MAX_CHARS];
	for(int i=0;i<MAX_CHARS;i++) chars[i] = FIRST_CHAR + i;

	int c = getchar();
	while(c != EOF)
	{
		if(c >= FIRST_CHAR && c < LAST_CHAR) count[c-FIRST_CHAR]++;

		c = getchar();
	}

	int temp;
	for(int i=0;i<MAX_CHARS-1;i++)
	{
		for(int j=1;j<MAX_CHARS;j++)
		{
			if(count[j-1] < count[j] || (count[j-1] == count[j] && chars[j-1] > chars[j]))
			{
				temp = count[j-1];
                count[j-1] = count[j];
                count[j] = temp;

				temp = chars[j-1];
                chars[j-1] = chars[j];
                chars[j] = temp;
			}
		}
	}

	*n_char = chars[char_no-1];
	*cnt = count[char_no-1];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[])
{
	int count[MAX_DIGRAMS] = {0};
	int digrams[MAX_DIGRAMS];
	for(int i=0;i<MAX_CHARS;i++) for(int j=0;j<MAX_CHARS;j++) digrams[i+j*MAX_CHARS] = i+j*MAX_CHARS;

	int c = getchar();
	int c_next = getchar();
	while(c_next != EOF)
	{
		if(c >= FIRST_CHAR && c < LAST_CHAR && c_next >= FIRST_CHAR && c_next < LAST_CHAR)
		{
			count[(c_next-FIRST_CHAR)+(c-FIRST_CHAR)*MAX_CHARS]++;
		}

		c = c_next;
		c_next = getchar();
	}

	int temp;
	for(int i=0;i<MAX_DIGRAMS-1;i++)
	{
		for(int j=1;j<MAX_DIGRAMS;j++)
		{
			if(count[j-1] < count[j] || (count[j-1] == count[j] && digrams[j-1] > digrams[j]))
			{
				temp = count[j-1];
                count[j-1] = count[j];
                count[j] = temp;

				temp = digrams[j-1];
                digrams[j-1] = digrams[j];
                digrams[j] = temp;
			}
		}
	}

	digram[0] = FIRST_CHAR + digrams[digram_no-1] / MAX_CHARS;
	digram[1] = FIRST_CHAR + digrams[digram_no-1] % MAX_CHARS;
	digram[2] = count[digram_no-1];
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter)
{
	*line_comment_counter = 0;
	*block_comment_counter = 0;
	
	int koment_lin = 0;
	int koment_blo = 0;

	int c = getchar();
	int c_next = getchar();
	while(c_next != EOF)
	{
		if(koment_lin == 0 && koment_blo == 0)
		{
			if(c == '/' && c_next == '/')
			{
				koment_lin = 1;
				c_next = getchar();
			}
			else if(c == '/' && c_next == '*')
			{
				koment_blo = 1;
				c_next = getchar();
			}
		}
		else if(koment_blo && c == '*' && c_next == '/')
		{
			koment_blo = 0;
			*block_comment_counter = *block_comment_counter + 1;
			c_next = getchar();
		}
		else if(koment_lin && c == NEWLINE)
		{
			koment_lin = 0;
			*line_comment_counter = *line_comment_counter + 1;
			c_next = getchar();
		}

		c = c_next;
		c_next = getchar();
	}
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_line();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
			char_no = read_line();
			digram_count(char_no, digram);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

