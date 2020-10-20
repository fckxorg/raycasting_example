#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <math.h>
#include <unistd.h>

class Vector3 {
   public:
    int x;
    int y;
    int z;

    Vector3(int x, int y, int z)
        : x(x), y(y), z(z) {}
    ~Vector3() = default;
    
    float abs() {
        return sqrt(x * x + y * y + z * z);
    }
};

int operator^(const Vector3& first, const Vector3& second) {
    return first.x * second.x + first.y * second.y + first.z * second.z;
}


void place_pixel(sf::Image& img, sf::Color color, Vector3 pos) {
    pos.x += img.getSize().x / 2;
    pos.y += img.getSize().y / 2;
    img.setPixel(pos.x, pos.y, color);
}

void draw_circle(sf::Image& image, int radius, Vector3 pos, Vector3 lighting_pos) {
    for (int i = pos.x - radius; i < pos.x + radius; ++i) {
        for (int j = pos.y - radius; j < pos.y + radius; ++j) {
            if (i * i + j * j <= radius * radius) {
                int z = sqrt(radius* radius - i * i - j * j);
                
                Vector3 sphere_point = Vector3(i, j, z);

                float intensity = std::max(0.f, (sphere_point ^ lighting_pos) / (sphere_point.abs() * lighting_pos.abs())); 
                printf("Intensity %f\n", intensity);

                sf::Color color(intensity * 255, 0, 0);
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
        draw_circle(image, 200, Vector3(0, 0, 0), Vector3(-300, -300, i));
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
