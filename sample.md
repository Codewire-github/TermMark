### 🔹 1.10 Supervised and Unsupervised Algorithms

#### 🎯 **Supervised Learning**:

- Requires labeled data (input-output pairs).
- Examples:
  - Linear Regression
  - Logistic Regression
  - Support Vector Machines (SVMs)
  - Neural Networks
  - Decision Trees
  - Random Forests

> Used for tasks like classification and regression.

#### 🧠 **Unsupervised Learning**:

- No labeled data; the model discovers hidden patterns or groupings in the input.
- Examples:
  - K-Means Clustering
  - Principal Component Analysis (PCA)
  - Autoencoders
  - Self-Organizing Maps

> Used for tasks like dimensionality reduction, clustering, and anomaly detection.

```cpp
int add(int a, int b) {
    return a + b;
}
```

```cpp
#include <iostream>
#include <vector>

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    for (int num : nums) {
        std::cout << "Factorial of " << num << " is ";
        std::cout << factorial(num) << std::endl;
    }
    return 0;
}
```
