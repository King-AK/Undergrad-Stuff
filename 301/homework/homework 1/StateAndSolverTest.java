package edu.wm.cs.cs301.slidingpuzzle;
import static org.junit.Assert.*;

import java.util.Random;
import java.util.logging.Logger;

import org.junit.Test;


/**
 * Bundle of test cases to verify the puzzle solver.
 * @author Probir, Peter Kemper
 *
 */
public class StateAndSolverTest {
	
	static final Logger LOG = Logger.getLogger(StateAndSolverTest.class.getName());
	
	/**
	 * Wrapper to encapsulate constructor call for class that implements PuzzleState interface
	 * @param puzzle
	 * @return
	 */
	PuzzleState createPuzzleState(int[] puzzle) {
		PuzzleState pState= new SimplePuzzleState();
	    pState.configureState(puzzle);
	    return pState ;
	}
	
	// a few particular puzzles to test
	// 2 8 3
	// 1 6 4
	// 7 5 0
	final int[] puzzleLowerRightCorner = {2,8,3,1,6,4,7,5,0};
	// 2 8 3
	// 1 6 4
	// 7 0 5	
	final int[] puzzleLastRowMiddlePosition = {2,8,3,1,6,4,7,0,5};
	// 2 8 3	
	// 1 0 4 
	// 7 6 5	
	final int[] puzzleCenterPosition = {2,8,3,1,0,4,7,6,5};
	// 2 0 3
	// 1 6 4 
	// 7 8 5
	final int[] puzzleFirstRowMiddlePosition = {2,0,3,1,6,4,7,8,5};
	// 2 0 3
	// 1 8 4 
	// 7 6 5
	final int[] puzzleFirstRowMiddlePosition2 = {2,0,3,1,8,4,7,6,5};
	// 2 8 3
	// 1 6 4
	// 0 7 5 
	final int[] puzzleLowerLeftCorner = {2,8,3,1,6,4,0,7,5} ;
	// 0 8 3
	// 1 6 4
	// 2 7 5 
	final int[] puzzleUpperLeftCorner = {0,8,3,1,6,4,2,7,5} ;
	// 8 3 0
	// 1 6 4
	// 2 7 5 	
	final int[] puzzleUpperRightCorner = {8,3,0,1,6,4,2,7,5} ;
	
	// some 2 x 2 puzzles
	// note that puzzles are not necessarily reachable from each other
	final int[] smallPuzzleUpperLeftCorner = {0,1,2,3};
	final int[] smallPuzzleLowerLeftCorner = {1,3,0,2};
	final int[] smallPuzzleLowerRightCorner = {3,1,2,0}; 
	final int[] smallPuzzleUpperRightCorner = {1,0,2,3};

	/**
	 * Produces a string with the content of a puzzle, used for error messages
	 * @param puzzle
	 * @return
	 */
	private String printPuzzle(int[] puzzle) {
		String result = "" ;
		for (int i = 0 ; i < puzzle.length ; i++) {
			result += " " + puzzle[i] ;
		}
		return result ;
	}
	/**
	 * Test case verifies the isEqual function of a Puzzle state. 
	 * isEqual function must handle Null puzzle state.
	 */
	@Test
	public void testIsEquals() {
		// create two puzzles of dimensions 3x3 that differ
		int[] puzzle1 = puzzleLastRowMiddlePosition.clone();
		int[] puzzle2 = puzzleLowerRightCorner.clone();
		// create 3 puzzle states to compare, 
	    PuzzleState pState = createPuzzleState(puzzle1) ;
	    PuzzleState pSame = createPuzzleState(puzzle1) ;
	    PuzzleState pDifferent= createPuzzleState(puzzle2) ;
	     
	    // check properties of isEqual method
	    // no state is equal to null
	    assertFalse(pState.isEqual(null));
	    assertFalse(pSame.isEqual(null));
	    assertFalse(pDifferent.isEqual(null));
	    // equality is reflexive: every state is equal to itself
	    assertTrue(pState.isEqual(pState));
	    assertTrue(pSame.isEqual(pSame));
	    assertTrue(pDifferent.isEqual(pDifferent));
	    // equality is symmetric
	    assertTrue(pState.isEqual(pSame));
	    assertTrue(pSame.isEqual(pState));
	    assertFalse(pState.isEqual(pDifferent));
	    assertFalse(pDifferent.isEqual(pState));
	    // equality is transitive: not checked
	}

