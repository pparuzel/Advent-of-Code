#include <iostream>
#include <vector>

void FillWithCheckSum(std::vector<int>& res, std::size_t n, int value,
                      std::size_t& checksum) {
        for (std::size_t pos = res.size(); pos < res.size() + n; ++pos) {
            checksum += pos * value;
        }
        std::fill_n(std::back_inserter(res), n, value);
}

int main() {
    char c;
    std::vector<int> blocks;
    std::vector<int> free;
    std::vector<int> result;
    bool assign_block = true;
    while (std::cin >> c) {
        (assign_block ? blocks : free).push_back(c - '0');
        assign_block = !assign_block;
    }
    std::size_t checksum = 0;

    // Fill the first block
    FillWithCheckSum(result, blocks[0], 0, checksum);

    // Go in reverse order of the blocks and fill the free space
    auto Solve = [&]() {
        int free_index = 0;
        // Assumption: max(free_index) == (free.size() - 1) == (blocks.size() - 2)
        auto GetFreeSpace = [&]() { return free[free_index]; };
        for (int i = blocks.size() - 1; i > 0;) {
            while (free[free_index] <= 0) {
                ++free_index;
                if (free_index >= free.size()) {
                    return;
                }
                // Insert static block
                FillWithCheckSum(result, blocks[free_index], free_index, checksum);
            }
            // Put N (block[i]) values (i) into free space
            // Insert moved block
            if (GetFreeSpace() < blocks[i]) {
                // Not enough space, gotta split the block
                FillWithCheckSum(result, GetFreeSpace(), i, checksum);
                blocks[i] -= GetFreeSpace();
                free[free_index] = 0;
            } else {
                FillWithCheckSum(result, blocks[i], i, checksum);
                free[free_index] -= blocks[i];
                blocks[i] = 0;
                --i;
                free[i] = 0;
            }
        }
    };
    Solve();

    std::cout << "Answer: " << checksum << '\n';
}
