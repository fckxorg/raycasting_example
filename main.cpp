#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <math.h>

class Vector3 {
   public:
    int x;
    int y;
    int z;

    Vector3(int x, int y, int z)
        : x(x), y(y), z(z) {}
    ~Vector3() = default;
};

void place_pixel(sf::Image& img, sf::Color color, Vector3 pos) {
    pos.x += img.getSize().x / 2;
    pos.y += img.getSize().y / 2;
    img.setPixel(pos.x, pos.y, color);
}

void draw_circle(sf::Image& image, int radius, Vector3 pos) {
    for (int i = pos.x - radius; i < pos.x + radius; ++i) {
        for (int j = pos.y - radius; j < pos.y + radius; ++j) {
            if (i * i + j * j <= radius * radius) {
                int z = sqrt(radius* radius - i * i - j * j);
                sf::Color color((static_cast<float>(z) / radius) * 255, 0, 0);
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

    draw_circle(image, 200, Vector3(0, 0, 0));
    image_texture.loadFromImage(image);
    image_sprite.setTexture(image_texture);

    window.clear(sf::Color::Black);
    while (window.isOpen()) {
        window.draw(image_sprite);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.display();
    }
    return 0;
}
