# Оконный менеджер на C++ и SFML

## Описание проекта

Простой оконный менеджер, написанный на C++ с использованием библиотеки SFML.  
Позволяет создавать окна, перетаскивать их за заголовок, закрывать и наблюдать анимацию "встряски" при клике по телу окна.


---

## Структура проекта

```
OconnyMenedzher/
├── main.cpp          # Весь код проекта
├── arial.ttf         # Шрифт для заголовков
└── README.md         # Этот файл
```

---

## Код проекта

```
#include <SFML/Graphics.hpp> // подключение графической библиотеки SFML
#include <vector> // подключение библиотеки vector
#include <cstdlib> // генерация случайных чисел/цветов
#include <ctime>   // инициализации генератора случайных чисел
#include <string> 
#include <iostream>





struct Okno 
{
    int x, y; // координаты левого верхнего угла
    int w, h; // ширина и высота окна
    bool aktivno; // активно ли окно 
    bool zakryto; // помечено ли окно на закрытие
    std::string nazvanie; // имя окна 
    bool peretaskivaetsya; // перетаскивается ли окно сейчас
    int otnosX, otnosY; // смещение клика относительно угла окна
    int vstryaskaTimer = 0; // таймер для анимации встряски
};

int main()
{
    // Переменные для перетаскивания окон
    int indeksPeretaskivaemogoOkna = -1; // -1 - окно не перетаскивается

    // Главное окно приложения 
    sf::RenderWindow desktop(sf::VideoMode(1024, 768), "Oconnyj Menedzher");

    // Список всех управляемых окон
    std::vector<Okno> okna;

    // Цвета для каждого окна 
    std::vector<sf::Color> cveta;

    // Инициализация генератора случайных чисел для цветов окон
    std::srand(static_cast<unsigned>(std::time(nullptr)));


    // Загрузка шрифта для текста в заголовках окон
    sf::Font shrift;
    if (!shrift.loadFromFile("arial.ttf")) // пытаемся загрузить стандартный шрифт
    {
       
    }

    // Главный цикл приложения
    while (desktop.isOpen()) // открыто oкно
    {
        sf::Event sobytie; // хранение события

        // pollEvent - проверка и обработка событий, поступающих к окну приложения
        while (desktop.pollEvent(sobytie))
        {
            //закрытие окна
            if (sobytie.type == sf::Event::Closed)
            {
               desktop.close();
            }

            if (sobytie.type == sf::Event::KeyPressed)
            {
                // проверка на нажатие клавиши N
                if (sobytie.key.code == sf::Keyboard::N)
                {
                    // Создание нового окна
                    Okno novoeOkno;
                    novoeOkno.x = 50 + okna.size() * 30; // сдвиг по X
                    novoeOkno.y = 50 + okna.size() * 30; // сдвиг по Y
                    novoeOkno.w = 200; // ширина
                    novoeOkno.h = 150; // высота
                    novoeOkno.aktivno = false; // пока неактивно
                    novoeOkno.zakryto = false; // не закрыто
                    novoeOkno.nazvanie = "Okno " + std::to_string(okna.size() + 1); // название
                    novoeOkno.peretaskivaetsya = false; // изначально не перетаскивается
                    novoeOkno.otnosX = 0; // начальное смещение по X
                    novoeOkno.otnosY = 0; // начальное смещение по Y
                    novoeOkno.vstryaskaTimer = 0;

                    // Добавляем окно в список
                    okna.push_back(novoeOkno);

                    // создание случайного цвета
                    sf::Color sluchajnyjCvet(
                        std::rand() % 200 + 55, // красный 
                        std::rand() % 200 + 55, // зелёный
                        std::rand() % 200 + 55  // синий
                    );

                    cveta.push_back(sluchajnyjCvet);
                }
            }

            // MouseButtonPressed - обрабатывает нажатие кнопки мыши
            if (sobytie.type == sf::Event::MouseButtonPressed)
            {
                // Сброс нажания перед новым кликом
                indeksPeretaskivaemogoOkna = -1;
                for (size_t j = 0; j < okna.size(); ++j)
                {
                    okna[j].peretaskivaetsya = false;
                }

                if (sobytie.mouseButton.button == sf::Mouse::Left)
                {
                    int myshX = sobytie.mouseButton.x;
                    int myshY = sobytie.mouseButton.y;

                    // Поиск окна по какому кликнули для выдачи встряски
                    int indexKliknutogoOkna = -1;
                    for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
                    {
                        if (okna[i].zakryto) continue;

                        int vysotaZagolovka = 25;

                        // Проверяем, попадает ли клик в пределы окна (включая заголовок и тело)
                        if (myshX >= okna[i].x && myshX <= okna[i].x + okna[i].w && myshY >= okna[i].y && myshY <= okna[i].y + okna[i].h)
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

                        // Проверка на кнопку закрытия
                        if (myshX >= knopkaX && myshX <= knopkaX + knopkaW &&
                            myshY >= knopkaY && myshY <= knopkaY + knopkaH)
                        {
                            okna[i].zakryto = true;
                            break;
                        }

                        // Проверка заголовка на перетаскивание
                        if (myshX >= okna[i].x && myshX <= okna[i].x + okna[i].w &&
                            myshY >= okna[i].y && myshY <= okna[i].y + vysotaZagolovka)
                        {
                            // Поднимаем окно наверх
                            Okno vremennoe = okna[i];
                            okna.erase(okna.begin() + i);
                            okna.push_back(vremennoe);

                            // Обновка цветов
                            sf::Color vremennyjCvet = cveta[i];
                            cveta.erase(cveta.begin() + i);
                            cveta.push_back(vremennyjCvet);

                            // Новый индекс окна (последний в векторе)
                            int novyjIndeks = static_cast<int>(okna.size()) - 1;

                            // Активирование перетаскивания
                            indeksPeretaskivaemogoOkna = novyjIndeks;
                            okna[novyjIndeks].peretaskivaetsya = true;
                            okna[novyjIndeks].otnosX = myshX - okna[novyjIndeks].x;
                            okna[novyjIndeks].otnosY = myshY - okna[novyjIndeks].y;

                            break;
                        }
                    }

                    // Встряска: примение к окну, по которому  кликнули
                    if (indexKliknutogoOkna != -1 && !okna[indexKliknutogoOkna].zakryto)
                    {
                        int vysotaZagolovka = 25;
                        // Проверяем, что клик был именно по телу (не по заголовку)
                        if (myshY >= okna[indexKliknutogoOkna].y + vysotaZagolovka &&
                            myshY <= okna[indexKliknutogoOkna].y + okna[indexKliknutogoOkna].h)
                        {
                            okna[indexKliknutogoOkna].vstryaskaTimer = 12;
                        }
                    }
                }
            }

            // MouseButtonReleased - обрабатывает отпускание кнопки мыши
            if (sobytie.type == sf::Event::MouseButtonReleased)
            {
                if (sobytie.mouseButton.button == sf::Mouse::Left)
                {
                    indeksPeretaskivaemogoOkna = -1;
                    for (size_t j = 0; j < okna.size(); ++j)
                    {
                        okna[j].peretaskivaetsya = false;
                    }
                }
            }

            // MouseMoved - обрабатывает движение мыши
            if (sobytie.type == sf::Event::MouseMoved)
            {
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
            }

            // Удаление закрытых окон из списков
            for (int i = static_cast<int>(okna.size()) - 1; i >= 0; --i)
            {
                if (okna[i].zakryto)
                {
                    okna.erase(okna.begin() + i);
                    cveta.erase(cveta.begin() + i);
                }
            }



        }

      
        desktop.clear(sf::Color(50, 50, 80)); // тёмно-синий фон

        // Отрисовка всех окон
        for (size_t i = 0; i < okna.size(); ++i) // size_t - размер вектора
        {
            // Если окно не закрыто
            if (!okna[i].zakryto)
            {
                // ВСТРЯСКА: вычисляем сдвиг по X
                int sdvigX = 0;
                if (okna[i].vstryaskaTimer > 0)
                {
                    sdvigX = (okna[i].vstryaskaTimer % 2 == 0) ? 8 : -8; // сдвиг на 8 пикселей
                    okna[i].vstryaskaTimer -= 1;
                }

                // Координаты окна с учётом сдвига
                int tekX = okna[i].x + sdvigX;
                int tekY = okna[i].y;

                // Основной прямоугольник окна
                sf::RectangleShape prjamougolnik(sf::Vector2f(okna[i].w, okna[i].h));
                prjamougolnik.setPosition(tekX, tekY);
                prjamougolnik.setFillColor(cveta[i]);
                prjamougolnik.setOutlineThickness(2);
                prjamougolnik.setOutlineColor(sf::Color::White);
                desktop.draw(prjamougolnik);

                int vysotaZagolovka = 25;

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
            }
        }

        // отображаение на экране
        desktop.display();
    }

    return 0; 
}
```

