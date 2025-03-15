#include "SlotMachine.h"
#include "../resource.h"
#include "stb_image.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <vector>
#include <windows.h>
#include <random>
#include <algorithm>

// Загрузка PNG
static bool LoadPngFromResource(int resourceId, std::vector<unsigned char>& data, int& w, int& h, int& c) {
    HRSRC res = FindResource(NULL, MAKEINTRESOURCE(resourceId), L"PNG");
    if (!res) return false;
    HGLOBAL resHandle = LoadResource(NULL, res);
    if (!resHandle) return false;
    void* resData = LockResource(resHandle);
    if (!resData) return false;
    DWORD resSize = SizeofResource(NULL, res);
    if (resSize == 0) return false;

    int outW, outH, outC;
    unsigned char* img = stbi_load_from_memory((stbi_uc*)resData, resSize, &outW, &outH, &outC, 4);
    if (!img) return false;

    data.assign(img, img + (outW * outH * 4));
    w = outW; h = outH; c = 4;
    stbi_image_free(img);
    return true;
}

SlotMachine::SlotMachine() {
    std::cout << "[SlotMachine] Constructor\n";
    InitializeReels();
    LoadResources();
}

SlotMachine::~SlotMachine() {
    std::cout << "[SlotMachine] Destructor\n";
    for (auto& kv : m_symbolTextures) {
        glDeleteTextures(1, &kv.second.texId);
    }
    glDeleteTextures(1, &m_goButtonTexture.texId);
    glDeleteTextures(1, &m_stopButtonTexture.texId);
}

