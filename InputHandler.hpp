#ifndef SFML_INPUT_HANDLER_HPP
#define SFML_INPUT_HANDLER_HPP
#include <unordered_map>
#include <SFML/Window.hpp>
#include "hash_pair.hpp"

class InputHandler
{
public:
	struct MousePosition
	{
		MousePosition():
		x(0),y(0){}
		int x;
		int y;
	};
	InputHandler()
	{
		auto p = sf::Mouse::getPosition();
		p_current_mouse_info.x = p.x;
		p_current_mouse_info.y = p.y;
		p_previous_mouse_info = p_current_mouse_info;
	}
	~InputHandler(){}

	void update()
	{
		#if !(INPUT_DISABLE_KEYBOARD)
		keyUpdate();
		#endif
		#if !(INPUT_DISABLE_MOUSE)
		mouseUpdate();
		#endif
		#if !(INPUT_DISABLE_JOYSTICK)
		joystickUpdate();
		#endif
	}

	void handleEvent(const sf::Event& event)
	{
		#if !(INPUT_DISABLE_KEYBOARD)
		if (keyEvent(event)) return;
		#endif
		#if !(INPUT_DISABLE_MOUSE)
		if (mouseEvent(event)) return;
		#endif
		#if !(INPUT_DISABLE_JOYSTICK)
		if (joystickEvent(event)) return;
		#endif
	}

#if !(INPUT_DISABLE_KEYBOARD)
	bool isKeyPressed(sf::Keyboard::Key key)
	{
		return p_key_pressed[key];
	}

	bool isKeyDown(sf::Keyboard::Key key)
	{
		return p_key_down[key];
	}

	bool isKeyReleased(sf::Keyboard::Key key)
	{
		return p_key_released[key];
	}
#endif

#if !(INPUT_DISABLE_MOUSE)
	bool mouseMoved()
	{
		return p_mouse_move_event;
	}

	const MousePosition& getMouseCurrentPosition()
	{
		return p_current_mouse_info;
	}

	const MousePosition& getMousePreviousPosition()
	{
		return p_previous_mouse_info;
	}

	bool isMouseButtonPressed(sf::Mouse::Button button)
	{
		return p_mouse_button_pressed[button];
	}

	bool isMouseButtonDown(sf::Mouse::Button button)
	{
		return p_mouse_button_down[button];
	}

	bool isMouseButtonReleased(sf::Mouse::Button button)
	{
		return p_mouse_button_released[button];
	}

	bool mouseWheelScrolled()
	{
		return p_mouse_wheel_scrolled;
	}

	float getMouseWheelScrollDelta()
	{
		return p_mouse_scroll_delta;
	}
#endif

#if !(INPUT_DISABLE_JOYSTICK)
	bool isJoystickConnected(unsigned int joystickId)
	{
		return p_joystick_connected[joystickId];
	}

	bool isJoystickActive(unsigned int joystickId)
	{
		return p_joystick_active[joystickId];
	}

	bool isJoystickDisconnected(unsigned int joystickId)
	{
		return p_joystick_disconnected[joystickId];
	}

	bool isJoystickButtonPressed(unsigned int joystickId, unsigned int button)
	{
		return p_joystick_button_pressed[std::pair<unsigned int, unsigned int>(joystickId, button)];
	}

	bool isJoystickButtonDown(unsigned int joystickId, unsigned int button)
	{
		return p_joystick_button_down[std::pair<unsigned int, unsigned int>(joystickId, button)];
	}

	bool isJoystickButtonReleased(unsigned int joystickId, unsigned int button)
	{
		return p_joystick_button_released[std::pair<unsigned int, unsigned int>(joystickId, button)];
	}

	float getJoystickAxisPosition(unsigned int joystickId, sf::Joystick::Axis axis)
	{
		return p_joystick_axis_position[std::pair<unsigned int, unsigned int>(joystickId, axis)];
	}
#endif

private:
#if !(INPUT_DISABLE_KEYBOARD)
	void keyUpdate()
	{
		for (auto it = p_key_pressed.begin(); it != p_key_pressed.end(); ++it)
		{
			if (it->second)
			{
				p_key_down[it->first] = true;
				it->second = false;
			}
		}

		for (auto it = p_key_released.begin(); it != p_key_released.end(); ++it)
		{
			if (it->second)
				it->second = false;
		}
	}

	bool keyEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			p_key_pressed[event.key.code] = not p_key_down[event.key.code];
			p_key_released[event.key.code] = false;
			return true;
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			p_key_pressed[event.key.code] = false;
			p_key_down[event.key.code] = false;
			p_key_released[event.key.code] = true;
			return true;
		}
		return false;
	}

	std::unordered_map<unsigned int, bool> p_key_pressed;
	std::unordered_map<unsigned int, bool> p_key_down;
	std::unordered_map<unsigned int, bool> p_key_released;
#endif

