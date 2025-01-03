#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <polylines.hpp>

void generateSegmentsInCube(std::vector<lines::Point> &points, float size, float n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> coord(size, -size);

    for (int i = 0; i < n; i++) {
        points.push_back(lines::Point(coord(gen), coord(gen), coord(gen)));
    }
}