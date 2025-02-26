#pragma once
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime> 
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
        for (int i = 0; i < 3; ++i) { // Usa 3 onde con fasi diverse
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