void SlotMachine::GenerateSymbolStrip(std::vector<Symbol>& strip) {

    std::vector<Symbol> base = { Symbol::Strawberry, Symbol::Grapes, Symbol::Pineapple, Symbol::Lemon };

    strip.clear();
    strip.reserve(STRIP_LENGTH);
 
    for (int i = 0; i < 5; ++i) {
        for (auto s : base) strip.push_back(s);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(strip.begin(), strip.end(), g);
}

void SlotMachine::InitializeReels() {
    m_reels.resize(REEL_COUNT);
    for (int i = 0; i < REEL_COUNT; ++i) {
        GenerateSymbolStrip(m_reels[i].symbols);
        m_reels[i].rotation = 0.0f;
        m_reels[i].speed = 0.0f;
        m_reels[i].spinning = false;
        m_reels[i].elapsedTime = 0.0f;
        m_reels[i].accelerating = true;
        m_reels[i].decelerating = false;

        if (i == 0) { m_reels[i].maxSpeed = 800.0f; m_reels[i].startDelay = 0.0f; }
        if (i == 1) { m_reels[i].maxSpeed = 850.0f; m_reels[i].startDelay = 0.2f; }
        if (i == 2) { m_reels[i].maxSpeed = 780.0f; m_reels[i].startDelay = 0.4f; }
        if (i == 3) { m_reels[i].maxSpeed = 820.0f; m_reels[i].startDelay = 0.6f; }
        m_reels[i].started = false;
    }
}

void SlotMachine::LoadResources() {
    auto loadTex = [&](int id, TextureInfo& info) {
        std::vector<unsigned char> data;
        int w, h, c;
        if (!LoadPngFromResource(id, data, w, h, c)) {
            throw std::runtime_error("Failed to load texture ID:" + std::to_string(id));
        }
        glGenTextures(1, &info.texId);
        glBindTexture(GL_TEXTURE_2D, info.texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        info.width = w;
        info.height = h;
        };

    loadTex(IDB_PINEAPPLE, m_symbolTextures[Symbol::Pineapple]);
    loadTex(IDB_STRAWBERRY, m_symbolTextures[Symbol::Strawberry]);
    loadTex(IDB_GRAPES, m_symbolTextures[Symbol::Grapes]);
    loadTex(IDB_LEMON, m_symbolTextures[Symbol::Lemon]);

    loadTex(IDB_GO, m_goButtonTexture);
    loadTex(IDB_STOP, m_stopButtonTexture);
}

void SlotMachine::DrawTexture(GLuint texId, int x, int y, int width, int height) {
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texId);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + width, y);
    glTexCoord2f(1, 1); glVertex2i(x + width, y + height);
    glTexCoord2f(0, 1); glVertex2i(x, y + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

int SlotMachine::GetWidth() const { return m_windowWidth; }
int SlotMachine::GetHeight() const { return m_windowHeight; }

bool SlotMachine::CheckJackpot() {
    Symbol s0 = GetSymbolAtCentralLine(0);
    for (int i = 1; i < REEL_COUNT; ++i) {
        if (GetSymbolAtCentralLine(i) != s0) return false;
    }
    return true;
}

Symbol SlotMachine::GetSymbolAtCentralLine(int reelIndex) {

    Reel& r = m_reels[reelIndex];
    int totalSymbols = (int)r.symbols.size();

    int baseIndex = (int)std::floor(r.rotation / (float)SYMBOL_SIZE) % totalSymbols;
    if (baseIndex < 0) baseIndex += totalSymbols;

    int symbolIndex = (baseIndex + 1) % totalSymbols;
    return r.symbols[symbolIndex];
}

void SlotMachine::ShowStartButton(bool show) {
    m_showStartButton = show;
}
void SlotMachine::ShowStopButton(bool show) {
    m_showStopButton = show;
}

ButtonType SlotMachine::CheckButtonClick(int x, int y) {
    if (m_showStartButton) {
        if (x >= m_startButtonRect.x && x < (m_startButtonRect.x + m_startButtonRect.width) &&
            y >= m_startButtonRect.y && y < (m_startButtonRect.y + m_startButtonRect.height))
            return ButtonType::Start;
    }
    if (m_showStopButton) {
        if (x >= m_stopButtonRect.x && x < (m_stopButtonRect.x + m_stopButtonRect.width) &&
            y >= m_stopButtonRect.y && y < (m_stopButtonRect.y + m_stopButtonRect.height))
            return ButtonType::Stop;
    }
    return ButtonType::None;
}

void SlotMachine::OnResize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    UpdateButtonPositions();
}

void SlotMachine::UpdateButtonPositions() {
    m_startButtonRect.width = m_goButtonTexture.width;
    m_startButtonRect.height = m_goButtonTexture.height;
    m_startButtonRect.x = 10;
    m_startButtonRect.y = m_windowHeight - m_startButtonRect.height - 10;

    m_stopButtonRect.width = m_stopButtonTexture.width;
    m_stopButtonRect.height = m_stopButtonTexture.height;
    m_stopButtonRect.x = m_windowWidth - m_stopButtonRect.width - 10;
    m_stopButtonRect.y = m_windowHeight - m_stopButtonRect.height - 10;
}

void SlotMachine::StartSpinning() {
    std::cout << "[SlotMachine] StartSpinning\n";
    m_shouldStop = false;

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(4000, 12000);

    for (int i = 0; i < REEL_COUNT; ++i) {
        Reel& r = m_reels[i];
        r.spinning = false;
        r.started = false;
        r.accelerating = true;
        r.decelerating = false;
        r.elapsedTime = 0.0f;
        r.speed = 0.0f;
        r.targetRotation = r.rotation + dist(g);
    }

}

void SlotMachine::StopSpinning() {
    std::cout << "[SlotMachine] StopSpinning\n";
    m_shouldStop = true;
}

bool SlotMachine::IsSpinning() const {
    for (auto& r : m_reels) {
        if (r.spinning || !r.started)
            return true;
    }
    return false;
}

bool SlotMachine::IsStopped() const {
    return !IsSpinning();
}

void SlotMachine::Update(float dt) {
    UpdateAnimation(dt);
}

void SlotMachine::Render() {
    // Устанавливаем область отображения (весь экран)
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Устанавливаем серый фон
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Устанавливаем ортографическую проекцию для 2D отрисовки
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_windowWidth, m_windowHeight, 0, -1, 1);

    // Переходим в модельное пространство для отрисовки объектов
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Отрисовка кнопки "Старт", если она активна
    if (m_showStartButton) {
        DrawTexture(m_goButtonTexture.texId, 10, m_windowHeight - m_goButtonTexture.height - 10,
            m_goButtonTexture.width, m_goButtonTexture.height);
    }

    // Отрисовка кнопки "Стоп", если она активна
    if (m_showStopButton) {
        DrawTexture(m_stopButtonTexture.texId, m_windowWidth - m_stopButtonTexture.width - 10,
            m_windowHeight - m_stopButtonTexture.height - 10,
            m_stopButtonTexture.width, m_stopButtonTexture.height);
    }

    // Рассчитываем размеры и позиции барабанов
    int symbolSize = SYMBOL_SIZE;                      // Размер одного символа
    int totalWidth = REEL_COUNT * symbolSize;          // Общая ширина всех барабанов
    int totalHeight = VISIBLE_SYMBOLS * symbolSize;    // Высота области с видимыми символами
    int startX = (m_windowWidth - totalWidth) / 2;     // Координата X начала отрисовки
    int startY = (m_windowHeight - totalHeight) / 2;   // Координата Y начала отрисовки

    // Отрисовка каждого барабана
    for (int reelIndex = 0; reelIndex < REEL_COUNT; ++reelIndex) {
        Reel& r = m_reels[reelIndex];                 // Текущий барабан
        int totalSymbols = (int)r.symbols.size();     // Общее количество символов в ленте

        // Рассчитываем текущую позицию в ленте
        float rot = r.rotation / (float)symbolSize;   // Позиция в символах (с дробной частью)
        int baseIndex = (int)std::floor(rot) % totalSymbols; // Индекс текущего символа
        if (baseIndex < 0) baseIndex += totalSymbols; // Учитываем отрицательные индексы

        float fraction = rot - std::floor(rot);       // Дробная часть для смещения
        float offsetY = -fraction * symbolSize;       // Смещение символов по оси Y

        // Отрисовываем видимые символы
        for (int lineIndex = 0; lineIndex < VISIBLE_SYMBOLS; ++lineIndex) {
            int symbolIndex = (baseIndex + lineIndex) % totalSymbols; // Индекс символа для отрисовки
            Symbol sym = r.symbols[symbolIndex];      // Символ для отображения

            int x = startX + reelIndex * symbolSize;  // Позиция по X
            int y = startY + (lineIndex * symbolSize) + (int)offsetY; // Позиция по Y с учетом смещения

            auto& texInfo = m_symbolTextures[sym];    // Текстура текущего символа
            DrawTexture(texInfo.texId, x, y, symbolSize, symbolSize); // Отрисовка символа
        }
    }

    // Отрисовка центральной линии (выделяет линию выигрыша)
    int centralLineY = startY + symbolSize;           // Координата Y центральной линии
    glEnable(GL_BLEND);                               // Включаем альфа-прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Устанавливаем режим смешивания
    glColor4f(1.0f, 0.0f, 0.0f, 0.3f);               // Полупрозрачный красный цвет
    glBegin(GL_QUADS);                                // Рисуем прямоугольник
    glVertex2i(startX, centralLineY);
    glVertex2i(startX + totalWidth, centralLineY);
    glVertex2i(startX + totalWidth, centralLineY + symbolSize);
    glVertex2i(startX, centralLineY + symbolSize);
    glEnd();
}

