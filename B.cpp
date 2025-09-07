#include <iostream>
#include <vector>

class BitArray {
private:
    std::vector<unsigned int> arr;
    unsigned int numOnes;

public:
    BitArray(size_t size) : arr(size, 0), numOnes(0) {}

    void performOperation(char operation, unsigned int position) {
        if (operation == '+') {
            addBit(position);
        } else if (operation == '-') {
            removeBit(position);
        }
        std::cout << numOnes << std::endl;
    }

private:
    void addBit(unsigned int position) {
        if (arr[position]) {
            unsigned int firstIndex = position;
            do {
                numOnes -= 1;
                arr[firstIndex] = 0;
                firstIndex += 1;
            } while (arr[firstIndex] == 1);
            arr[firstIndex] = 1;
            numOnes++;
            return;
        }
        numOnes += 1;
        arr[position] = 1;
    }

    void removeBit(unsigned int position) {
        if (!arr[position]) {
            unsigned int firstIndex = position;
            do {
                numOnes += 1;
                arr[firstIndex] = 1;
                firstIndex += 1;
            } while (arr[firstIndex] == 0);
            numOnes -= 1;
            arr[firstIndex] = 0;
            return;
        }
        arr[position] = 0;
        numOnes -= 1;
    }
};

int main() {
    unsigned int numOperations;
    std::cin >> numOperations;
    BitArray bitArray(1000016);

    for (unsigned int i = 0; i < numOperations; ++i) {
        char operation;
        unsigned int position;
        std::cin >> operation >> position;
        bitArray.performOperation(operation, position);
    }

    return 0;
}
