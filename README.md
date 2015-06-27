InputHandler
============
For SFML 2.1  
Stores the state of the input events, i.e. if a key is kept pressed (down), and 
allows for a easy way of testing if a input event has happened:
```
InputHandler input;
...
if (input.mouseMoved())
{
    prev_x = input.getMousePreviousPosition().x;
    curr_x = input.getMouseCurrentPosition().x;
    delta_x = curr_x - prev_x;
}
...
```

##Usage
For the InputHandler to work you need to put two method calls in your code:  
`input.update()` before handling events.  
`input.handleEvent(event)` for each event polled.  

A usage example can be found in example.cpp.  

##Modules
It has 3 modules. All active by default  

###Keyboard
To disable the Keyboard module put the following line before including the InputHandler in your code:  
`#define INPUT_DISABLE_KEYBOARD 1`  

The following methods are available in this module:  
bool                 **isKeyPressed**             (sf::Keyboard::Key key)  
bool                 **isKeyDown**                (sf::Keyboard::Key key)  
bool                 **isKeyReleased**            (sf::Keyboard::Key key)  

###Mouse
To disable the Mouse module put the following line before including the InputHandler in your code:  
`#define INPUT_DISABLE_MOUSE 1`  

The following methods are available in this module:  
bool                 **mouseMoved**               ()  
const MousePosition& **getMouseCurrentPosition**  ()  
const MousePosition& **getMousePreviousPosition** ()  
bool                 **isMouseButtonPressed**     (sf::Mouse::Button button)  
bool                 **isMouseButtonDown**        (sf::Mouse::Button button)  
bool                 **isMouseButtonReleased**    (sf::Mouse::Button button)  
bool                 **mouseWheelScrolled**       ()  
float                **getMouseWheelScrollDelta**  ()  

###Joystick
To disable the Joystick module put the following line before including the InputHandler in your code:  
`#define INPUT_DISABLE_JOYSTICK 1`  

The following methods are available in this module:  
bool                 **isJoystickConnected**      (unsigned int joystickId)  
bool                 **isJoystickActive**         (unsigned int joystickId)  
bool                 **isJoystickDisconnected**   (unsigned int joystickId)  
bool                 **isJoystickButtonPressed**  (unsigned int joystickId, unsigned int button)  
bool                 **isJoystickButtonDown**     (unsigned int joystickId, unsigned int button)  
bool                 **isJoystickButtonReleased** (unsigned int joystickId, unsigned int button)  
float                **getJoystickAxisPosition**  (unsigned int joystickId, sf::Joystick::Axis axis)  