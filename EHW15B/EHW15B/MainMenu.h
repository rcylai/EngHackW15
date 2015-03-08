#ifndef MAINMENU_H
#define MAINMENU_H
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>

class MainMenu
{
private:
	sf::RenderWindow* window;
	sf::Texture bgtx, sbtx;
	sf::Sprite background, startBtn;
public:
	MainMenu(sf::RenderWindow* window)
	{
		this->window = window;
		
		if (!bgtx.loadFromFile("Images/startscreen.png")) {
			std::cerr << "Could not load bg" << std::endl;
		}
		background.setTexture(bgtx);

		if (!sbtx.loadFromFile("Images/Start.png")) {
			std::cerr << "Could not load spread" << std::endl;
		}
		startBtn.setTexture(sbtx);
	}

	void draw()
	{
		window->draw(background);
		window->draw(startBtn);
	}

	int checkStart(DataCollector& collector)
	{
		if (myo::Pose::fist == collector.getPose())
		{
			return collector.yaw_w;
		}
		else
		{
			return -1;
		}
	}

	/*void animateStart(sf::Clock& clock)
	{
		sf::RectangleShape fadeRect(sf::Vector2f(1366, 768));
		sf::Color rectColor(255, 255, 255, 0);
		fadeRect.setFillColor(rectColor);
		long elapsed = 0;
		bool done = false;

		while (done == false)
		{
			if (elapsed <= 510)
			{
				if (elapsed % 2 == 0)
				{
					rectColor.a += 1;
					fadeRect.setFillColor(rectColor);
				}
			}
			else
			{
				done = true;
			}
			window->clear(sf::Color(255, 255, 255));
			draw();
			window->draw(fadeRect);
			elapsed += clock.restart().asMilliseconds();
		}
	}*/
};
#endif