#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LEN 8
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


// ============================================================
// SHA-256 FUNCTION
// ============================================================

void sha256_string(const char *str, char outputBuffer[65]) {

    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256((unsigned char*)str, strlen(str), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = '\0';
}


// ============================================================
// BRUTE FORCE RECURSIVE
// ============================================================

void brute_force_recursive(char *current,
                           int position,
                           int length,
                           ThreadData *data) {

    if (found)
        return;

    if (position == length) {

        current[position] = '\0';

        char hashed[65];

        sha256_string(current, hashed);

        pthread_mutex_lock(&lock);

        attempts++;

        if (verbose) {

            printf("\r[Thread %d] Trying: %-10s | Attempts: %llu",
                   data->thread_id,
                   current,
                   attempts);

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

        if (found)
            return;

        current[position] = CHARSET[i];

        brute_force_recursive(current,
                              position + 1,
                              length,
                              data);
    }
}


// ============================================================
// THREAD WORKER
// ============================================================

void *thread_worker(void *arg) {

    ThreadData *data = (ThreadData *)arg;

    char current[MAX_LEN + 1];

    for (int length = 1;
         length <= max_password_length;
         length++) {

        for (int i = data->start_index;
             i < data->end_index;
             i++) {

            if (found)
                pthread_exit(NULL);

            current[0] = CHARSET[i];

            if (length == 1) {

                current[1] = '\0';

                char hashed[65];

                sha256_string(current, hashed);

                pthread_mutex_lock(&lock);

                attempts++;

                if (verbose) {

                    printf("\r[Thread %d] Trying: %-10s | Attempts: %llu",
                           data->thread_id,
                           current,
                           attempts);

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

                brute_force_recursive(current,
                                      1,
                                      length,
                                      data);
            }
        }
    }

    pthread_exit(NULL);
}


// ============================================================
// WORDLIST ATTACK
// ============================================================

void crack_with_wordlist() {

    FILE *fp = fopen(wordlist_file, "r");

    if (!fp) {

        printf("Error: Cannot open wordlist file.\n");

        return;
    }

    char word[256];

    char hashed[65];

    while (fgets(word, sizeof(word), fp)) {

        if (found)
            break;

        word[strcspn(word, "\r\n")] = '\0';

        if (strlen(word) == 0)
            continue;

        sha256_string(word, hashed);

        pthread_mutex_lock(&lock);

        attempts++;

        if (verbose) {

            printf("\rTrying: %-20s | Attempts: %llu",
                   word,
                   attempts);

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


// ============================================================
// HASH TYPE IDENTIFIER
// ============================================================

void identify_hash() {

    char hash[256];

    printf("\n============================================\n");
    printf("             HASH TYPE ANALYZER\n");
    printf("============================================\n");

    printf("\nEnter hash: ");

    scanf("%255s", hash);

    int length = strlen(hash);

    int valid_hex = 1;

    for (int i = 0; i < length; i++) {

        char c = hash[i];

        if (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') ||
              (c >= 'A' && c <= 'F'))) {

            valid_hex = 0;

            break;
        }
    }

    printf("\nHash Length : %d characters\n",
           length);

    if (length == 64 && valid_hex) {

        printf("Format      : Hexadecimal\n");

        printf("Hash Type   : Probable SHA-256\n");

    } else {

        printf("Format      : Unknown / Invalid\n");

        printf("Hash Type   : Unknown\n");
    }

    printf("\n============================================\n");
}


// ============================================================
// CLEAR SCREEN
// ============================================================

void clear_screen() {

    system("clear");
}


// ============================================================
// MAIN MENU
// ============================================================

int show_main_menu() {

    int choice = 0;

    char key;

    while (1) {

        clear_screen();

        printf("\n============================================\n");
        printf("       PASSWORD SECURITY ANALYZER\n");
        printf("============================================\n\n");

        if (choice == 0) {

            printf(" > Crack Password\n");
            printf("   Analyze Hash\n");

        } else {

            printf("   Crack Password\n");
            printf(" > Analyze Hash\n");
        }

        printf("\nUse UP/DOWN arrow keys and press ENTER.\n");

        printf("\nYou can also use the command line directly\n");
        printf("to crack the password using the following options:\n\n");

        printf("  -h <hash>       Target SHA-256 hash\n");
        printf("  -l <max_len>    Maximum password length\n");
        printf("  -w <file>       Use wordlist file\n");
        printf("  -v              Enable verbose mode\n");

        printf("\nExample:\n");
        printf("  ./cracker -h <hash> -l 4 -v\n");

        printf("\n============================================\n");

        struct termios oldt, newt;

        tcgetattr(STDIN_FILENO, &oldt);

        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO,
                  TCSANOW,
                  &newt);

        key = getchar();

        if (key == 27) {

            key = getchar();

            if (key == '[') {

                key = getchar();

                // UP ARROW

                if (key == 'A') {

                    choice--;

                    if (choice < 0)
                        choice = 1;
                }

                // DOWN ARROW

                else if (key == 'B') {

                    choice++;

                    if (choice > 1)
                        choice = 0;
                }
            }

        } else if (key == '\n' || key == '\r') {

            tcsetattr(STDIN_FILENO,
                      TCSANOW,
                      &oldt);

            return choice;
        }

        tcsetattr(STDIN_FILENO,
                  TCSANOW,
                  &oldt);
    }
}


// ============================================================
// CRACKING MENU
// ============================================================

int show_cracking_menu() {

    int choice = 0;

    char key;

    while (1) {

        clear_screen();

        printf("\n============================================\n");
        printf("             PASSWORD CRACKER\n");
        printf("============================================\n\n");

        if (choice == 0) {

            printf(" > Brute Force Attack\n");
            printf("   Dictionary Attack\n");

        } else {

            printf("   Brute Force Attack\n");
            printf(" > Dictionary Attack\n");
        }

        printf("\nUse UP/DOWN arrow keys and press ENTER.\n");

        struct termios oldt, newt;

        tcgetattr(STDIN_FILENO, &oldt);

        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO,
                  TCSANOW,
                  &newt);

        key = getchar();

        if (key == 27) {

            key = getchar();

            if (key == '[') {

                key = getchar();

                if (key == 'A') {

                    choice--;

                    if (choice < 0)
                        choice = 1;

                } else if (key == 'B') {

                    choice++;

                    if (choice > 1)
                        choice = 0;
                }
            }

        } else if (key == '\n' || key == '\r') {

            tcsetattr(STDIN_FILENO,
                      TCSANOW,
                      &oldt);

            return choice;
        }

        tcsetattr(STDIN_FILENO,
                  TCSANOW,
                  &oldt);
    }
}


// ============================================================
// MAIN
// ============================================================

int main(int argc, char *argv[]) {


    // ========================================================
    // DIRECT COMMAND-LINE MODE
    // ========================================================

    if (argc > 1) {

        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-help") == 0) {

            printf("\n============================================\n");
            printf("       PASSWORD SECURITY ANALYZER\n");
            printf("============================================\n\n");

            printf("Usage:\n");
            printf("  ./cracker\n");
            printf("      Open interactive menu\n\n");

            printf("Command Line Options:\n\n");

            printf("  -h <hash>\n");
            printf("      Target SHA-256 hash\n\n");

            printf("  -l <max_len>\n");
            printf("      Maximum password length\n\n");

            printf("  -w <file>\n");
            printf("      Use a local wordlist file\n\n");

            printf("  -v\n");
            printf("      Enable verbose mode\n\n");

            printf("  -help / --help\n");
            printf("      Display this help section\n\n");

            printf("Examples:\n\n");

            printf("  ./cracker -h <hash>\n");

            printf("  ./cracker -h <hash> -l 4 -v\n");

            printf("  ./cracker -h <hash> -w words.txt\n");

            printf("\n============================================\n");

            return 0;
        }


        // ====================================================
        // EXISTING COMMAND-LINE ARGUMENT PARSING
        // ====================================================

        for (int i = 1; i < argc; i++) {

            if (strcmp(argv[i], "-h") == 0 &&
                i + 1 < argc) {

                strncpy(target_hash,
                        argv[i + 1],
                        64);

                target_hash[64] = '\0';

                i++;

            }

            else if (strcmp(argv[i], "-l") == 0 &&
                     i + 1 < argc) {

                max_password_length =
                    atoi(argv[i + 1]);

                i++;

            }

            else if (strcmp(argv[i], "-v") == 0) {

                verbose = 1;

            }

            else if (strcmp(argv[i], "-w") == 0 &&
                     i + 1 < argc) {

                strncpy(wordlist_file,
                        argv[i + 1],
                        sizeof(wordlist_file) - 1);

                wordlist_file[
                    sizeof(wordlist_file) - 1
                ] = '\0';

                use_wordlist = 1;

                i++;
            }
        }

    }

    // ========================================================
    // INTERACTIVE MODE
    // ========================================================

    else {

        int main_choice = show_main_menu();


        // ====================================================
        // CRACK PASSWORD
        // ====================================================

        if (main_choice == 0) {

            int crack_choice = show_cracking_menu();


            printf("\n============================================\n");
            printf("             PASSWORD CRACKER\n");
            printf("============================================\n");

            printf("\nEnter target SHA-256 hash: ");

            scanf("%64s", target_hash);


            // Dictionary selected

            if (crack_choice == 1) {

                printf("\nEnter wordlist file: ");

                scanf("%255s", wordlist_file);

                use_wordlist = 1;

            }

            // Brute force selected

            else {

                printf("\nEnter maximum password length ");
                printf("(default %d): ",
                       max_password_length);

                int input_length;

                if (scanf("%d", &input_length) == 1) {

                    if (input_length > 0 &&
                        input_length <= MAX_LEN) {

                        max_password_length =
                            input_length;
                    }
                }

                use_wordlist = 0;
            }
        }


        // ====================================================
        // ANALYZE HASH
        // ====================================================

        else {

            identify_hash();

            printf("\nPress ENTER to exit...");

            getchar();
            getchar();

            return 0;
        }
    }


    // ========================================================
    // VALIDATE HASH
    // ========================================================

    if (strlen(target_hash) == 0) {

        printf("Error: Hash required.\n");

        return 1;
    }


    // ========================================================
    // EXISTING CRACKING ENGINE
    // ========================================================

    pthread_mutex_init(&lock, NULL);

    clock_t start = clock();


    if (use_wordlist) {

        printf("[+] Wordlist Mode\n");

        crack_with_wordlist();

    }

    else {

        printf("[+] Brute Force Mode\n");


        pthread_t threads[NUM_THREADS];

        ThreadData data[NUM_THREADS];


        int len = strlen(CHARSET);

        int chunk = len / NUM_THREADS;

        int start_idx = 0;


        for (int i = 0;
             i < NUM_THREADS;
             i++) {

            data[i].thread_id = i + 1;

            data[i].start_index = start_idx;


            if (i == NUM_THREADS - 1)

                data[i].end_index = len;

            else

                data[i].end_index =
                    start_idx + chunk;


            start_idx =
                data[i].end_index;


            pthread_create(
                &threads[i],
                NULL,
                thread_worker,
                &data[i]
            );
        }


        for (int i = 0;
             i < NUM_THREADS;
             i++) {

            pthread_join(
                threads[i],
                NULL
            );
        }
    }


    // ========================================================
    // RESULT
    // ========================================================

    clock_t end = clock();

    double time_taken =
        (double)(end - start) /
        CLOCKS_PER_SEC;


    printf("\n\n==== RESULT ====\n");


    if (found)

        printf("Password Found: %s\n",
               found_password);

    else

        printf("Password Not Found\n");


    printf("Attempts: %llu\n",
           attempts);

    printf("Time: %.2f sec\n",
           time_taken);


    if (time_taken > 0)

        printf("Attempts/sec: %.2f\n",
               attempts / time_taken);


    pthread_mutex_destroy(&lock);

    return 0;
}
