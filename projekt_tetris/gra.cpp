#include "gra.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <fstream>

Gra::Gra()
    : window(sf::VideoMode(COLS* BLOCK, ROWS* BLOCK), "Tetris")
    , state(State::Start)
    , score(0), lastScore(0), bestScore(0)
    , timerRunning(false), elapsedSeconds(0)
    , widmo(true)
{
    window.setFramerateLimit(60);
    if (!czcionka.loadFromFile("arial.ttf"))
    {
        std::cerr << "Brak czcionki arial.ttf " << std::endl;
    }
    loadScores();
}

Gra::~Gra()
{
    timerRunning = false;
    if (timerThread.joinable())
    {
        timerThread.join();
    }
}

void Gra::loadScores()
{
    std::ifstream in("scores.dat");
    if (!(in >> lastScore >> bestScore))
    {
        lastScore = bestScore = 0;
    }
}

void Gra::saveScores()
{
    std::ofstream out("scores.dat");
    out << lastScore << " " << bestScore;
}

void Gra::startGame()
{
    area = Obszar();
    obecny = factory.losowa();
    obecny->przestun(COLS / 2 - 2, 0);

    score = lastScore = 0;
    state = State::Playing;
    elapsedSeconds = 0;
    timerRunning = true;
    timerThread = std::thread(&Gra::timerThreadFunc, this);

    dropClock.restart();
}

void Gra::timerThreadFunc()
{
    using namespace std::chrono;
    while (timerRunning)
    {
        std::this_thread::sleep_for(seconds(1));
        if (timerRunning)
        {
            ++elapsedSeconds;
        }
    }
}

void Gra::run()
{
    while (window.isOpen())
    {
        sterowanie();
        update();
        rysuj();
    }
}

