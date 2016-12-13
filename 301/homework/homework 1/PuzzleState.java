package edu.wm.cs.cs301.slidingpuzzle;
/**
 * Puzzle State Data Structure. The user of this interface can perform operations 
 * on the implemented puzzle game. Users can compare current puzzle state to other specified state.
 * Users can read the current puzzle state in single dimensional array.
 * 
 * The PuzzleStateInterface provides four move operations: moveUp, moveDown, moveLeft and moveRight.
 * Generally on each move operation puzzle state should be changed.
 * 
 * 
 * @author Probir
 *
 */
public interface PuzzleState {
	/**
	 * Allows to configure the puzzle state to a particular game state. 
	 * The state is provided as an array of integer.
	 * For Instance, an one dimensional integer array {0,1,3,2} represents a state of 2x2 puzzle
	 * with the first row carrying values 0 and 1, the second row carrying values 3 and 2.
	 * @param gameState provides a value for each tile in the current state of the game. 
	 */
	public void configureState(int[] gameState);
	/**
	 * Returns a new instance of PuzzleState where the gap and the value above it are switched.
	 * @return new PuzzleState for legal move. For illegal move operation it return null.
	 */
	PuzzleState moveUp();
	/**
	 * Returns a new instance of PuzzleState where the gap and the value below it are switched.
	 * @return new PuzzleState for legal move. For illegal move operation it return null.
	 */
	PuzzleState moveDown();
	/**
	 * Returns a new instance of PuzzleState where the gap and the value to its left are switched.
	 * @return new PuzzleState for legal move. For illegal move operation it return null.
	 */
	PuzzleState moveLeft();
	/**
	 * Returns a new instance of PuzzleState where the gap and the value to its right are switched.
	 * @return new PuzzleState for legal move. For illegal move operation it return null.
	 */
	PuzzleState moveRight();
	
	/**
	 * Compare other puzzle state to current puzzle state.
	 * @param other puzzle state to compare
	 * @return true if equal and false otherwise
	 */
	boolean isEqual(PuzzleState other);
	/**
	 * Get the current puzzle state in a single dimensional array (i.e. {0,1,3,2} for 2x2 puzzle). 
	 * Blank tile is represented as 0. The coding goes by rows, for the example the top row 
	 * contains values 0 and 1 from left to right.
	 * @return Single dimensional array of integer representing puzzle state
	 */
	int[] getStateArray();
}
