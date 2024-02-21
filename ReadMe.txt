basic TDA tools project
Building the file : in the project direction, type "cmake .", then "make"
You will find the binary files in ./bin directory, where you can run the executables
barcode_filtration : extracts barcode from a filtration
generate_filtration : generates filtration of either "moebius strip", "torus", "projective plane", "klein bottle", by gluing together a
    triangularized unit square(you can find the filtration in the "./resources/square_filtration.txt")

This code defines a function barcode_filtration that takes a vector of simplices as input
and returns a vector of barcodes. The barcodes represent the topological features that persist 
across the simplices in the filtration. The function computes the boundary matrix of the filtration,
 reduces it using Gaussian elimination, and extracts the barcodes from the reduced matrix.

The code includes several header files, such as cstdlib, string, iostream, sstream, fstream, vector, algorithm, set, and map.
It also includes a custom header file read_filtration.hpp and a source file generate_filtration.cpp.

The SparseMatrix struct represents a sparse matrix using an unordered map of unordered maps. 
The compute_boundary_matrix function computes the boundary matrix of the filtration using the definition of the boundary operator. 
The barcode struct represents a barcode using the birth and death times of a topological feature.

The barcode_filtration function first initializes some variables and computes the maximum dimension of the simplices. 
It then computes the boundary matrix of the filtration, computes the low vector of the boundary matrix, and reduces the boundary matrix 
using Gaussian elimination. Finally, it extracts the barcodes from the reduced matrix and returns them.

The main function is included for testing purposes. It reads a filtration from a file, 
computes the barcodes using the barcode_filtration function, and prints them to the console.

Filtration_B, is similarly to A also a 2-d sphere

Filtration_C, we see a prominent 1-d feature : the topological space is homotopic to a loop

Filtration_D, same thing as as C
