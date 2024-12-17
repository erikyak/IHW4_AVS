#include <iostream>
#include <fstream>
#include <omp.h>
#include <queue>
#include <random>
#include <sstream>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
// Task queue (forest sections)
std::queue<int> task_queue;

// Mutex for synchronizing access to the task queue
std::mutex task_mutex;

// Flags for termination and Winnie-the-Pooh detection
std::atomic<bool> winnie_found(false);

// Variable to store the section where Winnie-the-Pooh is located
int winnie_section;

// Variables to store the result
std::atomic<int> found_by_bee(-1);
std::atomic<int> found_in_section(-1);

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());

// Output file stream and flag for writing results to a file
std::ofstream output_file;
bool write_to_file = false;

// Function to process a task
void process_task(int bee_id, int task_id, std::ostringstream& log) {
    log << "Bee swarm " << bee_id << " is searching section " << task_id << '\n';

    // Random delay to simulate processing
    std::uniform_int_distribution<> sleep_dist(500, 1000); // Delay between 500 and 1000 ms
    int sleep_time = sleep_dist(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

    if (task_id == winnie_section) {
        if (!winnie_found.exchange(true)) { // Set the flag atomically
            found_by_bee = bee_id;
            found_in_section = task_id;
            log << "Bee swarm " << bee_id << " found and punished Winnie the Pooh in section " << task_id << '\n';
        }
    } else {
        log << "Bee swarm " << bee_id << " did not find Winnie the Pooh in section " << task_id << " and returned to the hive" << '\n';
    }
}

// Function to read input data from a file
void read_from_file(const std::string& filename, int& num_sections, int& num_bees, int& winnie_section) {
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

            std::uniform_int_distribution<> section_dist(1, num_sections);
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

    // Populate the task queue
    for (int i = 1; i <= num_sections; ++i) {
        task_queue.push(i);
    }

    // Parallel region with OpenMP
#pragma omp parallel num_threads(num_bees)
    {
        int bee_id = omp_get_thread_num() + 1;
        std::ostringstream log;

        while (!winnie_found) {
            int task_id = -1;

            // Retrieve a task from the queue
            {
                std::lock_guard<std::mutex> lock(task_mutex);
                if (!task_queue.empty()) {
                    task_id = task_queue.front();
                    task_queue.pop();
                }
            }

            if (task_id != -1) {
                process_task(bee_id, task_id, log);
            } else {
                break; // Exit if there are no more tasks
            }
        }

#pragma omp critical
        {
            std::cout << log.str();
            if (write_to_file) {
                output_file << log.str();
            }
        }
    }

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
