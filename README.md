# Общие сведения
1. ФИО: Яковлев Эрик денисович
2. Группа: БПИ239
3. - Вариант: 4
   - Текст задачи: Неправильные пчелы, подсчитав в конце месяца убытки от наличия в лесу Винни Пуха, решили разыскать его и наказать в назидание всем другим любителям сладкого.
   Для поисков медведя они поделили лес на участки, каждый из которых прочесывает одна стая неправильных пчел. В случае нахождения медведя на своем участке стая проводит
   показательное наказание и возвращается в улей. Если участок прочесан, а Винни-Пух на нем не обнаружен, стая также возвращается в улей.
 Там она получает информацию об еще неисследованных участках и снова улетает. Требуется создать многопоточное приложение, моделирующее действия пчел.
 При решении использовать парадигму «портфель задач». Каждая стая пчел — отдельный поток.
4. Метод решения задачи:  
4.1. Сценарий: В задаче рои пчёл ищут Винни-Пуха, который спрятался в одной из секций леса. Каждый рой проверяет секции по очереди.
    Если один из роев находит Винни-Пуха, он сигнализирует остальным и завершает поиск.  
  4.2. Роли и объекты:  
    4.2.1. Рои пчёл: Многопоточные сущности, которые обрабатывают секции леса.  
    4.2.2. Секции леса: Очередь задач, по которым ведётся поиск.  
    4.2.3. Винни-Пух: Объект, спрятанный в одной из секций, которого нужно найти.  
4.3. Синхронизация: Мьютекс используется для защиты доступа к очереди задач, а условная переменная — для уведомления о новых задачах.  

# Отчет

## На 4-5 баллов
### Модель параллельных вычислений  
Используется POSIX Threads:
- Мьютекс (pthread_mutex_t) синхронизирует доступ к очереди.
- Условная переменная (pthread_cond_t) позволяет потокам ждать новых задач.  
Каждый поток обрабатывает секции из очереди и завершает работу при обнаружении Винни-Пуха.
### Входные данные
Ввод с консоли:
- Размер леса (количество секций).
- Количество роев пчёл.
- Номер секции, где спрятан Винни-Пух.  
Пример ввода:
```
What is the size of the forest? (enter a positive number):  10  
How many bee swarms are there? (enter a positive number): 3  
Where are you hiding? (choose a section between 1 and 10): 5
```
### Вывод программы
Вывод отображает ключевые события:
```
Bee swarm 1 is searching section 1
Bee swarm 1 did not find Winnie the Pooh in section 1 and returned to the hive
Bee swarm 3 is searching section 3
Bee swarm 3 did not find Winnie the Pooh in section 3 and returned to the hive
Bee swarm 2 is searching section 2
Bee swarm 2 did not find Winnie the Pooh in section 2 and returned to the hive
Bee swarm 1 is searching section 4
Bee swarm 1 did not find Winnie the Pooh in section 4 and returned to the hive
Bee swarm 2 is searching section 6
Bee swarm 2 did not find Winnie the Pooh in section 6 and returned to the hive
Bee swarm 3 is searching section 5
Bee swarm 3 found and punished Winnie the Pooh in section 5
Bee swarm 1 is searching section 7
Bee swarm 1 did not find Winnie the Pooh in section 7 and returned to the hive
Bee swarm 2 is searching section 8
Bee swarm 2 did not find Winnie the Pooh in section 8 and returned to the hive
All bee swarms have returned to the hive.
Winnie the Pooh was found by bee swarm 3 in section 5.
Bee swarm 1 is searching section 3
Bee swarm 2 found and punished Winnie the Pooh in section 5
All bee swarms have returned to the hive.
```
### Скриншоты начала и конца работы ввода с клавиатуры
<img width="671" alt="Снимок экрана 2024-12-17 в 23 47 44" src="https://github.com/user-attachments/assets/235fa83c-8719-4a31-977e-1621d46994fb" />
<img width="681" alt="Снимок экрана 2024-12-17 в 23 47 51" src="https://github.com/user-attachments/assets/e8569a1a-4819-4d98-b81e-da0ec7a2ecf1" />


