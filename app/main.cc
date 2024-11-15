#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::string readToString(char *filename) {
  std::ifstream t(filename); // Crear el stream
  // Obtener tamaño del archivo
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();

  // Escribir el archivo a un std::string
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);

  return buffer;
}

std::vector<int> createKMPTable(const std::string &substring) {
  // Regresa la tabla de compariasion de 'substring'
  std::vector<int> res(substring.length() + 1);
  res[0] = -1;

  // Posicion calculada y siguiente candidato
  int pos = 1, cnd = 0;

  while (pos < substring.length()) {
    if (substring[pos] == substring[cnd]) {
      res[pos] = res[cnd];
    } else {
      res[pos] = cnd;

      while (cnd >= 0 && substring[pos] != substring[cnd]) {
        cnd = res[cnd];
      }
    }

    ++pos;
    ++cnd;
  }

  res[pos] = cnd;

  return res;
}

int kmpSearch(std::string &str, std::string &sub, std::vector<int> &table) {
  // Indice en el string completo y en el substring que se busca
  int j = 0, k = 0;

  while (j < str.length()) {
    if (sub[k] == str[j]) {
      ++j;
      ++k;
      if (k == sub.length())
        return j - k; // Match al substring completo!
    } else {
      k = table[k];
      if (k < 0) {
        ++j;
        ++k;
      }
    }
  }

  return -1;
}

void imprimirParte1(std::string trans, std::string code, std::vector<int> table) {
  int pos = kmpSearch(trans, code, table);
  if (pos >= 0) {
    std::printf("true %i\n", pos + 1);
  } else {
    std::printf("false\n");
  }
}

// Function to expand around the center and find the longest palindrome
std::pair<int, int> expandAroundCenter(const std::string &s, int left, int right) {
  while (left >= 0 && right < s.size() && s[left] == s[right]) {
    left--;
    right++;
  }
  return {left + 1, right - 1};
}

// Function to find the longest palindrome in a string
std::pair<int, int> findLongestPalindrome(const std::string &s) {
  int start = 0, end = 0;
  for (int i = 0; i < s.size(); ++i) {
    auto [left1, right1] = expandAroundCenter(s, i, i);
    auto [left2, right2] = expandAroundCenter(s, i, i + 1);

    if (right1 - left1 > end - start) {
      start = left1;
      end = right1;
    }
    if (right2 - left2 > end - start) {
      start = left2;
      end = right2;
    }
  }
  return {start, end};
}

void imprimirParte2(const std::string &trans) {
  auto [start, end] = findLongestPalindrome(trans);
  std::string palindrome = trans.substr(start, end - start + 1); // palindrome
  std::printf("%i %i %s\n", start + 1, end + 1, palindrome.c_str()); // position
}

size_t index2D(size_t c, int x, int y) { return (c * x) + y; }

std::pair<int, int> findLongestCommonSubstring(const std::string &a, const std::string &b) {
  size_t rows = a.length(), cols = b.length();
  std::vector<int16_t> lengths(rows * cols); // Vector de RxC (plano porque es
                                             // muy inefficiente de otra forma)

  int largest = 0;                           // Longitud mas grande encontrada
  std::pair<int, int> res;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (a[i] == b[j]) {
        if (i == 0 || j == 0) {
          lengths[(cols * i) + j] = 1;
        } else {
          lengths[(cols * i) + j] = lengths[(cols * (i - 1)) + j - 1] + 1;
        }

        // Si este substring es mas grande que el mas grande hasta
        // ahora, guardar su posicion y longitud.
        if (lengths[(cols * i) + j] > largest) {
          largest = lengths[(cols * i) + j];
          res = {i - largest + 1, largest};
        }
      }
    }
  }
  return res;
}

int main() {

  // Cargar los archivos
  std::string trans1 = readToString("transmission1.txt");
  std::string trans2 = readToString("transmission2.txt");
  std::string mcode1 = readToString("mcode1.txt");
  std::string mcode2 = readToString("mcode2.txt");
  std::string mcode3 = readToString("mcode3.txt");

  // std::printf("Cargados los archivos!\n");
  // std::printf("%s\n\n", trans1.c_str());

  std::vector<int> tableC1 = createKMPTable(mcode1);
  std::vector<int> tableC2 = createKMPTable(mcode2);
  std::vector<int> tableC3 = createKMPTable(mcode3);

  std::printf("Parte 1:\n");
  imprimirParte1(trans1, mcode1, tableC1);
  imprimirParte1(trans1, mcode2, tableC2);
  imprimirParte1(trans1, mcode3, tableC3);
  imprimirParte1(trans2, mcode1, tableC1);
  imprimirParte1(trans2, mcode2, tableC2);
  imprimirParte1(trans2, mcode3, tableC3);

  // Parte 2
  std::printf("Parte 2:\n");
  imprimirParte2(trans1);
  imprimirParte2(trans2);

  // Parte 3
  std::printf("Parte 3:\n");
  auto [idx, l] = findLongestCommonSubstring(trans1, trans2);
  std::printf("%i %i %s\n", idx + 1, idx + l, trans1.substr(idx, l).c_str());

  return 0;
}
