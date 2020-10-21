#include <bits/stdint-uintn.h>
#include <math.h>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>

/* -------------- VECTOR CLASS REPRESENTATION -----------------------*/

class Vector3 {
   public:
    int x;
    int y;
    int z;

    Vector3(int x, int y, int z) : x(x), y(y), z(z) {}
    ~Vector3() = default;

    float abs() const { return sqrt(x * x + y * y + z * z); }
};

int operator^(const Vector3& first, const Vector3& second) {
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

float operator|(const Vector3& first, const Vector3& second) {
    return (first ^ second) / (first.abs() * second.abs());
}

Vector3 operator+(const Vector3& first, const Vector3& second) {
    return Vector3(first.x + second.x, first.y + second.y, first.z + second.z);
}

/* -------------- COLOR CLASS REPRESENTATION ------------------------*/

class Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

   public:
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    void operator<<(const float intensity) {
        r = std::min(255.f, r * intensity);
        g = std::min(255.f, g * intensity);
        b = std::min(255.f, b * intensity);
    }

    Color(sf::Color color) : r(color.r), g(color.g), b(color.b) {}

    operator sf::Color() { return sf::Color(r, g, b); }

    Color& operator+=(const Color& other) {
        r = std::min(r + other.r, 255);
        g = std::min(g + other.g, 255);
        b = std::min(b + other.b, 255);

        return *this;
    }
};


/* ----------------------- SCENE CONSTANTS ---------------------------*/

struct Material {
    const float ambient_sensetivity; 
    const float diffuse_sensetivity;
    const float specular_sensetivity;   
};

/* ----------------------- SCENE CONSTANTS ---------------------------*/
const float         AMBIENT_INTESITY            = 0.15;
const int           SPHERE_RADIUS               = 150;
const Vector3       SPHERE_CENTER               = Vector3(0, 0, 0);
const Vector3       LIGHTING_POS                = Vector3(-200, -200, 200);
const Vector3       EYE_POS                     = Vector3(0, 0, 400);
const int           SPECULAR_COEFFICIENT_POWER  = 30;
const Color         SPHERE_COLOR                = Color(0, 0, 255);
const Color         LIGHTING_COLOR              = Color(255, 255, 255); 
const Material      SPHERE_MATERIAL             = {0.8, 0.6, 0.25};

/* ---------------------- MISCELLANEOUS FUNCS ------------------------*/

Color get_color(Color sphere_color, Color lighting_color, const float diffuse_intensity, const float specular_intensity, const float ambient_intensity) {
    sphere_color << diffuse_intensity * SPHERE_MATERIAL.diffuse_sensetivity + ambient_intensity * SPHERE_MATERIAL.ambient_sensetivity;
    lighting_color << diffuse_intensity * SPHERE_MATERIAL.diffuse_sensetivity + ambient_intensity * SPHERE_MATERIAL.ambient_sensetivity + specular_intensity * SPHERE_MATERIAL.specular_sensetivity;
    sphere_color += lighting_color;

    return sphere_color;
}

void place_pixel(sf::Image& img, Color color, Vector3 pos) {
    pos.x += img.getSize().x / 2;
    pos.y += img.getSize().y / 2;
    img.setPixel(pos.x, pos.y, color);
}

void draw_sphere(sf::Image& image, int radius, Vector3 pos, Vector3 lighting_pos, Vector3 eye_pos, Color sphere_color, Color lighting_color) {
    for (int x = pos.x - radius; x < pos.x + radius; ++x) {
        for (int y = pos.y - radius; y < pos.y + radius; ++y) {
            if (x * x + y * y <= radius * radius) {
                int z = sqrt(radius * radius - x * x - y * y);

                Vector3 sphere_point = Vector3(x, y, z);

                float diffuse_intensity = std::max(0.f, sphere_point | lighting_pos);
                float specular_intensity = std::pow(std::max(0.f, (eye_pos + lighting_pos) | sphere_point), SPECULAR_COEFFICIENT_POWER);
                Color color = get_color(sphere_color, lighting_color, diffuse_intensity, specular_intensity, AMBIENT_INTESITY);

                place_pixel(image, color, Vector3(x, y, z));
            }
        }
    }
}

/*----------------------- TEST DRIVER MAIN ------------------------*/

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Raycast sphere");
    sf::Sprite image_sprite;
    sf::Image image;
    sf::Texture image_texture;
    image.create(800, 800, sf::Color::Black);
    image_texture.create(800, 800);

    window.clear(sf::Color::Black);
    draw_sphere(image, SPHERE_RADIUS, SPHERE_CENTER, LIGHTING_POS, EYE_POS, SPHERE_COLOR, LIGHTING_COLOR);

    while (window.isOpen()) {
        image_texture.loadFromImage(image);
        image_sprite.setTexture(image_texture);
        window.draw(image_sprite);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.display();
    }
    return 0;
}
