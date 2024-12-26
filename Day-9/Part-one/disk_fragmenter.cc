#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class DiskDefragmenter {
 public:
  explicit DiskDefragmenter(const std::string& disk_map)
      : disk_map_(disk_map) {}

  std::vector<int> ParseBlocks() {
    std::vector<int> blocks;
    int current_file_id = 0;
    int pos = 0;

    for (size_t i = 0; i < disk_map_.length(); ++i) {
      int length = disk_map_[i] - '0';

      // Add file blocks or free space
      if (i % 2 == 0) {  // File blocks
        for (int j = 0; j < length; ++j) {
          blocks.push_back(current_file_id);
        }
        current_file_id++;
      } else {  // Free space
        for (int j = 0; j < length; ++j) {
          blocks.push_back(-1);  // -1 for free space
        }
      }
    }
    return blocks;
  }

  void CompactDisk(std::vector<int>& blocks) {
    bool changes;
    do {
      changes = false;
      // Find rightmost file
      int rightmost_file = -1;
      for (int i = blocks.size() - 1; i >= 0; --i) {
        if (blocks[i] != -1) {
          rightmost_file = i;
          break;
        }
      }
      if (rightmost_file == -1) break;

      // Find leftmost free space
      int leftmost_space = -1;
      for (int i = 0; i < rightmost_file; ++i) {
        if (blocks[i] == -1) {
          leftmost_space = i;
          break;
        }
      }

      if (leftmost_space == -1) break;

      // Move file block
      int file_id = blocks[rightmost_file];
      blocks[leftmost_space] = file_id;
      blocks[rightmost_file] = -1;
      changes = true;

    } while (changes);
  }

  int64_t CalculateCheckSum(const std::vector<int>& blocks) {
    int64_t checksum = 0;
    for (size_t i = 0; i < blocks.size(); ++i) {
      if (blocks[i] != -1) {  // Skip free space
        checksum += static_cast<int64_t>(i) * blocks[i];
      }
    }
    return checksum;
  }

  int64_t Solve() {
    auto blocks = ParseBlocks();
    CompactDisk(blocks);
    return CalculateCheckSum(blocks);
  }

 private:
  std::string disk_map_;
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::ifstream input_file(argv[1]);
  if (!input_file) {
    std::cerr << "Error: Can't open file: " << argv[1] << std::endl;
    return 1;
  }

  std::string input;
  std::getline(input_file, input);

  DiskDefragmenter defragmenter(input);
  std::cout << defragmenter.Solve() << std::endl;

  return 0;
}