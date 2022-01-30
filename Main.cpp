#include <SFML/Graphics.hpp>

unsigned int MAX_ITER = 10;

int iterate(double real, double complex) {
	double zReal = real;
	double zComplex = complex;
	for (int iter = 1; iter < MAX_ITER; iter++) {
		double r2 = zReal * zReal;
		double c2 = zComplex * zComplex;

		zComplex = 2.0 * zReal * zComplex + complex;
		zReal = r2 - c2 + real;

		if (zReal * zReal + zComplex * zComplex > 4.0) {
			return iter;
		}
	}
	return MAX_ITER;
}


int main()
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 800;

	double min_x = -2;
	double max_x = 2;
	double min_y = -2;
	double max_y = 2;

	double dx = 0;
	double dy = 0;

	float speed = 0.1;

	sf::RenderWindow window;
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot", sf::Style::Close, settings);
	window.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	sf::Image img;
	const int imgW = 500;
	const int imgH = 500;
	img.create(imgW, imgH);

	sf::Texture texture;
	texture.loadFromImage(img);

	sf::RectangleShape rect;
	rect.setTexture(&texture);
	rect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	bool recalculate = true;

	double zoom_factor = 2;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::R) {
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			recalculate = true;
			if (MAX_ITER > 0)
				MAX_ITER--;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			recalculate = true;
			MAX_ITER++;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			recalculate = true;
			zoom_factor *= 0.95;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			recalculate = true;
			zoom_factor /= 0.95;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			recalculate = true;
			dy -= speed * zoom_factor;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			recalculate = true;
			dx -= speed * zoom_factor;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			recalculate = true;
			dy += speed * zoom_factor;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			recalculate = true;
			dx += speed * zoom_factor;
		}

		//calculate image if needed
		if (recalculate) {
			recalculate = false;
			double _dx = (2 * zoom_factor) / imgW;
			double _dy = (2 * zoom_factor) / imgH;

			double real, complex;
			int r, g, b;
			int iter = 0;
			for (int y = 0; y < imgH; y++) {
				for (int x = 0; x < imgW; x++) {

					real = dx - zoom_factor + _dx * x;
					complex = dy - zoom_factor + _dy * y;

					iter = iterate(real, complex);
					if (iter == MAX_ITER) {
						r = 0;
						g = 0;
						b = 0;
					}
					else {
						r = iter;
						b = iter * iter;
						g = iter * iter * iter;
					}

					img.setPixel(x, y, sf::Color(r, g, b));

				}
			}
			texture.loadFromImage(img);
		}

		window.clear();
		window.draw(rect);
		window.display();
	}

	return 0;
}