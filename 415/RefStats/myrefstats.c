#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <netdb.h>
#define FQDNLEN 128
#define IP_ADDRESS_LENGTH 16 //IPv4 addrs are 15 chars in size, save a spot for the terminating char

//-----------Buffer Data Structure and related functions-------------
//DATA STRUCT
struct buffer{
	char buffer_pointer[1000][IP_ADDRESS_LENGTH]; //buffer can be 1000 lines at most
	int numLines; //max size of the buffer
	int r_index, w_index;//keeps track of the read and write locations on the buffer
	//mutex and conditions
	pthread_mutex_t buffer_gate;
	pthread_cond_t not_empty; //signaled when buffer is not empty
	pthread_cond_t not_full; //signaled when buffer is not full
};

//FUNCTIONS
struct buffer *newBuffer(int numLines){
	struct buffer *buffer = (struct buffer *) calloc(sizeof(struct buffer),1);
	if(!buffer){
		perror("unable to create buffer");
		exit(-1);
	}
	//init buffer
	buffer->numLines = numLines;
	pthread_mutex_init(&buffer->buffer_gate, NULL);
	pthread_cond_init(&buffer->not_empty, NULL);
	pthread_cond_init(&buffer->not_full, NULL);
	buffer->r_index = 0;
	buffer->w_index = 0;
	return buffer;
}
//add information into the buffer 
void add_to_buffer(struct buffer *buffer, char *data){
	pthread_mutex_lock(&buffer->buffer_gate);
	if(buffer->numLines == 1){//one line buffer		
		while(buffer->w_index != 0)//must wait until writer thread takes info off buffer before writing to it again
			pthread_cond_wait(&buffer->not_full, &buffer->buffer_gate);
		//copy data into the buffer
		strcpy(buffer->buffer_pointer[buffer->w_index], data);
		buffer->w_index=1;//move the write index
		pthread_cond_signal(&buffer->not_empty);//signal that the buffer has information to be read 	
	}else{
		//wait until the buffer is not full modulo used hre to determine length of buffer
		while((buffer->w_index+1) % buffer->numLines == buffer->r_index)
			pthread_cond_wait(&buffer->not_full, &buffer->buffer_gate);
		//cpy data into buffer
		strcpy(buffer->buffer_pointer[buffer->w_index], data);
		buffer->w_index++;
		if(buffer->w_index >= buffer->numLines)
			buffer->w_index = 0;
		pthread_cond_signal(&buffer->not_empty);
	}
	pthread_mutex_unlock(&buffer->buffer_gate);
}

//take information off the buffer 
void fetch_from_buffer(struct buffer *buffer, char *data){
	pthread_mutex_lock(&buffer->buffer_gate);
	if(buffer->numLines == 1){//case for a one line buffer
		while(buffer->w_index == 0)
			pthread_cond_wait(&buffer->not_empty, &buffer->buffer_gate);
		//copy data from single line buffer
		strcpy(data, buffer->buffer_pointer[buffer->r_index]);
		buffer->w_index = 0;
		pthread_cond_signal(&buffer->not_full);
	}else{
		//wait until buffer is not empty
		while(buffer->w_index == buffer->r_index)
			pthread_cond_wait(&buffer->not_empty, &buffer->buffer_gate);
		//cpy data from buffer
		strcpy(data, buffer->buffer_pointer[buffer->r_index]);
		buffer->r_index++;
		if(buffer->r_index >= buffer->numLines)
			buffer->r_index = 0;
		pthread_cond_signal(&buffer->not_full);
	}
	pthread_mutex_unlock(&buffer->buffer_gate);
}


