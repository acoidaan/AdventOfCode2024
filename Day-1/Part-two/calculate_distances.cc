#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Función para leer los números de un archivo y separarlos por línea
void ReadNumberLists(const std::string& filename,
                     std::vector<int64_t>* left_list,
                     std::vector<int64_t>* right_list) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error al abrir el archivo: " << filename << std::endl;
    exit(1);
  }

  int64_t left_num, right_num;
  while (file >> left_num >> right_num) {
    left_list->emplace_back(left_num);
    right_list->emplace_back(right_num);
  }

  file.close();
}

// Función para calcular la distancia total entre dos listas ordenadas
int64_t CalculateSimilarityScore(const std::vector<int64_t>& left_list,
                                 const std::vector<int64_t>& right_list) {
  // Crear un mapa para contar ocurrencias
  std::unordered_map<int64_t, int> right_counts;
  for (const auto& num : right_list) {
    right_counts[num]++;
  }

  // Calcular la puntuación de similitud
  int64_t similarity_score{0};
  for (const auto& num : left_list) {
    // Mutiplicar cada número por su ocurrencia
    similarity_score += num * right_counts[num];
  }

  return similarity_score;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
    return 1;
  }

  std::vector<int64_t> left_list;
  std::vector<int64_t> right_list;

  // Leer los números del archivo
  ReadNumberLists(argv[1], &left_list, &right_list);

  // Calcular y mostrar la distancia total
  int64_t result = CalculateSimilarityScore(left_list, right_list);
  std::cout << "La puntuación de similitud es: " << result << std::endl;

  return 0;
}