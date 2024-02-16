#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

namespace jenks {
    namespace detail {
        template<typename T>
        using matrix = std::vector<std::vector<T>>;

        template<typename T>
        struct matrices {
            matrix<int> lower_class_limits;
            matrix<T> variance_combinations;

            matrices(int data_length, int n_classes) :
                lower_class_limits(data_length + 1, std::vector<int>(n_classes + 1, 0)),
                variance_combinations(data_length + 1, std::vector<T>(n_classes + 1, 0)) {
                for (int i = 1; i <= n_classes; ++i) {
                    lower_class_limits[1][i] = 1;
                    variance_combinations[1][i] = 0;
                    for (int j = 2; j <= data_length; ++j) {
                        variance_combinations[j][i] = std::numeric_limits<T>::infinity();
                    }
                }
            }
        };

        template<typename T>
        matrices<T> generate_matrices(const std::vector<T>& data, int n_classes) {
            int data_length = static_cast<int>(data.size());
            matrices<T> matrices(data_length, n_classes);
            T variance = {};

            for (int l = 2; l <= data_length; ++l) {
                T sum = {}, sum_squares = {};
                for (int m = 1; m <= l; ++m) {
                    int lower_class_limit = l - m + 1;
                    auto val = data[lower_class_limit - 1];
                    sum += val;
                    sum_squares += val * val;
                    variance = sum_squares - (sum * sum) / m;
                    int i4 = lower_class_limit - 1;
                    if (i4 != 0) {
                        for (int j = 2; j <= n_classes; ++j) {
                            if (matrices.variance_combinations[l][j] >=
                                (variance + matrices.variance_combinations[i4][j - 1])) {
                                matrices.lower_class_limits[l][j] = lower_class_limit;
                                matrices.variance_combinations[l][j] = variance +
                                    matrices.variance_combinations[i4][j - 1];
                            }
                        }
                    }
                }
                matrices.lower_class_limits[l][1] = 1;
                matrices.variance_combinations[l][1] = variance;
            }

            return matrices;
        }
    }

    template<typename T>
    std::vector<T> natural_breaks(const std::vector<T>& inp_data, int n_classes, bool is_sorted = true) {
        std::vector<T> sorted_data;
        if (!is_sorted) {
            sorted_data = inp_data;
            std::sort(sorted_data.begin(), sorted_data.end());
        }
        const auto& data = (is_sorted) ? inp_data : sorted_data;

        detail::matrix<int> lower_class_limits = detail::generate_matrices(data, n_classes).lower_class_limits;
        int k = static_cast<int>(data.size()) - 1;
        std::vector<T> kclass(n_classes + 1, 0);

        kclass[n_classes] = data[k];
        kclass[0] = data[0];

        for (int countNum = n_classes; countNum > 1; countNum--) {
            kclass[countNum - 1] = data[lower_class_limits[k][countNum] - 2];
            k = lower_class_limits[k][countNum] - 1;
        }

        return kclass;
    }
}