//-------------------------------------------------------------------
//------------Cache Data Structure and related functions-------------
//DATA STRUCTS
struct cache_node{
	//important data
	char ipAddr[IP_ADDRESS_LENGTH];
	char fqdn[FQDNLEN];
	struct cache_node *next;
};
struct cache{
	struct cache_node *head;
	//Hash? Weed? Ganja?
	/*
	note: a hash would be useful here to make cache operations o(1) instead of o(n) like
	a pure linked list, but since this project is more focused on multithreading...fuck it
	*/
	//struct cache_node **cache_array;
	//data concerning the cache
	int cacheSize;//max size for the cache
	int cache_len;//number of items currently in the cache
	pthread_mutex_t cache_lock;//mutex used to ensure exclusive access to the cache by a thread as needed. Call a cachelock before all cachefunctions
};
//FUNCTIONS
//cache is going to be a linked list
struct cache *newCache(int cacheSize){
	struct cache *cache = (struct cache *) calloc(sizeof(struct cache),1);
	if(!cache){
		perror("unable to create cache");
		exit(-1);
	}
	//cache init
	cache->head = NULL;
	cache->cacheSize = cacheSize;
	cache->cache_len = 0;
	pthread_mutex_init(&cache->cache_lock, NULL);//mutex guarding
	return cache;
}

//look through the cache. Return 1 on a hit, return 0 on a miss
int find_in_cache(struct cache *cache, char *ipAddr, char *fqdn)
{
	if(cache->head == NULL){
		return 0; //cache is empty
	}
	if(!strcmp(ipAddr, cache->head->ipAddr)){
		strcpy(fqdn, cache->head->fqdn);//load fqdn with what the cache has saved
		return 1; //hit on the head node
	}

	struct cache_node *current = cache->head->next;
	struct cache_node *prev = cache->head;

	//run through the linked list until we get a hit or reach the end
	//this is O(n) due to lack of hash
	while(current != NULL){
		if(!strcmp(ipAddr, current->ipAddr)){
			strcpy(fqdn, current->fqdn);
			//we have a hit. Rearrange the cache accordingly
			prev->next    = current->next;
			current->next = cache->head;
			cache->head   = current;
			return 1;
		}
		current = current->next;
		prev = prev->next;
	}
	//we missed entirely if we get here
	return 0;
}
//insert a new item at the front of the cache. 
void add_to_cache(struct cache *cache, char *ipAddr, char *fqdn){
	//init a new cache_node
	struct cache_node *temp_item = (struct cache_node*)malloc(sizeof(struct cache_node));
	strcpy(temp_item->ipAddr, ipAddr);
	strcpy(temp_item->fqdn, fqdn);
	temp_item->next = NULL;
	//check if the cache even is established yet
	if(cache->head == NULL){
		cache->head = temp_item;
		cache->cache_len++;
		return;
	}
	if(cache->cache_len >= cache->cacheSize){ //if the cache gets pushed to its max, delete the item at the end of the list
		//loop through to end of cache
		int i;
		struct cache_node *current = cache->head;
		for(i=0; i < (cache->cache_len)-1; i++)
			current = current->next;
		free(current->next);
		current->next=NULL;
		cache->cache_len--;
	}
	//insert a new cache item at the front if there are other items already there
	temp_item->next = cache->head;
	cache->head = temp_item;
	cache->cache_len++;
}




//------------------------------------------------------------------
//---------List Data structure and related functions----------------
//A linked list of the results of the lookup threads. This is our final output.
//The  output  should  be FQDNs (fqdns) in ascending lexicographic order. 
//This means the linked list should be built in the style of a priority queue.
//ipAddrs should only be used if the FQDN cannot be obtained
//DATA STRUCTS
struct list_item{
	char fqdn[FQDNLEN];
	int count;
	struct list_item *next;
};
struct list{
	struct list_item *head;
	pthread_mutex_t list_lock; //lock is necessary for the reader threads adding to the output
};

//FUNCTIONS
struct list *newList(){
	struct list *list = (struct list *) calloc(sizeof(struct list),1);
	if(!list){
		perror("unable to create output list");
		exit(-1);
	}
	list->head = NULL;
	pthread_mutex_init(&list->list_lock, NULL);
	return list;
}

