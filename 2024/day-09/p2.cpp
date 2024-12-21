#include <iostream>
#include <list>

struct Block {
    int value;
    int size;
};


auto BuildMemory(const std::string& sizes) -> std::list<Block> {
    std::list<Block> blocks;
    blocks.push_back({0, sizes[0] - '0'});
    for (int i = 1; i < sizes.size(); i += 2) {
        blocks.push_back({-1, sizes[i] - '0'});
        blocks.push_back({i / 2 + 1, sizes[i + 1] - '0'});
    }
    return blocks;
}

auto Checksum(const std::list<Block>& memory) -> std::size_t {
    std::size_t result = 0;
    std::size_t pos = 0;
    for (const auto& block : memory) {
        if (block.value != -1) {
            for (int i = 0; i < block.size; ++i) {
                result += (pos + i) * block.value;
            }
        }
        pos += block.size;
    }
    return result;
}

int main() {
    std::string sizes;
    std::getline(std::cin, sizes);
    auto memory = BuildMemory(sizes);

    auto block = std::prev(memory.end());
    while (block->value != 0) {
        auto free = std::next(memory.begin());
        while (std::next(block) != free) {
            if (block->size > free->size) {
                free = std::next(free, 2);
                continue;
            }
            std::prev(block)->size += block->size;
            Block new_block = *block;
            block->size = 0;

            auto right_cut = std::next(free);
            memory.insert(right_cut, new_block);
            memory.insert(right_cut, Block{-1, free->size - new_block.size});
            free->size = 0;
            break;
        }
        block = std::prev(block, 2);
    }
    std::cout << "Answer: " << Checksum(memory) << '\n';
}