---

## UML-диаграммы

### Диаграмма классов

```
┌─────────────────────────────┐
│           Okno              │
├─────────────────────────────┤
│ + x : int                   │
│ + y : int                   │
│ + w : int                   │
│ + h : int                   │
│ + aktivno : bool            │
│ + zakryto : bool            │
│ + nazvanie : string         │
│ + peretaskivaetsya : bool   │
│ + otnosX : int              │
│ + otnosY : int              │
│ + vstryaskaTimer : int      │
└─────────────────────────────┘
```

### Диаграмма последовательности (перетаскивание окна)

```
Пользователь → Приложение: Клик по заголовку
Приложение → Окно: Поднять наверх
Приложение → Окно: Начать перетаскивание
Пользователь → Приложение: Движение мыши
Приложение → Окно: Изменить координаты x, y
Пользователь → Приложение: Отпустить мышь
Приложение → Окно: Остановить перетаскивание
```

### Блок-схема главного цикла

```
[Старт]
    │
    ▼
[Обработка событий]
    │
    ├── Закрытие главного окна → [Выход]
    │
    ├── Нажатие N → [Создать окно]
    │
    ├── Клик мыши → [Закрыть / Перетащить / Встряхнуть]
    │
    ▼
[Отрисовка всех окон]
    │
    ▼
[Отображение на экране]
    │
    └──► [Назад к событиям]
```

