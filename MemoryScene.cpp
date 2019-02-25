#include <string>
#include <RGF/Game.h>
#include <RGF/HighScoreManager.h>
#include <RGF/Score.h>
#include <RGFUtil/Timer.h>

#include "MemoryScene.h"

MemoryScene::MemoryScene(rgf::Game& game)
	:rgf::Scene(game),
	panel(new rgf::SidePanel(*this)),
	currentDiff(Card::Difficulty::EASY),
	btnEasy(new rgf::Button(*this, BTN_EASY_PRESSED, L"EASY")),
	btnMedium(new rgf::Button(*this, BTN_MEDIUM_PRESSED, L"MEDIUM")),
	btnHard(new rgf::Button(*this, BTN_HARD_PRESSED, L"HARD")),
	bgTexture(game.getResourceManager().loadTexture("res/images/bg.png")),
	btnFont(game.getResourceManager().loadFont("res/fonts/SpaceComics.ttf")),
	lblFont(game.getResourceManager().loadFont("res/fonts/HYDRO.ttf")),
	bgSprite(*bgTexture),
	tryCounterText("TRIES : 0", *lblFont),
	congratsText("", *lblFont),
	endScoreText("", *lblFont),
	highScoreTitle("", *lblFont),
	highScoreTime("", *lblFont),
	highScoreTries("", *lblFont),
	timer(new rgf::Timer(*this)),
	tryCounter(0),
	titleTexture(game.getResourceManager().loadTexture("res/images/memorylogo.png")),
	winningSprite(*getGame().getResourceManager().loadTexture("res/images/win.png")),
	winningDrawable(new rgf::DrawableSprite(winningSprite))
{
	timer->setBgColor({ 0, 204, 0 });

	timer->setSize(sf::Vector2f(115.f, 63.f));
	btnEasy->setSize(sf::Vector2f(150.f, 75.f));
	btnMedium->setSize(sf::Vector2f(150.f, 75.f));
	btnHard->setSize(sf::Vector2f(150.f, 75.f));

	winningDrawable->setPosition({ 300, 500 });

	btnEasy->setFont(btnFont);
	btnMedium->setFont(btnFont);
	btnHard->setFont(btnFont);

	btnEasy->setCharacterSize(13);
	btnMedium->setCharacterSize(13);
	btnHard->setCharacterSize(13);
	tryCounterText.setCharacterSize(20);
	congratsText.setCharacterSize(40);
	endScoreText.setCharacterSize(20);
	highScoreTitle.setCharacterSize(20);
	highScoreTime.setCharacterSize(12);
	highScoreTries.setCharacterSize(12);

	timer->setTextColor(sf::Color::White);
	btnEasy->setFillColor({ 233, 109, 255 });
	btnMedium->setFillColor({ 109, 192, 255 });
	btnHard->setFillColor({ 234, 199, 0 });
	tryCounterText.setFillColor(sf::Color::White);
	congratsText.setFillColor(sf::Color::White);
	endScoreText.setFillColor(sf::Color::White);
	highScoreTitle.setFillColor(sf::Color::White);
	highScoreTime.setFillColor(sf::Color::White);
	highScoreTries.setFillColor(sf::Color::White);

	attachChild(timer, true);
	attachChild(btnEasy);
	attachChild(btnMedium);
	attachChild(btnHard);
	attachChild(panel);

	attachChild(winningDrawable);
	bringToFront(*winningDrawable);
	winningDrawable->setVisibility(false);

	panel->setDescription(getGame().getGameSettings().description);
	panel->setCredits(getGame().getGameSettings().credits);
	panel->setCreditsCharacterSize(22);
	panel->setTitleImage(titleTexture);
	panel->setPosition(1440, 0);

	timer->setPosition(1190, 33);
	btnEasy->setPosition(1175, 150);
	btnMedium->setPosition(1175, 300);
	btnHard->setPosition(1175, 450);

	tryCounterText.setPosition(1175, 600);
	congratsText.setPosition(280, 300);
	endScoreText.setPosition(305, 400);
	highScoreTitle.setPosition(1160, 710);
	highScoreTime.setPosition(1175, 770);
	highScoreTries.setPosition(1275, 770);

	getGame().getHighScoreManager().createScoreGroup(xmlGroupTime, false);
	getGame().getHighScoreManager().createScoreGroup(xmlGroupTries, false);

	highScoreTitle.setString("Best Times:");
	printTimeWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTime));
	printTriesWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTries));

	onEasyPressed();
}


