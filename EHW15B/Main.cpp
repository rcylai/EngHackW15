// ----------------------------------- //
#define _USE_MATH_DEFINES
// ----------------------------------- //
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <fstream>
// ----------------------------------- //
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
// ----------------------------------- //
#include "DataCollector.h"
#include "GameEngine.h"
#include "MainMenu.h"
#include "Note.h"
// ----------------------------------- //

int main()
{
	int centerYaw = 0;
	//bool isInGame = false;
	int state = 0;
	// We catch any exceptions that might occur below -- see the catch statement for more details.
	try {
		sf::RenderWindow* window(new sf::RenderWindow(/*sf::VideoMode::getDesktopMode()*/ sf::VideoMode(1366, 768), "Title", sf::Style::None));
		//window->setFramerateLimit(120);
		window->setPosition(sf::Vector2i(0, 0));

		sf::Font font;
		if (!font.loadFromFile("GOTHAM-ROUNDED-BOOK.OTF"))
		{
			std::cerr << "Could not load font" << std::endl;
		}

		sf::RectangleShape fadeRect(sf::Vector2f(1366, 768));
		sf::Color rectColor(255, 255, 255, 0);

		sf::Clock clock;
		MainMenu mm(window);
		GameEngine ge(window);
		ge.loadMap("EpicSong.txt");

		myo::Hub hub("com.example.hello-myo");

		std::cout << "Attempting to find a Myo..." << std::endl;

		// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
		// immediately.
		// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
		// if that fails, the function will return a null pointer.
		myo::Myo* myo = hub.waitForMyo(10000);

		// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
		if (!myo) {
			throw std::runtime_error("Unable to find a Myo!");
			return 1;
		}

		// We've found a Myo.
		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		DataCollector collector;

		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);

		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);

		long elapsedTime = -2000;

		// Finally we enter our main loop.
		while (window->isOpen()) {
			// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
			// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
			hub.run(1000 / 166);

			sf::Event event;
			while (window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
				{
					window->close();
				}
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					window->close();
				}
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::G))
				{
					clock.restart();
					state = 2;
				}
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T))
				{
					std::cout << elapsedTime << std::endl;
				}
			}

			if (state == 0)
			{
				centerYaw = mm.checkStart(collector);
				std::cout << collector.yaw_w << std::endl;
				if (centerYaw >= 0)
				{
					//std::cout << centerYaw << std::endl;
					clock.restart();
					state = 2;
					
				}
				window->clear(sf::Color(255, 255, 255));
				mm.draw();
				window->display();
			}
			/*else if (state == 1)
			{
				std::cout << elapsedTime << std::endl;
				window->clear(sf::Color(255, 255, 255));
				if (elapsedTime < 2040 && elapsedTime%8 == 0)
				{
					rectColor.a += 1;
					fadeRect.setFillColor(rectColor);
					mm.draw();
					//elapsedTime += clock.restart().asMilliseconds();
				}
				else if (elapsedTime < 4080 && elapsedTime%8 == 0)
				{
					rectColor.a -= 1;
					fadeRect.setFillColor(rectColor);
					ge.draw();
				}
				else
				{
					clock.restart();
					//elapsedTime = 0;
					state = 2;		
				}
				elapsedTime += clock.restart().asMilliseconds();
				window->draw(fadeRect);
				window->display();
			}*/
			else if (state == 2)
			{
				if (elapsedTime >= -7 && elapsedTime <= 7)
				{
					ge.playMusic();
				}
				ge.update(elapsedTime);
				if (ge.isFinished())
				{
					state = 3;
				}
				ge.setActiveColumn(collector, centerYaw);
				ge.tryHitNext(collector);

				window->clear(sf::Color(255, 255, 255));

				ge.draw();

				window->display();
				elapsedTime += clock.restart().asMilliseconds();

			}
			else
			{
				ge.draw();
				int temp = ge.checkDone(collector);
				if (temp == 1)
				{
					ge.reset();
					elapsedTime = -2000;
					state = 0;
				}
			}
		}
	}				// If a standard exception occurred, we print out its message and exit.
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
		return 1;
	}
}
