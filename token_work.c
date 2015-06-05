#ifndef TRUE
#include "laba8_10_header.h"
#endif
#include "global_definition.h"

int first_letter_token(FILE* f)
{
	char ch;
	ch = fgetc(f);
	if(ch == 'i' || ch=='d' || ch == 'f' || ch == 'w' || ch == 'e' || ch == 'p' || ch == 's')
		return TRUE;
	else
		return FALSE;
}

char take_char_f(FILE* f)
{
	int temp;
	char ch;
	temp = ftell(f);
	ch = fgetc(f);
	fseek(f,temp, SEEK_SET);
	return ch;
}

void miss_spaces(FILE* f)
{
	for(;;)
		if(take_char_f(f)!=' ' && take_char_f(f)!='\n' && take_char_f(f)!='\t')
			return;
		else
			fgetc(f);
}

int is_alpha(char ch)
{
	if((ch>='a' && ch<='z') || (ch>='A' && ch<='B'))
		return TRUE;
	else 
		return FALSE;
}

int take_size_word(FILE* f)
{
	int size;
	size = 0;
	while(is_alpha(take_char_f(f)))
	{
		fgetc(f);
		++size;
	}
	return size;
}

//find first token from f, and left fptr on the next symbol
char* get_token_name(FILE* f, int* lvl)
{
	char* token;
	int pos, size, i;

	pos = ftell(f);
	while(!first_letter_token(f))
	{
		while(take_char_f(f)!=' '&&take_char_f(f)!='\n'&&take_char_f(f)!='('&&take_char_f(f)!='{'&&take_char_f(f)!='}')
		{
			fgetc(f);
		}
		if(take_char_f(f) == '{')
		{
			++(*lvl);
		}
		if(take_char_f(f) == '}')
		{
			--(*lvl);
		}
		miss_spaces(f);
		pos = ftell(f);
		if(take_char_f(f) == '{')
		{
			*lvl = *lvl + 1;
		}
		if(take_char_f(f) == '}')
		{
			*lvl-=1;
		}
	}
	fseek(f,pos, SEEK_SET);
	if(!first_letter_token(f))
	{
		token = (char*)malloc(sizeof(char));
		token[0] = '\0';
		return token;
	}
	fseek(f,pos, SEEK_SET);
	size = take_size_word(f);
	fseek(f,pos, SEEK_SET);
	token = (char*)malloc(sizeof(char)*size+1);
	i = 0;
	while(is_alpha(take_char_f(f)))
	{
		token[i] = fgetc(f);
		++i;
	}
	token[i] = '\0';
	return token;
}

char* make_word(FILE*f)
{
	int size, pos, i;
	char* str;

	pos = ftell(f);
	size = take_size_word(f);
	fseek(f,pos,SEEK_SET);

	str = (char*)malloc(sizeof(char)*size+1);
	i = 0;
	while(is_alpha(take_char_f(f)))
	{
		str[i] = fgetc(f);
		++i;
	}
	str[i] = '\0';

	return str;
}

val* make_val(FILE* f, int num_token_name, int lvl)
{
	int i;
	val* res;

	miss_spaces(f);

	res = (val*)malloc(sizeof(val));
	res->name = make_word(f);
	res->type = num_token_name;
	res->lvl = lvl;
	
	return res;

}

int feof_star(FILE* f)/*return 1 if EOF, 0 - if not*/
{
	if(feof(f))
	{
		return 1;
	}
	fgetc(f);
	if(feof(f))
	{
		return 1;
	}
	fseek(f,-1,SEEK_CUR);
	return 0;
}

void make_token_arrays(FILE* f, val* vals, other* others, cond* conds, loop* loops)
{
	char* temp;
	int lvl;
	lvl = 0;
	rewind(f);
	do
	{
		temp = get_token_name(f, &lvl);
		if(!strcmp(temp, token_names[INT]))
		{
			add_val(&vals,make_val(f, INT, lvl));
		}
		if(!strcmp(temp, token_names[CHAR]))
		{
			add_val(&vals,make_val(f, CHAR, lvl));
		}
		if(!strcmp(temp, token_names[DOUBLE]))
		{
			add_val(&vals,make_val(f, DOUBLE, lvl));
		}	
	}while(!feof_star(f));
}