#pragma once
#include <vector>
#include <random>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>
#include <lines.hpp>

void generatePointsInCube(std::vector<lines::LinesVertex> &points, float size, float n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> color(0, 1);
    std::uniform_real_distribution<> coord(-size, size);

    for (int i = 0; i < n; i++) {
        points.push_back(lines::LinesVertex(coord(gen), coord(gen), coord(gen), lines::LinesVertex::COLOR(color(gen), color(gen), color(gen), 1.0)));
    }
}

void generateFrequency(std::vector<lines::LinesVertex> &points, float minAmplitude, float maxAmplitude, float minFrequency, float maxFrequency, float sampleRate, float duration, float traslate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    auto randomFloat = [&gen](float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    };

    for (float x = -duration / 2; x <= duration / 2; x += sampleRate) {

        float amplitude = randomFloat(minAmplitude, maxAmplitude);
        float frequency = randomFloat(minFrequency, maxFrequency);

        float signal = 0.0f;
        for (int i = 0; i < 3; ++i) {
            float localFreq = randomFloat(minFrequency, maxFrequency);
            float localAmp = randomFloat(minAmplitude, maxAmplitude);
            signal += localAmp * std::sin(2 * M_PI * localFreq * x + randomFloat(0, 2 * M_PI));
        }

        points.push_back(lines::LinesVertex(x, traslate + (signal), 0));
    }
}

void generateBezierCurve(std::vector<lines::LinesVertex> &points, const lines::LinesVertex& P0, const lines::LinesVertex& P1, const lines::LinesVertex& P2, int numPoints) {

    auto bezierPoint = [&P0, &P1, &P2](float t) {
        lines::LinesVertex p(0, 0, 0);
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        p.X = uu * P0.X + 2 * u * t * P1.X + tt * P2.X;
        p.Y = uu * P0.Y + 2 * u * t * P1.Y + tt * P2.Y;
        return p;
    };
    
    for (int i = 0; i <= numPoints; ++i) {
        float t = static_cast<float>(i) / numPoints;
        points.push_back(bezierPoint(t));
    }
}

inline std::vector<float> linespace(int n, float start, float stop) {
    std::vector<float> T(n);
    float step = (stop - start) / (n - 1);

    for (int i = 0; i < n; ++i)
        T[i] = start + i * step;
    return T;
}

void generateSphere(std::vector<lines::LinesVertex> &points, int n) {

    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);
    
    for (int i = 0; i < n; i++) {
        float X1 = cosf(T[i]) * sinf(R[i]);
        float Y1 = sinf(T[i]) * sinf(R[i]);
        float Z1 = cosf(R[i]);

        points.push_back(lines::LinesVertex(X1, Y1, Z1));
    }
}

void generateTourus(std::vector<lines::LinesVertex> &points, float R, float r, int numMajor, int numMinor) {
    std::vector<lines::LinesVertex> vertices;
    
    constexpr float TWO_PI = 2.0f * M_PI;

    for (int i = 0; i < numMajor; ++i) {
        float majorAngle = i * TWO_PI / numMajor;

        for (int j = 0; j < numMinor; ++j) {
            float minorAngle = j * TWO_PI / numMinor;

            float x = (R + r * cos(minorAngle)) * cos(majorAngle);
            float y = (R + r * cos(minorAngle)) * sin(majorAngle);
            float z = r * sin(minorAngle);

            vertices.emplace_back(x, y, z);
        }
    }

    std::vector<lines::LinesVertex> segments;

    for (int i = 0; i < numMajor; ++i) {
        for (int j = 0; j < numMinor; ++j) {
            int current = i * numMinor + j;
            int nextMinor = i * numMinor + (j + 1) % numMinor;
            int nextMajor = ((i + 1) % numMajor) * numMinor + j;

            segments.push_back(vertices[current]);
            segments.push_back(vertices[nextMinor]);

            segments.push_back(vertices[current]);
            segments.push_back(vertices[nextMajor]);
        }
    }
    
    points.assign(segments.begin(), segments.end());

}