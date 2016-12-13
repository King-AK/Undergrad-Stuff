#VJ Davey and Frank Brown
#CS 312

import re, sys, string

debug = False
dict = { } 
tokens = [ ] #for syntax
declarations = [] #list to hold declarations -- we havent covered types yet


#Statement objects here - Statement + 4 subclasses: Assign, If, Else, While
class Statement(object):
	def __str__(self):
		return ""


class Assign(Statement):
	def __init__(self, target, source):
		self.target = target
		self.source = source
	def __str__(self):
		return "= "+ str(self.target) + " " + str(self.source) #print function for the Assignment - source is an expression object


class If(Statement):
	def __init__(self, test, statement):
		self.test = test
		self.statement = statement
	def __str__(self):
		return "if " + str(self.test) +str(self.statement) + "\nend if" #print func for If - test is an Expression object
#could also have modified If __init__ to take multiple parameters with a tuple, but I felt the string print would get too messy for me
class Else(Statement):
	def __init__(self,test, statement, elseStatement):
		self.test = test
		self.statement = statement
		self.elseStatement = elseStatement
	def __str__(self):
		return "if "+str(self.test)+str(self.statement)+"\nelse\n"+str(self.elseStatement)+"\nend if"

class While(Statement):
	def __init__(self, test, statement):
		self.test = test
		self.statement = statement
	def __str__(self):
		return "while " + str(self.test) + str(self.statement) + "end while" #print func on While  - test is an Expression object



#  Expression class and its subclasses
class Expression( object ):
	def __str__(self):
		return "" 
	
class BinaryExpr( Expression ):
	def __init__(self, op, left, right):
		self.op = op
		self.left = left
		self.right = right
		
	def __str__(self):
		return str(self.op) + " " + str(self.left) + " " + str(self.right) + "\n" 

class Number( Expression ):
	def __init__(self, value):
		self.value = value
		
	def __str__(self):
		return str(self.value)
class Ident(Expression):
	def __init__(self, value):
		self.value = value
	def __str__(self):
		return str(self.value)
class String(Expression):
	def __init__(self,value):
		self.value = value
	def __str__(self):
		return str(self.value)

def error( msg ):
	#print msg
	sys.exit(msg)

# The "parse" function. This builds a list of tokens from the input string,
# and then hands it to a recursive descent parser for the PAL grammar.

def match(matchtok):
	tok = tokens.peek( )
	if (tok != matchtok): error("Match error : was expecting ' "+ matchtok+ " '")
	tokens.next( )
	return tok
	
def factor( ):
	#factor class expanded -- what was given was not enough
	"""factor     = number | string | ident |  "(" expression ")" """

	tok = tokens.peek( )
	if debug: print ("Factor: ", tok)
	if re.match(Lexer.number, tok):
		expr = Number(tok)
		tokens.next( )
		return expr
	if re.match(Lexer.identifier,tok):
		expr = Ident(tok)
		tokens.next( )
		return expr
	if re.match(Lexer.string, tok):
		expr = String(tok)
		tokens.next( )
		return expr
	if tok == "(":
		tokens.next( )  # or match( tok )
		expr = addExpr( )
		tokens.peek( )
		tok = match(")")
		return expr
	error("Invalid operand")
	return


def term( ):
	""" term    = factor { ('*' | '/') factor } """

	tok = tokens.peek( )
	if debug: print ("Term: ", tok)
	left = factor( )
	tok = tokens.peek( )
	while tok == "*" or tok == "/":
		tokens.next()
		right = factor( )
		left = BinaryExpr(tok, left, right)
		tok = tokens.peek( )
	return left

def addExpr( ):
	""" addExpr    = term { ('+' | '-') term } """

	tok = tokens.peek( )
	if debug: print ("addExpr: ", tok)
	left = term( )
	tok = tokens.peek( )
	while tok == "+" or tok == "-":
		tokens.next()
		right = term( )
		left = BinaryExpr(tok, left, right)
		tok = tokens.peek( )
	return left

'''
MY ADDITIONS START HERE
CREATE FUNCTIONS FOR EACH LHS NONTERMINAL NOT YET COVERED
'''
def relationalExpr( ):
	'''relationalExpr = addExpr [ relation addExpr ]'''

	tok = tokens.peek( )
	if debug: print ("relationalExpr: ",tok)
	left = addExpr( )
	tok = tokens.peek( )
	while tok == "==" or tok=="!=" or tok== "<" or tok== "<=" or tok ==">" or tok == ">=":
		tokens.next( )
		right = addExpr( )
		left = BinaryExpr(tok, left, right)
		tok = tokens.peek( )
	return left

def andExpr( ):
	'''andExpr    = relationalExpr { "and" relationalExpr }'''

	tok = tokens.peek( )
	if debug: print ("andExpr: ",tok)
	left = relationalExpr( )
	tok = tokens.peek( )
	while tok == "and":
		tokens.next()
		right = relationalExpr( )
		left = BinaryExpr(tok, left, right)
		tok = tokens.peek( )
	return left

