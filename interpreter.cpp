#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stack>
#include <cstdlib>
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
    POLIZ_FGO,                                                                                  	/*44*/
    LEX_UPLUS,                                                                                   	/*45*/
    LEX_UMINUS                                                                                   	/*46*/
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
    string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_END )
        t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_QUOTE ){
        t = "QUOTE";
        s << '(' << t << ',' << l.str_lex << ");" << endl;
		return s;
    }else if ( l.t_lex == LEX_DOUBLE ){
        t = "DOUBLE";
        s << '(' << t << ',' << l.doub_lex << ");" << endl;
		return s;
    }else if ( l.t_lex == LEX_UPLUS )
        t = "UPLUS";
    else if ( l.t_lex == LEX_UMINUS )
        t = "UMINUS";
    else if ( l.t_lex == LEX_ID ){
        t = TID[l.v_lex].get_name ();
        if (TID[l.v_lex].get_type() == LEX_INT){
			s << '(' << t << ',' << TID[l.v_lex].get_value_int() << ");" << endl;
		}else if (TID[l.v_lex].get_type() == LEX_STRING){
			s << '(' << t << ',' << TID[l.v_lex].get_value_str() << ");" << endl;
		}else if (TID[l.v_lex].get_type() == LEX_REAL){ 
			s << '(' << t << ',' << TID[l.v_lex].get_value_doub() << ");" << endl;
		}
		return s;
    }else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO ) 
        t = "!F";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val_int;
    string 		 c_val_str;
    double		 c_val_doub;
    Scanner      scan;
    int 		 marker;
    int			 Marker;
    stack < type_of_lex >   st_lex;
	struct dint{
		int v;
		int p;
	};
	dint  gt, lb;
    vector < dint > 			Label_vector;
    vector < dint >				goto_vector;
    void  Program();
    void  Descriptions();
    void  Description();
    void  Type();
    void  Variable(type_of_lex tp);
    void  Constant(type_of_lex tp, int i);
    void  Operators();
    void  Operator();
    void  CompoundOp();
    void  Expression();
    void  Assignment();
    void  Expression1();
    void  PlusAndMinus();
    void  Comparison();
    void  Multiplication();
    void  Unary();
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_int ();
    void  check_unary ();
    void  check_id_in_read ();
    void  check_label ();
    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val_int = curr_lex.get_value_int ();
       	c_val_str = curr_lex.get_value_str();
       	c_val_doub= curr_lex.get_value_doub();
    }
public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
    gl ();
    Program();
    if (c_type != LEX_FIN)
        throw curr_lex;
    check_label ();
    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    for ( Lex l : poliz ) 
        cout << l;
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
	type_of_lex tp = curr_lex.get_type();
	Type();
	Variable(tp);
	while( c_type == LEX_COMMA){
		gl();
		Variable(tp);
	}	
}

void Parser::Type() {
	if ( c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_REAL )
		gl();
	else 
		throw curr_lex;
}

void Parser::Variable(type_of_lex tp) {
	int i;
	if (c_type == LEX_ID){
		if ( TID[curr_lex.get_value_int()].get_declare () ) 
            throw "twice";
        else {
            TID[curr_lex.get_value_int()].put_declare ();
            TID[curr_lex.get_value_int()].put_type ( tp );
            i = curr_lex.get_value_int();
        }
		gl();
	}else
		throw curr_lex;
	if (c_type == LEX_EQ ){
		gl();
		Constant(tp, i);
		TID[i].put_assign ();
	}
}

