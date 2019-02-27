#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FIFO 1
#define SJF  2
#define PR   3
#define RR   4

// PCB structure
typedef struct PCB_st {
	int ProcId;
	int ProcPR;
	int CPUBurst;
	int Reg[8];
	int queueEnterClock, waitingTime;
	struct PCB_st *next;
} PCB_st;

// Method Declarations/Global Variables
int fileRead(char *argv[ ], int flag);
int FIFO_Scheduling(PCB_st *PCB);
int SJF_Scheduling(PCB_st *PCB);
int total_waiting_time = 0, total_turnaround_time = 0, total_job = 0;
int CLOCK = 0;
int CPUReg[8] = {0};

// int main(int argc, char *argv[ ])
//
// Task:
// Parameters:
// Return:
int main(int argc, char *argv[ ]) {
	char *alg = argv[2];
	int flag;

	if (strcmp(argv[2], "FIFO") == 0) {
		if (argc != 5) {
			fprintf(stderr, "USAGE: prog -alg FIFO -input fileName\n");
			return -1;
		}

		flag = FIFO;
		fileRead(argv, flag);
	} else if (strcmp(argv[2], "SJF") == 0) {
		if (argc != 5) {
			fprintf(stderr, "USAGE: prog -alg FIFO -input fileName\n");
			return -1;
		}

		flag = SJF;
		fileRead(argv, flag);
	}

	fflush(stderr);
	return 0;
}

// int fileRead(char *argv[ ], int flag)
//
// Task:
// Parameters:
// Return:
int fileRead(char *argv[ ], int flag) {
	FILE *inputFile;
	struct PCB_st *head = NULL;
	struct PCB_st *tail = NULL;
	struct PCB_st *current = NULL;
	char buffer[100];
	int scan;

	// Open file for reading
	inputFile = fopen(argv[4], "r");

	// Loop through the file line by line and read in contents to a PCB_st
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		struct PCB_st *pcb;
		int i;
		int ProcId, ProcPR, CPUBurst;

		scan = sscanf(buffer, "%d %d %d", &ProcId, &ProcPR, &CPUBurst);
		if (scan != 3) {
			fprintf(stderr, "Line does not contain 2 digits\n");
			continue;
		}

		// Allocate the memory for the struct and assign variables
		pcb = malloc(sizeof(PCB_st));
		pcb->ProcId = ProcId;
		pcb->ProcPR = ProcPR;
		pcb->CPUBurst = CPUBurst;
		for (i = 0; i <= 8; i++) {
			pcb->Reg[i] = pcb->ProcId;
		}
		pcb->queueEnterClock = 0;
		pcb->waitingTime = 0;
		pcb->next = NULL;

		// Append struct onto linked list
		pcb->next = NULL;

		if (head == NULL) {
			head = pcb;
			tail = pcb;
		} else {
			current = head;
			while (current->next != NULL) {
				current = current->next;
			}

			current->next = pcb;
		}
	}

	// Close the file and print correct fields
	fclose(inputFile);

	// Switch case for flag raised on type of algorithm being called
	switch (flag) {
		case FIFO:
			printf("Student Name : Nicholas Simmons\n");
			printf("Input File Name : %s\n", argv[4]);
			printf("CPU Scheduling Alg : %s\n\n", argv[2]);
			FIFO_Scheduling(head);
			break;
		case SJF:
			printf("Student Name : Nicholas Simmons\n");
			printf("Input File Name : %s\n", argv[4]);
			printf("CPU Scheduling Alg : %s\n\n", argv[2]);
			SJF_Scheduling(head);
			break;
	}

	return 0;
}

// int FIFO_Scheduling(PCB_st *PCB)
//
// Task:		Will perform a logical first in first out CPU scheduling algorithm and measure
//				performance metrics related to the algorithm.
// Parameters:	PCB_st *PCB		PCB head node for iteration through the list
// Return:		int 			0 if successful
int FIFO_Scheduling(PCB_st *PCB) {
	struct PCB_st *temp = NULL;
	int PCB_num = 1;
	int i;

	// Loop through the list
	while (PCB != NULL) {

		// Perform context switching
		for (i = 0; i < 8; i++) {
			CPUReg[i] = PCB->Reg[i];
			CPUReg[i]++;
			PCB->Reg[i] = CPUReg[i];
		}

		// Performance metrics for time
		PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
		total_waiting_time = total_waiting_time + PCB->waitingTime;
		CLOCK = CLOCK + PCB->CPUBurst;
		total_turnaround_time = total_turnaround_time + CLOCK;
		total_job = total_job + 1;

		// Print process number and time to completion
		printf("Process %d is completed at %d ms\n", PCB_num, CLOCK);

		// Continue to next node and free the last node
		PCB_num++;
		temp = PCB->next;
		free(PCB);
		PCB = temp;
	}

	// Print average time metrics
	printf("\nAverage Waiting Time: %.2f per ms\n", (double)total_waiting_time/(double)total_job);
	printf("Average Turnaround Time: %.2f per ms\n", (double)total_turnaround_time/(double)total_job);
	printf("Throughput: %.2f per ms\n", (double)total_job/(double)CLOCK);

	return 0;
}

// int SJF_Scheduling(PCB_st *PCB)
//
// Task:
// Parameters:	PCB_st *PCB		PCB head node for iteration through the list
// Return:		int 			0 if successful
int SJF_Scheduling(PCB_st *PCB) {
	struct PCB_st *head = PCB;
	struct PCB_st *temp = NULL;
	struct PCB_st *prev = PCB;
	int PCB_num = 1, i;

	while (PCB != NULL) {
		int min = INT32_MAX;

		for (temp = head; temp != NULL; temp = temp->next) {
			if (temp->CPUBurst < min)
				min = temp->CPUBurst;
		}

		if (PCB->CPUBurst == min) {
			// Perform context switching
			for (i = 0; i < 8; i++) {
				CPUReg[i] = PCB->Reg[i];
				CPUReg[i]++;
				PCB->Reg[i] = CPUReg[i];
			}

			// Performance metrics for time
			PCB->waitingTime = PCB->waitingTime + CLOCK - PCB->queueEnterClock;
			total_waiting_time = total_waiting_time + PCB->waitingTime;
			CLOCK = CLOCK + PCB->CPUBurst;
			total_turnaround_time = total_turnaround_time + CLOCK;
			total_job = total_job + 1;

			// Print process number and time to completion
			printf("Process %d is completed at %d ms\n", PCB_num, CLOCK);

			PCB_num++;

			free(PCB);
			PCB = head;
		}

		PCB = PCB->next;
	}

	// Print average time metrics
	printf("\nAverage Waiting Time: %.2f per ms\n", (double)total_waiting_time/(double)total_job);
	printf("Average Turnaround Time: %.2f per ms\n", (double)total_turnaround_time/(double)total_job);
	printf("Throughput: %.2f per ms\n", (double)total_job/(double)CLOCK);

	return 0;
}
