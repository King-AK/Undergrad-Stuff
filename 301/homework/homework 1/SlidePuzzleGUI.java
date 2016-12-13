package edu.wm.cs.cs301.slidingpuzzle;
// SlidePuzzleGUI.java - GUI for SlidePuzzle
// Created By : Fred Swartz, 2003-May-10, 2004-May-3
// Modified By: Probir Roy, 2014-may-28

import java.awt.*;
import java.awt.event.*;
import java.util.Random;

import javax.swing.*;
import javax.swing.border.BevelBorder;

/**
 * This class contains all the parts of the GUI interface of the puzzle. The GUI contains three panel area representing
 * the initial, current and goal state of the puzzle. Panel representing current state evolve with move operations 
 * specified by the solver.
 * @author Probir
 *
 */
class SlidePuzzleGUI extends JPanel {
	// Graphics elements visible on the display
	private GraphicsPanel    initalPuzzleGraphics;  // panel on left hand side with starting state
	private GraphicsPanel    intermediatePuzzleGraphics; // middle panel with intermediate state
	private GraphicsPanel    finalPuzzleGraphics; // panel on right hand side with final/goal state
	private  JMenuBar menuBar; // menu bar on top with pull down menu to configure puzzle dimensions
	private int nPuzzle; // size of the puzzle, uses common terminology, e.g. 15 is for a 15 puzzle, which is 4 x 4
	private PuzzleSolver pSolver;
	private Operation[] moveList; // a sequence of steps from the initial state to the final state
	private boolean solved; // flag to indicate if a solution has been computed and moveList holds valid content
	private int stage; // the current position in moveList, used when a solution is show step by step
	private PuzzleState pState; // the current, intermediate state