	//////////////////////// test individual moves ////////////////////////////////
	/**
	 * Test case verifies moveRight returns null on invalid move.
	 */
	@Test
	public void testMoveRightExpectException(){
		// create an arbitrary puzzle and puzzle state
		PuzzleState pState2 = createPuzzleState(puzzleLowerRightCorner.clone()) ;
	    // perform a move that is not legal
	    // current state:
	    // 2 8 3
	    // 1 6 4 -> the 0 position can not move right or down
	    // 7 5 0
	    // 
		PuzzleState pStateNull = pState2.moveRight();
		assertNull(pStateNull);
	}
	/**
	 * Test case verifies moveRight function performs a valid right shift of specified block.
	 */
	@Test
	public void testMoveRightNoException(){
		// 2 8 3		2 8 3
		// 1 6 4  -> 	1 6 4
		// 7 0 5		7 5 0
		PuzzleState start = createPuzzleState(puzzleLastRowMiddlePosition.clone()) ;
		PuzzleState goal = createPuzzleState(puzzleLowerRightCorner.clone()) ;
		
		PuzzleState result = start.moveRight();
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
			
	}
	/**
	 * Test case verifies moveDown returns null on invalid move.
	 */
	@Test
	public void testMoveDownExpectException(){
		PuzzleState start = createPuzzleState(puzzleLowerRightCorner.clone()) ;
	    // perform a move that is not legal
	    // current state:
	    // 2 8 3
	    // 1 6 4 -> the 0 position can not move right or down
	    // 7 5 0
	    // 
	    PuzzleState pStateNull = start.moveDown();
		assertNull(pStateNull);
	}
	/**
	 * Test case verifies moveDown function performs a valid down shift of specified block.
	 */
	@Test
	public void testMoveDownNoException(){
		// 2 8 3	2 8 3
		// 1 0 4 ->	1 6 4
		// 7 6 5	7 0 5
		PuzzleState start = createPuzzleState(puzzleCenterPosition.clone()) ;
		PuzzleState goal = createPuzzleState(puzzleLastRowMiddlePosition.clone()) ;
		
		PuzzleState result = start.moveDown();
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
			
	}
	/**
	 * Test case verifies moveUp returns null on invalid move.
	 */	
	@Test
	public void testMoveUpExpectException(){
		PuzzleState start = createPuzzleState(puzzleFirstRowMiddlePosition.clone()) ;
	    PuzzleState pStateNull = start.moveUp();
	    assertNull(pStateNull);
	}
	/**
	 * Test case verifies moveUp function performs a valid up shift of specified block.
	 */	
	@Test
	public void testMoveUpNoException(){
		// 2 8 3	2 0 3	
		// 1 0 4 -> 1 8 4
		// 7 6 5	7 6 5
		PuzzleState start = createPuzzleState(puzzleCenterPosition.clone()) ;
		PuzzleState goal = createPuzzleState(puzzleFirstRowMiddlePosition2.clone()) ; // note: 2 puzzles of that kind, 2nd one is it
	
		PuzzleState result = start.moveUp();
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
		
	}
	/**
	 * Test case verifies moveLeft returns null on invalid move.
	 */
	@Test
	public void testMoveLeftExpectException(){
		PuzzleState start = createPuzzleState(puzzleLowerLeftCorner.clone()) ;
		PuzzleState pStateNull = start.moveLeft();
	    assertNull(pStateNull);
	}
	/**
	 * Test case verifies moveLeft function performs a valid left shift of specified block.
	 */
	@Test
	public void testMoveLeftNoException(){
		PuzzleState start = createPuzzleState(puzzleLastRowMiddlePosition.clone()) ;
		PuzzleState goal = createPuzzleState(puzzleLowerLeftCorner.clone()) ;
	
		PuzzleState result = start.moveLeft();
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
			
	}
	////////////////// test sequences of moves //////////////////////////////////////////////////
	/**
	 * Helper method that performs a move into a given direction and then back 
	 * such that the returned state equals the starting state if the movement is possible.
	 * @param start
	 * @param op the direction to move
	 * @return null if movement is not possible
	 */
	private PuzzleState moveBackAndForth(PuzzleState start, Operation op){
		PuzzleState result = start ;
		switch (op) {
		case MOVEUP :
			if(result!=null)
				result = result.moveUp() ;
			if(result!=null)
				result = result.moveDown() ;
			break ;
		case MOVEDOWN :
			if(result!=null)
				result = result.moveDown() ;
			if(result!=null)
				result = result.moveUp() ;
			break ;
		case MOVELEFT :
			if(result!=null)
				result = result.moveLeft() ;
			if(result!=null)
				result = result.moveRight() ;
			break ;
		case MOVERIGHT :
			if(result!=null)
				result = result.moveRight() ;
			if(result!=null)
				result = result.moveLeft() ;
			break ;
		default: 
			fail("Impossible input for parameter op: " + op) ;
		}
		return result ;
	}
	/**
	 * Test possible moves in all directions from the center position of a 3 x 3 puzzle.
	 * We use the invariant that a pair of right-left moves and up-down moves cancel out.
	 */
	@Test
	public void testMovesFromCenterPosition() {
		// create a puzzle with 0 at center position
		PuzzleState start = createPuzzleState(puzzleCenterPosition.clone()) ;
		PuzzleState end ;
		// test all movements and check invariant
			end = this.moveBackAndForth(start, Operation.MOVEUP) ;
			assertTrue(start.isEqual(end));
			end = this.moveBackAndForth(start, Operation.MOVEDOWN) ;
			assertTrue(start.isEqual(end));
			end = this.moveBackAndForth(start, Operation.MOVELEFT) ;
			assertTrue(start.isEqual(end));
			end = this.moveBackAndForth(start, Operation.MOVERIGHT) ;
			assertTrue(start.isEqual(end));
	}
	/**
	 * Helper method that performs a move into a given direction in a circular way 
	 * such that the returned state equals the starting state if the movement is possible.
	 * The cycle is minimal and performs on a 2 x 2 area.
	 * Clockwise on a 2 x 2 puzzle works for gap on upper left corner.
	 * Counterclockwise on a 2 x 2 puzzle works for gap on lower right corner.
	 * @param start
	 * @param op the direction to move
	 * @return null if movement is not possible
	 */
	private PuzzleState performSmallCycle(PuzzleState start, boolean clockwise){
		PuzzleState result = start ;
		for (int i = 1 ; i <= 3 ; i++) {
			result = (clockwise) ? doFourStepsClockwise(result) : doFourStepsCounterClockwise(result) ;
		}
		return result ;
	}
	private PuzzleState doFourStepsCounterClockwise(PuzzleState result) {
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveUp() ;
		return result;
	}
	private PuzzleState doFourStepsClockwise(PuzzleState result) {
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveUp() ;
		return result;
	}
	