void Gra::sterowanie()
{
    sf::Event e;
    while (window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mp(
                static_cast<float>(e.mouseButton.x),
                static_cast<float>(e.mouseButton.y)
            );
            float panelY = window.getSize().y * 0.2f;
            float panelH = window.getSize().y * 0.5f;
            sf::Vector2f bs(200, 50);
            sf::Vector2f startPos(
                (window.getSize().x - bs.x) / 2.f,
                panelY + panelH - bs.y - 20.f
            );
            sf::Vector2f exitPos(startPos.x, startPos.y + bs.y + 10.f);

            if (state == State::Start)
            {
                if (sf::FloatRect(startPos, bs).contains(mp))
                {
                    startGame();
                }
                else if (sf::FloatRect(exitPos, bs).contains(mp))
                {
                    window.close();
                }
            }
            else if (state == State::GameOver)
            {
                if (sf::FloatRect(startPos, bs).contains(mp))
                {
                    startGame();
                }
                else if (sf::FloatRect(exitPos, bs).contains(mp))
                {
                    window.close();
                }
            }
        }
        else if (e.type == sf::Event::KeyPressed)
        {
            if (state == State::Start && e.key.code == sf::Keyboard::Enter)
            {
                startGame();
            }
            else if (state == State::Playing)
            {

                if (e.key.code == sf::Keyboard::G)
                {
                    widmo = !widmo;
                }
                else if (e.key.code == sf::Keyboard::Left)
                {
                    obecny->przestun(-1, 0);
                    if (!area.poprawna(obecny->getBlok()))
                    {
                        obecny->przestun(1, 0);
                    }
                }
                else if (e.key.code == sf::Keyboard::Right)
                {
                    obecny->przestun(1, 0);
                    if (!area.poprawna(obecny->getBlok()))
                    {
                        obecny->przestun(-1, 0);
                    }
                }
                else if (e.key.code == sf::Keyboard::Up)
                {
                    obecny->obrot();
                    if (!area.poprawna(obecny->getBlok()))
                    {
                        obecny->obrot(), obecny->obrot(), obecny->obrot();
                    }
                }
                else if (e.key.code == sf::Keyboard::Down)
                {
                    obecny->przestun(0, 1);
                    if (!area.poprawna(obecny->getBlok()))
                    {
                        obecny->przestun(0, -1);
                    }
                }
                else if (e.key.code == sf::Keyboard::Space)
                {

                    while (area.poprawna(obecny->getBlok()))
                    {
                        obecny->przestun(0, 1);
                    }
                    obecny->przestun(0, -1);
                }
            }
            else if (state == State::GameOver)
            {
                if (e.key.code == sf::Keyboard::Enter)
                {
                    startGame();
                }
                else if (e.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }
    }
}

void Gra::update()
{
    if (state != State::Playing) return;

    if (dropClock.getElapsedTime().asSeconds() > 0.5f)
    {
        obecny->przestun(0, 1);
        if (!area.poprawna(obecny->getBlok()))
        {
            obecny->przestun(0, -1);
            area.zablokuj(*obecny);
            int lines = area.usun();
            static const int pts[5] = { 0,100,300,500,800 };
            score += pts[lines];

            obecny = factory.losowa();
            obecny->przestun(COLS / 2 - 2, 0);
            if (!area.poprawna(obecny->getBlok()))
            {
                timerRunning = false;
                if (timerThread.joinable())
                {
                    timerThread.join();
                }

                state = State::GameOver;
                lastScore = score;
                if (lastScore > bestScore) bestScore = lastScore;
                {
                    saveScores();
                }
            }
        }
        dropClock.restart();
    }
}

void Gra::rysuj()
{
    window.clear(sf::Color(15, 15, 15));

    float panelY = window.getSize().y * 0.2f;
    float panelH = window.getSize().y * 0.5f;
    sf::Vector2f mousePos(
        static_cast<float>(sf::Mouse::getPosition(window).x),
        static_cast<float>(sf::Mouse::getPosition(window).y)
    );

    if (state == State::Start)
    {

        sf::RectangleShape panel({ window.getSize().x * 0.6f, panelH });
        panel.setPosition((window.getSize().x - panel.getSize().x) / 2.f, panelY);
        panel.setFillColor({ 30,30,30,220 });
        panel.setOutlineColor(sf::Color::White);
        panel.setOutlineThickness(2);
        window.draw(panel);

        sf::Text title("TETRIS", czcionka, 48);
        title.setFillColor(sf::Color::White);
        title.setPosition(
            panel.getPosition().x + (panel.getSize().x - title.getGlobalBounds().width) / 2.f,
            panelY + 20.f
        );
        window.draw(title);

        sf::Text info(
            "Last: " + std::to_string(lastScore) +
            "   Best: " + std::to_string(bestScore),
            czcionka, 18
        );
        info.setFillColor(sf::Color::White);
        auto ib = info.getLocalBounds();
        info.setPosition(
            panel.getPosition().x + (panel.getSize().x - ib.width) / 2.f - ib.left,
            panelY + 100.f
        );
        window.draw(info);

        sf::Vector2f bs(200, 50);
        sf::Vector2f startPos(
            panel.getPosition().x + (panel.getSize().x - bs.x) / 2.f,
            panelY + panelH - bs.y - 20.f
        );
        sf::Vector2f exitPos(startPos.x, startPos.y + bs.y + 10.f);

        sf::RectangleShape b1(bs);
        b1.setPosition(startPos);
        b1.setFillColor(
            sf::FloatRect(startPos, bs).contains(mousePos)
            ? sf::Color(100, 200, 250)
            : sf::Color(50, 150, 250)
        );
        window.draw(b1);
        sf::Text t1("START", czcionka, 24);
        t1.setFillColor(sf::Color::White);
        auto lb1 = t1.getLocalBounds();
        t1.setPosition(
            startPos.x + (bs.x - lb1.width) / 2.f - lb1.left,
            startPos.y + (bs.y - lb1.height) / 2.f - lb1.top
        );
        window.draw(t1);

        sf::RectangleShape b2(bs);
        b2.setPosition(exitPos);
        b2.setFillColor(
            sf::FloatRect(exitPos, bs).contains(mousePos)
            ? sf::Color(200, 50, 50)
            : sf::Color(150, 30, 30)
        );
        window.draw(b2);
        sf::Text t2("EXIT", czcionka, 24);
        t2.setFillColor(sf::Color::White);
        auto lb2 = t2.getLocalBounds();
        t2.setPosition(
            exitPos.x + (bs.x - lb2.width) / 2.f - lb2.left,
            exitPos.y + (bs.y - lb2.height) / 2.f - lb2.top
        );
        window.draw(t2);
    }
    else if (state == State::Playing)
    {
        area.rysuj(window);

        if (widmo)
        {
            std::vector<Punkt> ghost = obecny->getBlok();
            while (area.poprawna(ghost))
                for (auto& p : ghost) ++p.y;
            for (auto& p : ghost) --p.y;

            sf::Color gc = obecny->getKolor(); gc.a = 100;
            sf::RectangleShape ghostCell({ BLOCK - 1,BLOCK - 1 });
            ghostCell.setFillColor(gc);
            for (auto& p : ghost)
            {
                ghostCell.setPosition(p.x * BLOCK, p.y * BLOCK);
                window.draw(ghostCell);
            }
        }

        sf::RectangleShape cell({ BLOCK - 1,BLOCK - 1 });
        cell.setFillColor(obecny->getKolor());
        for (auto& p : obecny->getBlok())
        {
            cell.setPosition(p.x * BLOCK, p.y * BLOCK);
            window.draw(cell);
        }

        int total = elapsedSeconds.load();
        int m = total / 60, s = total % 60;
        sf::Text timer(
            (m < 10 ? "0" : "") + std::to_string(m) + ":" +
            (s < 10 ? "0" : "") + std::to_string(s),
            czcionka, 18
        );
        timer.setFillColor(sf::Color::White);
        timer.setPosition(10, 10);
        window.draw(timer);

        sf::Text sc("Score: " + std::to_string(score), czcionka, 18);
        sc.setFillColor(sf::Color::White);
        sc.setPosition(10, 30);
        window.draw(sc);
    }
    else
    { // GameOver

        sf::RectangleShape panel({ window.getSize().x * 0.6f, panelH });
        panel.setPosition((window.getSize().x - panel.getSize().x) / 2.f, panelY);
        panel.setFillColor({ 30,30,30,220 });
        panel.setOutlineColor(sf::Color::White);
        panel.setOutlineThickness(2);
        window.draw(panel);

        sf::Text over("GAME OVER", czcionka, 28);
        over.setFillColor(sf::Color::Red);
        over.setPosition(
            panel.getPosition().x + (panel.getSize().x - over.getGlobalBounds().width) / 2.f,
            panelY + 20.f
        );
        window.draw(over);

        sf::Text scoreT("Score: " + std::to_string(lastScore), czcionka, 24);
        scoreT.setFillColor(sf::Color::White);
        scoreT.setPosition(
            panel.getPosition().x + (panel.getSize().x - scoreT.getGlobalBounds().width) / 2.f,
            panelY + 100.f
        );
        window.draw(scoreT);

        sf::Text bestT("Best: " + std::to_string(bestScore), czcionka, 24);
        bestT.setFillColor(sf::Color::White);
        bestT.setPosition(
            panel.getPosition().x + (panel.getSize().x - bestT.getGlobalBounds().width) / 2.f,
            panelY + 140.f
        );
        window.draw(bestT);

        // Przyciski RESTART / EXIT
        sf::Vector2f bs(200, 50);
        sf::Vector2f restartPos(
            panel.getPosition().x + (panel.getSize().x - bs.x) / 2.f,
            panelY + panelH - bs.y - 20.f
        );
        sf::Vector2f exitPos(restartPos.x, restartPos.y + bs.y + 10.f);

        // RESTART
        sf::RectangleShape b1(bs);
        b1.setPosition(restartPos);
        b1.setFillColor(
            sf::FloatRect(restartPos, bs).contains(mousePos)
            ? sf::Color(100, 200, 250)
            : sf::Color(50, 150, 250)
        );
        window.draw(b1);
        sf::Text rt("RESTART", czcionka, 24);
        rt.setFillColor(sf::Color::White);
        auto lb1 = rt.getLocalBounds();
        rt.setPosition(
            restartPos.x + (bs.x - lb1.width) / 2.f - lb1.left,
            restartPos.y + (bs.y - lb1.height) / 2.f - lb1.top
        );
        window.draw(rt);

        // EXIT
        sf::RectangleShape b2(bs);
        b2.setPosition(exitPos);
        b2.setFillColor(
            sf::FloatRect(exitPos, bs).contains(mousePos)
            ? sf::Color(200, 50, 50)
            : sf::Color(150, 30, 30)
        );
        window.draw(b2);
        sf::Text et("EXIT", czcionka, 24);
        et.setFillColor(sf::Color::White);
        auto lb2 = et.getLocalBounds();
        et.setPosition(
            exitPos.x + (bs.x - lb2.width) / 2.f - lb2.left,
            exitPos.y + (bs.y - lb2.height) / 2.f - lb2.top
        );
        window.draw(et);
    }

    window.display();
}