	public static void main(String[] args) {
		final JFrame window = new JFrame("Sliding Puzzle");
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setContentPane(new SlidePuzzleGUI());
		window.pack();  // finalize layout
		window.setVisible(true);  
		window.setResizable(false);
	}
	/**
	 * Constructor creates graphic components and adds them to the JPanel this class inherits from.
	 */
	public SlidePuzzleGUI() {
		// default settings for internal attributes: unsolved 15 puzzle
		nPuzzle = 15;
		solved = false;

		// create graphics 
		// create menu bar with pull down menu for puzzle configuration
		// this needs a menu and a menu item in it
		menuBar = new JMenuBar();
		menuBar.setBorder(new BevelBorder(BevelBorder.RAISED));
		// create a menu and add it to the menu bar.
		JMenu menu = new JMenu("Menu");
		menu.setMnemonic(KeyEvent.VK_M);
		// create menu item
		JMenuItem eMenuItem = new JMenuItem("Configuration");
		eMenuItem.setMnemonic(KeyEvent.VK_C);
		eMenuItem.setToolTipText("Set Puzzle Configuration");
		eMenuItem.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent event) {
				createFrame();  //create configuration frame
			}
		});
		menu.add(eMenuItem);
		menuBar.add(menu);



		// create button to solve puzzle in a step by step manner
		// The listener is too lengthy for an anonymous class, 
		// code for the listener resides in internal NewGameAction class.
		JButton newGameButton = new JButton("Solve");
		ActionListener gameAction = new NewGameAction();
		newGameButton.addActionListener(gameAction);

		// create control panel that holds the solve button
		JPanel controlPanel = new JPanel();
		controlPanel.setLayout(new FlowLayout());
		controlPanel.add(newGameButton);

		controlPanel.add(redButton);
		redButton.addActionListener(gameAction);
		redButton.setBackground(Color.RED);
		redButton.setForeground(Color.RED);
		redButton.setOpaque(true);

		// create graphics panel
		initalPuzzleGraphics = new GraphicsPanel(nPuzzle, PanelType.INITIALPANEL);
		intermediatePuzzleGraphics = new GraphicsPanel(nPuzzle, PanelType.INTERMEDIATEPANEL);
		finalPuzzleGraphics = new GraphicsPanel(nPuzzle, PanelType.GOALPANEL);
		// create and configure a solver
		constructPuzzleSolver();
		// the panel uses a borderlayout
		// the menubar goes on top
		setLayout(new BorderLayout());
		add(menuBar, BorderLayout.NORTH);
		add(controlPanel, BorderLayout.SOUTH);
		add(initalPuzzleGraphics, BorderLayout.WEST);
		add(intermediatePuzzleGraphics, BorderLayout.CENTER);
		add(finalPuzzleGraphics, BorderLayout.EAST);
	}

	/**
	 * Configures the puzzle solver by creating initial and goal states from a list of predefined puzzles. It also 
	 * draws the GUI panels after creation of puzzle states
	 * @param nPuzzle represents the size of the puzzle
	 */
	public void setConfiguration(final int nPuzzle)
	{
		initalPuzzleGraphics.setConfiguration(nPuzzle, PanelType.INITIALPANEL);
		intermediatePuzzleGraphics.setConfiguration(nPuzzle, PanelType.INTERMEDIATEPANEL);
		finalPuzzleGraphics.setConfiguration(nPuzzle, PanelType.GOALPANEL);
		initalPuzzleGraphics.repaint();
		intermediatePuzzleGraphics.repaint();
		finalPuzzleGraphics.repaint();

		constructPuzzleSolver();
	}
	/**
	 * Create a solver instance and configures it with initial and goal state.
	 */
	private void constructPuzzleSolver() {
		pSolver = new SimplePuzzleSolver(); // this is the call to the actual implementation of PuzzleSolver
		pSolver.configure(initalPuzzleGraphics.getPuzzle(), finalPuzzleGraphics.getPuzzle());
	}
	/**
	 * create frame to handle the puzzle settings. For now only the puzzle size can be configured.
	 */
	public void createFrame()
	{
		EventQueue.invokeLater(new Runnable()
		{
			@Override
			public void run()
			{
				final JFrame frame = new JFrame("Configuration");
				frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
				try 
				{
					UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
				} catch (Exception e) {
					e.printStackTrace();
				}
				JPanel panel = new JPanel();
				panel.setLayout(new FlowLayout());
				panel.setOpaque(true);
				final JLabel label = new JLabel("Set the number of tiles of the puzzle: Currently supported tiles are 3,8,15");
				final JTextField input = new JTextField(20);

				JButton button = new JButton("Enter");

				button.addActionListener( new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						String text = input.getText();
						int dim = 0;
						try{
							dim = Integer.parseInt(text);
						}
						catch(NumberFormatException e5)
						{
							JOptionPane.showMessageDialog( new JFrame(), "Gets Integer Only");
						}
						dim = (int) Math.sqrt(dim+1);
						if(dim > 4)
						{
							JOptionPane.showMessageDialog(null,"Creating puzzle with maximum supported tiles", "Dimension not supported",JOptionPane.ERROR_MESSAGE);
							dim = 15;
						}
						else if(dim < 2)
						{
							JOptionPane.showMessageDialog(null,"Creating puzzle with minimum supported tiles", "Dimension not supported",JOptionPane.ERROR_MESSAGE);
							dim = 3;
						}
						else
						{
							dim = (dim * dim) -1;
						}
						setConfiguration(dim);
						frame.setVisible(false); //you can't see me!
						frame.dispose(); //Destroy the JFrame object

					}
				});
				panel.add(label);
				panel.add(input);
				panel.add(button);
				frame.getContentPane().add(BorderLayout.CENTER, panel);
				frame.pack();
				frame.setLocationByPlatform(true);
				frame.setVisible(true);
				frame.setResizable(false);
				input.requestFocus();
			}
		});
	}

	/**
	 * Internal class to represent the panels within GUI
	 * @author Probir
	 *
	 */
	class GraphicsPanel extends JPanel{
		private transient int rows;
		private transient int cols;
		private transient PanelType panelType;

		private transient int[] puzzleState;

		private static final int CELL_SIZE = 80; // Pixels
		private transient Font biggerFont;

		public GraphicsPanel(final int nPuzzle, final PanelType panelType) {
			setConfiguration(nPuzzle, panelType);
		}
		/**
		 * set configuration of the panels according to the puzzle state 
		 * @param nPuzzle gives the size of the puzzle, e.g. a 15 puzzle, possible values are 3, 8, 15
		 * @param panelType
		 */
		public void setConfiguration(final int nPuzzle, final PanelType panelType)
		{
			rows = (int) Math.sqrt(nPuzzle+1);
			cols = rows;
			this.panelType = panelType;

			puzzleState = getPuzzle();

			biggerFont = new Font("SansSerif", Font.BOLD, CELL_SIZE/2);
			setPreferredSize(new Dimension(CELL_SIZE * cols, CELL_SIZE*rows));
			setBackground(Color.black);

		}
		/**
		 * Set puzzle state of panel with sPuzzle
		 * @param sPuzzle puzzle state to set on panel
		 */
		private void setPuzzle(final int[] sPuzzle)
		{
			for(int i=0;i<rows*rows;i++)
			{
				puzzleState[i] = sPuzzle[i];
			}
		}
		/**
		 * Get new puzzle problem from a list of puzzles. It is possible to have solution of these puzzles.
		 * @return puzzle state in a single dimensional array
		 */
		private int[] getPuzzle() {
			int[] result ;
			switch (rows){
			case 2:
				if(PanelType.GOALPANEL!=panelType){
					int[] puzzle = {0,1,2,3} ;
					result = puzzle ;
				}
				else
				{
					int[] puzzle = {1,3,0,2};
					result = puzzle ;
				}
				break ;
			case 3:
				if(PanelType.GOALPANEL!=panelType){
					int[] puzzle = {2,8,3,1,6,4,7,0,5};
					result = puzzle ;
				}
				else
				{
					int[] puzzle = {0,2,3,1,8,6,7,5,4};
					result = puzzle ;
				}
				break ;
			case 4:
			default:
				if(PanelType.GOALPANEL!=panelType){
					int[] puzzle = {1, 2, 3, 4, 5, 0, 6, 8, 9, 10, 7, 12, 13, 14, 11, 15};
					result = puzzle ;
				}
				else
				{
					int[] puzzle = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
					result = puzzle ;
				}
				break ;
			}
			return result ;
		}

		/**
		 * Draw panel to display puzzle state change
		 */
		public void paintComponent(final Graphics g) {

			super.paintComponent(g);

			for (int r=0; r<rows; r++) {
				for (int c=0; c<cols; c++) {
					int x = c * CELL_SIZE;
					int y = r * CELL_SIZE;
					String text = getFace(r, c);
					if (text != null) {
						g.setColor(Color.gray);
						g.fillRect(x+2, y+2, CELL_SIZE-4, CELL_SIZE-4);
						g.setColor(Color.black);
						g.setFont(biggerFont);
						g.drawString(text, x+20, y+(3*CELL_SIZE)/4);
					}
				}
			}
		}

		/**
		 * Get block name (i.e. 1, 2 etc.). For block 0 it returns null
		 * @param r row
		 * @param c column
		 * @return string representing the block name
		 */
		private String getFace(final int r,final int c) {

			if(0!=puzzleState[r*rows+c])
			{
				return Integer.toString(puzzleState[r*rows+c]);
			}
			else
			{
				return null;
			}
		}

	}//end class GraphicsPanel

	/**
	 * Inner class to create solver and configure it to solve the puzzle on pressing solve button of GUI. It also 
	 * verifies the move operations to find goal state from initial state.
	 * @author Probir
	 *
	 */
	public class NewGameAction implements ActionListener {

		public void actionPerformed(ActionEvent e) {

			if (e.getSource() == redButton) {
				redButtonResponse();
			}
			else {
				if(!solved) {
					moveList = pSolver.movesToSolve();
					if(null==moveList) {
						JOptionPane.showMessageDialog(null,"Impossible Case","Impossible Case", JOptionPane.ERROR_MESSAGE);
						return;
					}
					solved = true;
					stage = 0;
					pState = pSolver.getSolverInitialState();
				}
				solveButtonResponse();
			}
		}

		private void solveButtonResponse() {
			switch (moveList[stage]) {
			case MOVEUP:
				pState = pState.moveUp();
				break;
			case MOVEDOWN:
				pState = pState.moveDown();
				break;
			case MOVELEFT:
				pState = pState.moveLeft();
				break;
			case MOVERIGHT:
				pState = pState.moveRight();
				break;
			default:
				break;
			}
			if (null!=pState)
			{
				intermediatePuzzleGraphics.setPuzzle(pState.getStateArray());
				intermediatePuzzleGraphics.repaint();

				stage++;
				if(stage==moveList.length)
				{
					pState.isEqual(pSolver.getSolverFinalState());
					solved = false;
					JOptionPane.showMessageDialog(null,"Solved","Solved", JOptionPane.INFORMATION_MESSAGE);
					JOptionPane.showMessageDialog(null,"Creating a New Puzzle","Puzzle", JOptionPane.INFORMATION_MESSAGE);
					setConfiguration(15);
				}
			}
			else
				JOptionPane.showMessageDialog(null,"Move Operation is not possible","Bad Move", JOptionPane.INFORMATION_MESSAGE);
		}

		private void redButtonResponse() {
			switch(count) {
			case 0 :
				System.out.println("Please do not press this button again!");
				System.out.println("The secret number to answer Q1 for HW1 is: "
						+ r.nextInt());
				count++;
				break;
			case 1:
				System.out.println("Sure, you are one of those guys who couldn't resist!");
				System.out.println("Thank you for making a simple program really happy, good bye!");
				count++;
				break;
			case 2:
				System.out.println("Still pressing the red button?");
				System.out.println("One more time and you will execute a call for /bin/rm -rf ~ in the background!");
				count++;
				break;
			default:
				System.out.println("You are really not easy to scare ... but isn't this getting boring after a while?");
				break;
			}
		}
	} //end inner class NewGameAction

	final static String BUTTON_LABEL3 = "RED BUTTON";
	JButton redButton = new JButton(BUTTON_LABEL3);
	Random r = new Random(12345);
	int count = 0;
}
