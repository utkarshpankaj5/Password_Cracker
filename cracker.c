#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <time.h>

#define MAX_LEN 8
// #define CHARSET "abcdefghijklmnopqrstuvwxyz"
#define CHARSET "abcdefghijklmnopqrstuvwxyz0123456789"
#define NUM_THREADS 4

char target_hash[65];
int max_password_length = 6;
int found = 0;
char found_password[MAX_LEN + 1];
unsigned long long attempts = 0;
int verbose = 0;

char wordlist_file[256] = "";
int use_wordlist = 0;

pthread_mutex_t lock;

typedef struct {
    int thread_id;
    int start_index;
    int end_index;
} ThreadData;

// SHA256 function
void sha256_string(const char *str, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)str, strlen(str), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = '\0';
}

// Recursive brute force
void brute_force_recursive(char *current, int position, int length, ThreadData *data) {
    if (found) return;

    if (position == length) {
        current[position] = '\0';

        char hashed[65];
        sha256_string(current, hashed);

        pthread_mutex_lock(&lock);
        attempts++;
        if (verbose) {
            printf("\r[Thread %d] Trying: %-10s | Attempts: %llu", data->thread_id, current, attempts);
            fflush(stdout);
        }
        pthread_mutex_unlock(&lock);

        if (strcmp(hashed, target_hash) == 0) {
            pthread_mutex_lock(&lock);
            found = 1;
            strcpy(found_password, current);
            pthread_mutex_unlock(&lock);
        }
        return;
    }

    for (int i = 0; i < strlen(CHARSET); i++) {
        if (found) return;
        current[position] = CHARSET[i];
        brute_force_recursive(current, position + 1, length, data);
    }
}

// Thread worker
void *thread_worker(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    char current[MAX_LEN + 1];

    for (int length = 1; length <= max_password_length; length++) {
        for (int i = data->start_index; i < data->end_index; i++) {
            if (found) pthread_exit(NULL);

            current[0] = CHARSET[i];

            if (length == 1) {
                current[1] = '\0';

                char hashed[65];
                sha256_string(current, hashed);

                pthread_mutex_lock(&lock);
                attempts++;
                if (verbose) {
                    printf("\r[Thread %d] Trying: %-10s | Attempts: %llu", data->thread_id, current, attempts);
                    fflush(stdout);
                }
                pthread_mutex_unlock(&lock);

                if (strcmp(hashed, target_hash) == 0) {
                    pthread_mutex_lock(&lock);
                    found = 1;
                    strcpy(found_password, current);
                    pthread_mutex_unlock(&lock);
                    pthread_exit(NULL);
                }
            } else {
                brute_force_recursive(current, 1, length, data);
            }
        }
    }

    pthread_exit(NULL);
}

// Wordlist attack
void crack_with_wordlist() {
    FILE *fp = fopen(wordlist_file, "r");
    if (!fp) {
        printf("Error: Cannot open wordlist file.\n");
        return;
    }

    char word[256];
    char hashed[65];

    while (fgets(word, sizeof(word), fp)) {
        if (found) break;

        word[strcspn(word, "\r\n")] = '\0';
        if (strlen(word) == 0) continue;

        sha256_string(word, hashed);

        pthread_mutex_lock(&lock);
        attempts++;
        if (verbose) {
            printf("\rTrying: %-20s | Attempts: %llu", word, attempts);
            fflush(stdout);
        }
        pthread_mutex_unlock(&lock);

        if (strcmp(hashed, target_hash) == 0) {
            pthread_mutex_lock(&lock);
            found = 1;
            strcpy(found_password, word);
            pthread_mutex_unlock(&lock);
            break;
        }
    }

    fclose(fp);
}

// MAIN
int main(int argc, char *argv[]) {

    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        printf("\n==== Password Cracker Simulator ====\n\n");
        printf("Usage: %s [OPTIONS]\n\n", argv[0]);
        printf("Options:\n");
        printf("  -h <hash>        Target SHA-256 hash\n");
        printf("  -l <max_len>     Max password length (default = 6)\n");
        printf("  -w <file>        Use wordlist file\n");
        printf("  -v               Verbose mode\n");
        printf("  --help           Show help\n\n");
        printf("Examples:\n");
        printf("  %s -h <hash>\n", argv[0]);
        printf("  %s -h <hash> -l 4 -v\n", argv[0]);
        printf("  %s -h <hash> -w words.txt\n\n", argv[0]);
        return 0;
    }

    // Parse args
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            strncpy(target_hash, argv[i + 1], 64);
            i++;
        }
        else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            max_password_length = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            strncpy(wordlist_file, argv[i + 1], sizeof(wordlist_file));
            use_wordlist = 1;
            i++;
        }
    }

    if (strlen(target_hash) == 0) {
        printf("Error: Hash required.\n");
        return 1;
    }

    pthread_mutex_init(&lock, NULL);
    clock_t start = clock();

    if (use_wordlist) {
        printf("[+] Wordlist Mode\n");
        crack_with_wordlist();
    } else {
        printf("[+] Brute Force Mode\n");

        pthread_t threads[NUM_THREADS];
        ThreadData data[NUM_THREADS];

        int len = strlen(CHARSET);
        int chunk = len / NUM_THREADS;
        int start_idx = 0;

        for (int i = 0; i < NUM_THREADS; i++) {
            data[i].thread_id = i + 1;
            data[i].start_index = start_idx;
            data[i].end_index = start_idx + chunk;
            start_idx = data[i].end_index;

            pthread_create(&threads[i], NULL, thread_worker, &data[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n\n==== RESULT ====\n");
    if (found) printf("Password Found: %s\n", found_password);
    else printf("Password Not Found\n");

    printf("Attempts: %llu\n", attempts);
    printf("Time: %.2f sec\n", time_taken);
    if (time_taken > 0)
        printf("Attempts/sec: %.2f\n", attempts / time_taken);

    pthread_mutex_destroy(&lock);
    return 0;
}