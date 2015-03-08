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
#include "Note.h"
// ----------------------------------- //

int main()
{
	// We catch any exceptions that might occur below -- see the catch statement for more details.
	try {
		sf::RenderWindow* window(new sf::RenderWindow(/*sf::VideoMode::getDesktopMode()*/ sf::VideoMode(1366,768), "Title", sf::Style::None));
		//window->setFramerateLimit(120);
		window->display();

		GameEngine ge(window);

		ge.loadMap("map.txt");
		//std::cout << ge.getTitle() << std::endl;
		//ge.spitOutMap();

		sf::Clock clock;

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

		//myo->unlock(myo::Myo::UnlockType::unlockHold);

		//myo->unlock(myo::Myo::unlockHold);

		//long msCount = 0;
		long elapsedTime = 0;

		//ge.loadMap("map.txt");

		// Finally we enter our main loop.
		while (window->isOpen()) {
			//myo->unlock(myo::Myo::unlockHold);
			// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
			// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
			hub.run(1000 / 20);
			// After processing events, we call the print() member function we defined above to print out the values we've
			// obtained from any events that have occurred.
			//collector.print();

			//std::cout << collector.getPose().toString();
			//std::cout << "wtf" << std::endl;

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
			}

				ge.update(elapsedTime);
				

				//std::cout << elapsedTime;
				window->clear(sf::Color(255, 255, 255));

				ge.drawNotes();

				window->display();
				elapsedTime += clock.restart().asMilliseconds();
			// If a standard exception occurred, we print out its message and exit.
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
		return 1;
	}
}