def expression( ):
	'''expression = andExpr { "or" andExpr }'''

	tok  = tokens.peek( )
	if debug: print ("expression :",tok)
	left = andExpr( )
	tok = tokens.peek( )
	while tok == "or":
		tokens.next()
		right = andExpr( )
		left = BinaryExpr(tok, left, right)
		tok = tokens.peek( )
	return left

def block( ):
	'''block = ":" eoln indent stmtList undent'''
	#returns statement aggregates
	tok = match(":")
	if debug : print ("Block :", tok)
	tok = match(";")#eoln check
	tok = match("@")#indent check
	tok =tokens.peek( )
	block = []#list to contain all the statements that make up the block
	#while we arent at an undent, we are inside the block
	while tok != "~":
		#statement list
		statement = parseStmt(tokens)
		block.append(statement)
		tok = tokens.peek( )
	tok = match("~")#undent match discount double check 
	statement = ""
	for item in block:
		#concatenate all items inside block list into a string so we can print more simply
		statement = statement + str(item)
	return statement

def whileStatement( ):
	'''whileStatement = "while"  expression  block'''
	tok = match("while")
	if debug : print("while Statement : ", tok)
	tok = tokens.peek( )
	test = expression( )
	#print "is it true" +str(test) + "is it true?"
	tokens.peek( )
	statement = block( ) 
	return While(test, statement)#While is a subclass of Statement
	
		
def ifStatement( ):
	'''ifStatement = "if" expression block   [ "else" block ] '''
	tok = match("if")
	if debug: print("if Statement : ", tok)
	tok = tokens.peek( )
	test = expression( )
	tok = tokens.peek( )
	statement = block( ) #this is a list of statements -- a block
	tok = tokens.peek( )
	if tok == "else":
		tokens.next( )
		elseStatement = block( )
		return Else(test, statement, elseStatement)#Else is a subclass of Statement
	else:
		return If(test, statement)#If is a subclass of Statement
	
	
def assign( ):
	'''assign = ident "=" expression  eoln'''
	tok = tokens.peek( )
	if debug: print("Assignment : ", tok)
	target = re.match(Lexer.identifier, tok)
	target = Ident(tok)
	if target.__str__() not in declarations:
		declarations.append(target.__str__()) #every indentifier is a declarator in this language, this should result in a list of Ident objects
	tokens.next()
	match("=")#assignment match 
	source = expression( )#rhs derivation
	match(";")#eoln match
	return Assign(target,source) #Assign is a subclass of Statement
	
def parseStmt(tokens):
	#creates 1 of 3 possible statement objects through use of their corresponding functions and returns that object, otherwise returns an error
	'''statement = ifStatement |  whileStatement  |  assign'''
	tok = tokens.peek( )
	if tok ==  "if":
		statement = ifStatement( )
	elif tok == "while":
		statement = whileStatement( )
	elif re.match(Lexer.identifier,tok):
		statement = assign( )
	else:
		return error("Syntax Error on Statement")
	return statement

def parseStmtList( tokens ):
	""" gee = { Statement } """
	tok = tokens.peek( )
	ast = [] #abstract syntax tree
	while tok is not None:
		# need to store each statement in a list
		statement = parseStmt(tokens)
		ast.append(statement) #just to be safe, AST is still an aggregate that holds all the statement objects created as we go along
		tok = tokens.peek( )
	print("\n")
	return ast


def applyBinary(operator, value1, value2):
	#different than text, not checking every individual arithmetic operator
	return eval(str(value1)+str(operator)+str(value2))


def expressionMeaning(expr, state):
	#should also somehow be able to return boolean status for condiitonals and loops
	if isinstance(expr, Number):
		return str(expr)
	elif isinstance(expr, Ident):
		return state[str(expr)]
	elif isinstance(expr, String):
		return str(expr) #possibly use this as dictionary feeder?
	elif isinstance(expr, BinaryExpr):
		#check this
		return applyBinary(expr.op , statementMeaning(expr.left), statementMeaning(expr.right))

	else:
		return error("something fucked up, mang")	

	



def assignMeaning(assignment, state ):
	#replace the value of the target variable in the dictionary with the value of the source expression
	state[str(assignment.target)] = str(assignment.source)
	return state

def ifMeaning(conditional, state):
	#need to reevaluate how im writing this and also for else and while
	#tests are mem objects, will need to call str(x.test), additionally they come in polish prefix
	#this is probably where I should call the expression value method
	#if .test is true should be if expressionMeaning is true??
	if expressionMeaning(conditional.test, state) is True:
		return statementMeaning(conditional.statement)
	else:
		return state
def elseMeaning(conditional, state):
	#expression evaluation
	if expressionMeaning(conditional.test, state) is False:
		return statementMeaning(conditional.elseStatement)
	else:
		return state
