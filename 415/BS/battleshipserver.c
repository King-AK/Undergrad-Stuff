#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define MAX_SHIPS 2

#define GAME_STATE_AWAY_QUIT -2
#define GAME_STATE_HOME_QUIT -1
#define GAME_STATE_PLAY       0
#define GAME_STATE_LOSE       1
#define GAME_STATE_WIN        2
#define GAME_STATE_NO_SESSION_IN_PLAY 3
#define GAME_STATE_PLAYER_WAITING 4

#define PLAYER_1_TURN 0
#define PLAYER_2_TURN 1

#define NOT_CONNECTED -1

/*
	Battleship server
*/
int password_mode = 0;
char password[11];
void handler (){
		//delete the server location file
		int status = remove("serverLocation.txt");
		if(status == 0)
			printf("successfully removed serverLocation.txt");
		else{
			printf("unable to remove serverLocation.txt");
		}
		//give the users a clean exit, tell them the server is shut down or whatever if a game is in play
		exit(1);
}

void query_check (int queryfd, int gameState, char handle1[], char handle2[]){
	
	struct sockaddr_storage from; socklen_t fsize = sizeof(from); 
	struct hostent *hostp; int unlock = 0;
	int n; char buf[100]; char query_code[11];
	memset(buf, 0, sizeof(buf));
	memset(query_code, 0, sizeof(query_code));
	printf("in query check. queryfd is %d\n", queryfd);

	n = recvfrom(queryfd, buf, 100, MSG_PEEK, (struct sockaddr *) &from, &fsize);
	if(n < 0){
		perror("recvfrom:");
		exit(0);
	}
		
	printf("%s on the socket\n", buf);
	if(strlen(buf) > 0){
		//recieve a datagram "query"
		if((n = recvfrom(queryfd, buf, 100, 0, (struct sockaddr *) &from, &fsize)) < 0){
			perror("BS:recvfrom_query");
			exit(-1);
		}	
		memset(buf, 0, sizeof(buf));
		//recieve a datagram query_code
		if((n = recvfrom(queryfd, query_code, 11, 0, (struct sockaddr *) &from, &fsize)) < 0){
			perror("BS:recvfrom_query");
			exit(-1);
		}	

		//check if query code matches password settings for the server
		if(password_mode == 0)
			unlock = 1;
		if(password_mode == 1){
			printf("client attempted to connect with password: %s\n", query_code);
			if(strcmp(password, query_code) == 0)//unlock game message
				unlock = 1;
			else{//tell the queryer that they fucked with the wrong server
				sprintf(buf, "Wrong password.");
				if(n = sendto(queryfd, buf, 100, 0, (struct sockaddr *) &from, fsize) < 0){
					perror("BS:sendto_query");
					exit(-1);
				}
			}
		}
		if(unlock == 1){
			//craft message
			if(gameState == GAME_STATE_PLAY){
				sprintf(buf, "Game in session. Player1[%s] vs Player2[%s].", handle1, handle2);
			}else if(gameState == GAME_STATE_NO_SESSION_IN_PLAY){
				sprintf(buf, "No game in session. Nobody waiting to play.");
			}else if(gameState == GAME_STATE_PLAYER_WAITING){
				sprintf(buf, "No game in session. Player1[%s] is waiting to play.", handle1);
			}
			//send message to whoever asked
			if(n = sendto(queryfd, buf, 100, 0, (struct sockaddr *) &from, fsize) < 0){
				perror("BS:sendto_query");
				exit(-1);
			}
		}

	}

}
void game_mode_query(int query, fd_set readfds, int gameState, char p1Name[], char p2Name[], struct timeval tv ){
	FD_SET(query, &readfds);
	if(select(query+1, &readfds, NULL, NULL, &tv) > 0){
		if(FD_ISSET(query, &readfds)){
			query_check(query, gameState, p1Name, p2Name);
			printf("completed a query\n");
		}	
	}
	FD_SET(query, &readfds);

}
void checkPassword(){
	//checks if the password used by the client matches up with what the server wants
}


