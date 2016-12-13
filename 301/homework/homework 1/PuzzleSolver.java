package edu.wm.cs.cs301.slidingpuzzle;
/**
 * The Puzzle solver. The user of this interface can configure the initial and goal state
 * of the puzzle. The user can also get the list of operations required to reach the goal state 
 * from the initial state. The PuzzleSolverInterface provides methods to get current and final 
 * puzzle state data structure implemented by the developer.
 * 
 * @author Probir
 *
 */
public interface PuzzleSolver {
	/**
	 * Configures the solver with initial and goal states. 
	 * As interfaces do not allow for the specification of a constructor. A default constructor with
	 * no parameters is assumed and the configure method is used to clarify what is necessary
	 * to properly initialize/configure an object of a class that implements the PuzzleSolver interface.
	 * The initial and goal states along with puzzle 
	 * dimensions(i.e. number of rows) are passed to this function. The states are provided as an array of integer.
	 * For Instance, an one dimensional integer array {0,1,3,2} represents a state of 2x2 puzzle. The puzzle 
	 * solver will be provided with two one dimensional integer arrays, initial and goal, representing the initial
	 * and goal state of the puzzle. The dimensions of the puzzle represents the number of blocks of the puzzle.
	 * For instance the dimension of a 2x2 puzzle is 3.
	 * @param initial Initial state represented as single dimensional array(i.e. {0,1,3,2} for 2x2 puzzle).
	 * Blank state is represented as 0.
	 * @param goal Goal state represented as single dimensional array(i.e. {0,1,3,2} for 2x2 puzzle).
	 * Blank state is represented as 0.
	 * @return Return true on successful configuration. false otherwise.
	 */
	boolean configure(int[] initial, int[] goal);
	/**
	 * Returns a list of operations representing the directions to move the 
	 * blank space in order to solve the puzzle. This method internally performs
	 * a search for a solution to a configured puzzle problem. If there is no solution
	 * movesToSolve must return null. For instance, {0,1,2,3} and {1,0,2,3} are the initial and goal state
	 * of a 2x2 puzzle. For this puzzle movesToSolve of the puzzle solver must return an array of operations
	 * containing only one element MOVELEFT. For impossible puzzle states i.e. {0,1,2,3} and {3,1,2,0} as initial
	 * and goal state respectively there is no solution. movesToSolve of puzzle solver must return null for invalid puzzles.
	 * @return array of operations or null
	 */
	Operation[] movesToSolve();
	/**
	 * get the solver's initial Puzzle State.
	 * @return initial Puzzle State
	 */
	PuzzleState getSolverInitialState();
	/**
	 * get the solver's goal Puzzle State
	 * @return Goal Puzzle State
	 */
	PuzzleState getSolverFinalState();
	/**
	 * get the number of states explored.
	 * This number provides a measure for the number of steps a search algorithm perform, 
	 * i.e. it is a measure of efficiency of a search algorithm. This method should
	 * only be called after a computation of a solution, i.e. movesToSolve().
	 */
	int getNumberOfStatesExplored();
	/**
	 * get the maximum number of elements in the queue of states that need to be explored.
	 * The queue is an internal data structure to store intermediate solutions that need
	 * further exploration. Its size provides a measure on the maximum number of states
	 * that are stored in memory during the exploration.
	 */
	int getMaxSizeOfQueue();
}