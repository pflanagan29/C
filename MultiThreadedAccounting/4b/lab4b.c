/*
 * This is the code for lab4b.c in Lab 4
 * Created by: Patrick Flanagan
 * Created on: 03/24/2020
 * Compilation command: Makefile
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

/* Thread routines */
void *threadA();
void *threadB();
void *threadC();

/* Stucture for products produced by threadA */
struct ProductA {
	int valA;
	char a[5];
} bufferX[20], x, itemA;

/* Structure for products produced by threadB */
struct ProductB {
	int valB;
	char b[3];
} bufferY[40], y, itemB;

/* Shared variables */
pthread_t tid[3];		/* Thread IDs */
sem_t S1, S2;			/* Synchronization Semaphores */
sem_t bufferXEmpty;		/* Semaphore to determine whether bufferX is empty */
sem_t bufferYEmpty;		/* Semaphore to determine whether bufferY is empty */
sem_t buffersFull;		/* Semaphore to determine if buffers are full */
sem_t mutex;			/* Mutual Exclusion Semaphore */
int producer = 0;		/* Integer used to determine which producer buffer has a product */

int main(int argc, char **argv) {
	/* Declarations */
	int j;				/* Loop counter */
	int returnVal;			/* Return value from function calls */

	/* Initialize all semaphores and threads */
	returnVal = sem_init(&S1, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S1\n");
		exit;
	}

	returnVal = sem_init(&S2, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S2\n");
		exit;
	}

	returnVal = sem_init(&bufferXEmpty, 0, 20);
	if (returnVal < 0) {
		printf("Error creating semaphore bufferXEmpty\n");
		exit;
	}

	returnVal = sem_init(&bufferYEmpty, 0, 40);
	if (returnVal < 0) {
		printf("Error creating semaphore bufferYEmpty\n");
		exit;
	}

	returnVal = sem_init(&buffersFull, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore buffersFull\n");
		exit;
	} 

	returnVal = sem_init(&mutex, 0, 1);
	if (returnVal < 0) {
		printf("Error creating semaphore mutex\n");
	}

	returnVal = pthread_create(&tid[0], NULL, threadA, NULL);
	if (returnVal < 0) {
		printf("Error creating threadA\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, threadB, NULL);
	if (returnVal < 0) {
		printf("Error creating threadB\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, threadC, NULL);
	if (returnVal < 0) {
		printf("Error creating threadC\n");
		exit(0);
	}

	for (j = 0; j < 3; j++) {
		returnVal = pthread_join(tid[j], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", j + 1);
			exit;
		}
	}

exit;
}

/* First Producer */
void *threadA() {
	/* Declarations */
	int k;				/* Loop counter */
	int inA = 0;			/* Index of buffer */
	int returnVal;			/* Return value from fuction calls */

	/* Two Process Two Way Synchronization */
	returnVal = sem_post(&S1);
	if (returnVal < 0) {
		printf("Error signalling semaphore S1\n");
		exit;
	}

	returnVal = sem_wait(&S2);
	if (returnVal < 0) {
		printf("Error waiting semaphore S2\n");
		exit;
	}

	/* Produce Items */
	for (k = 0; k < 3000; k++) {

		if ((k + 1) % 2 == 0) {
			/* Item number is even */
			x.a[0] = 'A';
			x.a[1] = 'B';
			x.a[2] = 'C';
			x.a[3] = 'a';
			x.a[4] = 'a';
		} else {
			/* Item number is odd */
			x.a[0] = 'X';
			x.a[1] = 'Y';
			x.a[2] = 'Z';
			x.a[3] = 'x';
			x.a[4] = 'x';
		}

		x.valA = k + 1;

		returnVal = sem_wait(&bufferXEmpty);
		if (returnVal < 0) {
			printf("Error waiting semaphore bufferXEmpty\n");
			exit;
		}

		bufferX[inA] = x;

		/* Start of Critical Section */
		returnVal = sem_wait(&mutex);
		if (returnVal < 0) {
			printf("Error waiting semaphore mutex\n");
			exit;
		}

		producer++;

		/* End of Critical Section */
		returnVal = sem_post(&mutex);
		if (returnVal < 0) {
			printf("Error signalling semaphore mutex\n");
			exit;
		}

		returnVal = sem_post(&buffersFull);
		if (returnVal < 0) {
			printf("Error signalling semaphore buffersFull\n");
			exit;
		}

		inA = (inA + 1) % 20;

		/* Sleep every 500 items */
		if (k > 0 && (k % 500 == 0)) {
			usleep(100000);
		}
	}
exit;
}

/* Second Producer */
void *threadB() {
	/* Declarations */
	int i;				/* Loop counter */
	int inB = 0;			/* Index of buffer */
	int returnVal;			/* Return value from fuction calls */

	/* Two Process Two Way Synchronization */
	returnVal = sem_post(&S2);
	if (returnVal < 0) {
		printf("Error signalling semaphore S2\n");
		exit;
	}

	returnVal = sem_wait(&S1);
	if (returnVal < 0) {
		printf("Error waiting semaphore S1\n");
		exit;
	}

	/* Produce Items */
	for (i = 0; i < 4000; i++) {

		if (i < 2000) {
			y.b[0] = 'M';
			y.b[1] = 'M';
			y.b[2] = 'M';
		} else {
			y.b[0] = 'N';
			y.b[1] = 'N';
			y.b[2] = 'N';
		}

		y.valB = i + 1;

		returnVal = sem_wait(&bufferYEmpty);
		if (returnVal < 0) {
			printf("Error waiting semaphore bufferYEmpty\n");
			exit;
		}

		bufferY[inB] = y;

		returnVal = sem_post(&buffersFull);
		if (returnVal < 0) {
			printf("Error signalling semaphore buffersFull\n");
			exit;
		}

		inB = (inB + 1) % 40;

		/* Sleep every 1000 items */
		if (i > 0 && (i % 1000 == 0)) {
			usleep(200000);
		}
	}
exit;
}

/* Consumer */
void *threadC() {
	/* Declarations */
	int l = 0;			/* Loop Counter */
	int outA = 0, outB = 0;		/* Index of buffer */
	int val;			/* Integer from buffer item */
	char charsA[5], charsB[3];	/* Character array from buffer item */
	int returnVal;			/* Return value from fuction calls */

	/* Consume items */
	for (l = 0; l < 7000; l++) {
		returnVal = sem_wait(&buffersFull);
		if (returnVal < 0) {
			printf("Error waiting semaphore buffersFull\n");
			exit;
		}

		if (producer > 0) {
			/* Consume item from bufferX */
			itemA = bufferX[outA];
			outA = (outA + 1) % 20;

			/* Start of Critical Section */
			returnVal = sem_wait(&mutex);
			if (returnVal < 0) {
				printf("Error waiting semaphore mutex\n");
				exit;
			}

			producer--;

			/* End of Critical Section */
			returnVal = sem_post(&mutex);
			if (returnVal < 0) {
				printf("Error signalling semaphore mutex\n");
				exit;
			}

			/* Display contents of itemA */
			printf("%d%s\n", itemA.valA, itemA.a);

			returnVal = sem_post(&bufferXEmpty);
			if (returnVal < 0) {
				printf("Error signalling semaphore bufferXEmpty\n");
				exit;
			}
		} else {
			/* Consume item from bufferY */
			itemB = bufferY[outB];
			outB = (outB + 1) % 40;

			/* Display contents of itemB */
			printf("%d%s\n", itemB.valB, itemB.b);

			returnVal = sem_post(&bufferYEmpty);
			if (returnVal < 0) {
				printf("Error signalling semaphore bufferYEmpty\n");
				exit;
			}
		}

		/* Sleep every 2000 items */
		if (l > 0 && (l % 2000 == 0)) {
			usleep(300000);
		}
	}
exit;
}
