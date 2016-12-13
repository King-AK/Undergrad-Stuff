--VJ Davey and Frank Brown
--knightPlace.hs
--cs312 wm

--function knightPlace. Used to tell the safe spots to place a knight based on an input list which tells queen placements on an arbitrary sized chess board  
--"list" is the queen placement input. Assume there is at most 1 queen per column.

--algorithm:
--for every item in the input list that is not 0, that index+1 is an illegal column on the board
--that explicit item in the input list is an illegal row
--Updiagonal illegals are row+1,col+1 and row-1, col-1 relative to queen spot
--Downdiagonal illegals are row-1,col+1 and row+1,col-1 relative to queen spot
--return list that omits all the marked illegal spots
--have to make L moves illegal too
--board size = (length list)**2
import Data.List

knightPlace list = zeroReplace boardSize board
	    where 
	      --zeroReplace is a recursive function used to replace all necessary indices with the [0] matrix
	      zeroReplace 0 board = board
	      zeroReplace n board
	      		|elem n illegalCols = replaceAtIndex (n-1) [0] (zeroReplace (n-1) board)
	      		|otherwise = zeroReplace (n-1) board
	      
	      board = square boardSize boardSize
	      boardSize = length list
	      square x y = [[x|x<-[1..boardSize],takeRows x, sameDiag (x,y) numQueens, lOmit (x,y) numQueens]
	      			  |y <-[1..boardSize]] 

	      --helper functions
	      --used to replace item in list, effectively for clearing off illegal columns
	      replaceAtIndex n item ls = a ++ (item:b) where (a, (_:b)) = splitAt n ls
	      takeRows::Int->Bool
	      --used to clear off illegal Rows
	      takeRows j = 
		  if elem j illegalRows 
			  then False
			  else True    
	      --used to clear off illegal diagonals
	      sameDiag (a,b) numQueens
			|numQueens > 0 = sameDiagonal(a,b) (illegalRows!!numQueens, illegalCols!!numQueens) && sameDiag (a,b) (numQueens-1)
			|otherwise = sameDiagonal (a,b) (illegalRows!!numQueens, illegalCols!!numQueens)
	      sameDiagonal (a,b) (x,y) = abs (a-x) /= abs(b-y)
	      --used to clear off illegal knight L-moves
	      lOmit (a,b) numQueens
		        |numQueens >0 = lOmission(a,b) (illegalRows!!numQueens, illegalCols!!numQueens) && lOmit (a,b) (numQueens-1)
	                |otherwise = lOmission(a,b) (illegalRows!!numQueens, illegalCols!!numQueens)
	      lOmission (a,b) (x,y) 
			|(abs(a-x) == 1) && (abs(b-y)==2) = False 
                        |(abs(a-x) == 2) && (abs(b-y)==1) = False
			|otherwise = True
	
	      		

	      --important base stuff 
	      numQueens = length illegalCols-1 --number of queens placed on the board minus one, for list indices
	      illegalCols = [i+1 | i<-[0..length list -1], list!!i > 0] --list of the illegal columns
	      illegalRows = [i| i<-list, i > 0]--list of the illegal rows
