#VJ Davey and Frank Brown
#CS 312
#Project 2 for CS312 - SYNTAX

#proper __str__ functions on Statement subclasses
#check code on the statement functions -- I feel there may be too many peeks which might cause jumping and shit


import re, sys, string

debug = True
dict = { } #used for semantics
tokens = [ ] #used for syntax



#create a class for ident(VarRef)<identifier> and String up here after parse routines r done
class Ident ( VarRef ) :
	def __str__(self):
		return ""
class String ( String ) :
	def __str__(self):
		return ""


#Make Statement objects here - Statement + 3 subclasses: Assign, If, While
class Statement(object):
	def __str__(self):
		return ""


class Assign(Statement):
	def __init__(self, target, source):
		self.target = target
		self.source = source
	def __str__(self):
		#print function for the Assignment - source is an expression object


class If(Statement):
	def __init__(self, test, statement):
		self.test = test
		self.statement = statement
	def __str__(self):
		#print func for If - test is an Expression object


class While(Statement):
	def __init__(self, test, statement):
		self.test = test
		self.statement = statement
	def __str__(self):
		#print func on While  - test is an Expression object



#  Expression class and its subclasses - MAKE SURE THESE PRINT IN POLISH PREFIX
class Expression( object ):
	def __str__(self):
		return "" 
	
class BinaryExpr( Expression ):
	def __init__(self, op, left, right):
		self.op = op
		self.left = left
		self.right = right
		
	def __str__(self):
		return str(self.op) + " " + str(self.left) + " " + str(self.right)

class Number( Expression ):
	def __init__(self, value):
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
	if (tok != matchtok): error("Expecting "+ matchtok)
	tokens.next( )
	return tok
	
def factor( ):
	"""factor     = number |  '(' expression ')' """

	tok = tokens.peek( )
	if debug: print ("Factor: ", tok)
	if re.match(Lexer.number, tok):
		expr = Number(tok)
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
	while tok == "==" | "!=" | "<" | "<=" | ">" | ">=":
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
	tok = match(":")
	if debug : print ("Block :", tok)
	tok = match(";")#eoln check
	tok = match("@")#indent check
	tok =tokens.peek( ) 
	stmtList = parseStmtList(tok) #this might go too far -- we want it to stop at the undent!
	tok = tokens.peek( )
	match("~")#undent match
	return stmtlist

def whileStatement( ):
	'''whileStatement = "while"  expression  block'''
	tok = match("while")
	if debug : print("while Statement : ", tok)
	tok = tokens.peek( )
	test = expression( )
	tokens.peek( )
	statement = block( ) 
	return While(test, statement)#While is a subclass of Statement
	
		
def ifStatement( ):
	'''ifStatement = "if" expression block   [ "else" block ] '''
	tok = match("if" )
	if debug: print("if Statement : ", tok)
	tok = tokens.peek( )
	test = expression( )
	tok = tokens.peek( )
	statement = block( )
	tok = tokens.peek( )
	if tok == "else":
		tokens.peek()
		elseStatement = block( )
		return If(test, statement, elseStatement)#If is a subclass of Statement
	else:
		return If(test, statement)#If is a subclass of Statement
	
	
def assign( ):
	'''assign = ident "=" expression  eoln'''
	tok = tokens.peek( )
	if debug: print("Assignment : ", tok)
	target = re.match(Lexer.identifier, tok):
	match("=")#assignment match
	source = expression( )#rhs derivation
	match(";")#eoln match
	return Assign(target,source) #Assign is a subclass of Statement
	
def parseStmt(tokens):
	#creates 1 of 3 possible statement objects and returns that object, otherwise returns an error
	'''statement = ifStatement |  whileStatement  |  assign'''
	tok = tokens.peek( )
	if tok ==  "if":
		statement = ifStatement( )
	elif tok == "while":
		statement = whileStatement( )
	else if re.match(Lexer.identifier,tok):
		statement = assign( )
	else:
		return error("Syntax Error on Statement")

	return statement

def parseStmtList( tokens ):
	#as long as the token list is not empty this function considers the possiblity of more statements existing
	#uses parseStmt to discern the kind of statement we have and appends it to the ast
	""" gee = { Statement } """
	tok = tokens.peek( )
	ast = [] #abstract syntax tree
	while tok is not None:
		# need to store each statement in a list
		statement = parseStmt(tokens)
		ast.append(statement)
    print(str(ast))
	return ast

def parse( text ) :
	global tokens
	tokens = Lexer( text )
	'''expr = addExpr( )
	print (str(expr))
	#     Or:'''
	stmtlist = parseStmtList( tokens )
	print str(stmtlist)
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
