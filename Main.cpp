#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

unsigned int MAX_ITER = 200;

int iterate(double real, double complex) {
	double zReal = real;
	double zComplex = complex;
	double r2, c2;
	for (int iter = 1; iter < MAX_ITER; iter++) {
		r2 = zReal * zReal;
		c2 = zComplex * zComplex;

		zComplex = 2.0 * zReal * zComplex + complex;
		zReal = r2 - c2 + real;

		if (zReal * zReal + zComplex * zComplex > 4.0) {
			return iter;
		}
	}
	return MAX_ITER;
}

const int color_panel[21] = {
	255,   0,   0,
	255, 255,   0,
	255, 255, 255,
	255,   0, 255,
	  0,   0, 255,
	  0, 255, 255,
	  0, 255,   0 };


void help() {
	cout << "\n";
	cout << "    Mandelbrot set\n";
	cout << "\n";
	cout << "           Controls:\n";
	cout << "   W/A/S/D    -   move up/right/down/left\n";
	cout << "   Up/Down    -   zoom in/out\n";
	cout << "   Left/Right -   decrease/increase iterationlimit\n";
	cout << "   R          -   switch between CPU and GPU computation\n";
	cout << "   Enter      -   save to file (only with CPU)\n";
	cout << endl;
}

bool draw_with_shader = false;

void update_title(sf::RenderWindow& window) {
	std::string com = "";
	if (draw_with_shader) {
		com = "GPU";
	}
	else {
		com = "CPU";
	}
	window.setTitle("Mandelbrot   " + com + "    iterationlimit: " + std::to_string(MAX_ITER));
}


int main()
{

	help();

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 800;


	double min_x = -2;
	double max_x = 2;
	double min_y = -2;
	double max_y = 2;

	double dx = 0;
	double dy = 0;

	float speed = 0.06;

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
	const int imgW = 800;
	const int imgH = 800;
	img.create(imgW, imgH);

	sf::Texture texture;
	texture.loadFromImage(img);

	sf::RectangleShape rect;
	rect.setTexture(&texture);
	rect.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	bool recalculate = true;
	double zoom_factor = 2;
	int sub_division = 40;
	int min_sub_division = 40;

	sf::Texture tex;
	tex.create(SCREEN_WIDTH, SCREEN_HEIGHT);
	sf::Sprite sprite(tex);

	sf::Shader shader;
	shader.loadFromFile("shader.frag", sf::Shader::Fragment);

	update_title(window);


	while (window.isOpen())
	{
		recalculate = false;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased) {

				//switch between shader and cpu computation
				if (event.key.code == sf::Keyboard::R) {
					draw_with_shader = !draw_with_shader;
					update_title(window);
				}

				//save image
				if (event.key.code == sf::Keyboard::Enter) {
					if (!draw_with_shader) {
						img.saveToFile(std::to_string(MAX_ITER) + ".png");
					}
					else {
						cout << "Switch to CPU with 'R' to save the image" << endl;
					}
				}

			}
		}

		//increase max_iterations limit
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			recalculate = true;
			if (MAX_ITER > 0)
				MAX_ITER--;
			update_title(window);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			recalculate = true;
			MAX_ITER++;
			update_title(window);
		}

		//zoom
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			recalculate = true;
			zoom_factor *= 0.95;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			recalculate = true;
			zoom_factor /= 0.95;
		}

		//move
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

		//restart computations
		if (recalculate) {
			sub_division = min_sub_division;
		}

		recalculate = true;
		if (sub_division > 801) {
			recalculate = false;
		}

		//calculate image if needed
		if (!draw_with_shader && recalculate) {
			recalculate = false;

			double _dx = (2 * zoom_factor) / imgW;
			double _dy = (2 * zoom_factor) / imgH;

			double real, complex;
			int r, g, b;
			int iter = 0;

			//compute color for each pixel
			for (int y = 0; y < sub_division; y++) {
				for (int x = 0; x < sub_division; x++) {

					int w = 800 / sub_division;
					real = dx - zoom_factor + _dx * x * w;
					complex = dy - zoom_factor + _dy * y * w;

					//get color for pixel
					iter = iterate(real, complex);

					if (iter == MAX_ITER) {
						r = 0;
						g = 0;
						b = 0;
					}
					else {
						float index = (float(iter) / 40.0f);
						int i_index = (int(index) % 7);
						index = index - int(index);
						int next_index = ((i_index + 1) % 7) * 3;
						i_index *= 3;

						float r_ = color_panel[i_index];
						float g_ = color_panel[i_index + 1];
						float b_ = color_panel[i_index + 2];

						r = r_ + (index * (color_panel[next_index] - r_));
						g = g_ + (index * (color_panel[next_index + 1] - g_));
						b = b_ + (index * (color_panel[next_index + 2] - b_));
					}

					//set Color for pixels
					for (int Y = y * w; Y < y * w + w; Y++) {
						for (int X = x * w; X < x * w + w; X++) {
							img.setPixel(X, Y, sf::Color(r, g, b));
						}
					}

				}
			}
			//reload texture
			texture.loadFromImage(img);

			//increase subdivisions each frame  (room for improvement)
			if (sub_division < 8) {
				sub_division *= 2;
			}
			else if (sub_division == 8) {
				sub_division = 20;
			}
			else if (sub_division < 80) {
				sub_division *= 2;
			}
			else if (sub_division == 80) {
				sub_division = 200;
			}
			else if (sub_division < 801) {
				sub_division *= 2;
			}

		}




		//draw to screen

		if (!draw_with_shader)
			window.draw(rect);

		if (draw_with_shader) {
			int limit = MAX_ITER;
			shader.setUniform("texture", texture);
			shader.setUniform("limit", limit);
			shader.setUniform("min_x", (float)(dx - zoom_factor));
			shader.setUniform("max_x", (float)(dx + zoom_factor));
			shader.setUniform("min_y", (float)(dy - zoom_factor));
			shader.setUniform("max_y", (float)(dy + zoom_factor));
			window.draw(sprite, &shader);
		}

		window.display();
	}

	return 0;
}