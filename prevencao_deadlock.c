#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Cria dois mutexes
pthread_mutex_t lock1, lock2;

// Processo A
void* processoA() {
	pthread_mutex_lock(&lock1);
	printf("Processo A pegou lock1\n");

	sleep(1); // Simula algum processamento

	pthread_mutex_lock(&lock2);
	printf("Processo A pegou lock2\n");

	// Liberar os dois mutexes
	pthread_mutex_unlock(&lock2);
	pthread_mutex_unlock(&lock1);
	printf("Processo A finalizado.\n\n");

}

// Processo B
void* processoB() {
	pthread_mutex_lock(&lock1);
	printf("Processo B pegou lock1\n");

	sleep(1); // Simula algum processamento

	pthread_mutex_lock(&lock2);
	printf("Processo B pegou lock2\n");

	// Liberar os dois mutexes
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
	printf("Processo B finalizado.\n\n");
}

int main() {
	pthread_t t1, t2;

	// Inicia os dois mutexes
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	// Cria duas threads distintas
	pthread_create(&t1, NULL, processoA, NULL);
	pthread_create(&t2, NULL, processoB, NULL);

	// Espera as duas threads acabarem
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	// Destroi os dois mutexes
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);

	printf("Programa finalizou.");

	return 0;
}