#if !(INPUT_DISABLE_MOUSE)
	void mouseUpdate()
	{
		p_mouse_move_event = false;

		for (auto it = p_mouse_button_pressed.begin(); it != p_mouse_button_pressed.end(); ++it)
		{
			if (it->second)
			{
				p_mouse_button_down[it->first] = true;
				it->second = false;
			}
		}

		for (auto it = p_mouse_button_released.begin(); it != p_mouse_button_released.end(); ++it)
		{
			if (it->second)
				it->second = false;
		}

		if (p_mouse_wheel_scrolled)
		{
			p_mouse_wheel_scrolled = false;
			p_mouse_scroll_delta = 0.0f;
		}
	}

	bool mouseEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			p_mouse_button_pressed[event.mouseButton.button] = not p_mouse_button_down[event.mouseButton.button];
			p_mouse_button_released[event.mouseButton.button] = false;
			return true;
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			p_mouse_button_pressed[event.mouseButton.button] = false;
			p_mouse_button_down[event.mouseButton.button] = false;
			p_mouse_button_released[event.mouseButton.button] = true;
			return true;
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			p_previous_mouse_info = p_current_mouse_info;
			MousePosition mouse;
			mouse.x = event.mouseMove.x;
			mouse.y = event.mouseMove.y;
			p_current_mouse_info = mouse;
			p_mouse_move_event = true;
			return true;
		}
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			p_mouse_wheel_scrolled = true;
			p_mouse_scroll_delta = event.mouseWheel.delta;
			return true;
		}
		return false;
	}

	bool p_mouse_move_event;
	bool p_mouse_wheel_scrolled;
	float p_mouse_scroll_delta;
	MousePosition p_current_mouse_info;
	MousePosition p_previous_mouse_info;

	std::unordered_map<unsigned int, bool> p_mouse_button_pressed;
	std::unordered_map<unsigned int, bool> p_mouse_button_down;
	std::unordered_map<unsigned int, bool> p_mouse_button_released;
#endif

#if !(INPUT_DISABLE_JOYSTICK)
	void joystickUpdate()
	{
		for (auto it = p_joystick_button_pressed.begin(); it != p_joystick_button_pressed.end(); ++it)
		{
			if (it->second)
			{
				p_joystick_button_down[it->first] = true;
				it->second = false;
			}
		}

		for (auto it = p_joystick_button_released.begin(); it != p_joystick_button_released.end(); ++it)
		{
			if (it->second)
				it->second = false;
		}

		for (auto it = p_joystick_connected.begin(); it != p_joystick_connected.end(); ++it)
		{
			if (it->second)
			{
				p_joystick_active[it->first] = true;
				it->second = false;
			}
		}

		for (auto it = p_joystick_disconnected.begin(); it != p_joystick_disconnected.end(); ++it)
		{
			if (it->second)
				it->second = false;
		}
	}

	bool joystickEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::JoystickConnected)
		{
			p_joystick_connected[event.joystickConnect.joystickId] = not p_joystick_active[event.joystickConnect.joystickId];
			p_joystick_disconnected[event.joystickConnect.joystickId] = false;
			return true;
		}
		else if (event.type == sf::Event::JoystickDisconnected)
		{
			p_joystick_connected[event.joystickConnect.joystickId] = false;
			p_joystick_active[event.joystickConnect.joystickId] = false;
			p_joystick_disconnected[event.joystickConnect.joystickId] = true;
			return true;
		}
		else if (event.type == sf::Event::JoystickButtonPressed)
		{
			std::pair<unsigned int, unsigned int> k(event.joystickButton.joystickId, event.joystickButton.button);
			p_joystick_button_pressed[k] = not p_joystick_button_down[k];
			p_joystick_button_released[k] = false;
			return true;
		}
		else if (event.type == sf::Event::JoystickButtonReleased)
		{
			std::pair<unsigned int, unsigned int> k(event.joystickButton.joystickId, event.joystickButton.button);
			p_joystick_button_pressed[k] = false;
			p_joystick_button_down[k] = false;
			p_joystick_button_released[k] = true;
			return true;
		}
		else if (event.type == sf::Event::JoystickMoved)
		{
			std::pair<unsigned int, unsigned int> k(event.joystickMove.joystickId, event.joystickMove.axis);
			p_joystick_axis_position[k] = event.joystickMove.position;
			return true;
		}
		return false;
	}

	std::unordered_map<unsigned int, bool> p_joystick_connected;
	std::unordered_map<unsigned int, bool> p_joystick_active;
	std::unordered_map<unsigned int, bool> p_joystick_disconnected;
	std::unordered_map<std::pair<unsigned int, unsigned int>, bool> p_joystick_button_pressed;
	std::unordered_map<std::pair<unsigned int, unsigned int>, bool> p_joystick_button_down;
	std::unordered_map<std::pair<unsigned int, unsigned int>, bool> p_joystick_button_released;
	std::unordered_map<std::pair<unsigned int, unsigned int>, float> p_joystick_axis_position;
#endif

};
#endif
