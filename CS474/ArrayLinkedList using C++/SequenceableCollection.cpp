#include <iostream>
#include <stdexcept>
#include <cstring>

class SequenceableCollection {
private:
    int* data;
    int capacity;
    int start;
    int end;

    //this method will be called to allocate the extra memory needed when inserting an element and the size limit has been reached
    void grow() {
        int newCapacity = capacity * 2;
        int* newData = new int[newCapacity];
        int currentSize = end - start + 1;
        int newStart = (newCapacity - currentSize) / 2;
        std::memcpy(newData + newStart, data + start, currentSize * sizeof(int));
    
        delete[] data;
        data = newData;
        capacity = newCapacity;
        start = newStart;
        end = newStart + currentSize - 1;
    }

public:
    SequenceableCollection() : capacity(8), start(3), end(2) {
        data = new int[capacity];
    }
    ~SequenceableCollection() {
        delete[] data;
    }

    SequenceableCollection& insertAt(int i, int x) {
        int currentSize = end - start + 1;
        if (i < 0 || i > currentSize) {
            throw std::out_of_range("index out of range");
        }
        if (currentSize == capacity) { grow(); }
        if (currentSize == 0) {
            data[start] = x;
            end = start;
            return *this;
        }
        int actualIndex = start + i;
        //decide  direction to shift based on the insertion
        if (actualIndex - start < end - actualIndex) {
            for (int j = start; j < actualIndex; ++j) {
                data[j - 1] = data[j];
            } start--;
            data[actualIndex-1] = x;
        } else {
            for (int j = end; j >= actualIndex; --j) {
                data[j+1] = data[j];
            }
            end++;
            data[actualIndex] = x;
        }

        return *this;
    }

    int find(int x) const {
        for (int i = start; i <= end; ++i) {
            if (data[i] == x) {
                return i - start;
            }
        }
        return -1;
    }

    SequenceableCollection& removeAt(int i) {
        int currentSize = end - start + 1;
        if (i < 0 || i >= currentSize) {
            return *this;
        }
        int actualIndex = start + i;
        if (actualIndex - start < end - actualIndex) {

            for (int j = actualIndex; j > start; --j) {
                data[j - 1] = data[j]; //left shift
            }
            start++; 
        } else {
            for (int j = actualIndex; j < end; ++j) {
                data[j] = data[j + 1]; //right shift
            }
            end--;
        }

        return *this;
    }

    SequenceableCollection& iterate(int (*fn)(int)) {
        for (int i = start; i <= end; ++i) {
            data[i] = fn(data[i]);
        }
        return *this;
    }

    void printAll() const {
        std::cout << "Elements in the collection: ";
        for (int i = start; i <= end; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    int size() const {
        return end - start + 1;
    }

    int basicSize() const {
        return capacity;
    }
};

class Main {
public:
    static void run() {
        SequenceableCollection collection;
        while (true){
            int choice;
            std::cout << "--------------------------\n";
            std::cout << "Choose:\n";
            std::cout << "1. basicSize \n";
            std::cout << "2. size \n";
            std::cout << "3. insertAt \n";
            std::cout << "4. find \n";
            std::cout << "5. removeAt \n";
            std::cout << "6. iterate \n";
            std::cout << "7. printAll \n";
            std::cout << "8. Exit \n";
            std::cout << "--------------------------\n";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    std::cout << "Basic Size: " << collection.basicSize() << std::endl;
                    break;
                case 2:
                    std::cout << "Size: " << collection.size() << std::endl;
                    break;
                case 3: {
                    int index, value;
                    std::cout << "index: ";
                    std::cin >> index;
                    std::cout << "value: ";
                    std::cin >> value;
                    try {
                        collection.insertAt(index, value);
                        std::cout << "Inserted " << value << " Index " << index << "\n";
                    } catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                    break;
                }
                case 4: {
                    int value;
                    std::cout << "Enter value: ";
                    std::cin >> value;
                    int index = collection.find(value);
                    if (index != -1) {
                        std::cout << "Element " << value << " Index " << index << ".\n";
                    } else {
                        std::cout << "Element " << value << " not found.\n";
                    }
                    break;
                }
                case 5: {
                    int index;
                    std::cout << "Enter index: ";
                    std::cin >> index;
                    collection.removeAt(index);
                    std::cout << "Removed element at index " << index << ".\n";
                    break;
                }
                case 6: {
                    auto increment = [](int x) { return x + 1; };
                    collection.iterate(increment);
                    std::cout << "all elements incremented.\n";
                    break;
                }
                case 7:
                    collection.printAll();
                    break;
                case 8:
                    exit(0);
                default:
                    std::cout << "invalid choice. Please enter 1, 2, 3, 4, 5, 6, or 7.8 to exit" << std::endl;
                    break;
            }
        }
    }
};

int main() {
    Main::run();
    return 0;
}
