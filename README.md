# intel_writing_test
implement a small compute graph which consists of a max_pooling op and an element-wise add (support broadcast) op in C/C++
# functions
```c++
vector<vector<vector<vector<int>>>> inputTensor()
```
Manual input and return a 4D tensor of `(b, c, h, w)`
```c++
vector<vector<vector<vector<int>>>> randomTensor()
```
Randomly generate and return a 4D tensor of `(b, c, h, w)`
```c++
void printTensor(vector<vector<vector<vector<int>>>> output)
```
Print a 4D tensor
```c++
void printSize(vector<vector<vector<vector<int>>>> tensor)
```
Print the size of a 4D tensor
```c++
vector<vector<vector<vector<int>>>> maxpooling(vector<vector<vector<vector<int>>>> tensor1)
```
Receive a 4D tensor of `(b, c, h, w)` and return the tensor after maxpooling with parameters `kernel [3,3], pad [1,1], stride [2, 2]`
```c++
vector<vector<vector<vector<int>>>> add(vector<vector<vector<vector<int>>>> tensor1, vector<vector<vector<vector<int>>>> tensor2)
```
Receive two 4D tensors of `(b1, c1, h1, w1)` and `(b2, c2, h2, w2)`, return the 4D tensor after element-wise add (support broadcast)
# optimization skills
1.OpenMp in maxpooling

2.OpenMp and SIMD in element-wise add
# run
```
g++ -fopenmp answer.cpp
.\a.exe
```
