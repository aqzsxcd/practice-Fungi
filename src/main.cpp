#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

struct Okno
{
    int x, y;                     // координаты левого верхнего угла
    int w, h;                     // ширина и высота окна
    bool aktivno;                 // активно ли окно 
    bool zakryto;                 // помечено ли окно на закрытие
    std::string nazvanie;         // имя окна 
    bool peretaskivaetsya;        // перетаскивается ли окно сейчас
    int otnosX, otnosY;           // смещение клика относительно угла окна
    int vstryaskaTimer;           // таймер для анимации встряски
    std::string tekstVOkne;       // текст внутри окна
    bool izmenenieRazmera;        // режим изменения размера
    int otnosW, otnosH;           // смещение для изменения размера
};

int main()
{
    int indeksPeretaskivaemogoOkna = -1;
    int indeksIzmeneniyaRazmera = -1;

    sf::RenderWindow desktop(sf::VideoMode(1024, 768), "Oconnyj Menedzher");

    std::vector<Okno> okna;
    std::vector<sf::Color> cveta;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Font shrift;
    if (!shrift.loadFromFile("arial.ttf"))
    {
        // Шрифт не загрузился — текста не будет видно
    }

    while (desktop.isOpen())
    {
        sf::Event sobytie;

        while (desktop.pollEvent(sobytie))
        {
            if (sobytie.type == sf::Event::Closed)
            {
                desktop.close();
            }

            // СОЗДАНИЕ НОВОГО ОКНА (клавиша N)
            if (sobytie.type == sf::Event::KeyPressed)
            {
                if (sobytie.key.code == sf::Keyboard::N)
                {
                    Okno novoeOkno;
                    novoeOkno.x = 50 + okna.size() * 30;
                    novoeOkno.y = 50 + okna.size() * 30;
                    novoeOkno.w = 200;
                    novoeOkno.h = 150;
                    novoeOkno.aktivno = false;
                    novoeOkno.zakryto = false;
                    novoeOkno.nazvanie = "Okno " + std::to_string(okna.size() + 1);
                    novoeOkno.peretaskivaetsya = false;
                    novoeOkno.otnosX = 0;
                    novoeOkno.otnosY = 0;
                    novoeOkno.vstryaskaTimer = 0;
                    novoeOkno.tekstVOkne = "Dvojnoj klik dlya vvoda";
                    novoeOkno.izmenenieRazmera = false;
                    novoeOkno.otnosW = 0;
                    novoeOkno.otnosH = 0;

                    okna.push_back(novoeOkno);

                    sf::Color sluchajnyjCvet(
                        std::rand() % 200 + 55,
                        std::rand() % 200 + 55,
                        std::rand() % 200 + 55
                    );
                    cveta.push_back(sluchajnyjCvet);
                }
            }

            // НАЖАТИЕ КНОПКИ МЫШИ
            if (sobytie.type == sf::Event::MouseButtonPressed)
            {
                // Сброс перед новым кликом
                indeksPeretaskivaemogoOkna = -1;
                indeksIzmeneniyaRazmera = -1;
                for (size_t j = 0; j < okna.size(); ++j)
                {
                    okna[j].peretaskivaetsya = false;
                    okna[j].izmenenieRazmera = false;
                }

                if (sobytie.mouseButton.button == sf::Mouse::Left)
                {
                    int myshX = sobytie.mouseButton.x;
                    int myshY = sobytie.mouseButton.y;

                    // Поиск окна, по которому кликнули (для встряски)
                    int indexKliknutogoOkna = -1;
                    for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
                    {
                        if (okna[i].zakryto) continue;

                        if (myshX >= okna[i].x && myshX <= okna[i].x + okna[i].w &&
                            myshY >= okna[i].y && myshY <= okna[i].y + okna[i].h)
                        {
                            indexKliknutogoOkna = i;
                            break;
                        }
                    }

                    // Обработка действий с самого верхнего окна
                    for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
                    {
                        if (okna[i].zakryto) continue;

                        int vysotaZagolovka = 25;
                        int knopkaW = 20;
                        int knopkaH = 18;
                        int knopkaX = okna[i].x + okna[i].w - knopkaW - 5;
                        int knopkaY = okna[i].y + 3;

                        // Зона изменения размера (правый нижний угол, 15x15 пикселей)
                        int zonaRazmeraW = 15;
                        int zonaRazmeraH = 15;
                        int zonaX = okna[i].x + okna[i].w - zonaRazmeraW;
                        int zonaY = okna[i].y + okna[i].h - zonaRazmeraH;

                        // 1. Проверка на изменение размера
                        if (myshX >= zonaX && myshX <= zonaX + zonaRazmeraW &&
                            myshY >= zonaY && myshY <= zonaY + zonaRazmeraH)
                        {
                            indeksIzmeneniyaRazmera = i;
                            okna[i].izmenenieRazmera = true;
                            okna[i].otnosW = okna[i].w - myshX;
                            okna[i].otnosH = okna[i].h - myshY;
                            break;
                        }

                        // 2. Проверка на кнопку закрытия
                        if (myshX >= knopkaX && myshX <= knopkaX + knopkaW &&
                            myshY >= knopkaY && myshY <= knopkaY + knopkaH)
                        {
                            okna[i].zakryto = true;
                            break;
                        }

                        // 3. Проверка заголовка на перетаскивание
                        if (myshX >= okna[i].x && myshX <= okna[i].x + okna[i].w &&
                            myshY >= okna[i].y && myshY <= okna[i].y + vysotaZagolovka)
                        {
                            // Поднимаем окно наверх
                            Okno vremennoe = okna[i];
                            okna.erase(okna.begin() + i);
                            okna.push_back(vremennoe);

                            sf::Color vremennyjCvet = cveta[i];
                            cveta.erase(cveta.begin() + i);
                            cveta.push_back(vremennyjCvet);

                            int novyjIndeks = static_cast<int>(okna.size()) - 1;

                            indeksPeretaskivaemogoOkna = novyjIndeks;
                            okna[novyjIndeks].peretaskivaetsya = true;
                            okna[novyjIndeks].otnosX = myshX - okna[novyjIndeks].x;
                            okna[novyjIndeks].otnosY = myshY - okna[novyjIndeks].y;

                            break;
                        }
                    }

                    // Встряска для кликнутого окна
                    if (indexKliknutogoOkna != -1 && !okna[indexKliknutogoOkna].zakryto)
                    {
                        int vysotaZagolovka = 25;
                        if (myshY >= okna[indexKliknutogoOkna].y + vysotaZagolovka &&
                            myshY <= okna[indexKliknutogoOkna].y + okna[indexKliknutogoOkna].h)
                        {
                            okna[indexKliknutogoOkna].vstryaskaTimer = 12;
                        }
                    }
                }
            }

            // ДВОЙНОЙ КЛИК МЫШИ (для ввода текста в тело окна)
            // Событие MouseButtonPressed счётчик кликов
            static int schetchikDvojnogo = 0;
            static sf::Clock chasyDvojnogo;
            static int poslednijIndeks = -1;

            if (sobytie.type == sf::Event::MouseButtonPressed && sobytie.mouseButton.button == sf::Mouse::Left)
            {
                int myshX = sobytie.mouseButton.x;
                int myshY = sobytie.mouseButton.y;

                // Находим окно, по которому кликнули
                int indeksDvojnogo = -1;
                for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
                {
                    if (okna[i].zakryto) continue;
                    if (myshX >= okna[i].x && myshX <= okna[i].x + okna[i].w &&
                        myshY >= okna[i].y && myshY <= okna[i].y + okna[i].h)
                    {
                        indeksDvojnogo = i;
                        break;
                    }
                }

                // Проверяем двойной клик
                if (indeksDvojnogo != -1 && indeksDvojnogo == poslednijIndeks && chasyDvojnogo.getElapsedTime().asMilliseconds() < 300)
                {
                    // Двойной клик! Проверяем, что кликнули по телу (не по заголовку)
                    int vysotaZagolovka = 25;
                    if (myshY >= okna[indeksDvojnogo].y + vysotaZagolovka &&
                        myshY <= okna[indeksDvojnogo].y + okna[indeksDvojnogo].h)
                    {
                        // Открываем консоль для ввода текста
                        std::string novyjTekst;
                        std::cout << "Vvedite tekst dlya okna " << okna[indeksDvojnogo].nazvanie << ": ";
                        std::getline(std::cin, novyjTekst);

                        if (!novyjTekst.empty())
                        {
                            okna[indeksDvojnogo].tekstVOkne = novyjTekst;
                        }
                    }

                    schetchikDvojnogo = 0;
                    poslednijIndeks = -1;
                }
                else
                {
                    schetchikDvojnogo = 1;
                    poslednijIndeks = indeksDvojnogo;
                    chasyDvojnogo.restart();
                }
            }

            // ОТПУСКАНИЕ КНОПКИ МЫШИ
            if (sobytie.type == sf::Event::MouseButtonReleased)
            {
                if (sobytie.mouseButton.button == sf::Mouse::Left)
                {
                    indeksPeretaskivaemogoOkna = -1;
                    indeksIzmeneniyaRazmera = -1;
                    for (size_t j = 0; j < okna.size(); ++j)
                    {
                        okna[j].peretaskivaetsya = false;
                        okna[j].izmenenieRazmera = false;
                    }
                }
            }

            // ДВИЖЕНИЕ МЫШИ
            if (sobytie.type == sf::Event::MouseMoved)
            {
                // Перетаскивание окна
                if (indeksPeretaskivaemogoOkna != -1 &&
                    indeksPeretaskivaemogoOkna < static_cast<int>(okna.size()))
                {
                    if (!okna[indeksPeretaskivaemogoOkna].zakryto)
                    {
                        int myshX = sobytie.mouseMove.x;
                        int myshY = sobytie.mouseMove.y;

                        okna[indeksPeretaskivaemogoOkna].x = myshX - okna[indeksPeretaskivaemogoOkna].otnosX;
                        okna[indeksPeretaskivaemogoOkna].y = myshY - okna[indeksPeretaskivaemogoOkna].otnosY;
                    }
                    else
                    {
                        indeksPeretaskivaemogoOkna = -1;
                    }
                }

                // Изменение размера окна
                if (indeksIzmeneniyaRazmera != -1 &&
                    indeksIzmeneniyaRazmera < static_cast<int>(okna.size()))
                {
                    if (!okna[indeksIzmeneniyaRazmera].zakryto)
                    {
                        int myshX = sobytie.mouseMove.x;
                        int myshY = sobytie.mouseMove.y;

                        int novayaShirina = myshX + okna[indeksIzmeneniyaRazmera].otnosW;
                        int novayaVysota = myshY + okna[indeksIzmeneniyaRazmera].otnosH;

                        if (novayaShirina >= 80) okna[indeksIzmeneniyaRazmera].w = novayaShirina;
                        if (novayaVysota >= 60) okna[indeksIzmeneniyaRazmera].h = novayaVysota;
                    }
                    else
                    {
                        indeksIzmeneniyaRazmera = -1;
                    }
                }
            }

            // Удаление закрытых окон
            for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
            {
                if (okna[i].zakryto)
                {
                    okna.erase(okna.begin() + i);
                    cveta.erase(cveta.begin() + i);
                }
            }
        }

        desktop.clear(sf::Color(50, 50, 80));

        // ОТРИСОВКА ВСЕХ ОКОН
        for (size_t i = 0; i < okna.size(); ++i)
        {
            if (!okna[i].zakryto)
            {
                // Встряска
                int sdvigX = 0;
                if (okna[i].vstryaskaTimer > 0)
                {
                    sdvigX = (okna[i].vstryaskaTimer % 2 == 0) ? 8 : -8;
                    okna[i].vstryaskaTimer--;
                }

                int tekX = okna[i].x + sdvigX;
                int tekY = okna[i].y;
                int vysotaZagolovka = 25;

                // Основной прямоугольник окна
                sf::RectangleShape prjamougolnik(sf::Vector2f(okna[i].w, okna[i].h));
                prjamougolnik.setPosition(tekX, tekY);
                prjamougolnik.setFillColor(cveta[i]);
                prjamougolnik.setOutlineThickness(2);
                prjamougolnik.setOutlineColor(sf::Color::White);
                desktop.draw(prjamougolnik);

                // Текст внутри окна
                sf::Text vnutrenniyTekst;
                vnutrenniyTekst.setFont(shrift);
                // Обрезаем длинный текст
                std::string dlyaOtobrazheniya = okna[i].tekstVOkne;
                if (dlyaOtobrazheniya.length() > 25)
                {
                    dlyaOtobrazheniya = dlyaOtobrazheniya.substr(0, 22) + "...";
                }
                vnutrenniyTekst.setString(dlyaOtobrazheniya);
                vnutrenniyTekst.setCharacterSize(12);
                vnutrenniyTekst.setFillColor(sf::Color::Black);
                vnutrenniyTekst.setPosition(tekX + 5, tekY + vysotaZagolovka + 5);
                desktop.draw(vnutrenniyTekst);

                // Заголовок
                sf::RectangleShape zagolovok(sf::Vector2f(okna[i].w, vysotaZagolovka));
                zagolovok.setPosition(tekX, tekY);
                zagolovok.setFillColor(sf::Color(80, 80, 120));
                desktop.draw(zagolovok);

                // Текст заголовка
                sf::Text tekst;
                tekst.setFont(shrift);
                tekst.setString(okna[i].nazvanie);
                tekst.setCharacterSize(14);
                tekst.setFillColor(sf::Color::White);
                tekst.setPosition(tekX + 5, tekY + 4);
                desktop.draw(tekst);

                // Кнопка закрытия
                int knopkaW = 20;
                int knopkaH = 18;
                sf::RectangleShape knopka(sf::Vector2f(knopkaW, knopkaH));
                knopka.setPosition(tekX + okna[i].w - knopkaW - 5, tekY + 3);
                knopka.setFillColor(sf::Color(180, 60, 60));
                knopka.setOutlineThickness(1);
                knopka.setOutlineColor(sf::Color::White);
                desktop.draw(knopka);

                // Текст [X] на кнопке
                sf::Text krestik;
                krestik.setFont(shrift);
                krestik.setString("X");
                krestik.setCharacterSize(14);
                krestik.setFillColor(sf::Color::White);
                krestik.setPosition(tekX + okna[i].w - knopkaW - 5 + 6, tekY + 4);
                desktop.draw(krestik);

                // Индикатор изменения размера (маленький треугольник в правом нижнем углу)
                sf::ConvexShape indikatorRazmera;
                indikatorRazmera.setPointCount(3);
                indikatorRazmera.setPoint(0, sf::Vector2f(tekX + okna[i].w - 12, tekY + okna[i].h - 2));
                indikatorRazmera.setPoint(1, sf::Vector2f(tekX + okna[i].w - 2, tekY + okna[i].h - 2));
                indikatorRazmera.setPoint(2, sf::Vector2f(tekX + okna[i].w - 2, tekY + okna[i].h - 12));
                indikatorRazmera.setFillColor(sf::Color(200, 200, 200, 180));
                desktop.draw(indikatorRazmera);
            }
        }

        desktop.display();
    }

    return 0;
}