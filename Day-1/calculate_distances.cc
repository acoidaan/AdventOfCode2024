#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
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
int64_t CalculateDistance(std::vector<int64_t>* left_list,
                          std::vector<int64_t>* right_list) {
  // Ordenar listas
  std::sort(left_list->begin(), left_list->end());
  std::sort(right_list->begin(), right_list->end());

  int64_t total_distance{0};

  // Verificar que tengan el mismo tamaño
  if (left_list->size() != right_list->size()) {
    std::cerr << "Error: Las listas no tienen el mismo tamaño" << std::endl;
    exit(1);
  }

  // Calcular la distancia entre cada ID
  for (size_t i = 0; i < left_list->size(); i++) {
    total_distance += abs((*left_list)[i] - (*right_list)[i]);
  }

  return total_distance;
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
  int64_t result = CalculateDistance(&left_list, &right_list);
  std::cout << "La distancia total entre las listas es: " << result
            << std::endl;

  return 0;
}