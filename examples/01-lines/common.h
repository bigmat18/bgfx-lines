#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <lines.hpp>

void generatePointsInSphere(std::vector<lines::LinesVertex> &points, float radius, float n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_phi(0.0, 2 * M_PI);
    std::uniform_real_distribution<> dis_theta(-M_PI / 2, M_PI / 2);
    std::uniform_real_distribution<> color(0, 1);

    float phi0, theta0, phi1, theta1;
    float x0, y0, z0, x1, y1, z1;
    float r, g, b, a;

    for (uint i = 0; i < n; i++) {
        phi0 = dis_phi(gen);
        theta0 = dis_theta(gen);

        x0 = radius * cos(theta0) * cos(phi0);
        y0 = radius * cos(theta0) * sin(phi0);
        z0 = radius * sin(theta0);

        phi1 = dis_phi(gen);
        theta1 = dis_theta(gen);

        x1 = radius * cos(theta1) * cos(phi1);
        y1 = radius * cos(theta1) * sin(phi1);
        z1 = radius * sin(theta1);

        points.push_back(lines::LinesVertex(x0, y0, z0, lines::COLOR(color(gen), color(gen), color(gen), 1.0)));
        points.push_back(lines::LinesVertex(x1, y1, z1, lines::COLOR(color(gen), color(gen), color(gen), 1.0)));
    }
}

void generatePointsInCube(std::vector<lines::LinesVertex> &points, float size, float n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(size, -size);

    for (int i = 0; i < n; i++) {
        points.push_back(lines::LinesVertex(coord(gen), coord(gen), coord(gen), lines::COLOR(color(gen), color(gen), color(gen), 1.0)));
        points.push_back(lines::LinesVertex(coord(gen), coord(gen), coord(gen), lines::COLOR(color(gen), color(gen), color(gen), 1.0)));
    }
}