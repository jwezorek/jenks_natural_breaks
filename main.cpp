#include "jenks_natural_breaks.h"
#include <iostream>

int main() {
    std::vector<double> data = { 1.0, 2.0, 3.0, 400.0, 432.0, 466.0 };
    auto natural_breaks = jenks::natural_breaks(data, 2);
    for (auto natural_break : natural_breaks) {
        std::cout << natural_break << " ";
    }
    std::cout << "\n";
    
    
    std::vector<double> unsorted_data = { 1.0, 3000, 2.0, 40,50 ,3002};
    for (auto natural_break : jenks::natural_breaks(unsorted_data, 3, false)) {
        std::cout << natural_break << " ";
    }
    std::cout << "\n";
    
    return 0;
}