def whileMeaning(loop, state):
	#expression evaluation
	if expressionMeaning(loop.test, state) is True:
		return whileMeaning(loop, statementMeaning(loop.Statement))
	else:
		return state

def statementMeaning(statement, state):
	#type check on the statement for various classes
	if isinstance(statement, Assign):
		return assignMeaning(statement, state)
	elif isinstance(statement, If):
		return ifMeaning(statement, state)
	elif isinstance(statement, Else):
		return elseMeaning(statement, state)
	elif isinstance(statement, While):
		return whileMeaning(statement,state)
	else:
		return error("something fucked up, mang")

def initialState(declarations):
	state={}#new blank dict to keep track of state
	#declarations are made when the parser runs through
	for variable in declarations:
		state[variable] = None #initialize to nothing at first
	return state

def programMeaning(statements):
	state = initialState(declarations)#dictionary that maps variable-value pairs. this is program state.
	for statement in statements:
		statementMeaning(statement, state)
	return state

def parse( text ) :
	global tokens
	tokens = Lexer( text )
	stmtlist = parseStmtList( tokens )
	for item in stmtlist:
		print "[ "+ item.__str__() +" ]"

	state = programMeaning(stmtlist)
	print state

	return



# Lexer, a private class that represents lists of tokens from a Gee
# statement. This class provides the following to its clients:
#
#   o A constructor that takes a string representing a statement
#       as its only parameter, and that initializes a sequence with
#       the tokens from that string.
#
#   o peek, a parameterless message that returns the next token
#       from a token sequence. This returns the token as a string.
#       If there are no more tokens in the sequence, this message
#       returns None.
#
#   o removeToken, a parameterless message that removes the next
#       token from a token sequence.
#
#   o __str__, a parameterless message that returns a string representation
#       of a token sequence, so that token sequences can print nicely

class Lexer :
	
	
	# The constructor with some regular expressions that define Gee's lexical rules.
	# The constructor uses these expressions to split the input expression into
	# a list of substrings that match Gee tokens, and saves that list to be
	# doled out in response to future "peek" messages. The position in the
	# list at which to dole next is also saved for "nextToken" to use.
	
	special = r"\(|\)|\[|\]|,|:|;|@|~|;|\$"
	relational = "<=?|>=?|==?|!="
	arithmetic = "\+|\-|\*|/"
	#char = r"'."
	string = r"'[^']*'" + "|" + r'"[^"]*"'
	number = r"\-?\d+(?:\.\d+)?"
	literal = string + "|" + number
	#idStart = r"a-zA-Z"
	#idChar = idStart + r"0-9"
	#identifier = "[" + idStart + "][" + idChar + "]*"
	identifier = "[a-zA-Z]\w*"
	lexRules = literal + "|" + special + "|" + relational + "|" + arithmetic + "|" + identifier
	
	def __init__( self, text ) :
		self.tokens = re.findall( Lexer.lexRules, text )
		self.position = 0
		self.indent = [ 0 ]
	
	
	# The peek method. This just returns the token at the current position in the
	# list, or None if the current position is past the end of the list.
	
	def peek( self ) :
		if self.position < len(self.tokens) :
			return self.tokens[ self.position ]
		else :
			return None
	
	
	# The removeToken method. All this has to do is increment the token sequence's
	# position counter.
	
	def next( self ) :
		self.position = self.position + 1
		return self.peek( )
	
	
	# An "__str__" method, so that token sequences print in a useful form.
	
	def __str__( self ) :
		return "<Lexer at " + str(self.position) + " in " + str(self.tokens) + ">"



def chkIndent(line):
	ct = 0
	for ch in line:
		if ch != " ": return ct
		ct += 1
	return ct
		

def delComment(line):
	pos = line.find("#")
	if pos > -1:
		line = line[0:pos]
		line = line.rstrip()
	return line

def mklines(filename):
	inn = open(filename, "r")
	lines = [ ]
	pos = [0]
	ct = 0
	for line in inn:
		ct += 1
		line = line.rstrip( )+";"
		line = delComment(line)
		if len(line) == 0 or line == ";": continue
		indent = chkIndent(line)
		line = line.lstrip( )
		if indent > pos[-1]:
			pos.append(indent)
			line = '@' + line
		elif indent < pos[-1]:
			while indent < pos[-1]:
				del(pos[-1])
				line = '~' + line
		print (ct, "\t", line)
		lines.append(line)
	# print len(pos)
	undent = ""
	for i in pos[1:]:
		undent += "~"
	lines.append(undent)
	# print undent
	return lines



def main():
	"""main program for testing"""
	global debug
	ct = 0
	for opt in sys.argv[1:]:
		if opt[0] != "-": break
		ct = ct + 1
		if opt == "-d": debug = True
	if len(sys.argv) < 2+ct:
		print ("Usage:  %s filename" % sys.argv[0])
		return
	parse("".join(mklines(sys.argv[1+ct])))
	return


main()