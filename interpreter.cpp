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

void from_st (stack< type_of_lex > & st, type_of_lex & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val_int;
    string 		 c_val_str;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
    void  Constant(type_of_lex tp);
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
       	c_val_doub= curr_lex.get_value_doub();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
	if (c_type == LEX_ID){
		if ( TID[curr_lex.get_value_int()].get_declare () ) 
            throw "twice";
        else {
            TID[curr_lex.get_value_int()].put_declare ();
            TID[curr_lex.get_value_int()].put_type ( tp );
        }
		gl();
	}else
		throw curr_lex;
	if (c_type == LEX_EQ ){
		gl();
		Constant(tp);
	}
}

void Parser::Constant(type_of_lex tp) {
	if (c_type == LEX_QUOTE){
		gl();
		if (tp != LEX_STRING)
			throw "the constant type does not match";
	}else{
		if (c_type == LEX_PLUS || c_type == LEX_MINUS )
			gl();
		if (c_type == LEX_NUM || c_type == LEX_DOUBLE){
			gl();
			if ((c_type == LEX_NUM && tp != LEX_INT) || (c_type == LEX_DOUBLE && tp != LEX_REAL))
				throw "the constant type does not match";
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
		poliz.push_back ( Lex (c_type) );
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
