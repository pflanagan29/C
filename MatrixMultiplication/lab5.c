/*
 * This is the code for lab5.c in Lab 5
 * Created by: Patrick Flanagan
 * Created on: 04/01/2020
 * Compilation command: Makefile
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

/* Matrix multiplication thread */
void *multiply();

/* Matrices */
int **A, **B, **C, **C1;

/* Arguments for Threads */
struct arg_struct {
	int argi;
	int argn;
	int argp;
	int argm;
};

int main(int argc, char **argv) {
/******************************* Declarations **************************************/
	int n, m, p;				/* The dimensions of the matrix */
	int i, j, k;				/* Loop counters to traverse matrix */
	pthread_t tid[6];			/* Thread IDs */
	int returnVal;				/* Return value for function calls */
	struct timeval start, end;		/* Used to record start and end times of calculation */
	time_t seconds;				/* Represents seconds */
	suseconds_t micro;			/* Represents microseconds */
	double elapsed;				/* Total time elapsed */
	struct arg_struct args1;		/* Arguments to be passed in to thread1 */
	struct arg_struct args2;		/* Arguments to be passed in to thread2 */
	struct arg_struct args3;		/* Arguments to be passed in to thread3 */
	struct arg_struct args4;		/* Arguments to be passed in to thread4 */
	struct arg_struct args5;		/* Arguments to be passed in to thread5 */
	struct arg_struct args6;		/* Arguments to be passed in to thread6 */
	

	/* Prompt and read in user input */
	printf("Enter n (<=6000), m (<=3000), p (<=1000): ");
	scanf("%d %d %d", &n, &m, &p);
	putchar('\n');

	/* Dynamically allocate space for matrices */
	A = (int**)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) {
		A[i] = (int*)malloc(m * sizeof(int));
	}

	B = (int**)malloc(m * sizeof(int*));
	for (i = 0; i < m; i++) {
		B[i] = (int*)malloc(p * sizeof(int));
	}

	C = (int**)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) {
		C[i] = (int*)malloc(p * sizeof(int));
	}

	C1 = (int**)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) {
		C1[i] = (int*)malloc(p * sizeof(int));
	}

	/* Initialize matrix A */
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			A[i][j] = i * j;
		}
	}

	/* Initalize matrix B */
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			B[i][j] = i + j;
		}
	}

	printf("Threads\t\tSeconds\t\tComparison\n");


/************************************* One Thread ******************************************/

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Perform matrix multiplication */
	for (i = 0; i < n; i++) {
		for (j = 0; j < p; j++) {
			C[i][j] = 0;
			for (k = 0; k < m; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	
	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Display results */
	printf("   1   \t\t %.2lf \t\t   N/A\n", elapsed);


/*********************************** Two Threads *********************************************/

	/* Prepare arguments for two thread solution */
	args1.argi = 0;
	args1.argn = n / 2;
	args1.argp = p;
	args1.argm = m;
	args2.argi = n / 2;
	args2.argn = n;
	args2.argp = p;
	args2.argm = m;

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Create threads */
	returnVal = pthread_create(&tid[0], NULL, multiply, (void*)&args1);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, multiply, (void*)&args2);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	/* Wait for both threads to finish */
	for (i = 0; i < 2; i++) {
		returnVal = pthread_join(tid[i], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", i + 1);
			exit;
		}
	}

	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Compare matrices */
	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			if (C[i][j] != C1[i][j]) {
				printf("Error: Matrices C and C1 are not equal\n");
				exit(0);
			}
		}
	}
	
	/* Display results */
	printf("   2   \t\t %.2lf \t\t No Error\n", elapsed);

	
/*********************************** Three Threads *********************************************/

	/* Prepare arguments for three thread solution */
	args1.argi = 0;
	args1.argn = n / 3;
	args1.argp = p;
	args1.argm = m;
	args2.argi = n / 3;
	args2.argn = (2 * n) / 3;
	args2.argp = p;
	args2.argm = m;
	args3.argi = (2 * n) / 3;
	args3.argn = n;
	args3.argp = p;
	args3.argm = m;

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Create threads */
	returnVal = pthread_create(&tid[0], NULL, multiply, (void*)&args1);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, multiply, (void*)&args2);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, multiply, (void*)&args3);
	if (returnVal < 0) {
		printf("Error creating thread3\n");
		exit(0);
	}

	/* Wait for both threads to finish */
	for (i = 0; i < 3; i++) {
		returnVal = pthread_join(tid[i], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", i + 1);
			exit;
		}
	}

	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Compare matrices */
	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			if (C[i][j] != C1[i][j]) {
				printf("Error: Matrices C and C1 are not equal\n");
				exit(0);
			}
		}
	}
	
	/* Display results */
	printf("   3   \t\t %.2lf \t\t No Error\n", elapsed);


/*********************************** Four Threads *********************************************/

	/* Prepare arguments for four thread solution */
	args1.argi = 0;
	args1.argn = n / 4;
	args1.argp = p;
	args1.argm = m;
	args2.argi = n / 4;
	args2.argn = n / 2;
	args2.argp = p;
	args2.argm = m;
	args3.argi = n / 2;
	args3.argn = (3 * n) / 4;
	args3.argp = p;
	args3.argm = m;
	args4.argi = (3 * n) / 4;
	args4.argn = n;
	args4.argp = p;
	args4.argm = m;

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Create threads */
	returnVal = pthread_create(&tid[0], NULL, multiply, (void*)&args1);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, multiply, (void*)&args2);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, multiply, (void*)&args3);
	if (returnVal < 0) {
		printf("Error creating thread3\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[3], NULL, multiply, (void*)&args4);
	if (returnVal < 0) {
		printf("Error creating thread4\n");
		exit(0);
	}

	/* Wait for both threads to finish */
	for (i = 0; i < 4; i++) {
		returnVal = pthread_join(tid[i], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", i + 1);
			exit;
		}
	}

	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Compare matrices */
	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			if (C[i][j] != C1[i][j]) {
				printf("Error: Matrices C and C1 are not equal\n");
				exit(0);
			}
		}
	}
	
	/* Display results */
	printf("   4   \t\t %.2lf \t\t No Error\n", elapsed);