void add_to_list(struct list *list, char *ipAddr, char *fqdn){
	//init a new list item 
	struct list_item *temp_item = (struct list_item*)malloc(sizeof(struct list_item));
	if(fqdn == NULL)
		strcpy(temp_item->fqdn, ipAddr);
	else
		strcpy(temp_item->fqdn, fqdn);
	temp_item->count=1;
	temp_item->next = NULL;

	if(list->head == NULL){
		list->head = temp_item;
		return;
	}else{
		struct list_item *temp = NULL;
		struct list_item *current = list->head;
		struct list_item *prev = list->head;
		while(current){
			if(strcmp(temp_item->fqdn, current->fqdn) == 0){
				(current->count)++;//increment count on a match
				break;
			}else if(strcmp(temp_item->fqdn, current->fqdn) < 0){
				if(current == list->head){//if we need to insert a new head
					temp = list->head;
					list->head = temp_item;
					temp_item->next = temp;
				}else{
					prev->next = temp_item;
					temp_item->next = current;
				}
				break;
			}else{
				prev = current;
				current = current->next;
			}

			if(current == NULL){//we hit the end of the list
				prev->next = temp_item;
			}
		}

	}

}

//print the results in a neat table
void printList(struct list *list){
	struct list_item *current = list->head;
	printf("\n\n||%-60s|\t%s\t||\n", "FQDN", "NUM LOG RECORDS");
	while(current){
		printf("||%-60s|\t%d\t\t||\n", current->fqdn, current->count);
		//consider adding another print statement here to emulate line dividers
		current = current->next;
	}
}
//------------------------------------------------------------------
//----------------------Main and Threads----------------------------
//shared global variables for the threads
struct buffer *buffer;
struct cache *cache;
struct list *list;
pthread_mutex_t thread_gate;
pthread_mutex_t hit_gate;
int num_files = 0;
float total_lines = 0;
float hit = 0;
int fileDelay = 0;
int threadDelay = 0;

//reader thread -- TODO: instead of passing it a single file name, pass a pointer to a char* array of filenames 
void *reader_thread(void *files){
	char **filenames = files;
	int i;
	for(i=0; i<num_files; i++){
		char *filename = filenames[i];
		FILE *current_file;
		char ip_address[IP_ADDRESS_LENGTH];
		struct timespec delay;
		delay.tv_sec = fileDelay;
		delay.tv_nsec = 0;

		current_file = fopen(filename, "r");
		if(current_file == NULL){
			perror("Error opening file" );
		}
		while(fscanf(current_file, "%s", ip_address) == 1){
			nanosleep(&delay, (struct timespec *)NULL);
		total_lines++;
		add_to_buffer(buffer, ip_address);
		}
		fclose(current_file);
	}
	add_to_buffer(buffer, "EOB");
	add_to_buffer(buffer, "EOB");
	return NULL;
}

//lookup thread...doesnt mess with any arguments
void *lookup_thread(void *arg){
	char ip_address[IP_ADDRESS_LENGTH];
	char fqdn[FQDNLEN];
	struct cache_node *temp_item;
	struct timespec delay;
	delay.tv_sec = threadDelay;
	delay.tv_nsec = 0;

	for(;;)
	{
		//clear everything and sleep as needed between buffer fetches
		memset(ip_address, '\0', IP_ADDRESS_LENGTH);
		memset(fqdn, '\0', FQDNLEN);
		nanosleep(&delay,(struct timespec *)NULL);
		//fetch a new ip_address from the buffer
		fetch_from_buffer(buffer, ip_address);
		//stop the presses if we reach the end of buffer string
		if(!strcmp(ip_address, "EOB")){
			break;
		}
		ip_address[strlen(ip_address)-1] = '\0';
		//check to see if the item is in the cache
		pthread_mutex_lock(&(cache->cache_lock));
		if(find_in_cache(cache, ip_address, fqdn)){
			//cache hit
			pthread_mutex_lock(&hit_gate);hit++;pthread_mutex_unlock(&hit_gate);
		}else{
			//cache miss -- go to internet with gethostbyaddr()
			struct hostent *host;
			struct in_addr addr;

			if(inet_aton(ip_address, &addr)==0)
				strcpy(fqdn, "fail");
			else{
				pthread_mutex_lock(&thread_gate);host = gethostbyaddr(&addr, sizeof(addr), AF_INET);pthread_mutex_unlock(&thread_gate);

				if(host==NULL)
					strcpy(fqdn, "fail");
				else
					strcpy(fqdn, host->h_name);
			}
			add_to_cache(cache, ip_address, fqdn);
		}
		pthread_mutex_unlock(&(cache->cache_lock));
		//add results of lookup to the list datastruct
		pthread_mutex_lock(&(list->list_lock));
		if(strcmp(fqdn, "fail")==0)	
			add_to_list(list, ip_address,NULL);	
		else
			add_to_list(list, ip_address, fqdn);
		pthread_mutex_unlock(&(list->list_lock));
	}
	return NULL;
}
//function used to determine if an entire string is a number or not. Saves from goddamn code reuse
int isInt(char string[]){
	int i, length = strlen(string);
	for(i = 0; i < length; i++)
		if(!isdigit(string[i]))
			return -1;
	return 0;

}

