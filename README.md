# Elysium Launcher

**Elysium Launcher** — кроссплатформенное десктоп-приложение с фронтендом на `ReactJS` и бэкендом на `C++`, `Rust`.

## ⚙️ Установка зависимостей проекта

```bash
./launch.py install
```

- Устанавливает `npm` зависимости через NodeJS
- Устанавливает `Rust` зависимости через Cargo

## 🛠️ Сборка проекта

```bash
./launch.py build
```

- Очищает старую сборку
- Собирает фронтенд (`npm run build`)
- Копирует `dist/` в `build/`
- Собирает Rust библиотеки
- Генерирует заголовки
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
├── include/              # C headers, генерируемые из Rust
├── libs/
│   └── el-core-lib/      # Rust библиотека
├── src/                  # C++ исходники
├── manage.py             # Скрипт управления
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## ✅ TODO

- [X] Подключить ReactJS к проекту
- [ ] Добавить Windows-поддержку (через WebView2)
- [ ] CI/CD пайплайн (GitHub Actions)
- [ ] Написать библтиотеку для запуска и установки Minecraft
- [ ] Написаь фронтенд
