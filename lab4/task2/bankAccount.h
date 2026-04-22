/* bankAccount.h simulates a simple bank account
 *
 * Joel Adams, Calvin College, Fall 2013.
 */


// Shared Variables
double bankAccountBalance = 0;

pthread_mutex_t balance_lock = PTHREAD_MUTEX_INITIALIZER;

// add amount to bankAccountBalance
void deposit(double amount) {
   pthread_mutex_lock(&balance_lock); //lock before critical section
   bankAccountBalance += amount;
   pthread_mutex_unlock(&balance_lock); //unlock after critical section
}

// subtract amount from bankAccountBalance
void withdraw(double amount) {
   pthread_mutex_lock(&balance_lock); //lock before critical section
   bankAccountBalance -= amount;
   pthread_mutex_unlock(&balance_lock); //unlock after critical section
}

void cleanup() {
   pthread_mutex_destroy(&balance_lock); //deallocate mutex
}

