#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t lock1, lock2;

// Variável de controle de deadlock
int deadlock_detectado = 0; 

// Processo A
void* processA(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Processo A pegou lock1\n");
    sleep(1); // Simula algum processamento
    
    pthread_mutex_lock(&lock2);
    printf("Processo A pegou lock2\n");
    
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
}

// Processo B
void* processB(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Processo B pegou lock2\n");
    sleep(1); // Simula algum processamento

    pthread_mutex_lock(&lock1);
    printf("Processo B pegou lock1\n");
    
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
}

// Monitoramento para detecção de deadlock
void* detectar_deadlock(void* arg) {
    int tempo_espera_A = 0;
    int tempo_espera_B = 0;
    
    while (!deadlock_detectado) {
        sleep(1); // Monitora a cada 1 segundo
        tempo_espera_A++;
        tempo_espera_B++;
        
        // Se os processos esperarem por mais de 5 segundos, detecta deadlock
        if (tempo_espera_A > 5 || tempo_espera_B > 5) {
            printf("Deadlock detectado!\n");
            
            // Forçar liberação dos recursos
            pthread_mutex_unlock(&lock1);
            printf("Liberação forçada do lock1\n");
            pthread_mutex_unlock(&lock2);
            printf("Liberação forçada do lock2\n");
            deadlock_detectado = 1;
        }
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    // Threads do processoA e do processoB
    pthread_create(&t1, NULL, processA, NULL);
    pthread_create(&t2, NULL, processB, NULL);

    // Thread para rodar o detector de deadlocks
    pthread_create(&t3, NULL, detectar_deadlock, NULL);

    // Espera as threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    
    printf("Programa finalizou.");

    return 0;
}