int main(int argc, char const *argv[])
{
	signal(SIGHUP, handler);
	int request, query, p1fd = NOT_CONNECTED, p2fd = NOT_CONNECTED;//fds
	struct sockaddr_in p1_addr, p2_addr; //client addresses
	socklen_t p1_addr_size, p2_addr_size, length;
	if(argc == 2){
		password_mode = 1;
		printf("The server will accept passwords that are at most 10 chars long.\nIt will truncate if the password is too long...\n");
		snprintf(password, 10, argv[1]);
		printf("the password is:   %s\n", password);
		printf("connections to this server will require a password...\n");
	}else{
		snprintf(password, 10, "normal");
	}

	//set up the server
	struct addrinfo hints, *res, q_hints, *q_res;
	struct sockaddr_in *server, *server_q;
	
	
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	char hostname[25];
	char p1Ships[MAX_SHIPS][3], p2Ships[MAX_SHIPS][3];
	char p1Move[3], p2Move[3];
	char p1Name[10], p2Name[10];
	char handReq[] = "Please select a handle between 1 and 10 chars:";
	char boardReq[] = "Place your ships (Select a spot 0-24):";
	char quitmsg[] = "Q";
	fd_set readfds;
	int gameState = GAME_STATE_NO_SESSION_IN_PLAY;


	//setup 4 sockets?
	memset(&hints, 0, sizeof(hints));
	memset(&q_hints, 0, sizeof(q_hints));
	
	hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
	q_hints.ai_family = AF_INET; q_hints.ai_socktype = SOCK_DGRAM;

	if(gethostname(hostname, sizeof(hostname)) != 0){
		perror("BS:gethostname:");
		exit(-1);
	}
	if(getaddrinfo(hostname, NULL, &hints, &res) != 0){
		perror("BS:getaddrinfo_request:");
		exit(-1);
	}
	if(getaddrinfo(hostname,NULL, &q_hints, &q_res) !=0){
		perror("BS:getaddrinfo_query");
		exit(-1);
	}

	server = (struct sockaddr_in *) res->ai_addr;
	server_q = (struct sockaddr_in *) res-> ai_addr+30;

	//create sockets
	if((request = socket(res->ai_family, res->ai_socktype, 0)) < 0){
		perror("BS:socket_server");
		exit(-1);
	}
	if((query = socket(q_res->ai_family, q_res->ai_socktype, 0)) <0){
		perror("BS:socket_query");
		exit(-1);
	}

	//bind sockets?
	if(bind(request, (struct sockaddr *) server, sizeof(struct sockaddr_in)) < 0){
		perror("BS:bind_server");
		exit(-1);
	}
	if(bind(query, (struct sockaddr *) server_q, sizeof(struct sockaddr_in)) < 0){
		perror("BS:bind_query");
		exit(-1);
	}

	//get address info for the server socket
	length = sizeof(struct sockaddr_in);
	if(getsockname(request, (struct sockaddr *) server, &length) < 0){
		perror("BS:getsockname_server");
		exit(-1);
	}
	if(getsockname(query, (struct sockaddr *) server_q, &length) < 0){
		perror("BS:getsockname_query");
		exit(-1);
	}

	//create a file for others to use to connect
	FILE *f = fopen("serverLocation.txt", "w");
	gethostname(hostname, sizeof(hostname));
	fprintf(f, "%s\n", hostname);
	fprintf(f, "%d\n", ntohs(server->sin_port));
	fprintf(f, "%d\n", ntohs(server_q->sin_port));
	fclose(f);
	FD_ZERO(&readfds);
	listen(request, 1);
	while(1){
		//peek at queries
		//take a look at both sockets to see if there is information to receive
		printf("server idle...\n");
		while(gameState != GAME_STATE_PLAY){
			FD_SET(request, &readfds);
			FD_SET(query, &readfds);	
			if(select(query+1, &readfds, NULL, NULL, NULL) < 0){
				if(errno == EINTR)
					continue;
				else
					perror("select:");
			}	
		//if the game has yet to begin...
				
			if(FD_ISSET(query, &readfds)){
				query_check(query, gameState, p1Name, p2Name);
				printf("completed a query\n");
				continue;
			}
					
			if(FD_ISSET(request, &readfds)){
				char access_code[11];
				memset(access_code, 0, sizeof(access_code));
				char ok[] = "ok";
				char no[] = "no";
				if(p1fd == NOT_CONNECTED){
					
					p1_addr_size  = sizeof(p1_addr);
					p1fd = accept(request, (struct sockaddr *)&p1_addr, &p1_addr_size);
					//recieve data access_code
					recv(p1fd, &access_code, sizeof(access_code),0);
					//check if code matches password
					if(strcmp(access_code, password) == 0){
						printf("user attempted connect with password : %s\n", access_code);
						send(p1fd, &ok, sizeof(ok), 0);
						printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , p1fd , inet_ntoa(p1_addr.sin_addr) , ntohs(p1_addr.sin_port));
						send(p1fd, &handReq, strlen(handReq), 0);
						recv(p1fd, &p1Name, sizeof(p1Name), 0);
						printf("Player 1 is %s\n", p1Name);
						gameState = GAME_STATE_PLAYER_WAITING;
					}else{
						printf("user attempted connect with password : %s\n", access_code);
						send(p1fd, &no, sizeof(no), 0);
						p1fd = NOT_CONNECTED;
					}
					continue;
					
				}else{
					p2_addr_size = sizeof(p2_addr);
					p2fd = accept(request, (struct sockaddr *)&p2_addr, &p2_addr_size);
					recv(p2fd, &access_code, sizeof(access_code),0);
					if(strcmp(access_code, password) == 0){
						printf("user attempted connect with password : %s\n", access_code);
						send(p2fd, &ok, sizeof(ok), 0);
						printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , p2fd , inet_ntoa(p2_addr.sin_addr) , ntohs(p2_addr.sin_port));
						send(p2fd, &handReq, strlen(handReq), 0);
						recv(p2fd, &p2Name, sizeof(p2Name), 0);
						printf("Player 2 is %s\n", p2Name);
						send(p1fd, &p2Name, strlen(p2Name), 0);
						send(p2fd, &p1Name, strlen(p1Name), 0);
						gameState = GAME_STATE_PLAY;
					}else{
						printf("user attempted connect with password : %s\n", access_code);
						send(p2fd, &no, sizeof(no), 0);
						p2fd = NOT_CONNECTED;
					}
				}
			}
			
		}
		while(gameState == GAME_STATE_PLAY){//game in session. Things can get messy here 

			//functions are magical and shit
			game_mode_query(query, readfds, gameState, p1Name, p2Name, tv );
			
			//begin the game and ask for ship positions
			int i = 0;
			//prompt each player to place their ships one at a time
			send(p1fd, &boardReq, strlen(boardReq), 0);
			send(p2fd, &boardReq, strlen(boardReq), 0);
			//recieve ships from P1 and P2
			while(i < MAX_SHIPS){
				memset(&p1Ships[i], 0, sizeof(p1Ships[i]));
				recv(p1fd, &p1Ships[i], sizeof(p1Ships[i]), 0);
				memset(&p2Ships[i], 0, sizeof(p2Ships[i]));
				recv(p2fd, &p2Ships[i], sizeof(p2Ships[i]), 0);
				printf("recieved p1ship at %s\n", p1Ships[i]);
				printf("recieved p2ship at %s\n", p2Ships[i]);
				i++;
			}
			game_mode_query(query, readfds, gameState, p1Name, p2Name, tv );

			//send ship positions to the other player once all ships have been collected
			for(i = 0; i < MAX_SHIPS; i++){
				send(p1fd, &p2Ships[i], sizeof(p2Ships[i]), 0);
				send(p2fd, &p1Ships[i], sizeof(p1Ships[i]), 0);
			}
			//start gameplay
			char active[] = "m";
			char waiting[] = "w";
			char victory[] = "v";
			char loss[] = "l";
			char disconnect[] = "q";

			int p1hits = 0, p2hits = 0;
			//tell player 1 it is their move. tell player 2 they must wait for player 1s move
			send(p1fd, &active, sizeof(active), 0);
			send(p2fd, &waiting, sizeof(waiting), 0);
			printf("sent turn messages to clients\n");
			while( gameState == GAME_STATE_PLAY){
				//printf("In gameplay mode\n");
				memset(p1Move, 0, sizeof(p1Move));
				memset(p2Move, 0, sizeof(p2Move));
				//check for disconnect
				recv(p1fd, &p1Move, sizeof(p1Move), MSG_PEEK);
				game_mode_query(query, readfds, gameState, p1Name, p2Name, tv );
				if(strlen(p1Move) <= 0){
					printf("p1 disconnected\n");
					send(p2fd, &disconnect, sizeof(disconnect), 0);
					break;
				}
				//full p1 turn 
				recv(p1fd, &p1Move, sizeof(p1Move), 0);
				printf("p1 has chosen to attack spot %s\n", p1Move);
				//check p1Move for a gamebreaker. Quit or Winning Move.
				if(strcmp(p1Move, "q") ==0 || strcmp(p1Move, "Q") == 0){
					printf("player 1 has quit the game.\n");
					send(p2fd, &p1Move, sizeof(p1Move), 0);
					break;
				}

				for(i = 0; i < MAX_SHIPS; i++)
					if(strcmp(p1Move, p2Ships[i]) == 0)
						p1hits++;
				if(p1hits == MAX_SHIPS){//announce wins
					printf("player 1 has won the game\n");
					send(p2fd, &p1Move, sizeof(p1Move), 0);
					send(p1fd, &victory, sizeof(victory), 0);
					send(p2fd, &loss, sizeof(loss), 0);
					break;
				}
				//if not a gamebreaker, send to P2 normally and continue game
				send(p2fd, &p1Move, sizeof(p1Move), 0);
				send(p1fd, &waiting, sizeof(waiting), 0);
				send(p2fd, &active, sizeof(active), 0);



				//check for disconnect
				recv(p2fd, &p2Move, sizeof(p2Move), MSG_PEEK);
				game_mode_query(query, readfds, gameState, p1Name, p2Name, tv );
				if(strlen(p2Move) <= 0){
					printf("p1 disconnected\n");
					send(p1fd, &disconnect, sizeof(disconnect), 0);
					break;
				}


				//full p2 turn
				recv(p2fd, &p2Move, sizeof(p2Move), 0);
				printf("p2 has chosen to attack spot %s\n", p2Move);
				//check p2Move for a gamebreaker. Quit or Winning Move.
				if(strcmp(p2Move, "q") ==0 || strcmp(p2Move, "Q") == 0){
					printf("player 2 has quit the game.\n");
					send(p1fd, &p2Move, sizeof(p2Move), 0);
					break;
				}
				for(i = 0; i < MAX_SHIPS; i++)
					if(strcmp(p2Move, p1Ships[i]) == 0)
						p2hits++;
				if(p2hits == MAX_SHIPS){//announce wins
					printf("player 2 has won the game\n");
					send(p1fd, &p2Move, sizeof(p2Move), 0);
					send(p2fd, &victory, sizeof(victory), 0);
					send(p1fd, &loss, sizeof(loss), 0);
					break;
				}
				//if not a gamebreaker, send to P1 normally and continue game
				send(p1fd, &p2Move, sizeof(p2Move), 0);
				send(p2fd, &waiting, sizeof(waiting), 0);
				send(p1fd, &active, sizeof(active), 0);

			}
			//game ended, blank all the game variables
			memset(&p1_addr_size, 0, sizeof(p1_addr_size));
			memset(&p2_addr_size, 0, sizeof(p2_addr_size));
			memset(&p1_addr, 0, sizeof(p1_addr));
			memset(&p2_addr, 0, sizeof(p2_addr));
			gameState = GAME_STATE_NO_SESSION_IN_PLAY;
			p1fd = NOT_CONNECTED;
			p2fd = NOT_CONNECTED;
		}
	}

	return 0;
}