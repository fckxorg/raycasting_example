#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <math.h>
#include <unistd.h>

const float AMBIENT_INTESITY = 0.15;

class Vector3 {
   public:
    int x;
    int y;
    int z;

    Vector3(int x, int y, int z)
        : x(x), y(y), z(z) {}
    ~Vector3() = default;
    
    float abs() const {
        return sqrt(x * x + y * y + z * z);
    }
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


sf::Color get_color(sf::Color sphere_color, sf::Color lighting_color, const float diffuse_intensity, const float specular_intensity, const float ambient_intensity) {
    sphere_color.r = std::min(255.f, (sphere_color.r  + lighting_color.r) * (diffuse_intensity * 0.6f + ambient_intensity) + lighting_color.r * specular_intensity * 0.25f);
    sphere_color.g = std::min(255.f, (sphere_color.g  + lighting_color.g) * (diffuse_intensity * 0.6f + ambient_intensity) + lighting_color.g * specular_intensity * 0.25f);
    sphere_color.b = std::min(255.f, (sphere_color.b  + lighting_color.b) * (diffuse_intensity * 0.6f + ambient_intensity) + lighting_color.b * specular_intensity * 0.25f);

    return sphere_color;
}

void place_pixel(sf::Image& img, sf::Color color, Vector3 pos) {
    pos.x += img.getSize().x / 2;
    pos.y += img.getSize().y / 2;
    img.setPixel(pos.x, pos.y, color);
}

void draw_sphere(sf::Image& image, int radius, Vector3 pos, Vector3 lighting_pos, Vector3 eye_pos, sf::Color sphere_color, sf::Color lighting_color) {
    for (int i = pos.x - radius; i < pos.x + radius; ++i) {
        for (int j = pos.y - radius; j < pos.y + radius; ++j) {
            if (i * i + j * j <= radius * radius) {
                int z = sqrt(radius* radius - i * i - j * j);
                
                Vector3 sphere_point = Vector3(i, j, z);

                float diffuse_intensity = std::max(0.f, sphere_point | lighting_pos);
                float specular_intensity = std::pow(std::max(0.f, (eye_pos + lighting_pos) | sphere_point), 30);
                sf::Color color = get_color(sphere_color, lighting_color, diffuse_intensity, specular_intensity, AMBIENT_INTESITY);
                
                place_pixel(image, color, Vector3(i, j, z));
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Raycast sphere");
    sf::Sprite image_sprite;
    sf::Image image;
    sf::Texture image_texture;
    image.create(800, 800, sf::Color::Black);
    image_texture.create(800, 800);


    window.clear(sf::Color::Black);
    int i = 0;
    while (window.isOpen()) {
        draw_sphere(image, 150, Vector3(0, 0, 0), Vector3(-200, -200, i), Vector3(0, 0, 400), sf::Color::Red, sf::Color::White);
        image_texture.loadFromImage(image);
        image_sprite.setTexture(image_texture);
        window.draw(image_sprite);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.display();
        sleep(1);
        i += 50;
    }
    return 0;
}