/*********************************** Five Threads *********************************************/

	/* Prepare arguments for five thread solution */
	args1.argi = 0;
	args1.argn = n / 5;
	args1.argp = p;
	args1.argm = m;
	args2.argi = n / 5;
	args2.argn = (2 * n) / 5;
	args2.argp = p;
	args2.argm = m;
	args3.argi = (2 * n) / 5;
	args3.argn = (3 * n) / 5;
	args3.argp = p;
	args3.argm = m;
	args4.argi = (3 * n) / 5;
	args4.argn = (4 * n) / 5;
	args4.argp = p;
	args4.argm = m;
	args5.argi = (4 * n) / 5;
	args5.argn = n;
	args5.argp = p;
	args5.argm = m;

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Create threads */
	returnVal = pthread_create(&tid[0], NULL, multiply, (void*)&args1);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, multiply, (void*)&args2);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, multiply, (void*)&args3);
	if (returnVal < 0) {
		printf("Error creating thread3\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[3], NULL, multiply, (void*)&args4);
	if (returnVal < 0) {
		printf("Error creating thread4\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[4], NULL, multiply, (void*)&args5);
	if (returnVal < 0) {
		printf("Error creating thread5\n");
		exit(0);
	}

	/* Wait for both threads to finish */
	for (i = 0; i < 5; i++) {
		returnVal = pthread_join(tid[i], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", i + 1);
			exit;
		}
	}

	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Compare matrices */
	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			if (C[i][j] != C1[i][j]) {
				printf("Error: Matrices C and C1 are not equal\n");
				exit(0);
			}
		}
	}
	
	/* Display results */
	printf("   5   \t\t %.2lf \t\t No Error\n", elapsed);


/*********************************** Six Threads *********************************************/

	/* Prepare arguments for six thread solution */
	args1.argi = 0;
	args1.argn = n / 6;
	args1.argp = p;
	args1.argm = m;
	args2.argi = n / 6;
	args2.argn = n / 3;
	args2.argp = p;
	args2.argm = m;
	args3.argi = n / 3;
	args3.argn = n / 2;
	args3.argp = p;
	args3.argm = m;
	args4.argi = n / 2;
	args4.argn = (2 * n) / 3;
	args4.argp = p;
	args4.argm = m;
	args5.argi = (2 * n) / 3;
	args5.argn = (5 * n) / 6;
	args5.argp = p;
	args5.argm = m;
	args6.argi =(5 * n) / 6;
	args6.argn = n;
	args6.argp = p;
	args6.argm = m;

	/* Start time */
	returnVal = gettimeofday(&start, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing start time\n");
		exit;
	}

	/* Create threads */
	returnVal = pthread_create(&tid[0], NULL, multiply, (void*)&args1);
	if (returnVal < 0) {
		printf("Error creating thread1\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[1], NULL, multiply, (void*)&args2);
	if (returnVal < 0) {
		printf("Error creating thread2\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[2], NULL, multiply, (void*)&args3);
	if (returnVal < 0) {
		printf("Error creating thread3\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[3], NULL, multiply, (void*)&args4);
	if (returnVal < 0) {
		printf("Error creating thread4\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[4], NULL, multiply, (void*)&args5);
	if (returnVal < 0) {
		printf("Error creating thread5\n");
		exit(0);
	}

	returnVal = pthread_create(&tid[5], NULL, multiply, (void*)&args6);
	if (returnVal < 0) {
		printf("Error creating thread6\n");
		exit(0);
	}

	/* Wait for both threads to finish */
	for (i = 0; i < 6; i++) {
		returnVal = pthread_join(tid[i], NULL);
		if (returnVal < 0) {
			printf("Error joining thread%d\n", i + 1);
			exit;
		}
	}

	/* End time */
	returnVal = gettimeofday(&end, NULL);
	if (returnVal < 0) {
		printf("Error retrieveing end time\n");
		exit;
	}

	/* Calculate time */
	seconds = end.tv_sec - start.tv_sec;
	micro = end.tv_usec - start.tv_usec;
	if (micro < 0) {
		micro *= -1;
	}

	elapsed = (seconds * 1000000) + micro;
	elapsed /= 1000000;

	/* Compare matrices */
	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			if (C[i][j] != C1[i][j]) {
				printf("Error: Matrices C and C1 are not equal\n");
				exit(0);
			}
		}
	}
	
	/* Display results */
	printf("   6   \t\t %.2lf \t\t No Error\n", elapsed);
	putchar('\n');

return 0;
}

void *multiply(void *arguments) {
/******************************* Declarations **************************************/
	struct arg_struct *args = arguments;		/* Passed in arguments */
	int i, j, k;					/* Loop Counters */
	int n = args->argn;				/* End of outer loop */
	int p = args->argp;				/* End of middle loop */
	int m = args->argm;				/* End of inner loop */

	/* Perform matrix multiplication */
	for (i = args->argi; i < n; i++) {
		for (j = 0; j < p; j++) {
			C1[i][j] = 0;
			for (k = 0; k < m; k++) {
				C1[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	
}

