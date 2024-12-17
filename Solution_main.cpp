#include <iostream>
#include <fstream>
#include <pthread.h>
#include <queue>
#include <random>
#include <thread>
#include <string>
#include <sstream>

// Mutex for synchronizing access to the task queue
pthread_mutex_t task_mutex;
// Condition variable for notifying threads about new tasks
pthread_cond_t task_cond;

// Task queue (forest sections)
std::queue<int> task_queue;

// Flags for termination and Winnie-the-Pooh detection
bool all_tasks_done = false; // Flag for indicating all tasks are done
bool winnie_found = false;  // Flag for indicating Winnie-the-Pooh has been found

// Variable to store the section where Winnie-the-Pooh is located
int winnie_section;

// Variables to store the result
int found_by_bee = -1;      // Bee swarm that found Winnie-the-Pooh
int found_in_section = -1;  // Section where Winnie-the-Pooh was found

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> section_dist;

// Output file stream and flag for writing results to a file
std::ofstream output_file;
bool write_to_file = false;

// Function executed by each thread
void* bee_task(void* arg) {
    int bee_id = *(int*)arg;
    delete (int*)arg;

    while (true) {
        pthread_mutex_lock(&task_mutex);

        // If Winnie-the-Pooh has been found, exit
        if (winnie_found) {
            pthread_mutex_unlock(&task_mutex);
            break;
        }

        // Wait for tasks if the queue is empty
        while (task_queue.empty() && !all_tasks_done && !winnie_found) {
            pthread_cond_wait(&task_cond, &task_mutex);
        }

        // Check if all tasks are done
        if (all_tasks_done && task_queue.empty() || winnie_found) {
            pthread_mutex_unlock(&task_mutex);
            break;
        }

        // Retrieve a task from the queue
        int task_id = task_queue.front();
        task_queue.pop();
        pthread_mutex_unlock(&task_mutex);

        // Process the task
        std::ostringstream log;
        log << "Bee swarm " << bee_id << " is searching section " << task_id << '\n';
        std::uniform_int_distribution<> sleep_dist(500, 1000); // Random delay between 500 and 1000 ms
        int sleep_time = sleep_dist(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

        if (task_id == winnie_section) {
            if (!winnie_found) {
                winnie_found = true;
                found_by_bee = bee_id;
                found_in_section = task_id;
                log << "Bee swarm " << bee_id << " found and punished Winnie the Pooh in section " << task_id << '\n';
            }
            pthread_cond_broadcast(&task_cond); // Notify all threads
            if (write_to_file) output_file << log.str();
            std::cout << log.str();
            break;
        } else {
            log << "Bee swarm " << bee_id << " did not find Winnie the Pooh in section " << task_id << " and returned to the hive" << '\n';
        }

        if (write_to_file) output_file << log.str();
        std::cout << log.str();
    }

    return nullptr;
}

// Function to read input data from a file
void read_from_file(const std::string& filename, int& num_sections, int& num_bees, int& winnie_section_file) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw std::runtime_error("Error opening file " + filename);
    }

    input_file >> num_sections >> num_bees >> winnie_section;
    if (input_file.fail()) {
        throw std::runtime_error("Invalid data format in file " + filename);
    }
}