	/**
	 * Test if we move on a 2 x 2 puzzle in a circle, if we get back to the 
	 * initial state. We try both directions, clockwise and counterclockwise.
	 */
	@Test
	public void testCycleOn2x2Clockwise() {
		PuzzleState start = createPuzzleState(smallPuzzleUpperLeftCorner.clone()) ;
		PuzzleState goal = start ;
		
		PuzzleState result = performSmallCycle(start, true) ;
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
	}
	@Test
	public void testCycleOn2x2Counterclockwise() {
		PuzzleState start = createPuzzleState(smallPuzzleUpperRightCorner.clone()) ;
		PuzzleState goal = start ;
		
		PuzzleState result = performSmallCycle(start, false) ;
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
	}
	/**
	 * Performs a move into a given direction in a circular way 
	 * such that the returned state equals the starting state if the movement is possible.
	 * The cycle performs on a 3 x 3 area.
	 * Clockwise on a 3 x 3 puzzle works for gap on upper left corner.
	 * Counterclockwise on a 3 x 3 puzzle works for gap on lower right corner.
	 * Note that more turns are necessary to get back to the initial state since not
	 * only the gap moves around.
	 * @param start
	 * @param op the direction to move
	 * @return null if movement is not possible
	 */
	private PuzzleState performMediumCycle(PuzzleState start, boolean clockwise){
		PuzzleState result = start ;
		for (int i = 1 ; i <= 7 ; i++) {
			result = (clockwise) ? doEightStepsClockwise(result) : doEightStepsCounterClockwise(result) ;
		}
		return result ;
	}
	private PuzzleState doEightStepsCounterClockwise(PuzzleState result) {
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveUp() ;
		if(result!=null)
			result = result.moveUp() ;
		return result;
	}
	private PuzzleState doEightStepsClockwise(PuzzleState result) {
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveRight() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveDown() ;
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveLeft() ;
		if(result!=null)
			result = result.moveUp() ;
		if(result!=null)
			result = result.moveUp() ;
		return result;
	}
	@Test
	public void testCycleOn3x3Clockwise() {
		PuzzleState start = createPuzzleState(puzzleUpperLeftCorner.clone()) ;
		PuzzleState goal = start ;
		
		PuzzleState result = performMediumCycle(start, true) ;
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
	}
	@Test
	public void testCycleOn3x3Counterclockwise() {
		PuzzleState start = createPuzzleState(puzzleUpperRightCorner.clone()) ;
		PuzzleState goal = start ;
		
		PuzzleState result = performMediumCycle(start, false) ;
		assertTrue("Expected" + printPuzzle(goal.getStateArray()) + ", computed: " + printPuzzle(result.getStateArray()), 
					goal.isEqual(result));
	}
	///////////////////// tests for the solver ////////////////////////////
	/**
	 * This test case verifies that solvers configuration function, getSolverState and getSolverFinalState function.
	 * getSolverState must return the current state. Initially, before solving the puzzle, the initial 
	 * state and current state of solver must be same. getSolverState and getSolverFinalState must return 
	 * initial state and goal state respectively.
	 * This test case configure the solver with valid 2x2 initial and goal state.
	 */	
	@Test
	public void testSolverConfigure(){
		int[] puzzle1 = smallPuzzleUpperLeftCorner.clone();
		int[] puzzle2 = smallPuzzleLowerLeftCorner.clone();
	    
		PuzzleSolver pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle1, puzzle2);
		
