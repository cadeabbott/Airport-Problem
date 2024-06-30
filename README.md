# Airport Pathfinding using Monte Carlo Algorithm

## Overview

This project aims to solve the problem of finding an optimal route for a pilot to land at 110 airports within a specified time frame using a Monte Carlo algorithm. The project reads a list of airport coordinates, calculates distances between them, and finds the best path based on user-defined parameters.

## Branch: monte_carlo

This branch focuses on the Monte Carlo algorithm for pathfinding. Other algorithms have been removed to streamline the focus on Monte Carlo.

## Features

- Reads a list of airport coordinates from a file.
- Calculates distances between each pair of airports using the Haversine formula.
- Uses a Monte Carlo algorithm to find the best path based on user-defined runtime, nearest airport probability, and number of threads.
- Saves the resulting path and total distance to `airport_list.txt`.

## Getting Started

### Prerequisites

- C++ Compiler (e.g., g++)
- Make

### Building the Project

1. Clone the repository and switch to the `monte_carlo` branch:

    ```sh
    git clone <repository-url>
    cd <repository-directory>
    git checkout monte_carlo
    ```

2. Build the project using `make`:

    ```sh
    make
    ```

### Running the Program

After building the project, run the executable to start the Monte Carlo search:

```sh
./airport_program
```

You will be prompted to enter the following parameters:
- Runtime for the Monte Carlo search (in seconds)
- Probability for selecting the nearest airport (0.0 to 1.0)
- Number of threads for parallel processing

### Example Usage

```sh
./airport_program
Starting to create the trimmed list of airports...
Trimmed list created successfully. Now calculating distances...
Distances calculated and saved successfully.
Enter runtime for Monte Carlo search (in seconds): 60
Enter probability for the nearest airport (0.0 to 1.0): 0.9
Enter number of threads for Monte Carlo search: 8
Starting Monte Carlo search with 8 threads...
<Progress updates will be displayed here>
Monte Carlo search completed. Best path saved to airport_list.txt
```

### Files

- `main.cpp`: Main program that initializes the Monte Carlo search.
- `monte_carlo.h` and `monte_carlo.cpp`: Implementation of the Monte Carlo algorithm.
- `trim_list.h` and `trim_list.cpp`: Functions to trim the list of airports.
- `calculate_distances.h` and `calculate_distances.cpp`: Functions to calculate distances between airports.
- `read_distances.h` and `read_distances.cpp`: Functions to read distances from a file.
- `common.h` and `common.cpp`: Common utilities, including the Haversine distance calculation.
- `Makefile`: Build script for compiling the project.

### Notes

- Ensure that the `airport_coordinates.txt` file is present in the root directory.
- The Monte Carlo algorithm is designed to be highly parallelizable. Adjust the number of threads based on your system's capabilities.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request if you have improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
