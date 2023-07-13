#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
using namespace std;

enum type_of_lex {
    LEX_NULL,                                                                                   	/* 0*/
    LEX_AND, LEX_STRING, LEX_DO, LEX_ELSE, LEX_GOTO, LEX_REAL, LEX_IF, LEX_INT, LEX_NOT,        	/* 9*/
    LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_WHILE, LEX_WRITE,									/*15*/
    LEX_FIN,                                                                                    	/*16*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   				/*23*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              	/*31*/
    LEX_DEQ, LEX_TSLASH, LEX_SLASHT, LEX_BEGIN, LEX_END, 											/*36*/
    LEX_NUM,																						/*37*/
	LEX_QUOTE,                                                                                    	/*38*/
	LEX_DOUBLE,																						/*39*/
    LEX_ID,                                                                                     	/*40*/
    POLIZ_LABEL,                                                                                	/*41*/
    POLIZ_ADDRESS,                                                                              	/*42*/
    POLIZ_GO,                                                                                   	/*43*/
    POLIZ_FGO                                                                                   	/*44*/
};
 
class Lex {
    type_of_lex   t_lex;
    int           v_lex;
    string        str_lex;
    double        doub_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, string s = "", double f = 0 ): t_lex (t), v_lex (v), str_lex (s), doub_lex(f)  { }
    type_of_lex  get_type () const { 
    	  return t_lex; 
    }
    Lex operator=(const Lex& a){
    	t_lex=a.t_lex;
    	v_lex=a.v_lex;
    	str_lex=a.str_lex;
    	doub_lex=a.doub_lex;
    	return *this;
	}
    int get_value_int () const { 
    	  return v_lex; 
    }
    string get_value_str () const {
    	return str_lex;
	}
	double get_value_doub () const {
		return doub_lex;
	}
  friend ostream & operator<< ( ostream &s, Lex l );
};
 
class Ident {
    string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    int         value_int;
    string      value_str;
    double		value_doub;
public:
   	Ident() { 
        declare = false; 
        assign  = false; 
    }
    bool operator== ( const string& s ) const { 
        return name == s; 
    }
    Ident ( const string n ) {
        name    = n;
        declare = false; 
        assign  = false;
    }
    string get_name () const { 
      return name; 
    }
    bool get_declare () const { 
      return declare; 
    }
    void put_declare () { 
      declare   = true; 
    }
    type_of_lex get_type () const { 
      return type; 
    }
    void put_type ( type_of_lex t ) { 
      type      = t; 
    }
    bool get_assign () const { 
      return assign; 
    }
    void put_assign () { 
      assign    = true; 
    }
    int  get_value_int () const { 
      return value_int; 
    }
    void put_value_int ( int v ) { 
      value_int = v; 
    }
     string  get_value_str () const { 
      return value_str; 
    }
    void put_value_str ( string s ) { 
      value_str = s; 
    }
     double  get_value_doub () const { 
      return value_doub; 
    }
    void put_value_doub ( double d ) { 
      value_doub = d; 
    }
};
 
vector<Ident> TID;
 
int put ( const string & buf ) {
    vector<Ident>::iterator k;
 
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}
 
class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) {
        int i = 0;
        while ( list[i] ) {
            if ( buf == list[i] )
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) ) 
            throw  "can�t open file" ;
    }
    Lex get_lex ();
};
 
const char *
Scanner::TW    [] = { "", "and", "string", "do", "else",
					 "goto", "real", "if", "int", "not", "or",
					  "program", "read", "then", "while", "write", NULL };
 
const char *
Scanner::TD    [] = { 
    "", ";", ",", ":", "(", ")", 
    "=", "<", ">", "+", "-", "*", 
	"/","<=", "!=", ">=", "==",  
    "*/", "/*", "{", "}", 
    NULL 
};