    	PuzzleState psInitial = pSolver.getSolverInitialState();
    	PuzzleState psGoal = pSolver.getSolverFinalState();
    	
		PuzzleState pState1 = createPuzzleState(puzzle1) ;
	    PuzzleState pState2 = createPuzzleState(puzzle2) ;

	    assertTrue(pState1.isEqual(psInitial));
	    assertTrue(pState2.isEqual(psGoal));
	}
	/**
	 * This test case verifies that solver can find route from initial state to goal state.
	 * This test case configure the solver with valid 2x2 initial and goal state. The movesToSolve function 
	 * is called to verify the list moves the solver found to reach the goal state.
	 */	
	@Test
	public void testSolver2x2(){
		int[] puzzle1 = smallPuzzleUpperLeftCorner.clone();
		int[] puzzle2 = smallPuzzleLowerLeftCorner.clone();
	    
		PuzzleSolver pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle1, puzzle2);
		
	    PuzzleState goal = createPuzzleState(puzzle2) ;

	    checkThatComputedSolutionIsCorrect(pSolver, goal);
	}
	/**
	 * For a given, already configured solver, this method checks if the solver finds a solution and
	 * if that solution leads to the given goal
	 * @param pSolver
	 * @param goal
	 */
	private void checkThatComputedSolutionIsCorrect(PuzzleSolver pSolver, PuzzleState goal) {
		Operation[] moveList = pSolver.movesToSolve();
    	PuzzleState start = pSolver.getSolverInitialState();
		assertNotNull("Solver fails to find solution for starting state: " + 
				printPuzzle(start.getStateArray())  + " towards " + printPuzzle(goal.getStateArray()), 
				moveList) ;
    	PuzzleState result = performSequencesOfMoves(moveList, start);
 		assertTrue(goal.isEqual(result));
	}
	/**
	 * This test case verifies that solver can find route from initial state to goal state 
	 * and also traverse from goal state to initial state. This test case configure the solver with valid 2x2 initial 
	 * and goal state.
	 */
	@Test
	public void testSolverBothDirection2x2(){
		int[] puzzle1 = smallPuzzleUpperLeftCorner.clone();
		int[] puzzle2 = smallPuzzleLowerLeftCorner.clone();
	    
		PuzzleSolver pSolver ;
		PuzzleState goal ;

		// forward direction
		pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle1, puzzle2);
		goal = createPuzzleState(puzzle2);
		checkThatComputedSolutionIsCorrect(pSolver, goal) ;
    	
    	// search reverse path by switching start and goal
    	pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle2, puzzle1);
    	goal = createPuzzleState(puzzle1) ;
    	checkThatComputedSolutionIsCorrect(pSolver, goal) ;
	}

	/**
	 * Performs a sequences of moves starting from the given state and returns the resulting state.
	 * @param moveList
	 * @param pState
	 * @return resulting state
	 */
	private PuzzleState performSequencesOfMoves(Operation[] moveList, PuzzleState pState) {	
		for(int i=0;i<moveList.length;i++)
		{
			switch (moveList[i]){
				case MOVEUP:
					if(pState!=null)
						pState = pState.moveUp();
					break;
				case MOVEDOWN:
					if(pState!=null)
						pState = pState.moveDown();
					break;
				case MOVELEFT:
					if(pState!=null)
						pState = pState.moveLeft() ;
					break;
				case MOVERIGHT:
					if(pState!=null)
						pState = pState.moveRight();
					break;
				default:
					fail("List of moves contained an unknown type element: " + moveList[i] + " at position " + i) ;
					break;
			}
			if(pState==null)
			{
				fail("List of moves contained illegal move at position: " + i) ;
			}
		}
		return pState;
	}
	/**
	 * This test case verifies solvers capability of handling impossible cases. It configures solver with 2x2 puzzle.
	 * In the test case goal state is impossible to reach from initial puzzle state. movesToSolve function of solver 
	 * is called to verify the response. movesToSolve function must return null for puzzles that are impossible to solve.
	 */
	@Test
	public void testSolver2x2ImpossibleCase(){
		int[] puzzle1 = smallPuzzleUpperLeftCorner.clone();
		int[] puzzle2 = smallPuzzleLowerRightCorner.clone();
	    
		PuzzleSolver pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle1, puzzle2);
		
	    Operation[] moveList = pSolver.movesToSolve();
    	
    	assertNull(moveList);
	}

	/**
	 * Configure solver with automatically generated 2x2 initial and goal puzzle states.
	 * movesToSolve function is called to verify the moves from initial state to goal state.
	 * movesToSolve function must return a list of moves for this test case.
	 */
	@Test
	public void testSolver2x2AutoGeneratePuzzle(){
		int[] puzzle1 = smallPuzzleUpperLeftCorner.clone();
		int[] puzzle2 = smallPuzzleLowerRightCorner.clone();
		
		// get to randomly generated puzzles
		Random rand = new Random();
		performRandomSwitches(puzzle1, rand, 4);
		performRandomSwitches(puzzle2, rand, 4);
		
		// try to compute a path from puzzle1 to puzzle2
		PuzzleSolver pSolver = new SimplePuzzleSolver();
    	pSolver.configure(puzzle1, puzzle2);	
	    PuzzleState goal = createPuzzleState(puzzle2) ;
	    Operation[] moveList = pSolver.movesToSolve();
	    // only if a solution is found, we can check something
    	if(null!=moveList) {
    		PuzzleState start = pSolver.getSolverInitialState();
        	PuzzleState result = performSequencesOfMoves(moveList, start);
        	assertTrue(result.isEqual(goal));
    	}
	}
	/**
	 * Perform a number of random switches between values in the given puzzle
	 * @param puzzle
	 * @param rand stream of random numbers
	 * @param total number of switches
	 * @return
	 */
	private void performRandomSwitches(int[] puzzle, Random rand, int total) {
		int count = 0 ;
		int max = puzzle.length ;
		int a ;
		int b ;
		while(count != total)
		{
			a =	rand.nextInt(max);
			b =	rand.nextInt(max);
			
			if(a!=b)
			{
				switchPlacesInPuzzle(puzzle, a, b);
				count++;
			}
		}
	}
	/**
	 * Exchanges values at positions a and b within given array
	 * @param puzzle
	 * @param a
	 * @param b
	 */
	private void switchPlacesInPuzzle(int[] puzzle, int a, int b) {
		int temp = puzzle[a];
		puzzle[a] = puzzle[b];
		puzzle[b] = temp;
	}


}