## На 6-7 баллов
### Обобщённый алгоритм
Инициализация:
- Чтение данных из файла, консоли или генерация случайных значений.
- Заполнение очереди задач секциями леса.   
Создание потоков:
- Создаются потоки для каждого роя пчёл.   
Поиск:
- Поток проверяет секцию и сравнивает с местоположением Винни Пуха
- При обнаружении устанавливается флаг завершения.   
Завершение:
- Все потоки завершают работу, программа выводит результаты.   
  
### Генерация случайных данных
Генерация параметров:
```
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> sections(1, 10000);
std::uniform_int_distribution<> bees(1, 1000);
```
Пример случайного ввода:

```
Forest size: 2624
Number of bee swarms: 259
Winnie the Pooh is hiding in section: 2154
```
### Скриншоты начала и конца работы случайного ввода
<img width="712" alt="Снимок экрана 2024-12-17 в 23 46 25" src="https://github.com/user-attachments/assets/6d3e8dbc-7fef-4f03-bcb5-5c1ad6cfd9ee" />
<img width="685" alt="Снимок экрана 2024-12-17 в 23 46 39" src="https://github.com/user-attachments/assets/d40855ed-5970-4dd9-b51f-1544e89f319e" />


### Ввод данных из командной строки
Добавлена возможность ввода через аргументы командной строки:
```
./program --file input.txt
```
## На 8 баллов
### Вывод результатов в файл
Добавлен вывод результатов в файл. Имя файла задаётся через аргумент:
```
./program --file ../input/test.txt --output ../output/test_output.txt
```
Файл test_output.txt можно найти в папке output

### Скриншоты начала и конца работы ввода через файл test.txt
<img width="545" alt="Снимок экрана 2024-12-17 в 23 44 01" src="https://github.com/user-attachments/assets/13d54578-e1ea-4c56-99a6-b9688fe9e21b" />
<img width="705" alt="Снимок экрана 2024-12-17 в 23 44 13" src="https://github.com/user-attachments/assets/054e0d50-f0e1-4336-b1d2-9e6bbd923f2b" />

### Скриншоты начала и конца работы ввода через файл two_bees.txt
<img width="508" alt="Снимок экрана 2024-12-17 в 23 45 39" src="https://github.com/user-attachments/assets/1587d074-f8fc-4488-95a2-40be106f4db7" />
<img width="689" alt="Снимок экрана 2024-12-17 в 23 45 46" src="https://github.com/user-attachments/assets/d0e7dbd6-54c4-4cf6-80d6-3496e541ae5a" />

## На 9 баллов
Альтернативное решение (Solution_on_9.cpp):

### Синхронизация:
std::mutex защищает доступ к очереди задач.   
std::condition_variable уведомляет потоки о новых задачах.   
std::atomic<bool> используется для флагов завершения и нахождения Винни.   
### Реализация:
Потоки создаются с использованием std::thread, а не POSIX Threads.
```
std::vector<std::thread> threads;
for (int i = 0; i < num_bees; ++i) {
    threads.emplace_back(bee_task, i + 1);
}
```
### Сравнение с POSIX Threads:
Код становится более читабельным благодаря использованию высокоуровневых инструментов C++.   
Производительность аналогична при небольшом количестве потоков.

## На 10 баллов
### Реализация с использованием OpenMP
Добавлена версия с OpenMP, позволяющая распараллелить обработку секций.
```
#pragma omp parallel num_threads(num_bees)
    {
        int bee_id = omp_get_thread_num() + 1;
        std::ostringstream log;

        while (!winnie_found) {
            int task_id = -1;

            // Retrieve a task from the queue
            {
                std::lock_guard<std::mutex> lock(task_mutex);
                if (!task_queue.empty()) {
                    task_id = task_queue.front();
                    task_queue.pop();
                }
            }

            if (task_id != -1) {
                process_task(bee_id, task_id, log);
            } else {
                break; // Exit if there are no more tasks
            }
        }

#pragma omp critical
        {
            std::cout << log.str();
            if (write_to_file) {
                output_file << log.str();
            }
        }
    }
```
### Сравнение производительности
|Параметр|POSIX Threads|OpenMP|
|-------------|-------------|-------------|
|Простота реализации|Средняя|Высокая|
|Производительность|Высокая|Отличная|
|Количество строк кода|Больше|Меньше|
