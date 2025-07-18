## Переводы:
- [日本語](README_JP.md) (Устарел)

# Elysium Launcher

**Elysium Launcher** — кроссплатформенное десктоп-приложение с фронтендом на `ReactJS` и бэкендом на `C++`, `Rust`.

## 🛠️ Подготовка к сборку

Для сборки требуются: cargo, npm, gcc/clang, cmake, python3

## 🧱 Скачивание репозитория

```bash
git clone --recurse-submodules https://github.com/FreedomDevs/elysium_launcher.git 
```

## ⚙️ Установка зависимостей проекта

```bash
./launch.py install
```

- Устанавливает Node.js зависимости (`npm install`)
- Устанавливает `cbindgen` через Cargo

## 🛠️ Сборка проекта

```bash
./launch.py build
```

- Очищает старую сборку
- Собирает фронтенд (`npm run build`)
- Копирует `dist/` в `build/`
- Собирает Rust библиотеки 
- Генерирует заголовки `Rust` библиотек
- Собирает C++ backend через `CMake`

## 🚀 Запуск

### Продакшен:

```bash
./launch.py run
```

Запускает скомпилированный лаунчер: `build/elysium_launcher`

### Режим разработки:

```bash
./launch.py dev
```

- Запускает `npm run dev` (фронт)
- Собирает C++ с `USE_BUILTIN_WEBSERVER=OFF`
- Запускает лаунчер, соединённый с дев-сервером

Остановка — через `Ctrl+C`, всё завершится корректно.

## 🧱 Структура проекта

```
.
├── app/                  # npm фронтенд
├── build/                # директория сборки
├── include/              # C/C++ headers
├── libs/
│   └── el-core-lib/      # Rust библиотека
├── src/                  # C++ исходники
├── manage.py             # Скрипт управления
├── CMakeLists.txt
└── README.md
```

## ✅ TODO

- [X] Подключить ReactJS к проекту
- [ ] Добавить Windows-поддержку (через WebView2)
- [ ] CI/CD пайплайн (GitHub Actions)
- [ ] Юнит-тесты для C++ компонентов
- [ ] Документация по API Rust-библиотеки
- [ ] Интернационализация интерфейса

## 🤝 Contributing

Pull-реквесты приветствуются! Чтобы внести вклад:

1. Форкни репозиторий
2. Создай новую ветку: `git checkout -b feature/ваша-фича`
3. Сделай коммиты и пуш: `git push origin feature/ваша-фича`
4. Создай Pull Request с описанием изменений

Пожалуйста, следуй существующему стилю кода и комментируй важные участки.
