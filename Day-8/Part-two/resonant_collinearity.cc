#include <cmath>
#include <iostream>
#include <numeric>  // Para std::gcd
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Custom hash function for std::pair<int, int>
struct PairHash {
  template <typename T1, typename T2>
  size_t operator()(const pair<T1, T2>& p) const {
    return hash<T1>()(p.first) ^ (hash<T2>()(p.second) << 1);
  }
};

// Main function to compute unique antinodes
int countUniqueAntinodes(const vector<string>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();

  unordered_map<char, vector<pair<int, int>>> antennas;

  // Collect positions of all antennas
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      char cell = grid[r][c];
      if (isalnum(cell)) {
        antennas[cell].emplace_back(r, c);
      }
    }
  }

  unordered_set<pair<int, int>, PairHash> uniqueAntinodes;

  // Check for antinodes for each frequency
  for (const auto& [freq, positions] : antennas) {
    int n = positions.size();
    if (n < 2) {
      continue;
    }

    // Add all antenna positions themselves as antinodes
    for (const auto& pos : positions) {
      uniqueAntinodes.emplace(pos);
    }

    // Check all pairs of positions to find collinear antinodes
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        int r1 = positions[i].first, c1 = positions[i].second;
        int r2 = positions[j].first, c2 = positions[j].second;

        // Calculate the step direction for all points along the line
        int dr = r2 - r1;
        int dc = c2 - c1;
        int stepGcd = gcd(dr, dc);  // Standard GCD to normalize the step
        int stepR = dr / stepGcd;
        int stepC = dc / stepGcd;

        // Add all points along the line between the two antennas
        int currentR = r1, currentC = c1;
        while (currentR >= 0 && currentR < rows && currentC >= 0 &&
               currentC < cols) {
          uniqueAntinodes.emplace(currentR, currentC);
          currentR += stepR;
          currentC += stepC;
        }

        // Reset and go in the opposite direction
        currentR = r1 - stepR;
        currentC = c1 - stepC;
        while (currentR >= 0 && currentR < rows && currentC >= 0 &&
               currentC < cols) {
          uniqueAntinodes.emplace(currentR, currentC);
          currentR -= stepR;
          currentC -= stepC;
        }
      }
    }
  }

  // Return the number of unique antinodes
  return uniqueAntinodes.size();
}

int main() {
  // Example grid input
  vector<string> grid = {
      ".............................R......p.......7.....",
      ".........................L........................",
      "................4.................................",
      "..............Nr......4...........................",
      "............D.....................................",
      "..........................U......................b",
      "..f..........................L......J.............",
      ".f...................X.4.......U..................",
      "..................u..............U......K.........",
      ".........D..........X......j...R..................",
      ".........u.......................................p",
      "....D............Q..R.q................d....F.....",
      "............N...6...........L.....................",
      "...............................................K..",
      ".........N........RQ..X...............17.b.V......",
      "93....f.......................................7...",
      "...f...................X..................b.......",
      "............................u............7........",
      "..............j........d...q.....B..........k.....",
      ".................2......jN.............B.....p....",
      ".............3....................U..J......V.....",
      "............2....xl..1d.......................k.V.",
      "..3....4..........Qj..x..D...l.................h..",
      "................x..........V.8......Q.............",
      "...............6........d.8.......A...............",
      "..........3......6L...........8..l..1.b.J.........",
      "...................1.F............................",
      ".................................O.......M........",
      ".....x....2.......................................",
      "........6.h.......r......n.......J.......k..K..a..",
      "...I............F.....................a...........",
      ".................F...P....z....q..................",
      "....................l.................B.......P...",
      "..........................5.....O........Z......M.",
      "..i.......zn........q.............................",
      ".....................9............m...............",
      ".......................r......n..............Z....",
      "........................n..........5..............",
      ".o..............................h.M...............",
      "...............H.........................h......a.",
      "............r................v....O.....0..M...k..",
      ".....i..o.............H................A.......0..",
      ".........i....m....8..............................",
      "..........i........v..B.......................A.Z.",
      "............I....z....92.O....v..................0",
      "...o......I.....................P.................",
      "........................H......5..................",
      "..........................v......m..............0.",
      "..o............................9.....P.mZ.........",
      ".........z...I....................................",
  };

  // Compute result
  int result = countUniqueAntinodes(grid);
  cout << "Unique antinodes: " << result << endl;

  return 0;
}