int main(int argc, char* argv[]) {
    int num_sections = 0, num_bees = 0;
    std::string input_filename;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--file" && i + 1 < argc) {
                input_filename = argv[++i];
            } else if (arg == "--output" && i + 1 < argc) {
                output_file.open(argv[++i]);
                if (!output_file.is_open()) {
                    std::cerr << "Failed to open output file.\n";
                    return 1;
                }
                write_to_file = true;
            }
        }
    }

    if (!input_filename.empty()) {
        try {
            read_from_file(input_filename, num_sections, num_bees, winnie_section);
            std::cout << "Data read from file " << input_filename << ":\n";
            std::cout << "Forest size: " << num_sections << "\n";
            std::cout << "Number of bee swarms: " << num_bees << "\n";
            std::cout << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
            if (write_to_file) {
                output_file << "Data read from file " << input_filename << ":\n";
                output_file << "Forest size: " << num_sections << "\n";
                output_file << "Number of bee swarms: " << num_bees << "\n";
                output_file << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            return 1;
        }
    } else {
        std::cout << "Do you want to record results to a file? (1 = yes, 0 = no): ";
        int record_choice;
        std::cin >> record_choice;
        if (record_choice == 1) {
            std::string filename;
            std::cout << "Enter the output file name: ";
            std::cin >> filename;
            output_file.open(filename);
            if (!output_file.is_open()) {
                std::cerr << "Failed to open output file." << "\n";
                return 1;
            }
            write_to_file = true;
        }

        std::cout << "Choose input method: (1 = console, 2 = random, 3 = file): ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            do {
                std::cout << "What is the size of the forest? (enter a positive number): ";
                std::cin >> num_sections;
            } while (num_sections <= 0);

            do {
                std::cout << "How many bee swarms are there? (enter a positive number): ";
                std::cin >> num_bees;
            } while (num_bees <= 0);

            do {
                std::cout << "Where are you hiding? (choose a section between 1 and " << num_sections << "): ";
                std::cin >> winnie_section;
            } while (winnie_section < 1 || winnie_section > num_sections);

        } else if (choice == 2) {
            std::uniform_int_distribution<> random_sections(1, 10000); // Forest size between 1 and 10000
            std::uniform_int_distribution<> random_bees(1, 1000);      // Number of bee swarms between 1 and 1000

            num_sections = random_sections(gen);
            num_bees = random_bees(gen);

            section_dist = std::uniform_int_distribution<>(1, num_sections);
            winnie_section = section_dist(gen);

            std::cout << "Randomly generated inputs:\n";
            std::cout << "Forest size: " << num_sections << "\n";
            std::cout << "Number of bee swarms: " << num_bees << "\n";
            std::cout << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
            if (write_to_file) {
                output_file << "Randomly generated inputs:\n";
                output_file << "Forest size: " << num_sections << "\n";
                output_file << "Number of bee swarms: " << num_bees << "\n";
                output_file << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
            }
        } else if (choice == 3) {
            std::cout << "Enter the input file name: ";
            std::cin >> input_filename;
            try {
                read_from_file(input_filename, num_sections, num_bees, winnie_section);
                std::cout << "Data read from file " << input_filename << ":\n";
                std::cout << "Forest size: " << num_sections << "\n";
                std::cout << "Number of bee swarms: " << num_bees << "\n";
                std::cout << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
                if (write_to_file) {
                    output_file << "Data read from file " << input_filename << ":\n";
                    output_file << "Forest size: " << num_sections << "\n";
                    output_file << "Number of bee swarms: " << num_bees << "\n";
                    output_file << "Winnie the Pooh is hiding in section: " << winnie_section << "\n";
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << "\n";
                return 1;
            }
        } else {
            std::cerr << "Invalid choice." << "\n";
            return 1;
        }
    }

    // Initialize mutexes and condition variables
    pthread_mutex_init(&task_mutex, nullptr);
    pthread_cond_init(&task_cond, nullptr);

    // Populate the task queue
    for (int i = 1; i <= num_sections; ++i) {
        task_queue.push(i);
    }

    // Create threads for the bee swarms
    auto* threads = new pthread_t[num_bees];
    for (int i = 0; i < num_bees; ++i) {
        int* bee_id = new int(i + 1);
        pthread_create(&threads[i], nullptr, bee_task, bee_id);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_bees; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Release resources
    delete[] threads;
    pthread_mutex_destroy(&task_mutex);
    pthread_cond_destroy(&task_cond);

    // Final result log
    std::ostringstream final_log;
    final_log << "All bee swarms have returned to the hive.\n";
    if (found_by_bee != -1) {
        final_log << "Winnie the Pooh was found by bee swarm " << found_by_bee
                  << " in section " << found_in_section << ".\n";
    } else {
        final_log << "Winnie the Pooh was not found.\n";
    }

    std::cout << final_log.str();
    if (write_to_file) {
        output_file << final_log.str();
        output_file.close();
    }

    return 0;
}
