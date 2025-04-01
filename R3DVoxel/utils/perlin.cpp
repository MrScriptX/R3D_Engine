#include "perlin.h"

PerlinNoise::PerlinNoise(unsigned int seed) : m_seed(seed)
{
    perm.resize(256);
    std::iota(perm.begin(), perm.end(), 0);
    /*std::default_random_engine engine(seed);
    std::shuffle(perm.begin(), perm.end(), engine);*/
    perm.insert(perm.end(), perm.begin(), perm.end());
}

float PerlinNoise::perlin(float x, float y) const
{
    x *= 0.00999999978f;
    y *= 0.00999999978f;

    int x0 = FastFloor(x);
    int y0 = FastFloor(y);

    const float f_x0 = x - static_cast<float>(x0);
    const float f_y0 = y - static_cast<float>(y0);
    const float f_x1 = f_x0 - 1.0f;
    const float f_y1 = f_y0 - 1.0f;

    const float xs = static_cast<float>(fade(f_x0));
    const float ys = static_cast<float>(fade(f_y0));

    x0 *= 501125321;
    y0 *= 1136930381;
    const int x1 = x0 + 501125321;
    const int y1 = y0 + 1136930381;

    const double n0 = lerp(xs, dotgridgradient(x0, y0, f_x0, f_y0), dotgridgradient(x1, y0, f_x1, f_y0));
	const double n1 = lerp(xs, dotgridgradient(x0, y1, f_x0, f_y1), dotgridgradient(x1, y1, f_x1, f_y1));

    const auto r = lerp(ys, static_cast<float>(n0), static_cast<float>(n1));
    return static_cast<float>(r) * 1.4247691104677813f;
}

void PerlinNoise::exportppm() const
{
    std::ofstream file;
    file.open("image.ppm");

    // Image
    constexpr int image_width = 500;
	constexpr int image_height = 500;

    // Render
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) 
    {
        for (int i = 0; i < image_width; ++i) 
        {
            float x = float(i);
            float y = float(j);

            double n = perlin(x, y);
            n += 1.0;
            n /= 2.0;

            int ir = static_cast<int>(255 * n);
            int ig = static_cast<int>(255 * n);
            int ib = static_cast<int>(255 * n);

            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    file.close();
}

float PerlinNoise::dotgridgradient(int ix, int iy, float x, float y) const
{
    int hash = m_seed ^ ix ^ iy;
    hash *= 0x27d4eb2d;
    hash ^= hash >> 15;
    hash &= 127 << 1;

    const float xg = Lookup<float>::Gradients2D[hash];
    const float yg = Lookup<float>::Gradients2D[hash | 1];

    return x * xg + y * yg;
}

double PerlinNoise::gradiant(int hash, double x, double y) const
{
    int h = hash & 7;
    double u = h < 4 ? x : y;
    double v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}

double PerlinNoise::lerp(float t, float a, float b) const
{
    return a + t * (b - a);
}

double PerlinNoise::fade(double t) const
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