MemoryScene::~MemoryScene()
{
}

void MemoryScene::initCurrent()
{
	getGame().getEventManager().subscribe(sf::Event::Closed, *this, [this](auto& data) {this->getGame().close(); });
	getEventManager().subscribe(BTN_EASY_PRESSED, *this, [this](auto& data) { this->onEasyPressed(); });
	getEventManager().subscribe(BTN_MEDIUM_PRESSED, *this, [this](auto& data) { this->onMediumPressed(); });
	getEventManager().subscribe(BTN_HARD_PRESSED, *this, [this](auto& data) { this->onHardPressed(); });
	getEventManager().subscribe(Card::CARD_PRESSED, *this, [this](auto& data) {this->onCardPressed(data); });
	getEventManager().subscribe(DELAY_FLIP, *this, [this](auto& data) { this->onDelayFlip(); });
	getEventManager().subscribe(DELAY_PAIR, *this, [this](auto& data) { this->onDelayPair(); });
	getEventManager().subscribe(WINNING_SCENARIO, *this, [this](auto& data) { this->winScenario(); });
}

void MemoryScene::updateCurrent(const sf::Time & dt)
{
	tryCounterText.setString("TRIES : " + std::to_string(tryCounter));
}

void MemoryScene::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(bgSprite, states);
	target.draw(tryCounterText, states);
	target.draw(congratsText, states);
	target.draw(endScoreText, states);
	target.draw(highScoreTitle, states);
	target.draw(highScoreTime, states);
	target.draw(highScoreTries, states);
}

void MemoryScene::cleanupCurrent()
{
	getGame().getEventManager().unsubscribe(sf::Event::Closed, *this);
	getEventManager().unsubscribe(BTN_EASY_PRESSED, *this);
	getEventManager().unsubscribe(BTN_MEDIUM_PRESSED, *this);
	getEventManager().unsubscribe(BTN_HARD_PRESSED, *this);
	getEventManager().unsubscribe(Card::CARD_PRESSED, *this);
	getEventManager().unsubscribe(DELAY_FLIP, *this);
	getEventManager().unsubscribe(DELAY_PAIR, *this);
	getEventManager().unsubscribe(WINNING_SCENARIO, *this);
}

void MemoryScene::onEasyPressed()
{
	createDeck(Card::Difficulty::EASY);
	resetGame(Card::Difficulty::EASY);

	btnEasy->setScale(1.2f, 1.2f);
	btnMedium->setScale(1.f, 1.f);
	btnHard->setScale(1.f, 1.f);
}

void MemoryScene::onMediumPressed()
{
	createDeck(Card::Difficulty::MEDIUM);
	resetGame(Card::Difficulty::MEDIUM);

	btnEasy->setScale(1.f, 1.f);
	btnMedium->setScale(1.2f, 1.2f);
	btnHard->setScale(1.f, 1.f);
}

void MemoryScene::onHardPressed()
{
	createDeck(Card::Difficulty::HARD);
	resetGame(Card::Difficulty::HARD);

	btnEasy->setScale(1.f, 1.f);
	btnMedium->setScale(1.f, 1.f);
	btnHard->setScale(1.2f, 1.2f);
}

void MemoryScene::onCardPressed(const rgf::EventData & data)
{
	auto cardPtr = data.getDataAs<Card*>();
	if (firstCard == nullptr)
	{
		timer->start();
		firstCard = cardPtr;
		firstCard->flipCard();
	}
	else if (secondCard == nullptr)
	{
		if (firstCard != cardPtr)
		{
			secondCard = cardPtr;
			secondCard->flipCard();
			increaseTry();

			if (firstCard->getName() == secondCard->getName())
			{
				pairCount++;
				getEventManager().delayEvent(rgf::DelayedEvent(DELAY_PAIR, sf::seconds(0.75f)));
				isPair = true;
				if (secondCard->getDifficulty() == Card::Difficulty::EASY && pairCount == 8)
				{
					getEventManager().delayEvent(rgf::DelayedEvent(WINNING_SCENARIO, sf::seconds(1.0f)));
				}
				if (secondCard->getDifficulty() == Card::Difficulty::MEDIUM && pairCount == 18)
				{
					getEventManager().delayEvent(rgf::DelayedEvent(WINNING_SCENARIO, sf::seconds(1.0f)));
				}
				if (secondCard->getDifficulty() == Card::Difficulty::HARD && pairCount == 32)
				{
					getEventManager().delayEvent(rgf::DelayedEvent(WINNING_SCENARIO, sf::seconds(1.0f)));
				}
			}
			else
			{
				getEventManager().delayEvent(rgf::DelayedEvent(DELAY_FLIP, sf::seconds(0.75f)));
				isPair = false;
			}
		}
	}
	else
	{
		if (firstCard != cardPtr && secondCard != cardPtr)
		{
			if (isPair)
			{
				onDelayPair();
				firstCard = cardPtr;
				firstCard->flipCard();
				getEventManager().cancelEvent(DELAY_PAIR);
			}
			else
			{
				onDelayFlip();
				firstCard = cardPtr;
				firstCard->flipCard();
				getEventManager().cancelEvent(DELAY_FLIP);
			}

		}
	}
}

