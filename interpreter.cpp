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
    LEX_DEQ, LEX_PER, LEX_TSLASH, LEX_SLASHT, LEX_BEGIN, LEX_END, 									/*37*/
    LEX_NUM,																						/*38*/
	LEX_QUOTE,                                                                                    	/*39*/
    LEX_ID,                                                                                     	/*40*/
    POLIZ_LABEL,                                                                                	/*41*/
    POLIZ_ADDRESS,                                                                              	/*42*/
    POLIZ_GO,                                                                                   	/*43*/
    POLIZ_FGO                                                                                   	/*44*/
};

/////////////////////////  ????? Lex  //////////////////////////
 
class Lex {
    type_of_lex   t_lex;
    int           v_lex;
    string        str_lex;
    double        doub_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, string s = "", double d = 0 ): t_lex (t), v_lex (v), str_lex (s), doub_lex(d)  { }
    type_of_lex  get_type () const { 
    	  return t_lex; 
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

/////////////////////  ????? Ident  ////////////////////////////
 
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

//////////////////////  TID  ///////////////////////
 
vector<Ident> TID;
 
int put ( const string & buf ) {
    vector<Ident>::iterator k;
 
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}
 
/////////////////////////////////////////////////////////////////
 
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
    "@", ";", ",", ":", "(", ")", 
    "=", "<", ">", "+", "-", "*", 
	"/","<=", "!=", ">=", "==",  
    "%","*/", "/*", "{", "}", 
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
                    else if (c == '@')
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
                }
                else if (i == 0.1) {
                	throw c;
				}
				else {
                    ungetc ( c, fp );
                    return Lex ( LEX_NUM, 0, "", f );
                }
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
    } //end switch
  } while (true);
}
 
 ostream & operator<< ( ostream &s, Lex l ) {
    s << '(' << l.t_lex << ',' << l.v_lex << ',' << l.str_lex << ',' << l.doub_lex << ");" << endl;
    return s;
}
 
int main(){
	Scanner s("prog.txt");
	Lex l;
	try{
		while(true){
			cout << s.get_lex();
		}
	}
	catch(char c){
	}
	
	return 0;
}