### Диаграмма состояний окна

```
        [Создано]
            │
            ▼
      ┌─────────────┐
      │  Пассивное  │
      └─────────────┘
            │
            ├── Клик по заголовку → [Активное + перетаскивание]
            │
            ├── Клик по телу → [Встряска] ──► (возврат)
            │
            └── Клик по [X] → [Закрыто] ──► [Удалено]
```

---

### Техническое руководство для начинающих

### 1. Установка SFML

1. Скачайте SFML для Visual Studio 2022 с официального сайта
2. Распакуйте архив в `C:\Program Files\SFML`
3. В проекте VS2022 настройте:
   - Include Directories: `C:\Program Files\SFML\include`
   - Library Directories: `C:\Program Files\SFML\lib`
   - Additional Dependencies: `sfml-graphics-d.lib`, `sfml-window-d.lib`, `sfml-system-d.lib`

### 2. Создание окна приложения

```cpp
sf::RenderWindow desktop(sf::VideoMode(1024, 768), "Oconnyj Menedzher");
```

### 3. Структура окна

```cpp
struct Okno {
    int x, y;          // координаты
    int w, h;          // размеры
    bool zakryto;      // флаг закрытия
    std::string nazvanie;
    int vstryaskaTimer;
};
```

### 4. Обработка событий

```cpp
while (desktop.pollEvent(sobytie)) {
    if (sobytie.type == sf::Event::Closed)
        desktop.close();
    
    if (sobytie.type == sf::Event::KeyPressed && sobytie.key.code == sf::Keyboard::N)
        // создать новое окно
}
```

### 5. Отрисовка окна

```cpp
sf::RectangleShape prjamougolnik(sf::Vector2f(okna[i].w, okna[i].h));
prjamougolnik.setPosition(okna[i].x, okna[i].y);
desktop.draw(prjamougolnik);
```

### 6. Перетаскивание

При клике по заголовку запоминаем смещение мыши относительно угла окна:

```cpp
okna[i].otnosX = myshX - okna[i].x;
okna[i].otnosY = myshY - okna[i].y;
```

При движении мыши обновляем координаты:

```cpp
okna[indeks].x = myshX - okna[indeks].otnosX;
```

### 7. Модификация: встряска окна

При клике по телу окна запускается таймер:

```cpp
okna[i].vstryaskaTimer = 12;
```

При отрисовке окно сдвигается влево-вправо:

```cpp
if (okna[i].vstryaskaTimer > 0) {
    sdvigX = (okna[i].vstryaskaTimer % 2 == 0) ? 8 : -8;
    okna[i].vstryaskaTimer--;
}
```

---

## Управление

| Клавиша / Действие | Результат |
|-------------------|-----------|
| `N` | Создать новое окно |
| Клик по заголовку | Перетаскивание окна |
| Клик по телу окна | Анимация встряски |
| Клик по `[X]` | Закрыть окно |

---

## Сборка и запуск

1. Открыть проект в Visual Studio 2022
2. Убедиться, что DLL-файлы SFML скопированы в папку с `.exe`
3. Нажать `F5`

---

## Заключение

Проект демонстрирует базовые принципы работы графического интерфейса: обработку событий, отрисовку, перетаскивание объектов. Модификация (встряска) добавляет интерактивности и визуальной обратной связи.

```

