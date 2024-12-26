#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class DiskDefragmenter {
 public:
  explicit DiskDefragmenter(const std::string& disk_map) : disk_map_(disk_map) {}

  struct FileInfo {
    int id;
    int size;
    int start_pos;
    std::vector<int> positions;
  };

  std::vector<int> ParseBlocks() {
    std::vector<int> blocks;
    int current_file_id = 0;

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

  std::vector<FileInfo> GetFileInfo(const std::vector<int>& blocks) {
    std::map<int, FileInfo> files;

    // First pass: collect file sizes and positions
    for (size_t i = 0; i < blocks.size(); ++i) {
      if (blocks[i] != -1) {
        int file_id = blocks[i];
        if (files.find(file_id) == files.end()) {
          files[file_id] =
              FileInfo{file_id, 1, static_cast<int>(i), {static_cast<int>(i)}};
        } else {
          files[file_id].size++;
          files[file_id].positions.push_back(static_cast<int>(i));
        }
      }
    }

    std::vector<FileInfo> file_list;
    for (const auto& pair : files) {
      file_list.push_back(pair.second);
    }

    // Sort by file ID in descending order
    std::sort(file_list.begin(), file_list.end(),
              [](const FileInfo& a, const FileInfo& b) { return a.id > b.id; });

    return file_list;
  }

  void CompactDiskWholeFiles(std::vector<int>& blocks) {
    auto files = GetFileInfo(blocks);

    for (const auto& file : files) {
      // Find leftmost free space that can fit the file
      int best_position = -1;
      int current_span = 0;
      int span_start = -1;

      for (size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i] == -1) {
          if (span_start == -1) span_start = i;
          current_span++;
          if (current_span >= file.size && (best_position == -1 || span_start < best_position)) {
            best_position = span_start;
          }
        } else {
          current_span = 0;
          span_start = -1;
        }

        // Stop searching if we've found a position before the file's current location
        if (best_position != -1 && best_position < file.start_pos) break;
      }

      // If we foud a valid position and it's to the left of the current position
      if (best_position != -1 && best_position < file.start_pos) {
        // Clear old positions
        for (int pos : file.positions) {
          blocks[pos] = -1;
        }
        // Place file in new position
        for (int i = 0; i < file.size; ++i) {
          blocks[best_position + i] = file.id;
        }
      }
    }
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
    CompactDiskWholeFiles(blocks);
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