Lex Scanner::get_lex () {
    enum    state { H, IDENT, QUOTE, NUMB, SLASH, COM, POINT, ALE, NEQ };
    int     d, j;
    double  f, i=0.1;
    string  buf;
    state   CS = H;
    do {
        gc ();
        switch ( CS ) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' ); 
                    else if ( isalpha (c) ) {
                        buf.push_back (c); 
                        CS  = IDENT;
                    }
                    else if (c == '"' ){
                		CS = QUOTE;
					}
                    else if ( isdigit (c) ) {
                        d   = c - '0';
                        CS  = NUMB;
                    } 
                    else if ( c == '/' ) { 
                    	buf.push_back(c);
                        CS  = SLASH;
                    }
                    else if (c == '=' || c == '<' || c == '>' ) { 
                        buf.push_back (c);
                        CS  = ALE; 
                    }
                    else if (c == EOF)
                        return Lex ( LEX_FIN );
                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {
                        buf.push_back (c);
                        if ( ( j = look ( buf, TD) ) ){
                            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                        }
                        else
                            throw c;
                    }
                    break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c); 
                }
                else {
                    ungetc ( c, fp );
                    if ( (j = look ( buf, TW) ) ) {
                        return Lex ( (type_of_lex) j, j );
                        }
                        else {
                        j   = put ( buf );
                        return Lex ( LEX_ID, j );
                    }
                }
                break;
            case QUOTE:
            	if (c != '"'){
            		buf.push_back(c);
				}
				else{
					return Lex ( LEX_QUOTE, 0, buf);
				}
				break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                }
                else {
                	if (c == '.'){
                		f = d;
                		CS = POINT;
					}
					else{
                    	ungetc ( c, fp );
                    	return Lex ( LEX_NUM, d );
                    }
                }
                break;
            case SLASH:
                if ( c == '*' ) {
                	buf="";
                    CS = COM;
                }
                else{
                	ungetc(c, fp);
                	j   = look ( buf, TD );
                	return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
				}
                break;
            case COM:
            	if ( c == '*'){
            		gc();
            		if (c == '/'){
            			CS = H;
					}
					else {
						ungetc(c, fp);
					}
				}
				break;
			case POINT:
				if ( isdigit (c) ) {
					f = f + (c - '0')*i;
                    i=i*0.1;
                }else if (i == 0.1) {
                	throw c;
				}else {
           	    	ungetc ( c, fp );
               	    return Lex ( LEX_DOUBLE, 0, "", f );
               	}
               	break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {
                    ungetc ( c, fp );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
    }
  } while (true);
}
 
 ostream & operator<< ( ostream &s, Lex l ) {
    s << '(' << l.t_lex << ',' << l.v_lex << ',' << l.str_lex << ',' << l.doub_lex << ");" << endl;
    return s;
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val_int;
    string 		 c_val_str;
    double		 c_val_doub;
    Scanner      scan;
   // stack < int >           st_int;
   // stack < type_of_lex >   st_lex;
    void  Program();
    void  Descriptions();
    void  Description();
    void  Type();
    void  Variable();
    void  Constant();
    void  Operators();
    void  Operator();
    void  CompoundOp();
    void  Expression();
    void  Assignment();
    void  Expression1();
    void  PlusAndMinus();
    void  Comparison();
    void  Multiplication();
    void  Negation();
    void  Unary();
    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val_int = curr_lex.get_value_int ();
        c_val_str = curr_lex.get_value_str();
        c_val_doub= curr_lex.get_value_doub();
        
    }
public:
   // vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
    gl ();
    Program();
    if (c_type != LEX_FIN)
        throw curr_lex;
    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
   // for ( Lex l : poliz ) 
    //    cout << l;
    cout << endl << "Yes!!!" << endl;
}

void Parser::Program () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else 
        throw curr_lex;      
    if ( c_type == LEX_BEGIN )
        gl ();
    else
        throw curr_lex;
    Descriptions(); 
    Operators();
    if ( c_type == LEX_END)
    	gl();
    else
    	throw curr_lex;
}
void Parser::Descriptions() {
	while( c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_REAL ){
		Description();
		if (c_type == LEX_SEMICOLON )
			gl();
		else
			throw curr_lex;
	}
}

void Parser::Description() {
	Type();
	Variable();
	while( c_type == LEX_COMMA){
		gl();
		Variable();
	}	
}

void Parser::Type() {
	if ( c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_REAL )
		gl();
	else 
		throw curr_lex;
}

void Parser::Variable() {
	if (c_type == LEX_ID)
		gl();
	else
		throw curr_lex;
	if (c_type == LEX_EQ ){
		gl();
		Constant();
	}
}

void Parser::Constant() {
	if (c_type == LEX_QUOTE)
		gl();
	else{
		if (c_type == LEX_PLUS || c_type == LEX_MINUS )
			gl();
		if (c_type == LEX_NUM || c_type == LEX_DOUBLE)
			gl();
		else
			throw curr_lex;
	}		
}

