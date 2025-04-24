# Lab work <mark>3</mark>: <mark>My vector, my array, smart pointers</mark>
Authors (team): Oleksandr Ivaniuk
## Prerequisites

```
sudo apt-get install libgtest-dev
```

### Compilation

```shell
mkdir build
cd build
cmake .. 
make -j4
```

### Usage
Tests:
```
./tests/vector-tests
./tests/array-tests
./tests/unique-ptr-tests
```
Also for time measurement:
```shell
./vector-array
```

### Results
![img.png](images/img.png)  
As we can see my vector is a little bit faster than std::vector. It probably because of fewer count of checks.

# Additional tasks
Comparison of my vector and std::vector.
Smart pointer (only unique ptr)