int main(int argc, char **argv)
{	
	//numLines and cacheSize set to baseline numbers so the program functions properly
	int numLines = 1, cacheSize = 1, arg;
	char *files[100];//program handles at most 100 appended files
	//parse args
	while((arg = getopt(argc, argv, "b:N:d:D:")) != -1) {
		switch (arg)
		{
			case 'b':
				if(isInt(optarg) == -1 || (atoi(optarg) < 1 || atoi(optarg) > 1000)){
					printf("usage: -b argument must be a positive integer between 1 and 1000\n");
					exit(1);
				}
				numLines = atoi(optarg);
				printf("numLines has been set to %d\n", numLines);
				break;
			case 'N':
				if(isInt(optarg) == -1 || (atoi(optarg) < 1 || atoi(optarg) > 10000)){
					printf("usage: -N argument must be a positive integer between 1 and 10000\n");
					exit(1);
				}
				cacheSize = atoi(optarg);
				printf("cacheSize has been set to %d\n", cacheSize);
				break;
			case 'd':
				if(isInt(optarg) == -1 || (atoi(optarg) < 0 )){
					printf("usage: -d argument must be either 0 or a positive integer\n");
					exit(1);
				}
				fileDelay = atoi(optarg);
				printf("fileDelay has been set to %d\n", fileDelay);
				break;
			case 'D':
				if(isInt(optarg) == -1 || (atoi(optarg) < 0)){
					printf("usage: -D argument must be either 0 or a positive integer\n");
					exit(1);
				}
				threadDelay = atoi(optarg);
				printf("threadDelay has been set to %d\n", threadDelay);
				break;
			case '?': 
				if (isprint (optopt))
				    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			    else
			        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			    exit(1);
			default: break;
		}
	}
	//cycle through leftEND args as filenames
	int index = optind;
	while(index!=argc){
		files[num_files] = argv[index];
		//maybe add fopen testing to see if file exists/can be opened
		num_files++;
		index++;
	}
	//threads	
	pthread_t reader, lookup1, lookup2;
	//init datastructs and mutex locks
	buffer = newBuffer(numLines);
	cache = newCache(cacheSize);
	list = newList();
	pthread_mutex_init(&thread_gate, NULL);
	pthread_mutex_init(&hit_gate, NULL);
	//create threads
	pthread_create (&reader, NULL, reader_thread, (void *)files);
	pthread_create(&lookup1, NULL, lookup_thread, (void*)NULL);
	pthread_create(&lookup2, NULL, lookup_thread, (void*)NULL);
	//wait for threads to finish
	pthread_join(reader, NULL);
	pthread_join(lookup1, NULL);
	pthread_join(lookup2, NULL);
	//print output and cache hit ratio
	printList(list);
	printf("\nCache Hit Caclulations:\n");
	printf("# of lines from files:  %.0f\n# of cache hits:  %.0f\ncache hit ratio:  %.5f\n", total_lines, hit, hit/total_lines);

	return 0;
}