void Parser::Operators() {
	while ( c_type != LEX_END)
		Operator();
}

void Parser::Operator() {
	if ( c_type == LEX_IF){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			Expression();
			if ( c_type == LEX_RPAREN){
				gl();
				Operator();
				if ( c_type == LEX_ELSE){
					gl();
					Operator();
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_WHILE){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			Expression();
			if ( c_type == LEX_RPAREN){
				gl();
				Operator();
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_READ){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			if ( c_type == LEX_ID){
				gl();
				if ( c_type == LEX_RPAREN){
					gl();
					if ( c_type == LEX_SEMICOLON)
						gl();
					else
						throw curr_lex;
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_WRITE){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			Expression();
			while ( c_type == LEX_COMMA){
				gl();
				Expression();
			}
			if ( c_type == LEX_RPAREN){
				gl();
				if (c_type == LEX_SEMICOLON){
					gl();
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_DO) {
		gl();
		Operator();
		if ( c_type == LEX_WHILE){
			gl();
			if ( c_type == LEX_LPAREN){
				gl();
				Expression();
				if ( c_type == LEX_RPAREN){
					gl();
					if ( c_type == LEX_SEMICOLON){
						gl();
					}else
						throw curr_lex;
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_BEGIN){
		gl();
		Operators();
		if (c_type == LEX_END){
			gl();
		}else
			throw curr_lex;
	}else if ( c_type == LEX_NOT || c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_ID || c_type == LEX_NUM|| c_type == LEX_QUOTE || c_type == LEX_DOUBLE){
		if (c_type == LEX_ID){
			gl();
			if (c_type == LEX_COLON){
				gl();
				Operator();
			}else if (c_type == LEX_EQ)
				Assignment();
				else
					throw curr_lex;
		}else{
			Expression();
			if ( c_type == LEX_SEMICOLON){
				gl();
			}else
				throw curr_lex;
		}
	}else if ( c_type == LEX_GOTO) {
		gl();
		if ( c_type == LEX_ID){
			gl();
			if ( c_type == LEX_SEMICOLON){
				gl();
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else
		throw curr_lex;
}
 
void Parser::Assignment(){
	while(c_type == LEX_EQ){
			gl();
			if ( c_type == LEX_ID)
				gl();
			else{
				Expression();
				if ( c_type == LEX_SEMICOLON){
					gl();
				}else
					throw curr_lex;
				break;
			}
	}
} 
 
void Parser::Expression() {
	Expression1();
	while( c_type == LEX_OR){
		gl();
		Expression1();
	}
}

void Parser::Expression1() {
	Comparison();
	while( c_type == LEX_AND) {
		gl();
		Comparison();
	}
}

void Parser::Comparison() {
	PlusAndMinus();
	while( c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_NEQ || c_type == LEX_DEQ || c_type == LEX_LEQ || c_type == LEX_GEQ){
		gl();
		PlusAndMinus();
	}
}

void Parser::PlusAndMinus() {
	Multiplication();
	while( c_type == LEX_PLUS || c_type == LEX_MINUS){
		gl();
		Multiplication();
	}
}

void Parser::Multiplication() {
	Negation();
	while( c_type == LEX_TIMES || c_type == LEX_SLASH){
		gl();
		Negation();
	}
}

void Parser::Negation() {
	if ( c_type == LEX_NOT){
		gl();
		Negation();
	}else
		Unary();
}

void Parser::Unary() {
	if ( c_type == LEX_ID){
		gl();
	}else if ( c_type == LEX_NUM){
		gl();
	}else if ( c_type == LEX_QUOTE){
		gl();
	}else if ( c_type == LEX_DOUBLE){
		gl();
	}else if ( c_type == LEX_PLUS || c_type == LEX_MINUS){
		gl();
		Unary();
	}else if ( c_type == LEX_LPAREN){
		gl();
		Expression();
		if ( c_type == LEX_RPAREN)
            gl ();
        else 
            throw curr_lex;
	}else
		throw curr_lex;
}

int main(int argc, char *argv[]){
	try{
		if (argc==2){
			Parser p(argv[1]);
			p.analyze();
		}else{
			throw "File was submitted incorrectly";
		}
	}
	catch(char c){
		cout << c << endl;;
		return 1;
	}
	catch ( Lex l){
		cout << l << endl;
		return 1;
	}
	catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
	
	return 0;
}
