/*
 * This is the code for lab4a.c in Lab 4
 * Created by: Patrick Flanagan
 * Created on: 03/19/2020
 * Compilation command: Makefile
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

/* Thread routines */
void *thread1();
void *thread2();
void *thread3();
void *thread4();

pthread_t tid[4];					/* Thread IDs */

sem_t mutex;						/* Mutual Exclusion Semaphore */
sem_t S1, S2, S3, S4;					/* Synchronization Semaphores */

int Account[3] = {100000, 100000, 100000};		/* Shared integer array for 3 Accounts */

int main(int argc, char **argv) {
	/* Declarations */
	int i;						/* Loop Counter */
	int returnVal;					/* Intefer to check return values of function calls */

	/* Create all semaphores and threads */
	returnVal = sem_init(&S1, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S1\n");
		exit(0);
	}

	returnVal = sem_init(&S2, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S2\n");
		exit(0);
	} 

	returnVal = sem_init(&S3, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S3\n");
		exit(0);
	} 

	returnVal = sem_init(&S4, 0, 0);
	if (returnVal < 0) {
		printf("Error creating semaphore S4\n");
		exit(0);
	}

	returnVal = sem_init(&mutex, 0, 1);
	if (returnVal < 0) {
		printf("Error creating semaphore mutex\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[0], NULL, thread1, NULL);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, thread2, NULL);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, thread3, NULL);
	if (returnVal < 0) {
		printf("Error creating thread3\n");
		exit(0);
	}

	/* Wait until thread 2 terminates to terminate */
	for (i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}

exit;
}

/* Thread 1 Procedure */
void *thread1() {
	/* Declarations */
	int i;					/* Loop Counter */
	int internal_reg;			/* Internal Register for account transfers */
	int returnVal;				/* Return value from function calls */
	int acct1, acct2, acct3;		/* Values in the accounts at one specific moment */

	/* Create thread 4 */
	returnVal = pthread_create(&tid[3], NULL, thread4, NULL);
	if (returnVal < 0) {
		printf("Error creating thread 4\n");
		exit;
	}

	/* 4 Process 4 Way Synchronization */
	returnVal = sem_post(&S2);
	if (returnVal < 0) {
		printf("Error signalling semaphore S2\n");
		exit;
	}

	returnVal = sem_post(&S3);
	if (returnVal < 0) {
		printf("Error signalling semaphore S3\n");
		exit;
	}

	returnVal = sem_post(&S4);
	if (returnVal < 0) {
		printf("Error signalling semaphore S4\n");
		exit;
	}

	returnVal = sem_wait(&S1);
	if (returnVal < 0) {
		printf("Error waiting semaphore S1\n");
		exit;
	}

	returnVal = sem_wait(&S1);
	if (returnVal < 0) {
		printf("Error waiting semaphore S1\n");
		exit;
	}

	returnVal = sem_wait(&S1);
	if (returnVal < 0) {
		printf("Error waiting semaphore S1\n");
		exit;
	}

	/* Perform transaction */
	for (i = 0; i < 10000; i++) {
		/* Start Critical Section */
		returnVal = sem_wait(&mutex);
		if (returnVal < 0) {
			printf("Error waiting semaphore mutex\n");
			exit;
		}

		internal_reg = Account[0];
		internal_reg = internal_reg - 200;
		Account[0] = internal_reg;

		internal_reg = Account[1];
		internal_reg = internal_reg + 200;
		Account[1] = internal_reg;

		/* Read from the 3 Accounts */
		acct1 = Account[0];
		acct2 = Account[1];
		acct3 = Account[2];

		/* End of Critical Section */
		returnVal = sem_post(&mutex);
		if (returnVal < 0) {
			printf("Error signalling semaphore mutex\n");
			exit;
		}

		/* Print contents of each account every 1000 iterations */
		if (i > 0 && i % 1000 == 0) {
			printf("Thread No. %u: Account[1] = %d, Account[2] = %d, Account[3] = %d\n", pthread_self(), acct1, acct2, acct3);
			printf("Thread No. %u: Sum of Accounts: %d\n\n", pthread_self(), (acct1 + acct2 + acct3));
		}

		/* Sleep every 3000 iterations */
		if (i > 0 && (i % 3000 == 0)) {
			usleep(200000);
		}
	}
}

/* Thread 2 Procedure */
void *thread2() {
	/* Declarations */
	int i;					/* Loop Counter */
	int internal_reg;			/* Internal Register for account transfers */
	int returnVal;				/* Return value for function calls */
	int acct1, acct2, acct3;		/* Values in the accounts at one specific moment */

	/* 4 Process 4 Way Synchronization */
	returnVal = sem_post(&S1);
	if (returnVal < 0) {
		printf("Error signalling semaphore S1\n");
		exit;
	}

	returnVal = sem_post(&S3);
	if (returnVal < 0) {
		printf("Error signalling semaphore S3\n");
		exit;
	}

	returnVal = sem_post(&S4);
	if (returnVal < 0) {
		printf("Error signalling semaphore S4\n");
		exit;
	}

	returnVal = sem_wait(&S2);
	if (returnVal < 0) {
		printf("Error waiting semaphore S2\n");
		exit;
	}

	returnVal = sem_wait(&S2);
	if (returnVal < 0) {
		printf("Error waiting semaphore S2\n");
		exit;
	}

	returnVal = sem_wait(&S2);
	if (returnVal < 0) {
		printf("Error waiting semaphore S2\n");
		exit;
	}

	/* Perform transaction */
	for (i = 0; i < 10000; i++) {
		/* Start Critical Section */
		returnVal = sem_wait(&mutex);
		if (returnVal < 0) {
			printf("Error waiting semaphore mutex\n");
			exit;
		}

		internal_reg = Account[1];
		internal_reg = internal_reg - 200;
		Account[1] = internal_reg;

		internal_reg = Account[2];
		internal_reg = internal_reg + 200;
		Account[2] = internal_reg;

		/* Read from the 3 Accounts */
		acct1 = Account[0];
		acct2 = Account[1];
		acct3 = Account[2];

		/* End of Critical Section */
		returnVal = sem_post(&mutex);
		if (returnVal < 0) {
			printf("Error signalling semaphore mutex\n");
			exit;
		}

		/* Print contents of each account every 1000 iterations */
		if (i > 0 && i % 1000 == 0) {
			printf("Thread No. %u: Account[1] = %d, Account[2] = %d, Account[3] = %d\n", pthread_self(), acct1, acct2, acct3);
			printf("Thread No. %u: Sum of Accounts: %d\n\n", pthread_self(), (acct1 + acct2 + acct3));
		}

		/* Sleep after 2000, 4000, 6000, 9000 iterations */
		if (i > 0 && ((i % 2000 == 0) || (i % 4000 == 0) || (i % 6000 == 0) || (i % 9000 == 0))) {
			usleep(200000);
		}
	}
}

/* Thread 3 Procedure */
void *thread3() {
	/* Declarations */
	int i;					/* Loop Counter */
	int internal_reg;			/* Internal Register for account transfers */
	int returnVal;				/* Return value for function calls */
	int acct1, acct2, acct3;		/* Values in the accounts at one specific moment */

	/* 4 Process 4 Way Synchronization */
	returnVal = sem_post(&S1);
	if (returnVal < 0) {
		printf("Error signalling semaphore S1\n");
		exit;
	}

	returnVal = sem_post(&S2);
	if (returnVal < 0) {
		printf("Error signalling semaphore S2\n");
		exit;
	}

	returnVal = sem_post(&S4);
	if (returnVal < 0) {
		printf("Error signalling semaphore S4\n");
		exit;
	}

	returnVal = sem_wait(&S3);
	if (returnVal < 0) {
		printf("Error waiting semaphore S3\n");
		exit;
	}

	returnVal = sem_wait(&S3);
	if (returnVal < 0) {
		printf("Error waiting semaphore S3\n");
		exit;
	}

	returnVal = sem_wait(&S3);
	if (returnVal < 0) {
		printf("Error waiting semaphore S3\n");
		exit;
	}

	/* Perform transaction */
	for (i = 0; i < 10000; i++) {
		/* Start of Critical Section */
		returnVal = sem_wait(&mutex);
		if (returnVal < 0) {
			printf("Error waiting semaphore mutex\n");
			exit;
		}

		internal_reg = Account[2];
		internal_reg = internal_reg - 200;
		Account[2] = internal_reg;

		internal_reg = Account[0];
		internal_reg = internal_reg + 200;
		Account[0] = internal_reg;

		/* Read from the 3 Accounts */
		acct1 = Account[0];
		acct2 = Account[1];
		acct3 = Account[2];

		/* End of Critical Section */
		returnVal = sem_post(&mutex);
		if (returnVal < 0) {
			printf("Error signalling semaphore mutex\n");
			exit;
		}

		/* Print contents of each account every 1000 iterations */
		if (i > 0 && i % 1000 == 0) {
			printf("Thread No. %u: Account[1] = %d, Account[2] = %d, Account[3] = %d\n", pthread_self(), acct1, acct2, acct3);
			printf("Thread No. %u: Sum of Accounts: %d\n\n", pthread_self(), (acct1 + acct2 + acct3));
		}

		/* Sleep after 2000, 7500 iteration */
		if (i > 0 && ((i % 2000 == 0) || (i % 7500 == 0))) {
			usleep(300000);
		}
	}
}

/* Thread 4 Procedure */
void *thread4() {
	/* Declarations */
	int i;					/* Loop Counter */
	int acct1, acct2, acct3;		/* Values in the accounts at one specific moment */
	int returnVal;				/* Return value for function calls */
	int sumCorrect = 0;			/* Number of times sum of accounts is equal to 300000 */	

	/* 4 Process 4 Way Synchronization */
	returnVal = sem_post(&S1);
	if (returnVal < 0) {
		printf("Error signalling semaphore S1\n");
		exit;
	}

	returnVal = sem_post(&S2);
	if (returnVal < 0) {
		printf("Error signalling semaphore S2\n");
		exit;
	}

	returnVal = sem_post(&S3);
	if (returnVal < 0) {
		printf("Error signalling semaphore S3\n");
		exit;
	}

	returnVal = sem_wait(&S4);
	if (returnVal < 0) {
		printf("Error waiting semaphore S4\n");
		exit;
	}

	returnVal = sem_wait(&S4);
	if (returnVal < 0) {
		printf("Error waiting semaphore S4\n");
		exit;
	}

	returnVal = sem_wait(&S4);
	if (returnVal < 0) {
		printf("Error waiting semaphore S4\n");
		exit;
	}

	/* Loop */
	for (i = 0; i < 50000; i++) {
		/* Start Critical Section */
		returnVal = sem_wait(&mutex);
		if (returnVal < 0) {
			printf("Error waiting semaphore mutex\n");
			exit;
		}

		/* Check sum of accounts */
		if ((Account[0] + Account[1] + Account[2]) == 300000) {
			sumCorrect++;
		}

		/* End of Critical Section */
		returnVal = sem_post(&mutex);
		if (returnVal < 0) {
			printf("Error signalling semaphore mutex\n");
			exit;
		}

		/* Sleep every 10000 iterations */
		if (i > 0 && (i % 10000 == 0)) {
			usleep(50000);
		}
	}

	/* Display results */
	printf("------------------------------------------------\n");
	printf("Thread No. %u: Number of times sum checked: %d\n", pthread_self(), i);
	printf("Thread No. %u: Number of times sum correct: %d\n", pthread_self(), sumCorrect);
	printf("------------------------------------------------\n");

exit;
}