void MemoryScene::onDelayFlip()
{
	if (firstCard != nullptr)
	{
		firstCard->flipCard();
		firstCard = nullptr;
	}
	if (secondCard != nullptr)
	{
		secondCard->flipCard();
		secondCard = nullptr;
	}
}

void MemoryScene::onDelayPair()
{
	if (firstCard != nullptr)
	{
		firstCard->setVisibility(false);
		firstCard = nullptr;
	}
	if (secondCard != nullptr)
	{
		secondCard->setVisibility(false);
		secondCard = nullptr;
	}
}

void MemoryScene::createDeck(Card::Difficulty diff)
{
	if (deck.size() > 0)
	{
		for (unsigned int i = 0; i < deck.size(); i++)
		{
			detachChild(*deck[i], true);
		}
	}

	deck = (Card::generateDeck(*this, diff));

	for (auto& c : deck)
	{
		attachChild(c, true);
	}

}

void MemoryScene::resetGame(Card::Difficulty diff)
{
	firstCard = nullptr;
	secondCard = nullptr;
	winningDrawable->setVisibility(false);
	timer->reset();

	tryCounter = 0;
	pairCount = 0;

	congratsText.setString("");
	endScoreText.setString("");
	highScoreTime.setString("");
	highScoreTries.setString("");

	std::shuffle(deck.begin(), deck.end(), rng.getEngine());

	const int spacing = 33;
	unsigned int row = spacing;
	unsigned int col = spacing;
	const int maxWidth = 1100;

	for (auto& c : deck)
	{
		c->setDifficulty(diff);
		c->setVisibility(true);
		if (col + c->getSize().x >= maxWidth)
		{
			col = spacing;
			row += spacing + c->getSize().y;
		}
		c->setPosition(static_cast<float>(col), static_cast<float>(row));
		col += spacing + c->getSize().x;
	}

	updateCurrentDifficulty(diff);
	
	printTimeWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTime));
	printTriesWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTries));
}

void MemoryScene::updateCurrentDifficulty(Card::Difficulty newDiff)
{
	currentDiff = newDiff;
}

void MemoryScene::winScenario()
{
	winningDrawable->setVisibility(true);
	timer->stop();
	congratsText.setString(L"CONGRATULATIONS!");
	endScoreText.setString(std::wstring(L"You cleared the board in: " + sf::String(std::to_string(tryCounter))) + sf::String(" tries"));

	float fTime = timer->getTime().asSeconds();
	int tempTime = int(fTime);

	getGame().getHighScoreManager().submitScore(xmlGroupTime, { L" ", tempTime });
	getGame().getHighScoreManager().submitScore(xmlGroupTries, { L" ", tryCounter });
	printTimeWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTime));
	printTriesWrapper(getGame().getHighScoreManager().queryHighScores(xmlGroupTries));
}

void MemoryScene::increaseTry()
{
	tryCounter += 1;
}

void MemoryScene::printTriesWrapper(std::list<rgf::Score> outputList)
{
	std::string tempStr;
	for (const rgf::Score & output : outputList)
	{
		tempStr.append(std::to_string(output.value) + " Tries" + "\n");
	}
	highScoreTries.setString(tempStr);
}

void MemoryScene::printTimeWrapper(std::list<rgf::Score> outputList)
{
	std::string tempStr;
	for (const rgf::Score & output : outputList)
	{
		tempStr.append(std::to_string(output.value) + " Sec :" + "\n");
	}
	highScoreTime.setString(tempStr);
}
