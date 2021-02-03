/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!&%!!\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}



// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
   
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

}


void Parser::parse_input()
{
	parse_program();
	parse_inputs();
}

void Parser::parse_program()
{
	parse_poly_decl_section();
	parse_start();
}

void Parser::parse_poly_decl_section()
{
	parse_poly_decl();
	Token t = lexer.peek(1);
	if(t.token_type == POLY)
	{
		parse_poly_decl();
	}
	else if(t.token_type != START)
	{
		syntax_error();
	}
	else
	{
		//do nothing
	}
}

void Parser::parse_poly_decl()
{
	expect(POLY);
	parse_polynomial_header();
	expect(EQUAL);
	parse_polynomial_body();
	expect(SEMICOLON);
}

void parse_polynomial_header()
{
	parse_polynomial_name();
	Token t = lexer.peek(1);
	if (t.token_type == LPAREN)
	{
		expect(LPAREN);
		parse_id_list();
		expect(RPAREN);
	}
	else if(t.token_type != EQUAL)
	{
		syntax_error();
	}
	else
	{
		//do nothing
	}
}

void Parser::id_list()
{
	expect(ID);
	Token t = lexer.peek(1);
	if(t.token_type == COMMA)
	{
		expect(COMMA);
		parse_id_list();
	}
	else if(t.token_type != RPAREN)
	{
		syntax_error();
	}
	else 
	{
		//do nothing
	}
}

void Parser::parse_polynomial_name()
{
	expect(ID);
}

void Parser::parse_polynomial_body()
{
	parse_term_list();
}

void Parser::parse_term_list()
{
	parse_term();
	Token t == lexer.peek(1);
	if(t.token_type == MINUS || t.token_type == PLUS)
	{
		parse_add_operator();
		parse_term_list();
	}
	else if (t.token_type != NUM || t.token_type != ID)
	{
		syntax_error();
	}
	else
	{
		//do nothing
	}
}

void Parser::parse_monomial_list()
{
	expect(ID);
	Token t = lexer.peek(1);
	if(t.token_type == POWER)
	{
		t = lexer.peek(2);
		if(t.token_type = ID)
		{
			parse_monomial_list();
		}
		else if (t.token_type == ID)
		{
			parse_monomial_list();
		}
		else if(t.token_type != SEMICOLON)
		{
			syntax_error();
		}
		else
		{
			//All is good in the hood
		}
	}	
}

void Parser::parse_monomial()
{
	expect(ID);
	Token t = lexer.peek(1);
	if(t.token_type == POWER)
	{
		parse_exponent();
	}
	else if(t.token_type != SEMICOLON || t.token_type != ID)
	{
		syntax_error();
	}
	else
	{
		//All is good in the hood
	}
}

void Parser::parse_exponent()
{
	expect(POWER);
	expect(NUM);
}

void Parser::parse_add_operator()
{
	Token t = lexer.peek(1);
	if(t.token_type == MINUS)
	{
		expect(MINUS);
	}
	else if(t.token_type == PLUS)
	{
		expect(PLUS);
	}
	else 
	{
		//All is good in the hood
	}
}

void Parser::parse_coeficient()
{
	expect(NUM);
}

void Parser::parse_start()
{
	exepct(START);
	parse_statement_list();
}

void Parser::parse_inputs()
{
	expect(NUM);
	Token t = lexer.peek(1);
	if(t.token_type == NUM)
	{
		parse_inputs();
	}
	else if(t.token_type != EOF)
	{
		syntax_error();
	}
	else 
	{
		//All is good in the hood
	}
}

void Parser::parse_statement_list()
{
	parse_statement();
	Token t = lexer.peek(1);
	
	if(t.token_type == INPUT || t.token_type == ID)
	{
		parse_statement_list();
	}
	else if(t.token_type != NUM)						//after start is inputs
	{
		syntax_error();
	}
	else 
	{
		//All is good in the hood
	}

}

void Parser::parse_statement()
{
	Token t = lexer.peek(1);
	if(t.token_type == INPUT)
	{
		parse_input_statement();
	}
	else if(t.token_type == ID)
	{
		parse_poly_evaluation_statement();
	}
	else
	{
		syntax_error();
	}
}


void Parser::parse_poly_evaluation_statement()
{
	parse_polynomial_evaluation();
	expect(SEMICOLON);
}

void Parser::parse_input_statement()
{
	expect(INPUT);
	expect(ID);
	expect(SEMICOLON);
}

void Parser::parse_polynomial_evaluation()
{
	parse_polynomial_name();
	expect(LPAREN);
	parse_argument_list();
	expect(RPAREN);
}

void Parser::parser_argument_list()
{
	parse_argument();
	Token t = lexer.peek(1);
	if (t.token_type == COMMA)
	{
		expect(COMMA);
		parse_argument_list();
	}
	else if(t.token_type != RPAREN)
	{
		syntax_error();
	}
	else
	{
		//good in the hood
	}
}

void Parser::parse_argument()
{
	Token t = lexer.peek(1);
	if(t.token_type == ID)
	{
		t = lexer.peek(2)
		if(t.token_type == LPAREN)
		{
			parse_polynomial_evaluation();
		}
		else if(t.token_type == LPAREN || t.token_type == COMMA)
		{
			expect(ID);
		}
		else
		{
			syntax_error();
		}
	}
	else if(t.token_type == NUM)
	{
		expect(NUM);
	}
	else
	{
		syntax_error();
	}
}











int main()
{
	// note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    Parser parser;

	parser.ConsumeAllInput();
	
}
