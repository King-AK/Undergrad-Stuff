#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_SHIPS 2
#define BOARD_SIZE 25

#define GAME_STATE_AWAY_QUIT -2
#define GAME_STATE_HOME_QUIT -1
#define GAME_STATE_PLAY       0
#define GAME_STATE_LOSE       1
#define GAME_STATE_WIN        2

/*
	Battleship client program. Connects to a well known server and 
	provides the front end for a game of Battleship by displaying 
	graphics and updates from the server to the user's terminal.
*/

void showBoards(char home_board[], char away_board[]);

int main(int argc, char *argv[])
{
	int query_mode = 0, t_mode = 0, password_mode = 0, arg;
	char *password;
	struct timeval timeout;
	timeout.tv_sec = -1;
	timeout.tv_usec = 0;
	//parse through argument list for set up
	while((arg = getopt(argc, argv, "qtp:")) != -1) 
		switch (arg)
			{
			case 'q': //query mode activated
				query_mode = 1; break;
			case 't': //timeout mode activated
				t_mode = 1; timeout.tv_sec = atoi(optarg); timeout.tv_usec =0; break;
			case 'p': //password mode activated
				password_mode = 1; password = optarg; break;
			case '?': //invalid option selected
				if (isprint (optopt))
       				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			     else
			       fprintf (stderr, "Unknown option character\n");
			     exit(-1);
   			default: break;
			}


	//connection stuff
	struct addrinfo hints, *res;
	struct sockaddr_in *server;
	int sockfd;
	char line1[25];//hostname
	char line2[25];//location for game requests
	char line3[25];//location for query requests

	//get the information on the server from the file. If it fails, wait 60 secs

	FILE *f;
	if((f = fopen("serverLocation.txt", "r")) == NULL){
		printf("serverLocation file missing. Please start the server before the client\n");
		sleep(60);
		if((f = fopen("serverLocation.txt", "r")) == NULL)
			exit(0);
	}

	//TODO: if file does not exist, wait 60 seconds, then retry to access the file. Obviously within those 60 seconds, the server must be activated
	//TODO: change below to if statement, so if the file can be opened scan in, otherwise SHUT IT DOWN
	fscanf(f, "%s", line1); fscanf(f, "%s", line2); fscanf(f, "%s", line3);
	fclose(f);
	printf("Game server location -> %s : %s\n", line1, line2);
	printf("Game query location  -> %s : %s\n", line1, line3);


	//connection set up for normal game
	memset(&hints, 0, sizeof(hints)); //check to see if this is necessary
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(line1, line2, &hints, &res);
	if(query_mode == 0){
		hints.ai_socktype = SOCK_STREAM; //trying to play a game
		getaddrinfo(line1, line2, &hints, &res);
		printf("Connecting to play a game...\n");
	}else{
		hints.ai_socktype = SOCK_DGRAM; //sending a query to the server
		getaddrinfo(line1, line3, &hints, &res);
		printf("Connecting to query the server...\n");
	}	
	server = (struct sockaddr_in *) res->ai_addr;

	if((sockfd = socket( res -> ai_family, res -> ai_socktype, 0 )) < 0){
		perror("client: socket");
		exit(1);
	}	

	if(connect(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr_in)) < 0){
		close(sockfd);
		perror("client: connect");
		exit(1);
	}

	printf("connected to the server...\n");
	//msg peek for a busy server. A server should immediately respond to its clients if not in game mode...
	
	if(query_mode == 1)
	{
		//ask server who's playing
		char query[] = "query";
		char query_code[] = "normal\0";
		char temp[11];
		sprintf(temp, "%s", password);
		char buf[100];
		sendto(sockfd, &query, sizeof(query), 0, (struct sockaddr *) server, sizeof(struct sockaddr_in));
		if(password_mode == 1){
			printf("using password : %s\n", temp);
			sendto(sockfd, &temp, strlen(temp)+1, 0, (struct sockaddr *) server, sizeof(struct sockaddr_in));
		}else
			sendto(sockfd, &query_code, strlen(query_code), 0, (struct sockaddr *) server, sizeof(struct sockaddr_in));
		recvfrom(sockfd, &buf, 100, 0, (struct sockaddr *) server, (socklen_t *) sizeof(struct sockaddr_in));
		printf("%s\n", buf);
		exit(1);
	}



	/*TODO: if query mode is activated, send a datagram to the server asking if there 
	is a game in progress, then end the program.*/

	/*TODO: if timeout mode is active, the program will end if no connection to 
	the server is made within the specified amount of time.
	*/
	

	

	/*TODO: check handle to make sure it is no longer than 10 characters*/
	//prompt user for handle


	//Game setup
	char home_board[BOARD_SIZE], away_board[BOARD_SIZE], msg[100], home_handle[10], away_handle[10];
	char away_temp_ship_spot[3], home_move[3], away_move[3]; 
	int home_ship_spots[MAX_SHIPS], away_board_ship_spots[MAX_SHIPS];
	int gameState = GAME_STATE_PLAY, i;
	int place, placedShips = 0, inputTest, temp_char;
	char temp[3];	

	//passcode matching
	char status[3];
	char query_code[11];
	char temp_code[11];

	sprintf(temp_code, "%s", password);
	//try to connect with password
	if(password_mode == 1){
		printf("using password : %s\n", temp_code);
		send(sockfd, &temp_code, strlen(temp_code)+1, 0);
	}else{
		snprintf(query_code, 10, "normal");
		send(sockfd, &query_code, strlen(query_code)+1, 0);
	}
	//if(timeout.tv_sec != NULL){//drop if we end up waiting too long
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		if(select(sockfd+1, &readfds, NULL, NULL, &timeout) <= 0){
			perror("time out");
			exit(-1);
		}
	//}
	//wait to see if connect was successful
	recv(sockfd, &status, sizeof(status), 0);
	if(strcmp(status, "no") == 0){
		printf("Wrong Password\n");
		exit(0);
	}
		



	//get user handle
	recv(sockfd, &msg, sizeof(msg), 0);
	printf("%s\n", msg);
	memset(&msg, 0, sizeof(msg));
	memset(home_handle, 0, sizeof(home_handle));
	printf("\nPlease note : Any handle longer than 10 chars will be truncated.\n\n");
	
	while(1){	
		printf("%s", msg);
		fgets(home_handle, 10, stdin);
		if(strlen(home_handle) <= 0 || strlen(home_handle) > 10){
			printf("Invalid Handle. Please Re-enter\n");
			do{
				temp_char = getchar();
			}while(temp_char != '\n' && temp_char!= EOF);
			continue;
		}else{
			char *tmp = strchr(home_handle, '\n');
			if(tmp) *tmp = '\0';
			printf("Your name is:  %s.\n", home_handle);
			send(sockfd, &home_handle, strlen(home_handle)+1, 0);
			break;
		}
	}

	//wait to recieve your opponents handle
	recv(sockfd, &away_handle, sizeof(away_handle), 0); //should say something like "your opponent is xxx"
	printf("You have been matched against: %s. Good Luck.\n", away_handle);

	//clear the boards
	for(i = 0; i < BOARD_SIZE; i++){
		home_board[i] = ' '; away_board[i] = ' ';
	}

	//get user ship locations
	recv(sockfd, &msg, sizeof(msg), 0); //should say something like "place your ships"
	i=0;
	while(placedShips != MAX_SHIPS){
			printf("%s", msg);
			inputTest = scanf("%d", &place);
			if(strcmp(temp, "Q") == 0 || strcmp(temp, "q") == 0){
				gameState = GAME_STATE_HOME_QUIT;
			}
			if(inputTest == 0){
				printf("Invalid Input. Press Enter and Select Another Space\n");
				do{
					temp_char = getchar();
				}while(temp_char != '\n' && temp_char!= EOF);
				continue;
			}
			if(place < 0 || place > BOARD_SIZE){
				printf("Invalid placement...\n");
				do{
					temp_char = getchar();
				}while(temp_char != '\n' && temp_char!= EOF);
				continue;
			}
			if(home_board[place] == 'S'){
				printf("Ship already placed here...\n");
				continue;
			}
			home_board[place] = 'S';
			home_ship_spots[i] = place; i++;
			placedShips++;
			sprintf(temp, "%d", place);
			send(sockfd, temp, strlen(temp)+1, 0);
	}
	memset(&msg, 0, sizeof(msg));
	
	//recieve opponent ship locations
	i=0;
	while(i != MAX_SHIPS){
		//clear the temp holder for safety
		memset(away_temp_ship_spot, 0 , sizeof(away_temp_ship_spot));
		//receive a ship location off of the socket
		recv(sockfd, &away_temp_ship_spot, sizeof(away_temp_ship_spot), 0);
		printf("loaded opponent ship %d...\n", i+1);
		if(strcmp(away_temp_ship_spot, "Q") == 0){
			printf("The enemy quit like a little bitch. You win, I guess.\n");
			exit(1);
		}
		//load it into the away board with a temp holder
		away_board_ship_spots[i] = atoi(away_temp_ship_spot);
		//printf("spot loaded locally on enemy board at spot %d\n", away_board_ship_spots[i]);	
		i++;
	}
	printf("All opponent ships have been loaded locally!\n");
	//final checks
	int home_ships_left = MAX_SHIPS;
	int away_ships_left = MAX_SHIPS;
	int turn = 0, hit = 0, first_move = 1;
	//Begin the game, displaying elements based on the state the game is in
	//define magic numbers up top
	showBoards(home_board, away_board);
	while(gameState == GAME_STATE_PLAY){	
		//wait to recieve a message stating it is your turn
		memset(msg, 0, sizeof(msg));
		recv(sockfd, &msg, sizeof(msg), 0);
		printf("server sent %s\n", msg);
		//printf("%s was sent from the server. msg[0] is %c \n", msg, msg[0]);
		if(strcmp(msg,"w") == 0){
			printf("It is the other players turn.\n");
			printf("Wait for their move.\n");
			//wait to receive the other persons move from the server
			recv(sockfd, &away_move, sizeof(away_move), 0);
			if(strcmp(away_move, "Q") == 0 || strcmp(away_move, "q") == 0){
				gameState = GAME_STATE_AWAY_QUIT;
				printf("The opponent quit or dropped their connection.\n");
				exit(1);
			}
			hit=0; i = 0;
			while(i != MAX_SHIPS){
				if(atoi(away_move) == home_ship_spots[i]){
					home_board[atoi(away_move)] = '*';
					home_ships_left--; hit = 1;
					printf("You've been hit!\n");
					break;
				}
				i++;
			}
			if(hit == 0){
				home_board[atoi(away_move)] = '@';
				printf("Phew! They missed!");
			}
			//display results of turn
			showBoards(home_board, away_board);
		}else if(strcmp(msg,"v") ==0){
			gameState == GAME_STATE_WIN;
			printf("You won. Good job murdering all those innocent seafarers, you monster.\n");
			break;
		}else if(strcmp (msg, "l") ==0){
			gameState == GAME_STATE_LOSE;
			printf("You lost. Thank the lord you aren't an actual battleship commander.\n");
			break;
		}else{			
			printf("Its your turn to attack. Choose a spot from 0-24. Make it count!\n");
			int legalmove = 0;
			while(legalmove == 0){
				//check if string is numeric or 'q' or 'Q'
				memset(home_move, 0, sizeof(home_move));
				int violations = 0;
				int inputTest = scanf("%2s", &home_move);
				if(inputTest < 1){
					printf("Invalid Choice. Choose Again..\n");
					continue;
				}
				if(strcmp(home_move, "Q") == 0 || strcmp(home_move, "q")==0){
					printf("You quit the game.\n");
					send(sockfd, &home_move, sizeof(home_move), 0);
					exit(1);
				}
				for(i = 0; i < strlen(home_move); i++)
					if(!isdigit(home_move[i])){
							violations++;
					}
				if(violations > 0){
					printf("Invalid Choice. Choose Again...\n");
					continue;
				}
				if(atoi(home_move) >= 0 && atoi(home_move) < BOARD_SIZE){
					if(away_board[atoi(home_move)] == ' '){
						legalmove = 1;
						break;
					}else{
						printf("You already attacked this spot.\n");
						continue;
					}
				}
				printf("Invalid Choice. Choose Again...\n");
				continue;

			}

			int move  = atoi(home_move);
			printf("you have chosen to attack spot %d\n", move);
			//check if you hit the enemy
			hit = 0; i = 0;
			while(i != MAX_SHIPS){
				if(move == away_board_ship_spots[i]){
					away_board[move] = '*';
					hit = 1; away_ships_left--;
					printf("You hit the enemy ship!\n");
					break;
				}
				i++;
			}
			if(hit == 0){
				away_board[move] = '@';
				printf("You missed, you moron.\n");
			}
			showBoards(home_board, away_board);
			send(sockfd, &home_move, sizeof(home_move), 0);
		}
	}

	return 0;
}

void showBoards(char home_board[], char away_board[])
{
	printf("\n  HOME\t\tSHOTS\n");
	int i, x = 0;
	for(i = 0; i < 11 ; i++)
		if(i % 2 == 0)
			printf("+-+-+-+-+-+\t+-+-+-+-+-+\n");
		else{
			printf("|%c|%c|%c|%c|%c|\t|%c|%c|%c|%c|%c|\n", home_board[x],home_board[x+1],home_board[x+2], home_board[x+3],home_board[x+4],away_board[x],away_board[x+1],away_board[x+2],away_board[x+3],away_board[x+4]);
			x += 5;
		}
}