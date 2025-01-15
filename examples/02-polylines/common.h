#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <polylines.hpp>

void generateSegmentsInCube(std::vector<lines::LinesVertex> &points, float size, float n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(size, -size);

    for (int i = 0; i < n; i++) {
        points.push_back(lines::LinesVertex(coord(gen), coord(gen), coord(gen), lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0)));
    }
}