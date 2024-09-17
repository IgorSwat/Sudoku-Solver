# Sudoku solver

A simple graphic C++ program for solving sudoku with user interface created with SFML library.

<img src="md/img1.png" alt="Opis" width="600"/>
<br>

## External resource
The following external resources (fonts and icons) were used:
+ <a href="https://www.flaticon.com/free-icons/undo" title="undo icons">Undo icons created by Freepik - Flaticon</a>
+ <a href="https://www.flaticon.com/free-icons/check" title="check icons">Check icons created by Google - Flaticon</a>
+ <a href="https://www.flaticon.com/free-icons/reset" title="reset icons">Reset icons created by inkubators - Flaticon</a>
+ <a href="https://www.flaticon.com/free-icons/sudoku" title="sudoku icons">Sudoku icons created by Freepik - Flaticon</a>
+ <a href="https://www.flaticon.com/free-icons/delete" title="delete icons">Delete icons created by Pixel perfect - Flaticon</a>
+ <a href="https://www.flaticon.com/free-icons/yes" title="yes icons">Yes icons created by hqrloveq - Flaticon</a>
+ <a href="https://www.dafont.com/coolvetica.font" title="coolvetica font">Coolvetica font created by Typodermic Fonts - dafont</a>

## Requirements
+ C++ 20
+ SFML 2.5.1 library
+ CMake >= 3.0

## Setup and usage
To build the solution from source, follow the steps below:
1. Download SFML 2.5.1 library (if you haven't done it yet) from <a href="https://www.sfml-dev.org/download/sfml/2.5.1/">here</a>.
2. Clone this repository to your local environment:
    ```
    git clone https://github.com/IgorSwat/Sudoku-Solver
    cd Sudoku-Solver
    ```
3. Build the solution with CMake. Remember to set DSFML_DIR variable pointing to the subdirectory of the downloaded SFML library 
containing the file **SFMLConfig.cmake** - usually it's <path_to_sfml>/lib/cmake/SFML.
Below is an example build for the Microsoft Visual Studio environment:
    ```
    mkdir build
    cd build
    cmake -DSFML_DIR=<path_to_SFMLConfig_directory> -G "Visual Studio 17 2022" ..
    ```
4. Compile the obtained project with the tool of your choice (for example, MSVC compiler for Visual Studio 2022).
It's highly recommended to build and compile in 'Release' mode.
5. Run the obtained executable file.
