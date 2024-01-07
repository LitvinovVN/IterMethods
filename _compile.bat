echo "vector1d_tests.c compiling..."
gcc vector1d_tests.c -o vector1d_tests

echo "vector1d_files_tests.c compiling..."
gcc vector1d_files_tests.c -o vector1d_files_tests

echo "algorithms_inner_product_tests.cpp compiling..."
g++ algorithms_inner_product_tests.cpp -o algorithms_inner_product_tests -fopenmp