float SlotMachine::CalculateSpeed(Reel& r, float dt) {
    // Если барабан еще не начал вращение, скорость равна 0
    if (!r.started) return 0.0f;

    // Ускорение барабана
    if (r.accelerating && !r.decelerating) {
        float t = r.elapsedTime / m_accelerationTime; // Нормализованное время ускорения
        if (t >= 1.0f) {
            r.accelerating = false;                  // Останавливаем ускорение
            return r.maxSpeed;                       // Возвращаем максимальную скорость
        }
        return r.maxSpeed * (t * t);                 // Квадратичная зависимость скорости
    }

    // Постоянная скорость (максимальная)
    if (!r.accelerating && !r.decelerating) {
        return r.maxSpeed;
    }

    // Замедление барабана
    if (r.decelerating) {
        float t = r.elapsedTime / m_decelerationTime; // Нормализованное время замедления
        if (t >= 1.0f) {
            return 0.0f;                             // Скорость равна 0, когда замедление завершено
        }
        return r.maxSpeed * (1 - t) * (1 - t);       // Квадратичная зависимость скорости
    }

    return 0.0f; // На всякий случай возвращаем 0
}

void SlotMachine::AlignRotation(Reel& r) {
    // Округляем текущую позицию вращения до ближайшего символа
    int symbolSize = SYMBOL_SIZE;
    float symbolsPassed = r.rotation / symbolSize;   // Количество пройденных символов
    float nearest = std::round(symbolsPassed);       // Ближайший целый символ
    r.rotation = nearest * symbolSize;               // Выравниваем rotation
}

void SlotMachine::UpdateAnimation(float dt) {
    for (int i = 0; i < REEL_COUNT; ++i) {
        Reel& r = m_reels[i];

        // Задержка перед стартом барабана
        if (!r.started) {
            r.startDelay -= dt;
            if (r.startDelay <= 0.0f) {
                r.started = true;                    // Барабан начинает вращаться
                r.spinning = true;
                r.accelerating = true;
                r.decelerating = false;
                r.elapsedTime = 0.0f;                // Сбрасываем счетчик времени
            }
            else {
                continue;                            // Переходим к следующему барабану
            }
        }

        if (r.spinning) {
            r.elapsedTime += dt;                    // Увеличиваем время вращения

            // Начинаем замедление, если запрошено
            if (m_shouldStop && !r.decelerating) {
                r.decelerating = true;
                r.accelerating = false;
                r.elapsedTime = 0.0f;
            }

            r.speed = CalculateSpeed(r, dt);        // Рассчитываем текущую скорость
            r.rotation += r.speed * dt;             // Обновляем позицию вращения

            // Если замедление завершено, выравниваем барабан
            if (r.decelerating && r.speed <= 0.001f) {
                r.spinning = false;
                r.speed = 0.0f;
                AlignRotation(r);                   // Выравниваем позицию
            }
        }
    }
}
