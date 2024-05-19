#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>

using namespace std;

// Structure for Huffman tree node
struct Node {
    char data;
    int freq;
    Node* left;
    Node* right;
};

// Function to create a new Huffman tree node
Node* createNode(char data, int freq) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Function to compare nodes in priority queue
struct compareNodes {
    bool operator()(const Node* left, const Node* right) {
        return left->freq > right->freq;
    }
};

// Function to build Huffman tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, compareNodes> pq;
    for (const auto& pair : freqMap) {
        pq.push(createNode(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = createNode('$', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    return pq.top();
}

// Function to generate Huffman codes
void generateCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;
    if (root->data != '$') {
        codes[root->data] = code;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Function to compress file using Huffman codes
void compressFile(const string& inputFileName, const string& outputFileName) {
    // Step 1: Read the input file and calculate character frequencies
    ifstream inputFile(inputFileName);
    unordered_map<char, int> freqMap;
    char ch;
    while (inputFile.get(ch)) {
        freqMap[ch]++;
    }
    inputFile.close();

    // Step 2: Build Huffman tree
    Node* root = buildHuffmanTree(freqMap);

    // Step 3: Generate Huffman codes
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    // Step 4: Write compressed data to output file
    ifstream inputFile2(inputFileName);
    ofstream outputFile(outputFileName);
    string encodedData = "";
    while (inputFile2.get(ch)) {
        encodedData += codes[ch];
    }
    inputFile2.close();

    // Pad the encoded data to make its length a multiple of 8
    int padding = 8 - (encodedData.length() % 8);
    for (int i = 0; i < padding; ++i) {
        encodedData += '0';
    }

    // Write the encoded data length and encoded data to the output file
    outputFile << encodedData.length() << "\n";
    for (size_t i = 0; i < encodedData.length(); i += 8) {
        bitset<8> bits(encodedData.substr(i, 8));
        outputFile << static_cast<char>(bits.to_ulong());
    }
    outputFile.close();

    // Step 5 (Optional): Decode the compressed file to verify correctness
}

int main() {
    string inputFileName = "input.txt";
    string outputFileName = "output.txt";
    compressFile(inputFileName, outputFileName);
    return 0;
}
