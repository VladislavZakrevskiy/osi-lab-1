# Лабораторная работа №1 - Управление процессами в ОС

## Задание (Вариант 2)

Пользователь вводит команды вида: «число число число `<endline>`». Далее эти числа передаются от родительского процесса в дочерний. Дочерний процесс считает их сумму и выводит её в файл. Числа имеют тип float. Количество чисел может быть произвольным.

### Сборка и запуск

```bash
make           # Сборка проекта
make run       # Запуск программы
make clean     # Очистка сгенерированных файлов
```

## Пример использования

```
Enter numbers separated by spaces (empty line to exit):
1.5 2.3 3.7
Sent 3 numbers to child process
Child process calculated sum: 7.50

10.0 -5.5 2.25 8.75
Sent 4 numbers to child process
Child process calculated sum: 15.50

[пустая строка для выхода]
Child process finished with exit code: 0
Results saved to result.txt
```

Результат в файле `result.txt`:

```
Numbers: 1.50 2.30 3.70 | Sum: 7.50
Numbers: 10.00 -5.50 2.25 8.75 | Sum: 15.50
```
