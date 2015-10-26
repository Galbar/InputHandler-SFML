// For this example we don't need these modules
#define INPUT_DISABLE_KEYBOARD 1
#define INPUT_DISABLE_JOYSTICK 1

#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	InputHandler input;
	while (window.isOpen())
	{
		input.update(); //<-- update()
		sf::Event event;
		while (window.pollEvent(event))
		{
			input.handleEvent(event); //<-- handleEvent(event)
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Change the circle radius on mouse wheel scroll.
		shape.setRadius(shape.getRadius() + input.getMouseWheelScrollDelta());

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
