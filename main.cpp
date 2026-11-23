
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::chrono;

// System configuration
constexpr int MAX_TRAINS = 100;
constexpr int CAPACITY = 500;
constexpr int BOOK_MIN = 5;
constexpr int BOOK_MAX = 10;
constexpr int MAX_THREADS = 20;
constexpr int MAX_ACTIVE_QUERIES = 5;
constexpr int SIMULATION_MINUTES = 1;

// Shared train data
int available_seats[MAX_TRAINS];

// Each train has its own reader-writer lock
shared_mutex train_mutex[MAX_TRAINS];

// Protects console output only
mutex print_mutex;

// Controls the maximum number of active queries
class QueryLimiter {
private:
    mutex mtx;
    condition_variable cv;
    int active_queries = 0;

public:
    void acquire() {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [this]() {
            return active_queries < MAX_ACTIVE_QUERIES;
        });

        active_queries++;
    }

    void release() {
        {
            lock_guard<mutex> lock(mtx);
            active_queries--;
        }

        cv.notify_one();
    }
};

QueryLimiter query_limiter;

// RAII: automatically releases a query slot
class QueryPermit {
private:
    QueryLimiter& limiter;

public:
    explicit QueryPermit(QueryLimiter& q) : limiter(q) {
        limiter.acquire();
    }

    ~QueryPermit() {
        limiter.release();
    }

    QueryPermit(const QueryPermit&) = delete;
    QueryPermit& operator=(const QueryPermit&) = delete;
};

// Query types
enum class QueryType {
    INQUIRY = 1,
    BOOKING = 2,
    CANCELLATION = 3
};

// Worker thread
void worker_thread(
    int thread_id,
    steady_clock::time_point deadline
) {
    // Independent random generator for every worker
    mt19937 rng(
        random_device{}() ^
        static_cast<unsigned>(
            hash<thread::id>{}(this_thread::get_id())
        )
    );

    uniform_int_distribution<int> sleep_dist(0, 9999);
    uniform_int_distribution<int> train_dist(0, MAX_TRAINS - 1);
    uniform_int_distribution<int> type_dist(1, 3);
    uniform_int_distribution<int> booking_dist(BOOK_MIN, BOOK_MAX);

    while (steady_clock::now() < deadline) {

        // Simulate delay between user requests
        auto delay = milliseconds(sleep_dist(rng));
        auto wake_time = steady_clock::now() + delay;

        if (wake_time >= deadline) {
            this_thread::sleep_until(deadline);
            break;
        }

        this_thread::sleep_until(wake_time);

        if (steady_clock::now() >= deadline) {
            break;
        }

        int train = train_dist(rng);

        QueryType type = static_cast<QueryType>(
            type_dist(rng)
        );

        // Acquire one of the five available query slots
        QueryPermit permit(query_limiter);

        if (steady_clock::now() >= deadline) {
            break;
        }

        ostringstream output;

        output << "\nThread " << thread_id << ": ";

        // INQUIRY
        if (type == QueryType::INQUIRY) {

            output << "Inquiring Train " << train << "\n";

            int seats;

            {
                // Multiple inquiries can access the same train
                shared_lock<shared_mutex> lock(train_mutex[train]);

                seats = available_seats[train];
            }

            output << "Thread " << thread_id
                   << ": " << seats
                   << " seats available in Train "
                   << train << "\n";
        }

        // BOOKING
        else if (type == QueryType::BOOKING) {

            output << "Booking seats in Train "
                   << train << "\n";

            {
                // Exclusive access while modifying seat availability
                unique_lock<shared_mutex> lock(train_mutex[train]);

                if (available_seats[train] >= BOOK_MIN) {

                    int num_booked = min(
                        booking_dist(rng),
                        available_seats[train]
                    );

                    output << "Thread " << thread_id
                           << ": booking " << num_booked
                           << " seats in Train " << train << "\n";

                    available_seats[train] -= num_booked;

                    output << "Thread " << thread_id
                           << ": successfully booked "
                           << num_booked << " seats in Train "
                           << train << "\n";
                }
                else {
                    output << "Thread " << thread_id
                           << ": insufficient seats available in Train "
                           << train << "\n";
                }
            }
        }

        // CANCELLATION
        else if (type == QueryType::CANCELLATION) {

            output << "Cancelling bookings in Train "
                   << train << "\n";

            {
                // Cancellation also requires exclusive access
                unique_lock<shared_mutex> lock(train_mutex[train]);

                int booked_seats =
                    CAPACITY - available_seats[train];

                if (booked_seats > 0) {

                    uniform_int_distribution<int>
                        cancel_dist(1, booked_seats);

                    int num_cancelled = cancel_dist(rng);

                    available_seats[train] += num_cancelled;

                    output << "Thread " << thread_id
                           << ": successfully cancelled "
                           << num_cancelled << " seats in Train "
                           << train << "\n";
                }
                else {
                    output << "Thread " << thread_id
                           << ": no bookings available to cancel in Train "
                           << train << "\n";
                }
            }
        }

        // Print the complete transaction atomically
        {
            lock_guard<mutex> lock(print_mutex);
            cout << output.str() << flush;
        }

    }
}

int main() {

    // Initialize every train
    for (int i = 0; i < MAX_TRAINS; i++) {
        available_seats[i] = CAPACITY;
    }

    cout << "========================================\n";
    cout << "   RAILWAY RESERVATION SYSTEM\n";
    cout << "========================================\n";

    cout << "Initializing " << MAX_TRAINS
         << " trains...\n";

    cout << "Creating " << MAX_THREADS
         << " worker threads...\n";

    cout << "Maximum active queries: "
         << MAX_ACTIVE_QUERIES << "\n";

    cout << "Simulation duration: "
         << SIMULATION_MINUTES << " minute(s)\n";

    // All threads share the same deadline
    auto deadline = steady_clock::now()
                  + minutes(SIMULATION_MINUTES);

    // Create worker threads
    thread threads[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i] = thread(
            worker_thread,
            i,
            deadline
        );
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i].join();
    }

    // Final reservation chart
    cout << "\n========================================\n";
    cout << "       FINAL RESERVATION CHART\n";
    cout << "========================================\n";

    cout << "Train Number\tAvailable Seats\n";

    for (int i = 0; i < MAX_TRAINS; i++) {

        // All worker threads have finished, so no lock is needed
        cout << i << "\t\t"
             << available_seats[i] << "\n";
    }

    cout << "========================================\n";
    cout << "Thanks for using our services!\n";

    return 0;
}