void Parser::Constant(type_of_lex tp, int i) {
	if (c_type == LEX_QUOTE){
		if (tp != LEX_STRING)
			throw "the constant type does not match";
		TID[i].put_value_str(c_val_str);
		gl();
	}else{
		int sign = 1;
		if (c_type == LEX_PLUS )
			gl();
		else if ( c_type == LEX_MINUS ){
			sign = -1;
			gl();
		}	
		if ( c_type == LEX_NUM ){
			if (c_type == LEX_NUM && tp != LEX_INT)
				throw "the constant type does not match";
			TID[i].put_value_int(sign*c_val_int);
			gl();	
		}else if ( c_type == LEX_DOUBLE ){
			if (c_type == LEX_DOUBLE && tp != LEX_REAL)
				throw "the constant type does not match";
			TID[i].put_value_doub(sign*c_val_doub);
			gl();
		}else
			throw curr_lex;
	}		
}

void Parser::Operators() {
	while ( c_type != LEX_END)
		Operator();
}

void Parser::Operator() {
	int pl0, pl1, pl2, pl3, pl4, pl5;
 
	if ( c_type == LEX_IF){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			Expression();
			eq_int ();
        	pl2 = poliz.size();
        	poliz.push_back ( Lex() );
        	poliz.push_back ( Lex(POLIZ_FGO) );
			if ( c_type == LEX_RPAREN){
				gl();
				Operator();
				pl3 = poliz.size ();
           		 poliz.push_back ( Lex () );
            	poliz.push_back ( Lex ( POLIZ_GO ) );
            	poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );
				if ( c_type == LEX_ELSE){
					gl();
					Operator();
                	poliz[pl3] = Lex ( POLIZ_LABEL, poliz.size() );
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_WHILE){
        pl0 = poliz.size ();
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			Expression();
			eq_int ();
        	pl1 = poliz.size (); 
        	poliz.push_back ( Lex () );
        	poliz.push_back ( Lex (POLIZ_FGO) );
			if ( c_type == LEX_RPAREN){
				gl();
				Operator();
            	poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
            	poliz.push_back ( Lex ( POLIZ_GO) );
            	poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_READ){
		gl();
		if ( c_type == LEX_LPAREN){
			gl();
			if ( c_type == LEX_ID){
				check_id_in_read ();
                poliz.push_back ( Lex( POLIZ_ADDRESS, c_val_int) );
				gl();
				if ( c_type == LEX_RPAREN){
					gl();
                    poliz.push_back ( Lex (LEX_READ) );
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
			st_lex.pop();
			while ( c_type == LEX_COMMA){
                poliz.push_back ( Lex ( LEX_WRITE ) );
				gl();
				Expression();
				st_lex.pop();
			}
			if ( c_type == LEX_RPAREN){
				gl();
                poliz.push_back ( Lex ( LEX_WRITE ) );
				if (c_type == LEX_SEMICOLON){
					gl();
				}else
					throw curr_lex;
			}else
				throw curr_lex;
		}else
			throw curr_lex;
	}else if ( c_type == LEX_DO) {
        pl5 = poliz.size ();
		gl();
		Operator();
		if ( c_type == LEX_WHILE){
			gl();
			if ( c_type == LEX_LPAREN){
				gl();
				Expression();
				eq_int ();
        		pl4 = poliz.size (); 
				poliz.push_back ( Lex () );
        		poliz.push_back ( Lex (POLIZ_FGO) );
        		poliz.push_back ( Lex ( POLIZ_LABEL, pl5 ) );
            	poliz.push_back ( Lex ( POLIZ_GO) );
            	poliz[pl4] = Lex ( POLIZ_LABEL, poliz.size() );
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
		marker = 1;
		Marker = 0;
		Assignment();
		if (Marker){
			gl();
			Operator();
		}else{
			st_lex.pop();
			if ( c_type == LEX_SEMICOLON){
				gl();
			}else
				throw curr_lex;
		}
	}else if ( c_type == LEX_GOTO) {
		gl();
		if ( c_type == LEX_ID){
			gt.p=poliz.size();
			gt.v=c_val_int;
			goto_vector.push_back(gt);
			poliz.push_back( Lex () );
			poliz.push_back( POLIZ_GO );
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
 
void Parser::Assignment() {
	Expression();
	if (c_type == LEX_EQ){
		if (poliz.back().get_type()==LEX_ID){
			int c = poliz.back().get_value_int();
			poliz.pop_back();
			poliz.push_back(Lex ( POLIZ_ADDRESS, c ));
		}else 
			throw "wrong expression are in =";
		gl();
		Assignment();
		eq_type();
        poliz.push_back ( Lex (LEX_EQ) );
	}
} 
 
void Parser::Expression() {
	Expression1();
	while( c_type == LEX_OR){
		st_lex.push ( c_type );
		gl();
		Expression1();
		check_op ();
	}
}

void Parser::Expression1() {
	Comparison();
	while( c_type == LEX_AND) {
		st_lex.push ( c_type);
		gl();
		Comparison();
		check_op ();
	}
}

void Parser::Comparison() {
	PlusAndMinus();
	while( c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_NEQ || c_type == LEX_DEQ || c_type == LEX_LEQ || c_type == LEX_GEQ){
		st_lex.push ( c_type );
		gl();
		PlusAndMinus();
		check_op ();
	}
}

void Parser::PlusAndMinus() {
	Multiplication();
	while( c_type == LEX_PLUS || c_type == LEX_MINUS){
		st_lex.push ( c_type );
		gl();
		Multiplication();
		check_op ();
	}
}

void Parser::Multiplication() {
	Unary();
	while( c_type == LEX_TIMES || c_type == LEX_SLASH){
		st_lex.push ( c_type );
		gl();
		Unary();
		check_op ();
	}
}

void Parser::Unary() {
	if ( c_type == LEX_ID){	
		if (TID[c_val_int].get_declare()){
			st_lex.push ( TID[c_val_int].get_type () );	
        	poliz.push_back ( curr_lex );
			gl();
		}else{
			lb.p=poliz.size();
			lb.v=c_val_int;
			Label_vector.push_back(lb);
			gl();
			if ( c_type == LEX_COLON && marker == 1)
				Marker = 1; 
			else 
				throw "not declared";
		}
	}else if ( c_type == LEX_NOT){
		gl();
		Unary();
		check_not ();
	}else if ( c_type == LEX_NUM){
		st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
		gl();
	}else if ( c_type == LEX_QUOTE){
		st_lex.push ( LEX_STRING );
        poliz.push_back ( curr_lex );
		gl();
	}else if ( c_type == LEX_DOUBLE){
		st_lex.push ( LEX_REAL );
        poliz.push_back ( curr_lex );
		gl();
	}else if ( c_type == LEX_PLUS || c_type == LEX_MINUS){
		if (c_type == LEX_PLUS){
			poliz.push_back ( Lex (LEX_UPLUS) );	
		}else{
			poliz.push_back ( Lex (LEX_UMINUS) );
		}
		gl();
		Unary();
		check_unary();
	}else if ( c_type == LEX_LPAREN){
		gl();
		Expression();
		if ( c_type == LEX_RPAREN)
            gl ();
        else 
            throw curr_lex;
	}else
		throw curr_lex;
	marker=0;
}

void Parser::check_id () {
    if ( TID[c_val_int].get_declare() )
        st_lex.push ( TID[c_val_int].get_type () );
    else 
        throw "not declared";
}

void Parser::check_op () {
    type_of_lex t1, t2, op, r = LEX_NULL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
 
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH ){
    	if ( t2==t1 && t1==LEX_INT)
    		r = LEX_INT;
    	else if ((t2==t1 && t1==LEX_REAL)||(t2==LEX_INT && t1==LEX_REAL)||(t1==LEX_INT && t2==LEX_REAL))
    		r = LEX_REAL;
    }
	if ((op == LEX_OR || op == LEX_AND) && (t2==t1 && t1==LEX_INT))
		r = LEX_INT;
	if (( op == LEX_GTR || op == LEX_LSS || op == LEX_LEQ || op == LEX_NEQ || op == LEX_GEQ || op == LEX_DEQ) && (t1 == LEX_INT || t1 == LEX_REAL) && (t2 == LEX_INT || t2 == LEX_REAL))
    	r = LEX_INT;
	if (t1 == LEX_STRING && t2 == LEX_STRING ){
		if (op == LEX_PLUS)
			r = LEX_STRING;
		else if ( op == LEX_GTR || op == LEX_LSS || op == LEX_NEQ || op == LEX_DEQ)
				r = LEX_INT;
	}
    if ( r == LEX_NULL)
        throw "wrong types are in operation";
    st_lex.push(r);
    poliz.push_back (Lex (op) );
}

void Parser::check_not () {
    if (st_lex.top() != LEX_INT)
        throw "wrong type is in not";
    else  
        poliz.push_back ( Lex (LEX_NOT) );
}

void Parser::check_unary () {
	if (st_lex.top() != LEX_INT && st_lex.top() != LEX_REAL)
        throw "wrong type is in unary";
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
		if (!((t == LEX_INT && st_lex.top() == LEX_REAL) || (t == LEX_REAL && st_lex.top() == LEX_INT))) 
			throw "wrong types are in =";
}

void Parser::eq_int () {
    if ( st_lex.top () != LEX_INT )
        throw "expression is not integer";
    st_lex.pop ();
}

void Parser::check_id_in_read () {
    if ( !TID [c_val_int].get_declare() )
        throw "not declared";
}

void Parser::check_label () {
	int index;
	for (unsigned i = 0; i < Label_vector.size(); i++){
		index = Label_vector[i].v;
		//cout << "LABEL:" << endl;
		//cout << TID[index].get_name() << endl;
		if (TID[index].get_declare())
			throw "twice";
		else
			TID[index].put_declare();
	}
	vector<dint>::iterator it;
	for (unsigned j = 0; j < goto_vector.size(); j++){
		index = goto_vector[j].v;
		//cout << "GOTO:" << endl;
		//cout << TID[index].get_name() << endl;
		it = find_if(Label_vector.begin(), Label_vector.end(), [&index](dint a){
			return a.v == index;
		} );
		if (it==Label_vector.end())
			throw "not declared";
		poliz[unsigned(goto_vector[j].p)]=Lex(POLIZ_LABEL, it->p);
	}
}

class Executer {
public:
    void execute ( vector<Lex> & poliz );
};

void Executer::execute ( vector<Lex> & poliz ) {
    Lex pc_el;
    int st_last1=0;
    int st_last2;
    stack < int > args_int;
    stack < string > args_str;
    stack < double > args_doub;
    int i_int, j_int, index = 0, size = poliz.size();
    string i_str, j_str;
    double i_doub, j_doub;
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args_int.push ( pc_el.get_value_int() );
                st_last2 = st_last1;
                st_last1 = 0;
                break;
                
            case LEX_QUOTE: 
            	args_str.push ( pc_el.get_value_str() ); 
            	st_last2 = st_last1;
            	st_last1 = 1;
            	break;
            	
            case LEX_DOUBLE:
            	args_doub.push ( pc_el.get_value_doub() ); 
            	st_last2 = st_last1;
            	st_last1 = 2;
            	break;
            
            case LEX_ID:
                i_int = pc_el.get_value_int ();
                if ( TID[i_int].get_assign () ) {
                	if (TID[i_int].get_type() == LEX_INT){
                    	args_int.push ( TID[i_int].get_value_int () );
                    	st_last2 = st_last1;
                		st_last1 = 0;
                    }else if (TID[i_int].get_type() == LEX_STRING){
                    	args_str.push( TID[i_int].get_value_str() );
                    	st_last2 = st_last1;
            			st_last1 = 1;
                    }else{
						args_doub.push( TID[i_int].get_value_doub());	
						st_last2 = st_last1;
            			st_last1 = 2;
					}
                }else
                    throw "POLIZ: indefinite identifier";
                break;
 
            case LEX_NOT:
                from_st ( args_int, i_int );
                args_int.push( !i_int );
                st_last1 = 0;
                break;
 
            case LEX_OR:
                from_st ( args_int, i_int ); 
                from_st ( args_int, j_int );
                st_last1 = 0;
                if (j_int)
                	args_int.push (1);
				else 
	                args_int.push ( j_int || i_int );
				break;
				
            case LEX_AND:
                from_st ( args_int, i_int ); 
                from_st ( args_int, j_int );
                st_last1 = 0;
                if (!j_int)
                	args_int.push (0);
				else 
	                args_int.push ( j_int && i_int );
				break;
 
            case POLIZ_GO:
                from_st ( args_int, i_int );
                index = i_int - 1;
                break;
 
            case POLIZ_FGO:
                from_st ( args_int, i_int );
                from_st ( args_int, j_int );
                if ( !j_int ) index = i_int - 1;
                break;
 
            case LEX_WRITE:
            	if (st_last1 == 0){
                	from_st ( args_int, j_int );
                	cout << j_int << endl;
            	}else if (st_last1 == 1){
            		from_st ( args_str, j_str);
            		cout << j_str << endl;
				}else {
					from_st ( args_doub, j_doub);
            		cout << j_doub << endl;
				}
                break;
 
            case LEX_READ:
                from_st ( args_int, i_int );
                if ( TID[i_int].get_type () == LEX_INT ) {
                	int k;
                    cout << "Input int value for " << TID[i_int].get_name () << endl;
                    cin >> k;
                	TID[i_int].put_value_int (k);
                }
                else if (TID[i_int].get_type () == LEX_STRING){
                	string k;
                    cout << "Input string value for " << TID[i_int].get_name () << endl;
                    cin >> k;
                	TID[i_int].put_value_str (k);
                }else{
                	double k;
                    cout << "Input double value for " << TID[i_int].get_name () << endl;
                    cin >> k;
                	TID[i_int].put_value_doub (k);
				}
                TID[i_int].put_assign ();
                break;
 
            case LEX_PLUS:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( i_int + j_int);
            		st_last1 = 0;
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( i_doub + j_doub);
            		st_last1 = 2;
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_doub.push ( i_doub + j_int);
            		st_last1 = 2;
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( i_int + j_doub);
            		st_last1 = 2;
            	}else if ((st_last1 == 1)&&(st_last2 == 1)){
            		from_st ( args_str, i_str);
            		from_st ( args_str, j_str);
            		args_str.push ( j_str + i_str);
            		st_last1 = 1;
            	}
                break;
                
            case LEX_UPLUS:
            	if (st_last1 == 0){
            		from_st ( args_int, i_int);
            		args_int.push ( +i_int);
            		st_last1 = 0;
				}else {
					from_st ( args_doub, i_doub);
            		args_doub.push ( +i_doub);
            		st_last1 = 2;
				}
            	break;
            	
            case LEX_UMINUS:
            	if (st_last1 == 0){
            		from_st ( args_int, i_int);
            		args_int.push ( -i_int);
            		st_last1 = 0;
				}else {
					from_st ( args_doub, i_doub);
            		args_doub.push ( -i_doub);
            		st_last1 = 2;
				}
            	break;
 
            case LEX_TIMES:
                if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( i_int * j_int);
            		st_last1 = 0;
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( i_doub * j_doub);
            		st_last1 = 2;
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_doub.push ( i_doub * j_int);
            		st_last1 = 2;
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( i_int * j_doub);
            		st_last1 = 2;
            	}
                break;
 
            case LEX_MINUS:
                if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int - i_int);
            		st_last1 = 0;
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( j_doub - i_doub);
            		st_last1 = 2;
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_doub.push ( j_doub - i_int);
            		st_last1 = 2;
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_doub.push ( j_int - i_doub);
            		st_last1 = 2;
            	}
                break;
 
            case LEX_SLASH:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		if (!i_int)
                    	throw "POLIZ:divide by zero";
                    args_int.push ( j_int / i_int );
            		st_last1 = 0;
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		if (i_int==0)
                    	throw "POLIZ:divide by zero";
            		args_doub.push ( j_doub / i_doub);
            		st_last1 = 2;
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		if (i_int==0)
                    	throw "POLIZ:divide by zero";
            		args_doub.push ( j_doub / i_int);
            		st_last1 = 2;
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		if (i_int==0)
                    	throw "POLIZ:divide by zero";
            		args_doub.push ( j_int / i_doub);
            		st_last1 = 2;
            	}
                break;
 
            case LEX_DEQ:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( i_int == j_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( i_doub == j_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( i_doub == j_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( i_int == j_doub);
            	}else if ((st_last1 == 1)&&(st_last2 == 1)){
            		from_st ( args_str, i_str);
            		from_st ( args_str, j_str);
            		args_int.push ( i_str == j_str);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_LSS:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int < i_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_doub < i_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( j_doub < i_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_int < i_doub);
            	}else if ((st_last1 == 1)&&(st_last2 == 1)){
            		from_st ( args_str, i_str);
            		from_st ( args_str, j_str);
            		args_int.push ( j_str < i_str);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_GTR:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int > i_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_doub > i_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( j_doub > i_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_int > i_doub);
            	}else if ((st_last1 == 1)&&(st_last2 == 1)){
            		from_st ( args_str, i_str);
            		from_st ( args_str, j_str);
            		args_int.push ( j_str > i_str);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_LEQ:
				if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int <= i_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_doub <= i_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( j_doub <= i_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_int <= i_doub);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_GEQ:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int >= i_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_doub >= i_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( j_doub >= i_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_int >= i_doub);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_NEQ:
            	if ((st_last1 == 0)&&(st_last2 == 0)){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		args_int.push ( j_int != i_int);
            	}else if ((st_last1 == 2)&&(st_last2 == 2)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_doub != i_doub);
            	}else if ((st_last1 == 2)&&(st_last2 == 0)){
            		from_st ( args_doub, i_doub);
            		from_st ( args_int, j_int);
            		args_int.push ( j_doub != i_int);
            	}else if ((st_last1 == 0)&&(st_last2 == 2)){
            		from_st ( args_int, i_int);
            		from_st ( args_doub, j_doub);
            		args_int.push ( j_int != i_doub);
            	}else if ((st_last1 == 1)&&(st_last2 == 1)){
            		from_st ( args_str, i_str);
            		from_st ( args_str, j_str);
            		args_int.push ( j_str != i_str);
            	}
            	st_last1 = 0;
                break;
 
            case LEX_EQ:
            	if ( st_last1 == 0){
            		from_st ( args_int, i_int);
            		from_st ( args_int, j_int);
            		if (TID[j_int].get_type() == LEX_INT)
						TID[j_int].put_value_int(i_int);
					else 
						TID[j_int].put_value_doub(i_int);
            		if ((index + 1 < size) && (poliz[index + 1].get_type() == LEX_EQ)){
                		args_int.push( i_int );
					}
				}else if ( st_last1 == 1){
					from_st ( args_str, i_str);
					from_st ( args_int, j_int);
					TID[j_int].put_value_str(i_str);
					if ((index + 1 < size) && (poliz[index + 1].get_type() == LEX_EQ)){
                		args_str.push( i_str );
                	}
				}else{
					from_st ( args_doub, i_doub);
					from_st ( args_int, j_int);
					if (TID[j_int].get_type() == LEX_INT)
						TID[j_int].put_value_int(int(i_doub));
					else 
						TID[j_int].put_value_doub(i_doub);
					if ((index + 1 < size) && (poliz[index + 1].get_type() == LEX_EQ)){
                		args_doub.push( i_doub );
                	}
				}
                TID[j_int].put_assign (); 
                break;
 
            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

int main(int argc, char *argv[]){
	try{
		if (argc==2){
			Interpretator I(argv[1]);
			I.interpretation();
			return 0;
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
}
